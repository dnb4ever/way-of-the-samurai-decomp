/* alarm.c -- libkernl: the alarm service the library installs in the kernel */

#include "sdk/eekernel.h"

/*
================
func_0010F980

Copies size bytes, a word at a time; run in kernel mode, where the
caller's patches go.
================
*/
int func_0010F980(u32 *dst, u32 *src, u32 size)
{
    u32 i;

    for (i = 0; i < size / 4; i++) {
        *dst++ = *src++;
    }
    return 0;
}
