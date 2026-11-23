/* func_001EF370.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_001EF370

Returns the element of the iterator's node, stored after its links.
================
*/
void *func_001EF370(ListIter *it)
{
    return it->node + 1;
}
