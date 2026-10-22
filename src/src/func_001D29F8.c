/* identical to func_001D06E8 (matched), with its names changed by decomp.wtf */
extern u8 D_00210F40[];
s32 func_001D2A60(s32, s32, s32);
s32 func_001D0C10(s32, s32);
void malloc_free(s32, s32);
void func_001D1760(s32);
void func_001D17E0(s32);

s32 func_001D29F8(s32 arg0, s32 arg1) {
    s32 r;
    func_001D1760(*(s32 *)D_00210F40);
    r = func_001D2A60(*(s32 *)D_00210F40, arg0, arg1);
    func_001D17E0(*(s32 *)D_00210F40);
    return r;
}

s32 func_001D0918(s32 arg0) {
    s32 r;
    func_001D1760(*(s32 *)D_00210F40);
    r = func_001D0C10(*(s32 *)D_00210F40, arg0);
    func_001D17E0(*(s32 *)D_00210F40);
    return r;
}

void func_001D0970(s32 arg0) {
    func_001D1760(*(s32 *)D_00210F40);
    malloc_free(*(s32 *)D_00210F40, arg0);
    func_001D17E0(*(s32 *)D_00210F40);
}

s32 func_001D1338(s32 a0, s32 *a1, u8 *a2, s32 a3) {
    s32 result = 0;
    s32 local;
    s32 *p = &local;
    if (a2 != NULL) {
        if (a1 != NULL) p = a1;
        result = -1;
        if (a3 != 0) {
            *p = *a2;
            result = (*a2 != 0);
        }
    }
    return result;
}