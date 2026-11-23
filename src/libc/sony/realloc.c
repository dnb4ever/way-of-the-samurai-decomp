/* realloc.c -- realloc under the allocator lock */

#include <stdlib.h>
#include "sonylibc.h"

/*
================
realloc
================
*/
void *realloc(void *ap, size_t nbytes)
{
    void *p;

    __malloc_lock(_REENT);
    p = _realloc_r(_REENT, ap, nbytes);
    __malloc_unlock(_REENT);
    return p;
}
