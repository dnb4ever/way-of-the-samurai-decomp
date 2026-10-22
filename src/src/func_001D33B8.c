s32 doprnt_core(void *ctx, s32 fmt, void *ap);

typedef struct {
    char *f0;
    s32 f4;
    s32 f8;
    u16 fC;
    u16 fE;
    char *f10;
    s32 f14;
    s32 f18[15];
    s32 f54;
} Ctx;

void func_001D33B8(s32 arg0, char *out, s32 fmt, ...) {
    Ctx ctx;
    void *ap;
    ap = (void *)((char *)__builtin_next_arg(fmt) - 0x28);
    ctx.f0 = out;
    ctx.f10 = out;
    ctx.f8 = 0x7FFFFFFF;
    ctx.fC = 0x208;
    ctx.f14 = 0x7FFFFFFF;
    ctx.f54 = arg0;
    doprnt_core(&ctx, fmt, ap);
    *ctx.f0 = 0;
}
