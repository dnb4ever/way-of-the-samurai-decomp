extern int func_00124080();
s32 func_00132280(void *obj, s32 id, s32 code, s32 arg3);
void func_00105CE0(f32 *v);
void func_00105AF0(f32 *a, f32 *b);
void func_00105A30(f32 *a, s32 b, f32 *c);
void func_00131BF0(void *obj, f32 *v, s32 *p);

s32 func_0012D360(void *arg0) {
    return func_00132280(arg0, 2, 0x3070, 0) != 0;
}

s32 func_0012D390(void *arg0) {
    s32 flags = *(s32 *)((char *)arg0 + 0x3C8);
    if (flags & 0x10040000) {
        *(s32 *)((char *)arg0 + 0x3C8) = flags & 0xBFFFFFFF;
        return 1;
    }
    func_00124080();
    if (func_00132280(arg0, 2, 0x6D, 0)) {
        return 1;
    }
    *(s32 *)((char *)arg0 + 0x3C8) = *(s32 *)((char *)arg0 + 0x3C8) & ~0xF;
    *(s32 *)((char *)arg0 + 0x518) = 0x22;
    return 0;
}

s32 func_0012D2A0(void *arg0) {
    f32 sp20;
    s32 sp24;
    f32 sp28;
    s32 sp3C;

    func_00105CE0(&sp20);
    sp24 = 0;
    sp20 = -sp20;
    sp28 = -sp28;
    func_00105AF0(&sp20, &sp20);
    func_00105A30(&sp20, *(s32 *)((char *)arg0 + 0x388), &sp20);
    sp3C = 0x3C00ADFD;
    func_00131BF0(arg0, &sp20, &sp3C);
    func_00124080(arg0);
    if (func_00132280(arg0, 2, 0x1000006E, 0)) {
        return 1;
    }
    *(s32 *)((char *)arg0 + 0x518) = 0x2C;
    return 0;
}