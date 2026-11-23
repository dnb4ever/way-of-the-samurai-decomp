/* new.c -- Metrowerks C++ runtime: operator new and std::bad_alloc

   The original is C++ (bad_alloc::what is a const member); written here
   as the C its bytes are. */

#include "sdk/mwrt.h"

/*
================
func_00100620

std::bad_alloc::what.
================
*/
const char *func_00100620(void *self)
{
    return "bad_alloc";
}

/*
================
func_00100530

operator new[]: arrays come from operator new.
================
*/
void *func_00100530(u32 size)
{
    return func_00100630(size);
}
