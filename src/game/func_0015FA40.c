/* func_0015FA40.c -- list iterator: element address through the vtable */

#include "game/listiter.h"

#ifdef NON_MATCHING

/*
================
func_0015FA40

Returns the element address through the iterator's own get slot.
Slot 7 (unk_24) of the iterator vtable D_00223A10.

Parked: retail tail-calls the slot (jr $t9); the stock compilers make a
full call with a stack frame.
================
*/
void *func_0015FA40(ListIter *it)
{
    return it->vtbl->get(it);
}
#endif
