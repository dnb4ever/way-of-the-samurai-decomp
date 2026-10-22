void *func_00136210(void **arg0) {
    void *val;
    val = *(void **)arg0;
    val = *(void **)val;
    *arg0 = (void **)val;
    return arg0;
}