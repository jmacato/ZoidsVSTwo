#include "types.h"
#include "metrotrk.h"

static s32 TRKSendACK(void *buffer)
{
    s32 error;
    s32 tries = 3;
    do {
        error = TRKMessageSend(buffer);
        --tries;
    } while (error != 0 && tries > 0);
    return error;
}

s32 TRKStandardACK(TRKBuffer *buffer, u8 command, s32 reply)
{
    TRKMessageIntoReply(buffer, command, reply);
    return TRKSendACK(buffer);
}

s32 TRKDoUnsupported(void *buffer)
{
    return TRKStandardACK(buffer, kDSReplyACK, kDSReplyUnsupportedCommandError);
}

s32 TRKDoConnect(void *buffer)
{
    return TRKStandardACK(buffer, kDSReplyACK, kDSReplyNoError);
}

s32 TRKDoDisconnect(void *buffer)
{
    s32 error = 0;
    TRKEvent event;
    if ((error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyNoError)) == 0) {
        TRKConstructEvent(&event, kShutdownEvent);
        TRKPostEvent(&event);
    }
    return error;
}

s32 TRKDoReset(void *buffer)
{
    TRKStandardACK(buffer, kDSReplyACK, kDSReplyNoError);
    __TRK_reset();
    return 0;
}

s32 TRKDoContinue(void *buffer)
{
    s32 error;
    if (TRKTargetStopped() == 0) {
        error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyNotStopped);
    } else {
        error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyNoError);
        if (error == 0)
            error = TRKTargetContinue();
    }
    return error;
}

s32 TRKDoStop(void *buffer)
{
    s32 error;
    if (TRKTargetStop() == 0)
        error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyNoError);
    else
        error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyError);
    return error;
}

void TRKMessageIntoReply(TRKBuffer *message, u8 command, s32 error)
{
    TRKBuffer *buffer = message;
    TRKResetBuffer(buffer, 1);
    TRKAppendBuffer1_ui8(buffer, command);
    TRKAppendBuffer1_ui8(buffer, error);
}

s32 TRKDoVersions(TRKBuffer *buffer)
{
    DSVersions versions;
    s32 error;
    if (buffer->length != DS_MIN_MSG_LENGTH) {
        error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyPacketSizeError);
    } else {
        TRKMessageIntoReply(buffer, kDSReplyACK, kDSReplyNoError);
        error = TRKTargetVersions(&versions);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, versions.kernelMajor);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, versions.kernelMinor);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, versions.protocolMajor);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, versions.protocolMinor);
        if (error != 0)
            error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyCWDSError);
        else
            error = TRKSendACK(buffer);
    }
    return error;
}

s32 TRKDoSupportMask(TRKBuffer *buffer)
{
    u8 mask[32];
    s32 error;
    if (buffer->length != DS_MIN_MSG_LENGTH) {
        error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyPacketSizeError);
    } else {
        TRKMessageIntoReply(buffer, kDSReplyACK, kDSReplyNoError);
        error = TRKTargetSupportMask(mask);
        if (error == 0)
            error = TRKAppendBuffer(buffer, mask, sizeof(mask));
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, 2);
        if (error != 0)
            error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyCWDSError);
        else
            error = TRKSendACK(buffer);
    }
    return error;
}

s32 TRKDoCPUType(TRKBuffer *buffer)
{
    DSCPUType type;
    s32 error;
    if (buffer->length != DS_MIN_MSG_LENGTH) {
        error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyPacketSizeError);
    } else {
        TRKMessageIntoReply(buffer, kDSReplyACK, kDSReplyNoError);
        error = TRKTargetCPUType(&type);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, type.cpuMajor);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, type.cpuMinor);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, type.bigEndian);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, type.defaultTypeSize);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, type.floatTypeSize);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, type.extended1TypeSize);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, type.extended2TypeSize);
        if (error != 0)
            error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyCWDSError);
        else
            error = TRKSendACK(buffer);
    }
    return error;
}

s32 TRKDoReadMemory(TRKBuffer *buffer)
{
    u8 data[DS_MAXREADWRITELENGTH];
    u32 actual;
    u32 address;
    u16 length;
    u8 options;
    u8 command;
    s32 error;
    s32 reply;
    if (buffer->length != DS_MIN_MSG_READMEMORY_LENGTH)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyPacketSizeError);
    TRKSetBufferPosition(buffer, 0);
    error = TRKReadBuffer1_ui8(buffer, &command);
    if (error == 0)
        error = TRKReadBuffer1_ui8(buffer, &options);
    if (error == 0)
        error = TRKReadBuffer1_ui16(buffer, &length);
    if (error == 0)
        error = TRKReadBuffer1_ui32(buffer, &address);
    if (options & DS_MSG_MEMORY_EXTENDED)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyUnsupportedOptionError);
    if (length > sizeof(data))
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyParameterError);
    TRKMessageIntoReply(buffer, kDSReplyACK, kDSReplyNoError);
    if (error == 0) {
        actual = length;
        error = TRKTargetAccessMemory(data, address, &actual, (options & DS_MSG_MEMORY_USERVIEW) ? kUserMemory : kDebuggerMemory, 1);
        length = actual;
        if (error == 0)
            error = TRKAppendBuffer1_ui16(buffer, length);
        if (error == 0)
            error = TRKAppendBuffer(buffer, data, actual);
    }
    if (error != 0) {
        switch (error) {
        case kCWDSException:
            reply = kDSReplyCWDSException;
            break;
        case kInvalidMemory:
            reply = kDSReplyInvalidMemoryRange;
            break;
        default:
            reply = kDSReplyCWDSError;
            break;
        }
        return TRKStandardACK(buffer, kDSReplyACK, reply);
    }
    return TRKSendACK(buffer);
}

s32 TRKDoWriteMemory(TRKBuffer *buffer)
{
    u8 data[DS_MAXREADWRITELENGTH];
    u32 actual;
    u32 address;
    u16 length;
    u8 options;
    u8 command;
    s32 error;
    s32 reply;
    if (buffer->length <= DS_MIN_MSG_WRITEMEMORY_LENGTH)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyPacketSizeError);
    TRKSetBufferPosition(buffer, 0);
    error = TRKReadBuffer1_ui8(buffer, &command);
    if (error == 0)
        error = TRKReadBuffer1_ui8(buffer, &options);
    if (error == 0)
        error = TRKReadBuffer1_ui16(buffer, &length);
    if (error == 0)
        error = TRKReadBuffer1_ui32(buffer, &address);
    if (options & DS_MSG_MEMORY_EXTENDED)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyUnsupportedOptionError);
    if (buffer->length != length + DS_MIN_MSG_WRITEMEMORY_LENGTH || length > sizeof(data))
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyParameterError);
    if (error == 0) {
        actual = length;
        error = TRKReadBuffer(buffer, data, actual);
        if (error == 0)
            error = TRKTargetAccessMemory(data, address, &actual, (options & DS_MSG_MEMORY_USERVIEW) ? kUserMemory : kDebuggerMemory, 0);
        length = actual;
    }
    if (error == 0)
        TRKMessageIntoReply(buffer, kDSReplyACK, kDSReplyNoError);
    if (error == 0)
        error = TRKAppendBuffer1_ui16(buffer, length);
    if (error != 0) {
        switch (error) {
        case kCWDSException:
            reply = kDSReplyCWDSException;
            break;
        case kInvalidMemory:
            reply = kDSReplyInvalidMemoryRange;
            break;
        default:
            reply = kDSReplyCWDSError;
            break;
        }
        return TRKStandardACK(buffer, kDSReplyACK, reply);
    }
    return TRKSendACK(buffer);
}

s32 TRKDoFlushCache(TRKBuffer *buffer)
{
    u32 end;
    u32 start;
    u8 options;
    u8 command;
    s32 error;
    s32 reply;
    if (buffer->length != DS_MIN_MSG_FLUSHCACHE_LENGTH)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyPacketSizeError);
    TRKSetBufferPosition(buffer, 0);
    error = TRKReadBuffer1_ui8(buffer, &command);
    if (error == 0)
        error = TRKReadBuffer1_ui8(buffer, &options);
    if (error == 0)
        error = TRKReadBuffer1_ui32(buffer, &start);
    if (error == 0)
        error = TRKReadBuffer1_ui32(buffer, &end);
    if (!TRKTargetStopped())
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyNotStopped);
    if (start > end)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyInvalidMemoryRange);
    if (error == 0)
        error = TRKTargetFlushCache(options, start, end);
    if (error == 0)
        TRKMessageIntoReply(buffer, kDSReplyACK, kDSReplyNoError);
    if (error != 0) {
        switch (error) {
        case kUnsupportedError:
            reply = kDSReplyUnsupportedOptionError;
            break;
        default:
            reply = kDSReplyCWDSError;
            break;
        }
        return TRKStandardACK(buffer, kDSReplyACK, reply);
    }
    return TRKSendACK(buffer);
}

s32 TRKDoReadRegisters(TRKBuffer *buffer)
{
    u32 storage;
    u16 last;
    u16 first;
    u8 options;
    u8 command;
    s32 error;
    s32 reply;
    if (buffer->length != DS_MIN_MSG_READREGISTERS_LENGTH)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyPacketSizeError);
    TRKSetBufferPosition(buffer, 0);
    error = TRKReadBuffer1_ui8(buffer, &command);
    if (error == 0)
        error = TRKReadBuffer1_ui8(buffer, &options);
    if (error == 0)
        error = TRKReadBuffer1_ui16(buffer, &first);
    if (error == 0)
        error = TRKReadBuffer1_ui16(buffer, &last);
    if (!TRKTargetStopped())
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyNotStopped);
    if (first > last)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyInvalidRegisterRange);
    if (error == 0)
        TRKMessageIntoReply(buffer, kDSReplyACK, kDSReplyNoError);
    switch (options & 7) {
    case kDSRegistersDefault:
        error = TRKTargetAccessDefault(first, last, buffer, &storage, 1);
        break;
    case kDSRegistersFP:
        error = TRKTargetAccessFP(first, last, buffer, &storage, 1);
        break;
    case kDSRegistersExtended1:
        error = TRKTargetAccessExtended1(first, last, buffer, &storage, 1);
        break;
    case kDSRegistersExtended2:
        error = TRKTargetAccessExtended2(first, last, buffer, &storage, 1);
        break;
    default:
        error = kUnsupportedError;
        break;
    }
    if (error != 0) {
        switch (error) {
        case kUnsupportedError:
            reply = kDSReplyUnsupportedOptionError;
            break;
        case kInvalidRegister:
            reply = kDSReplyInvalidRegisterRange;
            break;
        case kCWDSException:
            reply = kDSReplyCWDSException;
            break;
        default:
            reply = kDSReplyCWDSError;
            break;
        }
        return TRKStandardACK(buffer, kDSReplyACK, reply);
    }
    return TRKSendACK(buffer);
}

s32 TRKDoWriteRegisters(TRKBuffer *buffer)
{
    u32 storage;
    u16 last;
    u16 first;
    u8 options;
    u8 command;
    s32 error;
    s32 reply;
    if (buffer->length <= DS_MIN_MSG_WRITEREGISTERS_LENGTH)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyPacketSizeError);
    TRKSetBufferPosition(buffer, 0);
    error = TRKReadBuffer1_ui8(buffer, &command);
    if (error == 0)
        error = TRKReadBuffer1_ui8(buffer, &options);
    if (error == 0)
        error = TRKReadBuffer1_ui16(buffer, &first);
    if (error == 0)
        error = TRKReadBuffer1_ui16(buffer, &last);
    if (!TRKTargetStopped())
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyNotStopped);
    if (first > last)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyInvalidRegisterRange);
    switch (options) {
    case kDSRegistersDefault:
        error = TRKTargetAccessDefault(first, last, buffer, &storage, 0);
        break;
    case kDSRegistersFP:
        error = TRKTargetAccessFP(first, last, buffer, &storage, 0);
        break;
    case kDSRegistersExtended1:
        error = TRKTargetAccessExtended1(first, last, buffer, &storage, 0);
        break;
    case kDSRegistersExtended2:
        error = TRKTargetAccessExtended2(first, last, buffer, &storage, 0);
        break;
    default:
        error = kUnsupportedError;
        break;
    }
    if (error == 0)
        TRKMessageIntoReply(buffer, kDSReplyACK, kDSReplyNoError);
    if (error != 0) {
        switch (error) {
        case kUnsupportedError:
            reply = kDSReplyUnsupportedOptionError;
            break;
        case kInvalidRegister:
            reply = kDSReplyInvalidRegisterRange;
            break;
        case kMessageBufferReadError:
            reply = kDSReplyPacketSizeError;
            break;
        case kCWDSException:
            reply = kDSReplyCWDSException;
            break;
        default:
            reply = kDSReplyCWDSError;
            break;
        }
        return TRKStandardACK(buffer, kDSReplyACK, reply);
    }
    return TRKSendACK(buffer);
}

s32 TRKDoStep(TRKBuffer *buffer)
{
    u32 end;
    u32 start;
    u8 count;
    u8 options;
    u8 command;
    s32 error;
    u32 pc;
    if (buffer->length < DS_MIN_MSG_STEP_COUNT_LENGTH)
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyPacketSizeError);
    TRKSetBufferPosition(buffer, 0);
    error = TRKReadBuffer1_ui8(buffer, &command);
    if (error == 0)
        error = TRKReadBuffer1_ui8(buffer, &options);
    if (!TRKTargetStopped())
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyNotStopped);
    switch (options) {
    case kDSStepIntoCount:
    case kDSStepOverCount:
        if (error == 0)
            error = TRKReadBuffer1_ui8(buffer, &count);
        if (count < 1)
            return TRKStandardACK(buffer, kDSReplyACK, kDSReplyParameterError);
        break;
    case kDSStepIntoRange:
    case kDSStepOverRange:
        if (buffer->length != DS_MIN_MSG_STEP_RANGE_LENGTH)
            return TRKStandardACK(buffer, kDSReplyACK, kDSReplyPacketSizeError);
        if (error == 0)
            error = TRKReadBuffer1_ui32(buffer, &start);
        if (error == 0)
            error = TRKReadBuffer1_ui32(buffer, &end);
        pc = TRKTargetGetPC();
        if (pc < start || pc > end)
            return TRKStandardACK(buffer, kDSReplyACK, kDSReplyParameterError);
        break;
    default:
        return TRKStandardACK(buffer, kDSReplyACK, kDSReplyUnsupportedOptionError);
    }
    error = TRKStandardACK(buffer, kDSReplyACK, kDSReplyNoError);
    if (error == 0) {
        switch (options) {
        case kDSStepIntoCount:
        case kDSStepOverCount:
            error = TRKTargetSingleStep(count, options == kDSStepOverCount);
            break;
        case kDSStepIntoRange:
        case kDSStepOverRange:
            error = TRKTargetStepOutOfRange(start, end, options == kDSStepOverRange);
            break;
        }
    }
    return error;
}
