/* Types follow https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/dvd.h.
 */
#ifndef ZOIDS_DVD_H
#define ZOIDS_DVD_H
#include "types.h"
typedef struct DVDDiskID DVDDiskID;
struct DVDDiskID {
    char gameName[4];
    char company[2];
    u8 diskNumber;
    u8 gameVersion;
    u8 streaming;
    u8 streamingBufSize;
    u8 padding[22];
};
typedef struct DVDDriveInfo {
    u16 revisionLevel;
    u16 deviceCode;
    u32 releaseDate;
    u8 padding[24];
} DVDDriveInfo;
typedef struct DVDCommandBlock DVDCommandBlock;
typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock *block);
struct DVDCommandBlock {
    DVDCommandBlock *next;
    DVDCommandBlock *prev;
    u32 command;
    s32 state;
    u32 offset;
    u32 length;
    void *addr;
    u32 currTransferSize;
    u32 transferredSize;
    DVDDiskID *id;
    DVDCBCallback callback;
    void *userData;
};
#define DVD_RESULT_GOOD 0
#define DVD_RESULT_FATAL_ERROR -1
#define DVD_RESULT_IGNORED -2
#define DVD_RESULT_CANCELED -3
#define DVD_STATE_FATAL_ERROR -1
#define DVD_STATE_END 0
#define DVD_STATE_BUSY 1
#define DVD_STATE_WAITING 2
#define DVD_STATE_COVER_CLOSED 3
#define DVD_STATE_NO_DISK 4
#define DVD_STATE_COVER_OPEN 5
#define DVD_STATE_WRONG_DISK 6
#define DVD_STATE_MOTOR_STOPPED 7
#define DVD_STATE_PAUSING 8
#define DVD_STATE_IGNORED 9
#define DVD_STATE_CANCELED 10
#define DVD_STATE_RETRY 11
#define DVD_MIN_TRANSFER_SIZE 32
#define DVD_COMMAND_NONE 0
#define DVD_COMMAND_READ 1
#define DVD_COMMAND_SEEK 2
#define DVD_COMMAND_CHANGE_DISK 3
#define DVD_COMMAND_BSREAD 4
#define DVD_COMMAND_READID 5
#define DVD_COMMAND_INITSTREAM 6
#define DVD_COMMAND_CANCELSTREAM 7
#define DVD_COMMAND_STOP_STREAM_AT_END 8
#define DVD_COMMAND_REQUEST_AUDIO_ERROR 9
#define DVD_COMMAND_REQUEST_PLAY_ADDR 10
#define DVD_COMMAND_REQUEST_START_ADDR 11
#define DVD_COMMAND_REQUEST_LENGTH 12
#define DVD_COMMAND_AUDIO_BUFFER_CONFIG 13
#define DVD_COMMAND_INQUIRY 14
#define DVD_COMMAND_BS_CHANGE_DISK 15
extern void DVDInit(void);
extern s32 DVDSetAutoInvalidation(s32 enabled);
extern s32 DVDCheckDisk(void);
extern s32 DVDReadAbsAsyncForBS(DVDCommandBlock *block, void *address, s32 length,
                                s32 offset, DVDCBCallback callback);
extern s32 DVDInquiryAsync(DVDCommandBlock *block, DVDDriveInfo *information,
                           DVDCBCallback callback);
extern void DVDReset(void);
extern s32 DVDReadAbsAsyncPrio(DVDCommandBlock *block, void *address, s32 length,
                               s32 offset, DVDCBCallback callback, s32 priority);
extern s32 DVDSeekAbsAsyncPrio(DVDCommandBlock *block, s32 offset,
                               DVDCBCallback callback, s32 priority);
extern s32 DVDReadDiskID(DVDCommandBlock *block, DVDDiskID *id, DVDCBCallback callback);
extern s32 DVDPrepareStreamAbsAsync(DVDCommandBlock *block, u32 length, u32 offset,
                                    DVDCBCallback callback);
extern s32 DVDCancelStream(DVDCommandBlock *block);
extern s32 DVDStopStreamAtEndAsync(DVDCommandBlock *block, DVDCBCallback callback);
extern s32 DVDGetCommandBlockStatus(DVDCommandBlock *block);
extern s32 DVDGetDriveStatus(void);
extern s32 DVDCancelAsync(DVDCommandBlock *block, DVDCBCallback callback);
extern s32 DVDCancel(DVDCommandBlock *block);
extern s32 DVDCancelAll(void);
extern DVDDiskID *DVDGetCurrentDiskID(void);
extern void __DVDPrepareResetAsync(DVDCBCallback callback);
#endif
