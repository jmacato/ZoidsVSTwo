/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/debug.c. */
#include "types.h"
typedef struct OSContext OSContext;
typedef void (*HSD_PanicCallback)(OSContext *context, ...);
extern OSContext debug_context;
extern HSD_PanicCallback panic_func;
extern void HSD_SaveContext(void);
extern void OSReport(const char *format, ...);
extern void OSPanic(const char *file, u32 line, const char *format, ...);
extern void HSD_Panic(const char *file, u32 line, const char *message);
/* Preserve the original diagnostic call sequence. */
#pragma push
#pragma peephole off
void __assert(const char *file, u32 line, const char *condition)
{
    OSReport("assertion \"%s\" failed", condition);
    HSD_Panic(file, line, "");
}
void HSD_Panic(const char *file, u32 line, const char *message)
{
    if (panic_func != 0) {
        HSD_SaveContext();
        OSReport("%s in %s on line %d.\n", message, file, line);
        panic_func(&debug_context);
    }
    OSPanic(file, line, message);
}
#pragma pop
void HSD_GetVersion(int *major, int *minor, int *patch, int *build)
{
    if (major != 0)
        *major = 1;
    if (minor != 0)
        *minor = 3;
    if (patch != 0)
        *patch = 0;
    if (build != 0)
        *build = 1;
}
