#include "types.h"
#include "dvd_file.h"

extern s32 fexist(char *name);
extern void *mallocF(u32 size);
extern void free(void *ptr);

s32 sFileLenAlloc(char *path)
{
    DVDFileInfo info;
    void *memory;
    s32 length;
    if (fexist(path) == 0)
        return 0;
    if (DVDOpen(path, &info) == 0)
        return 0;
    length = info.length;
    DVDClose(&info);
    if (length == 0)
        return 0;
    memory = mallocF(length);
    if (memory == 0)
        return 0;
    free(memory);
    return 1;
}
