#include "initialize.h"

extern void *OSAllocFromArenaLo(u32 size, u32 alignment);
extern void HSD_Panic(const char *file, u32 line, const char *message);

void *HSD_AllocateFIFO(u32 size)
{
    void *fifo = OSAllocFromArenaLo(size, 32);
    if (fifo == 0)
        HSD_Panic("initialize.c", 251, "no space remains for gx fifo.\n");
    return fifo;
}
