/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/dvd/dvdqueue.c. */
#include "dvd.h"
#include "os_interrupt.h"
#define TRUE 1
#define FALSE 0
#define NULL 0
#define MAX_QUEUES 4
typedef struct {
    DVDCommandBlock *next;
    DVDCommandBlock *prev;
} DVDQueue;

static DVDQueue WaitingQueue[MAX_QUEUES];

void __DVDClearWaitingQueue(void)
{
    u32 i;

    for (i = 0; i < MAX_QUEUES; i++) {
        DVDCommandBlock *q;

        q = (DVDCommandBlock *)&(WaitingQueue[i]);
        q->next = q;
        q->prev = q;
    }
}

s32 __DVDPushWaitingQueue(s32 prio, DVDCommandBlock *block)
{
    s32 enabled;
    DVDCommandBlock *q;

    enabled = OSDisableInterrupts();

    q = (DVDCommandBlock *)&(WaitingQueue[prio]);

    q->prev->next = block;
    block->prev = q->prev;
    block->next = q;
    q->prev = block;

    OSRestoreInterrupts(enabled);

    return TRUE;
}

static inline DVDCommandBlock *PopWaitingQueuePrio(s32 prio)
{
    DVDCommandBlock *tmp;
    s32 enabled;
    DVDCommandBlock *q;

    enabled = OSDisableInterrupts();

    q = (DVDCommandBlock *)&(WaitingQueue[prio]);

    tmp = q->next;
    q->next = tmp->next;
    tmp->next->prev = q;

    OSRestoreInterrupts(enabled);

    tmp->next = (DVDCommandBlock *)NULL;
    tmp->prev = (DVDCommandBlock *)NULL;

    return tmp;
}

DVDCommandBlock *__DVDPopWaitingQueue(void)
{
    u32 i;
    s32 enabled;
    DVDCommandBlock *q;

    enabled = OSDisableInterrupts();

    for (i = 0; i < MAX_QUEUES; i++) {
        q = (DVDCommandBlock *)&(WaitingQueue[i]);
        if (q->next != q) {
            OSRestoreInterrupts(enabled);
            return PopWaitingQueuePrio((s32)i);
        }
    }

    OSRestoreInterrupts(enabled);

    return (DVDCommandBlock *)NULL;
}

s32 __DVDCheckWaitingQueue(void)
{
    u32 i;
    s32 enabled;
    DVDCommandBlock *q;

    enabled = OSDisableInterrupts();

    for (i = 0; i < MAX_QUEUES; i++) {
        q = (DVDCommandBlock *)&(WaitingQueue[i]);
        if (q->next != q) {
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    OSRestoreInterrupts(enabled);

    return FALSE;
}

s32 __DVDDequeueWaitingQueue(DVDCommandBlock *block)
{
    s32 enabled;
    DVDCommandBlock *prev;
    DVDCommandBlock *next;

    enabled = OSDisableInterrupts();

    prev = block->prev;
    next = block->next;

    if ((prev == (DVDCommandBlock *)NULL) || (next == (DVDCommandBlock *)NULL)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    prev->next = next;
    next->prev = prev;

    OSRestoreInterrupts(enabled);

    return TRUE;
}
