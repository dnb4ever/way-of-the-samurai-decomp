extern void (*D_001FD210[])(void);
void func_00100980(void) {
    D_001FD210[0]();
}

s32 *func_00100630(s32);
s32 func_00166CD0(s32, s32);

void *func_001A2D20(void *arg0, s32 arg1, s32 arg2) {
    s32 r = func_00166CD0(arg2, *(s32 *)0x8EDF34);
    *(s32 *)arg0 = r;
    if (r != 0) {
        s32 *p = func_00100630(4);
        if (p != 0) *p = 1;
        *(void **)((u8 *)arg0 + 4) = p;
    }
    return arg0;
}

void *func_001A2E50(void *arg0, s32 arg1, s32 arg2) {
    s32 r = func_00166CD0(arg2, *(s8 *)0x8EDE83);
    *(s32 *)arg0 = r;
    if (r != 0) {
        s32 *p = func_00100630(4);
        if (p != 0) *p = 1;
        *(void **)((u8 *)arg0 + 4) = p;
    }
    return arg0;
}

void *func_001A2F80(void *arg0, s32 arg1, s32 arg2) {
    s32 r = func_00166CD0(arg2, *(s8 *)0x8EDE81);
    *(s32 *)arg0 = r;
    if (r != 0) {
        s32 *p = func_00100630(4);
        if (p != 0) *p = 1;
        *(void **)((u8 *)arg0 + 4) = p;
    }
    return arg0;
}