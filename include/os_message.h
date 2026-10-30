/* Types follow
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/os/OSMessage.h. */
#ifndef ZOIDS_OS_MESSAGE_H
#define ZOIDS_OS_MESSAGE_H
#include "os_context.h"
typedef struct OSMessageQueue {
    OSThreadQueue queueSend;
    OSThreadQueue queueReceive;
    void *msgArray;
    long msgCount;
    long firstIndex;
    long usedCount;
} OSMessageQueue;
extern void OSInitMessageQueue(OSMessageQueue *queue, void *messages, long capacity);
extern int OSSendMessage(OSMessageQueue *queue, void *message, long flags);
extern int OSReceiveMessage(OSMessageQueue *queue, void *message, long flags);
#endif
