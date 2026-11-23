/* func_0013A2F0.c -- set two 64-bit members */

#include "game/unsorted.h"

/*
================
func_0013A2F0

Stores value with bit 31 set, zero-extended to 64 bits, in unk_100 and
unk_1F0.

Parked: retail builds the value in $v1, which ee-gcc2.96 does only when
the function is declared to return a value it never sets; as a void
function the stock compilers build it in $a1 or $v0.
================
*/
#ifdef NON_MATCHING
void func_0013A2F0(Obj_0013A2F0 *obj, u32 value)
{
    u64 word;

    word = value | 0x80000000;
    obj->unk_100 = word;
    obj->unk_1F0 = word;
}
#endif
