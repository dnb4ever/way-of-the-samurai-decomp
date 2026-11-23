/* func_0019BB30.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_0019BB30

Returns the address of the element of the iterator's node.
================
*/
void *func_0019BB30(ListIter *it)
{
    return &it->node->value;
}
