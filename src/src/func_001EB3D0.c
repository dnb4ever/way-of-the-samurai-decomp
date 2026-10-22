s32 D_00224AC0;
s32 D_00224AC4;
s32 D_00224AD4;

s32 func_00103360(s32);
void func_0010EB10(u32);

void func_001EB3D0(void) {
    D_00224AC0 = 0;
    D_00224AD4 = 0;
}

void func_001EB3E0(s32 arg0) {
    while (func_00103360(0) == arg0);
    D_00224AD4 = 0;
    D_00224AC0 = 1;
    D_00224AC4 = 0;
}

void func_001EBF80(void) {
    func_0010EB10(0x20);
}