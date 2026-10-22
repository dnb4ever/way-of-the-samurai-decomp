extern u32 D_00225BD8[];

s32 func_001111C0(s32 arg0, s32 arg1) {
    u32 *ptr = (u32 *)D_00225BD8;
    s32 temp_2;

    temp_2 = ptr[3];         // 0xC offset as 4-byte int
    ptr[4] = arg1;           // 0x10 offset
    ptr[3] = arg0;           // 0xC offset
    return temp_2;
}