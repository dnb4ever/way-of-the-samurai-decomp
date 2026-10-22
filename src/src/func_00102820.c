extern u8 D_001FD230[];
extern u8 D_002240F0[];
extern void *func_00100A90(void *a, void *b);

u32 *func_00102820(void)
{
    return (u32 *)D_001FD230;
}

s32 func_00102330(void *arg0)
{
    *(u8 **)((u8 *)arg0 + 0xC) = D_002240F0;
    *(u8 **)((u8 *)arg0 + 0x10) = D_002240F0;
    return 1;
}

void func_00102B08(void *arg0)
{
    u64 *s = (u64 *)arg0;
    u64 v0 = s[0];
    if (((s16 *)func_00102820())[3] == 1) {
        *(u64 *)0x12000000 = v0;
        *(u64 *)0x12000070 = s[2];
        *(u64 *)0x12000080 = s[3];
        *(u64 *)0x120000C0 = s[4];
    } else {
        *(u64 *)0x12000000 = v0;
        *(u64 *)0x12000020 = s[1];
        *(u64 *)0x12000090 = s[2];
        *(u64 *)0x120000A0 = s[3];
        *(u64 *)0x120000E0 = s[4];
    }
}

void func_00102350(void *arg0, void *arg1)
{
    volatile s8 *p = *(s8 **)((u8 *)arg1 + 4);
    s8 c = *p;
    s32 bit40 = c & 0x40;
    void *r;

    *(s32 *)((u8 *)arg0 + 0x230) = c & 0x20;
    *(s32 *)((u8 *)arg0 + 0x22C) = *p & 0x10;
    *(s32 *)((u8 *)arg0 + 0x228) = *p & 0xF;
    r = func_00100A90((void *)(p + 1), (u8 *)arg0 + 0x220);
    r = func_00100A90(r, (u8 *)arg0 + 0x224);
    if (*(s32 *)((u8 *)arg0 + 0x230) != 0) {
        *(s32 *)((u8 *)arg0 + 0x18) = (s32)*(u128 *)((u8 *)arg0 + 0x200);
    } else {
        *(s32 *)((u8 *)arg0 + 0x18) = *(s32 *)((u8 *)arg0 + 0x14);
    }
    if (bit40 != 0) {
        func_00100A90(r, (u8 *)arg0 + 0x234);
    } else {
        *(s32 *)((u8 *)arg0 + 0x234) = 0;
    }
}