void func_0010D198(void *arg0, s64 arg1) {
    s8 *p = *(s8 **)((s8 *)arg0 + 0x40);
    *(s64 *)(p + 0xF0) = arg1;
    *(s32 *)(p + 0xF8) = 1;
}
