/* libmc.c -- libmc: memory card access through the IOP's mcserv */

#include "sdk/eekernel.h"

/* what the IOP sends back for a GetInfo request */
typedef struct {
    int type;           /* 0x00 card type */
    int free;           /* 0x04 free clusters */
    u8  unk_08[0x88];
    int format;         /* 0x90 formatted or not */
} McInfoResult;

/* where the pending GetInfo request delivers its answers; each may be NULL */
static int *D_0022A5E8;  /* type */
static int *D_0022A5EC;  /* free */
static int *D_0022A5F0;  /* format */

/*
================
mceGetInfoApdx

End function of the GetInfo RPC: copies the answers out of the uncached
receive buffer into the caller's variables.
================
*/
void mceGetInfoApdx(void *data)
{
    McInfoResult *res = (McInfoResult *)((u32)data | UNCACHED_SEG);

    if (D_0022A5E8 != NULL) {
        *D_0022A5E8 = res->type;
    }
    if (D_0022A5EC != NULL) {
        *D_0022A5EC = res->free;
    }
    if (D_0022A5F0 != NULL) {
        *D_0022A5F0 = res->format;
    }
}
