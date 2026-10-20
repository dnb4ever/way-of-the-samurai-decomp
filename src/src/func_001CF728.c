#include "context.h"

// matched by claude-1 (operator: you)
extern u8 D_00210F40[];
void *func_001D0C10(void *, s32);
void func_001D16A0(void *, s32, s32);
extern void func_001D34D0(void);
extern void func_001D3538(void);
extern void func_001D35B8(void);
extern void func_001D3620(void);

s32 func_001CF728(void)
{
    return *(s32 *)D_00210F40;
}

void func_001CF850(void *arg0, s16 arg1, s16 arg2, s32 arg3)
{
    *(s32 *)((char *)arg0 + 0x54) = arg3;
    *(s16 *)((char *)arg0 + 0xC) = arg1;
    *(s16 *)((char *)arg0 + 0xE) = arg2;
    *(void **)((char *)arg0 + 0x20) = (void *)func_001D34D0;
    *(void **)((char *)arg0 + 0x24) = (void *)func_001D3538;
    *(void **)((char *)arg0 + 0x28) = (void *)func_001D35B8;
    *(void **)((char *)arg0 + 0x2C) = (void *)func_001D3620;
    *(void **)((char *)arg0 + 0x1C) = arg0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x10) = 0;
    *(s32 *)((char *)arg0 + 0x8) = 0;
    *(s32 *)((char *)arg0 + 0x4) = 0;
    *(s32 *)((char *)arg0 + 0x0) = 0;
}

void *func_001CF8B0(void *arg0, s32 arg1)
{
    s32 sz = arg1 * 0x58;
    void *p = func_001D0C10(arg0, sz + 0xC);
    if (p != 0) {
        void *q = (char *)p + 0xC;
        *(s32 *)((char *)p + 4) = arg1;
        *(s32 *)((char *)p + 0) = 0;
        *(void **)((char *)p + 8) = q;
        func_001D16A0(q, 0, sz);
    }
    return p;
}
