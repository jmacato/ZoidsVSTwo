#include "types.h"
#include "metrotrk.h"

s32 TRKDoNotifyStopped(u8 command)
{
    s32 response;
    TRKBuffer *buffer;
    s32 index;
    s32 error;
    error = TRKGetFreeBuffer(&index, &buffer);
    if (error == 0) {
        if (TRKAppendBuffer1_ui8(buffer, command) == 0) {
            if (command == kDSNotifyStopped)
                TRKTargetAddStopInfo(buffer);
            else
                TRKTargetAddExceptionInfo(buffer);
        }
        error = TRKRequestSend(buffer, &response, DS_MIN_REPLY_LENGTH, 3, 1);
        if (error == 0)
            TRKReleaseBuffer(response);
        TRKReleaseBuffer(index);
    }
    return error;
}

s32 TRKRequestSend(TRKBuffer *buffer, s32 *response, u32 minimumLength, u32 retryCount, s32 timed)
{
    u8 status;
    u8 command;
    s32 error = 0;
    TRKBuffer *reply;
    u32 polls;
    s32 attempts = retryCount + 1;
    s32 retry = 1;
    *response = -1;
    while (attempts != 0 && *response == -1 && error == 0) {
        error = TRKMessageSend(buffer);
        if (error == 0) {
            if (timed)
                polls = 0;
        poll_for_reply:
            do {
                *response = TRKTestForPacket();
            } while (*response == -1 && (!timed || ++polls < 79999980u));
            if (*response != -1) {
                retry = 0;
                reply = TRKGetBuffer(*response);
                TRKSetBufferPosition(reply, 0);
                error = TRKReadBuffer1_ui8(reply, &command);
                if (error == 0 && command < kDSReplyACK) {
                    TRKProcessInput(*response);
                    *response = -1;
                    goto poll_for_reply;
                }
            }
            if (*response != -1) {
                if (reply->length < minimumLength)
                    retry = 1;
                if (error == 0 && retry == 0)
                    error = TRKReadBuffer1_ui8(reply, &status);
                if (error == 0 && retry == 0 && (command != kDSReplyACK || status != kDSReplyNoError))
                    retry = 1;
                if (error != 0 || retry != 0) {
                    TRKReleaseBuffer(*response);
                    *response = -1;
                }
            }
        }
        attempts--;
    }
    if (*response == -1)
        error = kWaitACKError;
    return error;
}
