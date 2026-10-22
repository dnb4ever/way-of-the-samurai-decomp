#!/usr/bin/env python3
# Scratch-tree-only: SetupThread(stack = -1) must hand the root thread an sp at the TOP of the
# top-of-RAM stack region (EE kernel semantics), not at its bottom. The previous HLE returned
# RAM_SIZE - stack_size, so the game's stack grew down into the heap (SetupHeap(-1) caps the heap at
# that same boundary) and heap writes corrupted stack frames. The runtime also carves its EE
# invocation (interrupt/alarm/SIF handler) stacks downward from RAM_SIZE, so the main sp starts
# below a reserved band for those.
import shutil
import sys

P = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime/src/lib/Kernel/Syscalls/System.cpp"
shutil.copyfile(P, P + ".fixA")
s = open(P).read()
old = """        if (stack == 0xFFFFFFFFu)
        {
            if (stackSizeSigned > 0)
            {
                const uint32_t requestedSize = static_cast<uint32_t>(stackSizeSigned);
                if (requestedSize < PS2_RAM_SIZE)
                {
                    sp = PS2_RAM_SIZE - requestedSize;
                }
                else
                {
                    sp = PS2_RAM_SIZE;
                }
            }
            else
            {
                sp = PS2_RAM_SIZE;
            }
        }"""
new = """        uint32_t topOfRamStackBase = 0u;
        if (stack == 0xFFFFFFFFu)
        {
            // Kernel semantics: the stack occupies the top `stack_size` bytes of RAM and the thread
            // starts at its top. The runtime reserves the very top band for EE invocation stacks
            // (reserveAsyncCallbackStack grows down from PS2_RAM_SIZE), so start the root thread
            // just below that band; the region still belongs to the thread's stack.
            constexpr uint32_t kInvocationStackBand = 0x40000u;
            const uint32_t requestedSize = stackSizeSigned > 0 ? static_cast<uint32_t>(stackSizeSigned) : 0u;
            topOfRamStackBase = requestedSize != 0u && requestedSize < PS2_RAM_SIZE ? PS2_RAM_SIZE - requestedSize : 0u;
            sp = PS2_RAM_SIZE - kInvocationStackBand;
            if (topOfRamStackBase != 0u && sp <= topOfRamStackBase)
            {
                sp = PS2_RAM_SIZE;
            }
            std::cerr << "[SetupThread] stack=-1 size=0x" << std::hex << requestedSize
                      << " -> region 0x" << topOfRamStackBase << "..0x" << PS2_RAM_SIZE
                      << " initial sp=0x" << sp << std::dec << std::endl;
        }"""
n = s.count(old)
if n != 1:
    print("FAIL matches", n)
    sys.exit(1)
s = s.replace(old, new)
old2 = """        if (stack == 0xFFFFFFFFu)
        {
            initialStack = sp;
        }
        else if (stack != 0u)"""
new2 = """        if (stack == 0xFFFFFFFFu)
        {
            initialStack = topOfRamStackBase;
        }
        else if (stack != 0u)"""
n = s.count(old2)
if n != 1:
    print("FAIL matches2", n)
    sys.exit(1)
open(P, "w").write(s.replace(old2, new2))
print("patched", P)
