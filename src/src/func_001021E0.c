extern u8 D_00213578[];

u32 *func_001021E0(void)
{
    return (u32 *)D_00213578;
}

void *func_00100A90(void *, void *);

void func_001022E0(s8 *arg0)
{
    s32 flag;
    s32 local;
    void *r;

    flag = *arg0 & 0x40;
    r = func_00100A90(arg0 + 1, &local);
    r = func_00100A90(r, &local);
    if (flag != 0) {
        func_00100A90(r, &local);
    }
}