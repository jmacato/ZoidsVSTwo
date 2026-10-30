#include "types.h"

s32 Hu_IsStub(void)
{
    return 1;
}

void DBInitComm(void **pending, void (*callback)(s32, void *))
{
}

void DBInitInterrupts(void)
{
}

u32 DBQueryData(void)
{
    return 0;
}

u32 DBRead(void *data, u32 size)
{
    return 0;
}

u32 DBWrite(const void *data, u32 size)
{
    return 0;
}

void DBOpen(void)
{
}

void DBClose(void)
{
}
