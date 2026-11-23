/* sifcmd.h -- SIF commands: messages between the EE and the IOP */
#ifndef SDK_SIFCMD_H
#define SDK_SIFCMD_H

#include "common.h"

/* command IDs with this bit set are the system's own */
#define SIF_CMD_ID_SYSTEM   0x80000000

/* the header every command packet starts with */
typedef struct {
    u32   psize;    /* 0x00 packet size */
    void *dest;     /* 0x04 */
    int   cid;      /* 0x08 command ID */
    u32   opt;      /* 0x0C */
} sceSifCmdHdr;

typedef void (*sceSifCmdHandler)(void *data, void *harg);

typedef struct {
    sceSifCmdHandler func;  /* 0x00 */
    void            *data;  /* 0x04 */
} sceSifCmdData;

int              sceSifGetSreg(int reg);
int              sceSifSetSreg(int reg, int value);
void             sceSifExitCmd(void);
sceSifCmdData   *sceSifSetCmdBuffer(sceSifCmdData *buf, int size);
sceSifCmdData   *sceSifSetSysCmdBuffer(sceSifCmdData *buf, int size);
void             sceSifAddCmdHandler(int cid, sceSifCmdHandler handler, void *harg);
void             sceSifRemoveCmdHandler(int cid);

#endif /* SDK_SIFCMD_H */
