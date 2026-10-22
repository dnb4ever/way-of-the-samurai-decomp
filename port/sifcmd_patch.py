#!/usr/bin/env python3
# Scratch-tree-only patch: HLE the IOP-side SIFCMD INIT_CMD handler so a raw-recompiled EE
# libkernel sceSifInitRpc() gets its SET_SREG(0,1) answer (delivered through the game's own
# sceSifCmdIntrHdlr via the DMAC ch5 dispatch that sceSifSetDma already performs).
import sys

P = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime/src/lib/Kernel/Stubs/SIF.cpp"
s = open(P).read()


def rep(old, new):
    global s
    n = s.count(old)
    if n != 1:
        print(f"FAIL: expected 1 match, found {n} for:\n{old[:120]}")
        sys.exit(1)
    s = s.replace(old, new)


# 1) state + reset
rep("""        bool g_sifCmdInitialized = false;
        uint32_t g_sifGetRegLogCount = 0u;
        uint32_t g_sifSetRegLogCount = 0u;
""", """        bool g_sifCmdInitialized = false;
        uint32_t g_sifGetRegLogCount = 0u;
        uint32_t g_sifSetRegLogCount = 0u;
        // EE packet-receive buffer announced by the raw libkernel sceSifInitCmd (INIT_CMD opt=0).
        uint32_t g_sifEeCmdPktBuf = 0u;
""")
rep("""            g_sifCmdInitialized = false;
            g_sifGetRegLogCount = 0u;
            g_sifSetRegLogCount = 0u;

            g_sifRegs[kSifRegBootStatus] = kSifBootReadyMask;""",
    """            g_sifCmdInitialized = false;
            g_sifGetRegLogCount = 0u;
            g_sifSetRegLogCount = 0u;
            g_sifEeCmdPktBuf = 0u;

            g_sifRegs[kSifRegBootStatus] = kSifBootReadyMask;""")

# 2) responder helper, placed right after writeEeRange (end of the anonymous namespace)
rep("""            ps2TraceGuestRangeWrite(rdram, address, sizeBytes, "SIF IOP-to-EE DMA", nullptr);
            const auto *bytes = static_cast<const uint8_t *>(source);
            for (uint32_t i = 0u; i < sizeBytes; ++i)
            {
                uint8_t *destination = getMemPtr(rdram, address + i);
                if (!destination)
                    return false;
                *destination = bytes[i];
            }
            return true;
        }
    }
""", """            ps2TraceGuestRangeWrite(rdram, address, sizeBytes, "SIF IOP-to-EE DMA", nullptr);
            const auto *bytes = static_cast<const uint8_t *>(source);
            for (uint32_t i = 0u; i < sizeBytes; ++i)
            {
                uint8_t *destination = getMemPtr(rdram, address + i);
                if (!destination)
                    return false;
                *destination = bytes[i];
            }
            return true;
        }

        // HLE of the IOP-side SIFCMD "INIT_CMD" (0x80000002) handler (rom0:SIFCMD) for games whose EE
        // libkernel SIFCMD/SIFRPC layer is recompiled raw (no sceSif* stubs bound). The EE's
        // sceSifInitCmd announces its packet-receive buffer with INIT_CMD opt=0; sceSifInitRpc then
        // sends INIT_CMD opt=1 and spins on sceSifGetSreg(0) until the IOP answers with a
        // SET_SREG(0, 1) packet DMA'd into that buffer and signalled through the SIF0 (DMAC ch5)
        // interrupt. sceSifSetDma already dispatches DMAC ch5 after every transfer, so writing the
        // reply packet before that dispatch lets the game's own sceSifCmdIntrHdlr consume it.
        void hleIopSifCmdInit(uint8_t *rdram, const uint8_t *payload, uint32_t sizeBytes)
        {
            constexpr uint32_t kSifCmdSetSreg = 0x80000001u;
            constexpr uint32_t kSifCmdInitCmd = 0x80000002u;
            if (!payload || sizeBytes < 16u)
                return;
            uint32_t cid = 0u;
            uint32_t opt = 0u;
            std::memcpy(&cid, payload + 8u, sizeof(cid));
            std::memcpy(&opt, payload + 12u, sizeof(opt));
            if (cid != kSifCmdInitCmd)
                return;

            if (opt == 0u)
            {
                if (sizeBytes < 20u)
                    return;
                uint32_t pktBuf = 0u;
                std::memcpy(&pktBuf, payload + 16u, sizeof(pktBuf));
                {
                    std::lock_guard<std::mutex> lock(g_sifCmdStateMutex);
                    g_sifEeCmdPktBuf = pktBuf;
                }
                std::cerr << "[sif-cmd-hle] INIT_CMD opt=0: EE pktbuf=0x" << std::hex << pktBuf << std::dec << std::endl;
                return;
            }
            if (opt != 1u)
                return;

            uint32_t dest = 0u;
            {
                std::lock_guard<std::mutex> lock(g_sifCmdStateMutex);
                dest = g_sifEeCmdPktBuf;
                if (dest == 0u)
                {
                    const auto mainAddr = g_sifRegs.find(1u); // SIF_REG_MAINADDR
                    if (mainAddr != g_sifRegs.end())
                        dest = mainAddr->second;
                }
            }
            if (dest == 0u)
            {
                std::cerr << "[sif-cmd-hle] INIT_CMD opt=1 but EE pktbuf unknown; cannot answer" << std::endl;
                return;
            }
            // SifCmdHeader { u8 psize; u8 dsize[3]; u32 dest; u32 cid; u32 opt } + { u32 index; u32 value }
            const uint32_t packet[6] = {0x18u, 0u, kSifCmdSetSreg, 0u, 0u, 1u};
            if (!writeEeRange(rdram, dest, packet, sizeof(packet)))
            {
                std::cerr << "[sif-cmd-hle] failed to write SET_SREG reply at 0x" << std::hex << dest << std::dec << std::endl;
                return;
            }
            std::cerr << "[sif-cmd-hle] INIT_CMD opt=1: SET_SREG(0,1) reply placed at 0x" << std::hex << dest << std::dec << std::endl;
        }
    }
""")

# 3) hook into sceSifSetDma after the EE->IOP copy, before the AfterCopy notification
rep("""                {
                    ok = false;
                    break;
                }
                if (runtime)
                {
                    PS2IopTransport::notifyTransfer(runtime, rdram, {
                                                                        ps2x::iop::SifTransferKind::SetDma,
                                                                        ps2x::iop::SifTransferPhase::AfterCopy,""",
    """                {
                    ok = false;
                    break;
                }
                hleIopSifCmdInit(rdram, payload.data(), sizeBytes);
                if (runtime)
                {
                    PS2IopTransport::notifyTransfer(runtime, rdram, {
                                                                        ps2x::iop::SifTransferKind::SetDma,
                                                                        ps2x::iop::SifTransferPhase::AfterCopy,""")

open(P, "w").write(s)
print("patched", P)
