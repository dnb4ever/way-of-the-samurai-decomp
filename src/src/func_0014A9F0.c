extern void func_001238A0(void *, s32);

void func_0014ABC0(void *arg0, s32 arg1) {
    void *p = *(void **)((u8 *)arg0 + 0x20);
    if (p != NULL && *(s32 *)((u8 *)p + 0x548) == arg1) {
        func_001238A0(p, 0);
    }
}

s32 D_002248E0;

void func_0014A9F0(s32 arg0) {
    D_002248E0 = arg0;
}