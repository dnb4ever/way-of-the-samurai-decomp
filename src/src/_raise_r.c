extern s32 _init_signal_r(void *);
extern s32 _kill_r(void *, s32, u32);
extern s32 _getpid_r(void *);

typedef s32 (*SigHandler)(u32);

s32 _raise_r(void *obj, u32 sig) {
    SigHandler *table;
    SigHandler handler;

    if (sig >= 0x20) {
        *(s32 *)obj = 0x16;
        return -1;
    }
    table = (SigHandler *)*(s32 *)((char *)obj + 0x1D4);
    if (table == 0) {
        if (_init_signal_r(obj) != 0) {
            return -1;
        }
        table = (SigHandler *)*(s32 *)((char *)obj + 0x1D4);
    }
    handler = table[sig];
    if (handler == 0) {
        return _kill_r(obj, _getpid_r(obj), sig);
    }
    if (handler == (SigHandler)1) {
        return 0;
    }
    if (handler == (SigHandler)-1) {
        *(s32 *)obj = 0x16;
        return 1;
    }
    table[sig] = 0;
    handler(sig);
    return 0;
}