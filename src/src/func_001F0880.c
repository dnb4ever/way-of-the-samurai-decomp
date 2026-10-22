extern u8 D_00224010[];
typedef struct { void *f0; void *f4; } S00134970;
S00134970 *func_001F0880(S00134970 *arg0, void *arg1) {
    volatile S00134970 *p = arg0;
    p->f4 = (void *)D_00224010;
    p->f0 = arg1;
    return arg0;
}