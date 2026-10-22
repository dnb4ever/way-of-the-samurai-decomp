void *func_001858A0(void **arg0) {
    void *val;
    val = *arg0;
    val = ((void **)val)[1];
    *arg0 = val;
    return arg0;
}