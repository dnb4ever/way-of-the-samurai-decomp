void func_00105548(s32 *arg0, s32 arg1) {
    arg1 *= 4;
    *arg0 += arg1;
}

void func_001055E0(s32 **arg0, s32 arg1) {
    s32 *p = *arg0;
    *p++ = arg1;
    *arg0 = p;
}

void func_001054A0(void *arg0) {
    u8 *f0 = *(u8 **)((u8 *)arg0 + 0x0);
    u64 *p = *(u64 **)((u8 *)arg0 + 0x14);
    u64 hdr = *p;
    s32 count = ((s32)f0 - (s32)p) >> 3;
    u64 kind = (hdr >> 58) & 3;
    u32 base = (u32)(count - 2);
    if (kind != 1) base = base >> 1;
    if (kind != 2) {
        u32 d = (u32)(hdr >> 60);
        if (d == 0) d = 16;
        base = (base + d - 1) / d;
    }
    *(void **)((u8 *)arg0 + 0x14) = NULL;
    *p = hdr + base;
    {
        u8 *cur = f0;
        if (((u32)cur & 0xC) != 0) {
            do {
                *(u32 *)cur = 0;
                cur += 4;
            } while (((u32)cur & 0xC) != 0);
        }
        *(u8 **)((u8 *)arg0 + 0x0) = cur;
    }
}