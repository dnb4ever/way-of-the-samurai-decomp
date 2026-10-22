extern u8 D_00223750[];
extern u8 D_00223970[];

void func_001440A0(void *arg0, s8 arg1, s32 arg2) {
    *(s32 *)((char *)arg0 + 0x410) = arg2;
    *(s8 *)((char *)arg0 + 0x435) = arg1;
}

void *func_001440B0(void *arg0) {
    *(u8 * volatile *)((char *)arg0 + 0xC) = D_00223750;
    *(u8 * volatile *)((char *)arg0 + 0xC) = D_00223970;
    *(s16 *)((char *)arg0 + 0x2) = 1;
    *(s8 *)((char *)arg0 + 0x436) = 0;
    *(s8 *)((char *)arg0 + 0x437) = 0;
    *(s8 *)((char *)arg0 + 0x438) = 0;
    *(s8 *)((char *)arg0 + 0x435) = 1;
    *(s8 *)((char *)arg0 + 0x439) = 0;
    *(s32 *)((char *)arg0 + 0x410) = 0;
    *(s8 *)((char *)arg0 + 0x43A) = 0;
    return arg0;
}

void func_001443B0(s32 a0, s16 *out, s32 *in) {
    s32 v;
    v = (((in[0] + in[4]) >> 1) - 0x6C00) & 0xFFFF;
    if (v < 0) {
        out[0] = 0;
    } else if (v >= 0x2801) {
        out[0] = 0x2800;
    } else {
        out[0] = v;
    }
    v = (((in[1] + in[5]) >> 1) - 0x7900) & 0xFFFF;
    if (v < 0) {
        out[1] = 0;
    } else if (v >= 0xE01) {
        out[1] = 0xE00;
    } else {
        out[1] = v;
    }
}