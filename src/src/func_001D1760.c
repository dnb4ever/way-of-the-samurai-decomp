extern s32 D_C290C8[];
s32 func_0010F438(s32, s32);

void _kill_r(s32 *arg0, s32 arg1, s32 arg2) {
    D_C290C8[0] = 0;
    if (func_0010F438(arg1, arg2) == -1) {
        if (D_C290C8[0] != 0) {
            *arg0 = D_C290C8[0];
        }
    }
}

extern s32 D_001FE494[];
extern s32 D_002113A8[];
extern s32 D_002113AC[];
s32 func_0010EB50(void);
void func_0010ECA0(s32);

void func_001D1760(void) {
    s32 r = func_0010EB50();
    if (D_002113A8[0] == r) {
        D_002113AC[0] += 1;
    } else {
        func_0010ECA0(D_001FE494[0]);
        D_002113A8[0] = r;
        D_002113AC[0] += 1;
    }
}

s32 func_001DCAC0(void *arg0, s32 arg1) {
    u16 *p;
    u16 *q;
    s32 count;
    s32 i;
    s32 key;
    s32 base;
    s32 idx;
    base = *(s32 *)0x8DCB20;
    idx = *(s8 *)((s8 *)arg0 + 0xE);
    p = *(u16 **)(idx * 4 + base + 0x5AC);
    if (p == NULL) {
        return 0;
    }
    count = *p;
    q = p + 1;
    if (count <= 0) {
        return 0;
    }
    key = arg1 & 0xFFFF;
    i = 0;
    do {
        u16 v = *q;
        if (key < v) {
            return 0;
        }
        if (v == key) {
            return 1;
        }
        i++;
        q++;
    } while (i < count);
    return 0;
}