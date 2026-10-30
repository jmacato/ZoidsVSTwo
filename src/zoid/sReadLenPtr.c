#include "types.h"

extern s32 sReadPre(void);
extern void *sReadFileLenPtr(char *name, s32 a, s32 b, s32 c);

void *sReadLenPtr(char *name, s32 a, s32 b, s32 c)
{
    sReadPre();
    return sReadFileLenPtr(name, a, b, c);
}
