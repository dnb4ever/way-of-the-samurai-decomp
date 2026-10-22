void func_0010FFB8(s32 arg0) {
    s32 v = arg0;
    func_0010F090(0x10, &v);
}

typedef struct RingBuf {
    s32 f0;
    s32 f4;
    u8 *f8;
    u8 *fC;
} RingBuf;
extern RingBuf D_2256C0;

RingBuf *func_0010FFE0(s32 arg0) {
    RingBuf *p = &D_2256C0;
    u8 *q = (u8 *)p + 0x10;
    p->f0 = arg0;
    p->f8 = q;
    p->f4 = 0;
    p->fC = q;
    return p;
}

void func_00110008(RingBuf *p) {
    u8 *n;
    p->f4 = p->f4 + 1;
    n = p->fC + 1;
    p->fC = n;
    if (n == (u8 *)p + p->f0 + 0x10) {
        p->fC = (u8 *)p + 0x10;
    }
}