/* References: https://github.com/doldecomp/dolsdk2001/blob/main/src/os/OSAlarm.c
 * and https://github.com/doldecomp/dolsdk2004/blob/main/src/os/OSAlarm.c. */
#include "os_alarm.h"
#include "os.h"
#include "os_interrupt.h"
#define NULL 0
typedef u8 __OSException;
struct OSAlarmQueue {
    OSAlarm *head;
    OSAlarm *tail;
};
static struct OSAlarmQueue AlarmQueue;
extern void DecrementerExceptionHandler(__OSException, OSContext *);
extern OSTime __OSGetSystemTime(void);
extern void PPCMtdec(u32);
extern void OSDisableScheduler(void);
extern void OSEnableScheduler(void);
extern void __OSReschedule(void);
static inline void SetTimer(OSAlarm *alarm)
{
    OSTime delta = alarm->fire - __OSGetSystemTime();

    if (delta < 0) {
        PPCMtdec(0);
    } else if (delta < 0x80000000) {
        PPCMtdec((u32)delta);
    } else {
        PPCMtdec(0x7fffffff);
    }
}
void OSInitAlarm(void)
{
    if (__OSGetExceptionHandler(__OS_EXCEPTION_DECREMENTER) != DecrementerExceptionHandler) {
        AlarmQueue.head = AlarmQueue.tail = NULL;
        __OSSetExceptionHandler(__OS_EXCEPTION_DECREMENTER, DecrementerExceptionHandler);
    }
}
void OSCreateAlarm(OSAlarm *alarm)
{
    alarm->handler = 0;
}
void InsertAlarm(OSAlarm *alarm, OSTime fire, OSAlarmHandler handler)
{
    OSAlarm *next;
    OSAlarm *prev;

    if (0 < alarm->period) {
        OSTime time = __OSGetSystemTime();

        fire = alarm->start;
        if (alarm->start < time) {
            fire += alarm->period * ((time - alarm->start) / alarm->period + 1);
        }
    }

    alarm->handler = handler;
    alarm->fire = fire;

    for (next = AlarmQueue.head; next; next = next->next) {
        if (next->fire <= fire) {
            continue;
        }

        alarm->prev = next->prev;
        next->prev = alarm;
        alarm->next = next;
        prev = alarm->prev;

        if (prev) {
            prev->next = alarm;
        } else {
            AlarmQueue.head = alarm;
            SetTimer(alarm);
        }

        return;
    }

    alarm->next = 0;
    prev = AlarmQueue.tail;
    AlarmQueue.tail = alarm;
    alarm->prev = prev;

    if (prev) {
        prev->next = alarm;
    } else {
        AlarmQueue.head = AlarmQueue.tail = alarm;
        SetTimer(alarm);
    }
}
void OSSetAlarm(OSAlarm *alarm, OSTime tick, OSAlarmHandler handler)
{
    s32 enabled;
    enabled = OSDisableInterrupts();
    alarm->period = 0;
    InsertAlarm(alarm, __OSGetSystemTime() + tick, handler);
    OSRestoreInterrupts(enabled);
}
void OSCancelAlarm(OSAlarm *alarm)
{
    OSAlarm *next;
    s32 enabled;

    enabled = OSDisableInterrupts();

    if (alarm->handler == 0) {
        OSRestoreInterrupts(enabled);
        return;
    }

    next = alarm->next;
    if (next == 0) {
        AlarmQueue.tail = alarm->prev;
    } else {
        next->prev = alarm->prev;
    }
    if (alarm->prev) {
        alarm->prev->next = next;
    } else {
        AlarmQueue.head = next;
        if (next) {
            SetTimer(next);
        }
    }
    alarm->handler = 0;
    OSRestoreInterrupts(enabled);
}
/* The original assembly interrupt entry calls this private C callback. */
#pragma force_active on
void DecrementerExceptionCallback(register __OSException exception,
                                         register OSContext *context)
{
    OSAlarm *alarm;
    OSAlarm *next;
    OSAlarmHandler handler;
    OSTime time;
    OSContext exceptionContext;

    time = __OSGetSystemTime();
    alarm = AlarmQueue.head;
    if (alarm == 0) {
        OSLoadContext(context);
    }

    if (time < alarm->fire) {
        SetTimer(alarm);
        OSLoadContext(context);
    }

    next = alarm->next;
    AlarmQueue.head = next;
    if (next == 0) {
        AlarmQueue.tail = 0;
    } else {
        next->prev = 0;
    }

    handler = alarm->handler;
    alarm->handler = 0;
    if (0 < alarm->period) {
        InsertAlarm(alarm, 0, handler);
    }

    if (AlarmQueue.head) {
        SetTimer(AlarmQueue.head);
    }

    OSDisableScheduler();
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    handler(alarm, context);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
    OSEnableScheduler();
    __OSReschedule();
    OSLoadContext(context);
}
#pragma force_active off
