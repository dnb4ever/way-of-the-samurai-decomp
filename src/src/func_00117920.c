extern s32 D_001FE488[];
extern void func_00117808(void);
s32 func_00117840(void *arg);
void func_00117890(s32 a, s32 b);

s32 func_00117920(s32 *dst, s32 *src, u32 count) {
    u32 n = count >> 2;
    u32 i = 0;
    if (n != 0) {
        do {
            *dst = *src;
            src++;
            dst++;
            i++;
        } while (i < n);
    }
    return 0;
}

s32 func_00117850(void) {
    func_00117890(D_001FE488[0], D_001FE488[1]);
    return func_00117840(&func_00117808) - 0x20C;
}
