void *func_001CE180(void *, s32, s32);
void *Balloc(void *, s32);

s32 hi0bits(s32 arg0) {
    s32 var_2, var_4, var_5;
    var_4 = arg0;
    var_5 = 0;
    if (!(var_4 & 0xFFFF0000)) { var_5 = 0x10; var_4 <<= 16; }
    if (!(var_4 & 0xFF000000)) { var_5 += 8; var_4 <<= 8; }
    if (!(var_4 & 0xF0000000)) { var_5 += 4; var_4 <<= 4; }
    if (!(var_4 & 0xC0000000)) { var_5 += 2; var_4 <<= 2; }
    var_2 = var_5;
    if (var_4 >= 0) {
        var_2 = 0x20;
        if (var_4 & 0x40000000) { var_2 = var_5 + 1; }
    }
    return var_2;
}

s32 lo0bits(u32 *arg0) {
    s32 var_2;
    s32 var_3;
    u32 var_5;

    var_5 = *arg0;
    if (var_5 & 7) {
        var_2 = 0;
        if (!(var_5 & 1)) {
            if (var_5 & 2) {
                *arg0 = var_5 >> 1;
                return 1;
            }
            *arg0 = var_5 >> 2;
            return 2;
        }
        return var_2;
    }
    var_3 = 0;
    if (!(var_5 & 0xFFFF)) {
        var_3 = 0x10;
        var_5 = var_5 >> 0x10;
    }
    if (!(var_5 & 0xFF)) {
        var_3 += 8;
        var_5 = var_5 >> 8;
    }
    if (!(var_5 & 0xF)) {
        var_3 += 4;
        var_5 = var_5 >> 4;
    }
    if (!(var_5 & 3)) {
        var_3 += 2;
        var_5 = var_5 >> 2;
    }
    if (!(var_5 & 1)) {
        var_5 = var_5 >> 1;
        var_3 += 1;
        if (var_5 == 0) {
            return 0x20;
        }
        *arg0 = var_5;
        var_2 = var_3;
    } else {
        *arg0 = var_5;
        var_2 = var_3;
    }
    return var_2;
}

void *func_001D1C80(void *arg0, s32 arg1) {
    void *r;
    r = Balloc(arg0, 1);
    *(s32 *)((u8 *)r + 0x14) = arg1;
    *(s32 *)((u8 *)r + 0x10) = 1;
    return r;
}

void *Balloc(void *arg0, s32 arg1) {
    void **base;
    void *node;

    base = *(void ***)((u8 *)arg0 + 0x4C);
    if (base == NULL) {
        base = func_001CE180(arg0, 4, 0x10);
        *(void ***)((u8 *)arg0 + 0x4C) = base;
        if (base == NULL) {
            return base;
        }
    }
    node = base[arg1];
    if (node != NULL) {
        base[arg1] = *(void **)node;
    } else {
        s32 cnt = 1 << arg1;
        node = func_001CE180(arg0, 1, cnt * 4 + 0x14);
        if (node == NULL) {
            return node;
        }
        *(s32 *)((u8 *)node + 0x4) = arg1;
        *(s32 *)((u8 *)node + 0x8) = cnt;
    }
    *(s32 *)((u8 *)node + 0x10) = 0;
    *(s32 *)((u8 *)node + 0xC) = 0;
    return node;
}