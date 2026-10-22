extern volatile s32 D_001FD390[];

s32 func_001096A0(void *obj, u32 base) {
    u32 idx;
    s32 v;
    idx = base >> 28;
    *(volatile u32 *)0x10002000 = base;
    v = D_001FD390[idx];
    *(s32 *)((u8 *)obj + 0x818) = v;
    return v;
}