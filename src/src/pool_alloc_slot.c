void *func_001CF8B0(void *mgr, s32 n);
void func_001CFA60(void);

void *pool_alloc_slot(void *arg0) {
    void *block;
    void *slot;
    void *r;
    s32 n;

    if (M2C_FIELD(arg0, s32, 0x38) == 0) {
        func_001CFA60();
    }
    block = (char *)arg0 + 0x1D8;
    for (;;) {
        n = M2C_FIELD(block, s32, 0x4);
        n--;
        slot = M2C_FIELD(block, void *, 0x8);
        if (n >= 0) {
            do {
                if (M2C_FIELD(slot, s16, 0xC) == 0) {
                    goto found;
                }
                n--;
                slot = (char *)slot + 0x58;
            } while (n >= 0);
        }
        slot = M2C_FIELD(block, void *, 0x0);
        if (slot == NULL) {
            r = func_001CF8B0(arg0, 4);
            M2C_FIELD(block, void *, 0x0) = r;
            if (r == NULL) {
                M2C_FIELD(arg0, s32, 0x0) = 0xC;
                return r;
            }
            slot = r;
        }
        block = slot;
    }
found:
    M2C_FIELD(slot, s16, 0xC) = 1;
    M2C_FIELD(slot, s16, 0xE) = -1;
    M2C_FIELD(slot, void *, 0x54) = arg0;
    M2C_FIELD(slot, s32, 0x0) = 0;
    M2C_FIELD(slot, s32, 0x8) = 0;
    M2C_FIELD(slot, s32, 0x4) = 0;
    M2C_FIELD(slot, s32, 0x10) = 0;
    M2C_FIELD(slot, s32, 0x14) = 0;
    M2C_FIELD(slot, s32, 0x18) = 0;
    M2C_FIELD(slot, s32, 0x30) = 0;
    M2C_FIELD(slot, s32, 0x34) = 0;
    M2C_FIELD(slot, s32, 0x44) = 0;
    M2C_FIELD(slot, s32, 0x48) = 0;
    return slot;
}
