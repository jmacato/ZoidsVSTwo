#include "types.h"

extern s32 MemcardNowSlot;
extern s32 TEA_mcChkSlot(s32 channel);
extern s32 TEA_mcMount(s32 channel);
extern s32 TEA_mcFileOpen(s32 channel, char *name);
extern s32 TEA_mcFileClose(void);
extern s32 TEA_mcUnMount(s32 channel);

s32 TEA_mcFileMountExist(s32 channel, char *name)
{
    if (TEA_mcChkSlot(channel) == 0)
        return 0;
    if (TEA_mcMount(channel) == 0)
        return 0;
    MemcardNowSlot = channel;
    if (TEA_mcFileOpen(channel, name) == 0)
        return 0;
    TEA_mcFileClose();
    TEA_mcUnMount(channel);
    return 1;
}
