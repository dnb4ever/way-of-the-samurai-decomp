extern u8 iter_vtable_19b[];
typedef struct { void *f0; void *f4; } Pair;

Pair *func_0019BC50(Pair *arg0, void *arg1) {
    volatile Pair *p = arg0;
    p->f4 = (void *)iter_vtable_19b;
    p->f0 = arg1;
    return arg0;
}

void *func_0019C000(void *arg0, s32 arg1, s32 arg2) {
    u8 *rec = (u8 *)arg0 + arg1 * 0x1F0;
    s32 i;

    if (arg2 < 0) {
        i = *(s32 *)(rec + 0x1D4);
        arg2 = 1;
    } else {
        i = 0;
        if (arg2 == 0) {
            *(s32 *)(rec + 0x1D4) = 0;
            return NULL;
        }
    }
    while (i < *(u16 *)((u8 *)arg0 + 0xFA00)) {
        if (rec[0x1B4 + (i >> 3)] & (1 << (i & 7))) {
            if (--arg2 == 0) {
                *(s32 *)(rec + 0x1D4) = i + 1;
                return (u8 *)arg0 + i * 0x1F0;
            }
        }
        i++;
    }
    return NULL;
}

s32 func_0019C0B0(s32 arg0, s32 arg1) {
    if (arg1 >= 0 && arg1 < 0x80) {
        return arg0 + arg1 * 0x1F0;
    }
    return 0;
}

s32 func_0019C0F0(s32 arg0, s32 arg1) {
    u32 *p;
    if (arg1 < 0 || arg1 >= 0x80) {
        return -1;
    }
    p = (u32 *)(arg0 + arg1 * 0x1F0 + 0x198);
    *p |= 0x80000000;
    return arg1;
}
