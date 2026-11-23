/* func_001BBA90.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_001BBA90

Returns the address of the element of the iterator's node.
================
*/
void *func_001BBA90(ListIter *it)
{
    return &it->node->value;
}
