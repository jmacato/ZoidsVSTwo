/* Types follow https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/dsp.h.
 */
#ifndef ZOIDS_DSP_H
#define ZOIDS_DSP_H
#include "types.h"
#include "os_context.h"
typedef void (*DSPCallback)(void *task);

typedef struct STRUCT_DSP_TASK {
    /*0x00*/ volatile u32 state;
    /*0x04*/ volatile u32 priority;
    /*0x08*/ volatile u32 flags;
    /*0x0C*/ u16 *iram_mmem_addr;
    /*0x10*/ u32 iram_length;
    /*0x14*/ u32 iram_addr;
    /*0x18*/ u16 *dram_mmem_addr;
    /*0x1C*/ u32 dram_length;
    /*0x20*/ u32 dram_addr;
    /*0x24*/ u16 dsp_init_vector;
    /*0x26*/ u16 dsp_resume_vector;
    /*0x28*/ DSPCallback init_cb;
    /*0x2C*/ DSPCallback res_cb;
    /*0x30*/ DSPCallback done_cb;
    /*0x34*/ DSPCallback req_cb;
    /*0x38*/ struct STRUCT_DSP_TASK *next;
    /*0x3C*/ struct STRUCT_DSP_TASK *prev;
    s64 t_context;
    s64 t_task;
} DSPTaskInfo;

u32 DSPCheckMailToDSP(void);
u32 DSPCheckMailFromDSP(void);
u32 DSPReadMailFromDSP(void);
void DSPSendMailToDSP(u32 mail);
void DSPInit(void);
void DSPReset(void);
void DSPHalt(void);
u32 DSPGetDMAStatus(void);
DSPTaskInfo *DSPAddTask(DSPTaskInfo *task);
void __DSP_debug_printf(const char *fmt, ...);
void __DSPHandler(s16 intr, OSContext *context);
void __DSP_exec_task(DSPTaskInfo *curr, DSPTaskInfo *next);
void __DSP_boot_task(DSPTaskInfo *task);
void __DSP_insert_task(DSPTaskInfo *task);
void __DSP_remove_task(DSPTaskInfo *task);
#endif
