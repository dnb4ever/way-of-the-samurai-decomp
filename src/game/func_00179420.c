/* func_00179420.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_00179420

Returns the element of the iterator's node, stored after its links.
================
*/
void *func_00179420(ListIter *it)
{
    return it->node + 1;
}
