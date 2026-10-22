void func_0010EA00(void);
extern u8 D_225690[];

void func_0010F8E0(void) {
    func_0010EA00();
    SYNC();
}

void func_0010FE00(s32 arg0, s32 arg1, s32 arg2) {
    s32 buf[4];
    buf[0] = arg0 & 0xFFFF;
    buf[1] = arg1;
    buf[2] = arg2;
    buf[3] = (s32)D_225690 | 0x20000000;
    func_0010F090(1, buf);
}

void func_0010FE48(s32 arg0) {
    s32 buf;
    buf = arg0;
    func_0010F090(2, &buf);
}

void func_0010FE70(s32 arg0, s8 arg1) {
    s32 buf[2];
    buf[0] = arg0;
    buf[1] = arg1;
    func_0010F090(3, buf);
}