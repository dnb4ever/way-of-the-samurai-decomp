s32 func_0010F980(void *dst, void *src, u32 len) {
    s32 *d = (s32 *)dst;
    s32 *s = (s32 *)src;
    u32 count = len >> 2;
    u32 i = 0;
    if (count != 0) {
        do {
            s32 t = *s;
            i++;
            s++;
            *d = t;
            d++;
        } while (i < count);
    }
    return 0;
}

void func_0010FF38(s32 arg0, s8 arg1) {
    s32 buf[2];
    s32 v1 = arg1;
    buf[0] = arg0;
    buf[1] = v1;
    func_0010F090(-7, buf);
}

void func_0010FF68(s32 arg0) {
    s32 buf;
    void *p;
    buf = arg0;
    p = &buf;
    func_0010F090(-8, p);
}

void func_0010FF90(s32 arg0) {
    s32 buf;
    void *p;
    buf = arg0;
    p = &buf;
    func_0010F090(-9, p);
}