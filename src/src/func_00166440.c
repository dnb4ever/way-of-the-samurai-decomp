extern u8 list_iter_vtable[];
typedef struct { void *f0; void *f4; } S166440;
S166440 *func_00166440(S166440 *arg0, void *arg1) {
    volatile S166440 *p = arg0;
    p->f4 = (void *)list_iter_vtable;
    p->f0 = arg1;
    return arg0;
}

typedef void (*fp24_t)(void *);

void func_00166530(void *arg0) {
    fp24_t fp;
    while ((fp = *(fp24_t *)((char *)arg0 + 0x24)) != 0) {
        fp(arg0);
    }
}

s32 func_00166580(void *arg0) {
    return *(s32 *)((char *)arg0 + 0x24) == 0;
}

void func_00166790(void *arg0) {
    fp24_t fp = *(fp24_t *)((char *)arg0 + 0x24);
    if (fp != 0) fp(arg0);
}