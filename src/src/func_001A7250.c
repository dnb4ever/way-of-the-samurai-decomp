void func_001A7250(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x14) = 0;
    *(s32 *)((u8 *)arg0 + 0x18) = 0;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0;
    *(s32 *)((u8 *)arg0 + 0x13C) = 0;
    *(s32 *)((u8 *)arg0 + 0x140) = 0;
    *(s32 *)((u8 *)arg0 + 0x144) = 0;
    *(s32 *)((u8 *)arg0 + 0xC) = 0;
    *(s32 *)((u8 *)arg0 + 0x10) = 0;
    *(s32 *)((u8 *)arg0 + 0x134) = 0;
    *(s32 *)((u8 *)arg0 + 0x138) = 0;
    *(s32 *)((u8 *)arg0 + 0x4) = 0;
    *(s32 *)((u8 *)arg0 + 0x0) = 0;
}

s32 func_001A73F0(void *arg0) {
    s32 (*fp)(void) = *(s32 (**)(void))((u8 *)arg0 + 8);
    if (fp == 0) {
        return 1;
    }
    return fp();
}

s64 func_001A7090(void *arg0, s32 arg1) {
    s16 f4 = *(s16 *)((u8 *)arg0 + 4);
    s64 v;
    if (f4 == 2) {
        v = 0;
    } else if (*(s32 *)((u8 *)arg0 + 0x14) == 0) {
        v = -1;
    } else {
        v = *(s32 *)((u8 *)arg0 + 0x18) +
            *(s32 *)((u8 *)(*(u32 *)((u8 *)arg0 + 0xC)) + arg1 * 8);
    }
    v = (v << 32) >> 32;
    if (v != -1 && f4 == 0) {
        v += *(s64 *)(*(u8 **)arg0 + 0x118);
    }
    return v;
}