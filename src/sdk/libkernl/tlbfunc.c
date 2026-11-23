/* tlbfunc.c -- libkernl: TLB setup */

#include "sdk/eekernel.h"

/*
================
func_00116FC0

Copies size bytes, a word at a time; run in kernel mode, where the
caller's patches go.
================
*/
int func_00116FC0(u32 *dst, u32 *src, u32 size)
{
    u32 i;

    for (i = 0; i < size / 4; i++) {
        *dst++ = *src++;
    }
    return 0;
}

/*
================
InitTLB

Machines with 32 MB of main memory get the library's TLB setup; others
keep the kernel's.
================
*/
int InitTLB(void)
{
    if (GetMemorySize() == 0x2000000) {
        return func_001171D0();
    }
    return _InitTLB();
}
