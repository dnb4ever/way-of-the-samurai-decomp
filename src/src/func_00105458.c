void func_00105458(void *arg0) {
    s32 **pp = (s32 **)arg0;
    s32 v0 = (s32)pp[0];
    s32 *ptr = pp[3];
    v0 -= 4;
    pp[3] = 0;
    v0 -= (s32)ptr;
    *ptr += ((u32)(v0 >> 2)) >> 2;
}

void func_00105968(void **arg0, s64 *arg1, s32 arg2) {
    s32 *out;
    s32 n;
    s64 a, b;
    if (arg2 != 0) {
        n = arg2 - 1;
        out = (s32 *)*arg0;
        do {
            a = arg1[0];
            n -= 1;
            b = arg1[1];
            arg1 += 2;
            *out = (s32)a;
            out += 1;
            *out = (s32)(a >> 32);
            out += 1;
            *out = (s32)b;
            out += 1;
            *out = (s32)(b >> 32);
            out += 1;
        } while (n != -1);
        *arg0 = out;
    }
}

void func_001059E0(void **arg0, s64 *arg1, s32 arg2) {
    s32 *out;
    s32 n;
    s64 v;
    if (arg2 != 0) {
        n = arg2 - 1;
        out = (s32 *)*arg0;
        do {
            v = *arg1;
            n -= 1;
            arg1 += 1;
            *out = (s32)v;
            out += 1;
            *out = (s32)(v >> 32);
            out += 1;
        } while (n != -1);
        *arg0 = out;
    }
}