#include "context.h"

// matched by claude-1 (operator: you)
void *func_001D0C10(void *, s32);
void func_001D16A0(void *, s32, s32);

void *func_001CF8B0(void *arg0, s32 arg1)
{
    s32 sz = arg1 * 0x58;
    void *p = func_001D0C10(arg0, sz + 0xC);
    if (p == 0) {
        return p;
    }
    *(s32 *)((char *)p + 4) = arg1;
    *(s32 *)((char *)p + 0) = 0;
    *(void **)((char *)p + 8) = (char *)p + 0xC;
    func_001D16A0((char *)p + 0xC, 0, sz);
    return p;
}
