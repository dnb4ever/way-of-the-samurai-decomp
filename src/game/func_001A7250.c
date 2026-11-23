/* func_001A7250.c -- reset */

#include "game/upper.h"

/*
================
func_001A7250

Clears the object's state.
================
*/
void func_001A7250(Unk001A7250 *obj)
{
    obj->unk_014 = 0;
    obj->unk_018 = 0;
    obj->unk_01C = 0;
    obj->unk_13C = 0;
    obj->unk_140 = 0;
    obj->unk_144 = 0;
    obj->unk_00C = 0;
    obj->unk_010 = 0;
    obj->unk_134 = 0;
    obj->unk_138 = 0;
    obj->unk_004 = 0;
    obj->unk_000 = NULL;
}
