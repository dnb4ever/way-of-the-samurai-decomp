/* malign.c -- memalign under the allocator lock */

#include "sonylibc.h"

/*
================
memalign
================
*/
void *memalign(size_t align, size_t nbytes)
{
    void *p;

    __malloc_lock(_REENT);
    p = _memalign_r(_REENT, align, nbytes);
    __malloc_unlock(_REENT);
    return p;
}
