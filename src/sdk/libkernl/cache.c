/* cache.c -- libkernl: data cache write-back and invalidation */

#include "sdk/eekernel.h"

/* a data cache line is 64 bytes */
#define DCACHE_LINE_MASK    (~0x3f)

/*
================
iSyncDCache

Writes back the data cache lines over [start, end]; interrupt-handler
version, interrupts are already off.
================
*/
void iSyncDCache(void *start, void *end)
{
    _sceSDC((u32)start & DCACHE_LINE_MASK, (u32)end & DCACHE_LINE_MASK);
}

/*
================
iInvalidDCache

Drops the data cache lines over [start, end] without writing them back;
interrupt-handler version.
================
*/
void iInvalidDCache(void *start, void *end)
{
    _sceIDC((u32)start & DCACHE_LINE_MASK, (u32)end & DCACHE_LINE_MASK);
}
