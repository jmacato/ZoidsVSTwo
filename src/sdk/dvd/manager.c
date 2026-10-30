/* References: https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/dvd/dvd.c
 * and https://github.com/doldecomp/dolsdk2001/blob/main/src/dvd/dvd.c. */
#include "dvd_low.h"
#include "os_alarm.h"
#include "os_thread.h"
#include "os_error.h"
#define TRUE 1
#define FALSE 0
#define NULL 0
#define OSPhysicalToCached(value) ((void *)((u32)(value) + 0x80000000))
#define OSRoundUp32B(value) (((u32)(value) + 31) & ~31)
#define OSMillisecondsToTicks(value) ((value) * (__OSBusClock / 4 / 1000))
extern u32 __OSBusClock : 0x800000F8;
extern volatile u32 __DIRegs[16] : 0xCC006000;
extern void DCInvalidateRange(void *, u32);
extern void *memcpy(void *, const void *, u32);
extern int memcmp(const void *, const void *, u32);
extern OSThreadQueue __DVDThreadQueue;
extern void __DVDFSInit(void);
extern void __DVDClearWaitingQueue(void);
extern s32 __DVDPushWaitingQueue(long priority, DVDCommandBlock *block);
extern DVDCommandBlock *__DVDPopWaitingQueue(void);
extern s32 __DVDCheckWaitingQueue(void);
extern s32 __DVDDequeueWaitingQueue(DVDCommandBlock *block);
extern void __DVDStoreErrorCode(u32 error);
extern void __fstLoad(void);
typedef struct OSBootInfo_s {
    DVDDiskID DVDDiskID;
    unsigned long magic, version, memorySize, consoleType;
    void *arenaLo, *arenaHi, *FSTLocation;
    unsigned long FSTMaxLength;
} OSBootInfo;
typedef struct DVDBB2 {
    u32 bootFilePosition, FSTPosition, FSTLength, FSTMaxLength;
    void *FSTAddress;
    u32 userPosition, userLength, padding0;
} DVDBB2;
static inline s32 DVDCancelAllAsync(DVDCBCallback callback);
typedef void (*stateFunc)(DVDCommandBlock *block);
stateFunc LastState;

static u8 tmpBuffer[0x80] __attribute__((aligned(32)));
static DVDCommandBlock *executing;
static DVDDiskID *currID;
static OSBootInfo *bootInfo;
static s32 autoInvalidation = TRUE;
static volatile s32 PauseFlag = FALSE;
static volatile s32 PausingFlag = FALSE;
static volatile s32 AutoFinishing = FALSE;
static volatile s32 FatalErrorFlag = FALSE;
static volatile u32 CurrCommand;
static volatile u32 Canceling = FALSE;
static DVDCBCallback CancelCallback;
static volatile u32 ResumeFromHere = 0;
static volatile u32 CancelLastError;
static volatile u32 LastError;
static volatile s32 NumInternalRetry = 0;
static volatile s32 ResetRequired;
static volatile s32 CancelAllSyncComplete;
static volatile s32 FirstTimeInBootrom = FALSE;

static DVDCommandBlock DummyCommandBlock;
static OSAlarm ResetAlarm;

static s32 DVDInitialized = FALSE;

static void stateReadingFST();
static void stateTimeout();
static void stateGettingError();
static void stateGoToRetry();
static void stateCheckID();
static void stateCheckID3();
static void stateCheckID2();
static void stateCoverClosed();
static void stateCoverClosed_CMD();
static void stateCoverOpen();
static void stateMotorStopped();
static void stateReady();
static void stateBusy();

static void cbForStateReadingFST(u32 intType);
static void cbForStateError(u32 intType);
static void cbForStateGettingError(u32 intType);
static void cbForUnrecoveredError(u32 intType);
static void cbForUnrecoveredErrorRetry(u32 intType);
static void cbForStateGoToRetry(u32 intType);
static void cbForStateCheckID1(u32 intType);
static void cbForStateCheckID2(u32 intType);
static void cbForStateCheckID3(u32 intType);
static void cbForStateCoverClosed(u32 intType);
static void cbForStateMotorStopped(u32 intType);
static void cbForStateBusy(u32 intType);
static void cbForCancelStreamSync(s32 result, DVDCommandBlock *block);
static void cbForCancelSync(s32 result, DVDCommandBlock *block);

void DVDInit()
{
    if (DVDInitialized)
        return;

    OSInitAlarm();
    DVDInitialized = TRUE;
    __DVDFSInit();
    __DVDClearWaitingQueue();
    __DVDInitWA();
    bootInfo = (OSBootInfo *)OSPhysicalToCached(0x0000);
    currID = &(bootInfo->DVDDiskID);
    __OSSetInterruptHandler(__OS_INTERRUPT_PI_DI, __DVDInterruptHandler);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_DI);
    OSInitThreadQueue(&__DVDThreadQueue);
    __DIRegs[0] = 0x2a;
    __DIRegs[1] = 0;
    if (bootInfo->magic == 0xE5207C22) {
        OSReport("app booted via JTAG\n");
        OSReport("load fst\n");
        __fstLoad();
    } else if (bootInfo->magic == 0xD15EA5E) {
        OSReport("app booted from bootrom\n");
    } else {
        FirstTimeInBootrom = TRUE;
        OSReport("bootrom\n");
    }
}

static void stateReadingFST()
{
    LastState = (stateFunc)stateReadingFST;

    DVDLowRead(bootInfo->FSTLocation, OSRoundUp32B(((DVDBB2 *)&tmpBuffer)->FSTLength),
               ((DVDBB2 *)&tmpBuffer)->FSTPosition, cbForStateReadingFST);
}

static void cbForStateReadingFST(u32 intType)
{
    DVDCommandBlock *finished;

    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }

    if (intType & DVD_INTTYPE_TC) {
        NumInternalRetry = 0;
        finished = executing;
        executing = &DummyCommandBlock;
        finished->state = DVD_STATE_END;
        if (finished->callback) {
            (finished->callback)(DVD_RESULT_GOOD, finished);
        }

        stateReady();
    } else {

        stateGettingError();
    }
}

inline static void stateError(u32 error)
{
    __DVDStoreErrorCode(error);
    DVDLowStopMotor(cbForStateError);
}

static void cbForStateError(u32 intType)
{
    DVDCommandBlock *finished;

    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }

    FatalErrorFlag = TRUE;
    finished = executing;
    executing = &DummyCommandBlock;
    if (finished->callback) {
        (finished->callback)(DVD_RESULT_FATAL_ERROR, finished);
    }

    if (Canceling) {
        Canceling = FALSE;
        if (CancelCallback)
            (CancelCallback)(0, finished);
    }

    stateReady();

    return;
}

static void stateTimeout()
{
    __DVDStoreErrorCode(0x1234568);
    DVDReset();
    cbForStateError(0);
}

static void stateGettingError()
{
    DVDLowRequestError(cbForStateGettingError);
}

static u32 CategorizeError(u32 error)
{
    if (error == 0x20400) {
        LastError = error;
        return 1;
    }

    error &= 0xffffff;

    if ((error == 0x62800) || (error == 0x23a00) || (error == 0xb5a01)) {
        return 0;
    }

    ++NumInternalRetry;
    if (NumInternalRetry == 2) {
        if (error == LastError) {
            LastError = error;
            return 1;
        } else {
            LastError = error;
            return 2;
        }
    } else {
        LastError = error;

        if ((error == 0x31100) || (executing->command == DVD_COMMAND_READID)) {
            return 2;
        } else {
            return 3;
        }
    }
}

inline static s32 CheckCancel(u32 resume)
{
    DVDCommandBlock *finished;

    if (Canceling) {
        ResumeFromHere = resume;
        Canceling = FALSE;

        finished = executing;
        executing = &DummyCommandBlock;

        finished->state = DVD_STATE_CANCELED;
        if (finished->callback)
            (*finished->callback)(DVD_RESULT_CANCELED, finished);
        if (CancelCallback)
            (CancelCallback)(0, finished);
        stateReady();
        return TRUE;
    }
    return FALSE;
}

static void cbForStateGettingError(u32 intType)
{
    u32 error;
    u32 status;
    u32 errorCategory;
    u32 resume;

    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }

    if (intType & DVD_INTTYPE_DE) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateError(0x1234567);
        return;
    }

    error = __DIRegs[8];
    status = error & 0xff000000;

    errorCategory = CategorizeError(error);

    if (errorCategory == 1) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateError(error);
        return;
    }

    if ((errorCategory == 2) || (errorCategory == 3)) {
        resume = 0;
    } else {
        if (status == 0x01000000)
            resume = 4;
        else if (status == 0x02000000)
            resume = 6;
        else if (status == 0x03000000)
            resume = 3;
        else
            resume = 5;
    }

    if (CheckCancel(resume))
        return;

    if (errorCategory == 2) {
        __DVDStoreErrorCode(error);
        stateGoToRetry();
        return;
    }

    if (errorCategory == 3) {
        if ((error & 0x00ffffff) == 0x00031100) {
            DVDLowSeek(executing->offset, cbForUnrecoveredError);
        } else {
            LastState(executing);
        }
        return;
    }

    if (status == 0x01000000) {
        executing->state = DVD_STATE_COVER_OPEN;
        stateMotorStopped();
        return;
    } else if (status == 0x02000000) {
        executing->state = DVD_STATE_COVER_CLOSED;
        stateCoverClosed();
        return;
    } else if (status == 0x03000000) {
        executing->state = DVD_STATE_NO_DISK;
        stateMotorStopped();
        return;
    } else {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateError(0x1234567);
        return;
    }
}

static void cbForUnrecoveredError(u32 intType)
{
    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }

    if (intType & DVD_INTTYPE_TC) {
        stateGoToRetry();
        return;
    }

    DVDLowRequestError(cbForUnrecoveredErrorRetry);
}

static void cbForUnrecoveredErrorRetry(u32 intType)
{
    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }
    executing->state = DVD_STATE_FATAL_ERROR;

    if (intType & DVD_INTTYPE_DE) {
        __DVDStoreErrorCode(0x1234567);
        DVDLowStopMotor(cbForStateError);
        return;
    }

    __DVDStoreErrorCode(__DIRegs[8]);
    DVDLowStopMotor(cbForStateError);
}

static void stateGoToRetry()
{
    DVDLowStopMotor(cbForStateGoToRetry);
}

static void cbForStateGoToRetry(u32 intType)
{
    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }

    if (intType & DVD_INTTYPE_DE) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateError(0x1234567);
        return;
    }

    NumInternalRetry = 0;

    if ((CurrCommand == DVD_COMMAND_BSREAD) || (CurrCommand == DVD_COMMAND_READID) ||
        (CurrCommand == DVD_COMMAND_AUDIO_BUFFER_CONFIG) ||
        (CurrCommand == DVD_COMMAND_BS_CHANGE_DISK)) {
        ResetRequired = TRUE;
    }

    if (!CheckCancel(2)) {
        executing->state = DVD_STATE_RETRY;
        stateMotorStopped();
    }
}

static void stateCheckID()
{
    switch (CurrCommand) {
    case DVD_COMMAND_CHANGE_DISK:

        if (memcmp(&tmpBuffer, executing->id, sizeof(DVDDiskID) - sizeof(void *)) !=
            0) {
            DVDLowStopMotor(cbForStateCheckID1);
        } else {
            memcpy(currID, &tmpBuffer, sizeof(DVDDiskID));

            executing->state = DVD_STATE_BUSY;
            DCInvalidateRange(&tmpBuffer, sizeof(DVDBB2));
            LastState = stateCheckID2;
            stateCheckID2(executing);
            return;
        }
        break;

    default:
        if (memcmp(&tmpBuffer, currID, sizeof(DVDDiskID))) {
            DVDLowStopMotor(cbForStateCheckID1);
        } else {
            LastState = stateCheckID3;
            stateCheckID3(executing);
        }
        break;
    }
}

static void stateCheckID3()
{
    DVDLowAudioBufferConfig(currID->streaming, 10, cbForStateCheckID3);
}

static void stateCheckID2()
{
    DVDLowRead(&tmpBuffer, OSRoundUp32B(sizeof(DVDBB2)), 0x420, cbForStateCheckID2);
}

static void cbForStateCheckID1(u32 intType)
{
    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }

    if (intType & DVD_INTTYPE_DE) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateError(0x1234567);
        return;
    }

    NumInternalRetry = 0;

    if (!CheckCancel(1)) {
        executing->state = DVD_STATE_WRONG_DISK;
        stateMotorStopped();
    }
}

static void cbForStateCheckID2(u32 intType)
{
    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }

    if (intType & DVD_INTTYPE_TC) {

        NumInternalRetry = 0;

        stateReadingFST();
    } else {

        stateGettingError();
    }
}

static void cbForStateCheckID3(u32 intType)
{
    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }

    if (intType & DVD_INTTYPE_TC) {

        NumInternalRetry = 0;

        if (!CheckCancel(0)) {
            executing->state = DVD_STATE_BUSY;
            stateBusy(executing);
        }
    } else {
        stateGettingError();
    }
}

static void AlarmHandler(OSAlarm *alarm, OSContext *context)
{
    DVDReset();
    DCInvalidateRange(&tmpBuffer, sizeof(DVDDiskID));
    LastState = stateCoverClosed_CMD;
    stateCoverClosed_CMD(executing);
}

static void stateCoverClosed()
{
    DVDCommandBlock *finished;

    switch (CurrCommand) {
    case DVD_COMMAND_READID:
    case DVD_COMMAND_BSREAD:
    case DVD_COMMAND_AUDIO_BUFFER_CONFIG:
    case DVD_COMMAND_BS_CHANGE_DISK:
        __DVDClearWaitingQueue();
        finished = executing;
        executing = &DummyCommandBlock;
        if (finished->callback) {
            (finished->callback)(-4, finished);
        }
        stateReady();
        break;

    default:
        DVDReset();
        OSCreateAlarm(&ResetAlarm);
        OSSetAlarm(&ResetAlarm, OSMillisecondsToTicks(1150), AlarmHandler);
        break;
    }
}

static void stateCoverClosed_CMD(DVDCommandBlock *block)
{
    DVDLowReadDiskID((DVDDiskID *)&tmpBuffer, cbForStateCoverClosed);
}

static void cbForStateCoverClosed(u32 intType)
{
    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }

    if (intType & DVD_INTTYPE_TC) {
        NumInternalRetry = 0;
        stateCheckID();
    } else {
        stateGettingError();
    }
}

static void stateMotorStopped(void)
{
    DVDLowWaitCoverClose(cbForStateMotorStopped);
}

static void cbForStateMotorStopped(u32 intType)
{
    __DIRegs[1] = 0;
    executing->state = DVD_STATE_COVER_CLOSED;
    stateCoverClosed();
}

static void stateReady()
{
    DVDCommandBlock *finished;

    if (!__DVDCheckWaitingQueue()) {
        executing = (DVDCommandBlock *)NULL;
        return;
    }

    if (PauseFlag) {
        PausingFlag = TRUE;
        executing = (DVDCommandBlock *)NULL;
        return;
    }

    executing = __DVDPopWaitingQueue();

    if (FatalErrorFlag) {
        executing->state = DVD_STATE_FATAL_ERROR;
        finished = executing;
        executing = &DummyCommandBlock;
        if (finished->callback) {
            (finished->callback)(DVD_RESULT_FATAL_ERROR, finished);
        }
        stateReady();
        return;
    }

    CurrCommand = executing->command;

    if (ResumeFromHere) {
        switch (ResumeFromHere) {
        case 1:
            executing->state = DVD_STATE_WRONG_DISK;
            stateMotorStopped();
            break;

        case 2:
            executing->state = DVD_STATE_RETRY;
            stateMotorStopped();
            break;

        case 3:
            executing->state = DVD_STATE_NO_DISK;
            stateMotorStopped();
            break;

        case 7:
            executing->state = DVD_STATE_MOTOR_STOPPED;
            stateMotorStopped();
            break;

        case 4:
            executing->state = DVD_STATE_COVER_OPEN;
            stateMotorStopped();
            break;

        case 6:
            executing->state = DVD_STATE_COVER_CLOSED;
            stateCoverClosed();
            break;

        case 5:
            executing->state = DVD_STATE_FATAL_ERROR;
            stateError(CancelLastError);
            break;
        }

        ResumeFromHere = 0;
    } else {
        executing->state = DVD_STATE_BUSY;
        stateBusy(executing);
    }
}

static void stateBusy(DVDCommandBlock *block)
{
    DVDCommandBlock *finished;
    LastState = stateBusy;
    switch (block->command) {
    case DVD_COMMAND_READID:
        __DIRegs[1] = __DIRegs[1];
        block->currTransferSize = sizeof(DVDDiskID);
        DVDLowReadDiskID(block->addr, cbForStateBusy);
        break;
    case DVD_COMMAND_READ:
    case DVD_COMMAND_BSREAD:
        __DIRegs[1] = __DIRegs[1];
        block->currTransferSize = block->length - block->transferredSize > 0x80000
                                      ? 0x80000
                                      : block->length - block->transferredSize;
        DVDLowRead((void *)((u8 *)block->addr + block->transferredSize),
                   block->currTransferSize, block->offset + block->transferredSize,
                   cbForStateBusy);
        break;
    case DVD_COMMAND_SEEK:
        __DIRegs[1] = __DIRegs[1];
        DVDLowSeek(block->offset, cbForStateBusy);
        break;
    case DVD_COMMAND_CHANGE_DISK:
        DVDLowStopMotor(cbForStateBusy);
        break;
    case DVD_COMMAND_BS_CHANGE_DISK:
        DVDLowStopMotor(cbForStateBusy);
        break;
    case DVD_COMMAND_INITSTREAM:
        __DIRegs[1] = __DIRegs[1];
        if (AutoFinishing) {
            executing->currTransferSize = 0;
            DVDLowRequestAudioStatus(0, cbForStateBusy);
        } else {
            executing->currTransferSize = 1;
            DVDLowAudioStream(0, block->length, block->offset, cbForStateBusy);
        }
        break;
    case DVD_COMMAND_CANCELSTREAM:
        __DIRegs[1] = __DIRegs[1];
        DVDLowAudioStream(0x10000, 0, 0, cbForStateBusy);
        break;
    case DVD_COMMAND_STOP_STREAM_AT_END:
        __DIRegs[1] = __DIRegs[1];
        AutoFinishing = TRUE;
        DVDLowAudioStream(0, 0, 0, cbForStateBusy);
        break;
    case DVD_COMMAND_REQUEST_AUDIO_ERROR:
        __DIRegs[1] = __DIRegs[1];
        DVDLowRequestAudioStatus(0, cbForStateBusy);
        break;
    case DVD_COMMAND_REQUEST_PLAY_ADDR:
        __DIRegs[1] = __DIRegs[1];
        DVDLowRequestAudioStatus(0x10000, cbForStateBusy);
        break;
    case DVD_COMMAND_REQUEST_START_ADDR:
        __DIRegs[1] = __DIRegs[1];
        DVDLowRequestAudioStatus(0x20000, cbForStateBusy);
        break;
    case DVD_COMMAND_REQUEST_LENGTH:
        __DIRegs[1] = __DIRegs[1];
        DVDLowRequestAudioStatus(0x30000, cbForStateBusy);
        break;
    case DVD_COMMAND_AUDIO_BUFFER_CONFIG:
        __DIRegs[1] = __DIRegs[1];
        DVDLowAudioBufferConfig(block->offset, block->length, cbForStateBusy);
        break;
    case DVD_COMMAND_INQUIRY:
        __DIRegs[1] = __DIRegs[1];
        block->currTransferSize = sizeof(DVDDriveInfo);
        DVDLowInquiry(block->addr, cbForStateBusy);
        break;
    default:
        break;
    }
}

static void cbForStateBusy(u32 intType)
{
    DVDCommandBlock *finished;

    if (intType == DVD_INTTYPE_TIMEOUT) {
        executing->state = DVD_STATE_FATAL_ERROR;
        stateTimeout();
        return;
    }

    if ((CurrCommand == DVD_COMMAND_CHANGE_DISK) ||
        (CurrCommand == DVD_COMMAND_BS_CHANGE_DISK)) {
        if (intType & DVD_INTTYPE_DE) {
            executing->state = DVD_STATE_FATAL_ERROR;
            stateError(0x1234567);
            return;
        }

        NumInternalRetry = 0;

        if (CurrCommand == DVD_COMMAND_BS_CHANGE_DISK) {
            ResetRequired = TRUE;
        }

        if (CheckCancel(7)) {
            return;
        }

        executing->state = DVD_STATE_MOTOR_STOPPED;
        stateMotorStopped();
        return;
    }

    if (CurrCommand == DVD_COMMAND_READ || CurrCommand == DVD_COMMAND_BSREAD ||
        CurrCommand == DVD_COMMAND_READID || CurrCommand == DVD_COMMAND_INQUIRY) {
        executing->transferredSize += executing->currTransferSize - __DIRegs[6];
    }

    if (intType & DVD_INTTYPE_BREAK) {
        Canceling = FALSE;
        finished = executing;
        executing = &DummyCommandBlock;

        finished->state = DVD_STATE_CANCELED;
        if (finished->callback)
            (*finished->callback)(DVD_RESULT_CANCELED, finished);
        if (CancelCallback)
            (CancelCallback)(0, finished);
        stateReady();

        return;
    }

    if (intType & DVD_INTTYPE_TC) {
        NumInternalRetry = 0;

        if (CheckCancel(0))
            return;

        if (CurrCommand == DVD_COMMAND_READ || CurrCommand == DVD_COMMAND_BSREAD ||
            CurrCommand == DVD_COMMAND_READID || CurrCommand == DVD_COMMAND_INQUIRY) {
            if (executing->transferredSize != executing->length) {
                stateBusy(executing);
                return;
            }

            finished = executing;
            executing = &DummyCommandBlock;

            finished->state = DVD_STATE_END;
            if (finished->callback) {
                (finished->callback)((s32)finished->transferredSize, finished);
            }
            stateReady();
        } else if (CurrCommand == DVD_COMMAND_REQUEST_AUDIO_ERROR ||
                   CurrCommand == DVD_COMMAND_REQUEST_PLAY_ADDR ||
                   CurrCommand == DVD_COMMAND_REQUEST_START_ADDR ||
                   CurrCommand == DVD_COMMAND_REQUEST_LENGTH) {
            s32 result;

            if ((CurrCommand == DVD_COMMAND_REQUEST_START_ADDR) ||
                (CurrCommand == DVD_COMMAND_REQUEST_PLAY_ADDR)) {
                result = (s32)(__DIRegs[8] << 2);
            } else {
                result = (s32)__DIRegs[8];
            }
            finished = executing;
            executing = &DummyCommandBlock;

            finished->state = DVD_STATE_END;
            if (finished->callback) {
                (finished->callback)(result, finished);
            }
            stateReady();
        } else if (CurrCommand == DVD_COMMAND_INITSTREAM) {
            if (executing->currTransferSize == 0) {
                if (__DIRegs[8] & 1) {
                    finished = executing;
                    executing = &DummyCommandBlock;

                    finished->state = DVD_STATE_IGNORED;
                    if (finished->callback) {
                        (finished->callback)(DVD_RESULT_IGNORED, finished);
                    }
                    stateReady();
                } else {
                    AutoFinishing = FALSE;
                    executing->currTransferSize = 1;
                    DVDLowAudioStream(0, executing->length, executing->offset,
                                      cbForStateBusy);
                }
            } else {
                finished = executing;
                executing = &DummyCommandBlock;

                finished->state = DVD_STATE_END;
                if (finished->callback) {
                    (finished->callback)(DVD_RESULT_GOOD, finished);
                }
                stateReady();
            }
        } else {
            finished = executing;
            executing = &DummyCommandBlock;

            finished->state = DVD_STATE_END;
            if (finished->callback) {
                (finished->callback)(DVD_RESULT_GOOD, finished);
            }
            stateReady();
        }
    } else {
        if (CurrCommand == DVD_COMMAND_INQUIRY) {
            executing->state = DVD_STATE_FATAL_ERROR;
            stateError(0x01234567);
            return;
        }

        if ((CurrCommand == DVD_COMMAND_READ || CurrCommand == DVD_COMMAND_BSREAD ||
             CurrCommand == DVD_COMMAND_READID || CurrCommand == DVD_COMMAND_INQUIRY) &&
            (executing->transferredSize == executing->length)) {
            if (CheckCancel(0)) {
                return;
            }
            finished = executing;
            executing = &DummyCommandBlock;

            finished->state = DVD_STATE_END;
            if (finished->callback) {
                (finished->callback)((s32)finished->transferredSize, finished);
            }
            stateReady();
            return;
        }

        stateGettingError();
    }
}

static s32 issueCommand(s32 prio, DVDCommandBlock *block)
{
    s32 level;
    s32 result;

    if (autoInvalidation &&
        (block->command == DVD_COMMAND_READ || block->command == DVD_COMMAND_BSREAD ||
         block->command == DVD_COMMAND_READID || block->command == DVD_COMMAND_INQUIRY)) {
        DCInvalidateRange(block->addr, block->length);
    }

    level = OSDisableInterrupts();

    block->state = DVD_STATE_WAITING;
    result = __DVDPushWaitingQueue(prio, block);

    if ((executing == (DVDCommandBlock *)NULL) && (PauseFlag == FALSE)) {
        stateReady();
    }

    OSRestoreInterrupts(level);

    return result;
}

s32 DVDReadAbsAsyncPrio(DVDCommandBlock *block, void *addr, s32 length, s32 offset,
                        DVDCBCallback callback, s32 prio)
{
    s32 idle;
    block->command = DVD_COMMAND_READ;
    block->addr = addr;
    block->length = length;
    block->offset = offset;
    block->transferredSize = 0;
    block->callback = callback;

    idle = issueCommand(prio, block);
    return idle;
}

s32 DVDSeekAbsAsyncPrio(DVDCommandBlock *block, s32 offset, DVDCBCallback callback,
                        s32 prio)
{
    s32 idle;
    block->command = DVD_COMMAND_SEEK;
    block->offset = offset;
    block->callback = callback;
    idle = issueCommand(prio, block);
    return idle;
}
s32 DVDReadAbsAsyncForBS(DVDCommandBlock *block, void *addr, s32 length, s32 offset,
                         DVDCBCallback callback)
{
    s32 idle;
    block->command = DVD_COMMAND_BSREAD;
    block->addr = addr;
    block->length = length;
    block->offset = offset;
    block->transferredSize = 0;
    block->callback = callback;

    idle = issueCommand(2, block);
    return idle;
}

s32 DVDReadDiskID(DVDCommandBlock *block, DVDDiskID *diskID, DVDCBCallback callback)
{
    s32 idle;
    block->command = DVD_COMMAND_READID;
    block->addr = diskID;
    block->length = sizeof(DVDDiskID);
    ;
    block->offset = 0;
    block->transferredSize = 0;
    block->callback = callback;

    idle = issueCommand(2, block);
    return idle;
}

s32 DVDPrepareStreamAbsAsync(DVDCommandBlock *block, u32 length, u32 offset,
                             DVDCBCallback callback)
{
    s32 idle;
    block->command = DVD_COMMAND_INITSTREAM;
    block->length = length;
    block->offset = offset;
    block->callback = callback;

    idle = issueCommand(1, block);
    return idle;
}

static inline s32 DVDCancelStreamAsync(DVDCommandBlock *block, DVDCBCallback callback)
{
    s32 idle;
    block->command = DVD_COMMAND_CANCELSTREAM;
    block->callback = callback;
    idle = issueCommand(1, block);
    return idle;
}

s32 DVDCancelStream(DVDCommandBlock *block)
{
    s32 result, state, enabled, retVal;
    result = DVDCancelStreamAsync(block, cbForCancelStreamSync);
    if (result == FALSE) {
        return -1;
    }
    enabled = OSDisableInterrupts();
    for (;;) {
        state = ((volatile DVDCommandBlock *)block)->state;
        if (state == DVD_STATE_END || state == DVD_STATE_FATAL_ERROR ||
            state == DVD_STATE_CANCELED) {
            retVal = (s32)block->transferredSize;
            break;
        }
        OSSleepThread(&__DVDThreadQueue);
    }
    OSRestoreInterrupts(enabled);
    return retVal;
}
static void cbForCancelStreamSync(s32 result, DVDCommandBlock *block)
{
    block->transferredSize = (u32)result;
    OSWakeupThread(&__DVDThreadQueue);
}
s32 DVDStopStreamAtEndAsync(DVDCommandBlock *block, DVDCBCallback callback)
{
    s32 idle;

    block->command = DVD_COMMAND_STOP_STREAM_AT_END;
    block->callback = callback;

    idle = issueCommand(1, block);

    return idle;
}

static inline s32 DVDGetStreamPlayAddrAsync(DVDCommandBlock *block,
                                            DVDCBCallback callback)
{
    s32 idle;

    block->command = DVD_COMMAND_REQUEST_PLAY_ADDR;
    block->callback = callback;

    idle = issueCommand(1, block);

    return idle;
}
s32 DVDInquiryAsync(DVDCommandBlock *block, DVDDriveInfo *info, DVDCBCallback callback)
{
    s32 idle;

    block->command = DVD_COMMAND_INQUIRY;
    block->addr = (void *)info;
    block->length = sizeof(DVDDriveInfo);
    block->transferredSize = 0;
    block->callback = callback;

    idle = issueCommand(2, block);

    return idle;
}

void DVDReset(void)
{
    DVDLowReset();
    __DIRegs[0] = 0x2a;
    __DIRegs[1] = __DIRegs[1];
    ResetRequired = FALSE;
    ResumeFromHere = 0;
}

s32 DVDGetCommandBlockStatus(DVDCommandBlock *block)
{
    s32 enabled;
    s32 retVal;

    enabled = OSDisableInterrupts();

    if (block->state == DVD_STATE_COVER_CLOSED) {
        retVal = 1;
    } else {
        retVal = block->state;
    }

    OSRestoreInterrupts(enabled);

    return retVal;
}

s32 DVDGetDriveStatus()
{
    s32 enabled;
    s32 retVal;

    enabled = OSDisableInterrupts();

    if (FatalErrorFlag) {
        retVal = -1;
    } else if (PausingFlag) {
        retVal = 8;
    } else {
        if (executing == (DVDCommandBlock *)NULL) {
            retVal = 0;
        } else if (executing == &DummyCommandBlock) {
            retVal = 0;
        } else {
            retVal = DVDGetCommandBlockStatus(executing);
        }
    }

    OSRestoreInterrupts(enabled);

    return retVal;
}

s32 DVDSetAutoInvalidation(s32 autoInval)
{
    s32 prev;
    prev = autoInvalidation;
    autoInvalidation = autoInval;
    return prev;
}

static inline void DVDPause(void)
{
    s32 level;
    level = OSDisableInterrupts();
    PauseFlag = TRUE;
    if (executing == (DVDCommandBlock *)NULL) {
        PausingFlag = TRUE;
    }
    OSRestoreInterrupts(level);
}

static inline void DVDResume(void)
{
    s32 level;
    level = OSDisableInterrupts();
    PauseFlag = FALSE;
    if (PausingFlag) {
        PausingFlag = FALSE;
        stateReady();
    }
    OSRestoreInterrupts(level);
}

s32 DVDCancelAsync(DVDCommandBlock *block, DVDCBCallback callback)
{
    s32 enabled;
    DVDLowCallback old;

    enabled = OSDisableInterrupts();

    switch (block->state) {
    case DVD_STATE_FATAL_ERROR:
    case DVD_STATE_END:
    case DVD_STATE_CANCELED:
        if (callback)
            (*callback)(0, block);
        break;

    case DVD_STATE_BUSY:
        if (Canceling) {
            OSRestoreInterrupts(enabled);
            return FALSE;
        }

        Canceling = TRUE;
        CancelCallback = callback;
        if (block->command == DVD_COMMAND_BSREAD || block->command == DVD_COMMAND_READ) {
            DVDLowBreak();
        }
        break;

    case DVD_STATE_WAITING:
        __DVDDequeueWaitingQueue(block);
        block->state = DVD_STATE_CANCELED;
        if (block->callback)
            (block->callback)(DVD_RESULT_CANCELED, block);
        if (callback)
            (*callback)(0, block);
        break;

    case DVD_STATE_COVER_CLOSED:
        switch (block->command) {
        case DVD_COMMAND_READID:
        case DVD_COMMAND_BSREAD:
        case DVD_COMMAND_AUDIO_BUFFER_CONFIG:
        case DVD_COMMAND_BS_CHANGE_DISK:
            if (callback)
                (*callback)(0, block);
            break;

        default:
            if (Canceling) {
                OSRestoreInterrupts(enabled);
                return FALSE;
            }
            Canceling = TRUE;
            CancelCallback = callback;
            break;
        }
        break;

    case DVD_STATE_NO_DISK:
    case DVD_STATE_COVER_OPEN:
    case DVD_STATE_WRONG_DISK:
    case DVD_STATE_MOTOR_STOPPED:
    case DVD_STATE_RETRY:
        old = DVDLowClearCallback();
        if (old != cbForStateMotorStopped) {
            OSRestoreInterrupts(enabled);
            return FALSE;
        }

        if (block->state == DVD_STATE_NO_DISK)
            ResumeFromHere = 3;
        if (block->state == DVD_STATE_COVER_OPEN)
            ResumeFromHere = 4;
        if (block->state == DVD_STATE_WRONG_DISK)
            ResumeFromHere = 1;
        if (block->state == DVD_STATE_RETRY)
            ResumeFromHere = 2;
        if (block->state == DVD_STATE_MOTOR_STOPPED)
            ResumeFromHere = 7;

        block->state = DVD_STATE_CANCELED;
        if (block->callback) {
            (block->callback)(DVD_RESULT_CANCELED, block);
        }
        if (callback) {
            (callback)(0, block);
        }
        stateReady();
        break;
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

s32 DVDCancel(DVDCommandBlock *block)
{
    s32 result;
    s32 state;
    u32 command;
    s32 enabled;

    result = DVDCancelAsync(block, cbForCancelSync);

    if (result == FALSE) {
        return -1;
    }

    enabled = OSDisableInterrupts();

    for (;;) {
        state = ((volatile DVDCommandBlock *)block)->state;

        if ((state == DVD_STATE_END) || (state == DVD_STATE_FATAL_ERROR) ||
            (state == DVD_STATE_CANCELED)) {
            break;
        }

        if (state == DVD_STATE_COVER_CLOSED) {
            command = ((volatile DVDCommandBlock *)block)->command;

            if ((command == DVD_COMMAND_BSREAD) || (command == DVD_COMMAND_READID) ||
                (command == DVD_COMMAND_AUDIO_BUFFER_CONFIG) ||
                (command == DVD_COMMAND_BS_CHANGE_DISK)) {
                break;
            }
        }

        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return 0;
}

static void cbForCancelSync(s32 result, DVDCommandBlock *block)
{
    OSWakeupThread(&__DVDThreadQueue);
}

static inline s32 DVDCancelAllAsync(DVDCBCallback callback)
{
    s32 enabled;
    DVDCommandBlock *p;
    s32 retVal;

    enabled = OSDisableInterrupts();
    DVDPause();

    while ((p = __DVDPopWaitingQueue()) != 0) {
        DVDCancelAsync(p, NULL);
    }

    if (executing)
        retVal = DVDCancelAsync(executing, callback);
    else {
        retVal = TRUE;
        if (callback)
            (*callback)(0, NULL);
    }

    DVDResume();
    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForCancelAllSync(s32 result, DVDCommandBlock *block);
s32 DVDCancelAll(void)
{
    s32 result, enabled;
    enabled = OSDisableInterrupts();
    CancelAllSyncComplete = FALSE;
    result = DVDCancelAllAsync(cbForCancelAllSync);
    if (result == FALSE) {
        OSRestoreInterrupts(enabled);
        return -1;
    }
    for (;;) {
        if (CancelAllSyncComplete == FALSE) {
            OSSleepThread(&__DVDThreadQueue);
        } else {
            break;
        }
    }
    OSRestoreInterrupts(enabled);
    return 0;
}
static void cbForCancelAllSync(s32 result, DVDCommandBlock *block)
{
    CancelAllSyncComplete = TRUE;
    OSWakeupThread(&__DVDThreadQueue);
}
DVDDiskID *DVDGetCurrentDiskID(void)
{
    return (DVDDiskID *)OSPhysicalToCached(0);
}

s32 DVDCheckDisk(void)
{
    s32 enabled;
    s32 retVal;
    s32 state;
    u32 coverReg;

    enabled = OSDisableInterrupts();

    if (FatalErrorFlag) {
        state = DVD_STATE_FATAL_ERROR;
    } else if (PausingFlag) {
        state = DVD_STATE_PAUSING;
    } else {
        if (executing == (DVDCommandBlock *)NULL) {
            state = DVD_STATE_END;
        } else if (executing == &DummyCommandBlock) {
            state = DVD_STATE_END;
        } else {
            state = executing->state;
        }
    }

    switch (state) {
    case DVD_STATE_BUSY:
    case DVD_STATE_IGNORED:
    case DVD_STATE_CANCELED:
    case DVD_STATE_WAITING:
        retVal = TRUE;
        break;

    case DVD_STATE_FATAL_ERROR:
    case DVD_STATE_RETRY:
    case DVD_STATE_MOTOR_STOPPED:
    case DVD_STATE_COVER_CLOSED:
    case DVD_STATE_NO_DISK:
    case DVD_STATE_COVER_OPEN:
    case DVD_STATE_WRONG_DISK:
        retVal = FALSE;
        break;

    case DVD_STATE_END:
    case DVD_STATE_PAUSING:
        coverReg = __DIRegs[1];
        if (((coverReg >> 2) & 1) || (coverReg & 1)) {
            retVal = FALSE;
        } else {
            retVal = TRUE;
        }
    }

    OSRestoreInterrupts(enabled);

    return retVal;
}

void __DVDPrepareResetAsync(DVDCBCallback callback)
{
    s32 enabled;

    enabled = OSDisableInterrupts();

    __DVDClearWaitingQueue();

    if (Canceling) {
        CancelCallback = callback;
    } else {
        if (executing) {
            executing->callback = NULL;
        }

        DVDCancelAllAsync(callback);
    }

    OSRestoreInterrupts(enabled);
}
