extern u8 D_00223B80[];
s32 D_002249F8;

void func_001886A0(s32 arg0) {
    D_002249F8 = arg0;
}

void *func_00174170(void *arg0, s32 *arg1) {
    s32 t = arg1[1];
    *(u8 **)((u8 *)arg0 + 4) = D_00223B80;
    *(s32 *)arg0 = t;
    return arg0;
}
