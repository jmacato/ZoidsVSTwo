#ifndef ZOIDS_OS_H
#define ZOIDS_OS_H
#include "os_context.h"
#define __OS_EXCEPTION_SYSTEM_RESET 0
#define __OS_EXCEPTION_MACHINE_CHECK 1
#define __OS_EXCEPTION_DSI 2
#define __OS_EXCEPTION_ISI 3
#define __OS_EXCEPTION_EXTERNAL_INTERRUPT 4
#define __OS_EXCEPTION_ALIGNMENT 5
#define __OS_EXCEPTION_PROGRAM 6
#define __OS_EXCEPTION_FLOATING_POINT 7
#define __OS_EXCEPTION_DECREMENTER 8
#define __OS_EXCEPTION_SYSTEM_CALL 9
#define __OS_EXCEPTION_TRACE 10
#define __OS_EXCEPTION_PERFORMACE_MONITOR 11
#define __OS_EXCEPTION_BREAKPOINT 12
#define __OS_EXCEPTION_SYSTEM_INTERRUPT 13
#define __OS_EXCEPTION_THERMAL_INTERRUPT 14
#define __OS_EXCEPTION_MEMORY_PROTECTION 15
typedef void (*OSExceptionHandler)(u8 exception, OSContext *context);
extern void OSInit(void);
extern unsigned long OSGetConsoleType(void);
extern OSExceptionHandler __OSSetExceptionHandler(u8 exception,
                                                  OSExceptionHandler handler);
extern OSExceptionHandler __OSGetExceptionHandler(u8 exception);
extern u32 __OSGetDIConfig(void);
#endif
