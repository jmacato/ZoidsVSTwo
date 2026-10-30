#include "card.h"

extern s32 MemCardOn[2];
extern s32 MemCardStatus[2];
extern s32 MemcardNowSlot;
extern s32 printf(const char *format, ...);

void TEA_mcCardEject(void)
{
    s32 slot = MemcardNowSlot;
    MemCardOn[slot] = 0;
    MemCardStatus[slot] = 0;
    CARDUnmount(slot);
    printf("\203\201\203\202\203\212\203J\201[\203h:\203G\203W\203F\203N\203V\203\207\203\223 SLOT[%d]\n", slot);
}
