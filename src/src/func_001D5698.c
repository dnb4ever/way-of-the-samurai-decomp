extern s32 func_001CFFB8(void);
extern u8 _ctype_[];
extern s32 D_00210F40[];
extern void _strtoll_r(s32, s32, s32, s32);

s32 func_001D56E8(void *a0, s32 *arg1)
{
    s32 r;
    if (arg1[2] == 0) {
        arg1[1] = 0;
        return 0;
    }
    r = func_001CFFB8();
    arg1[1] = 0;
    arg1[2] = 0;
    return r;
}

s32 func_001D56C8(s32 arg0)
{
    return (_ctype_[arg0] & 2) ? (arg0 - 0x20) : arg0;
}

void func_001D5698(s32 arg0, s32 arg1, s32 arg2)
{
    _strtoll_r(D_00210F40[0], arg0, arg1, arg2);
}