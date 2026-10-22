void func_00117760(void *);
void func_001177A8(s32, s32);

typedef struct E {
    s32 f0;   /* 0x0 */
    s32 f4;   /* 0x4 */
    s32 f8;   /* 0x8 */
    s32 fC;   /* 0xC */
    s32 f10;  /* 0x10 */
} E;

s32 func_001EE4A0(E *p) {
    return p->fC == p->f10;
}

void func_001EE4C0(volatile E *p) {
    p->fC = 0;
    p->f8 = 0;
}

s32 func_001EE3C0(E *p) {
    if (p->fC == p->f10) {
        return 0;
    }
    return p->f0 + p->f8 * 0x195000;
}

void func_001EE410(E *p) {
    s32 idx;
    func_00117760(p);
    idx = p->f8;
    *(s32 *)((char *)p->f4 + idx * 0x4CE40) = 2;
    p->fC++;
    p->f8 = (p->f8 + 1) % p->f10;
    func_001177A8(idx * 0x2BF, 2);
}