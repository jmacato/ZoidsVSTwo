/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/dsp/dsp.c. */
#include "dsp_state.h"
#define BUILD_DATE "Dec 17 2001"
#define BUILD_TIME "18:25:00"
static s32 __DSP_init_flag = 0;
u32 DSPCheckMailToDSP(void)
{
    return (__DSPRegs[0] & (1 << 15)) >> 15;
}

u32 DSPCheckMailFromDSP(void)
{
    return (__DSPRegs[2] & (1 << 15)) >> 15;
}

u32 DSPReadMailFromDSP(void)
{
    return (__DSPRegs[2] << 16) | __DSPRegs[3];
}

void DSPSendMailToDSP(u32 mail)
{
    __DSPRegs[0] = mail >> 16;
    __DSPRegs[1] = mail & 0xFFFF;
}

void DSPInit(void)
{
    s32 old;
    u16 tmp;

    __DSP_debug_printf("DSPInit(): Build Date: %s %s\n", BUILD_DATE, BUILD_TIME);

    if (__DSP_init_flag == 1)
        return;

    old = OSDisableInterrupts();
    __OSSetInterruptHandler(__OS_INTERRUPT_DSP_DSP, __DSPHandler);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_DSP_DSP);

    tmp = __DSPRegs[5];
    tmp = (tmp & ~0xA8) | 0x800;
    __DSPRegs[5] = tmp;

    tmp = __DSPRegs[5];
    __DSPRegs[5] = tmp = tmp & ~0xAC;

    __DSP_first_task = __DSP_last_task = __DSP_curr_task = __DSP_tmp_task = 0;
    __DSP_init_flag = 1;

    OSRestoreInterrupts(old);
}







DSPTaskInfo *DSPAddTask(DSPTaskInfo *task)
{
    s32 old;

    old = OSDisableInterrupts();

    __DSP_insert_task(task);
    task->state = 0;
    task->flags = 1;

    OSRestoreInterrupts(old);
    if (task == __DSP_first_task)
        __DSP_boot_task(task);
    return task;
}