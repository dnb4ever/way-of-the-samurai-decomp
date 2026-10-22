/* --- func_0018E9E0 --- */
void *func_0018E9E0(void *arg0) {
    void *t;
    *(void **)((u8 *)arg0 + 0x4) = arg0;
    t = (u8 *)arg0 + 0xC;
    *(void **)((u8 *)arg0 + 0x0) = arg0;
    *(s32 *)((u8 *)arg0 + 0x8) = 0;
    *(void **)((u8 *)arg0 + 0x10) = t;
    *(void **)((u8 *)arg0 + 0xC) = t;
    *(s32 *)((u8 *)arg0 + 0x14) = 0;
    *(s32 *)((u8 *)arg0 + 0x20) = 0;
    *(s32 *)((u8 *)arg0 + 0x18) = 0;
    *(s32 *)((u8 *)arg0 + 0x24) = 0;
    *(s32 *)((u8 *)arg0 + 0x28) = 0;
    *(s8 *)((u8 *)arg0 + 0x34) = 0;
    return arg0;
}

/* --- func_0018EE80 --- */
void func_00105D58(void *);
void func_00105DF8(void *, f32, void *);
void func_00105CE0(void *, s32);
void *func_0018EA20(s32, s32, void *);
void func_0019CB60(s32, s32, s32);

void func_0018EE80(s32 arg0, s32 arg1, s32 arg2) {
    u8 sp40[0x30];
    u8 sp70[0x10];
    void *r;

    func_00105D58(sp40);
    func_00105DF8(sp40, -0.78539816f, sp40);
    func_00105CE0(sp70, arg2);
    r = func_0018EA20(arg0, arg1 & 0xFF, sp40);
    if (r != NULL) {
        func_0019CB60(0x4A, 0xFF, 0);
        *(s32 *)((u8 *)r + 0x128) = 0x3E4CCCCD;
        *(s32 *)((u8 *)r + 0x134) = 0x3DDF0124;
        *(s8 *)((u8 *)r + 0x9) = 2;
        *(s16 *)((u8 *)r + 0xC) = 0x1E;
        *(void **)((u8 *)r + 0x164) = r;
    }
}

/* --- func_0018E440 --- */
s32 D_00224A08;
void func_0018E440(s32 arg0) {
    D_00224A08 = arg0;
}

/* --- func_0018E430 --- */
s32 D_00224A14;
void func_0018E430(s32 arg0) {
    D_00224A14 = arg0;
}