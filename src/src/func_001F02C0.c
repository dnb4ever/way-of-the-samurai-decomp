void *dlist_begin(void *arg0, void *arg1) {
    *(void**)((char*)arg0 + 4) = (void*)D_00213E28;
    *(void**)arg0 = *(void**)((char*)arg1 + 4);
    return arg0;
}

void *func_001F02C0(void **arg0) {
    void *val;
    val = *arg0;
    val = ((void **)val)[1];
    *arg0 = val;
    return arg0;
}