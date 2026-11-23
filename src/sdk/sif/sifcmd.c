/* sifcmd.c -- SIF commands: dispatching the IOP's messages to handlers */

#include "sdk/eekernel.h"
#include "sdk/sifcmd.h"

/* the system command a sreg packet carries */
typedef struct {
    sceSifCmdHdr hdr;
    int          index;     /* 0x10 */
    int          value;     /* 0x14 */
} SifSregPacket;

/* the IOP announcing where its command buffer is */
typedef struct {
    sceSifCmdHdr hdr;
    void        *buf;       /* 0x10 */
} SifChangeAddrPacket;

/* the dispatcher's state */
typedef struct {
    void          *pktbuf;          /* 0x00 */
    void          *unk_04;
    void          *iopbuf;          /* 0x08 the IOP's command buffer */
    sceSifCmdData *sysCmdBuffer;    /* 0x0C */
    int            sysCmdSize;      /* 0x10 */
    sceSifCmdData *cmdBuffer;       /* 0x14 */
    int            cmdSize;         /* 0x18 */
    int           *sregs;           /* 0x1C */
} SifCmdState;

static int           D_001FDD90 = 0;      /* set by sceSifInitCmd */
static u128          D_00225B00[8];       /* packet buffer */
static u128          D_00225B80[4];
static u32           D_00225BC0[5];
static int           D_00225BD4;          /* the SIF0 DMAC handler */
static SifCmdState   D_00225BD8;
static sceSifCmdData D_00225C00[32] __attribute__((aligned(16)));  /* the system command handlers */
static int           D_00225D00[32];      /* the software registers */

/*
================
func_00110E78

System command handler: sets a software register.
================
*/
void func_00110E78(SifSregPacket *pkt, SifCmdState *state)
{
    state->sregs[pkt->index] = pkt->value;
}

/*
================
func_00110E98

System command handler: records the IOP's command buffer address.
================
*/
void func_00110E98(SifChangeAddrPacket *pkt, SifCmdState *state)
{
    state->iopbuf = pkt->buf;
}

/*
================
sceSifGetSreg
================
*/
int sceSifGetSreg(int reg)
{
    return D_00225D00[reg];
}

/*
================
sceSifSetSreg
================
*/
int sceSifSetSreg(int reg, int value)
{
    D_00225D00[reg] = value;
    return value;
}

/*
================
func_00110EE0
================
*/
SifCmdState *func_00110EE0(void)
{
    return &D_00225BD8;
}

/*
================
sceSifExitCmd
================
*/
void sceSifExitCmd(void)
{
    DisableDmac(DMAC_SIF0);
    RemoveDmacHandler(DMAC_SIF0, D_00225BD4);
    D_001FDD90 = 0;
}

/*
================
sceSifSetCmdBuffer

Installs the user command handler table; returns the previous one.
================
*/
sceSifCmdData *sceSifSetCmdBuffer(sceSifCmdData *buf, int size)
{
    sceSifCmdData *old = D_00225BD8.cmdBuffer;

    D_00225BD8.cmdBuffer = buf;
    D_00225BD8.cmdSize = size;
    return old;
}

/*
================
sceSifSetSysCmdBuffer
================
*/
sceSifCmdData *sceSifSetSysCmdBuffer(sceSifCmdData *buf, int size)
{
    sceSifCmdData *old = D_00225BD8.sysCmdBuffer;

    D_00225BD8.sysCmdBuffer = buf;
    D_00225BD8.sysCmdSize = size;
    return old;
}

/*
================
sceSifAddCmdHandler
================
*/
void sceSifAddCmdHandler(int cid, sceSifCmdHandler handler, void *harg)
{
    if (cid < 0) {
        D_00225BD8.sysCmdBuffer[cid & ~SIF_CMD_ID_SYSTEM].func = handler;
        D_00225BD8.sysCmdBuffer[cid & ~SIF_CMD_ID_SYSTEM].data = harg;
    } else {
        D_00225BD8.cmdBuffer[cid].func = handler;
        D_00225BD8.cmdBuffer[cid].data = harg;
    }
}

/*
================
sceSifRemoveCmdHandler
================
*/
void sceSifRemoveCmdHandler(int cid)
{
    if (cid < 0) {
        D_00225BD8.sysCmdBuffer[cid & ~SIF_CMD_ID_SYSTEM].func = NULL;
    } else {
        D_00225BD8.cmdBuffer[cid].func = NULL;
    }
}
