/* func_00168B60.c -- string of an object */

#include "game/upper.h"

/*
================
func_00168B60

Returns the string an object holds in its first word; a caller compares
it with strcmp.
================
*/
char *func_00168B60(Obj_00168B60 *obj)
{
    return obj->unk_00;
}
