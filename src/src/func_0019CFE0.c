void func_001B43C0(void);
void func_001B5240(void);

void func_0019CFE0(void) {
    void (*fp)(void);
    func_001B43C0();
    fp = func_001B5240;
    fp();
}