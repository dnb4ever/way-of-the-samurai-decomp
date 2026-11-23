/* sifrpc.c -- SIF remote procedure calls to IOP modules */

#include "sdk/eekernel.h"
#include "sdk/sifcmd.h"

/* rec_id flags of an RPC packet */
#define RPC_PACKET_ALLOC    0x01    /* in use */
#define RPC_PACKET_RPC      0x04

/* the header every RPC packet starts with; packets are 64 bytes apart */
typedef struct {
    sceSifCmdHdr hdr;
    u32          recId;     /* 0x10 slot << 16 | RPC_PACKET_* */
    void        *pktAddr;   /* 0x14 */
    int          rpcId;     /* 0x18 */
} SifRpcPktHdr;

#define RPC_PACKET_SIZE     64

/* the RPC layer's packet tables */
typedef struct {
    int  pid;               /* 0x00 last packet ID handed out */
    u8  *pktTable;          /* 0x04 */
    int  pktTableLen;       /* 0x08 */
    int  unk_0C;
    int  unk_10;
    u8  *rdataTable;        /* 0x14 */
    int  rdataTableLen;     /* 0x18 */
    u8  *clientTable;       /* 0x1C */
    int  clientTableLen;    /* 0x20 */
    int  rdataTableIdx;     /* 0x24 */
} SifRpcData;

static int D_001FDD98 = 0;  /* set by sceSifInitRpc */

/*
================
sceSifExitRpc
================
*/
void sceSifExitRpc(void)
{
    sceSifExitCmd();
    D_001FDD98 = 0;
}

#ifdef NON_MATCHING
/*
================
func_00111788

Takes a free packet from the packet table and stamps it with a new ID;
NULL when all are in use. Parked: when the new ID is 1, retail reloads
the constant 1 for rpcId instead of reusing the register that holds it.
================
*/
SifRpcPktHdr *func_00111788(SifRpcData *rpc)
{
    SifRpcPktHdr *pkt;
    int           i;
    int           pid;

    DIntr();
    pkt = (SifRpcPktHdr *)rpc->pktTable;
    for (i = 0; i < rpc->pktTableLen; i++) {
        if (!(pkt->recId & RPC_PACKET_ALLOC)) {
            pkt->recId = (i << 16) | RPC_PACKET_RPC | RPC_PACKET_ALLOC;
            pid = ++rpc->pid;
            if (pid == 1) {
                rpc->pid++;
            }
            pkt->pktAddr = pkt;
            pkt->rpcId = pid;
            EIntr();
            return pkt;
        }
        pkt = (SifRpcPktHdr *)((u8 *)pkt + RPC_PACKET_SIZE);
    }
    EIntr();
    return NULL;
}
#endif

/*
================
func_00111830

Returns a packet to the packet table.
================
*/
void func_00111830(SifRpcPktHdr *pkt)
{
    pkt->rpcId = 0;
    pkt->recId &= ~RPC_PACKET_ALLOC;
}

/*
================
func_00111850

Takes the next receive-data packet, round robin.
================
*/
u8 *func_00111850(SifRpcData *rpc)
{
    int index = rpc->rdataTableIdx % rpc->rdataTableLen;
    u8 *pkt = rpc->rdataTable + index * RPC_PACKET_SIZE;

    rpc->rdataTableIdx = index + 1;
    return pkt;
}

/*
================
func_00111880

Returns client packet rid, or a receive-data packet when rid is out of range.
================
*/
u8 *func_00111880(SifRpcData *rpc, int rid)
{
    if (rid < 0 || rid >= rpc->clientTableLen) {
        return func_00111850(rpc);
    }
    return rpc->clientTable + rid * RPC_PACKET_SIZE;
}
