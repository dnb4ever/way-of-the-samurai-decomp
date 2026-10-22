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

void func_0010CF88(void *arg0, s32 *arg1, s32 *arg2, s32 *arg3) {
    void *temp_8;

    temp_8 = M2C_FIELD(arg0, void **, 0x40);
    *arg1 = M2C_FIELD(temp_8, s32 *, 0x94);
    *arg2 = M2C_FIELD(temp_8, s32 *, 0x98);
    *arg3 = M2C_FIELD(temp_8, s32 *, 0x9C);
}
