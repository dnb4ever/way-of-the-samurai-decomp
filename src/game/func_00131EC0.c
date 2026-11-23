/* func_00131EC0.c -- handler: set the matrix */

#include "game/obj_00131f98.h"

/*
================
func_00131EC0

Handler of D_00202C90: copies matrix into the object's matrix and
returns 0.
================
*/
s32 func_00131EC0(Obj_00131F98 *obj, sceVu0FVECTOR *matrix)
{
    sceVu0CopyMatrix(obj->unk_388, matrix);
    return 0;
}
