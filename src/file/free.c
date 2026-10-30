#include "archive.h"
#include "os_alloc.h"

extern s32 HSD_GetHeap(void);
extern s32 usrHeap;
extern s32 dbgAllocU;

extern void debugFilePurge(void *memory);

void free(void *memory)
{
    if (memory != (void *)0) {
        dbgAllocU--;
        if (usrHeap == -1)
            OSFreeToHeap(HSD_GetHeap(), memory);
        else
            OSFreeToHeap(usrHeap, memory);
        debugFilePurge(memory);
    }
}
