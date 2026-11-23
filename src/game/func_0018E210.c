/* func_0018E210.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_0018E210

Returns the element of the iterator's node, stored after its links.
================
*/
void *func_0018E210(ListIter *it)
{
    return it->node + 1;
}
