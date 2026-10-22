s64 bigfloat_pack(void *x);

typedef struct BF {
    s32 type;
    s32 sign;
    s32 exp;
    s32 pad;
    s64 mant;
    s64 pad18;
} BF;

s64 int_to_double(s32 arg0) {
    BF v;
    s64 m;
    s32 exp;
    v.type = 3;
    v.sign = (u32)arg0 >> 31;
    if (arg0 == 0) {
        v.type = 2;
    } else {
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
        if ((u64)m <= ((u64)-1 >> 4)) {
            exp = v.exp;
            do {
                s64 t = m << 1;
                exp--;
                m = t;
            } while ((u64)m <= ((u64)-1 >> 4));
            v.exp = exp;
            v.mant = m;
        }
    }
    return bigfloat_pack(&v);
}