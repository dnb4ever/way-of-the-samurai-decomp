/* func_0019BB30.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_0019BB30

Returns the element of the iterator's node, stored after its links.
================
*/
void *func_0019BB30(ListIter *it)
{
    return it->node + 1;
}
