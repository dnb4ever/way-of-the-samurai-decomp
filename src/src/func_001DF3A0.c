typedef struct RP { void *ptr; s32 *refcount; } RP;

void func_00150120(RP *dst, RP *src) {
    void *tmp;
    s32 *rc;
    tmp = src->ptr;
    ((volatile RP *)dst)->ptr = tmp;
    tmp = ((volatile RP *)dst)->ptr;
    if (tmp != NULL) {
        rc = src->refcount;
        ((volatile RP *)dst)->refcount = rc;
        rc = ((volatile RP *)dst)->refcount;
        (*rc)++;
    }
}

void float_decompose(u32 *fp, s32 *out) {
    u32 bits = *fp;
    u32 exp = (bits >> 23) & 0xFF;
    out[1] = bits >> 31;
    if (exp == 0) {
        out[0] = 2;
        return;
    }
    out[2] = exp - 0x7F;
    out[0] = 3;
    out[3] = ((bits & 0x7FFFFF) << 7) | 0x40000000;
}

s32 float_to_fixed_saturated(f32 x) {
    s32 info[4];
    f32 fx;
    s32 r;
    fx = x;
    float_decompose((u32 *)&fx, info);
    r = 0;
    if (info[0] != 2) {
        if (info[2] >= 0) {
            if (info[2] < 0x1F) {
                u32 m = (u32)info[3] >> (0x1E - info[2]);
                r = info[1] ? -(s32)m : (s32)m;
            } else {
                r = info[1] ? (s32)0x80000000 : 0x7FFFFFFF;
            }
        }
    }
    return r;
}

void func_001DF208(void *arg0);

void func_001DF3A0(s32 a0, s32 a1, s32 a2, s32 a3) {
    s32 buf[4];
    buf[0] = a0;
    buf[1] = a1;
    buf[2] = a2;
    buf[3] = a3;
    func_001DF208(buf);
}