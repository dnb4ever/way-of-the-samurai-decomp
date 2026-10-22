/* ---- func_001D17E0 ---- */
void func_0010EC80(s32);
extern s32 D_001FE494[];
extern s32 D_002113A8[];
extern s32 D_002113AC[];

void func_001D17E0(void) {
    if (--D_002113AC[0] == 0) {
        D_002113A8[0] = -1;
        func_0010EC80(D_001FE494[0]);
    }
}

/* ---- doprnt_core ---- */
extern void *D_00210F40[];
void func_001CFA60(void *, void *);
void func_001D5860(void *, void *, s32, s32);

void doprnt_core(void *arg0, s32 arg1, s32 arg2) {
    void *var = *(void **)((u8 *)arg0 + 0x54);
    if (var == NULL) {
        var = D_00210F40[0];
        *(void **)((u8 *)arg0 + 0x54) = var;
    }
    if (*(s32 *)((u8 *)var + 0x38) == 0) {
        func_001CFA60(var, arg0);
        var = *(void **)((u8 *)arg0 + 0x54);
    }
    func_001D5860(var, arg0, arg1, arg2);
}

/* ---- bignum_cmp ---- */
s32 bignum_cmp(void *arg0, void *arg1) {
    s32 a = *(s32 *)((u8 *)arg0 + 0x10);
    s32 n = *(s32 *)((u8 *)arg1 + 0x10);
    s32 d = a - n;
    u32 *abase, *pa, *pb;
    if (d != 0) {
        return d;
    }
    abase = (u32 *)((u8 *)arg0 + 0x14);
    pa = abase + n;
    pb = (u32 *)((u8 *)arg1 + 0x14) + n;
    pa--;
    for (;;) {
        u32 av, bv;
        s32 result;
        pb--;
        av = *pa;
        bv = *pb;
        result = (av < bv) ? -1 : 1;
        if (av != bv) {
            return result;
        }
        if (!(abase < pa)) {
            return 0;
        }
        pa--;
    }
}
