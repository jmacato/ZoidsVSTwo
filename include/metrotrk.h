/* Names from https://github.com/doldecomp/melee/tree/master/src/MetroTRK */
#ifndef ZOIDS_METROTRK_H
#define ZOIDS_METROTRK_H

#include "types.h"

typedef enum DSError {
    kUARTError = -0x0001,
    kNoError = 0x0000,
    kStepError = 0x0001,
    kParameterError = 0x0002,
    kEventQueueFull = 0x0100,
    kNoMessageBufferAvailable = 0x0300,
    kMessageBufferOverflow = 0x0301,
    kMessageBufferReadError = 0x0302,
    kDispatchError = 0x0500,
    kInvalidMemory = 0x0700,
    kInvalidRegister = 0x0701,
    kCWDSException = 0x0702,
    kUnsupportedError = 0x0703,
    kInvalidProcessId = 0x0704,
    kInvalidThreadId = 0x0705,
    kOsError = 0x0706,
    kWaitACKError = 0x0800
} DSError;

typedef enum MessageCommandID {
    kDSPing = 0x00,
    kDSConnect = 0x01,
    kDSDisconnect = 0x02,
    kDSReset = 0x03,
    kDSVersions = 0x04,
    kDSSupportMask = 0x05,
    kDSOverride = 0x07,
    kDSReadMemory = 0x10,
    kDSWriteMemory = 0x11,
    kDSReadRegisters = 0x12,
    kDSWriteRegisters = 0x13,
    kDSSetOption = 0x17,
    kDSContinue = 0x18,
    kDSStep = 0x19,
    kDSStop = 0x1A,
    kDSReplyACK = 0x80,
    kDSReplyNAK = 0xFF,
    kDSNotifyStopped = 0x90,
    kDSNotifyException = 0x91,
    kDSWriteFile = 0xD0,
    kDSReadFile = 0xD1,
    kDSOpenFile = 0xD2,
    kDSCloseFile = 0xD3,
    kDSPositionFile = 0xD4
} MessageCommandID;

typedef enum DSReplyError {
    kDSReplyNoError = 0x00,
    kDSReplyError = 0x01,
    kDSReplyPacketSizeError = 0x02,
    kDSReplyCWDSError = 0x03,
    kDSReplyEscapeError = 0x04,
    kDSReplyBadFCS = 0x05,
    kDSReplyOverflow = 0x06,
    kDSReplySequenceMissing = 0x07,
    kDSReplyUnsupportedCommandError = 0x10,
    kDSReplyParameterError = 0x11,
    kDSReplyUnsupportedOptionError = 0x12,
    kDSReplyInvalidMemoryRange = 0x13,
    kDSReplyInvalidRegisterRange = 0x14,
    kDSReplyCWDSException = 0x15,
    kDSReplyNotStopped = 0x16,
    kDSReplyBreakpointsFull = 0x17,
    kDSReplyBreakpointConflict = 0x18,
    kDSReplyOsError = 0x20,
    kDSReplyInvalidProcessId = 0x21,
    kDSReplyInvalidThreadId = 0x22,
    kDSDebugSecurityError = 0x23
} DSReplyError;

typedef enum DSMessageRegisterOptions {
    kDSRegistersDefault = 0x0,
    kDSRegistersFP = 0x1,
    kDSRegistersExtended1 = 0x2,
    kDSRegistersExtended2 = 0x3
} DSMessageRegisterOptions;

typedef enum DSMessageStepOptions {
    kDSStepIntoCount = 0x00,
    kDSStepIntoRange = 0x01,
    kDSStepOverCount = 0x10,
    kDSStepOverRange = 0x11
} DSMessageStepOptions;

typedef enum DSIOResult {
    kDSIONoError = 0x00,
    kDSIOError = 0x01,
    kDSIOEOF = 0x02
} DSIOResult;

typedef enum NubEventType {
    kNullEvent = 0,
    kShutdownEvent = 1,
    kRequestEvent = 2,
    kBreakpointEvent = 3,
    kExceptionEvent = 4,
    kSupportEvent = 5
} NubEventType;

typedef enum MemoryAccessOptions {
    kUserMemory = 0,
    kDebuggerMemory = 1
} MemoryAccessOptions;

#define DS_MSG_MEMORY_SEGMENTED 0x01
#define DS_MSG_MEMORY_EXTENDED 0x02
#define DS_MSG_MEMORY_PROTECTED 0x04
#define DS_MSG_MEMORY_USERVIEW 0x08

#define DS_MIN_MSG_LENGTH 1
#define DS_MIN_REPLY_LENGTH 2
#define DS_MIN_MSG_READMEMORY_LENGTH (DS_MIN_MSG_LENGTH + 7)
#define DS_MIN_MSG_WRITEMEMORY_LENGTH (DS_MIN_MSG_LENGTH + 7)
#define DS_MIN_MSG_FLUSHCACHE_LENGTH (DS_MIN_MSG_LENGTH + 9)
#define DS_MIN_MSG_READREGISTERS_LENGTH (DS_MIN_MSG_LENGTH + 5)
#define DS_MIN_MSG_WRITEREGISTERS_LENGTH (DS_MIN_MSG_LENGTH + 5)
#define DS_MIN_MSG_STEP_COUNT_LENGTH (DS_MIN_MSG_LENGTH + 2)
#define DS_MIN_MSG_STEP_RANGE_LENGTH (DS_MIN_MSG_LENGTH + 9)

#define DS_MAXREADWRITELENGTH 0x0800
#define DS_MAXMESSAGESIZE (DS_MAXREADWRITELENGTH + 0x80)
#define NUM_BUFFERS 3

#define DS_CPU_MAJOR_PPC 0x00

#define TRK_DEFAULT_XER 36
#define TRK_FLOAT_FPSCR 32
#define TRK_FLOAT_FPECR 33

#define PPC_SYSTEMRESET 0x0100
#define PPC_MACHINECHECK 0x0200
#define PPC_DATAACCESSERROR 0x0300
#define PPC_INSTACCESSERROR 0x0400
#define PPC_EXTERNALINTERRUPT 0x0500
#define PPC_ALIGNMENTERROR 0x0600
#define PPC_PROGRAMERROR 0x0700
#define PPC_FPUNAVAILABLE 0x0800
#define PPC_DECREMENTERINTERRUPT 0x0900
#define PPC_SYSTEMCALL 0x0C00
#define PPC_TRACE 0x0D00
#define PPC_PERFORMANCE_MONITOR 0x0F00
#define PPC7xx_603E_INSTR_ADDR_BREAK 0x1300
#define PPC7xx_603E_SYS_MANAGE 0x1400
#define PPC_THERMAL_MANAGE 0x1700

#define MSR_FP 0x00002000
#define MSR_SE 0x00000400
#define MSR_DR 0x00000010

#define SPR_GQR0 912
#define SPR_HID2 920
#define SPR_FPECR 1022
#define HID2_LSQE 0x80000000
#define HID2_PSE 0x20000000

typedef struct TRKBuffer {
    u32 mutex;
    s32 isInUse;
    u32 length;
    u32 position;
    u8 data[DS_MAXMESSAGESIZE];
} TRKBuffer;

static inline s32 TRKAppendBuffer1_ui8(TRKBuffer *buffer, u8 data)
{
    if (buffer->position >= sizeof(buffer->data))
        return kMessageBufferOverflow;
    buffer->data[buffer->position++] = data;
    buffer->length++;
    return 0;
}

typedef struct TRKEvent {
    u8 eventType;
    s32 nextSequenceID;
    s32 msgBufID;
} TRKEvent;

typedef struct DSVersions {
    u8 kernelMajor;
    u8 kernelMinor;
    u8 protocolMajor;
    u8 protocolMinor;
} DSVersions;

typedef struct DSCPUType {
    u8 cpuMajor;
    u8 cpuMinor;
    u8 bigEndian;
    u8 defaultTypeSize;
    u8 floatTypeSize;
    u8 extended1TypeSize;
    u8 extended2TypeSize;
} DSCPUType;

extern s32 gTRKBigEndian;

s32 TRKInitializeMutex(void *mutex);
s32 TRKAcquireMutex(void *mutex);
s32 TRKReleaseMutex(void *mutex);
void *TRK_memcpy(void *dst, const void *src, u32 n);
void TRK_flush_cache(u32 address, u32 length);
void TRK_ppc_memcpy(void *destination, const void *source, u32 length, u32 destinationMSR, u32 sourceMSR);

s32 TRKInitializeMessageBuffers(void);
s32 TRKGetFreeBuffer(s32 *index, TRKBuffer **result);
TRKBuffer *TRKGetBuffer(s32 index);
void TRKReleaseBuffer(s32 index);
void TRKResetBuffer(TRKBuffer *buffer, u8 keepData);
s32 TRKSetBufferPosition(TRKBuffer *buffer, u32 position);
s32 TRKAppendBuffer(TRKBuffer *buffer, const void *data, u32 length);
s32 TRKReadBuffer(TRKBuffer *buffer, void *data, u32 length);
s32 TRKAppendBuffer1_ui16(TRKBuffer *buffer, const u16 data);
s32 TRKAppendBuffer1_ui32(TRKBuffer *buffer, const u32 data);
s32 TRKAppendBuffer1_ui64(TRKBuffer *buffer, const u64 data);
s32 TRKAppendBuffer_ui8(TRKBuffer *buffer, const u8 *data, s32 count);
s32 TRKAppendBuffer_ui32(TRKBuffer *buffer, const u32 *data, s32 count);
s32 TRKReadBuffer1_ui8(TRKBuffer *buffer, u8 *data);
s32 TRKReadBuffer1_ui16(TRKBuffer *buffer, u16 *data);
s32 TRKReadBuffer1_ui32(TRKBuffer *buffer, u32 *data);
s32 TRKReadBuffer1_ui64(TRKBuffer *buffer, u64 *data);
s32 TRKReadBuffer_ui8(TRKBuffer *buffer, u8 *data, s32 count);
s32 TRKReadBuffer_ui32(TRKBuffer *buffer, u32 *data, s32 count);

s32 TRKMessageSend(TRKBuffer *message);
s32 TRKStandardACK(TRKBuffer *buffer, u8 command, s32 reply);
void TRKMessageIntoReply(TRKBuffer *buffer, u8 command, s32 error);
s32 TRKDoNotifyStopped(u8 command);
s32 TRKRequestSend(TRKBuffer *buffer, s32 *response, u32 minimumLength, u32 retryCount, s32 timed);
DSError TRKSuppAccessFile(u32 file_handle, u8 *data, u32 *count, u8 *io_result, s32 need_reply, s32 read);
s32 TRKInitializeDispatcher(void);
s32 TRKDispatchMessage(TRKBuffer *buffer);

s32 TRKInitializeEventQueue(void);
void TRKConstructEvent(TRKEvent *event, s32 type);
void TRKDestructEvent(TRKEvent *event);
s32 TRKGetNextEvent(TRKEvent *event);
s32 TRKPostEvent(TRKEvent *event);

s32 TRKInitializeNub(void);
s32 TRKTerminateNub(void);
void TRKNubWelcome(void);
void TRKNubMainLoop(void);
void usr_put_initialize(void);
s32 TRKInitializeSerialHandler(void);
s32 TRKTerminateSerialHandler(void);
void TRKGetInput(void);
void TRKProcessInput(s32 index);
s32 TRKTestForPacket(void);

void __TRK_reset(void);
void __TRK_copy_vectors(void);
u32 TRKTargetTranslate(u32 address);
u8 TRKTargetCPUMinorType(void);
s32 TRKInitializeIntDrivenUART(u32 baudRate, s32 interruptDriven, s32 reserved, void **pending);
void EnableEXI2Interrupts(void);
s32 TRKPollUART(void);
s32 TRK_ReadUARTN(void *data, u32 length);
s32 TRK_WriteUARTN(const void *data, u32 length);
void ReserveEXI2Port(void);
void UnreserveEXI2Port(void);
void TRK_board_display(char *text);
void TRKEXICallBack(s32 interrupt, void *context);

s32 TRKInitializeTarget(void);
s32 TRKTargetContinue(void);
s32 TRKTargetStop(void);
s32 TRKTargetStopped(void);
void TRKTargetSetStopped(u32 stopped);
void TRKTargetSetInputPendingPtr(void *pointer);
u32 TRKTargetGetPC(void);
s32 TRKTargetVersions(DSVersions *versions);
s32 TRKTargetCPUType(DSCPUType *type);
s32 TRKTargetSupportMask(u8 mask[32]);
s32 TRKTargetSingleStep(u32 count, s32 stepOver);
s32 TRKTargetStepOutOfRange(u32 start, u32 end, s32 stepOver);
s32 TRKTargetAccessMemory(void *data, u32 address, u32 *length, s32 accessMode, s32 read);
s32 TRKTargetFlushCache(u8 mode, u32 start, u32 end);
s32 TRKTargetAccessDefault(u32 first, u32 last, TRKBuffer *buffer, u32 *storageSize, s32 read);
s32 TRKTargetAccessFP(u32 first, u32 last, TRKBuffer *buffer, u32 *storageSize, s32 read);
s32 TRKTargetAccessExtended1(u32 first, u32 last, TRKBuffer *buffer, u32 *storageSize, s32 read);
s32 TRKTargetAccessExtended2(u32 first, u32 last, TRKBuffer *buffer, u32 *storageSize, s32 read);
s32 TRKTargetAddStopInfo(TRKBuffer *buffer);
s32 TRKTargetAddExceptionInfo(TRKBuffer *buffer);
s32 TRKTargetSupportRequest(void);
s32 TRKTargetInterrupt(TRKEvent *event);
u32 __TRK_get_MSR(void);
void __TRK_set_MSR(u32 msr);
s32 TRKPPCAccessSPR(void *srcDestPtr, u32 spr, s32 read);
s32 TRKPPCAccessPairedSingleRegister(void *srcDestPtr, u32 psr, s32 read);
s32 TRKPPCAccessFPRegister(void *srcDestPtr, u32 fpr, s32 read);
s32 TRKPPCAccessSpecialReg(void *srcDestPtr, u32 *instructionData, s32 read);

#endif
