/* func_0017D2C0.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_0017D2C0

Returns the address of the element of the iterator's node.
================
*/
void *func_0017D2C0(ListIter *it)
{
    return &it->node->value;
}
