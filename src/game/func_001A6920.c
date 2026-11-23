/* func_001A6920.c -- reset */

#include "game/upper.h"

/*
================
func_001A6920

Clears the object's counters.
================
*/
void func_001A6920(Unk001A6920 *obj)
{
    obj->unk_20 = 0;
    obj->unk_00 = 0;
    obj->unk_0C = 0;
    obj->unk_10 = 0;
    obj->unk_14 = 0;
    obj->unk_28 = 0;
    obj->unk_1C = 0;
}
