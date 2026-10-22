#!/usr/bin/env python3
# Scratch-tree-only: log IOP->EE SIF DMA descriptors (bounded) and EE thread stack ranges in the tick dump.
import os
import shutil
import sys

IOP = "/tmp/ps2recomp_build/PS2Recomp/ps2xIOP"
RT = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime"


def backup(path):
    if not os.path.exists(path + ".orig"):
        shutil.copyfile(path, path + ".orig")


def rep(path, old, new):
    s = open(path).read()
    n = s.count(old)
    if n != 1:
        print(f"FAIL {path}: expected 1 match, found {n} for:\n{old[:160]}")
        sys.exit(1)
    open(path, "w").write(s.replace(old, new))
    print("patched", path)


p = IOP + "/src/emulator/services/iop_rpc.cpp"
backup(p)
rep(p, """            for (uint32_t i = 0u; i < pendingCount; ++i)
            {
                const PendingTransfer &transfer = pending[i];
                if (!m_memory.readRam(transfer.source, scratch.data(), transfer.size) || !m_host.writeGuest(transfer.destination, scratch.data(), transfer.size))
                {
                    setV0(0u);
                    return true;
                }
            }""", """            for (uint32_t i = 0u; i < pendingCount; ++i)
            {
                const PendingTransfer &transfer = pending[i];
                {
                    static uint32_t s_dmaLogs = 0u;
                    if (s_dmaLogs < 96u)
                    {
                        ++s_dmaLogs;
                        std::ostringstream out;
                        out << "[IOP sif-dma] IOP 0x" << std::hex << transfer.source << " -> EE 0x" << transfer.destination
                            << " size=0x" << transfer.size << std::dec;
                        m_host.log(LogLevel::Info, out.str());
                    }
                }
                if (!m_memory.readRam(transfer.source, scratch.data(), transfer.size) || !m_host.writeGuest(transfer.destination, scratch.data(), transfer.size))
                {
                    setV0(0u);
                    return true;
                }
            }""")
s = open(p).read()
if "#include <sstream>" not in s:
    s = s.replace("#include <algorithm>", "#include <algorithm>\n#include <sstream>", 1) if "#include <algorithm>" in s else "#include <sstream>\n" + s
    open(p, "w").write(s)
    print("sstream include added")

p = RT + "/src/lib/ps2_runtime.cpp"
rep(p, """                                                   << " wait=" << static_cast<int>(thread.waitReason)
                                                   << "/" << thread.waitId
                                                   << " prio=" << thread.currentPriority
                                                   << std::endl);""", """                                                   << " wait=" << static_cast<int>(thread.waitReason)
                                                   << "/" << thread.waitId
                                                   << " prio=" << thread.currentPriority
                                                   << std::hex << " sp=0x" << thread.sp
                                                   << " stack=0x" << thread.stack << "+0x" << thread.stackSize << std::dec
                                                   << std::endl);""")
print("OK")
