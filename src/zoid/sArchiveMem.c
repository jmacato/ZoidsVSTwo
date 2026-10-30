#include "archive.h"

extern void *mallocSys(s32 size);
extern void OSReport(const char *, ...);

HSD_Archive *sArchiveMem(u8 *data, u32 size)
{
    HSD_Archive *archive;
    if (data == 0)
        return 0;
    archive = mallocSys(0x44);
    if (archive == 0) {
        OSReport("sArcMem error lost \n");
        return 0;
    }
    OSReport("archive ok \n");
    HSD_ArchiveParse(archive, data, size);
    return archive;
}
