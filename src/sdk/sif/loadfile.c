/* loadfile.c -- loading IOP modules over the SIF */

#include "sdk/sifdev.h"

/*
================
sceSifLoadModuleBuffer

Loads and starts the module image at addr in IOP memory, discarding the
module's own result.
================
*/
int sceSifLoadModuleBuffer(void *addr, int argLen, const char *args)
{
    int result;

    return func_00116080(addr, argLen, args, &result);
}

/*
================
sceSifLoadStartModuleBuffer
================
*/
int sceSifLoadStartModuleBuffer(void *addr, int argLen, const char *args, int *result)
{
    return func_00116080(addr, argLen, args, result);
}

/*
================
sceSifLoadModule

Loads and starts the module file at path, discarding the module's own
result.
================
*/
int sceSifLoadModule(const char *path, int argLen, const char *args)
{
    int result;

    return func_00116690(path, argLen, args, &result, 0);
}

/*
================
sceSifLoadStartModule
================
*/
int sceSifLoadStartModule(const char *path, int argLen, const char *args, int *result)
{
    return func_00116690(path, argLen, args, result, 0);
}
