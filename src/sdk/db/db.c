#include "types.h"

typedef struct DBInterface {
    u32 present;
    u32 exceptionMask;
    void (*exceptionDestination)(void);
    void *exceptionReturn;
} DBInterface;

extern DBInterface *__DBInterface;
extern s32 DBVerbose;
extern void __DBExceptionDestination(void);

void DBInit(void)
{
    __DBInterface = (DBInterface *)0x80000040;
    __DBInterface->exceptionDestination =
        (void (*)(void))((u32)__DBExceptionDestination - 0x80000000);
    DBVerbose = 1;
}

s32 __DBIsExceptionMarked(u8 exception)
{
    u32 mask = 1 << exception;
    return __DBInterface->exceptionMask & mask;
}
