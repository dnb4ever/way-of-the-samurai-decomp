/* func_001500F0.c -- buffer: character address */

#include "game/strbuf.h"

/*
================
func_001500F0

Returns the address of character index of the buffer.
================
*/
char *func_001500F0(Obj_001509B0 *buf, s32 index)
{
    return buf->unk_00 + index;
}
