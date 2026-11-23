/* func_00191340.c -- call func_00190CB0 on the first word */

#include "game/upper.h"

/*
================
func_00191340

Calls func_00190CB0 with the object and the object its first word points
to.
================
*/
void func_00191340(Obj_00191340 *obj)
{
    func_00190CB0(obj, obj->unk_00);
}
