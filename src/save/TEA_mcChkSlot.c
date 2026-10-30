#include "card.h"
#include "os_time.h"

extern u32 __OSBusClock : 0x800000F8;
extern s32 MemcardNowSlot;
extern s32 MemCardStatus[2];
extern s32 MemCardSectorSize;
extern s32 MemCardFreeSize;
extern s32 MemCardFreeDir;
extern s32 MemCardOn[2];
extern s64 OSGetTime(void);
extern s32 TEA_mcError(s32 slot, s32 result);

s32 TEA_mcChkSlot(s32 slot)
{
    s32 memSize = 4;
    s32 sectorSize = 8;
    s32 unused = 0;
    s32 result = 0;
    s32 retry = 0;
    s64 time = OSGetTime() / (__OSBusClock / 4) + 10;

    MemcardNowSlot = slot;
    for (;;) {
        result = CARDProbeEx(slot, &memSize, &sectorSize);
        if (result == CARD_RESULT_READY) {
            MemCardSectorSize = sectorSize;
            CARDFreeBlocks(slot, &MemCardFreeSize, &MemCardFreeDir);
            MemCardOn[slot] = 1;
            MemCardStatus[slot] = CARD_RESULT_READY;
            return 1;
        }
        if (result == CARD_RESULT_WRONGDEVICE) {
            MemCardStatus[slot] = CARD_RESULT_WRONGDEVICE;
            return 1;
        }
        if (result == CARD_RESULT_NOCARD) {
            MemCardStatus[slot] = CARD_RESULT_NOCARD;
            MemCardOn[slot] = 0;
            return 0;
        }
        if (OSGetTime() / (__OSBusClock / 4) > time) {
            return 0;
        }
        if (result == CARD_RESULT_FATAL_ERROR) {
            puts("CARD FATAL ERROR ");
            retry++;
            CARDUnmount(slot);
            if (retry > 10) {
                retry = 0;
                CARDInit();
            }
        }
        if (result == CARD_RESULT_BUSY || result == 0) {
            continue;
        }
        MemCardOn[slot] = 0;
        return 0;
    }
}
