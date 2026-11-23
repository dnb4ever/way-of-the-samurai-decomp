/* func_00165190.c -- list iterator: element address */

#include "game/listiter.h"

/*
================
func_00165190

Returns the address of the element at the iterator's node.
Slot 1 (get) of the iterator vtable D_00223B80.
================
*/
void *func_00165190(ListIter *it)
{
    return &it->node->value;
}
