/* func_00159FC0.c -- exchange D_00224718 */

#include "game/unsorted.h"

/*
================
func_00159FC0

Sets D_00224718 to value and returns its old value.
================
*/
s32 func_00159FC0(s32 value)
{
    s32 old;

    old = D_00224718;
    D_00224718 = value;
    return old;
}
