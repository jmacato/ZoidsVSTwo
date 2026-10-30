/* Reference: https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/dvd/dvdlow.c.
 */
#include "dvd_low.h"
#include "os_alarm.h"
#include "os_time.h"
#include "os_interrupt.h"
#define TRUE 1
#define FALSE 0
#define NULL 0
#define OSMillisecondsToTicks(value) ((value) * (__OSBusClock / 4 / 1000))
#define OSMicrosecondsToTicks(value) ((value) * (__OSBusClock / 4 / 125000) / 8)
#define OSSecondsToTicks(value) ((value) * (__OSBusClock / 4))
extern u32 __OSBusClock : 0x800000F8;
extern volatile u32 __PIRegs[12] : 0xCC003000;
extern volatile u32 __DIRegs[16] : 0xCC006000;
static s32 FirstRead = TRUE;
static volatile s32 StopAtNextInt = FALSE;
static u32 LastLength = 0;
static DVDLowCallback Callback = NULL;
static DVDLowCallback ResetCoverCallback = NULL;
static volatile OSTime LastResetEnd = 0;
static volatile u32 ResetOccurred = FALSE;
static volatile s32 WaitingCoverClose = FALSE;
static s32 Breaking = FALSE;
static volatile u32 WorkAroundType = 0;
static u32 WorkAroundSeekLocation = 0;
static volatile OSTime LastReadFinished = 0;
static OSTime LastReadIssued = 0;
static volatile s32 LastCommandWasRead = FALSE;
static volatile u32 NextCommandNumber = 0;

typedef struct DVDBuffer {
    void *addr;
    u32 length;
    u32 offset;
} DVDBuffer;

typedef struct DVDCommand {
    s32 cmd;
    void *addr;
    u32 length;
    u32 offset;
    DVDLowCallback callback;
} DVDCommand;

static DVDCommand CommandList[3];
static OSAlarm AlarmForWA;
static OSAlarm AlarmForTimeout;
static OSAlarm AlarmForBreak;
static DVDBuffer Prev;
static DVDBuffer Curr;

void __DVDInitWA()
{
    NextCommandNumber = 0;
    CommandList[0].cmd = -1;
    __DVDLowSetWAType(0, 0);
    OSInitAlarm();
}

static void Read(void *addr, u32 length, u32 offset, DVDLowCallback callback);

static inline s32 ProcessNextCommand()
{
    s32 n = NextCommandNumber;

    if (CommandList[n].cmd == 1) {
        ++NextCommandNumber;
        Read(CommandList[n].addr, CommandList[n].length, CommandList[n].offset,
             CommandList[n].callback);
        return TRUE;
    } else if (CommandList[n].cmd == 2) {
        ++NextCommandNumber;
        DVDLowSeek(CommandList[n].offset, CommandList[n].callback);
        return TRUE;
    }

    return FALSE;
}

void __DVDInterruptHandler(__OSInterrupt interrupt, OSContext *context)
{
    DVDLowCallback cb;
    OSContext exceptionContext;
    u32 cause = 0;
    u32 reg;
    u32 intr;
    u32 mask;

    OSCancelAlarm(&AlarmForTimeout);

    if (LastCommandWasRead) {
        LastReadFinished = __OSGetSystemTime();
        FirstRead = FALSE;
        Prev.addr = Curr.addr;
        Prev.length = Curr.length;
        Prev.offset = Curr.offset;
        if (StopAtNextInt == TRUE) {
            cause |= DVD_INTTYPE_BREAK;
        }
    }

    LastCommandWasRead = FALSE;
    StopAtNextInt = FALSE;
    reg = __DIRegs[0];
    mask = reg & 0x2a;
    intr = (reg & 0x54) & (mask << 1);

    if (intr & 0x40) {
        cause |= DVD_INTTYPE_BREAK;
    }

    if (intr & 0x10) {
        cause |= DVD_INTTYPE_TC;
    }

    if (intr & 4) {
        cause |= DVD_INTTYPE_DE;
    }

    if (cause) {
        ResetOccurred = FALSE;
    }

    __DIRegs[0] = intr | mask;

    if (ResetOccurred &&
        (__OSGetSystemTime() - LastResetEnd) < OSMillisecondsToTicks(200)) {
        reg = __DIRegs[1];
        mask = reg & 0x2;
        intr = (reg & 4) & (mask << 1);
        if (intr & 4) {
            if (ResetCoverCallback) {
                ResetCoverCallback(DVD_INTTYPE_CVR);
            }
            ResetCoverCallback = NULL;
        }

        __DIRegs[1] = __DIRegs[1];
    } else if (WaitingCoverClose) {
        reg = __DIRegs[1];
        mask = reg & 2;
        intr = (reg & 4) & (mask << 1);

        if (intr & 4) {
            cause |= DVD_INTTYPE_CVR;
        }

        __DIRegs[1] = intr | mask;
        WaitingCoverClose = FALSE;
    } else {
        __DIRegs[1] = 0;
    }

    if ((cause & DVD_INTTYPE_BREAK) && !Breaking) {
        cause &= ~DVD_INTTYPE_BREAK;
    }

    if ((cause & DVD_INTTYPE_TC)) {
        if (ProcessNextCommand()) {
            return;
        }
    } else {
        CommandList[0].cmd = -1;
        NextCommandNumber = 0;
    }

    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);

    if (cause) {
        cb = Callback;
        Callback = NULL;
        if (cb) {
            cb(cause);
        }

        Breaking = FALSE;
    }

    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

static void AlarmHandler(OSAlarm *alarm, OSContext *context)
{
    s32 error = ProcessNextCommand();
}

static void AlarmHandlerForTimeout(OSAlarm *alarm, OSContext *context)
{
    OSContext tmpContext;
    DVDLowCallback callback;
    __OSMaskInterrupts(OS_INTERRUPTMASK_PI_DI);
    OSClearContext(&tmpContext);
    OSSetCurrentContext(&tmpContext);
    callback = Callback;
    Callback = NULL;
    if (callback != NULL) {
        callback(DVD_INTTYPE_TIMEOUT);
    }
    OSClearContext(&tmpContext);
    OSSetCurrentContext(context);
}

static inline void SetTimeoutAlarm(OSTime timeout)
{
    OSCreateAlarm(&AlarmForTimeout);
    OSSetAlarm(&AlarmForTimeout, timeout, AlarmHandlerForTimeout);
}

static void Read(void *addr, u32 length, u32 offset, DVDLowCallback callback)
{
    StopAtNextInt = FALSE;
    LastCommandWasRead = TRUE;
    Callback = callback;
    LastReadIssued = __OSGetSystemTime();

    __DIRegs[2] = 0xa8000000;
    __DIRegs[3] = offset / 4;
    __DIRegs[4] = length;
    __DIRegs[5] = (u32)addr;
    __DIRegs[6] = length;
    LastLength = length;
    __DIRegs[7] = 3;

    if (length > 0xa00000) {
        SetTimeoutAlarm(OSSecondsToTicks(20));
    } else {
        SetTimeoutAlarm(OSSecondsToTicks(10));
    }
}

static inline s32 HitCache(DVDBuffer *cur, DVDBuffer *prev)
{
    u32 uVar1 = (prev->offset + prev->length - 1) >> 15;
    u32 uVar2 = (cur->offset >> 15);
    u32 iVar3 = (DVDGetCurrentDiskID()->streaming ? TRUE : FALSE) ? 5 : 15;

    if ((uVar2 > uVar1 - 2) || (uVar2 < uVar1 + iVar3 + 3)) {
        return TRUE;
    }
    return FALSE;
}

static inline void DoJustRead(void *addr, u32 length, u32 offset,
                              DVDLowCallback callback)
{
    CommandList[0].cmd = -1;
    NextCommandNumber = 0;
    Read(addr, length, offset, callback);
}

#pragma push
#pragma dont_inline on
static void SeekTwiceBeforeRead(void *addr, u32 length, u32 offset,
                                DVDLowCallback callback)
{
    u32 newOffset = offset & ~0x7FFF;
    if (!newOffset) {
        newOffset = 0;
    } else {
        newOffset += WorkAroundSeekLocation;
    }
    CommandList[0].cmd = 2;
    CommandList[0].offset = newOffset;
    CommandList[0].callback = callback;
    CommandList[1].cmd = 1;
    CommandList[1].addr = addr;
    CommandList[1].length = length;
    CommandList[1].offset = offset;
    CommandList[1].callback = callback;
    CommandList[2].cmd = -1;
    NextCommandNumber = 0;
    DVDLowSeek(newOffset, callback);
}

#pragma pop
static inline void WaitBeforeRead(void *addr, u32 length, u32 offset,
                                  DVDLowCallback callback, OSTime timeout)
{
    CommandList[0].cmd = 1;
    CommandList[0].addr = addr;
    CommandList[0].length = length;
    CommandList[0].offset = offset;
    CommandList[0].callback = callback;
    CommandList[1].cmd = -1;
    NextCommandNumber = 0;
    OSCreateAlarm(&AlarmForWA);
    OSSetAlarm(&AlarmForWA, timeout, AlarmHandler);
}

s32 DVDLowRead(void *addr, u32 length, u32 offset, DVDLowCallback callback)
{
    OSTime diff;
    u32 prev;

    __DIRegs[6] = length;
    Curr.addr = addr;
    Curr.length = length;
    Curr.offset = offset;

    if (WorkAroundType == 0) {
        DoJustRead(addr, length, offset, callback);
    } else if (WorkAroundType == 1) {
        if (FirstRead) {
            SeekTwiceBeforeRead(addr, length, offset, callback);
        } else {
            if (!HitCache(&Curr, &Prev)) {
                DoJustRead(addr, length, offset, callback);
            } else {
                prev = (Prev.offset + Prev.length - 1) >> 15;
                if (prev == Curr.offset >> 15 || prev + 1 == Curr.offset >> 15) {
                    diff = __OSGetSystemTime() - LastReadFinished;
                    if (OSMillisecondsToTicks(5) < diff) {
                        DoJustRead(addr, length, offset, callback);
                    } else {
                        WaitBeforeRead(addr, length, offset, callback,
                                       OSMillisecondsToTicks(5) - diff +
                                           OSMicrosecondsToTicks(500));
                    }
                } else {
                    SeekTwiceBeforeRead(addr, length, offset, callback);
                }
            }
        }
    }
    return TRUE;
}

s32 DVDLowSeek(u32 offset, DVDLowCallback callback)
{
    StopAtNextInt = FALSE;
    Callback = callback;
    __DIRegs[2] = 0xab000000;
    __DIRegs[3] = offset / 4;
    __DIRegs[7] = 1;
    SetTimeoutAlarm(OSSecondsToTicks(10));
    return TRUE;
}

s32 DVDLowWaitCoverClose(DVDLowCallback callback)
{
    Callback = callback;
    WaitingCoverClose = TRUE;
    StopAtNextInt = FALSE;
    __DIRegs[1] = 2;
    return TRUE;
}

s32 DVDLowReadDiskID(DVDDiskID *diskID, DVDLowCallback callback)
{
    StopAtNextInt = FALSE;
    Callback = callback;
    __DIRegs[2] = 0xa8000040;
    __DIRegs[3] = 0;
    __DIRegs[4] = sizeof(DVDDiskID);
    __DIRegs[5] = (u32)diskID;
    __DIRegs[6] = sizeof(DVDDiskID);
    __DIRegs[7] = 3;
    SetTimeoutAlarm(OSSecondsToTicks(10));
    return TRUE;
}

s32 DVDLowStopMotor(DVDLowCallback callback)
{
    StopAtNextInt = FALSE;
    Callback = callback;
    __DIRegs[2] = 0xe3000000;
    __DIRegs[7] = 1;
    SetTimeoutAlarm(OSSecondsToTicks(10));
    return TRUE;
}

s32 DVDLowRequestError(DVDLowCallback callback)
{
    StopAtNextInt = FALSE;
    Callback = callback;
    __DIRegs[2] = 0xe0000000;
    __DIRegs[7] = 1;
    SetTimeoutAlarm(OSSecondsToTicks(10));
    return TRUE;
}

s32 DVDLowInquiry(DVDDriveInfo *info, DVDLowCallback callback)
{
    StopAtNextInt = FALSE;
    Callback = callback;
    __DIRegs[2] = 0x12000000;
    __DIRegs[4] = sizeof(DVDDriveInfo);
    __DIRegs[5] = (u32)info;
    __DIRegs[6] = sizeof(DVDDriveInfo);
    __DIRegs[7] = 3;
    SetTimeoutAlarm(OSSecondsToTicks(10));
    return TRUE;
}

s32 DVDLowAudioStream(u32 subcmd, u32 length, u32 offset, DVDLowCallback callback)
{
    StopAtNextInt = FALSE;
    Callback = callback;
    __DIRegs[2] = subcmd | 0xe1000000;
    __DIRegs[3] = offset >> 2;
    __DIRegs[4] = length;
    __DIRegs[7] = 1;
    SetTimeoutAlarm(OSSecondsToTicks(10));
    return TRUE;
}

s32 DVDLowRequestAudioStatus(u32 subcmd, DVDLowCallback callback)
{
    StopAtNextInt = FALSE;
    Callback = callback;
    __DIRegs[2] = subcmd | 0xe2000000;
    __DIRegs[7] = 1;
    SetTimeoutAlarm(OSSecondsToTicks(10));
    return TRUE;
}

s32 DVDLowAudioBufferConfig(s32 enable, u32 size, DVDLowCallback callback)
{
    StopAtNextInt = FALSE;
    Callback = callback;
    __DIRegs[2] = 0xe4000000 | (enable != 0 ? 0x10000 : 0) | size;
    __DIRegs[7] = 1;
    SetTimeoutAlarm(OSSecondsToTicks(10));
    return TRUE;
}

void DVDLowReset()
{
    u32 reg;
    OSTime resetStart;

    __DIRegs[1] = 2;
    reg = __PIRegs[9];
    __PIRegs[9] = (reg & ~4) | 1;

    resetStart = __OSGetSystemTime();
    while ((__OSGetSystemTime() - resetStart) < OSMicrosecondsToTicks(12))
        ;

    __PIRegs[9] = reg | 5;
    ResetOccurred = TRUE;
    LastResetEnd = __OSGetSystemTime();
}

s32 DVDLowBreak()
{
    StopAtNextInt = TRUE;
    Breaking = TRUE;
    return TRUE;
}

DVDLowCallback DVDLowClearCallback()
{
    DVDLowCallback old;
    __DIRegs[1] = 0;
    old = Callback;
    Callback = NULL;
    return old;
}

void __DVDLowSetWAType(u32 type, u32 location)
{
    s32 enabled;
    enabled = OSDisableInterrupts();
    WorkAroundType = type;
    WorkAroundSeekLocation = location;
    OSRestoreInterrupts(enabled);
}
