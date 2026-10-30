/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/card/CARDBlock.c. */
#include "card_internal.h"
#include "os_cache.h"


void *__CARDGetFatBlock(CARDControl *card)
{
    return card->currentFat;
}


static void WriteCallback(s32 chan, s32 result)
{
    CARDControl *card;
    CARDCallback callback;
    u16 *first;
    u16 *second;

    card = &__CARDBlock[chan];
    if (result >= 0) {
        first = (u16 *)((u8 *)card->workArea + 3 * CARD_SYSTEM_BLOCK_SIZE);
        second = (u16 *)((u8 *)card->workArea + 4 * CARD_SYSTEM_BLOCK_SIZE);
        if (card->currentFat == first) {
            card->currentFat = second;
            memcpy(second, first, CARD_SYSTEM_BLOCK_SIZE);
        } else {
            card->currentFat = first;
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

static inline u16 *GetFatBlock(CARDControl *card)
{
    return card->currentFat;
}

static void EraseCallback(s32 chan, s32 result)
{
    CARDControl *card;
    CARDCallback callback;
    u16 *fat;
    u32 addr;
    u32 offset;
    u32 block;

    card = &__CARDBlock[chan];
    if (result < 0)
        goto error;
    fat = GetFatBlock(card);
    offset = (u32)((u8 *)fat - (u8 *)card->workArea);
    block = offset / CARD_SYSTEM_BLOCK_SIZE;
    addr = card->sectorSize * block;
    result = __CARDWrite(chan, addr, CARD_SYSTEM_BLOCK_SIZE, fat, WriteCallback);
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

s32 __CARDAllocBlock(s32 chan, u32 cBlock, CARDCallback callback)
{
    CARDControl *card;
    u16 *fat;
    u16 iBlock;
    u16 startBlock;
    u16 prevBlock;
    u16 count;

    card = &__CARDBlock[chan];
    if (!card->attached)
        return CARD_RESULT_NOCARD;

    fat = __CARDGetFatBlock(card);
    if (fat[CARD_FAT_FREEBLOCKS] < cBlock)
        return CARD_RESULT_INSSPACE;

    fat[CARD_FAT_FREEBLOCKS] -= cBlock;
    startBlock = 0xFFFF;
    iBlock = fat[CARD_FAT_LASTSLOT];
    count = 0;
    while (0 < cBlock) {
        if (card->cBlock - CARD_NUM_SYSTEM_BLOCK < ++count)
            return CARD_RESULT_BROKEN;

        iBlock++;
        if (!CARDIsValidBlockNo(card, iBlock))
            iBlock = CARD_NUM_SYSTEM_BLOCK;

        if (fat[iBlock] == CARD_FAT_AVAIL) {
            if (startBlock == 0xFFFF)
                startBlock = iBlock;
            else
                fat[prevBlock] = iBlock;
            prevBlock = iBlock;
            fat[iBlock] = 0xFFFF;
            --cBlock;
        }
    }
    fat[CARD_FAT_LASTSLOT] = iBlock;
    card->startBlock = startBlock;

    return __CARDUpdateFatBlock(chan, fat, callback);
}

s32 __CARDFreeBlock(s32 chan, u16 nBlock, CARDCallback callback)
{
    CARDControl *card;
    u16 *fat;
    u16 nextBlock;
    CARDControl *c;

    c = &__CARDBlock[chan];
    card = c;
    if (!card->attached)
        return CARD_RESULT_NOCARD;

    fat = __CARDGetFatBlock(card);
    while (nBlock != 0xFFFF) {
        if (!CARDIsValidBlockNo(card, nBlock))
            return CARD_RESULT_BROKEN;

        nextBlock = fat[nBlock];
        fat[nBlock] = CARD_FAT_AVAIL;
        nBlock = nextBlock;
        ++fat[CARD_FAT_FREEBLOCKS];
    }
    return __CARDUpdateFatBlock(chan, fat, callback);
}


s32 __CARDUpdateFatBlock(s32 chan, u16 *fat, CARDCallback callback)
{
    CARDControl *card;
    u32 addr;

    card = &__CARDBlock[chan];
    ++fat[CARD_FAT_CHECKCODE];
    __CARDCheckSum(&fat[CARD_FAT_CHECKCODE], CARD_SYSTEM_BLOCK_SIZE - sizeof(u32),
                   &fat[CARD_FAT_CHECKSUM], &fat[CARD_FAT_CHECKSUMINV]);
    DCStoreRange(fat, CARD_SYSTEM_BLOCK_SIZE);
    card->eraseCallback = callback;
    addr = (((char *)fat - (char *)card->workArea) / CARD_SYSTEM_BLOCK_SIZE) * card->sectorSize;
    return __CARDEraseSector(chan, addr, EraseCallback);
}
