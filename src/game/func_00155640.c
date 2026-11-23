/* func_00155640.c -- constructor */

#include "game/unsorted.h"

/*
================
func_00155640

Constructs obj with unk_00 = value and unk_04 = 0, and returns it.
================
*/
Obj_00155640 *func_00155640(Obj_00155640 *obj, s32 value)
{
    obj->unk_00 = value;
    obj->unk_04 = 0;
    return obj;
}
