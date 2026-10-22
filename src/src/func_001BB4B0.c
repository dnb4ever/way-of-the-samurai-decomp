void func_00188210(void *);
void func_00187780(void *);

void func_001BB4B0(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    *(s32 *)((u8 *)arg0 + 0x928) = arg2;
    *(s32 *)((u8 *)arg0 + 0x92C) = arg3;
    *(s32 *)((u8 *)arg0 + 0x924) = arg1;
}

s32 func_001BB3E0(void *arg0, s32 arg1, s8 arg2) {
    *(s8 *)((u8 *)arg0 + 0xB84) = arg2;
    *(s32 *)((u8 *)arg0 + 0xA40) = arg1;
    *(s8 *)((u8 *)arg0 + 0xB85) = 3;
    return 0;
}

void *func_001BB140(void *volatile arg0) {
    func_00188210((u8 *)arg0 + 0x150);
    *(s8 *)((u8 *)arg0 + 0x147) = 1;
    *(s16 *)((u8 *)arg0 + 0x28) = 0;
    *(s32 *)((u8 *)arg0 + 0x2C) = 0;
    *(s32 *)((u8 *)arg0 + 0x30) = 0;
    *(s8 *)((u8 *)arg0 + 0x44) = 0;
    *(s16 *)((u8 *)arg0 + 0x144) = 0;
    *(s8 *)((u8 *)arg0 + 0x148) = 0;
    *(s8 *)((u8 *)arg0 + 0x14A) = 0;
    *(s32 *)((u8 *)arg0 + 0x14C) = 0;
    *(s8 *)((u8 *)arg0 + 0x14B) = 3;
    *(s8 *)((u8 *)arg0 + 0x149) = 1;
    func_00187780((u8 *)arg0 + 0x150);
    return (void *)arg0;
}

s32 func_001BB420(s32 dummy, void *p) {
    s32 a = *(u16 *)((u8 *)p + 0x10);
    s32 b = *(u16 *)((u8 *)p + 0x24);
    s32 c = *(u16 *)((u8 *)p + 0x12);
    s32 d = *(u16 *)((u8 *)p + 0x26);
    s32 ret = 1;
    if (a < 0x7000) {
        *(u16 *)((u8 *)p + 0x10) = 0x7000;
        ret = 0;
    } else if (a + b > 0x9000) {
        *(u16 *)((u8 *)p + 0x10) = 0x9000 - b;
        ret = 0;
    }
    if (c < 0x7A80) {
        *(u16 *)((u8 *)p + 0x12) = 0x7A80;
        ret = 0;
    } else if (c + d > 0x8580) {
        *(u16 *)((u8 *)p + 0x12) = 0x8580 - *(s16 *)((u8 *)p + 0x26);
        ret = 0;
    }
    return ret;
}
