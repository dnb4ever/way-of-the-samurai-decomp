s32 func_0010F0C0(void);
s32 func_0010F0D0(void);
s32 func_001171D0(void);

s32 func_00117190(void) {
    s32 r;
    if (func_0010F0C0() == 0x02000000) {
        r = func_001171D0();
    } else {
        r = func_0010F0D0();
    }
    return r;
}