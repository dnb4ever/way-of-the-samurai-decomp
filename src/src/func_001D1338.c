s32 func_001D1338(s32 a0, s32 *a1, u8 *a2, s32 a3) {
    s32 local;
    s32 *p;
    if (a2 == NULL) return 0;
    p = (a1 != NULL) ? a1 : &local;
    if (a3 == 0) return -1;
    *p = *a2;
    return *a2 != 0;
}