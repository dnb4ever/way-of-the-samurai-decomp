void func_00105040(s32 *arg0, s32 arg1) {
    *arg0 = *arg0 + arg1 * 4;
}

typedef unsigned int u128 __attribute__((mode(TI)));

void func_00105058(void *arg0, u128 arg1) {
    u128 *p = *(u128 **)arg0;
    *p = arg1;
    *(u128 **)((char *)arg0 + 0xC) = p;
    *(u128 **)arg0 = p + 1;
}