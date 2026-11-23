/* func_00175760.c -- add the value at 0x24 */

#include "game/upper.h"

/*
================
func_00175760

Adds src's float at 0x24 to dst's.
================
*/
void func_00175760(Unk001756F0 *dst, Unk001756F0 *src)
{
    dst->unk_24 += src->unk_24;
}
