/* alalc.c -- libmpeg: the linear allocator over the decoder's work area */

#include "sdk/libmpeg.h"

/*
================
_alalcInit
================
*/
void _alalcInit(Alalc *a, u8 *base, int size)
{
    a->base = base;
    a->size = size;
    a->pCurrent = base;
    a->pDynamic = base;
}

/*
================
_alalcSetDynamic

Marks the current position: _alalcFree returns to it.
================
*/
void _alalcSetDynamic(Alalc *a)
{
    a->pDynamic = a->pCurrent;
}

/*
================
_alalcFree

Releases everything allocated since _alalcSetDynamic.
================
*/
void _alalcFree(Alalc *a)
{
    a->pCurrent = a->pDynamic;
}

/*
================
func_0010D1E8

Allocates size bytes aligned to align, or reports that the work area is
too small and returns NULL.
================
*/
u8 *func_0010D1E8(MpegSys *sys, Alalc *a, int size, int align)
{
    u8 *p = (u8 *)(((u32)a->pCurrent + align - 1) / align * align);

    if (p + size <= a->base + a->size) {
        a->pCurrent = p + size;
        return p;
    }
    _Error(sys, "work area size is too small");
    return NULL;
}

/*
================
_alalcRest

Returns the bytes still free.
================
*/
int _alalcRest(Alalc *a)
{
    return a->base + a->size - a->pCurrent;
}
