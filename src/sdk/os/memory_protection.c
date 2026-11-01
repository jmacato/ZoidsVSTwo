/* Reference: https://github.com/doldecomp/dolsdk2004/blob/main/src/os/OSMemory.c. */
#include "os_interrupt.h"
#include "os_memory.h"
#include "os_reset.h"
#include "os.h"
#include "os_error.h"
#define NULL 0
#define FALSE 0
#define TRUE 1
#define OS_INTERRUPTMASK_MEM_RESET                                                     \
    (OS_INTERRUPTMASK_MEM_0 | OS_INTERRUPTMASK_MEM_1 | OS_INTERRUPTMASK_MEM_2 |        \
     OS_INTERRUPTMASK_MEM_3)
extern volatile u16 __MEMRegs[32];
__asm__(".set __MEMRegs, 0xCC004000");
#define OSPhysicalToCached(address) ((void *)((u32)(address) + 0x80000000))
extern void Config24MB(void);
extern void Config48MB(void);
extern void RealMode(u32);
static s32 OnReset(s32);
static OSResetFunctionInfo ResetFunctionInfo = {OnReset, 0x7F, 0, 0};
static inline u32 OSGetConsoleSimulatedMemSize(void)
{
    u32 *memSize = (u32 *)OSPhysicalToCached(0xF0);
    return *memSize;
}
u32 OSGetPhysicalMemSize(void)
{
    u32 *memSize = (u32 *)OSPhysicalToCached(0x28);
    return *memSize;
}
static s32 OnReset(s32 final)
{
    if (final != FALSE) {
        __MEMRegs[8] = 0xFF;
        __OSMaskInterrupts(OS_INTERRUPTMASK_MEM_RESET);
    }
    return TRUE;
}
static void MEMIntrruptHandler(__OSInterrupt interrupt, OSContext *context)
{
    u32 addr;
    u32 cause;

    cause = __MEMRegs[0xf];
    addr = (((u32)__MEMRegs[0x12] & 0x3ff) << 16) | __MEMRegs[0x11];
    __MEMRegs[0x10] = 0;

    if (__OSErrorTable[__OS_EXCEPTION_MEMORY_PROTECTION]) {
        __OSErrorTable[__OS_EXCEPTION_MEMORY_PROTECTION](
            __OS_EXCEPTION_MEMORY_PROTECTION, context, cause, addr);
        return;
    }

    __OSUnhandledException(__OS_EXCEPTION_MEMORY_PROTECTION, context, cause, addr);
}
#pragma peephole off
void __OSInitMemoryProtection(void)
{
    s32 enabled;
    u32 size;

    size = OSGetConsoleSimulatedMemSize();
    enabled = OSDisableInterrupts();

    if (size <= 0x1800000) {
        RealMode((u32)&Config24MB);
    } else if (size <= 0x3000000) {
        RealMode((u32)&Config48MB);
    }

    __MEMRegs[16] = 0;
    __MEMRegs[8] = 0xFF;

    __OSMaskInterrupts(OS_INTERRUPTMASK_MEM_0 | OS_INTERRUPTMASK_MEM_1 |
                       OS_INTERRUPTMASK_MEM_2 | OS_INTERRUPTMASK_MEM_3);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_0, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_1, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_2, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_3, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_ADDRESS, MEMIntrruptHandler);
    OSRegisterResetFunction(&ResetFunctionInfo);

    if (OSGetConsoleSimulatedMemSize() < OSGetPhysicalMemSize() && OSGetConsoleSimulatedMemSize() == 0x1800000) {
        __MEMRegs[20] = 2;
    }

    __OSUnmaskInterrupts(OS_INTERRUPTMASK_MEM_ADDRESS);
    OSRestoreInterrupts(enabled);
}
