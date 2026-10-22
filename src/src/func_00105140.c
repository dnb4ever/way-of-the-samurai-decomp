/* identical to func_00104E40 (matched), with its names changed by decomp.wtf */
/* ---- func_00105140 (conform to context.h prototype) ---- */
void func_00105140(volatile u8 *dst, const volatile u8 *src, u32 len) {
    volatile s32 *p = (volatile s32 *)dst;
    p[0] = (s32)src;
    p[2] = 0;
    p[1] = (s32)src;
}

/* ---- func_00105118 ---- */
void func_00105118(void **arg0, u32 arg1, u64 arg2) {
    u64 *p = (u64 *)*arg0;
    *p++ = arg2;
    *p++ = (u64)arg1;
    *arg0 = (void *)p;
}