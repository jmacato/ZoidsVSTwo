#include "types.h"
#include "os_alloc.h"

extern s32 HSD_GetHeap(void);
extern void OSReport(const char *, ...);
extern s32 usrHeap;
extern s32 dbgAllocU;

extern void debugFileInsert(void *memory);

void *mallocF(u32 size)
{
    void *memory;
    if (usrHeap == -1)
        memory = OSAllocFromHeap(HSD_GetHeap(), (size + 31) & ~31);
    else
        memory = OSAllocFromHeap(usrHeap, (size + 31) & ~31);
    if (memory != 0) {
        dbgAllocU++;
        debugFileInsert(memory);
    } else {
        OSReport("error alloc from usrHeap .......................\n");
    }
    return memory;
}
