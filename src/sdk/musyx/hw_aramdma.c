#include "ar.h"
#include "musyx_hardware.h"

typedef struct ARAMTransfer {
    ARQRequest request;
    void (*callback)(u32);
    u32 user;
} ARAMTransfer;
typedef struct ARAMTransferQueue {
    ARAMTransfer queue[16];
    volatile u8 write;
    volatile u8 valid;
} ARAMTransferQueue;


typedef struct STREAM_BUFFER {
    struct STREAM_BUFFER *next;
    u32 aramAddr;
    u32 length;
    u32 allocLength;
} STREAM_BUFFER;

static ARAMTransferQueue aramQueueLo;
static ARAMTransferQueue aramQueueHi;
static STREAM_BUFFER aramStreamBuffers[64];
static u32 aramTop;
static u32 aramWrite;
static u32 aramStream;
static void *(*aramUploadCallback)(u32 source, u32 length);
static u32 aramUploadChunkSize;
static STREAM_BUFFER *aramUsedStreamBuffers;
static STREAM_BUFFER *aramFreeStreamBuffers;
static STREAM_BUFFER *aramIdleStreamBuffers;

void aramSyncTransferQueue(void)
{
    while (aramQueueLo.valid != 0) {
    }
}



u32 aramGetZeroBuffer(void)
{
    return ARGetBaseAddress();
}

void aramRemoveData(void *address, u32 length)
{
    aramWrite -= (length + 31) & ~31;
}

u32 aramGetStreamBufferAddress(u8 id, u32 *length)
{
    if (length != 0)
        *length = aramStreamBuffers[id].length;
    return aramStreamBuffers[id].aramAddr;
}

extern void DCFlushRange(void *address, u32 length);
extern void aramUploadData(u32 source, u32 destination, u32 length, u32 highPriority,
                           void (*callback)(u32), u32 user);
u32 aramStoreData(u32 source, u32 length)
{
    u32 destination;
    u32 chunk;
    void *data;
    length = (length + 31) & ~31;
    destination = aramWrite;
    if (!aramUploadCallback) {
        DCFlushRange((void *)source, length);
        aramUploadData(source, aramWrite, length, 0, 0, 0);
        aramWrite += length;
        return destination;
    }
    while (length) {
        chunk = length >= aramUploadChunkSize ? aramUploadChunkSize : length;
        data = aramUploadCallback(source, chunk);
        DCFlushRange(data, chunk);
        aramUploadData((u32)data, aramWrite, chunk, 0, 0, 0);
        length -= chunk;
        source += chunk;
        aramWrite += chunk;
    }
    return destination;
}

extern s32 OSDisableInterrupts(void);
extern s32 OSRestoreInterrupts(s32 state);
static void aramQueueCallback(u32 request)
{
    ARAMTransferQueue *queue;
    u32 index;
    queue = ((ARQRequest *)request)->priority == 1 ? &aramQueueHi : &aramQueueLo;
    for (index = 0; index < 16; index++) {
        if (request == (u32)&queue->queue[index].request) {
            if (queue->queue[index].callback)
                queue->queue[index].callback(queue->queue[index].user);
        }
    }
    queue->valid--;
}
void aramUploadData(u32 source, u32 destination, u32 length, u32 highPriority,
                    void (*callback)(u32), u32 user)
{
    ARAMTransferQueue *queue;
    s32 state;
    queue = highPriority ? &aramQueueHi : &aramQueueLo;
    for (;;) {
        state = OSDisableInterrupts();
        if (queue->valid < 16) {
            queue->queue[queue->write].request.owner = 42;
            queue->queue[queue->write].request.type = 0;
            queue->queue[queue->write].request.priority = highPriority ? 1 : 0;
            queue->queue[queue->write].request.source = source;
            queue->queue[queue->write].request.dest = destination;
            queue->queue[queue->write].request.length = length;
            queue->queue[queue->write].request.callback = aramQueueCallback;
            queue->queue[queue->write].callback = callback;
            queue->queue[queue->write].user = user;
            ARQPostRequest(&queue->queue[queue->write].request,
                queue->queue[queue->write].request.owner,
                queue->queue[queue->write].request.type,
                queue->queue[queue->write].request.priority,
                queue->queue[queue->write].request.source,
                queue->queue[queue->write].request.dest,
                queue->queue[queue->write].request.length,
                queue->queue[queue->write].request.callback);
            queue->valid++;
            queue->write = (queue->write + 1) % 16;
            OSRestoreInterrupts(state);
            break;
        }
        OSRestoreInterrupts(state);
    }
}

static void InitStreamBuffers(void);
void aramInit(u32 length)
{
    u16 *zero;
    u32 base;
    s32 index;
    base = ARGetBaseAddress();
    zero = salMalloc(0x500);
    for (index = 0; index < 0x280; index++)
        zero[index] = 0;
    DCFlushRange(zero, 0x500);
    aramQueueLo.valid = 0;
    aramQueueLo.write = 0;
    aramQueueHi.valid = 0;
    aramQueueHi.write = 0;
    aramUploadData((u32)zero, base, 0x500, 0, 0, 0);
    while (aramQueueLo.valid) {
    }
    salFree(zero);
    aramTop = base + length;
    if (aramTop > ARGetSize())
        aramTop = ARGetSize();
    aramWrite = base + 0x500;
    aramUploadCallback = 0;
    InitStreamBuffers();
}
static void InitStreamBuffers(void)
{
    u32 index;
    aramUsedStreamBuffers = 0;
    aramFreeStreamBuffers = 0;
    aramIdleStreamBuffers = aramStreamBuffers;
    for (index = 1; index < 64; index++)
        aramStreamBuffers[index - 1].next = &aramStreamBuffers[index];
    aramStreamBuffers[index - 1].next = 0;
    aramStream = aramTop;
}
