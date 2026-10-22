typedef unsigned int u32;
typedef unsigned int u128 __attribute__((mode(TI)));

void dmatag_put_cnt_or_ref(void* arg0, s32 arg1) {
    s32** pp = (s32**)arg0;
    s32* temp5 = pp[0];
    pp[0] = (s32*)((u32)temp5 + 4);
    pp[3] = temp5;
    *temp5 = (arg1 != 0) ? 0xD0000000 : 0x50000000;
}

void func_00105488(void* arg0, u128 arg1) {
    u128* p = *(u128**)arg0;
    *p = arg1;
    *(u128**)((char*)arg0 + 0x14) = p;
    *(u128**)arg0 = p + 1;
}

void stream_put_s64_split(void** arg0, s64 arg1) {
    void* temp2 = *arg0;
    *(s32*)temp2 = (s32)(arg1 >> 32);
    *(s32*)((char*)temp2 + 4) = (s32)arg1;
    *arg0 = (void*)((char*)temp2 + 8);
}

void func_00105678(void** arg0, s128 arg1) {
    void* temp2 = *arg0;
    *(s32*)temp2 = (s32)arg1.high;
    *(s32*)((char*)temp2 + 4) = (s32)arg1.low;
    *arg0 = (void*)((char*)temp2 + 8);
}