/* func_001EB3C0.c -- construct */

#include "game/upper.h"

/*
================
func_001EB3C0

Constructs the 8-byte object, both words 0. Returns obj.
================
*/
Unk001EB3C0 *func_001EB3C0(Unk001EB3C0 *obj)
{
    obj->unk_00 = 0;
    obj->unk_04 = 0;
    return obj;
}
