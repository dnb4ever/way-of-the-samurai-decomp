/* memclr.c -- byte clear used by the SDK libraries */

#include "common.h"

/*
================
memclr

Sets size bytes at dst to zero.
================
*/
void memclr(void *dst, int size)
{
    char *p = dst;

    while (size--) {
        *p++ = 0;
    }
}
