/* Types follow
 * https://github.com/doldecomp/dolsdk2004/blob/main/include/dolphin/os/OSReset.h. */
#ifndef ZOIDS_OS_RESET_H
#define ZOIDS_OS_RESET_H
#include "types.h"
#define OS_RESET_RESTART 0
#define OS_RESET_HOTRESET 1
#define OS_RESET_SHUTDOWN 2
typedef struct OSResetFunctionInfo {
    s32 (*func)(s32 final);
    u32 priority;
    struct OSResetFunctionInfo *next;
    struct OSResetFunctionInfo *prev;
} OSResetFunctionInfo;
typedef void (*OSResetCallback)(void);
extern s32 OSGetResetButtonState(void);
extern s32 OSGetResetSwitchState(void);
extern u32 OSGetResetCode(void);
extern void OSRegisterResetFunction(OSResetFunctionInfo *function);
extern void __OSDoHotReset(s32 reset_code);
extern void OSResetSystem(int reset, unsigned long reset_code, int force_menu);
#endif
