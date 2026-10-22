extern u8 D_002239E0[];
typedef struct { void *f0; void *f4; } S0015F3B0;
S0015F3B0 *dlist_end(S0015F3B0 *arg0, void *arg1) {
    volatile S0015F3B0 *p = arg0;
    p->f4 = (void *)D_002239E0;
    p->f0 = arg1;
    return arg0;
}