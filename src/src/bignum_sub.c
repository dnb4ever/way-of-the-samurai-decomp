typedef struct BN {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 sign;   /* 0xC */
    s32 len;    /* 0x10 */
    u32 data[1];/* 0x14 */
} BN;

BN *Balloc(s32, s32);
s32 bignum_cmp(BN *, BN *);

BN *bignum_sub(s32 arg0, BN *a, BN *b) {
    s32 cmp;
    BN *res;
    u32 *pa, *pb, *pr, *enda, *endb;
    s32 borrow, len;
    s32 dlo, dhi;

    cmp = bignum_cmp(a, b);
    if (cmp == 0) {
        res = Balloc(arg0, 0);
        res->len = 1;
        res->data[0] = 0;
        return res;
    }
    if (cmp < 0) {
        res = a;
        a = b;
        b = res;
        cmp = 1;
    } else {
        cmp = 0;
    }
    res = Balloc(arg0, a->unk4);
    pa = a->data;
    pb = b->data;
    res->sign = cmp;
    pr = res->data;
    borrow = 0;
    len = a->len;
    enda = pa + len;
    endb = pb + b->len;
    do {
        u32 x = *pa++;
        u32 y = *pb++;
        dlo = ((x & 0xFFFF) - (y & 0xFFFF)) + borrow;
        borrow = dlo >> 16;
        *(s16 *)pr = (s16)dlo;
        dhi = ((x >> 16) - (y >> 16)) + borrow;
        *((s16 *)pr + 1) = (s16)dhi;
        pr++;
        borrow = dhi >> 16;
    } while (pb < endb);
    if (pa < enda) {
        do {
            u32 x = *pa++;
            dlo = (x & 0xFFFF) + borrow;
            borrow = dlo >> 16;
            *(s16 *)pr = (s16)dlo;
            dhi = (x >> 16) + borrow;
            *((s16 *)pr + 1) = (s16)dhi;
            pr++;
            borrow = dhi >> 16;
        } while (pa < enda);
    }
    pr--;
    if (*pr == 0) {
        do {
            pr--;
            len--;
        } while (*pr == 0);
    }
    res->len = len;
    return res;
}