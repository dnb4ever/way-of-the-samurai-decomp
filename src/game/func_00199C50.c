/* func_00199C50.c -- file lookup succeeds */

#include "game/upper.h"

/*
================
func_00199C50

Returns whether func_00190AA0 finds the file name in obj; callers pass
IOP module paths ("iop/modules/sio2man.irx").
================
*/
s32 func_00199C50(Obj_001485B0 *obj, char *name)
{
    return func_00190AA0(obj, name) != NULL;
}
