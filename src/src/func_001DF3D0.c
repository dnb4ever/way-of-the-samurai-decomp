typedef struct FParts { s32 f0; s32 f4; s32 f8; u32 fC; } FParts;
void func_001DF2C0(f32 *val, FParts *out);
void func_001DFFD8(s32 a, s32 b, s32 c, s64 d);

void func_001DF3D0(f32 fparg0) {
    FParts p;
    f32 fval;
    fval = fparg0;
    func_001DF2C0(&fval, &p);
    func_001DFFD8(p.f0, p.f4, p.f8, (s64)p.fC << 0x1E);
}

u32 func_001DF410(f32 fparg0) {
    FParts p;
    f32 fval;
    u32 result;
    fval = fparg0;
    func_001DF2C0(&fval, &p);
    result = 0;
    if (p.f0 != 2) {
        if (p.f4 == 0 && p.f8 >= 0 && p.f8 < 0x20) {
            result = -1;
            if (p.f8 < 0x1F) {
                result = p.fC >> (0x1E - p.f8);
            } else {
                result = p.fC << (p.f8 - 0x1E);
            }
        }
    }
    return result;
}

void bigfloat_unpack(u64 *arg0, void *arg1) {
    u64 v = *arg0;
    s32 exp = (s32)((v >> 52) & 0x7FF);
    u64 mant = v & 0xFFFFFFFFFFFFFULL;
    M2C_FIELD(arg1, s32, 0x4) = (s32)(v >> 63);
    if (exp == 0) {
        M2C_FIELD(arg1, s32, 0x0) = 2;
        return;
    }
    if (exp == 0x7FF) {
        M2C_FIELD(arg1, s32, 0x8) = 0x3FF;
        if (mant == 0) {
            M2C_FIELD(arg1, u64, 0x10) = (u64)0x8000 << 45;
            M2C_FIELD(arg1, s32, 0x0) = 4;
            return;
        }
        if (mant & ((u64)0x8000 << 36)) {
            M2C_FIELD(arg1, s32, 0x0) = 1;
        } else {
            M2C_FIELD(arg1, s32, 0x0) = 0;
        }
        M2C_FIELD(arg1, u64, 0x10) = mant;
        return;
    }
    M2C_FIELD(arg1, u64, 0x10) = (mant << 8) | ((u64)0x8000 << 45);
    M2C_FIELD(arg1, s32, 0x8) = exp - 0x3FF;
    M2C_FIELD(arg1, s32, 0x0) = 3;
}