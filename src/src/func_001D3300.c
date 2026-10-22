/* identical to func_001CFA40 (matched), with its names changed by decomp.wtf */
extern s32 D_00210F40[];
extern s32 D_C290C8;

void _init_signal_r(void *arg0);
void func_001CF850(void *dst, s32 a, s32 b, void *owner);
s32 func_0010F418(s32 a, s32 b);
s32 func_0010F358(s32 a, s32 b, s32 c);

void func_001D3300(void) {
    _init_signal_r((void *)D_00210F40[0]);
}

void func_001CFA60(void *arg0) {
    void *temp = (void *)((u8 *)arg0 + 0x1E4);
    *(void **)((u8 *)arg0 + 0x3C) = (void *)_init_signal_r;
    *(s32 *)((u8 *)arg0 + 0x38) = 1;
    func_001CF850(temp, 4, 0, arg0);
    func_001CF850((void *)((u8 *)arg0 + 0x23C), 9, 1, arg0);
    func_001CF850((void *)((u8 *)arg0 + 0x294), 0xA, 2, arg0);
    *(s32 *)((u8 *)arg0 + 0x1D8) = 0;
    *(void **)((u8 *)arg0 + 0x1E0) = temp;
    *(s32 *)((u8 *)arg0 + 0x1DC) = 3;
}

void func_001CFF58(s32 *arg0, s32 arg1, s32 arg2) {
    D_C290C8 = 0;
    if (func_0010F418(arg1, arg2) == -1 && D_C290C8 != 0) {
        *arg0 = D_C290C8;
    }
}

void func_001D0530(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_C290C8 = 0;
    if (func_0010F358(arg1, arg2, arg3) == -1 && D_C290C8 != 0) {
        *arg0 = D_C290C8;
    }
}