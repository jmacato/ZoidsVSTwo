#include "os_context.h"
#include "os_error.h"

extern void PPCHalt(void);

void __DBExceptionDestinationAux(void)
{
    u32 *contextAddr = (u32 *)0xC0;
    OSContext *context = (OSContext *)(0x80000000 + *contextAddr);

    OSReport("DBExceptionDestination\n");
    OSDumpContext(context);
    PPCHalt();
}
