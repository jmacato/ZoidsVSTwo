#include "card.h"

extern s32 MemCardStatus[2];
extern s32 CARDCheck(s32 chan);

s32 TEA_mcChkIoErr(s32 channel)
{
    s32 result = 0;
    s32 count = 0;
    do {
        result = CARDCheck(channel);
        if (result != CARD_RESULT_BUSY)
            break;
        count++;
    } while (count <= 60);
    MemCardStatus[channel] = result;
    if (result < CARD_RESULT_BUSY) {
        if (result == CARD_RESULT_FATAL_ERROR)
            return 0;
        if (result == CARD_RESULT_IOERROR)
            return 0;
        if (result == CARD_RESULT_WRONGDEVICE)
            return 0;
    } else {
        return 1;
    }
    return 1;
}
