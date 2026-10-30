#include "objalloc.h"
extern void OSFreeToHeap(s32 heap, void *address);
extern void *OSAllocFromHeap(s32 heap, u32 size);
void HSD_Free(void *address)
{
    OSFreeToHeap(HSD_GetHeap(), address);
}
void *HSD_Alloc(u32 size)
{
    void *address;
    if ((s32)size <= 0)
        return 0;
    address = OSAllocFromHeap(HSD_GetHeap(), size);
    if (address == 0)
        __assert("memory.c", 52, "adr");
    return address;
}
