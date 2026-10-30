#ifndef ZOIDS_OS_TIME_H
#define ZOIDS_OS_TIME_H
#include "types.h"
typedef struct OSCalendarTime {
    int sec;
    int min;
    int hour;
    int mday;
    int mon;
    int year;
    int wday;
    int yday;
    int msec;
    int usec;
} OSCalendarTime;
extern s64 OSGetTime(void);
extern s64 __OSGetSystemTime(void);
extern void OSTicksToCalendarTime(s64 ticks, OSCalendarTime *calendar);
#endif
