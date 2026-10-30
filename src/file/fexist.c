#include "dvd_file.h"
#include "archive.h"
#include "disk_error.h"

extern s32 sReadPre(void);

#pragma dont_inline on
s32 fexist(char *name)
{
    s32 entry;
    sReadPre();
    entry = DVDConvertPathToEntrynum(name);
    return entry != -1;
}
