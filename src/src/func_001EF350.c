extern u8 D_00223FE0[];
typedef struct { void *f0; void *f4; } S001EF350;
S001EF350 *func_001EF350(S001EF350 *arg0, void *arg1) {
    volatile S001EF350 *p = arg0;
    p->f4 = (void *)D_00223FE0;
    p->f0 = arg1;
    return arg0;
}

void func_00115DA8(s32);

s32 func_001EEE00(void *arg0) {
    func_00115DA8(*(s32*)((char*)arg0 + 0x44));
    func_00115DA8(*(s32*)((char*)arg0 + 0x58));
    return 1;
}

s64 func_0011C0C8();

void func_001EED70(void *arg0) {
    s32 f48 = *(s32*)((char*)arg0 + 0x48);
    s32 f44 = *(s32*)((char*)arg0 + 0x44);
    func_0011C0C8(1, 0x8010, 0xF80, 0x7FFF);
    func_0011C0C8(1, 0x8010, 0x1080, 0x7FFF);
    func_0011C0C8(1, 0x80E0, 0, 0x13, f44, (f48 / 1024) << 10, f44 + *(s32*)((char*)arg0 + 0x50));
    *(s32*)arg0 = 2;
}

void func_001EECB0(void *arg0) {
    s64 r;
    *(s32*)((char*)arg0 + 0x0) = 3;
    func_0011C0C8(1, 0x8010, 0xF80, 0);
    func_0011C0C8(1, 0x8010, 0x1080, 0);
    r = func_0011C0C8(1, 0x80E0, 0, 2, 0, 0);
    *(s32*)((char*)arg0 + 0x50) = ((u64)(r << 0x28) >> 0x28) - *(s32*)((char*)arg0 + 0x44);
    func_0011C0C8(1, 0x80D0, 0, 0, *(s32*)((char*)arg0 + 0x58), 0x4000, 0x800);
    *(s32*)((char*)arg0 + 0x0) = 0;
    *(s32*)((char*)arg0 + 0x2C) = 0;
    *(s32*)((char*)arg0 + 0x34) = 0;
    *(s32*)((char*)arg0 + 0x38) = 0;
    *(s32*)((char*)arg0 + 0x40) = 0;
    *(s32*)((char*)arg0 + 0x54) = 0;
    *(s32*)((char*)arg0 + 0x4C) = 0;
    *(s32*)((char*)arg0 + 0x50) = 0;
}