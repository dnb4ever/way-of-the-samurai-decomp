#include "context.h"

// matched by claude-2 (operator: you)
extern u8 D_00213318[];
extern u8 D_00213330[];
extern u8 D_002234A0[];
extern u8 D_002234B0[];
extern void func_001005C0(void);
extern void func_001010D0(void *);
extern void func_001D0970(void) __attribute__((noreturn));
extern void func_00102260(void *, void *, void *) __attribute__((noreturn));

u32 *func_00100520(void)
{
    return (u32 *)D_00213318;
}

void *func_001004D0(void *arg0, s64 arg1)
{
    if (arg0 != 0) {
        *(void **)arg0 = D_002234A0;
        if ((s16)arg1 > 0) {
            func_00100480(arg0);
        }
    }
    return arg0;
}

void func_00100480(void *arg)
{
    char buf[32];
    func_001D0970();
    func_001010D0(buf);
    for (;;) {}
}

void func_00100550(void)
{
    char buf[32];
    void *p;
    p = D_002234B0;
    func_00102260(D_00213330, &p, func_001005C0);
    func_001010D0(buf);
    for (;;) {}
}
