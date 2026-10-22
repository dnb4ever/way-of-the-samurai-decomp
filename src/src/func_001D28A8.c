extern void *D_00210F40[];

typedef char *va_list;

void doprnt_core(void *, s32, void *);

void func_001D28A8(s32 arg0, ...) {
    va_list ap;
    void *p = D_00210F40[0];
    *(void **)((u8 *)(*(void **)((u8 *)p + 8)) + 0x54) = p;
    ap = (va_list)((char *)__builtin_next_arg(arg0) - 0x38);
    doprnt_core(*(void **)((u8 *)p + 8), arg0, ap);
}