extern u32 D_00225BD8[];

s32 func_001111A8(s32 arg0, s32 arg1) {
    s32 temp_2;
    
    temp_2 = D_00225BD8[5];        // load from 0x14 offset (5*4)
    D_00225BD8[6] = arg1;          // store arg1 to 0x18 offset (6*4) 
    D_00225BD8[5] = arg0;          // store arg0 to 0x14 offset (5*4)
    return temp_2;
}