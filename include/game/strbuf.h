/* strbuf.h -- a growable character buffer and the handles that hold it */
#ifndef GAME_STRBUF_H
#define GAME_STRBUF_H

#include "common.h"

/*
 * guess: the header's name. Evidence: func_001509B0 grows unk_00 with
 * func_001D29F8 and frees it with func_001D0970; callers reserve a length
 * plus one, then copy a string into unk_00 and write a terminating 0 at
 * unk_00 + unk_0C - 1.
 */
typedef struct Obj_001509B0 {
    char *unk_00;     /* 0x00 characters */
    s32   unk_04;     /* 0x04 allocated size */
    u8    unk_08[4];
    s32   unk_0C;     /* 0x0C size in use */
} Obj_001509B0;

/* a handle to a buffer; the getters below are the methods of several
   handle classes with this first member */
typedef struct Obj_00150100 {
    Obj_001509B0 *unk_00;  /* 0x00 */
} Obj_00150100;

Obj_001509B0 *func_00150100(Obj_00150100 *handle);
Obj_001509B0 *func_00150110(Obj_00150100 *handle);
Obj_001509B0 *func_00150900(Obj_00150100 *handle);
Obj_001509B0 *func_001509A0(Obj_00150100 *handle);
Obj_001509B0 *func_00150AD0(Obj_00150100 *handle);

char *func_001500F0(Obj_001509B0 *buf, s32 index);
s32   func_001508F0(Obj_001509B0 *buf);
char *func_00150990(Obj_001509B0 *buf);

#endif /* GAME_STRBUF_H */
