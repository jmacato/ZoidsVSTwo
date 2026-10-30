/* Types follow
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/os/OSAlarm.h. */
#ifndef ZOIDS_OS_ALARM_H
#define ZOIDS_OS_ALARM_H
#include "os_context.h"
typedef s64 OSTime;
typedef struct OSAlarm OSAlarm;
typedef void (*OSAlarmHandler)(OSAlarm *, OSContext *);
struct OSAlarm {
    OSAlarmHandler handler;
    u32 tag;
    OSTime fire;
    OSAlarm *prev;
    OSAlarm *next;
    OSTime period;
    OSTime start;
};
extern void OSInitAlarm(void);
extern void OSCreateAlarm(OSAlarm *alarm);
extern void OSSetAlarm(OSAlarm *alarm, OSTime ticks, OSAlarmHandler handler);
extern void OSCancelAlarm(OSAlarm *alarm);
#endif
