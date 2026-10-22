#!/usr/bin/env python3
# Scratch-tree-only (ps2xIOP + ps2xRuntime): IOP per-thread snapshot and env-gated import trace.
import os
import shutil
import sys

IOP = "/tmp/ps2recomp_build/PS2Recomp/ps2xIOP"
RT = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime"


def backup(path):
    if not os.path.exists(path + ".orig"):
        shutil.copyfile(path, path + ".orig")


def rep(path, old, new, count=1):
    s = open(path).read()
    n = s.count(old)
    if n != count:
        print(f"FAIL {path}: expected {count} match(es), found {n} for:\n{old[:160]}")
        sys.exit(1)
    open(path, "w").write(s.replace(old, new))
    print("patched", path)


# ---- iop_kernel.h: snapshot API
p = IOP + "/src/emulator/core/iop_kernel.h"
backup(p)
rep(p, "#include <map>\n", "#include <map>\n#include <vector>\n")
rep(p, """        [[nodiscard]] size_t threadCount() const noexcept { return m_threads.size(); }
""", """        [[nodiscard]] size_t threadCount() const noexcept { return m_threads.size(); }
        struct ThreadSnapshot
        {
            int id = 0;
            IopThreadState state = IopThreadState::Dormant;
            uint32_t pc = 0;
            uint32_t entry = 0;
            uint32_t priority = 0;
            int waitId = 0;
            uint32_t waitBits = 0;
            uint64_t wakeCycle = 0;
            int wakeupCount = 0;
        };
        [[nodiscard]] std::vector<ThreadSnapshot> snapshotThreads() const;
        [[nodiscard]] int currentThreadId() const noexcept { return m_currentThread ? m_currentThread->id : 0; }
""")

# ---- iop_kernel.cpp: snapshot impl
p = IOP + "/src/emulator/core/iop_kernel.cpp"
backup(p)
rep(p, """    void IopKernel::reset()
    {
        m_threads.clear();""", """    std::vector<IopKernel::ThreadSnapshot> IopKernel::snapshotThreads() const
    {
        std::vector<ThreadSnapshot> out;
        out.reserve(m_threads.size());
        for (const auto &[id, thread] : m_threads)
        {
            ThreadSnapshot row;
            row.id = id;
            row.state = thread.state;
            row.pc = thread.cpu.pc;
            row.entry = thread.entry;
            row.priority = thread.priority;
            row.waitId = thread.waitId;
            row.waitBits = thread.waitBits;
            row.wakeCycle = thread.wakeCycle;
            row.wakeupCount = thread.wakeupCount;
            out.push_back(row);
        }
        return out;
    }

    void IopKernel::reset()
    {
        m_threads.clear();""")

# ---- iop_emulator.h: describeThreads
p = IOP + "/src/emulator/iop_emulator.h"
backup(p)
rep(p, """        [[nodiscard]] uint32_t rpcServerCount() const noexcept;
""", """        [[nodiscard]] uint32_t rpcServerCount() const noexcept;
        [[nodiscard]] std::vector<std::string> describeThreads() const;
""")

# ---- iop_emulator.cpp: trace + describeThreads
p = IOP + "/src/emulator/iop_emulator.cpp"
backup(p)
s = open(p).read()
if "#include <cstdlib>" not in s:
    s = s.replace("#include <map>", "#include <map>\n#include <cstdlib>", 1) if "#include <map>" in s else "#include <cstdlib>\n" + s
    print("cstdlib include added")
open(p, "w").write(s)
rep(p, """              loadcore(memory, imports)
        {
            reset();
        }""", """              loadcore(memory, imports)
        {
            traceImports = std::getenv("PS2X_IOP_TRACE_IMPORTS") != nullptr;
            reset();
        }
        static const char *threadStateName(IopThreadState state)
        {
            switch (state)
            {
            case IopThreadState::Dormant: return "Dormant";
            case IopThreadState::Ready: return "Ready";
            case IopThreadState::Running: return "Running";
            case IopThreadState::Sleep: return "Sleep";
            case IopThreadState::Delay: return "Delay";
            case IopThreadState::Semaphore: return "Semaphore";
            case IopThreadState::EventFlag: return "EventFlag";
            case IopThreadState::Suspended: return "Suspended";
            case IopThreadState::Dead: return "Dead";
            }
            return "?";
        }
        std::vector<std::string> describeThreads() const
        {
            std::vector<std::string> out;
            for (const IopKernel::ThreadSnapshot &thread : kernel.snapshotThreads())
            {
                std::ostringstream line;
                line << "id=" << thread.id << " state=" << threadStateName(thread.state)
                     << " pc=0x" << std::hex << thread.pc << " entry=0x" << thread.entry << std::dec
                     << " prio=" << thread.priority << " waitId=" << thread.waitId
                     << " waitBits=0x" << std::hex << thread.waitBits << std::dec
                     << " wakeCycle=" << thread.wakeCycle << " wakeups=" << thread.wakeupCount
                     << (kernel.currentThreadId() == thread.id ? " (current)" : "");
                out.push_back(line.str());
            }
            return out;
        }""")
rep(p, """            if (const auto import = imports.decode(cpu.pc))
            {
                const ImportDisposition disposition = dispatchImport(*import, cpu);
                ++totalInstructions;
                ++totalCycles;
                if (disposition == ImportDisposition::JumpToGuest)
                    return true;""", """            if (const auto import = imports.decode(cpu.pc))
            {
                const uint32_t traceArgs[4] = {cpu.gpr[4], cpu.gpr[5], cpu.gpr[6], cpu.gpr[7]};
                const uint32_t tracePc = cpu.pc;
                const uint32_t traceRa = cpu.gpr[31];
                const ImportDisposition disposition = dispatchImport(*import, cpu);
                if (traceImports && importTraceTotal < 8000u)
                {
                    const std::string key = import->library + ":" + std::to_string(import->ordinal);
                    uint32_t &count = importTraceCounts[key];
                    if (count < 40u)
                    {
                        ++count;
                        ++importTraceTotal;
                        std::ostringstream out;
                        out << "[IOP import] thr=" << kernel.currentThreadId() << ' ' << key
                            << " a0=0x" << std::hex << traceArgs[0] << " a1=0x" << traceArgs[1]
                            << " a2=0x" << traceArgs[2] << " a3=0x" << traceArgs[3]
                            << " -> v0=0x" << cpu.gpr[2] << " ra=0x" << traceRa << " stub=0x" << tracePc << std::dec
                            << (disposition == ImportDisposition::Missing ? " MISSING" : disposition == ImportDisposition::JumpToGuest ? " guest" : "")
                            << (cpu.yielded ? " yielded" : "");
                        log(LogLevel::Info, out.str());
                    }
                }
                ++totalInstructions;
                ++totalCycles;
                if (disposition == ImportDisposition::JumpToGuest)
                    return true;""")
rep(p, """        GuestCallback checkKelfPathCallback;
    };""", """        GuestCallback checkKelfPathCallback;
        bool traceImports = false;
        uint32_t importTraceTotal = 0u;
        std::map<std::string, uint32_t> importTraceCounts;
    };""")
rep(p, """    uint32_t IopEmulator::rpcServerCount() const noexcept
""", """    std::vector<std::string> IopEmulator::describeThreads() const
    {
        return m_impl->describeThreads();
    }
    uint32_t IopEmulator::rpcServerCount() const noexcept
""")

# ---- iop_types.h: DebugSnapshot.threads
p = IOP + "/include/ps2x/iop/iop_types.h"
backup(p)
rep(p, """        std::vector<DebugService> services;
        std::vector<std::string> diagnostics;
    };""", """        std::vector<DebugService> services;
        std::vector<std::string> diagnostics;
        std::vector<std::string> threads;
    };""")

# ---- iop_subsystem.cpp
p = IOP + "/src/iop_subsystem.cpp"
backup(p)
rep(p, """        snapshot.diagnostics = m_impl->loadOutcomes;
""", """        snapshot.diagnostics = m_impl->loadOutcomes;
        snapshot.threads = m_impl->emulator.describeThreads();
""")

# ---- runtime tick emitter
p = RT + "/src/lib/ps2_runtime.cpp"
rep(p, """                    for (const std::string &diagnostic : iop.diagnostics)
                    {
                        RUNTIME_LOG("[run:iop-diag] " << diagnostic << std::endl);
                    }""", """                    for (const std::string &thread : iop.threads)
                    {
                        RUNTIME_LOG("[run:iop-thread] " << thread << std::endl);
                    }""")
print("OK")
