/* videoDecSetStream.c -- register a stream callback */

#include "game/ezmpeg.h"

/*
================
videoDecSetStream

Registers callback for stream type strType, channel ch, with libmpeg.
Returns 1.
================
*/
s32 videoDecSetStream(VideoDec *vd, s32 strType, s32 ch,
                      s32 (*callback)(sceMpeg *mp, sceMpegCbData *cbdata, void *data),
                      void *data)
{
    sceMpegAddStrCallback(&vd->mpeg, strType, ch, callback, data);
    return 1;
}
