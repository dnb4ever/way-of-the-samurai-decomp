/* func_001EDD10.c -- decoder callback */

#include "game/ezmpeg.h"

/*
================
func_001EDD10

The callback videoDecCreate registers with libmpeg for callback type 0.
Returns 1.
================
*/
s32 func_001EDD10(sceMpeg *mp, sceMpegCbData *cbdata, void *data)
{
    return 1;
}
