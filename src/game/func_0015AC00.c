/* func_0015AC00.c -- vector: negate in place */

#include "game/obj_0015a.h"

#ifdef NON_MATCHING

/*
================
func_0015AC00

Negates x, y and z of the vector in place. Returns vec.

Not byte-exact: retail runs every element through $f0; at -O2 both stock
compilers alternate $f0 and $f1, and load all three elements before the
first store, where retail stores each element before it loads the next.
================
*/
Obj_0015AF70 *func_0015AC00(Obj_0015AF70 *vec)
{
    vec->v[0] = -vec->v[0];
    vec->v[1] = -vec->v[1];
    vec->v[2] = -vec->v[2];
    return vec;
}

#endif /* NON_MATCHING */
