/* libdma.c -- libdma: channel control */

#include "sdk/libdma.h"

#define D_ENABLER   ((volatile u32 *)0x1000f520)  /* volatile: DMA hold state register */
#define D_ENABLEW   ((volatile u32 *)0x1000f590)  /* volatile: DMA hold control register */

#define D_ENABLE_CPND   0x1000  /* hold all channels */

/* the register blocks of the ten channels */
static sceDmaChan *D_001FD270[10] = {
    (sceDmaChan *)0x10008000, (sceDmaChan *)0x10009000,
    (sceDmaChan *)0x1000a000, (sceDmaChan *)0x1000b000,
    (sceDmaChan *)0x1000b400, (sceDmaChan *)0x1000c000,
    (sceDmaChan *)0x1000c400, (sceDmaChan *)0x1000c800,
    (sceDmaChan *)0x1000d000, (sceDmaChan *)0x1000d400,
};

static int D_001FD298 = 0;  /* sceDmaDebug's setting */

/*
================
sceDmaGetChan

Returns channel id's registers, or NULL for an unknown channel.
================
*/
sceDmaChan *sceDmaGetChan(int id)
{
    if ((u32)id < 10) {
        return D_001FD270[id];
    }
    return NULL;
}

/*
================
sceDmaDebug

Turns the library's checks on or off; returns the previous setting.
================
*/
int sceDmaDebug(int enable)
{
    int old = D_001FD298;

    D_001FD298 = enable;
    return old;
}

#ifdef NON_MATCHING
/*
================
sceDmaPause

Stops channel d with the controller held; returns its CHCR from before.
Parked: retail puts the final volatile store to D_ENABLEW in the return's
delay slot, which ee-gcc 2.9-991111 never does with a volatile access
(nor 2.96); see the report on the SDK compiler build.
================
*/
u32 sceDmaPause(sceDmaChan *d)
{
    u32 chcr;

    if (*D_ENABLER == 0) {
        *D_ENABLEW = D_ENABLE_CPND;
    }
    chcr = d->chcr;
    d->chcr = chcr & ~D_CHCR_STR;
    *D_ENABLEW = 0;
    return chcr;
}
#endif

/*
================
sceDmaRestart

Writes chcr back to channel d; returns whether the channel was running.
================
*/
int sceDmaRestart(sceDmaChan *d, u32 chcr)
{
    u32 old = d->chcr;

    d->chcr = chcr;
    return (old >> 8) & 1;
}
