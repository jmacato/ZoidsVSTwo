/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/card/CARDRdwr.c. */
#include "card_internal.h"
#define NULL 0
static void BlockReadCallback(s32 chan, s32 result);
static void BlockWriteCallback(s32 chan, s32 result);

static void BlockReadCallback(s32 chan, s32 result)
{
    struct CARDControl *card;
    void (*callback)(s32, s32);

    card = &__CARDBlock[chan];

    if ((result >= 0)) {
        card->xferred += CARD_SEG_SIZE;
        card->addr += CARD_SEG_SIZE;
        card->buffer = (u8 *)card->buffer + CARD_SEG_SIZE;

        if (--card->repeat > 0) {
            result = __CARDReadSegment(chan, BlockReadCallback);
            if (result >= 0) {
                return;
            }
        }
    }
    if (!card->apiCallback) {
        __CARDPutControlBlock(card, result);
    }
    callback = card->xferCallback;
    if (callback) {
        card->xferCallback = NULL;
        callback(chan, result);
    }
}

s32 __CARDRead(s32 chan, u32 addr, s32 length, void *dst, void (*callback)(s32, s32))
{
    struct CARDControl *card;
    card = &__CARDBlock[chan];
    if (card->attached == 0) {
        return CARD_RESULT_NOCARD;
    }
    card->xferCallback = callback;
    card->repeat = (length / CARD_SEG_SIZE);
    card->addr = addr;
    card->buffer = dst;
    return __CARDReadSegment(chan, BlockReadCallback);
}

static void BlockWriteCallback(s32 chan, s32 result)
{
    struct CARDControl *card;
    void (*callback)(s32, s32);

    card = &__CARDBlock[chan];
    if (result >= 0) {
        card->xferred += CARD_PAGE_SIZE;
        card->addr += CARD_PAGE_SIZE;
        card->buffer = (u8 *)card->buffer + CARD_PAGE_SIZE;

        if (--card->repeat > 0) {
            result = __CARDWritePage(chan, BlockWriteCallback);
            if (result >= 0) {
                return;
            }
        }
    }
    if (!card->apiCallback) {
        __CARDPutControlBlock(card, result);
    }
    callback = card->xferCallback;
    if (callback) {
        card->xferCallback = NULL;
        callback(chan, result);
    }
}

s32 __CARDWrite(s32 chan, u32 addr, s32 length, void *dst, void (*callback)(s32, s32))
{
    struct CARDControl *card;
    card = &__CARDBlock[chan];
    if (card->attached == 0) {
        return CARD_RESULT_NOCARD;
    }
    card->xferCallback = callback;
    card->repeat = (length / CARD_PAGE_SIZE);
    card->addr = addr;
    card->buffer = dst;
    return __CARDWritePage(chan, BlockWriteCallback);
}
