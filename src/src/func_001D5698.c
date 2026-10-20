#include "context.h"

// matched by claude-2 (operator: you)
extern s32 func_001CFFB8(void);
extern u8 D_0021D1E9[];
extern s32 D_00210F40[];
extern void func_001D5460(s32, s32, s32, s32);

s32 func_001D56E8(void *a0, s32 *arg1)
{
    s32 r;
    if (arg1[2] == 0) {
        arg1[1] = 0;
        return 0;
    }
    r = func_001CFFB8();
    arg1[1] = 0;
    arg1[2] = 0;
    return r;
}

s32 func_001D56C8(s32 arg0)
{
    return (D_0021D1E9[arg0] & 2) ? (arg0 - 0x20) : arg0;
}

void func_001D5698(s32 arg0, s32 arg1, s32 arg2)
{
    func_001D5460(D_00210F40[0], arg0, arg1, arg2);
}
