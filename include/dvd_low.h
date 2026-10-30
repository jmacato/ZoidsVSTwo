#ifndef ZOIDS_DVD_LOW_H
#define ZOIDS_DVD_LOW_H
#include "dvd.h"
#include "os_interrupt.h"
#define DVD_INTTYPE_TC 1
#define DVD_INTTYPE_DE 2
#define DVD_INTTYPE_CVR 4
#define DVD_INTTYPE_BREAK 8
#define DVD_INTTYPE_TIMEOUT 16
typedef void (*DVDLowCallback)(u32 interrupt_type);
extern void __DVDInitWA(void);
extern void __DVDInterruptHandler(__OSInterrupt interrupt, OSContext *context);
extern void __DVDLowSetWAType(u32 type, u32 location);
extern s32 DVDLowRead(void *address, u32 length, u32 offset, DVDLowCallback callback);
extern s32 DVDLowSeek(u32 offset, DVDLowCallback callback);
extern s32 DVDLowWaitCoverClose(DVDLowCallback callback);
extern s32 DVDLowReadDiskID(DVDDiskID *id, DVDLowCallback callback);
extern s32 DVDLowStopMotor(DVDLowCallback callback);
extern s32 DVDLowRequestError(DVDLowCallback callback);
extern s32 DVDLowInquiry(DVDDriveInfo *info, DVDLowCallback callback);
extern s32 DVDLowAudioStream(u32 command, u32 length, u32 offset,
                             DVDLowCallback callback);
extern s32 DVDLowRequestAudioStatus(u32 command, DVDLowCallback callback);
extern s32 DVDLowAudioBufferConfig(s32 enabled, u32 size, DVDLowCallback callback);
extern void DVDLowReset(void);
extern s32 DVDLowBreak(void);
extern DVDLowCallback DVDLowClearCallback(void);
extern DVDDiskID *DVDGetCurrentDiskID(void);
#endif
