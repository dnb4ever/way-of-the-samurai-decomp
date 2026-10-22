extern u8 vtable_00223A10[];
typedef struct { void *f0; void *f4; } S0015F260;
S0015F260 *func_0015F260(S0015F260 *arg0, void *arg1) {
    volatile S0015F260 *p = arg0;
    p->f4 = (void *)vtable_00223A10;
    p->f0 = arg1;
    return arg0;
}