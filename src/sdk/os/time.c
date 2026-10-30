/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/os/OSTime.c. */
#include "os_time.h"
#include "asm_ops.h"
#include "os_interrupt.h"
extern u32 __OSBusClock : 0x800000F8;
#define OS_BUS_CLOCK __OSBusClock
#define OS_TIME_SPEED (OS_BUS_CLOCK / 4)

#define OS_TICKS_TO_SEC(x) ((x) / (OS_TIME_SPEED))
#define OS_TICKS_TO_MSEC(x) ((x) / (OS_TIME_SPEED / 1000))
#define OS_TICKS_TO_USEC(x) (((x) * 8) / (OS_TIME_SPEED / 125000))

#define OS_SEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED))

#define USEC_MAX 1000
#define MSEC_MAX 1000
#define MONTH_MAX 12
#define WEEK_DAY_MAX 7
#define YEAR_DAY_MAX 365

#define SECS_IN_MIN 60
#define SECS_IN_HOUR (SECS_IN_MIN * 60)
#define SECS_IN_DAY (SECS_IN_HOUR * 24)

#define BIAS 0xB2575

static int YearDays[MONTH_MAX] = {0,   31,  59,  90,  120, 151,
                                  181, 212, 243, 273, 304, 334};

static int LeapYearDays[MONTH_MAX] = {0,   31,  60,  91,  121, 152,
                                      182, 213, 244, 274, 305, 335};

static inline int IsLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
static inline int GetLeapDays(int year)
{

    if (year < 1) {
        return 0;
    }
    return (year + 3) / 4 - (year - 1) / 100 + (year - 1) / 400;
}
static void GetDates(int days, OSCalendarTime *td);
#pragma peephole off
long long __OSGetSystemTime(void)
{
    int enabled;
    long long *timeAdjustAddr;
    long long result;

    timeAdjustAddr = (long long *)0x800030D8;
    enabled = OSDisableInterrupts();

    result = OSGetTime() + *timeAdjustAddr;
    OSRestoreInterrupts(enabled);
    return result;
}

void OSTicksToCalendarTime(long long ticks, OSCalendarTime *td)
{
    int days;
    int secs;
    long long d;

    d = ticks % OS_SEC_TO_TICKS(1);
    if (d < 0) {
        d += OS_SEC_TO_TICKS(1);
    }

    td->usec = OS_TICKS_TO_USEC(d) % USEC_MAX;
    td->msec = OS_TICKS_TO_MSEC(d) % MSEC_MAX;

    ticks -= d;

    days = (OS_TICKS_TO_SEC(ticks) / SECS_IN_DAY) + BIAS;
    secs = OS_TICKS_TO_SEC(ticks) % SECS_IN_DAY;
    if (secs < 0) {
        days -= 1;
        secs += SECS_IN_DAY;
    }

    GetDates(days, td);
    td->hour = secs / 60 / 60;
    td->min = secs / 60 % 60;
    td->sec = secs % 60;
}

/* Keep this definition after its caller to preserve the original call. */
static void GetDates(int days, OSCalendarTime *td)
{
    int year;
    int n;
    int month;
    int *md;

    td->wday = (days + 6) % WEEK_DAY_MAX;

    for (year = days / YEAR_DAY_MAX;
         days < (n = year * YEAR_DAY_MAX + GetLeapDays(year)); year--) {
        ;
    }

    days -= n;
    td->year = year;
    td->yday = days;

    md = IsLeapYear(year) ? LeapYearDays : YearDays;
    for (month = MONTH_MAX; days < md[--month];) {
        ;
    }
    td->mon = month;
    td->mday = days - md[month] + 1;
}

u32 OSGetTick(void)
{
    register u32 tick;

    ASM_MFTB(tick);
    return tick;
}

#pragma peephole on
s64 OSGetTime(void)
{
    ASM_INSN(loop: mftbu r3);
    ASM_INSN(mftb r4);
    ASM_INSN(mftbu r5);
    ASM_INSN(cmpw r3, r5);
    ASM_INSN(bne loop);
}
