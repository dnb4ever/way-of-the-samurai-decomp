typedef struct Buf {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 fC;
    s32 f10;
    u32 data[1];
} Buf;

Buf *Balloc(s32 a, s32 b, s32 c);
s32 func_001D18D8(s32 a, Buf *b, u32 *c, u32 *d);

Buf *func_001D1FC8(s32 arg0, Buf *arg1, s32 arg2) {
    s32 shift5 = arg2 >> 5;
    s32 cap = arg1->f8;
    s32 var_18 = shift5 + arg1->f10 + 1;
    s32 var_5 = arg1->f4;
    Buf *out;
    u32 *dst;
    u32 *src;
    u32 *end;

    if (cap < var_18) {
        do {
            cap *= 2;
            var_5 += 1;
        } while (cap < var_18);
    }
    out = Balloc(arg0, var_5, cap);
    dst = (u32 *)((s8 *)out + 0x14);
    if (shift5 > 0) {
        cap = shift5;
        do {
            cap -= 1;
            *dst = 0;
            dst++;
        } while (cap != 0);
    }
    src = (u32 *)((s8 *)arg1 + 0x14);
    end = src + arg1->f10;
    arg2 = arg2 & 0x1F;
    if (arg2 != 0) {
        u32 carry = 0;
        s32 rsh = 0x20 - arg2;
        do {
            *dst = (*src << arg2) | carry;
            dst++;
            carry = *src >> rsh;
            src++;
        } while ((u32)src < (u32)end);
        *dst = carry;
        if (carry != 0) {
            var_18 += 1;
        }
    } else {
        do {
            *dst = *src;
            src++;
            dst++;
        } while ((u32)src < (u32)end);
    }
    out->f10 = var_18 - 1;
    func_001D18D8(arg0, arg1, end, dst);
    return out;
}