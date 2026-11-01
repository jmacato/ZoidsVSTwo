#pragma peephole off
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

extern OSThreadQueue __OSActiveThreadQueue;
__asm__(".set __OSActiveThreadQueue, 0x800000DC");
extern volatile u16 __VIRegs[32];
__asm__(".set __VIRegs, 0xCC002000");
extern volatile u32 __PIRegs[16];
__asm__(".set __PIRegs, 0xCC003000");
extern u8 OS_REBOOT_BOOL;
__asm__(".set OS_REBOOT_BOOL, 0x800030E2");
extern void __OSStopAudioSystem(void);
extern s32 __PADDisableRecalibration(s32);
extern OSSram *__OSLockSram(void);
extern s32 __OSUnlockSram(s32);
extern s32 __OSSyncSram(void);
extern void ICFlashInvalidate(void);
extern void LCDisable(void);
extern void Reset(u32);
extern void __OSReboot(u32, s32);
extern void *memset(void *, s32, u32);

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
    struct {
        OSThread *thread;
        OSThread *next;
    } cursor;

    for (cursor.thread = __OSActiveThreadQueue.head; cursor.thread; cursor.thread = cursor.next) {
        cursor.next = cursor.thread->linkActive.next;
        switch (cursor.thread->state) {
        case OS_THREAD_STATE_READY:
        case OS_THREAD_STATE_WAITING:
            OSCancelThread(cursor.thread);
            continue;
        default:
            continue;
        }
    }
}

void OSResetSystem(int reset, unsigned long resetCode, int forceMenu)
{
    s32 disableRecalibration;

    OSDisableScheduler();
    __OSStopAudioSystem();

    if (reset == OS_RESET_SHUTDOWN)
        disableRecalibration = __PADDisableRecalibration(1);

    while (!__OSCallResetFunctions(0)) {
    }

    if ((reset == OS_RESET_HOTRESET && (forceMenu != 0))) {
        __OSLockSram()->flags |= 0x40;
        __OSUnlockSram(1);
        do {
        } while (__OSSyncSram() == 0);
    }

    OSDisableInterrupts();
    __OSCallResetFunctions(TRUE);

    LCDisable();

    if (reset == OS_RESET_HOTRESET) {
        OSDisableInterrupts();
        __VIRegs[1] = 0;
        ICFlashInvalidate();
        Reset(resetCode * 8);
    } else if (reset == OS_RESET_RESTART) {
        KillThreads();
        OSEnableScheduler();
        __OSReboot(resetCode, forceMenu);
    }

    KillThreads();

    memset(OSPhysicalToCached(0x40), 0, 0xCC - 0x40);
    memset(OSPhysicalToCached(0xD4), 0, 0xE8 - 0xD4);
    memset(OSPhysicalToCached(0xF4), 0, 0xF8 - 0xF4);
    memset(OSPhysicalToCached(0x3000), 0, 0xC0);
    memset(OSPhysicalToCached(0x30C8), 0, 0xD4 - 0xC8);
    memset(&OS_REBOOT_BOOL, 0, 1);
    __PADDisableRecalibration(disableRecalibration);
}
