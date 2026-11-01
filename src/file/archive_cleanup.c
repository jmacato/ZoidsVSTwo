#include "archive.h"
#include "os_alloc.h"

extern s32 HSD_GetHeap(void);
extern s32 usrHeap;
extern s32 dbgAlloc;

#pragma dont_inline on
void debugFilePurge(void *memory)
{
    /* The shipped debug hook only checks its pointer and returns. */
    if (memory == 0)
        return;
}




