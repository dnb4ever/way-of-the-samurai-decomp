typedef struct Ring {
    s32 size;   /* 0x0 */
    s32 count;  /* 0x4 */
    u8 *ptr;    /* 0x8 */
} Ring;

void func_00110048(Ring *r)
{
    s32 c = r->count;
    u8 *p = r->ptr;
    s32 s = r->size;
    s32 e;
    r->count = c - 1;
    p = p + 1;
    r->ptr = p;
    e = s + 0x10;
    if (p == (u8 *)((s32)r + e)) {
        r->ptr = (u8 *)r + 0x10;
    }
}