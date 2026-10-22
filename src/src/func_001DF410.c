typedef struct FParts { s32 f0; s32 f4; s32 f8; u32 fC; } FParts;
void func_001DF2C0(f32 *val, FParts *out);

u32 func_001DF410(f32 fparg0) {
    FParts p;
    f32 fval;
    fval = fparg0;
    func_001DF2C0(&fval, &p);
    if ((p.f0 ^ 2) == 0) return 0;
    if (p.f4 != 0) return 0;
    if (p.f8 < 0) return 0;
    if (p.f8 >= 0x20) return -1;
    if (p.f8 >= 0x1F) return p.fC << (p.f8 - 0x1E);
    return p.fC >> (0x1E - p.f8);
}