#!/usr/bin/env python3
# Scratch-tree-only: env-gated guest-memory write watch using the two existing (no-op) trace hooks.
#   PS2X_WATCH_ADDR=0x1FBFF60 [PS2X_WATCH_LEN=8]  -> logs every generated-code store and HLE range write
#   that overlaps the window, with the EE pc of the storing instruction (or the HLE op name).
import shutil
import sys

P = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime/include/ps2_runtime.h"
shutil.copyfile(P, P + ".orig") if not __import__("os").path.exists(P + ".orig") else None
s = open(P).read()

old_write = """inline void ps2TraceGuestWrite(uint8_t *rdram,
                               uint32_t guestAddr,
                               uint32_t size,
                               uint64_t valueLo,
                               uint64_t valueHi,
                               const char *op,
                               const R5900Context *ctx)
{
    (void)rdram;
    (void)guestAddr;
    (void)size;
    (void)valueLo;
    (void)valueHi;
    (void)op;
    (void)ctx;
    // TODO we dont need this anymore so on next release it will be deleted
}
"""
new_write = """struct Ps2GuestWriteWatch
{
    bool enabled = false;
    uint32_t address = 0u;
    uint32_t length = 16u;
    uint32_t logs = 0u;
};

inline Ps2GuestWriteWatch &ps2GuestWriteWatch()
{
    static Ps2GuestWriteWatch watch = []()
    {
        Ps2GuestWriteWatch configured;
        if (const char *addr = std::getenv("PS2X_WATCH_ADDR"))
        {
            configured.address = static_cast<uint32_t>(std::strtoul(addr, nullptr, 0)) & 0x1FFFFFFFu;
            configured.enabled = configured.address != 0u;
            if (const char *len = std::getenv("PS2X_WATCH_LEN"))
            {
                configured.length = static_cast<uint32_t>(std::strtoul(len, nullptr, 0));
            }
        }
        return configured;
    }();
    return watch;
}

inline bool ps2GuestWriteWatchHit(uint32_t guestAddr, uint32_t size)
{
    const Ps2GuestWriteWatch &watch = ps2GuestWriteWatch();
    if (!watch.enabled || size == 0u)
    {
        return false;
    }
    const uint32_t physical = guestAddr & 0x1FFFFFFFu;
    return physical < watch.address + watch.length && watch.address < physical + size;
}

inline void ps2TraceGuestWrite(uint8_t *rdram,
                               uint32_t guestAddr,
                               uint32_t size,
                               uint64_t valueLo,
                               uint64_t valueHi,
                               const char *op,
                               const R5900Context *ctx)
{
    (void)rdram;
    if (!ps2GuestWriteWatchHit(guestAddr, size))
    {
        return;
    }
    Ps2GuestWriteWatch &watch = ps2GuestWriteWatch();
    if (watch.logs >= 64u)
    {
        return;
    }
    ++watch.logs;
    std::cerr << "[watch] " << op << " addr=0x" << std::hex << guestAddr << " size=" << std::dec << size
              << " value=0x" << std::hex << valueLo;
    if (size > 8u)
    {
        std::cerr << ':' << valueHi;
    }
    if (ctx)
    {
        uint32_t raValue = 0u;
        uint32_t spValue = 0u;
        std::memcpy(&raValue, &ctx->r[31], sizeof(raValue));
        std::memcpy(&spValue, &ctx->r[29], sizeof(spValue));
        std::cerr << " pc=0x" << ctx->pc << " ra=0x" << raValue << " sp=0x" << spValue;
    }
    std::cerr << std::dec << std::endl;
}
"""
old_range = """inline void ps2TraceGuestRangeWrite(uint8_t *rdram,
                                    uint32_t guestAddr,
                                    uint32_t size,
                                    const char *op,
                                    const R5900Context *ctx)
{
    (void)rdram;
    (void)guestAddr;
    (void)size;
    (void)op;
    (void)ctx;
    // TODO we dont need this anymore so on next release it will be deleted
}
"""
new_range = """inline void ps2TraceGuestRangeWrite(uint8_t *rdram,
                                    uint32_t guestAddr,
                                    uint32_t size,
                                    const char *op,
                                    const R5900Context *ctx)
{
    (void)rdram;
    if (!ps2GuestWriteWatchHit(guestAddr, size))
    {
        return;
    }
    Ps2GuestWriteWatch &watch = ps2GuestWriteWatch();
    if (watch.logs >= 64u)
    {
        return;
    }
    ++watch.logs;
    std::cerr << "[watch] range " << op << " addr=0x" << std::hex << guestAddr << " size=" << std::dec << size;
    if (ctx)
    {
        std::cerr << " pc=0x" << std::hex << ctx->pc << std::dec;
    }
    std::cerr << std::endl;
}
"""
for old, new in ((old_write, new_write), (old_range, new_range)):
    if s.count(old) != 1:
        print("FAIL anchor", old[:60])
        sys.exit(1)
    s = s.replace(old, new)
if "#include <cstdlib>" not in s:
    s = s.replace("#include <cstring>\n", "#include <cstring>\n#include <cstdlib>\n#include <iostream>\n", 1)
open(P, "w").write(s)
print("patched", P)
