#!/usr/bin/env python3
# Scratch-tree-only (ps2xIOP): HLE "libsd" IRX import library for emulated modules.
# There is no SPU2 in this runtime, so this models libsd's *programming interface* faithfully
# (parameter/switch/address/core-attribute shadows, batch processing, pitch math, transfer requests
# that complete and raise the registered transfer-interrupt handler, loop-mode block transfers
# that keep ticking) without producing sound. Logged once per unimplemented ordinal.
import os
import shutil
import sys

IOP = "/tmp/ps2recomp_build/PS2Recomp/ps2xIOP"


def backup(path):
    if os.path.exists(path) and not os.path.exists(path + ".orig"):
        shutil.copyfile(path, path + ".orig")


def rep(path, old, new):
    s = open(path).read()
    n = s.count(old)
    if n != 1:
        print(f"FAIL {path}: expected 1 match, found {n} for:\n{old[:160]}")
        sys.exit(1)
    open(path, "w").write(s.replace(old, new))
    print("patched", path)


HEADER = r'''#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace ps2x::iop
{
    class IopHost;
}

namespace ps2x::iop::detail
{
    struct IopCpuState;
    class IopMemory;

    // HLE of the libsd IRX export library (SPU2 driver API) for modules running on the R3000 core.
    // No SPU2 exists in this runtime: register/parameter state is shadowed, transfers complete
    // immediately and raise the registered transfer-interrupt handler, loop-mode block transfers
    // keep raising it at the audio rate. Sound is not produced.
    class IopLibsd
    {
    public:
        struct ScheduledCallback
        {
            uint64_t dueCycle = 0u;
            uint32_t function = 0u;
            uint32_t gp = 0u;
            uint32_t a0 = 0u;
            uint32_t a1 = 0u;
            uint32_t channel = 0u;
            uint32_t generation = 0u;
            uint64_t repeatCycles = 0u;
        };

        IopLibsd(IopHost &host, IopMemory &memory);
        ~IopLibsd();
        IopLibsd(const IopLibsd &) = delete;
        IopLibsd &operator=(const IopLibsd &) = delete;

        void reset() noexcept;
        [[nodiscard]] bool dispatchImport(uint16_t ordinal, IopCpuState &cpu, uint64_t currentCycle);
        [[nodiscard]] std::vector<ScheduledCallback> takeScheduledCallbacks() noexcept;
        // True while a loop-mode block transfer with this generation is still running on the channel.
        [[nodiscard]] bool loopActive(uint32_t channel, uint32_t generation) const noexcept;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
'''

SOURCE = r'''#include "iop_libsd.h"

#include "../core/iop_cpu.h"
#include "../core/iop_memory.h"
#include "ps2x/iop/iop_host.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

namespace ps2x::iop::detail
{
    namespace
    {
        // IOP clock 36.864 MHz; SPU2 consumes 48 kHz * 2 ch * 16-bit = 192000 B/s per core stream,
        // so one byte of a loop buffer lasts 192 IOP cycles; DMA itself is modelled at ~24 cycles/byte.
        constexpr uint64_t kCyclesPerStreamedByte = 192u;
        constexpr uint64_t kCyclesPerDmaByte = 24u;
        constexpr uint64_t kMinTransferCycles = 2000u;
        constexpr uint32_t kChannels = 2u;

        constexpr uint32_t kBlockTransWrite = 0u;
        constexpr uint32_t kBlockTransRead = 1u;
        constexpr uint32_t kBlockTransStop = 2u;
        constexpr uint32_t kBlockTransWriteFrom = 3u;
        constexpr uint32_t kBlockTransLoop = 0x10u;

        constexpr uint16_t kBatchSetParam = 0x1u;
        constexpr uint16_t kBatchSetSwitch = 0x2u;
        constexpr uint16_t kBatchSetAddr = 0x3u;
        constexpr uint16_t kBatchSetCore = 0x4u;
        constexpr uint16_t kBatchWriteIop = 0x5u;
        constexpr uint16_t kBatchWriteEe = 0x6u;
        constexpr uint16_t kBatchEeReturn = 0x7u;
        constexpr uint16_t kBatchGetParam = 0x10u;
        constexpr uint16_t kBatchGetSwitch = 0x12u;
        constexpr uint16_t kBatchGetAddr = 0x13u;
        constexpr uint16_t kBatchGetCore = 0x14u;
    }

    class IopLibsd::Impl
    {
    public:
        Impl(IopHost &hostRef, IopMemory &memoryRef) : host(hostRef), memory(memoryRef) {}

        struct Transfer
        {
            bool looping = false;
            uint32_t generation = 0u;
            uint32_t iopAddress = 0u;
            uint32_t size = 0u;
            uint64_t startCycle = 0u;
        };
        struct Handler
        {
            uint32_t function = 0u;
            uint32_t argument = 0u;
            uint32_t gp = 0u;
        };

        IopHost &host;
        IopMemory &memory;
        std::unordered_map<uint32_t, uint32_t> params;
        std::unordered_map<uint32_t, uint32_t> switches;
        std::unordered_map<uint32_t, uint32_t> addresses;
        std::unordered_map<uint32_t, uint32_t> coreAttrs;
        std::array<Handler, kChannels> transferHandlers{};
        Handler spu2Handler{};
        std::array<uint32_t, kChannels> transferCallbacks{};
        uint32_t irqCallback = 0u;
        std::array<Transfer, kChannels> transfers{};
        std::vector<ScheduledCallback> scheduled;
        std::set<uint16_t> loggedUnhandled;
        bool initialized = false;

        void reset()
        {
            params.clear();
            switches.clear();
            addresses.clear();
            coreAttrs.clear();
            transferHandlers = {};
            spu2Handler = {};
            transferCallbacks = {};
            irqCallback = 0u;
            transfers = {};
            scheduled.clear();
            initialized = false;
        }

        static uint32_t lookup(const std::unordered_map<uint32_t, uint32_t> &table, uint32_t key)
        {
            const auto it = table.find(key);
            return it == table.end() ? 0u : it->second;
        }

        static int32_t note2pitch(uint32_t centerNote, uint32_t centerFine, uint32_t note, uint32_t fine)
        {
            const double semis = (static_cast<double>(static_cast<int32_t>(note) - static_cast<int32_t>(centerNote)) * 128.0 +
                                  (static_cast<double>(static_cast<int32_t>(fine) - static_cast<int32_t>(centerFine)))) /
                                 1536.0;
            const double pitch = 4096.0 * std::pow(2.0, semis);
            return static_cast<int32_t>(std::clamp(pitch, 0.0, 16383.0));
        }

        static int32_t pitch2note(uint32_t centerNote, uint32_t centerFine, uint32_t pitch)
        {
            if (pitch == 0u)
                return 0;
            const double units = 1536.0 * std::log2(static_cast<double>(pitch) / 4096.0) +
                                 static_cast<double>(centerNote) * 128.0 + static_cast<double>(centerFine);
            const int32_t rounded = static_cast<int32_t>(std::lround(units));
            const int32_t note = rounded / 128;
            const int32_t fineValue = rounded % 128;
            return (note << 8) | (fineValue & 0x7F);
        }

        void completeTransfer(uint32_t channel, uint32_t size, bool loop, uint64_t currentCycle)
        {
            Transfer &transfer = transfers[channel];
            ++transfer.generation;
            transfer.looping = loop;
            transfer.size = size;
            transfer.startCycle = currentCycle;
            const Handler &handler = transferHandlers[channel];
            if (handler.function == 0u)
                return;
            ScheduledCallback callback;
            callback.function = handler.function;
            callback.gp = handler.gp;
            callback.a0 = channel;
            callback.a1 = handler.argument;
            callback.channel = channel;
            callback.generation = transfer.generation;
            if (loop)
            {
                // Loop mode raises the handler at every half-buffer boundary for as long as it runs.
                const uint64_t half = std::max<uint64_t>(1u, size / 2u);
                callback.repeatCycles = std::max<uint64_t>(kMinTransferCycles, half * kCyclesPerStreamedByte);
                callback.dueCycle = currentCycle + callback.repeatCycles;
            }
            else
            {
                callback.dueCycle = currentCycle + std::max<uint64_t>(kMinTransferCycles, static_cast<uint64_t>(size) * kCyclesPerDmaByte);
            }
            scheduled.push_back(callback);
        }

        void stopTransfer(uint32_t channel)
        {
            Transfer &transfer = transfers[channel];
            transfer.looping = false;
            ++transfer.generation;
        }

        bool dispatch(uint16_t ordinal, IopCpuState &cpu, uint64_t currentCycle)
        {
            const uint32_t a0 = cpu.gpr[4];
            const uint32_t a1 = cpu.gpr[5];
            const uint32_t a2 = cpu.gpr[6];
            const uint32_t a3 = cpu.gpr[7];
            const uint32_t sp = cpu.gpr[29];
            const uint32_t gp = cpu.gpr[28];
            auto setV0 = [&](uint32_t value) { cpu.gpr[2] = value; };
            auto stackArg = [&](uint32_t index) { return memory.read32(sp + 16u + (index - 4u) * 4u); };

            switch (ordinal)
            {
            case 2: // sceSdQuit
                reset();
                setV0(0u);
                return true;
            case 4: // sceSdInit(flag)
                initialized = true;
                for (uint32_t channel = 0; channel < kChannels; ++channel)
                    stopTransfer(channel);
                setV0(0u);
                return true;
            case 5: // sceSdSetParam(entry, value)
                params[a0 & 0xFFFFu] = a1 & 0xFFFFu;
                setV0(0u);
                return true;
            case 6: // sceSdGetParam(entry)
                setV0(lookup(params, a0 & 0xFFFFu));
                return true;
            case 7: // sceSdSetSwitch(entry, value)
                switches[a0 & 0xFFFFu] = a1;
                setV0(0u);
                return true;
            case 8: // sceSdGetSwitch(entry)
                setV0(lookup(switches, a0 & 0xFFFFu));
                return true;
            case 9: // sceSdSetAddr(entry, value)
                addresses[a0 & 0xFFFFu] = a1;
                setV0(0u);
                return true;
            case 10: // sceSdGetAddr(entry)
                setV0(lookup(addresses, a0 & 0xFFFFu));
                return true;
            case 11: // sceSdSetCoreAttr(entry, value)
                coreAttrs[a0 & 0xFFFFu] = a1 & 0xFFFFu;
                setV0(0u);
                return true;
            case 12: // sceSdGetCoreAttr(entry)
                setV0(lookup(coreAttrs, a0 & 0xFFFFu));
                return true;
            case 13: // sceSdNote2Pitch(center_note, center_fine, note, fine)
                setV0(static_cast<uint32_t>(note2pitch(a0 & 0xFFFFu, a1 & 0xFFFFu, a2 & 0xFFFFu, a3 & 0xFFFFu)));
                return true;
            case 14: // sceSdPitch2Note(center_note, center_fine, pitch)
                setV0(static_cast<uint32_t>(pitch2note(a0 & 0xFFFFu, a1 & 0xFFFFu, a2 & 0xFFFFu)));
                return true;
            case 15: // sceSdProcBatch(batch, rets, num)
            case 16: // sceSdProcBatchEx(batch, rets, num, voice)
            {
                const uint32_t batch = a0;
                const uint32_t rets = a1;
                const uint32_t count = std::min<uint32_t>(a2, 4096u);
                for (uint32_t i = 0; i < count; ++i)
                {
                    const uint32_t entryAddress = batch + i * 8u;
                    const uint16_t func = memory.read16(entryAddress);
                    const uint16_t entry = memory.read16(entryAddress + 2u);
                    const uint32_t value = memory.read32(entryAddress + 4u);
                    uint32_t result = 0u;
                    switch (func)
                    {
                    case kBatchSetParam: params[entry] = value & 0xFFFFu; break;
                    case kBatchSetSwitch: switches[entry] = value; break;
                    case kBatchSetAddr: addresses[entry] = value; break;
                    case kBatchSetCore: coreAttrs[entry] = value & 0xFFFFu; break;
                    case kBatchGetParam: result = lookup(params, entry); break;
                    case kBatchGetSwitch: result = lookup(switches, entry); break;
                    case kBatchGetAddr: result = lookup(addresses, entry); break;
                    case kBatchGetCore: result = lookup(coreAttrs, entry); break;
                    case kBatchWriteIop: memory.write32(value, entry); break;
                    case kBatchWriteEe:
                    case kBatchEeReturn:
                    default: break;
                    }
                    if (rets != 0u)
                        memory.write32(rets + i * 4u, result);
                }
                setV0(0u);
                return true;
            }
            case 17: // sceSdVoiceTrans(channel, mode, iopaddr, spuaddr, size)
            {
                const uint32_t channel = a0 & 1u;
                const uint32_t size = stackArg(4);
                transfers[channel].iopAddress = a2;
                completeTransfer(channel, size, false, currentCycle);
                setV0(size);
                return true;
            }
            case 18: // sceSdBlockTrans(channel, mode, iopaddr, size, startaddr)
            {
                const uint32_t channel = a0 & 1u;
                const uint32_t mode = a1;
                const uint32_t size = a3;
                if ((mode & 0xFu) == kBlockTransStop)
                {
                    stopTransfer(channel);
                    setV0(0u);
                    return true;
                }
                transfers[channel].iopAddress = a2;
                completeTransfer(channel, size, (mode & kBlockTransLoop) != 0u, currentCycle);
                setV0(size);
                return true;
            }
            case 19: // sceSdVoiceTransStatus(channel, flag)
                setV0(1u);
                return true;
            case 20: // sceSdBlockTransStatus(channel, flag)
            {
                // Returns the IOP address the hardware is currently streaming from.
                const Transfer &transfer = transfers[a0 & 1u];
                uint32_t position = transfer.iopAddress;
                if (transfer.looping && transfer.size != 0u)
                {
                    const uint64_t elapsed = currentCycle > transfer.startCycle ? currentCycle - transfer.startCycle : 0u;
                    position += static_cast<uint32_t>((elapsed / kCyclesPerStreamedByte) % transfer.size) & ~3u;
                }
                setV0(position);
                return true;
            }
            case 21: // sceSdSetTransCallback(core, callback)
            {
                const uint32_t previous = transferCallbacks[a0 & 1u];
                transferCallbacks[a0 & 1u] = a1;
                setV0(previous);
                return true;
            }
            case 22: // sceSdSetIRQCallback(callback)
            {
                const uint32_t previous = irqCallback;
                irqCallback = a0;
                setV0(previous);
                return true;
            }
            case 23: // sceSdSetEffectAttr(core, attr)
            case 25: // sceSdClearEffectWorkArea(core, channel, effect_mode)
            case 31: // sceSdCleanEffectWorkArea
            case 32: // sceSdSetEffectMode
            case 33: // sceSdSetEffectModeParams
                setV0(0u);
                return true;
            case 24: // sceSdGetEffectAttr(core, attr*)
                if (a1 != 0u)
                    for (uint32_t i = 0; i < 6u; ++i)
                        memory.write32(a1 + i * 4u, 0u);
                setV0(0u);
                return true;
            case 26: // sceSdSetTransIntrHandler(channel, handler, arg)
            {
                Handler &handler = transferHandlers[a0 & 1u];
                const uint32_t previous = handler.function;
                handler = Handler{a1, a2, gp};
                setV0(previous);
                return true;
            }
            case 27: // sceSdSetSpu2IntrHandler(handler, arg)
            {
                const uint32_t previous = spu2Handler.function;
                spu2Handler = Handler{a0, a1, gp};
                setV0(previous);
                return true;
            }
            case 28: // sceSdGetTransIntrHandlerArgument(channel)
                setV0(transferHandlers[a0 & 1u].argument);
                return true;
            case 29: // sceSdGetSpu2IntrHandlerArgument
                setV0(spu2Handler.argument);
                return true;
            case 30: // sceSdStopTrans(channel)
                stopTransfer(a0 & 1u);
                setV0(0u);
                return true;
            default:
                if (loggedUnhandled.insert(ordinal).second)
                {
                    std::ostringstream out;
                    out << "[IOP libsd-hle] unimplemented libsd ordinal " << ordinal << " -> 0";
                    host.log(LogLevel::Warning, out.str());
                }
                setV0(0u);
                return true;
            }
        }
    };

    IopLibsd::IopLibsd(IopHost &host, IopMemory &memory) : m_impl(std::make_unique<Impl>(host, memory)) {}
    IopLibsd::~IopLibsd() = default;

    void IopLibsd::reset() noexcept
    {
        m_impl->reset();
    }

    bool IopLibsd::dispatchImport(uint16_t ordinal, IopCpuState &cpu, uint64_t currentCycle)
    {
        return m_impl->dispatch(ordinal, cpu, currentCycle);
    }

    std::vector<IopLibsd::ScheduledCallback> IopLibsd::takeScheduledCallbacks() noexcept
    {
        std::vector<ScheduledCallback> out;
        out.swap(m_impl->scheduled);
        return out;
    }

    bool IopLibsd::loopActive(uint32_t channel, uint32_t generation) const noexcept
    {
        if (channel >= kChannels)
            return false;
        const Impl::Transfer &transfer = m_impl->transfers[channel];
        return transfer.looping && transfer.generation == generation;
    }
}
'''

hdr = IOP + "/src/emulator/imports/iop_libsd.h"
src = IOP + "/src/emulator/imports/iop_libsd.cpp"
open(hdr, "w").write(HEADER)
open(src, "w").write(SOURCE)
print("wrote", hdr, src)

# CMake
p = IOP + "/CMakeLists.txt"
backup(p)
rep(p, "    src/emulator/imports/iop_cdvd.cpp\n", "    src/emulator/imports/iop_cdvd.cpp\n    src/emulator/imports/iop_libsd.cpp\n")

# emulator wiring
p = IOP + "/src/emulator/iop_emulator.cpp"
backup(p)
rep(p, '#include "imports/iop_cdvd.h"\n', '#include "imports/iop_cdvd.h"\n#include "imports/iop_libsd.h"\n')
rep(p, """        struct ScheduledGuestCallback
        {
            uint32_t function = 0u;
            uint32_t gp = 0u;
            uint32_t argument = 0u;
        };""", """        struct ScheduledGuestCallback
        {
            uint32_t function = 0u;
            uint32_t gp = 0u;
            uint32_t argument = 0u;
            uint32_t argument2 = 0u;
            // libsd loop-mode transfers re-arm while the channel/generation is still looping.
            uint64_t repeatCycles = 0u;
            uint32_t libsdChannel = 0u;
            uint32_t libsdGeneration = 0u;
        };""")
rep(p, """              cdvd(host, memory, kernel),
              vblank(kernel),""", """              cdvd(host, memory, kernel),
              libsd(host, memory),
              vblank(kernel),""")
rep(p, """            cdvd.reset();
            intrman.reset();""", """            cdvd.reset();
            libsd.reset();
            intrman.reset();""")
rep(p, """            if (iequals(call.library, "loadcore") && loadcore.dispatchImport(call.ordinal, cpu))
                return ImportDisposition::Handled;""", """            if (iequals(call.library, "loadcore") && loadcore.dispatchImport(call.ordinal, cpu))
                return ImportDisposition::Handled;
            if (iequals(call.library, "libsd") && libsd.dispatchImport(call.ordinal, cpu, totalCycles))
            {
                for (const IopLibsd::ScheduledCallback &callback : libsd.takeScheduledCallbacks())
                {
                    ScheduledGuestCallback scheduledCallback{};
                    scheduledCallback.function = callback.function;
                    scheduledCallback.gp = callback.gp;
                    scheduledCallback.argument = callback.a0;
                    scheduledCallback.argument2 = callback.a1;
                    scheduledCallback.repeatCycles = callback.repeatCycles;
                    scheduledCallback.libsdChannel = callback.channel;
                    scheduledCallback.libsdGeneration = callback.generation;
                    pendingGuestCallbacks.emplace(callback.dueCycle, scheduledCallback);
                }
                return ImportDisposition::Handled;
            }""")
rep(p, """                for (const ScheduledGuestCallback &callback : callbacks)
                {
                    if (callback.function != 0u)
                    {
                        (void)callFunction(callback.function,
                                           callback.argument,
                                           0u,
                                           0u,
                                           0u,
                                           callback.gp,
                                           100000u);
                    }""", """                for (const ScheduledGuestCallback &callback : callbacks)
                {
                    if (callback.function != 0u)
                    {
                        (void)callFunction(callback.function,
                                           callback.argument,
                                           callback.argument2,
                                           0u,
                                           0u,
                                           callback.gp,
                                           100000u);
                    }
                    if (callback.repeatCycles != 0u && libsd.loopActive(callback.libsdChannel, callback.libsdGeneration))
                        pendingGuestCallbacks.emplace(totalCycles + callback.repeatCycles, callback);""")
rep(p, """        IopCdvd cdvd;
        IopVblank vblank;""", """        IopCdvd cdvd;
        IopLibsd libsd;
        IopVblank vblank;""")
print("OK")
