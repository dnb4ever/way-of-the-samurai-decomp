/* func_001BB4B0.c -- set the words at 0x924 */

#include "game/upper.h"

/*
================
func_001BB4B0

Sets the three words at 0x924.
================
*/
void func_001BB4B0(Unk001BB400 *obj, s32 a, s32 b, s32 c)
{
    obj->unk_928 = b;
    obj->unk_92C = c;
    obj->unk_924 = a;
}
