#include "types.h"

extern void *sReadFileLenU(char *name, s32 mode, s32 *length);

s32 fileRead(char *name, void **data)
{
    s32 length;
    void *buffer = sReadFileLenU(name, 0, &length);
    if (buffer == 0)
        return -1;
    if (data != 0)
        *data = buffer;
    return length;
}
