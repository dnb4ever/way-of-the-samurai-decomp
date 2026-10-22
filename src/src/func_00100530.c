void func_00100630(void);

void func_00100530(void) {
    void (*fp)(void) = func_00100630;
    fp();
}