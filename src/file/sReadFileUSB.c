#include "types.h"
extern s32 TEA_USBFileR2(char *name);
void *sReadFileUSB(char *name)
{
    return (void *)TEA_USBFileR2(name);
}
