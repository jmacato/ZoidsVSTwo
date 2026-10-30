#include "card.h"
#include "os_time.h"

extern u32 __OSBusClock : 0x800000F8;
extern s32 MemcardNowSlot;
extern s32 MemCardSectorSize;
extern s32 MemCardFreeSize;
extern s32 MemCardFreeDir;
extern s32 MemCardStatus[2];
extern s32 MemCardOn[2];
extern s64 OSGetTime(void);
extern void OSReport(const char *format, ...);

s32 TEA_mcChkSlotEx(s32 slot)
{
    s32 memSize = 4;
    s32 sectorSize = 8;
    s32 unused = 0;
    s16 memSize16 = 4;
    u32 sectorSize2 = 0;
    s32 result = 0;
    s32 retry = 0;
    s32 probe = 0;
    s64 time = OSGetTime() / (__OSBusClock / 4) + 30;

    MemcardNowSlot = slot;
    for (retry = 0; retry < 10; retry++) {
        probe = CARDProbe(slot);
        if (probe == 1) {
            break;
        }
    }
    if (probe == 0) {
        return 0;
    }
    for (retry = 0; retry < 10; retry++) {
        result = CARDGetSectorSize(slot, &sectorSize2);
        if (result == 0) {
            MemCardSectorSize = sectorSize2;
            CARDFreeBlocks(slot, &MemCardFreeSize, &MemCardFreeDir);
            CARDGetMemSize(slot, &memSize16);
            MemCardStatus[slot] = 0;
            MemCardOn[slot] = 1;
            return 1;
        }
    }
    OSReport("MemCardCHK EX %d \202\310\202\265 \n", slot);
    MemCardStatus[slot] = result;
    return 0;
}
