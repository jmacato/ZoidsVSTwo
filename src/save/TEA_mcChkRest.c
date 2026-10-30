#include "card.h"

extern s32 MemCardStatus[2];
extern s32 MemCardOn[2];
extern s32 TEA_mcError(s32 channel, s32 result);
extern void OSReport(const char *format, ...);

s32 TEA_mcChkRest(s32 channel)
{
    s32 result = 0;
    s32 retry = 0;

    do {
        result = CARDCheck(channel);
        if (result != CARD_RESULT_BUSY) {
            break;
        }
        retry++;
    } while (retry <= 60);
    MemCardStatus[channel] = result;
    if (result == CARD_RESULT_ENCODING) {
        return 1;
    }
    if (result < -1) {
        CARDUnmount(channel);
        if (result == CARD_RESULT_IOERROR) {
            return TEA_mcError(channel, result);
        } else if (result == CARD_RESULT_WRONGDEVICE) {
            return TEA_mcError(channel, result);
        } else {
            OSReport("\t\217C\225\2343\216\270\224s. (%d)\n", result);
            TEA_mcError(channel, result);
            return 0;
        }
    }
    OSReport("\t\217C\225\234 Ok \n");
    return 1;
}
