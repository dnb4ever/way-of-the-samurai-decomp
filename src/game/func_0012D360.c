/* func_0012D360.c -- handler: request 2/0x3070 */

#include "game/obj_00131f98.h"

/*
================
func_0012D360

Handler of D_00202C90: returns whether request 0x3070 of group 2 is
accepted.
================
*/
s32 func_0012D360(Obj_00131F98 *obj)
{
    return func_00132280(obj, 2, 0x3070, 0) != 0;
}
