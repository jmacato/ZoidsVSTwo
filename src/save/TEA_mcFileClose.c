#include "card.h"
#include "os_time.h"

extern u32 __OSBusClock : 0x800000F8;
extern s32 MemCardStatus[2];
extern s32 MemcardNowSlot;
extern CARDFileInfo MemCardFileInfo;
extern s64 OSGetTime(void);
extern void OSReport(const char *format, ...);

s32 TEA_mcFileClose(void)
{
    s32 retry = 0;
    s32 slot = MemcardNowSlot;
    s64 deadline = OSGetTime() / (__OSBusClock / 4) + 15;
    s32 result;

    OSReport("MemCardClose\t");
    for (;;) {
        result = CARDClose(&MemCardFileInfo);
        if (OSGetTime() / (__OSBusClock / 4) > deadline) {
            MemCardStatus[slot] = -128;
            return 0;
        }
        if (result == CARD_RESULT_BUSY) {
            continue;
        }
        if (result == 0) {
            break;
        }
        retry++;
        if (retry < 5) {
            continue;
        }
        OSReport("failed. (%d)\n", result);
        return 0;
    }
    OSReport("Done\n");
    return 1;
}
