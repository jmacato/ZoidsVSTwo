#ifndef ZOIDS_CARD_H
#define ZOIDS_CARD_H
#include "types.h"
#define CARD_FILENAME_MAX 32
#define CARD_MAX_FILE 127
typedef void (*CARDCallback)(s32 chan, s32 result);
typedef struct CARDFileInfo {
    /*0x00*/ s32 chan;
    /*0x04*/ s32 fileNo;
    /*0x08*/ s32 offset;
    /*0x0C*/ s32 length;
    /*0x10*/ u16 iBlock;
} CARDFileInfo;

#define CARD_RESULT_UNLOCKED 1
#define CARD_RESULT_READY 0
#define CARD_RESULT_BUSY -1
#define CARD_RESULT_WRONGDEVICE -2
#define CARD_RESULT_NOCARD -3
#define CARD_RESULT_NOFILE -4
#define CARD_RESULT_IOERROR -5
#define CARD_RESULT_BROKEN -6
#define CARD_RESULT_EXIST -7
#define CARD_RESULT_NOENT -8
#define CARD_RESULT_INSSPACE -9
#define CARD_RESULT_NOPERM -10
#define CARD_RESULT_LIMIT -11
#define CARD_RESULT_NAMETOOLONG -12
#define CARD_RESULT_ENCODING -13
#define CARD_RESULT_CANCELED -14
#define CARD_RESULT_FATAL_ERROR -128
extern void CARDInit(void);
extern s32 CARDGetResultCode(s32 chan);
extern s32 CARDGetXferredBytes(s32 chan);
extern s32 CARDFreeBlocks(s32 chan, s32 *bytes_free, s32 *files_free);
extern s32 CARDGetSectorSize(s32 chan, u32 *size);
extern s32 CARDUnmount(s32 chan);
extern s32 CARDOpen(s32 chan, char *name, CARDFileInfo *file);
extern s32 CARDClose(CARDFileInfo *file);
extern s32 CARDCheckExAsync(s32 chan, s32 *transferred_bytes, CARDCallback callback);
extern s32 CARDCheckAsync(s32 chan, CARDCallback callback);
extern s32 CARDReadAsync(CARDFileInfo *file, void *buffer, s32 length, s32 offset,
                         CARDCallback callback);
extern long CARDRead(CARDFileInfo *file, void *buffer, long length, long offset);
extern long CARDWriteAsync(CARDFileInfo *file, void *buffer, long length, long offset,
                          CARDCallback callback);
extern long CARDWrite(CARDFileInfo *file, void *buffer, long length, long offset);
extern s32 CARDProbeEx(s32 chan, s32 *memory_size, s32 *sector_size);
extern s32 CARDMountAsync(s32 chan, void *work_area, CARDCallback detach_callback,
                         CARDCallback attach_callback);
extern s32 CARDCreateAsync(s32 chan, const char *name, u32 size, CARDFileInfo *file,
                          CARDCallback callback);
#endif
