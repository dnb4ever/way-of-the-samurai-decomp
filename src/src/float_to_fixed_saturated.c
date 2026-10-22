s32 float_to_fixed_saturated(f32 x) {
    s32 info[4];
    f32 fx;
    s32 exp;
    fx = x;
    float_decompose((u32 *)&fx, info);
    if ((info[0] ^ 2) == 0) return 0;
    exp = info[2];
    if (exp < 0) return 0;
    if (exp >= 0x1F) {
        return info[1] ? (s32)0x80000000 : 0x7FFFFFFF;
    }
    {
        u32 m = (u32)info[3] >> (0x1E - exp);
        return info[1] ? -(s32)m : (s32)m;
    }
}

void float_decompose(u32 *fp, s32 *out);