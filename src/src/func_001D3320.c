extern s32 D_00210F40[];
void func_001D3220(s32, s32);

void func_001D3320(s32 arg0) {
    func_001D3220(D_00210F40[0], arg0);
}

long func_001D2998(s32, s16, s32, s32);

s32 func_001D34D0(void *arg0, s32 arg1, s32 arg2) {
    s32 r = (s32)func_001D2998(*(s32*)((u8*)arg0 + 0x54), *(s16*)((u8*)arg0 + 0xE), arg1, arg2);
    if (r >= 0) {
        *(s32*)((u8*)arg0 + 0x50) += r;
    } else {
        *(u16*)((u8*)arg0 + 0xC) &= 0xEFFF;
    }
    return r;
}

void func_001D0530(s32, s16, s32, s32);
long func_001D7170(s32, s16, s32, s32);

s32 func_001D3538(void *arg0, s32 arg1, s32 arg2) {
    if (*(u16*)((u8*)arg0 + 0xC) & 0x100) {
        func_001D0530(*(s32*)((u8*)arg0 + 0x54), *(s16*)((u8*)arg0 + 0xE), 0, 2);
    }
    *(u16*)((u8*)arg0 + 0xC) &= 0xEFFF;
    return (s32)func_001D7170(*(s32*)((u8*)arg0 + 0x54), *(s16*)((u8*)arg0 + 0xE), arg1, arg2);
}

s32 _init_signal_r(void *);

s32 func_001D30C8(void *arg0, u32 arg1, s32 arg2) {
    s32 *tbl;
    s32 old;
    if (arg1 >= 0x20U) {
        *(s32*)arg0 = 0x16;
        return -1;
    }
    tbl = *(s32**)((u8*)arg0 + 0x1D4);
    if (tbl == 0) {
        if (_init_signal_r(arg0) != 0) {
            return -1;
        }
        tbl = *(s32**)((u8*)arg0 + 0x1D4);
    }
    old = tbl[arg1];
    tbl[arg1] = arg2;
    return old;
}