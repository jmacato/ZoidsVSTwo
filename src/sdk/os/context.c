/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/os/OSContext.c. */
#include "os_context.h"
#include "os.h"
#include "db.h"
#include "os_interrupt.h"
#pragma peephole off
#define NULL 0
extern OSContext *__OSCurrentContext;
__asm__(".set __OSCurrentContext, 0x800000D4");
extern OSContext *__OSFPUContext;
__asm__(".set __OSFPUContext, 0x800000D8");
extern void OSSwitchFPUContext(u8, OSContext *);
extern void OSReport(const char *, ...);
OSContext *OSGetCurrentContext(void)
{
    return (OSContext *)__OSCurrentContext;
}
void OSClearContext(register OSContext *context)
{
    context->mode = 0;
    context->state = 0;
    if (context == __OSFPUContext)
        __OSFPUContext = NULL;
}
void OSDumpContext(OSContext *context)
{
    u32 i;
    u32 *p;

    OSReport("------------------------- Context 0x%08x -------------------------\n", context);
    for (i = 0; i < 16; ++i) {
        OSReport("r%-2d  = 0x%08x (%14d)  r%-2d  = 0x%08x (%14d)\n", i, context->gpr[i], context->gpr[i],
                 i + 16, context->gpr[i + 16], context->gpr[i + 16]);
    }
    OSReport("LR   = 0x%08x                   CR   = 0x%08x\n", context->lr, context->cr);
    OSReport("SRR0 = 0x%08x                   SRR1 = 0x%08x\n", context->srr0, context->srr1);
    OSReport("\nGQRs----------\n");
    for (i = 0; i < 4; ++i) {
        OSReport("gqr%d = 0x%08x \t gqr%d = 0x%08x\n", i, context->gqr[i], i + 4, context->gqr[i + 4]);
    }
    if (context->state & OS_CONTEXT_STATE_FPSAVED) {
        OSContext *currentContext;
        OSContext fpuContext;
        s32 enabled;

        enabled = OSDisableInterrupts();
        currentContext = OSGetCurrentContext();
        OSClearContext(&fpuContext);
        OSSetCurrentContext(&fpuContext);
        OSReport("\n\nFPRs----------\n");
        for (i = 0; i < 32; i += 2) {
            OSReport("fr%d \t= %d \t fr%d \t= %d\n", i, (u32)context->fpr[i], i + 1, (u32)context->fpr[i + 1]);
        }
        OSReport("\n\nPSFs----------\n");
        for (i = 0; i < 32; i += 2) {
            OSReport("ps%d \t= 0x%x \t ps%d \t= 0x%x\n", i, (u32)context->psf[i], i + 1, (u32)context->psf[i + 1]);
        }
        OSClearContext(&fpuContext);
        OSSetCurrentContext(currentContext);
        OSRestoreInterrupts(enabled);
    }
    OSReport("\nAddress:      Back Chain    LR Save\n");
    for (i = 0, p = (u32 *)context->gpr[1]; p && (u32)p != 0xFFFFFFFF && i++ < 16; p = (u32 *)*p) {
        OSReport("0x%08x:   0x%08x    0x%08x\n", p, p[0], p[1]);
    }
}
void __OSContextInit(void)
{
    __OSSetExceptionHandler(__OS_EXCEPTION_FLOATING_POINT, OSSwitchFPUContext);
    __OSFPUContext = NULL;
    DBPrintf("FPU-unavailable handler installed\n");
}
