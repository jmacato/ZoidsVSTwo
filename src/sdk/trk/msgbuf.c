#include "types.h"
#include "metrotrk.h"

extern TRKBuffer gTRKMsgBufs[NUM_BUFFERS];

void TRKSetBufferUsed(TRKBuffer *buffer, s32 used)
{
    buffer->isInUse = used;
}

TRKBuffer *TRKGetBuffer(s32 index)
{
    TRKBuffer *buffer = 0;
    if (index >= 0 && index < NUM_BUFFERS)
        buffer = &gTRKMsgBufs[index];
    return buffer;
}

void TRKResetBuffer(TRKBuffer *buffer, u8 keepData)
{
    buffer->length = 0;
    buffer->position = 0;
    if (!keepData)
        TRK_memcpy(buffer->data, 0, sizeof(buffer->data));
}

s32 TRKSetBufferPosition(TRKBuffer *buffer, u32 position)
{
    s32 error = 0;
    if (position > sizeof(buffer->data)) {
        error = kMessageBufferOverflow;
    } else {
        buffer->position = position;
        if (position > buffer->length)
            buffer->length = position;
    }
    return error;
}

s32 TRKAppendBuffer1_ui16(TRKBuffer *buffer, const u16 data)
{
    u8 *bigEndianData;
    u8 *byteData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian) {
        bigEndianData = (u8 *)&data;
    } else {
        byteData = (u8 *)&data;
        bigEndianData = swapBuffer;
        bigEndianData[0] = byteData[1];
        bigEndianData[1] = byteData[0];
    }
    return TRKAppendBuffer(buffer, bigEndianData, sizeof(data));
}

s32 TRKAppendBuffer1_ui32(TRKBuffer *buffer, const u32 data)
{
    u8 *bigEndianData;
    u8 *byteData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian) {
        bigEndianData = (u8 *)&data;
    } else {
        byteData = (u8 *)&data;
        bigEndianData = swapBuffer;
        bigEndianData[0] = byteData[3];
        bigEndianData[1] = byteData[2];
        bigEndianData[2] = byteData[1];
        bigEndianData[3] = byteData[0];
    }
    return TRKAppendBuffer(buffer, bigEndianData, sizeof(data));
}

s32 TRKReadBuffer1_ui8(TRKBuffer *buffer, u8 *data)
{
    return TRKReadBuffer(buffer, data, 1);
}

s32 TRKInitializeMessageBuffers(void)
{
    s32 index;
    for (index = 0; index < NUM_BUFFERS; index++) {
        TRKInitializeMutex(&gTRKMsgBufs[index].mutex);
        TRKAcquireMutex(&gTRKMsgBufs[index].mutex);
        TRKSetBufferUsed(&gTRKMsgBufs[index], 0);
        TRKReleaseMutex(&gTRKMsgBufs[index].mutex);
    }
    return 0;
}

s32 TRKGetFreeBuffer(s32 *index, TRKBuffer **result)
{
    s32 error = kNoMessageBufferAvailable;
    s32 i;
    TRKBuffer *buffer;
    *result = 0;
    for (i = 0; i < NUM_BUFFERS; i++) {
        buffer = TRKGetBuffer(i);
        TRKAcquireMutex(&buffer->mutex);
        if (!buffer->isInUse) {
            TRKResetBuffer(buffer, 1);
            TRKSetBufferUsed(buffer, 1);
            *result = buffer;
            *index = i;
            error = 0;
            i = NUM_BUFFERS;
        }
        TRKReleaseMutex(&buffer->mutex);
    }
    return error;
}

void TRKReleaseBuffer(s32 index)
{
    TRKBuffer *buffer;
    if (index != -1 && index >= 0 && index < NUM_BUFFERS) {
        buffer = &gTRKMsgBufs[index];
        TRKAcquireMutex(&buffer->mutex);
        TRKSetBufferUsed(buffer, 0);
        TRKReleaseMutex(&buffer->mutex);
    }
}

s32 TRKAppendBuffer(TRKBuffer *buffer, const void *data, u32 length)
{
    s32 error = 0;
    u32 available;
    if (length == 0)
        return 0;
    available = sizeof(buffer->data) - buffer->position;
    if (available < length) {
        error = kMessageBufferOverflow;
        length = available;
    }
    if (length == 1)
        buffer->data[buffer->position] = *(const u8 *)data;
    else
        TRK_memcpy(&buffer->data[buffer->position], data, length);
    buffer->position += length;
    buffer->length = buffer->position;
    return error;
}

s32 TRKReadBuffer(TRKBuffer *buffer, void *data, u32 length)
{
    s32 error = 0;
    u32 available;
    if (length == 0)
        return 0;
    available = buffer->length - buffer->position;
    if (length > available) {
        error = kMessageBufferReadError;
        length = available;
    }
    TRK_memcpy(data, &buffer->data[buffer->position], length);
    buffer->position += length;
    return error;
}

s32 TRKAppendBuffer_ui8(TRKBuffer *buffer, const u8 *data, s32 count)
{
    s32 index = 0;
    s32 error = 0;
    for (; error == 0 && index < count; index++)
        error = TRKAppendBuffer1_ui8(buffer, data[index]);
    return error;
}

s32 TRKAppendBuffer_ui32(TRKBuffer *buffer, const u32 *data, s32 count)
{
    s32 index;
    s32 error = 0;
    for (index = 0; error == 0 && index < count; index++)
        error = TRKAppendBuffer1_ui32(buffer, data[index]);
    return error;
}

s32 TRKReadBuffer_ui8(TRKBuffer *buffer, u8 *data, s32 count)
{
    s32 index;
    s32 error = 0;
    for (index = 0; error == 0 && index < count; index++)
        error = TRKReadBuffer1_ui8(buffer, &data[index]);
    return error;
}

s32 TRKReadBuffer1_ui16(TRKBuffer *buffer, u16 *data)
{
    s32 error;
    u8 *bigEndianData;
    u8 *byteData;
    u8 swapBuffer[sizeof(*data)];
    if (gTRKBigEndian)
        bigEndianData = (u8 *)data;
    else
        bigEndianData = swapBuffer;
    error = TRKReadBuffer(buffer, bigEndianData, sizeof(*data));
    if (!gTRKBigEndian && error == 0) {
        byteData = (u8 *)data;
        byteData[0] = bigEndianData[1];
        byteData[1] = bigEndianData[0];
    }
    return error;
}

s32 TRKReadBuffer1_ui32(TRKBuffer *buffer, u32 *data)
{
    s32 error;
    u8 *bigEndianData;
    u8 *byteData;
    u8 swapBuffer[sizeof(*data)];
    if (gTRKBigEndian)
        bigEndianData = (u8 *)data;
    else
        bigEndianData = swapBuffer;
    error = TRKReadBuffer(buffer, bigEndianData, sizeof(*data));
    if (!gTRKBigEndian && error == 0) {
        byteData = (u8 *)data;
        byteData[0] = bigEndianData[3];
        byteData[1] = bigEndianData[2];
        byteData[2] = bigEndianData[1];
        byteData[3] = bigEndianData[0];
    }
    return error;
}

s32 TRKReadBuffer1_ui64(TRKBuffer *buffer, u64 *data)
{
    s32 error;
    u8 *bigEndianData;
    u8 *byteData;
    u8 swapBuffer[sizeof(*data)];
    if (gTRKBigEndian)
        bigEndianData = (u8 *)data;
    else
        bigEndianData = swapBuffer;
    error = TRKReadBuffer(buffer, bigEndianData, sizeof(*data));
    if (!gTRKBigEndian && error == 0) {
        byteData = (u8 *)data;
        byteData[0] = bigEndianData[7];
        byteData[1] = bigEndianData[6];
        byteData[2] = bigEndianData[5];
        byteData[3] = bigEndianData[4];
        byteData[4] = bigEndianData[3];
        byteData[5] = bigEndianData[2];
        byteData[6] = bigEndianData[1];
        byteData[7] = bigEndianData[0];
    }
    return error;
}

s32 TRKReadBuffer_ui32(TRKBuffer *buffer, u32 *data, s32 count)
{
    s32 index;
    s32 error = 0;
    for (index = 0; error == 0 && index < count; index++)
        error = TRKReadBuffer1_ui32(buffer, &data[index]);
    return error;
}

s32 TRKAppendBuffer1_ui64(TRKBuffer *buffer, const u64 data)
{
    u8 *bigEndianData;
    u8 *byteData;
    u8 swapBuffer[sizeof(data)];
    if (gTRKBigEndian) {
        bigEndianData = (u8 *)&data;
    } else {
        byteData = (u8 *)&data;
        bigEndianData = swapBuffer;
        bigEndianData[0] = byteData[7];
        bigEndianData[1] = byteData[6];
        bigEndianData[2] = byteData[5];
        bigEndianData[3] = byteData[4];
        bigEndianData[4] = byteData[3];
        bigEndianData[5] = byteData[2];
        bigEndianData[6] = byteData[1];
        bigEndianData[7] = byteData[0];
    }
    return TRKAppendBuffer(buffer, bigEndianData, sizeof(data));
}
