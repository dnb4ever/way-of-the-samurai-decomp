#include "context.h"

// matched by autopilot (operator: you)
/* identical to func_00134970 (matched), with its names changed by decomp.wtf */
extern u8 D_00223670[];
typedef struct { void *f0; void *f4; } S00134970;
S00134970 *func_001B9470(S00134970 *arg0, void *arg1) {
    volatile S00134970 *p = arg0;
    p->f4 = (void *)D_00223670;
    p->f0 = arg1;
    return arg0;
}
