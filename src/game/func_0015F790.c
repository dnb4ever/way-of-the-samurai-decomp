/* func_0015F790.c -- list iterator: element address */

#include "game/listiter.h"

/*
================
func_0015F790

Returns the address of the element at the iterator's node.
Slot 1 (get) of the iterator vtable D_00223A10.
================
*/
void *func_0015F790(ListIter *it)
{
    return &it->node->value;
}
