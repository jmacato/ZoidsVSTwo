#include "fobj.h"

extern struct HSD_ObjAllocData alloc_data;
extern void *HSD_ObjAlloc(struct HSD_ObjAllocData *data);
extern void __assert(const char *file, u32 line, const char *condition);
extern void *memset(void *memory, s32 value, u32 size);

HSD_FObj *HSD_FObjAlloc(void)
{
    HSD_FObj *function = HSD_ObjAlloc(&alloc_data);

    if (function == 0) {
        __assert("fobj.c", 755, "new");
    }
    memset(function, 0, sizeof(*function));
    return function;
}
