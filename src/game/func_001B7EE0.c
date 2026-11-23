/* func_001B7EE0.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_001B7EE0

Returns the element of the iterator's node, stored after its links.
================
*/
void *func_001B7EE0(ListIter *it)
{
    return it->node + 1;
}
