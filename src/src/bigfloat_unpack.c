void func_00162730(s32 a, s32 b);
void func_001E60B0(s32 arg0, s32 arg1) {
    func_00162730(arg1, arg0 + 0x48);
}

s32 func_001E46B0(void *arg0, s32 key, u16 *arr) {
    s32 count;
    u16 *p;
    s32 i;
    if (arr != NULL) {
        count = arr[0];
        p = arr + 1;
        if (count > 0) {
            key = key & 0xFFFF;
            i = 0;
            do {
                u16 val = *p;
                if (key < (s32)val) return 0;
                if (val == key) return 1;
                i += 1;
                p++;
            } while (i < count);
        }
    }
    return 0;
}

void bigfloat_unpack(u64 *arg0, void *arg1) {
    u64 v = *arg0;
    s32 exp = (s32)(v >> 52) & 0x7FF;
    s32 sign = v >> 63;
    u64 mant = v & 0xFFFFFFFFFFFFFULL;
    M2C_FIELD(arg1, s32, 0x4) = sign;
    if (exp == 0) {
        M2C_FIELD(arg1, s32, 0x0) = 2;
        return;
    }
    if (exp == 0x7FF) {
        M2C_FIELD(arg1, s32, 0x8) = 0x3FF;
        if (mant == 0) {
            M2C_FIELD(arg1, s32, 0x0) = 4;
            M2C_FIELD(arg1, u64, 0x10) = (u64)0x8000 << 45;
            return;
        }
        if ((mant & ((u64)0x8000 << 36)) != 0) {
            M2C_FIELD(arg1, s32, 0x0) = 1;
        } else {
            M2C_FIELD(arg1, s32, 0x0) = 0;
        }
        M2C_FIELD(arg1, u64, 0x10) = mant;
        return;
    }
    M2C_FIELD(arg1, u64, 0x10) = (mant << 8) | ((u64)0x8000 << 45);
    M2C_FIELD(arg1, s32, 0x0) = 3;
    M2C_FIELD(arg1, s32, 0x8) = exp - 0x3FF;
}