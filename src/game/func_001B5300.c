/* func_001B5300.c -- construct */

#include "game/upper.h"

/*
================
func_001B5300

Constructs the 24-byte object: the words at 0x0, 0x10 and 0x14 are -1,
the rest 0. Returns obj.
================
*/
Obj_001B5300 *func_001B5300(Obj_001B5300 *obj)
{
    obj->unk_04 = 0;
    obj->unk_08 = 0;
    obj->unk_0C = 0;
    obj->unk_10 = -1;
    obj->unk_14 = -1;
    obj->unk_00 = -1;
    return obj;
}
