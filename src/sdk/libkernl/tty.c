/* tty.c -- libkernl: the host console (DECI2 TTY protocol) */

#include "common.h"

/* a byte ring between the DECI2 handler and sceTtyRead */
typedef struct {
    int  size;      /* 0x00 bytes in buf */
    int  count;     /* 0x04 bytes queued */
    u8  *pRead;     /* 0x08 next byte to read */
    u8  *pWrite;    /* 0x0C next byte to write */
    u8   buf[256];  /* 0x10 */
} TtyQueue;

static TtyQueue D_002256C0;

/*
================
QueueInit
================
*/
TtyQueue *QueueInit(int size)
{
    TtyQueue *q = &D_002256C0;

    q->size = size;
    q->count = 0;
    q->pWrite = q->buf;
    q->pRead = q->buf;
    return q;
}

/*
================
QueuePeekWriteDone

Commits the byte written at pWrite.
================
*/
void QueuePeekWriteDone(TtyQueue *q)
{
    q->count++;
    q->pWrite++;
    if (q->pWrite == &q->buf[q->size]) {
        q->pWrite = q->buf;
    }
}

/*
================
func_00110048

Releases the byte read at pRead.
================
*/
void func_00110048(TtyQueue *q)
{
    q->count--;
    q->pRead++;
    if (q->pRead == &q->buf[q->size]) {
        q->pRead = q->buf;
    }
}
