s32 func_00150900(s32);
s32 func_00168B50(s32);
s32 func_00168B60(s32);
s32 strcmp(s32, s32);

void func_00168A80(void *arg0) {
    *(void **)((u8 *)arg0 + 0) = arg0;
    *(s32 *)((u8 *)arg0 + 8) = 0;
    *(void **)((u8 *)arg0 + 4) = arg0;
}

s32 func_00168B00(s32 arg0, s32 arg1) {
    return strcmp(func_00168B50(arg0 + 0x24),
                         func_00168B60(func_00150900(arg1 + 0x24))) == 0;
}