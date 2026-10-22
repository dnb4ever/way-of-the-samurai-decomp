#define F24(p) (*(s32 *)((u8 *)(p) + 0x24))

s32 func_00175B40(void *arg0, void *arg1)
{
    return F24(arg0) >= F24(arg1);
}

s32 func_00175B60(void *arg0, void *arg1)
{
    return F24(arg0) > F24(arg1);
}

s32 func_00175B70(void *arg0, void *arg1)
{
    return F24(arg0) <= F24(arg1);
}

s32 func_00175BA0(void *arg0, void *arg1)
{
    return F24(arg0) == F24(arg1);
}