#include "context.h"

// matched by claude-1 (operator: you)
void *func_00181D80(void **arg0) {
    void *val;
    val = *arg0;
    val = ((void **)val)[1];
    *arg0 = val;
    return arg0;
}
