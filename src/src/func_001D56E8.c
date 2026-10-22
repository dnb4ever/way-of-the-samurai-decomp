extern s32 func_001CFFB8(void);

s32 func_001D56E8(void *a0, s32 *arg1)
{
    s32 r;
    if (arg1[2] == 0) {
        arg1[1] = 0;
        return 0;
    }
    r = func_001CFFB8();
    arg1[2] = 0;
    arg1[1] = 0;
    return r;
}