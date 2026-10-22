extern u8 D_00213398[];
extern u8 D_002133B0[];
extern u8 D_002234A0[];
extern u8 D_002234B0[];
extern s32 (*D_001FD200)(void);
extern s8 D_001FD208;
void func_00102260(void *a, void *b, void *c);
s32 func_001D0918(void *arg0);

void func_00100460(void *arg0)
{
    operator_delete(arg0);
}

void *func_001005C0(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        *(void **)arg0 = D_002234B0;
        *(void **)arg0 = D_002234A0;
        if (arg1 > 0) {
            operator_delete(arg0);
        }
    }
    return arg0;
}

u32 *func_00100620(void)
{
    return (u32 *)D_00213398;
}

s32 func_00100630(void *arg0)
{
    void *sp3C;
    s32 r;

    if (arg0 == NULL) {
        arg0 = (void *)1;
    }
    for (;;) {
        r = func_001D0918(arg0);
        if (r != 0) {
            return r;
        }
        if (D_001FD200 != NULL) {
            D_001FD200();
            continue;
        }
        if (D_001FD208 != 0) {
            sp3C = D_002234B0;
            func_00102260(D_002133B0, &sp3C, (void *)func_001005C0);
            continue;
        }
        return 0;
    }
}