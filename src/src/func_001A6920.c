extern u8 D_00C25A04[];

void func_001A6920(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 0;
    *(s32 *)((u8 *)arg0 + 0x0) = 0;
    *(s32 *)((u8 *)arg0 + 0xC) = 0;
    *(s32 *)((u8 *)arg0 + 0x10) = 0;
    *(s32 *)((u8 *)arg0 + 0x14) = 0;
    *(s32 *)((u8 *)arg0 + 0x28) = 0;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0;
}

s32 func_001A5190(void *arg0) {
    *(s32 *)(D_00C25A04 + *(s32 *)((u8 *)arg0 + 0x10) * 0x270) = 0;
    return 1;
}