#include "types.h"
#include "card.h"
#include "os_time.h"

extern u32 __OSBusClock : 0x800000F8;
extern s32 MemcardNowSlot;

s32 TEA_mcUnMount(s32 slot)
{
    s32 pad_a = 4;
    s32 pad_b = 8;
    s32 pad_c = 0;
    s64 time;
    s32 result = 0;
    time = OSGetTime() / (__OSBusClock / 4) + 3;
    MemcardNowSlot = slot;
    CARDUnmount(0);
    result = CARDUnmount(1);
    if (result < CARD_RESULT_BUSY)
        return 0;
    return 1;
}
