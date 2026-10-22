void *func_00104E60(void *arg0) {
    s32 *p = *(s32 **)arg0;
    s32 *q = *(s32 **)((u8 *)arg0 + 8);
    s32 d;
    if (((s32)p & 0xC) != 0) {
        do {
            *(volatile s32 *)p = 0;
            p++;
        } while (((s32)p & 0xC) != 0);
    }
    d = (s32)((u8 *)p - (u8 *)q);
    if (q != 0) {
        s32 t = (d >> 4) - 1;
        *q += t;
    }
    *(s32 * volatile *)((u8 *)arg0 + 8) = 0;
    *(s32 * volatile *)arg0 = p;
    return p;
}