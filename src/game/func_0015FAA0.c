/* func_0015FAA0.c -- list iterator: step back */

#include "game/listiter.h"

/*
================
func_0015FAA0

Moves the iterator to the previous node (--it) and returns it.
Slot 3 (prev) of the iterator vtable D_00223A40.

Parked: retail keeps the loaded node in $v1 for both loads; the stock
compilers at -O2 load the first into $a1. Only merging the two values
into one local reproduces it, which is not source.
Both stock compilers reproduce it at -Os, but no other function in these
objects tells -Os from -O2, and their constructors and slot-6/7 thunks
match under neither, so -Os is not taken as the objects' flag.
================
*/
#ifdef NON_MATCHING
ListIter *func_0015FAA0(ListIter *it)
{
    it->node = it->node->prev;
    return it;
}
#endif
