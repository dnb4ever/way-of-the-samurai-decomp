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

void func_00104370(s8 *arg0, s32 arg1) {
    s32 var_2;
    s8 *var_4;

    var_4 = arg0;
    var_2 = arg1 - 1;
    if (arg1 != 0) {
        do {
            *var_4 = 0;
            var_2 -= 1;
            var_4 += 1;
        } while (var_2 != -1);
    }
}
