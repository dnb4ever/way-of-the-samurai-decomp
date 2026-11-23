/* func_0017A880.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_0017A880

Returns the address of the element of the iterator's node.
================
*/
void *func_0017A880(ListIter *it)
{
    return &it->node->value;
}
