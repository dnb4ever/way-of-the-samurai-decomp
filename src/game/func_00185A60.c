/* func_00185A60.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_00185A60

Returns the element of the iterator's node, stored after its links.
================
*/
void *func_00185A60(ListIter *it)
{
    return it->node + 1;
}
