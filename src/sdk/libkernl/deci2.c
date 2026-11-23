/* deci2.c -- libkernl: DECI2 protocol calls through the Deci2Call syscall */

#include "sdk/eekernel.h"
#include "sdk/deci2.h"

/* the kernel's work area for the protocol opened by sceDeci2Open */
static u128 D_00225690[3];  /* size: the gap to the next object's data */

/*
================
sceDeci2Open
================
*/
int sceDeci2Open(u16 protocol, void *opt, void (*handler)(int event, int param, void *opt))
{
    u32 param[4];

    param[0] = protocol;
    param[1] = (u32)opt;
    param[2] = (u32)handler;
    param[3] = (u32)D_00225690 | UNCACHED_SEG;
    return Deci2Call(DECI2_OPEN, param);
}

/*
================
sceDeci2Close
================
*/
int sceDeci2Close(int s)
{
    u32 param[4];

    param[0] = s;
    return Deci2Call(DECI2_CLOSE, param);
}

/*
================
sceDeci2ReqSend
================
*/
int sceDeci2ReqSend(int s, char dest)
{
    u32 param[4];

    param[0] = s;
    param[1] = dest;
    return Deci2Call(DECI2_REQSEND, param);
}

/*
================
sceDeci2Poll
================
*/
void sceDeci2Poll(int s)
{
    u32 param[4];

    param[0] = s;
    Deci2Call(DECI2_POLL, param);
}

/*
================
sceDeci2ExRecv
================
*/
int sceDeci2ExRecv(int s, void *buf, u16 len)
{
    u32 param[4];

    param[0] = s;
    param[1] = (u32)buf;
    param[2] = len;
    return Deci2Call(DECI2_EXRECV, param);
}

/*
================
sceDeci2ExSend
================
*/
int sceDeci2ExSend(int s, void *buf, u16 len)
{
    u32 param[4];

    param[0] = s;
    param[1] = (u32)buf;
    param[2] = len;
    return Deci2Call(DECI2_EXSEND, param);
}

/*
================
sceDeci2ExReqSend
================
*/
int sceDeci2ExReqSend(int s, char dest)
{
    u32 param[4];

    param[0] = s;
    param[1] = dest;
    return Deci2Call(DECI2_EXREQSEND, param);
}

/*
================
sceDeci2ExLock
================
*/
int sceDeci2ExLock(int s)
{
    u32 param[4];

    param[0] = s;
    return Deci2Call(DECI2_EXLOCK, param);
}

/*
================
sceDeci2ExUnLock
================
*/
int sceDeci2ExUnLock(int s)
{
    u32 param[4];

    param[0] = s;
    return Deci2Call(DECI2_EXUNLOCK, param);
}

/*
================
kputs

Prints s on the host console.
================
*/
int kputs(char *s)
{
    u32 param[4];

    param[0] = (u32)s;
    return Deci2Call(DECI2_KPUTS, param);
}
