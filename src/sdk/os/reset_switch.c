/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/os/OSResetSW.c. */
#include "os_interrupt.h"
#include "os_reset.h"
#include "os_time.h"
#define TRUE 1
#define FALSE 0
#define NULL 0
#define OSMillisecondsToTicks(value) ((value) * (__OSBusClock / 4 / 1000))
#define OSMicrosecondsToTicks(value) ((value) * (__OSBusClock / 4 / 125000) / 8)
#define OSSecondsToTicks(value) ((value) * (__OSBusClock / 4))
#define OSTicksToSeconds(value) ((value) / (__OSBusClock / 4))
extern u32 __OSBusClock;
__asm__(".set __OSBusClock, 0x800000F8");
extern volatile u32 __PIRegs[12];
__asm__(".set __PIRegs, 0xCC003000");
extern s64 __OSStartTime;
extern u8 GameChoice;
__asm__(".set GameChoice, 0x800030E3");

static OSResetCallback ResetCallback;

static s32 Down;

static s32 LastState;

static OSTime HoldUp;
static OSTime HoldDown;

void __OSResetSWInterruptHandler(__OSInterrupt exception, struct OSContext *context)
{
    OSResetCallback callback;

    HoldDown = __OSGetSystemTime();
    while (__OSGetSystemTime() - HoldDown < OSMicrosecondsToTicks(100) &&
           !(__PIRegs[0] & 0x00010000)) {
        ;
    }
    if (!(__PIRegs[0] & 0x00010000)) {
        LastState = Down = TRUE;
        __OSMaskInterrupts(OS_INTERRUPTMASK_PI_RSW);
        if (ResetCallback) {
            callback = ResetCallback;
            ResetCallback = NULL;
            callback();
        }
    }
    __PIRegs[0] = 2;
}

s32 OSGetResetButtonState(void)
{
    s32 enabled = OSDisableInterrupts();
    s32 state;
    OSTime now = __OSGetSystemTime();
    u32 reg = __PIRegs[0];

    if (!(reg & 0x00010000)) {
        if (!Down) {
            Down = TRUE;
            state = HoldUp ? TRUE : FALSE;
            HoldDown = now;
        } else {
            state =
                HoldUp || (OSMicrosecondsToTicks(100) < now - HoldDown) ? TRUE : FALSE;
        }
    } else if (Down) {
        Down = FALSE;
        state = LastState;
        if (state) {
            HoldUp = now;
        } else {
            HoldUp = 0;
        }
    } else if (HoldUp && (now - HoldUp < OSMillisecondsToTicks(40))) {
        state = TRUE;
    } else {
        state = FALSE;
        HoldUp = 0;
    }

    LastState = state;

    if (GameChoice & 0x3F) {
        OSTime fire = (GameChoice & 0x3F) * 60;
        fire = __OSStartTime + OSSecondsToTicks(fire);
        if (fire < now) {
            now -= fire;
            now = OSTicksToSeconds(now) / 2;
            if ((now & 1) == 0) {
                state = TRUE;
            } else {
                state = FALSE;
            }
        }
    }

    OSRestoreInterrupts(enabled);
    return state;
}

s32 OSGetResetSwitchState(void)
{
    return OSGetResetButtonState();
}
