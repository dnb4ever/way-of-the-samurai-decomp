extern u8 D_00223C80[];
extern u8 D_00223C40[];
typedef struct { void *f0; void *f4; } Pair;

Pair *func_0017F500(Pair *arg0, void *arg1) {
    volatile Pair *p = arg0;
    p->f4 = (void *)D_00223C80;
    p->f0 = arg1;
    return arg0;
}

Pair *func_001803C0(Pair *arg0, void *arg1) {
    volatile Pair *p = arg0;
    p->f4 = (void *)D_00223C40;
    p->f0 = arg1;
    return arg0;
}