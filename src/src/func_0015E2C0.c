extern u8 D_00223A40[];
typedef struct { void *f0; void *f4; } S0015E2C0;
S0015E2C0 *func_0015E2C0(S0015E2C0 *arg0, void *arg1) {
    volatile S0015E2C0 *p = arg0;
    p->f4 = (void *)D_00223A40;
    p->f0 = arg1;
    return arg0;
}

long func_0015DA10(long, long, long, long, long, long);

long func_0015E2E0(long a0, long a1, long a2, long a3) {
    return func_0015DA10(a0, 0, a1, a2, 0xFFF001, a3);
}

void func_0015E300(long a0, long a1, long a2) {
    long v16 = 1, v19 = 0x10;
    do {
        long v17 = 1, v18 = 8;
        do {
            func_0015DA10(a0, 0, a1 + v19, a2 + v18, 0xFFF001, 0x2000000);
            v17 += 2;
            v18 += 0x10;
        } while (v17 < 0xF);
        v16 += 2;
        v19 += 0x20;
    } while (v16 < 0xF);
}

void func_0015DD20(int, int, int *);

void func_0015E3B0(int arg0, int arg1) {
    int sp[4];
    sp[0] = 0x7E60;
    sp[3] = arg1;
    sp[1] = 0x7E70;
    sp[2] = 0xFFF001;
    func_0015DD20(arg0, 7, sp);
}