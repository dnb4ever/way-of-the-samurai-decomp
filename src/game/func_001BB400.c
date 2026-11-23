/* func_001BB400.c -- call func_001B9D70 on the part at 0xA40 */

#include "game/upper.h"

/*
================
func_001BB400

Passes the part at 0xA40 and the other arguments to func_001B9D70.
Returns 0.
================
*/
s32 func_001BB400(Obj_001BB400 *obj, s32 arg1, s32 arg2, s32 arg3)
{
    func_001B9D70(obj->unk_A40, arg1, arg2, arg3);
    return 0;
}
