#ifndef ZOIDS_DVD_FILE_H
#define ZOIDS_DVD_FILE_H
#include "dvd.h"
typedef struct DVDFileInfo DVDFileInfo;
typedef void (*DVDCallback)(s32, DVDFileInfo *);
struct DVDFileInfo {
    DVDCommandBlock cb;
    u32 startAddr;
    u32 length;
    DVDCallback callback;
};
extern void __DVDFSInit(void);
extern s32 DVDConvertPathToEntrynum(char *path);
extern s32 DVDFastOpen(s32 entry, DVDFileInfo *file);
extern s32 DVDOpen(char *name, DVDFileInfo *file);
extern s32 DVDGetCurrentDir(char *path, u32 length);
extern s32 DVDClose(DVDFileInfo *file);
extern s32 DVDReadAsyncPrio(DVDFileInfo *file, void *address, s32 length, s32 offset,
                            DVDCallback callback, s32 priority);
extern s32 DVDReadPrio(DVDFileInfo *file, void *address, s32 length, s32 offset,
                       s32 priority);
extern s32 DVDSeekAsyncPrio(DVDFileInfo *file, s32 offset, DVDCallback callback,
                            s32 priority);
extern s32 DVDPrepareStreamAsync(DVDFileInfo *file, u32 length, u32 offset,
                                 DVDCallback callback);
#endif
