s32 func_0010F418(void *arg0, void *arg1) {
    *(s64 *)((char *)arg1 + 0x48) = 0;
    *(s32 *)((char *)arg1 + 4) = 0x2000;
    return 0;
}

s32 *func_001CF728(void);
s32 func_0010F320(void) {
    *func_001CF728() = 5;
    return -1;
}

s32 func_00110370(s32, s32);
s32 func_00110440(s32);
extern s32 D_001FD5D0[];
s32 func_0010F2A8(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 != 0) return -1;
    if (D_001FD5D0[0] == 0) {
        if (func_00110440(arg1) == 0) return -1;
        D_001FD5D0[0] = 1;
    }
    return func_00110370(arg1, arg2);
}

void func_0010F4B0(s32, s32);
void func_0010F5D8(s32 arg0, s32 arg1) {
    func_0010F4B0(arg0 & 0xFFFFFFC0, arg1 & 0xFFFFFFC0);
}