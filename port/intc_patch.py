#!/usr/bin/env python3
# Scratch-tree-only: model INTC_STAT (0x1000F000, write-1-to-clear) and INTC_MASK (0x1000F010,
# write-1-to-toggle). Previously both fell into the generic I/O register map, so libgraph's
# sceGsSyncV() `INTC_STAT = 4; while (!(INTC_STAT & 4) && !flag);` returned immediately and the
# kernel's one-shot SetVSyncFlag write landed on a dead stack frame.
import os
import re
import shutil
import sys

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


# header
p = RT + "/include/runtime/ps2_memory.h"
backup(p)
rep(p, "class PS2Memory\n{\npublic:\n", """class PS2Memory
{
public:
    // EE INTC: pending-interrupt status (write 1 to clear) and mask (write 1 to toggle).
    void raiseIntcStatus(uint32_t bits) noexcept { m_intcStat.fetch_or(bits & 0x7FFFu, std::memory_order_acq_rel); }
    [[nodiscard]] uint32_t intcStatus() const noexcept { return m_intcStat.load(std::memory_order_acquire); }
    [[nodiscard]] uint32_t intcMask() const noexcept { return m_intcMask.load(std::memory_order_acquire); }
""")
s = open(p).read()
m = re.search(r"\nprivate:\n", s)
if not m:
    print("FAIL: no private section in ps2_memory.h")
    sys.exit(1)
s = s[:m.end()] + "    std::atomic<uint32_t> m_intcStat{0u};\n    std::atomic<uint32_t> m_intcMask{0u};\n" + s[m.end():]
if "#include <atomic>" not in s:
    s = s.replace("#include <cstdint>", "#include <cstdint>\n#include <atomic>", 1)
open(p, "w").write(s)
print("members added")

# memory.cpp read/write
p = RT + "/src/lib/ps2_memory.cpp"
rep(p, "bool PS2Memory::writeIORegister(uint32_t address, uint32_t value)\n{\n", """bool PS2Memory::writeIORegister(uint32_t address, uint32_t value)
{
    if (address == 0x1000F000u) // INTC_STAT: writing 1 bits acknowledges (clears) them
    {
        m_intcStat.fetch_and(~(value & 0x7FFFu), std::memory_order_acq_rel);
        return true;
    }
    if (address == 0x1000F010u) // INTC_MASK: writing 1 bits toggles them
    {
        m_intcMask.fetch_xor(value & 0x7FFFu, std::memory_order_acq_rel);
        return true;
    }
""")
rep(p, "uint32_t PS2Memory::readIORegister(uint32_t address)\n{\n", """uint32_t PS2Memory::readIORegister(uint32_t address)
{
    if (address == 0x1000F000u)
    {
        return m_intcStat.load(std::memory_order_acquire);
    }
    if (address == 0x1000F010u)
    {
        return m_intcMask.load(std::memory_order_acquire);
    }
""")

# scheduler: raise bits
p = RT + "/src/lib/Kernel/EeScheduler.cpp"
backup(p)
rep(p, """    case EeEventType::VBlankStart:
        ++m_vsyncTick;
""", """    case EeEventType::VBlankStart:
        ++m_vsyncTick;
        m_runtime.memory().raiseIntcStatus(1u << 2); // INTC_VBLANK_S
""")
rep(p, """    case EeEventType::VBlankEnd:
""", """    case EeEventType::VBlankEnd:
        m_runtime.memory().raiseIntcStatus(1u << 3); // INTC_VBLANK_E
""")
rep(p, """        if ((timerInterrupts & (1u << timer)) != 0u)
        {
            dispatchIrq(false, 9u + timer);""", """        if ((timerInterrupts & (1u << timer)) != 0u)
        {
            m_runtime.memory().raiseIntcStatus(1u << (9u + timer)); // INTC_TIM0..3
            dispatchIrq(false, 9u + timer);""")
print("OK")
