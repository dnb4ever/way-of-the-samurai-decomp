/* func_00175B90.c -- compare the value at 0x24: less */

#include "game/upper.h"

/*
================
func_00175B90

Returns whether a's value at 0x24 is less than b's.
================
*/
s32 func_00175B90(Unk00175B60 *a, Unk00175B60 *b)
{
    return a->unk_24 < b->unk_24;
}
