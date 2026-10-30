/* Types follow
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/os/OSError.h. */
#ifndef ZOIDS_OS_ERROR_H
#define ZOIDS_OS_ERROR_H
#include "os_context.h"
typedef u16 OSError;
typedef void (*OSErrorHandler)(OSError error, OSContext *context, ...);
#define OS_ERROR_MACHINE_CHECK 1
extern OSErrorHandler __OSErrorTable[16];
extern OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler);
extern void OSReport(const char *format, ...);
extern void OSPanic(const char *file, int line, const char *format, ...);
extern void __OSUnhandledException(u8 exception, OSContext *context, u32 dsisr,
                                   u32 dar);
#endif
