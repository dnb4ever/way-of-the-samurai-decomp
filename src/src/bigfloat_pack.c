u64 bigfloat_pack(void *arg0, s64 arg1, u64 arg2) {
    u32 cls = *(u32 *)arg0;
    u64 mant = *(u64 *)((u8 *)arg0 + 0x10);
    s32 sign = *(s32 *)((u8 *)arg0 + 4);
    s32 exp;
    s32 var_7 = 0;

    if (cls < 2) {
        var_7 = 0x7FF;
        mant |= (u64)0x8000 << 36;
    } else if ((cls ^ 4) == 0) {
        var_7 = 0x7FF;
        mant = 0;
    } else if ((cls ^ 2) == 0) {
        mant = 0;
    } else if (mant != 0) {
        exp = *(s32 *)((u8 *)arg0 + 8);
        if (exp < -0x3FE) {
            mant = 0;
        } else if (exp >= 0x400) {
            var_7 = 0x7FF;
            mant = 0;
        } else {
            var_7 = exp + 0x3FF;
            mant >>= 8;
        }
    }

    arg2 &= (u64)0xFFF0 << 48;
    arg2 |= mant & ((u64)-1 >> 12);
    arg2 &= 0x800FFFFFFFFFFFFFULL;
    arg2 |= (u64)(var_7 & 0x7FF) << 52;
    arg2 &= ((u64)-1 >> 1);
    return arg2 | ((u64)sign << 63);
}