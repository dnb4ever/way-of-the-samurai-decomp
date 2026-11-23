/* sysbit.c -- libmpeg: the bit reader over the input ring buffer */

#include "sdk/libmpeg.h"

/*
================
_sysbitInit
================
*/
void _sysbitInit(SysBit *sb, u8 *start, u8 *ringBase, int ringSize)
{
    sb->start = start;
    sb->pRead = start;
    sb->buf = 0;
    sb->nbits = 0;
    sb->pos = 0;
    sb->ringBase = ringBase;
    sb->ringEnd = ringBase + ringSize;
    sb->ringSize = ringSize;
    func_0010BBF8(sb, 0);
}

#ifdef NON_MATCHING
/*
================
func_0010BBD8

Returns the next n bits without consuming them. Parked: retail splits the
sign extension of the result and moves its dsra32 into the return's delay
slot; ee-gcc 2.9-991111 keeps the pair together (2.96 splits it, but the
rest of the unit is 991111 code).
================
*/
int func_0010BBD8(SysBit *sb, int n)
{
    return sb->buf >> (64 - n);
}
#endif

/*
================
func_0010BBF8

Consumes n bits and refills buf to more than 56 valid bits.
================
*/
void func_0010BBF8(SysBit *sb, int n)
{
    sb->buf <<= n;
    sb->nbits -= n;
    while (sb->nbits <= 56) {
        sb->buf |= (u64)*sb->pRead++ << (56 - sb->nbits);
        if (sb->pRead >= sb->ringEnd) {
            sb->pRead = sb->ringBase;
        }
        sb->nbits += 8;
    }
    sb->pos += n;
}

/*
================
_sysbitGet

Reads n bits.
================
*/
u32 _sysbitGet(SysBit *sb, int n)
{
    u32 bits = func_0010BBD8(sb, n);

    func_0010BBF8(sb, n);
    return bits;
}

/*
================
_sysbitMarker

Reads a marker bit.
================
*/
u32 _sysbitMarker(SysBit *sb)
{
    u32 bit = func_0010BBD8(sb, 1);

    func_0010BBF8(sb, 1);
    return bit;
}

/*
================
_sysbitJump

Skips nbytes bytes.
================
*/
void _sysbitJump(SysBit *sb, int nbytes)
{
    sb->buf = 0;
    sb->nbits = 0;
    sb->pos += nbytes * 8;
    sb->pRead = sb->start + (int)(sb->pos >> 3);
    if (sb->pRead >= sb->ringEnd) {
        sb->pRead -= sb->ringSize;
    }
    func_0010BBF8(sb, 0);
}

/*
================
_sysbitPtr

Returns the address of the byte holding bit bitpos.
================
*/
u8 *_sysbitPtr(SysBit *sb, int bitpos)
{
    u8 *p = sb->start + (bitpos >> 3);

    if (p >= sb->ringEnd) {
        p -= sb->ringSize;
    }
    return p;
}
