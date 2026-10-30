#ifndef ZOIDS_DSP_STATE_H
#define ZOIDS_DSP_STATE_H
#include "dsp.h"
#include "os_interrupt.h"
#ifdef __MWERKS__
extern volatile u16 __DSPRegs[32] : 0xCC005000;
#else
#define __DSPRegs ((volatile u16 *)0xCC005000)
#endif
extern DSPTaskInfo *__DSP_curr_task;
extern DSPTaskInfo *__DSP_first_task;
extern DSPTaskInfo *__DSP_last_task;
extern DSPTaskInfo *__DSP_tmp_task;
extern DSPTaskInfo *__DSP_rude_task;
extern s32 __DSP_rude_task_pending;
#endif
