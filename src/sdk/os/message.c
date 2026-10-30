/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/os/OSMessage.c. */
#include "os_message.h"
#include "os_interrupt.h"
#define NULL 0
extern void OSInitThreadQueue(OSThreadQueue *);
extern void OSSleepThread(OSThreadQueue *);
extern void OSWakeupThread(OSThreadQueue *);

void OSInitMessageQueue(struct OSMessageQueue *mq, void *msgArray, long msgCount)
{
    OSInitThreadQueue(&mq->queueSend);
    OSInitThreadQueue(&mq->queueReceive);
    mq->msgArray = msgArray;
    mq->msgCount = msgCount;
    mq->firstIndex = 0;
    mq->usedCount = 0;
}

int OSSendMessage(struct OSMessageQueue *mq, void *msg, long flags)
{
    int enabled;
    long lastIndex;

    enabled = OSDisableInterrupts();
    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & 1)) {
            OSRestoreInterrupts(enabled);
            return 0;
        }
        OSSleepThread(&mq->queueSend);
    }
    lastIndex = (mq->firstIndex + mq->usedCount) % mq->msgCount;
    ((u32 *)mq->msgArray)[lastIndex] = (u32)msg;
    mq->usedCount++;
    OSWakeupThread(&mq->queueReceive);
    OSRestoreInterrupts(enabled);
    return 1;
}

int OSReceiveMessage(struct OSMessageQueue *mq, void *msg, long flags)
{
    int enabled = OSDisableInterrupts();

    while (mq->usedCount == 0) {
        if (!(flags & 1)) {
            OSRestoreInterrupts(enabled);
            return 0;
        }
        OSSleepThread(&mq->queueReceive);
    }
    if (msg != NULL) {
        *(u32 *)msg = ((u32 *)mq->msgArray)[mq->firstIndex];
    }

    mq->firstIndex = (mq->firstIndex + 1) % mq->msgCount;
    mq->usedCount--;
    OSWakeupThread(&mq->queueSend);
    OSRestoreInterrupts(enabled);
    return 1;
}
