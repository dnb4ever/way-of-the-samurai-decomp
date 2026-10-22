void *vec_negate3_copyw(void *dst, void *src) {
    volatile f32 *d = dst;
    volatile f32 *s = src;
    f32 t;
    t = -s[0]; d[0] = t;
    t = -s[1]; d[1] = t;
    t = -s[2]; d[2] = t;
    t = s[3]; d[3] = t;
    return dst;
}

void *vec_negate3_inplace(void *arg0) {
    volatile f32 *p = arg0;
    f32 t;
    t = -p[0]; p[0] = t;
    t = -p[1]; p[1] = t;
    t = -p[2]; p[2] = t;
    return arg0;
}

void *func_0015AF90(void *arg0, s32 arg1) {
    u8 *p = arg0;
loop:
    p[1] = arg1;
    {
        s32 v = *(u16 *)(p + 0x28);
        if (v >= 2) {
            p += 0x30;
            goto loop;
        }
    }
    return p;
}
