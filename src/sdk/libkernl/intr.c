/* intr.c -- libkernl: interrupt-handler versions of the INTC/DMAC masks */

#include "sdk/eekernel.h"

/*
================
iEnableIntc
================
*/
int iEnableIntc(int ch)
{
    int ret = _iEnableIntc(ch);

    SYNCL();
    return ret;
}

/*
================
iDisableIntc
================
*/
int iDisableIntc(int ch)
{
    int ret = _iDisableIntc(ch);

    SYNCL();
    return ret;
}

/*
================
iEnableDmac
================
*/
int iEnableDmac(int ch)
{
    int ret = _iEnableDmac(ch);

    SYNCL();
    return ret;
}

/*
================
iDisableDmac
================
*/
int iDisableDmac(int ch)
{
    int ret = _iDisableDmac(ch);

    SYNCL();
    return ret;
}
