/* intr.c -- libkernl: interrupt-handler versions of the INTC/DMAC masks */

#include "sdk/eekernel.h"

#ifdef NON_MATCHING
/*
================
iEnableIntc

Parked: the kernel call is followed by a sync.l, which only inline
assembly emits.
================
*/
int iEnableIntc(int ch)
{
    int ret = _iEnableIntc(ch);

    __asm__ volatile ("sync.l");
    return ret;
}

/*
================
iDisableIntc

Parked: sync.l after the kernel call, as iEnableIntc.
================
*/
int iDisableIntc(int ch)
{
    int ret = _iDisableIntc(ch);

    __asm__ volatile ("sync.l");
    return ret;
}

/*
================
iEnableDmac

Parked: sync.l after the kernel call, as iEnableIntc.
================
*/
int iEnableDmac(int ch)
{
    int ret = _iEnableDmac(ch);

    __asm__ volatile ("sync.l");
    return ret;
}

/*
================
iDisableDmac

Parked: sync.l after the kernel call, as iEnableIntc.
================
*/
int iDisableDmac(int ch)
{
    int ret = _iDisableDmac(ch);

    __asm__ volatile ("sync.l");
    return ret;
}
#endif
