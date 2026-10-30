#include "card.h"
#include "card_internal.h"
#include "os_interrupt.h"

static inline s32 PutControlBlock(CARDControl *card, s32 result)
{
    s32 enabled = OSDisableInterrupts();

    if (card->attached) {
        card->result = result;
    } else if (card->result == CARD_RESULT_BUSY) {
        card->result = result;
    }
    OSRestoreInterrupts(enabled);
    return result;
}

s32 CARDGetMemSize(s32 chan, u16 *nBlock)
{
    CARDControl *card;
    s32 result;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }
    *nBlock = card->size;
    return PutControlBlock(card, 0);
}
