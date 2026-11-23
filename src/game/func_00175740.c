/* func_00175740.c -- subtract the value at 0x24 */

#include "game/upper.h"

/*
================
func_00175740

Subtracts src's float at 0x24 from dst's.
================
*/
void func_00175740(Obj_001756F0 *dst, Obj_001756F0 *src)
{
    dst->unk_24 -= src->unk_24;
}
