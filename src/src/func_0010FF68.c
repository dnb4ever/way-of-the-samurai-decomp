s32 func_0010F980(void *dst, void *src, u32 len) {
    s32 *d = (s32 *)dst;
    s32 *s = (s32 *)src;
    u32 count = len >> 2;
    u32 i;
    if (count != 0) {
        i = 0;
        do {
            *d = *s;
            s++;
            d++;
            i++;
        } while (i < count);
    }
    return 0;
}

void func_0010FF38(s32 arg0, s8 arg1) {
    s32 buf[2];
    buf[0] = arg0;
    buf[1] = arg1;
    func_0010F090(-7, buf);
}

void func_0010FF68(s32 arg0) {
    s32 buf;
    buf = arg0;
    func_0010F090(-8, &buf);
}

void func_0010FF90(s32 arg0) {
    s32 buf;
    buf = arg0;
    func_0010F090(-9, &buf);
}