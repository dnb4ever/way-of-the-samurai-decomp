extern u8 ref_handle_vtable[];
extern u8 D_00223B80[];

typedef struct { void *f0; void *f4; } S00165210;

s32 func_00165210(s32 **arg0) {
    return **arg0;
}

s32 func_00165220(void **arg0) {
    return *(s32 **)((u8 *)arg0 + 0xC);
}

S00165210 *func_00165240(S00165210 *arg0, void *arg1) {
    volatile S00165210 *p = arg0;
    p->f4 = (void *)ref_handle_vtable;
    p->f0 = arg1;
    return arg0;
}

S00165210 *func_00165170(S00165210 *arg0, void *arg1) {
    volatile S00165210 *p = arg0;
    p->f4 = (void *)D_00223B80;
    p->f0 = arg1;
    return arg0;
}