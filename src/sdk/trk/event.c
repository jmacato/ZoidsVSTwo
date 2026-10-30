#include "types.h"
#include "metrotrk.h"

#define TRK_EVENT_QUEUE_SIZE 2

typedef struct TRKEventQueue {
    u32 mutex;
    s32 count;
    s32 next;
    TRKEvent events[TRK_EVENT_QUEUE_SIZE];
    u32 eventID;
} TRKEventQueue;

extern TRKEventQueue gTRKEventQueue;

s32 TRKInitializeEventQueue(void)
{
    TRKInitializeMutex(&gTRKEventQueue);
    TRKAcquireMutex(&gTRKEventQueue);
    gTRKEventQueue.count = 0;
    gTRKEventQueue.next = 0;
    gTRKEventQueue.eventID = 0x100;
    TRKReleaseMutex(&gTRKEventQueue);
    return 0;
}

void TRKCopyEvent(TRKEvent *dst, const TRKEvent *src)
{
    TRK_memcpy(dst, src, sizeof(TRKEvent));
}

void TRKConstructEvent(TRKEvent *event, s32 type)
{
    event->eventType = type;
    event->nextSequenceID = 0;
    event->msgBufID = -1;
}

void TRKDestructEvent(TRKEvent *event)
{
    TRKReleaseBuffer(event->msgBufID);
}

s32 TRKGetNextEvent(TRKEvent *event)
{
    s32 found = 0;
    TRKAcquireMutex(&gTRKEventQueue);
    if (gTRKEventQueue.count > 0) {
        TRKCopyEvent(event, &gTRKEventQueue.events[gTRKEventQueue.next]);
        gTRKEventQueue.count--;
        gTRKEventQueue.next++;
        if (gTRKEventQueue.next == TRK_EVENT_QUEUE_SIZE)
            gTRKEventQueue.next = 0;
        found = 1;
    }
    TRKReleaseMutex(&gTRKEventQueue);
    return found;
}

s32 TRKPostEvent(TRKEvent *event)
{
    s32 error = 0;
    s32 index;
    TRKAcquireMutex(&gTRKEventQueue);
    if (gTRKEventQueue.count == TRK_EVENT_QUEUE_SIZE) {
        error = kEventQueueFull;
    } else {
        index = (gTRKEventQueue.next + gTRKEventQueue.count) % TRK_EVENT_QUEUE_SIZE;
        TRKCopyEvent(&gTRKEventQueue.events[index], event);
        gTRKEventQueue.events[index].nextSequenceID = gTRKEventQueue.eventID;
        gTRKEventQueue.eventID++;
        if (gTRKEventQueue.eventID < 0x100)
            gTRKEventQueue.eventID = 0x100;
        gTRKEventQueue.count++;
    }
    TRKReleaseMutex(&gTRKEventQueue);
    return error;
}
