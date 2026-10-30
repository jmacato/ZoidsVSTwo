#include "archive.h"
#include "os_alloc.h"

extern s32 dbgAllocS;

extern void debugFilePurge(void *memory);

void sArchivePurge(HSD_Archive *archive)
{
    if (archive != (void *)0) {
        HSD_ArchiveFree(archive);
        debugFilePurge(archive);
        dbgAllocS--;
    }
}
