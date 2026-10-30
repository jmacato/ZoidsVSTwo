/* References:
 * https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/GXFifo.c
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/os/OSContext.h.
 */
#include "gx_fifo.h"
#include "gx_state.h"
#include "os_context.h"
#include "os_interrupt.h"
struct __GXFifoObj {
    u8 *base;
    u8 *top;
    u32 size;
    u32 hiWatermark;
    u32 loWatermark;
    void *rdPtr;
    void *wrPtr;
    s32 count;
    u8 bind_cpu;
    u8 bind_gp;
};
typedef void (*GXBreakPtCallback)(void);
extern u16 *__cpReg;
extern u32 *__piReg;
extern s32 OSSuspendThread(OSThread *);
extern s32 OSResumeThread(OSThread *);
extern OSThread *OSGetCurrentThread(void);

static void GXCPInterruptHandler(s16 interrupt, OSContext *context);
static void __GXFifoReadEnable(void);
static void __GXFifoReadDisable(void);
static void __GXFifoLink(u8 en);
static void __GXWriteFifoIntEnable(u8 hiWatermarkEn, u8 loWatermarkEn);
static void __GXWriteFifoIntReset(u8 hiWatermarkClr, u8 loWatermarkClr);

#define PHYSICAL(x) (((u32)(x)) & 0x3FFFFFFF)
static struct __GXFifoObj *CPUFifo = 0;
static struct __GXFifoObj *GPFifo = 0;
static OSThread *__GXCurrentThread = 0;
static u8 CPGPLinked = 0;
static s32 GXOverflowSuspendInProgress = 0;
static GXBreakPtCallback BreakPointCB = 0;
static u32 __GXOverflowCount = 0;
static inline void GXOverflowHandler(s16 interrupt, OSContext *context)
{

    __GXOverflowCount++;
    __GXWriteFifoIntEnable(0, 1);
    __GXWriteFifoIntReset(1, 0);
    GXOverflowSuspendInProgress = 1;

    OSSuspendThread(__GXCurrentThread);
}

static inline void GXUnderflowHandler(s16 interrupt, OSContext *context)
{

    OSResumeThread(__GXCurrentThread);
    GXOverflowSuspendInProgress = 0;
    __GXWriteFifoIntReset(1U, 1U);
    __GXWriteFifoIntEnable(1U, 0U);
}

static inline void GXBreakPointHandler(s16 interrupt, OSContext *context)
{
    OSContext exceptionContext;

    gx->cpEnable = gx->cpEnable & 0xFFFFFFDF;
    __cpReg[1] = gx->cpEnable;
    if (BreakPointCB != 0) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        BreakPointCB();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
}

static void GXCPInterruptHandler(s16 interrupt, OSContext *context)
{
    gx->cpStatus = __cpReg[0];
    if (GX_GET_FIELD(gx->cpEnable, 1, 3) && GX_GET_FIELD(gx->cpStatus, 1, 1)) {
        GXUnderflowHandler(interrupt, context);
    }
    if (GX_GET_FIELD(gx->cpEnable, 1, 2) && GX_GET_FIELD(gx->cpStatus, 1, 0)) {
        GXOverflowHandler(interrupt, context);
    }
    if (GX_GET_FIELD(gx->cpEnable, 1, 5) && GX_GET_FIELD(gx->cpStatus, 1, 4)) {
        GXBreakPointHandler(interrupt, context);
    }
}

#pragma dont_inline on
void GXInitFifoBase(GXFifoObj *fifo, void *base, u32 size)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;

    realFifo->base = base;
    realFifo->top = (u8 *)base + size - 4;
    realFifo->size = size;
    realFifo->count = 0;
    GXInitFifoLimits(fifo, size - 0x4000, (size >> 1) & ~0x1F);
    GXInitFifoPtrs(fifo, base, base);
}

void GXInitFifoPtrs(GXFifoObj *fifo, void *readPtr, void *writePtr)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;
    s32 enabled;

    enabled = OSDisableInterrupts();
    realFifo->rdPtr = readPtr;
    realFifo->wrPtr = writePtr;
    realFifo->count = (u8 *)writePtr - (u8 *)readPtr;
    if (realFifo->count < 0) {
        realFifo->count += realFifo->size;
    }
    OSRestoreInterrupts(enabled);
}

void GXInitFifoLimits(GXFifoObj *fifo, u32 hiWatermark, u32 loWatermark)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;

    realFifo->hiWatermark = hiWatermark;
    realFifo->loWatermark = loWatermark;
}

void GXSetCPUFifo(GXFifoObj *fifo)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;
    s32 enabled = OSDisableInterrupts();

    CPUFifo = realFifo;
    if (CPUFifo == GPFifo) {
        u32 reg = 0;

        __piReg[3] = PHYSICAL(realFifo->base);
        __piReg[4] = PHYSICAL(realFifo->top);
        GX_SET_FIELD(reg, 21, 5, PHYSICAL(realFifo->wrPtr) >> 5);
        GX_SET_FIELD(reg, 1, 26, 0);
        __piReg[5] = reg;
        CPGPLinked = 1;
        __GXWriteFifoIntReset(1, 1);
        __GXWriteFifoIntEnable(1, 0);
        __GXFifoLink(1);
    } else {
        u32 reg;

        if (CPGPLinked) {
            __GXFifoLink(0);
            CPGPLinked = 0;
        }
        __GXWriteFifoIntEnable(0, 0);
        reg = 0;
        __piReg[3] = PHYSICAL(realFifo->base);
        __piReg[4] = PHYSICAL(realFifo->top);
        GX_SET_FIELD(reg, 21, 5, PHYSICAL(realFifo->wrPtr) >> 5);
        GX_SET_FIELD(reg, 1, 26, 0);
        __piReg[5] = reg;
    }

    __sync();

    OSRestoreInterrupts(enabled);
}

void GXSetGPFifo(GXFifoObj *fifo)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;
    s32 enabled = OSDisableInterrupts();

    __GXFifoReadDisable();
    __GXWriteFifoIntEnable(0, 0);
    GPFifo = realFifo;

    __cpReg[16] = (u32)realFifo->base & 0xFFFF;
    __cpReg[18] = (u32)realFifo->top & 0xFFFF;
    __cpReg[24] = realFifo->count & 0xFFFF;
    __cpReg[26] = (u32)realFifo->wrPtr & 0xFFFF;
    __cpReg[28] = (u32)realFifo->rdPtr & 0xFFFF;
    __cpReg[20] = (u32)realFifo->hiWatermark & 0xFFFF;
    __cpReg[22] = (u32)realFifo->loWatermark & 0xFFFF;
    __cpReg[17] = PHYSICAL(realFifo->base) >> 16;
    __cpReg[19] = PHYSICAL(realFifo->top) >> 16;
    __cpReg[25] = realFifo->count >> 16;
    __cpReg[27] = PHYSICAL(realFifo->wrPtr) >> 16;
    __cpReg[29] = PHYSICAL(realFifo->rdPtr) >> 16;
    __cpReg[21] = (u32)realFifo->hiWatermark >> 16;
    __cpReg[23] = (u32)realFifo->loWatermark >> 16;

    __sync();

    if (CPUFifo == GPFifo) {
        CPGPLinked = 1;
        __GXWriteFifoIntEnable(1, 0);
        __GXFifoLink(1);
    } else {
        CPGPLinked = 0;
        __GXWriteFifoIntEnable(0, 0);
        __GXFifoLink(0);
    }
    __GXWriteFifoIntReset(1, 1);
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}

void __GXFifoInit(void)
{
    __OSSetInterruptHandler(__OS_INTERRUPT_PI_CP, GXCPInterruptHandler);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_CP);
    __GXCurrentThread = OSGetCurrentThread();
    GXOverflowSuspendInProgress = 0;
    CPUFifo = 0;
    GPFifo = 0;
}

static void __GXFifoReadEnable(void)
{
    GX_SET_FIELD(gx->cpEnable, 1, 0, 1);
    __cpReg[1] = gx->cpEnable;
}

static void __GXFifoReadDisable(void)
{
    GX_SET_FIELD(gx->cpEnable, 1, 0, 0);
    __cpReg[1] = gx->cpEnable;
}

static void __GXFifoLink(u8 en)
{
    GX_SET_FIELD(gx->cpEnable, 1, 4, (en != 0) ? 1 : 0);
    __cpReg[1] = gx->cpEnable;
}

static void __GXWriteFifoIntEnable(u8 hiWatermarkEn, u8 loWatermarkEn)
{
    GX_SET_FIELD(gx->cpEnable, 1, 2, hiWatermarkEn);
    GX_SET_FIELD(gx->cpEnable, 1, 3, loWatermarkEn);
    __cpReg[1] = gx->cpEnable;
}

static void __GXWriteFifoIntReset(u8 hiWatermarkClr, u8 loWatermarkClr)
{
    GX_SET_FIELD(gx->cpClr, 1, 0, hiWatermarkClr);
    GX_SET_FIELD(gx->cpClr, 1, 1, loWatermarkClr);
    __cpReg[2] = gx->cpClr;
}