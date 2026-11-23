/* exceptionmips.c -- Metrowerks C++ runtime: exception dispatch

   The original is C++; written here as the C its bytes are. */

#include "sdk/mwrt.h"

/*
================
func_001021E0

std::bad_exception::what.
================
*/
const char *func_001021E0(void *self)
{
    return "bad_exception";
}
