/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/card/CARDDir.c. */
#include "card_internal.h"
#include "runtime_string.h"

extern void DCStoreRange(void *address, u32 length);

CARDDir *__CARDGetDirBlock(CARDControl *card)
{
    return card->currentDir;
}


static void WriteCallback(s32 chan, s32 result)
{
    CARDControl *card = &__CARDBlock[chan];
    CARDCallback callback;

    if (result >= 0) {
        CARDDir *first = (CARDDir *)((u8 *)card->workArea + 1 * CARD_SYSTEM_BLOCK_SIZE);
        CARDDir *second = (CARDDir *)((u8 *)card->workArea + 2 * CARD_SYSTEM_BLOCK_SIZE);
        if (card->currentDir == first) {
            card->currentDir = second;
            memcpy(second, first, CARD_SYSTEM_BLOCK_SIZE);
        } else {
            card->currentDir = first;
            memcpy(first, second, CARD_SYSTEM_BLOCK_SIZE);
        }
    }
    if (card->apiCallback == 0)
        __CARDPutControlBlock(card, result);
    callback = card->eraseCallback;
    if (callback != 0) {
        card->eraseCallback = 0;
        callback(chan, result);
    }
}

static inline CARDDir *GetDirBlock(CARDControl *card)
{
    return card->currentDir;
}

static void EraseCallback(s32 chan, s32 result)
{
    CARDControl *card;
    CARDCallback callback;
    CARDDir *dir;
    u32 addr;
    u32 offset;
    u32 block;

    card = &__CARDBlock[chan];
    if (result < 0)
        goto error;
    dir = GetDirBlock(card);
    offset = (u32)((u8 *)dir - (u8 *)card->workArea);
    block = offset / CARD_SYSTEM_BLOCK_SIZE;
    addr = card->sectorSize * block;
    result = __CARDWrite(chan, addr, CARD_SYSTEM_BLOCK_SIZE, dir, WriteCallback);
    if (result < 0)
        goto error;
    return;

error:
    if (card->apiCallback == 0)
        __CARDPutControlBlock(card, result);
    callback = card->eraseCallback;
    if (callback != 0) {
        card->eraseCallback = 0;
        callback(chan, result);
    }
}

s32 CARDGetXferredBytes(s32 chan)
{
    return __CARDBlock[chan].xferred;
}

s32 __CARDUpdateDir(s32 chan, CARDCallback callback)
{
    CARDControl *card;
    CARDDirCheck *check;
    u32 addr;
    u32 offset;
    u32 block;
    CARDDir *dir;

    card = &__CARDBlock[chan];
    if (!card->attached)
        return CARD_RESULT_NOCARD;
    dir = __CARDGetDirBlock(card);
    check = CARDGetDirCheck(dir);
    ++check->checkCode;
    __CARDCheckSum(dir, CARD_SYSTEM_BLOCK_SIZE - sizeof(u32), &check->checkSum,
                   &check->checkSumInv);
    DCStoreRange(dir, CARD_SYSTEM_BLOCK_SIZE);
    card->eraseCallback = callback;
    offset = (u32)dir - (u32)card->workArea;
    block = offset / CARD_SYSTEM_BLOCK_SIZE;
    addr = card->sectorSize * block;
    return __CARDEraseSector(chan, addr, EraseCallback);
}
