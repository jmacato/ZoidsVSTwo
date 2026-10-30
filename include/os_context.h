/* Types follow
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/os/OSContext.h. */
#ifndef ZOIDS_OS_CONTEXT_H
#define ZOIDS_OS_CONTEXT_H
#include "types.h"
#define OS_CONTEXT_STATE_FPSAVED 0x01u
#define OS_CONTEXT_STATE_EXC 0x02u
typedef struct OSThread OSThread;
typedef struct OSContext {
    u32 gpr[32];
    u32 cr, lr, ctr, xer;
    f64 fpr[32];
    u32 fpscr_pad, fpscr;
    u32 srr0, srr1;
    u16 mode, state;
    u32 gqr[8];
    f64 psf[32];
} OSContext;
typedef struct OSThreadQueue {
    OSThread *head;
    OSThread *tail;
} OSThreadQueue;
extern OSContext *OSGetCurrentContext(void);
extern void OSClearContext(OSContext *context);
extern void OSSetCurrentContext(OSContext *context);
extern void OSLoadContext(OSContext *context);
extern void OSInitContext(OSContext *context, u32 pc, u32 sp);
extern void OSDumpContext(OSContext *context);
extern void __OSContextInit(void);
#endif
