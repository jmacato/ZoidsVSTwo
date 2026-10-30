#include "types.h"
#include "metrotrk.h"

typedef struct FramingState {
    s32 msgBufID;
    void *buffer;
    u8 receiveState;
    s32 isEscape;
    u8 fcsType;
} FramingState;

extern FramingState gTRKFramingState;

s32 TRKInitializeSerialHandler(void)
{
    gTRKFramingState.msgBufID = -1;
    gTRKFramingState.receiveState = 0;
    gTRKFramingState.isEscape = 0;
    return 0;
}

s32 TRKTerminateSerialHandler(void)
{
    return 0;
}

void TRKProcessInput(s32 index)
{
    TRKEvent event;
    TRKConstructEvent(&event, kRequestEvent);
    event.msgBufID = index;
    gTRKFramingState.msgBufID = -1;
    TRKPostEvent(&event);
}

void TRKGetInput(void)
{
    void *buffer;
    s32 index;
    u8 command;
    index = TRKTestForPacket();
    if (index != -1) {
        buffer = TRKGetBuffer(index);
        TRKSetBufferPosition(buffer, 0);
        TRKReadBuffer1_ui8(buffer, &command);
        if (command < kDSReplyACK)
            TRKProcessInput(index);
        else
            TRKReleaseBuffer(index);
    }
}

s32 TRKTestForPacket(void)
{
    TRKBuffer *buffer;
    /* The original no-data path does not initialize the buffer ID. */
    s32 index;
    s32 blockLength;
    s32 length;
    length = TRKPollUART();
    if (length > 0) {
        TRKGetFreeBuffer(&index, &buffer);
        if (length > DS_MAXMESSAGESIZE) {
            while (length > 0) {
                if (length > DS_MAXMESSAGESIZE)
                    blockLength = DS_MAXMESSAGESIZE;
                else
                    blockLength = length;
                TRK_ReadUARTN(buffer->data, blockLength);
                length -= blockLength;
            }
            TRKStandardACK(buffer, kDSReplyNAK, kDSReplyOverflow);
        } else {
            if (TRK_ReadUARTN(buffer->data, length) == 0) {
                buffer->length = length;
                return index;
            }
        }
    }
    if (index != -1)
        TRKReleaseBuffer(index);
    return -1;
}
