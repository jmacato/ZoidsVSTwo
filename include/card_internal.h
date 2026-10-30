#ifndef ZOIDS_CARD_INTERNAL_H
#define ZOIDS_CARD_INTERNAL_H
#include "card.h"
#include "dsp.h"
#include "dvd.h"
#include "os_alarm.h"
#include "os_thread.h"
#include "exi.h"
#define CARD_FAT_AVAIL 0x0000u
#define CARD_FAT_CHECKSUM 0x0000u
#define CARD_FAT_CHECKSUMINV 0x0001u
#define CARD_FAT_CHECKCODE 0x0002u
#define CARD_FAT_FREEBLOCKS 0x0003u
#define CARD_FAT_LASTSLOT 0x0004u
#define CARD_WORKAREA_SIZE (5 * 8 * 1024)
#define CARD_SEG_SIZE 0x200u
#define CARD_PAGE_SIZE 0x80u
#define CARD_MAX_SIZE 0x01000000U
#define CARD_NUM_SYSTEM_BLOCK 5
#define CARD_SYSTEM_BLOCK_SIZE (8 * 1024u)
#define CARD_MAX_MOUNT_STEP (CARD_NUM_SYSTEM_BLOCK + 2)
typedef struct CARDDir {
    /*0x00*/ u8 gameName[4];
    /*0x04*/ u8 company[2];
    /*0x06*/ u8 _padding0;
    /*0x07*/ u8 bannerFormat;
    /*0x08*/ u8 fileName[CARD_FILENAME_MAX];
    /*0x28*/ u32 time;
    /*0x2C*/ u32 iconAddr;
    /*0x30*/ u16 iconFormat;
    /*0x32*/ u16 iconSpeed;
    /*0x34*/ u8 permission;
    u8 copyTimes;
    /*0x36*/ u16 startBlock;
    u16 length;
    u8 _padding1[2];
    u32 commentAddr;
} CARDDir;
typedef struct CARDDirCheck {
    u8 padding0[56];
    u16 padding1;
    s16 checkCode;
    u16 checkSum;
    u16 checkSumInv;
} CARDDirCheck;
#define CARDGetDirCheck(dir) ((CARDDirCheck *)&(dir)[CARD_MAX_FILE])
#define CARDIsValidBlockNo(card, blockNo)                                              \
    ((blockNo) >= CARD_NUM_SYSTEM_BLOCK && (blockNo) < (card)->cBlock)

typedef struct CARDControl {
    /*0x00*/ s32 attached;
    /*0x04*/ s32 result;
    /*0x08*/ u16 size;
    /*0x0A*/ u16 pageSize;
    /*0x0C*/ s32 sectorSize;
    /*0x10*/ u16 cBlock;
    /*0x12*/ u16 vendorID;
    /*0x14*/ s32 latency;
    /*0x18*/ u8 id[12];
    /*0x24*/ int mountStep;
    int formatStep;
    u32 scramble;
    DSPTaskInfo task;
    void *workArea;
    /*0x84*/ CARDDir *currentDir;
    u16 *currentFat;
    OSThreadQueue threadQueue;
    u8 cmd[9];
    s32 cmdlen;
    volatile u32 mode;
    int retry;
    int repeat;
    u32 addr;
    void *buffer;
    s32 xferred;
    u16 freeNo;
    u16 startBlock;
    /*0xC0*/ CARDFileInfo *fileInfo;
    CARDCallback extCallback;
    CARDCallback txCallback;
    CARDCallback exiCallback;
    CARDCallback apiCallback;
    CARDCallback xferCallback;
    CARDCallback eraseCallback;
    CARDCallback unlockCallback;
    OSAlarm alarm;
    u32 cid;
    DVDDiskID *diskID;
} CARDControl;

long __CARDGetStatusEx(long chan, long fileNo, struct CARDDir *dirent);
long __CARDSetStatusExAsync(long chan, long fileNo, struct CARDDir *dirent,
                            void (*callback)(long, long));
long __CARDSetStatusEx(long chan, long fileNo, struct CARDDir *dirent);

s32 __CARDUnlock(s32 chan, u8 flashID[12]);

s32 __CARDSeek(CARDFileInfo *fileInfo, s32 length, s32 offset, CARDControl **pcard);

s32 __CARDRead(s32 chan, u32 addr, s32 length, void *dst, CARDCallback callback);
s32 __CARDWrite(s32 chan, u32 addr, s32 length, void *dst, CARDCallback callback);

long __CARDRawReadAsync(long chan, void *buf, long length, long offset,
                        void (*callback)(long, long));
long __CARDRawRead(long chan, void *buf, long length, long offset);

s32 __CARDCompareFileName(CARDDir *ent, const char *fileName);
s32 __CARDAccess(CARDControl *card, CARDDir *ent);
s32 __CARDIsPublic(CARDDir *ent);
s32 __CARDGetFileNo(CARDControl *card, const char *fileName, s32 *pfileNo);
s32 __CARDIsOpened(CARDControl *card, s32 fileNo);

void __CARDMountCallback(s32 chan, s32 result);

s32 CARDFormatAsync(s32 chan, CARDCallback callback);
#define __CARDGetDirCheck(dir) ((CARDDirCheck *)&(dir)[CARD_MAX_FILE])

CARDDir *__CARDGetDirBlock(CARDControl *card);
s32 __CARDUpdateDir(s32 chan, CARDCallback callback);

void __CARDCheckSum(void *ptr, int length, u16 *checksum, u16 *checksumInv);
s32 __CARDVerify(CARDControl *card);

void *__CARDGetFatBlock(CARDControl *card);
s32 __CARDAllocBlock(s32 chan, u32 cBlock, CARDCallback callback);
s32 __CARDFreeBlock(s32 chan, u16 nBlock, CARDCallback callback);
s32 __CARDUpdateFatBlock(s32 chan, u16 *fat, CARDCallback callback);

extern u16 __CARDVendorID;

typedef struct CARDID {
    /* 0x000 */ u8 serial[32];
    /* 0x020 */ u16 deviceID;
    /* 0x022 */ u16 size;
    /* 0x024 */ u16 encode;
    /* 0x026 */ u8 padding[470];
    /* 0x1FC */ u16 checkSum;
    /* 0x1FE */ u16 checkSumInv;
} CARDID;
extern struct CARDControl __CARDBlock[2];

extern DVDDiskID *__CARDDiskID;
extern DVDDiskID __CARDDiskNone;

void __CARDDefaultApiCallback(s32 chan, s32 result);
void __CARDSyncCallback(s32 chan, s32 result);
void __CARDExtHandler(s32 chan, OSContext *context);
void __CARDExiHandler(s32 chan, OSContext *context);
void __CARDTxHandler(s32 chan, OSContext *context);
void __CARDUnlockedHandler(s32 chan, OSContext *context);
int __CARDReadNintendoID(s32 chan, u32 *id);
s32 __CARDEnableInterrupt(s32 chan, s32 enable);
s32 __CARDReadStatus(s32 chan, u8 *status);
s32 __CARDClearStatus(s32 chan);
long __CARDSleep(long chan);
long __CARDWakeup(long chan);
s32 __CARDReadSegment(s32 chan, CARDCallback callback);
s32 __CARDWritePage(s32 chan, CARDCallback callback);
long __CARDErase(long chan, void (*callback)(long, long));
s32 __CARDEraseSector(s32 chan, u32 addr, CARDCallback callback);
void __CARDSetDiskID(DVDDiskID *id);
s32 __CARDGetControlBlock(s32 chan, CARDControl **pcard);
s32 __CARDPutControlBlock(CARDControl *card, s32 result);
s32 __CARDSync(s32 chan);

#endif
