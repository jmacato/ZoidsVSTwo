/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/GXMisc.c. */
#include "gx_misc.h"
#include "gx_state.h"
#include "os_context.h"
#include "os_interrupt.h"
extern u16 *__peReg;
extern void PPCSync(void);
extern void OSSleepThread(OSThreadQueue *);
extern void OSWakeupThread(OSThreadQueue *);
extern void OSInitThreadQueue(OSThreadQueue *);
static GXDrawSyncCallback TokenCB = 0;
static GXDrawDoneCallback DrawDoneCB = 0;
static u8 DrawDone = 0;
static OSThreadQueue FinishQueue;
void GXSetMisc(GXMiscToken token, u32 val)
{
    switch (token) {
    case GX_MT_XF_FLUSH:
        gx->vNum = val;
        gx->vcdClear = !gx->vNum;
        gx->bpSent = 1;
        if (gx->vNum != 0) {
            gx->dirtyState |= 8;
        }
        break;
    case GX_MT_DL_SAVE_CONTEXT:

        gx->dlSaveContext = (val > 0);
        break;
    case GX_MT_NULL:
        break;
    default:

        break;
    }
}

void GXFlush(void)
{
    u32 i;

    if (gx->dirtyState) {
        __GXSetDirtyState();
    }
    for (i = 8; i > 0; i--) {
        GX_WRITE_U32(0);
    }
    PPCSync();
}

void GXWaitDrawDone(void)
{
    s32 enabled;

    enabled = OSDisableInterrupts();
    while (!DrawDone) {
        OSSleepThread(&FinishQueue);
    }
    OSRestoreInterrupts(enabled);
}

#pragma dont_inline on
void GXDrawDone(void)
{

    u32 reg;
    s32 enabled;

    enabled = OSDisableInterrupts();
    reg = 0x45000002;
    GX_WRITE_RAS_REG(reg);
    GXFlush();
    DrawDone = 0;
    OSRestoreInterrupts(enabled);

    {
        s32 enabled = OSDisableInterrupts();
        while (!DrawDone)
            OSSleepThread(&FinishQueue);
        OSRestoreInterrupts(enabled);
    }
}

#pragma dont_inline off
void GXPixModeSync(void)
{

    GX_WRITE_RAS_REG(gx->peCtrl);
    gx->bpSent = 0;
}

void GXPokeAlphaMode(GXCompare func, u8 threshold)
{
    u32 reg;

    reg = (func << 8) | threshold;
    __peReg[3] = reg;
}

void GXPokeAlphaRead(GXAlphaReadMode mode)
{
    u32 reg;

    reg = 0;
    GX_SET_FIELD(reg, 2, 0, mode);
    GX_SET_FIELD(reg, 1, 2, 1);
    __peReg[4] = reg;
}

void GXPokeAlphaUpdate(u8 update_enable)
{
    u32 reg;

    reg = __peReg[1];
    GX_SET_FIELD(reg, 1, 4, update_enable);
    __peReg[1] = reg;
}

void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                     GXBlendFactor dst_factor, GXLogicOp op)
{
    u32 reg;

    reg = __peReg[1];
    GX_SET_FIELD(reg, 1, 0, (type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT));
    GX_SET_FIELD(reg, 1, 11, (type == GX_BM_SUBTRACT));
    GX_SET_FIELD(reg, 1, 1, (type == GX_BM_LOGIC));
    GX_SET_FIELD(reg, 4, 12, op);
    GX_SET_FIELD(reg, 3, 8, src_factor);
    GX_SET_FIELD(reg, 3, 5, dst_factor);
    GX_SET_FIELD(reg, 8, 24, 0x41);
    __peReg[1] = reg;
}

void GXPokeColorUpdate(u8 update_enable)
{
    u32 reg;

    reg = __peReg[1];
    GX_SET_FIELD(reg, 1, 3, update_enable);
    __peReg[1] = reg;
}

void GXPokeDstAlpha(u8 enable, u8 alpha)
{
    u32 reg = 0;

    GX_SET_FIELD(reg, 8, 0, alpha);
    GX_SET_FIELD(reg, 1, 8, enable);
    __peReg[2] = reg;
}

void GXPokeDither(u8 dither)
{
    u32 reg;

    reg = __peReg[1];
    GX_SET_FIELD(reg, 1, 2, dither);
    __peReg[1] = reg;
}

void GXPokeZMode(u8 compare_enable, GXCompare func, u8 update_enable)
{
    u32 reg = 0;

    GX_SET_FIELD(reg, 1, 0, compare_enable);
    GX_SET_FIELD(reg, 3, 1, func);
    GX_SET_FIELD(reg, 1, 4, update_enable);
    __peReg[0] = reg;
}

static void GXTokenInterruptHandler(s16 interrupt, OSContext *context)
{
    u16 token;
    OSContext exceptionContext;
    u32 reg;

    token = __peReg[7];
    if (TokenCB != 0) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        TokenCB(token);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
    reg = __peReg[5];
    GX_SET_FIELD(reg, 1, 2, 1);
    __peReg[5] = reg;
}

GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb)
{
    GXDrawDoneCallback oldcb;
    s32 enabled;

    oldcb = DrawDoneCB;
    enabled = OSDisableInterrupts();
    DrawDoneCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

static void GXFinishInterruptHandler(s16 interrupt, OSContext *context)
{
    OSContext exceptionContext;
    u32 reg;

    reg = __peReg[5];
    GX_SET_FIELD(reg, 1, 3, 1);
    __peReg[5] = reg;
    DrawDone = 1;
    if (DrawDoneCB != 0) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        DrawDoneCB();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
    OSWakeupThread(&FinishQueue);
}

void __GXPEInit(void)
{
    u32 reg;
    __OSSetInterruptHandler(__OS_INTERRUPT_PI_PE_TOKEN, GXTokenInterruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_PI_PE_FINISH, GXFinishInterruptHandler);
    OSInitThreadQueue(&FinishQueue);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_PE_TOKEN);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_PE_FINISH);
    reg = __peReg[5];
    GX_SET_FIELD(reg, 1, 2, 1);
    GX_SET_FIELD(reg, 1, 3, 1);
    GX_SET_FIELD(reg, 1, 0, 1);
    GX_SET_FIELD(reg, 1, 1, 1);
    __peReg[5] = reg;
}