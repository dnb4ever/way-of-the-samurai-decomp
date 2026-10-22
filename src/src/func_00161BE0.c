s32 func_00161BE0(void *arg0, void *arg1) {
    return *(s32 *)((u8 *)arg1 + 0x4C);
}

void func_00161FB0(void *arg0, void *arg1, s32 arg2) {
    *(s32 *)((u8 *)arg1 + 0x40) = 2;
    *(s32 *)((u8 *)arg1 + 0x48) = 0x3E99999A;
    *(s32 *)((u8 *)arg1 + 0x50) = arg2;
}