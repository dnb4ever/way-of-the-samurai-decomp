extern u8 D_00223E10[];
typedef struct { void *f0; void *f4; } ST_func_0019BB40;
ST_func_0019BB40 *func_0019BB40(ST_func_0019BB40 *arg0, void *arg1) {
    arg1 = ((void * volatile *)arg1)[1];
    arg0->f4 = (void *)D_00223E10;
    arg0->f0 = arg1;
    return arg0;
}