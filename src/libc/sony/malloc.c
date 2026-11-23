/* malloc.c -- malloc and free under the allocator lock */

#include <stdlib.h>
#include "sonylibc.h"

/*
================
malloc
================
*/
void *malloc(size_t nbytes)
{
    void *p;

    __malloc_lock(_REENT);
    p = _malloc_r(_REENT, nbytes);
    __malloc_unlock(_REENT);
    return p;
}

/*
================
free
================
*/
void free(void *aptr)
{
    __malloc_lock(_REENT);
    _free_r(_REENT, aptr);
    __malloc_unlock(_REENT);
}
