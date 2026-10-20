#include "context.h"

// matched by autopilot (operator: you)
/* machine draft (m2c), tried by autopilot */
#ifndef M2C_DEFINES
#define M2C_DEFINES
typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
#define M2C_BITWISE(type, expr) ((type)(expr))
#define M2C_ERROR(desc) (0)
#define M2C_CARRY 0
#define M2C_OVERFLOW(a) (0)
#endif

u32 func_0010BD80(void *arg0, s32 arg1) {
    u32 var_3;

    var_3 = M2C_FIELD(arg0, s32 *, 8) + (arg1 >> 3);
    if (var_3 >= (u32) M2C_FIELD(arg0, u32 *, 0x24)) {
        var_3 -= M2C_FIELD(arg0, s32 *, 0x28);
    }
    return var_3;
}
