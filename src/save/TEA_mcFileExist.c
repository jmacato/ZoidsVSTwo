#include "types.h"

extern s32 MemcardNowSlot;
extern s32 TEA_mcFileOpen(s32 slot, char *name);
extern s32 TEA_mcFileClose(void);

s32 TEA_mcFileExist(s32 slot, char *name)
{
    MemcardNowSlot = slot;
    if (TEA_mcFileOpen(slot, name) == 0)
        return 0;
    TEA_mcFileClose();
    return 1;
}
