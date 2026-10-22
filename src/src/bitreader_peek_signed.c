s32 bitreader_peek_signed(void *arg0, s32 arg1) {
    return (s32)(*(u64 *)arg0 >> (0x40 - arg1));
}

void bitreader_skip_refill(void *arg0, s32 arg1) {
    u8 *s = (u8 *)arg0;
    u32 n;
    *(u64 *)(s + 0x0) <<= arg1;
    n = *(u32 *)(s + 0x10) - arg1;
    *(u32 *)(s + 0x10) = n;
    if (n < 0x39) {
        u8 *end = *(u8 **)(s + 0x24);
        u8 *cur;
        u32 bc;
        do {
            cur = *(u8 **)(s + 0xC);
            bc = *(u32 *)(s + 0x10);
            *(u64 *)(s + 0x0) |= (u64)*cur << (0x38 - bc);
            cur++;
            *(u8 **)(s + 0xC) = cur;
            if (cur >= end) {
                *(u8 **)(s + 0xC) = *(u8 **)(s + 0x20);
            }
            *(u32 *)(s + 0x10) = bc + 8;
        } while (bc + 8 < 0x39);
    }
    *(s64 *)(s + 0x18) += arg1;
}

void bitreader_init(void *arg0, void *arg1, s32 arg2, s32 arg3) {
    u8 *s = (u8 *)arg0;
    *(u32 *)(s + 0xC) = (u32)arg1;
    *(u32 *)(s + 0x24) = arg2 + arg3;
    *(u32 *)(s + 0x28) = arg3;
    *(u32 *)(s + 0x8) = (u32)arg1;
    *(u64 *)(s + 0x0) = 0;
    *(u32 *)(s + 0x10) = 0;
    *(u64 *)(s + 0x18) = 0;
    *(u32 *)(s + 0x20) = arg2;
    bitreader_skip_refill(arg0, 0);
}