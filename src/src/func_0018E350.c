extern u8 D_00223D40[];
extern u8 list_iter_vtable_223D70[];

typedef struct { void *f0; void *f4; } Pair;

Pair *func_0018E220(Pair *arg0, void *arg1) {
    volatile Pair *p = arg0;
    p->f4 = (void *)D_00223D40;
    p->f0 = arg1;
    return arg0;
}

Pair *func_0018E350(Pair *arg0, void *arg1) {
    volatile Pair *p = arg0;
    p->f4 = (void *)list_iter_vtable_223D70;
    p->f0 = arg1;
    return arg0;
}

s32 D_00224A20;
void func_0018E3E0(s32 arg0) {
    D_00224A20 = arg0;
}

s32 D_00224A18;
void func_0018E3F0(s32 arg0) {
    D_00224A18 = arg0;
}
