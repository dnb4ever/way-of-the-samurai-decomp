/* initsys.c -- libkernl: library start-up */

#include "sdk/eekernel.h"

/* a system call the library installs: its number and handler */
typedef struct {
    int   num;
    u32 *(*func)(u32 value);
} SyscallEntry;

void setup(int num, void *handler);
u32 *FindAddress(u32 value);
u32 *kFindAddress(u32 value);

#define SYSCALL_FIND_ADDRESS    131

/*
================
kFindAddress

Kernel-mode search of kernel memory for the word value.
================
*/
u32 *kFindAddress(u32 value)
{
    u32 *p = (u32 *)0x80000000;

    while (*p != value) {
        p++;
    }
    return p;
}

static SyscallEntry D_001FE488 = { SYSCALL_FIND_ADDRESS, kFindAddress };

/*
================
func_00117850

Returns the kernel's system call table: installs kFindAddress as a system
call, which finds its own entry in the table.
================
*/
u32 *func_00117850(void)
{
    setup(D_001FE488.num, D_001FE488.func);
    return FindAddress((u32)kFindAddress) - SYSCALL_FIND_ADDRESS;
}

/*
================
_InitSys

Sets up the kernel services the library provides, before main.
================
*/
void _InitSys(void)
{
    func_001177C0();
    func_00117850();
    InitAlarm();
    InitThread();
    func_001179D0();
    InitTLBFunctions();
}

/*
================
func_00117920

Copies size bytes, a word at a time; run in kernel mode, where the
caller's patches go.
================
*/
int func_00117920(u32 *dst, u32 *src, u32 size)
{
    u32 i;

    for (i = 0; i < size / 4; i++) {
        *dst++ = *src++;
    }
    return 0;
}

/*
================
func_00117A80
================
*/
void func_00117A80(void)
{
    InitTLB();
}
