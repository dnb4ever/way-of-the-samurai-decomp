void func_0010D708(void *arg0);
void func_0010D7F8(void *arg0);
void func_0010D270(void *arg0, s32 a, s32 b, s32 c);

void func_0010CFC8(void *arg0) {
    void *p = *(void **)((u8 *)arg0 + 0x40);
    *(s32 *)((u8 *)p + 0x0) = 0;
    *(s32 *)((u8 *)p + 0x4) = 0;
    *(s32 *)((u8 *)p + 0x8) = 0;
    *(s32 *)((u8 *)arg0 + 0x8) = 0;
    *(s32 *)((u8 *)p + 0xAC) = 0;
    *(s32 *)((u8 *)p + 0x80) = -1;
    func_0010D7F8(p);
    *(s32 *)((u8 *)p + 0x118) = 0;
    func_0010D708(p);
}

void func_0010CF70(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *p = *(s32 **)((u8 *)arg0 + 0x40);
    *(s32 *)((u8 *)p + 0x94) = arg1;
    *(s32 *)((u8 *)p + 0x98) = arg2;
    *(s32 *)((u8 *)p + 0x9C) = arg3;
}

void func_0010CF20(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *p = *(void **)((u8 *)arg0 + 0x40);
    s32 v5 = (arg1 & 0x0FFFFFFF) | 0x20000000;
    *(s32 *)((u8 *)p + 0xE0) = arg3 * 16;
    *(s32 *)((u8 *)p + 0xD8) = v5;
    *(s32 *)((u8 *)p + 0xE4) = arg2 * arg3;
    *(s32 *)((u8 *)p + 0xDC) = arg2 * 16;
    *(s32 *)((u8 *)p + 0xB0) = 0;
    func_0010D270(arg0, v5, arg2 * 16, arg3 * 16);
}

s32 func_0010D070(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *base = *(s32 **)((u8 *)arg0 + 0x40);
    s32 off = arg1 * 8;
    s32 *p = (s32 *)((u8 *)base + 0xC + off);
    s32 ret;
    *(s32 *)((u8 *)base + off + 0x10) = arg3;
    ret = *p;
    *p = arg2;
    return ret;
}