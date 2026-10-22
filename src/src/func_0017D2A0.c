extern u8 list_iter_vtable[];
typedef struct { void *f0; void *f4; } S0017D2A0;
S0017D2A0 *func_0017D2A0(S0017D2A0 *arg0, void *arg1) {
    volatile S0017D2A0 *p = arg0;
    p->f4 = (void *)list_iter_vtable;
    p->f0 = arg1;
    return arg0;
}

void func_001F7450(void *arg0);
void func_0017EFF0(u8 *arg0) {
    func_001F7450(arg0 + 0x680);
}