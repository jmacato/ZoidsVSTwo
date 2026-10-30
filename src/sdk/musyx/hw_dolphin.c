#include "musyx_synth.h"
#include "dsp.h"
#include "musyx_hardware.h"

typedef struct SND_HOOKS {
    void *(*malloc)(u32 length);
    void (*free)(void *address);
} SND_HOOKS;

extern SND_HOOKS salHooks;
extern volatile u32 salDspIsDone;
extern volatile u32 salDspInitIsDone;
extern void AIStartDMA(void);
extern s32 OSDisableInterrupts(void);
extern s32 OSEnableInterrupts(void);
extern void OSRestoreInterrupts(s32 level);
extern u32 OSGetTick(void);
extern void *AIRegisterDMACallback(void *callback);
extern void AIStopDMA(void);
extern void DSPHalt(void);
extern u32 DSPGetDMAStatus(void);
extern void DSPReset(void);
extern u8 *salAIBufferBase;
extern u8 salAIBufferIndex;
extern volatile u32 salLastTick;
extern volatile s32 oldState;
extern volatile u16 hwIrqLevel;

extern u8 dspSlave[];
extern u16 dspSlaveLength;
static void dspResumeCallback(void *task);

static DSPTaskInfo dsp_task;
static u8 dram_image[8192] __attribute__((aligned(32)));

void dspInitCallback(void *task)
{
    salDspIsDone = 1;
    salDspInitIsDone = 1;
}

void salStartAi(void)
{
    AIStartDMA();
}



void *salAiGetDest(void)
{
    u8 index = (salAIBufferIndex + 2) % 4;
    return salAIBufferBase + index * 0x280;
}

unsigned long salInitDsp(unsigned long flags)
{
    dsp_task.iram_mmem_addr = (u16 *)dspSlave;
    dsp_task.iram_length = dspSlaveLength;
    dsp_task.iram_addr = 0;
    dsp_task.dram_mmem_addr = (u16 *)dram_image;
    dsp_task.dram_length = 0x2000;
    dsp_task.dram_addr = 0;
    dsp_task.dsp_init_vector = 0x10;
    dsp_task.dsp_resume_vector = 0x30;
    dsp_task.init_cb = dspInitCallback;
    dsp_task.res_cb = dspResumeCallback;
    dsp_task.done_cb = 0;
    dsp_task.req_cb = 0;
    dsp_task.priority = 0;
    DSPInit();
    DSPAddTask(&dsp_task);
    salDspInitIsDone = 0;
    hwEnableIrq();
    while (salDspInitIsDone == 0)
        ;
    hwDisableIrq();
    return 1;
}



/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/hw_dolphin.c (CC0-1.0). */
extern u16 dspCmdFirstSize;
extern u16 *dspCmdList;
extern void salBuildCommandList(s16 *dest, u32 delay);
extern void PPCSync(void);
extern void DSPSendMailToDSP(u32 mail);
extern u32 DSPCheckMailToDSP(void);
extern u32 salGetStartDelay(void);
static inline void salStartDsp(u16 *cmdList) {
  salDspIsDone = 0;
  PPCSync();

  DSPSendMailToDSP(dspCmdFirstSize | 0xbabe0000);
  while (DSPCheckMailToDSP())
    ;
  DSPSendMailToDSP((u32)cmdList);
  while (DSPCheckMailToDSP())
    ;
}
void salCtrlDsp(s16 *dest) {
  salBuildCommandList(dest, salGetStartDelay());
  salStartDsp(dspCmdList);
}

u32 salGetStartDelay(void)
{
    return ((OSGetTick() - salLastTick) * 8) / ((*(u32 *)0x800000F8 / 4) / 125000);
}



void hwEnableIrq(void)
{
    if (--hwIrqLevel == 0)
        OSRestoreInterrupts(oldState);
}

void hwDisableIrq(void)
{
    if ((hwIrqLevel++) == 0)
        oldState = OSDisableInterrupts();
}

void hwIRQEnterCritical(void)
{
    OSDisableInterrupts();
}

void hwIRQLeaveCritical(void)
{
    OSEnableInterrupts();
}

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/hw_dolphin.c (CC0-1.0). */
#define NULL 0
#define TRUE 1
#define FALSE 0
#define DMA_BUFFER_LEN 0x280
#define OSCachedToPhysical(pointer) ((u32)((u32)(pointer) - 0x80000000))
typedef void (*SND_SOME_CALLBACK)(void);
extern SND_SOME_CALLBACK userCallback;
extern volatile u32 salLogicIsWaiting;
extern volatile u32 salLogicActive;
extern void AIInitDMA(u32 start, u32 length);
extern void *memset(void *buffer, int value, u32 length);
extern void DCFlushRange(void *buffer, u32 length);
static inline void callUserCallback() {
  if (salLogicActive) {
    return;
  }
  salLogicActive = 1;
  OSEnableInterrupts();
  userCallback();
  OSDisableInterrupts();
  salLogicActive = 0;
}

#pragma force_active on
static void salCallback() {
  salAIBufferIndex = (salAIBufferIndex + 1) % 4;
  AIInitDMA(OSCachedToPhysical(salAIBufferBase) + (salAIBufferIndex * 0x280),
            0x280);
  salLastTick = OSGetTick();
  if (salDspIsDone) {
    callUserCallback();
  } else {
    salLogicIsWaiting = 1;
  }
}
static void dspResumeCallback(void *task) {
  salDspIsDone = TRUE;
  if (salLogicIsWaiting) {
    salLogicIsWaiting = FALSE;
    callUserCallback();
  }
}
unsigned long salInitAi(SND_SOME_CALLBACK callback, unsigned long flags, unsigned long *outFreq) {
  if ((salAIBufferBase = salMalloc(0x280 * 4)) != NULL) {
    memset(salAIBufferBase, 0, 0x280 * 4);
    DCFlushRange(salAIBufferBase, 0x280 * 4);
    salAIBufferIndex = TRUE;
    salLogicIsWaiting = FALSE;
    salDspIsDone = TRUE;
    salLogicActive = FALSE;
    userCallback = callback;
    AIRegisterDMACallback(salCallback);
    AIInitDMA(OSCachedToPhysical(salAIBufferBase) + (salAIBufferIndex * 0x280), 0x280);
    synthInfo.numSamples = 0x20;
    *outFreq = 32000;

    return TRUE;
  }
  return FALSE;
}
void hwInitIrq() {
  oldState = OSDisableInterrupts();
  hwIrqLevel = 1;
}
#pragma force_active reset

void *salMalloc(u32 length)
{
    return salHooks.malloc(length);
}

void salFree(void *address)
{
    salHooks.free(address);
}

void sndSetHooks(SND_HOOKS *hooks)
{
    salHooks = *hooks;
}
