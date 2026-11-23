/* func_001EF370.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_001EF370

Returns the address of the element of the iterator's node.
================
*/
void *func_001EF370(ListIter *it)
{
    return &it->node->value;
}
