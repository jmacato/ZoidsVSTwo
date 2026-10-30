/* References: https://github.com/doldecomp/dolsdk2001/blob/main/src/os/OSReset.c
 * and https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/os/OSReset.c. */
#include "asm_ops.h"
#include "os_reset.h"
#include "os_thread.h"
#include "os_interrupt.h"
#define NULL 0
#define TRUE 1
#define OSPhysicalToCached(address) ((void *)((u32)(address) + 0x80000000))
typedef struct OSSram {
    u16 checkSum, checkSumInv;
    u32 ead0, ead1, counterBias;
    s8 displayOffsetH;
    u8 ntd, language, flags;
} OSSram;
struct OSResetFunctionQueue {
    OSResetFunctionInfo *head;
    OSResetFunctionInfo *tail;
};
static struct OSResetFunctionQueue ResetFunctionQueue;
extern OSThreadQueue __OSActiveThreadQueue : 0x800000DC;
extern volatile u16 __VIRegs[32] : 0xCC002000;
extern volatile u32 __PIRegs[16] : 0xCC003000;
extern u8 OS_REBOOT_BOOL : 0x800030E2;
extern void __OSStopAudioSystem(void);
extern s32 __PADDisableRecalibration(s32);
extern OSSram *__OSLockSram(void);
extern s32 __OSUnlockSram(s32);
extern s32 __OSSyncSram(void);
extern void ICFlashInvalidate(void);
extern void LCDisable(void);
extern void __OSReboot(u32, s32);
extern void *memset(void *, s32, u32);
#define ENQUEUE_INFO(info, queue)                                                      \
    do {                                                                               \
        struct OSResetFunctionInfo *__prev = (queue)->tail;                            \
        if (__prev == 0) {                                                             \
            (queue)->head = (info);                                                    \
        } else {                                                                       \
            __prev->next = (info);                                                     \
        }                                                                              \
        (info)->prev = __prev;                                                         \
        (info)->next = 0;                                                              \
        (queue)->tail = (info);                                                        \
    } while (0);

#define ENQUEUE_INFO_PRIO(info, queue)                                                 \
    do {                                                                               \
        struct OSResetFunctionInfo *__prev;                                            \
        struct OSResetFunctionInfo *__next;                                            \
        for (__next = (queue)->head; __next && (__next->priority <= (info)->priority); \
             __next = __next->next)                                                    \
            ;                                                                          \
                                                                                       \
        if (__next == 0) {                                                             \
            ENQUEUE_INFO(info, queue);                                                 \
        } else {                                                                       \
            (info)->next = __next;                                                     \
            __prev = __next->prev;                                                     \
            __next->prev = (info);                                                     \
            (info)->prev = __prev;                                                     \
            if (__prev == 0) {                                                         \
                (queue)->head = (info);                                                \
            } else {                                                                   \
                __prev->next = (info);                                                 \
            }                                                                          \
        }                                                                              \
    } while (0);

void OSRegisterResetFunction(struct OSResetFunctionInfo *info)
{

    ENQUEUE_INFO_PRIO(info, &ResetFunctionQueue);
}

#pragma push
#pragma optimization_level 0
#pragma peephole off
static void Reset(u32 resetCode)
{
    ASM_INSN(b lab_20);
    ASM_INSN(lab_4: mfspr r8, 1008);
    ASM_INSN(ori r8, r8, 8);
    ASM_INSN(mtspr 1008, r8);
    ASM_INSN(isync);
    ASM_INSN(sync);
    ASM_INSN(nop);
    ASM_INSN(b lab_24);
    ASM_INSN(lab_20: b lab_40);
    ASM_INSN(lab_24: mftb r5, 0x10c);
    ASM_INSN(lab_28: mftb r6, 0x10c);
    ASM_INSN(subf r7, r5, r6);
    ASM_INSN(cmplwi r7, 0x1124);
    ASM_INSN(blt lab_28);
    ASM_INSN(nop);
    ASM_INSN(b lab_44);
    ASM_INSN(lab_40: b lab_60);
    ASM_INSN(lab_44: lis r8, -0x3400);
    ASM_INSN(ori r8, r8, 0x3000);
    ASM_INSN(li r4, 3);
    ASM_INSN(stw r4, 0x24(r8));
    ASM_INSN(stw r3, 0x24(r8));
    ASM_INSN(nop);
    ASM_INSN(b lab_64);
    ASM_INSN(lab_60: b lab_6c);
    ASM_INSN(lab_64: nop);
    ASM_INSN(b lab_64);
    ASM_INSN(lab_6c: b lab_4);
}
#pragma pop
static inline int CallResetFunctions(int final)
{
    struct OSResetFunctionInfo *info;
    int err = 0;

    for (info = ResetFunctionQueue.head; info; info = info->next) {
        err |= !info->func(final);
    }
    err |= !__OSSyncSram();
    if (err) {
        return 0;
    }
    return 1;
}
static inline s32 __OSCallResetFunctions(s32 final)
{
    OSResetFunctionInfo *info;
    s32 retCode = 0;
    s32 temp;

    for (info = ResetFunctionQueue.head; info != NULL;) {
        temp = !info->func(final);
        info = info->next;
        retCode |= temp;
    }
    retCode |= !__OSSyncSram();
    if (retCode) {
        return 0;
    }
    return 1;
}
static inline void KillThreads(void)
{
    OSThread *thread;
    OSThread *next;

    for (thread = __OSActiveThreadQueue.head; thread; thread = next) {
        next = thread->linkActive.next;
        switch (thread->state) {
        case OS_THREAD_STATE_READY:
        case OS_THREAD_STATE_WAITING:
            OSCancelThread(thread);
            continue;
        default:
            continue;
        }
    }
}
#pragma peephole off
void __OSDoHotReset(s32 resetCode)
{
    OSDisableInterrupts();
    __VIRegs[1] = 0;
    ICFlashInvalidate();
    Reset(resetCode * 8);
}
void OSResetSystem(int reset, unsigned long resetCode, int forceMenu);

u32 OSGetResetCode(void)
{
    if (OS_REBOOT_BOOL != 0)
        return 0x80000000u;
    return (__PIRegs[9] & ~7u) >> 3;
}
