extern u8 D_00223B80[];
typedef struct { void *f0; void *f4; } S00174150;
S00174150 *func_00174150(S00174150 *arg0, void *arg1) {
    volatile S00174150 *p = arg0;
    p->f4 = (void *)D_00223B80;
    p->f0 = arg1;
    return arg0;
}

s32 func_001747B0(s32 **arg0) {
    s32 *p = *arg0;
    s32 r = (p != 0);
    if (r) {
        r = (p[3] == 1);
    }
    return r;
}