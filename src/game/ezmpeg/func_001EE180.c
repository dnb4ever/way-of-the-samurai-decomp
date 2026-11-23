/* func_001EE180.c -- decoder word at 0xA8 */

#include "game/ezmpeg.h"

/*
================
func_001EE180

Returns the decoder's word at 0xA8; the player compares it with 3.
================
*/
s32 func_001EE180(VideoDec *vd)
{
    return vd->unk_A8;
}
