void func_00110818(void *a, void *b);

void func_00110E78(void *arg0, void *arg1) {
    s32 idx = *(s32 *)((u8 *)arg0 + 0x10);
    s32 *base = *(s32 **)((u8 *)arg1 + 0x1C);
    s32 val = *(s32 *)((u8 *)arg0 + 0x14);
    base[idx] = val;
}

void func_00110DE0(void *arg0, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, arg0);
    func_00110818(arg0, (void *)ap);
}