/* func_0012C810.c -- handler: request 1/0x1B */

#include "game/obj_00131f98.h"

/*
================
func_0012C810

Handler of D_00202C90: returns whether request 0x1B of group 1 is
accepted.
================
*/
s32 func_0012C810(Obj_00131F98 *obj)
{
    return func_00132280(obj, 1, 0x1B, 0) != 0;
}
