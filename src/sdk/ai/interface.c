/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/ai/ai.c. */
#include "ai.h"
#include "os_context.h"
#include "os_interrupt.h"
#include "gx_state.h"
static void __AI_set_stream_sample_rate(u32 rate);
static void __AIDHandler(s16 interrupt, OSContext *context);
static void __AISHandler(s16 interrupt, OSContext *context);
static void __AI_SRC_INIT(void);
extern void __AICallbackStackSwitch(AIDCallback callback);
extern volatile u16 __DSPRegs[32];
__asm__(".set __DSPRegs, 0xCC005000");
#define DSP_DMA_START_HI 24
#define DSP_DMA_START_LO 25
#define DSP_DMA_CONTROL_LEN 27
#define DSP_DMA_START_FLAG 0x8000
extern volatile u32 __AIRegs[4];
__asm__(".set __AIRegs, 0xCC006C00");
extern u32 __OSBusClock;
__asm__(".set __OSBusClock, 0x800000F8");
#define OSNanosecondsToTicks(nsec) (((nsec) * (__OSBusClock / 4 / 125000)) / 8000)
extern s64 OSGetTime(void);
static AISCallback __AIS_Callback = 0;
static AIDCallback __AID_Callback = 0;
static u8 *__CallbackStack = 0;
/* The original stack-switch routine still uses this saved stack pointer. */
#pragma force_active on
static u8 *__OldStack = 0;
#pragma force_active off
static s32 __AI_init_flag = 0;
static s64 bound_32KHz = 0;
static s64 bound_48KHz = 0;
static s64 min_wait = 0;
static s64 max_wait = 0;
static s64 buffer = 0;
static inline void AISetStreamTrigger(u32 trigger)
{
    __AIRegs[3] = trigger;
}
static inline void AIResetStreamSampleCount(void)
{
    __AIRegs[0] = (__AIRegs[0] & ~0x20) | 0x20;
}
AIDCallback AIRegisterDMACallback(AIDCallback callback)
{
    AIDCallback old_callback;
    s32 old;

    old_callback = __AID_Callback;
    old = OSDisableInterrupts();
    __AID_Callback = callback;
    OSRestoreInterrupts(old);
    return old_callback;
}

void AIInitDMA(u32 start_addr, u32 length)
{
    s32 old;

    old = OSDisableInterrupts();
    __DSPRegs[DSP_DMA_START_HI] =
        (__DSPRegs[DSP_DMA_START_HI] & 0xFFFFFC00) | (start_addr >> 16);
    __DSPRegs[DSP_DMA_START_LO] =
        (__DSPRegs[DSP_DMA_START_LO] & 0xFFFF001F) | (start_addr & 0xFFFF);

    __DSPRegs[DSP_DMA_CONTROL_LEN] =
        (__DSPRegs[DSP_DMA_CONTROL_LEN] & 0xFFFF8000) | ((length >> 5) & 0xFFFF);
    OSRestoreInterrupts(old);
}

void AIStartDMA(void)
{
    __DSPRegs[DSP_DMA_CONTROL_LEN] = __DSPRegs[DSP_DMA_CONTROL_LEN] | DSP_DMA_START_FLAG;
}



void AISetStreamPlayState(u32 state)
{
    s32 old;
    u8 vol_left;
    u8 vol_right;

    if (state != AIGetStreamPlayState()) {
        if (AIGetStreamSampleRate() == 0 && state == AI_STREAM_START) {
            vol_left = AIGetStreamVolRight();
            vol_right = AIGetStreamVolLeft();
            AISetStreamVolRight(0);
            AISetStreamVolLeft(0);
            old = OSDisableInterrupts();
            __AI_SRC_INIT();
            GX_SET_FIELD(__AIRegs[0], 1, 5, 1);
            GX_SET_FIELD(__AIRegs[0], 1, 0, AI_STREAM_START);
            OSRestoreInterrupts(old);
            AISetStreamVolLeft(vol_left);
            AISetStreamVolRight(vol_right);
            return;
        }
        GX_SET_FIELD(__AIRegs[0], 1, 0, state);
    }
}

u32 AIGetStreamPlayState(void)
{
    return __AIRegs[0] & 1;
}

void AISetDSPSampleRate(u32 rate)
{
    s32 old;
    u32 play_state;
    u32 afr_state;
    u8 vol_left;
    u8 vol_right;

    if (rate != AIGetDSPSampleRate()) {
        __AIRegs[0] = (__AIRegs[0] & 0xFFFFFFBF);
        if (rate == AI_SAMPLERATE_32KHZ) {
            vol_left = AIGetStreamVolLeft();
            vol_right = AIGetStreamVolRight();
            play_state = AIGetStreamPlayState();
            afr_state = AIGetStreamSampleRate();
            AISetStreamVolLeft(0U);
            AISetStreamVolRight(0U);
            old = OSDisableInterrupts();
            __AI_SRC_INIT();
            GX_SET_FIELD(__AIRegs[0], 1, 5, 1);
            GX_SET_FIELD(__AIRegs[0], 1, 1, afr_state);
            GX_SET_FIELD(__AIRegs[0], 1, 0, play_state);
            __AIRegs[0] |= 0x40;
            OSRestoreInterrupts(old);
            AISetStreamVolLeft(vol_left);
            AISetStreamVolRight(vol_right);
        }
    }
}

u32 AIGetDSPSampleRate(void)
{
    return GX_GET_FIELD(__AIRegs[0], 1, 6) ^ 1;
}

static void __AI_set_stream_sample_rate(u32 rate)
{
    s32 old;
    u32 play_state;
    u8 vol_left;
    u8 vol_right;
    u32 dsp_src_state;

    if (rate != AIGetStreamSampleRate()) {
        play_state = AIGetStreamPlayState();
        vol_left = AIGetStreamVolLeft();
        vol_right = AIGetStreamVolRight();
        AISetStreamVolRight(0);
        AISetStreamVolLeft(0);
        dsp_src_state = __AIRegs[0] & 0x40;
        GX_SET_FIELD(__AIRegs[0], 1, 6, 0);
        old = OSDisableInterrupts();
        __AI_SRC_INIT();
        __AIRegs[0] |= dsp_src_state;
        GX_SET_FIELD(__AIRegs[0], 1, 5, 1);
        GX_SET_FIELD(__AIRegs[0], 1, 1, rate);
        OSRestoreInterrupts(old);
        AISetStreamPlayState(play_state);
        AISetStreamVolLeft(vol_left);
        AISetStreamVolRight(vol_right);
    }
}

u32 AIGetStreamSampleRate(void)
{
    return GX_GET_FIELD(__AIRegs[0], 1, 1);
}

void AISetStreamVolLeft(u8 vol)
{
    GX_SET_FIELD(__AIRegs[1], 8, 0, vol);
}

u8 AIGetStreamVolLeft(void)
{
    return GX_GET_FIELD(__AIRegs[1], 8, 0);
}

void AISetStreamVolRight(u8 vol)
{
    GX_SET_FIELD(__AIRegs[1], 8, 8, vol);
}

u8 AIGetStreamVolRight(void)
{
    return (__AIRegs[1] & (0xFF << 8)) >> 8;
}

void AIInit(u8 *stack)
{
    if (__AI_init_flag != 1) {
        bound_32KHz = OSNanosecondsToTicks(31524);
        bound_48KHz = OSNanosecondsToTicks(42024);
        min_wait = OSNanosecondsToTicks(42000);
        max_wait = OSNanosecondsToTicks(63000);
        buffer = OSNanosecondsToTicks(3000);
        AISetStreamVolRight(0);
        AISetStreamVolLeft(0);
        AISetStreamTrigger(0);
        AIResetStreamSampleCount();
        __AI_set_stream_sample_rate(AI_SAMPLERATE_48KHZ);
        AISetDSPSampleRate(AI_SAMPLERATE_32KHZ);

        __AIS_Callback = 0;
        __AID_Callback = 0;
        __CallbackStack = stack;
        if (stack) {
        }
        __OSSetInterruptHandler(__OS_INTERRUPT_DSP_AI, __AIDHandler);
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_DSP_AI);
        __OSSetInterruptHandler(__OS_INTERRUPT_AI_AI, __AISHandler);
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_AI_AI);
        __AI_init_flag = 1;
    }
}

static void __AISHandler(s16 interrupt, OSContext *context)
{
    OSContext exceptionContext;

    __AIRegs[0] |= 8;
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    if (__AIS_Callback) {
        __AIS_Callback(__AIRegs[2]);
    }
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

static void __AIDHandler(s16 interrupt, OSContext *context)
{
    OSContext exceptionContext;
    u16 tmp;

    tmp = __DSPRegs[5];
    tmp = (tmp & ~0xA0) | 8;
    __DSPRegs[5] = tmp;
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    if (__AID_Callback) {
        if (__CallbackStack) {
            __AICallbackStackSwitch(__AID_Callback);
        } else {
            __AID_Callback();
        }
    }
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

#pragma peephole off
static void __AI_SRC_INIT(void)
{
    s64 rising_32khz = 0;
    s64 rising_48khz = 0;
    s64 diff = 0;
    s64 t1 = 0;
    s64 temp;
    u32 temp0;
    u32 temp1;
    u32 done = 0;
    u32 volume = 0;
    u32 Init_Cnt = 0;
    u32 walking = 0;

    walking = 0;
    Init_Cnt = 0;
    temp = 0;

    while (!done) {
        GX_SET_FIELD(__AIRegs[0], 1, 5, 1);
        GX_SET_FIELD(__AIRegs[0], 1, 1, 0);
        GX_SET_FIELD(__AIRegs[0], 1, 0, AI_STREAM_START);
        temp0 = __AIRegs[2];
        while (temp0 == __AIRegs[2]) {
        }
        rising_32khz = OSGetTime();
        GX_SET_FIELD(__AIRegs[0], 1, 1, 1);
        GX_SET_FIELD(__AIRegs[0], 1, 0, AI_STREAM_START);
        temp1 = __AIRegs[2];
        while (temp1 == __AIRegs[2]) {
        }
        rising_48khz = OSGetTime();
        diff = rising_48khz - rising_32khz;
        GX_SET_FIELD(__AIRegs[0], 1, 1, 0);
        GX_SET_FIELD(__AIRegs[0], 1, 0, AI_STREAM_STOP);
        if (diff < bound_32KHz - buffer) {
            temp = min_wait;
            done = 1;
            Init_Cnt++;
        } else if (diff >= bound_32KHz + buffer && diff < bound_48KHz - buffer) {
            temp = max_wait;
            done = 1;
            Init_Cnt++;
        } else {
            done = 0;
            walking = 1;
            Init_Cnt++;
        }
    }
    while (rising_48khz + temp > OSGetTime()) {
    }
}
