void bigfloat_unpack(s64 *src, void *dst);
void func_001DFD10(void *a, void *b);
void *bigfloat_add(void *a, void *b, void *out);
s64 bigfloat_pack(void *x);

void *func_001C5CE0(void *arg0, s64 arg1) {
    if (arg0 != NULL) {
        *(u8 **)((u8 *)arg0 + 0x3C) = D_00223F20;
        func_001E3D00(arg0, 0);
        if (((s64)(arg1 << 0x30) >> 0x30) > 0) {
            operator_delete(arg0);
        }
    }
    return arg0;
}

s64 func_001DFE30(s64 arg0, s64 arg1) {
    s64 x0;
    s64 x1;
    u8 d0[0x20];
    u8 d1[0x20];
    x0 = arg0;
    x1 = arg1;
    bigfloat_unpack(&x0, d0);
    bigfloat_unpack(&x1, d1);
    func_001DFD10(d0, d1);
}

s64 bigfloat_sub(s64 arg0, s64 arg1) {
    s64 x0;
    s64 x1;
    u8 d0[0x20];
    u8 d1[0x20];
    u8 res[0x20];
    x0 = arg0;
    x1 = arg1;
    bigfloat_unpack(&x0, d0);
    bigfloat_unpack(&x1, d1);
    ((s32 *)d1)[1] ^= 1;
    return bigfloat_pack(bigfloat_add(d0, d1, res));
}

typedef struct BF {
    s32 type;   /* 0x0 */
    s32 sign;   /* 0x4 */
    s32 exp;    /* 0x8 */
    s32 pad;    /* 0xc */
    s64 mant;   /* 0x10 */
    s64 pad18;  /* 0x18 */
} BF;

s64 int_to_double(s32 arg0) {
    BF v;
    s64 m;
    s32 exp;
    s64 limit;
    v.type = 3;
    v.sign = (u32)arg0 >> 31;
    if (arg0 == 0) {
        v.type = 2;
        return bigfloat_pack(&v);
    }
    v.exp = 0x3C;
    if (v.sign != 0) {
        if (arg0 == (s32)0x80000000) {
            return (s64)0xC1E0000000000000ULL;
        }
        v.mant = -arg0;
    } else {
        v.mant = arg0;
    }
    m = v.mant;
    limit = (s64)((u64)-1 >> 4);
    if ((u64)limit < (u64)m) {
        return bigfloat_pack(&v);
    }
    exp = v.exp;
    do {
        m <<= 1;
        exp--;
    } while ((u64)m <= (u64)limit);
    v.exp = exp;
    v.mant = m;
    return bigfloat_pack(&v);
}