extern u8 D_0022A5E8[16];
extern u8 D_0022A5EC[16];
extern u8 D_0022A5F0[16];

void func_0011B240(s32 arg0) {
    s32 *base = (s32 *)(arg0 | 0x20000000);
    s32 *p1, *p2, *p3;
    p1 = *(s32 **)D_0022A5E8; if (p1) *p1 = base[0];
    p2 = *(s32 **)D_0022A5EC; if (p2) *p2 = base[1];
    p3 = *(s32 **)D_0022A5F0; if (p3) *p3 = base[0x24];
}