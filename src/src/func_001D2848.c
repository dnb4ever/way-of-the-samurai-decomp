typedef char *va_list;

void func_001D5860(void *, void *, s32, void *);

void func_001D2848(void *arg0, s32 arg1, ...) {
    va_list ap;
    ap = (va_list)((char *)__builtin_next_arg(arg1) - 0x30);
    func_001D5860(arg0, *(void **)((u8 *)arg0 + 8), arg1, ap);
}
