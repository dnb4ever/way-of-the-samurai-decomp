extern u8 D_00223C40[];
s32 func_001910D0(void *, s32);
void func_00190CB0(void *, s32);

void *func_00180510(void *arg0, void *arg1) {
    s32 v = *(s32 *)((char *)arg1 + 4);
    *(void **)((char *)arg0 + 4) = D_00223C40;
    *(s32 *)arg0 = v;
    return arg0;
}

s32 func_00190C80(void *arg0) {
    if (*(u8 *)((char *)arg0 + 4) == 0) {
        return 1;
    }
    return func_001910D0(arg0, *(s32 *)arg0);
}

void func_00191340(void *arg0) {
    func_00190CB0(arg0, *(s32 *)arg0);
}