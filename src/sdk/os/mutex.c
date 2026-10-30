/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/os/OSMutex.c. */
#include "os_mutex.h"
#define NULL 0
extern void OSWakeupThread(OSThreadQueue *);
#define DEQUEUE_HEAD(mutex, queue, link)                                               \
    do {                                                                               \
        struct OSMutex *__next = mutex->link.next;                                     \
        if (__next == NULL) {                                                          \
            (queue)->tail = 0;                                                         \
        } else {                                                                       \
            __next->link.prev = 0;                                                     \
        }                                                                              \
        (queue)->head = __next;                                                        \
    } while (0);

void __OSUnlockAllMutex(struct OSThread *thread)
{
    struct OSMutex *mutex;

    while (thread->queueMutex.head) {
        mutex = thread->queueMutex.head;
        DEQUEUE_HEAD(mutex, &thread->queueMutex, link);
        mutex->count = 0;
        mutex->thread = 0;
        OSWakeupThread(&mutex->queue);
    }
}
