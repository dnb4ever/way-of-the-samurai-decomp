extern u8 ref_handle_vtable[];
typedef struct { void *f0; void *f4; } S00166A30;
S00166A30 *func_00166A30(S00166A30 *arg0, void *arg1) {
    volatile S00166A30 *p = arg0;
    p->f4 = (void *)ref_handle_vtable;
    p->f0 = arg1;
    return arg0;
}