/* func_001B83C0.c -- call func_001B8630 */

#include "game/obj_00131f98.h"

/*
================
func_001B83C0

Passes the object at D_008DCB70 (owner), obj and arg to func_001B8630.
Returns 0.
================
*/
s32 func_001B83C0(Obj_001B83E0 *owner, Obj_00131F98 *obj, s32 arg)
{
    func_001B8630(owner, obj, arg);
    return 0;
}
