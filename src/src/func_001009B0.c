extern s32 D_001FD210[];

void func_001009B0(void) {
    (*(void (**)(void)) &D_001FD210[0])();
}

s32 func_00166590(s32 x);
typedef struct Obj { void *p; s32 *rc; } Obj;

void func_001A3930(Obj *dst, s32 a1, s32 a2, Obj *src) {
    func_00166590(a1);
    dst->p = src->p;
    if (dst->p != 0) {
        dst->rc = src->rc;
        (*dst->rc)++;
    }
}