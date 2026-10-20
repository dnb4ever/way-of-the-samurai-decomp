#include "context.h"

// matched by api-8 (operator: you)
extern u8 D_00223980[];

typedef struct { void *f0; void *f4; } S001525A0;

S001525A0 *func_001525A0(S001525A0 *arg0, void *arg1) {
    volatile S001525A0 *p = arg0;
    p->f4 = (void *)D_00223980;
    p->f0 = arg1;
    return arg0;
}
