s32 func_0013A2F0(void *arg0, s32 arg1) {
    u32 v = 0x80000000;
    v |= arg1;
    *(u64 *)((u8 *)arg0 + 0x1F0) = (u64)v;
    *(u64 *)((u8 *)arg0 + 0x100) = (u64)v;
}