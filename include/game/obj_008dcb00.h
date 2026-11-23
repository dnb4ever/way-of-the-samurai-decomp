/* obj_008dcb00.h -- the global object at 0x8DCB00 (bss) */
#ifndef GAME_OBJ_008DCB00_H
#define GAME_OBJ_008DCB00_H

#include "common.h"

struct Obj_001B83E0;

/* reached with absolute addressing (%hi/%lo), never through $gp */
typedef struct Obj_008DCB00 {
    u8                   unk_00[0x70];
    struct Obj_001B83E0 *unk_70;  /* 0x70 */
} Obj_008DCB00;

extern Obj_008DCB00 D_008DCB00;

#endif /* GAME_OBJ_008DCB00_H */
