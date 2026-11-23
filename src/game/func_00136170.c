/* func_00136170.c -- list iterator: step forward */

#include "game/listiter.h"

#ifdef NON_MATCHING

/*
================
func_00136170

Moves the iterator to the next node (++it) and returns it.
Slot 2 (next) of the iterator vtable D_00223670.

Parked: retail keeps the loaded node in $v1 for both loads; the stock
compilers at -O2 load the first into $a1. Only merging the two values
into one local reproduces it, which is not source.
Both stock compilers reproduce it at -Os, but no other function in these
objects tells -Os from -O2, and their constructors and slot-6/7 thunks
match under neither, so -Os is not taken as the objects' flag.
================
*/
ListIter *func_00136170(ListIter *it)
{
    it->node = it->node->next;
    return it;
}
#endif
