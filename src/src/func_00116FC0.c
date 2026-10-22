s32 func_00116FC0(s32 *dst, s32 *src, u32 n) {
    u32 count = n >> 2;
    u32 i;
    for (i = 0; i < count; i++) {
        *dst++ = *src++;
    }
    return 0;
}
