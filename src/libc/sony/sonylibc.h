/* sonylibc.h -- Sony's thread-safe allocation layer over newlib */
#ifndef SONYLIBC_H
#define SONYLIBC_H

#include <reent.h>
#include <stddef.h>

#include "sdk/eekernel.h"

/* semaphore guarding the newlib allocator, created by libkernl at start-up */
extern int D_001FE494;

void __malloc_lock(struct _reent *ptr);
void __malloc_unlock(struct _reent *ptr);

void *_memalign_r(struct _reent *ptr, size_t alignment, size_t bytes);
void *memalign(size_t alignment, size_t bytes);

#endif /* SONYLIBC_H */
