extern u8 D_00223C80[];
typedef struct { void *f0; void *f4; } S0017F630;
S0017F630 *func_0017F630(S0017F630 *arg0, void *arg1) {
    arg1 = ((void * volatile *)arg1)[1];
    arg0->f4 = (void *)D_00223C80;
    arg0->f0 = arg1;
    return arg0;
}
