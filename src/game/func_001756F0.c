/* func_001756F0.c -- copy the value at 0x24 */

#include "game/upper.h"

/*
================
func_001756F0

Copies src's float at 0x24 into dst.
================
*/
void func_001756F0(Unk001756F0 *dst, Unk001756F0 *src)
{
    dst->unk_24 = src->unk_24;
}
