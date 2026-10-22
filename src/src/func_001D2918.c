extern void *D_00210F40[];
extern s32 D_C290C8[];

s32 func_0010F2A8(s32, s32, s32);
s64 __muldi3(s64, s64);

s32 func_001D2998(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 ret;
    D_C290C8[0] = 0;
    ret = func_0010F2A8(arg1, arg2, arg3);
    if (ret == -1) {
        if (D_C290C8[0] != 0) {
            *arg0 = D_C290C8[0];
        }
    }
    return ret;
}

s32 func_001D2930(void) {
    void *p = D_00210F40[0];
    s64 r = __muldi3(*(s64 *)((u8 *)p + 0xA8), 0x5851F42D4C957F2DLL) + 1;
    *(s64 *)((u8 *)p + 0xA8) = r;
    return (s32)(((u64)r >> 32) & 0x7FFFFFFF);
}

void func_001D2918(u32 arg0) {
    *(u64 *)((u8 *)D_00210F40[0] + 0xA8) = (u64)arg0;
}