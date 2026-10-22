extern u8 D_00223B80[];
typedef struct { void *f0; void *f4; } ST_func_00174170;
ST_func_00174170 *func_00174170(ST_func_00174170 *arg0, void *arg1) {
    arg1 = ((void * volatile *)arg1)[1];
    arg0->f4 = (void *)D_00223B80;
    arg0->f0 = arg1;
    return arg0;
}