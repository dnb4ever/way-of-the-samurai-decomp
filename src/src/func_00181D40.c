void *func_00181D40(void **arg0) {
    void *val;
    val = *arg0;
    val = ((void **)val)[1];
    *arg0 = val;
    return arg0;
}

void func_0019C560(void *arg0) {
    void (*fn)(void *) = *(void **)((u8 *)arg0 + 4);
    fn = *(void **)((u8 *)fn + 0xC);
    fn(arg0);
}

// Released immediately: frameless jr $t9 (vcall thunk)
// See project note: INDIRECT-SIBCALL FLOOR. Unmatchable
// No compiles needed.