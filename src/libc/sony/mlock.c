/* mlock.c -- recursive allocator lock on an EE semaphore */

#include "sonylibc.h"

/* thread holding the lock, or -1 */
static int D_002113A8 = -1;
/* how many times that thread has taken the lock */
static int D_002113AC = 0;

/*
================
__malloc_lock

Takes the allocator lock; a thread that already holds it only counts again.
================
*/
void __malloc_lock(struct _reent *ptr)
{
    int id = GetThreadId();

    if (D_002113A8 == id) {
        D_002113AC++;
    } else {
        WaitSema(D_001FE494);
        D_002113A8 = id;
        D_002113AC++;
    }
}

/*
================
__malloc_unlock

Releases one hold of the allocator lock.
================
*/
void __malloc_unlock(struct _reent *ptr)
{
    if (--D_002113AC == 0) {
        D_002113A8 = -1;
        SignalSema(D_001FE494);
    }
}
