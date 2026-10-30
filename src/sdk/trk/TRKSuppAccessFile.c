#include "types.h"
#include "metrotrk.h"

DSError TRKSuppAccessFile(u32 file_handle, u8 *data, u32 *count, u8 *io_result, s32 need_reply, s32 read)
{
    s32 replyBufferId;
    TRKBuffer *buffer;
    s32 bufferId;
    u16 replyLength;
    u8 replyIOResult;
    DSError error;
    u32 i;
    TRKBuffer *replyBuffer;
    u32 length;
    s32 exit;

    if (data == 0 || *count == 0)
        return kParameterError;
    exit = 0;
    *io_result = kDSIONoError;
    i = 0;
    error = 0;
    while (!exit && i < *count && error == 0 && *io_result == kDSIONoError) {
        if (*count - i > DS_MAXREADWRITELENGTH)
            length = DS_MAXREADWRITELENGTH;
        else
            length = *count - i;
        error = TRKGetFreeBuffer(&bufferId, &buffer);
        if (error == 0)
            error = TRKAppendBuffer1_ui8(buffer, read ? kDSReadFile : kDSWriteFile);
        if (error == 0)
            error = TRKAppendBuffer1_ui32(buffer, file_handle);
        if (error == 0)
            error = TRKAppendBuffer1_ui16(buffer, length);
        if (!read && error == 0)
            error = TRKAppendBuffer_ui8(buffer, data + i, length);
        if (error == 0) {
            if (need_reply) {
                replyLength = 0;
                replyIOResult = kDSIONoError;
                error = (0, TRKRequestSend(buffer, &replyBufferId, read ? 5 : 5, 3, !(read && file_handle == 0)));
                if (error == 0) {
                    replyBuffer = TRKGetBuffer(replyBufferId);
                    TRKSetBufferPosition(replyBuffer, 2);
                }
                if (error == 0)
                    error = TRKReadBuffer1_ui8(replyBuffer, &replyIOResult);
                if (error == 0)
                    error = TRKReadBuffer1_ui16(replyBuffer, &replyLength);
                if (read && error == 0) {
                    if (replyBuffer->length != replyLength + 5) {
                        replyLength = replyBuffer->length - 5;
                        if (replyIOResult == kDSIONoError)
                            replyIOResult = kDSIOError;
                    }
                    if (replyLength <= length)
                        error = TRKReadBuffer_ui8(replyBuffer, data + i, replyLength);
                }
                if (replyLength != length) {
                    if ((!read || replyLength >= length) && replyIOResult == kDSIONoError)
                        replyIOResult = kDSIOError;
                    length = replyLength;
                    exit = 1;
                }
                *io_result = replyIOResult;
                TRKReleaseBuffer(replyBufferId);
            } else {
                error = TRKMessageSend(buffer);
            }
        }
        TRKReleaseBuffer(bufferId);
        i += length;
    }
    *count = i;
    return error;
}

