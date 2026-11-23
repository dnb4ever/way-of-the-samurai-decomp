/* func_0015ABC0.c -- vector: negated copy */

#include "game/obj_0015a.h"

#ifdef NON_MATCHING

/*
================
func_0015ABC0

Stores in *ret the vector with x, y and z negated and w kept: the C form
of a member function that returns the vector by value. Returns ret.

Not byte-exact: retail runs every element through $f0; at -O2 both stock
compilers alternate $f0 and $f1 (-Os reproduces retail).
================
*/
Obj_0015AF70 *func_0015ABC0(Obj_0015AF70 *ret, Obj_0015AF70 *vec)
{
    ret->v[0] = -vec->v[0];
    ret->v[1] = -vec->v[1];
    ret->v[2] = -vec->v[2];
    ret->v[3] = vec->v[3];
    return ret;
}

#endif /* NON_MATCHING */
