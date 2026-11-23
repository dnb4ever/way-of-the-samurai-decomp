/* func_001EE4C0.c -- reset a VoBuf */

#include "game/ezmpeg.h"

/*
================
func_001EE4C0

Empties the buffer: clears its words at 0x8 and 0xC.
================
*/
void func_001EE4C0(VoBuf *buf)
{
    buf->unk_08 = 0;
    buf->unk_0C = 0;
}
