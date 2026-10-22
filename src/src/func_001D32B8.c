extern s32 D_00210F40[];
void _raise_r(s32, s32);
void func_001D32B8(s32 arg0) {
    _raise_r(D_00210F40[0], arg0);
}

s32 _init_signal_r(void *);
s32 func_001D3220(void *arg0, s32 arg1) {
    s32 (**tbl)(s32);
    s32 (*fn)(s32);

    if ((u32)arg1 >= 0x20U) {
        return -1;
    }
    tbl = *(s32 (***)(s32))((u8 *)arg0 + 0x1D4);
    if (tbl == 0) {
        if (_init_signal_r(arg0) != 0) {
            return -1;
        }
        tbl = *(s32 (***)(s32))((u8 *)arg0 + 0x1D4);
    }
    fn = tbl[arg1];
    if (fn == 0) {
        return 1;
    }
    if (fn == (s32 (*)(s32))-1) {
        return 2;
    }
    if (fn == (s32 (*)(s32))1) {
        return 3;
    }
    tbl[arg1] = 0;
    fn(arg1);
    return 0;
}