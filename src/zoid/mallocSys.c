#include "types.h"

extern void *HSD_Alloc(u32 size);
extern void debugFileInsert(void *memory);
extern s32 dbgAllocS;

void *mallocSys(s32 size)
{
    void *memory;
    memory = HSD_Alloc((size + 0x1F) & ~0x1F);
    if (memory != 0) {
        dbgAllocS++;
        debugFileInsert(memory);
    }
    return memory;
}
