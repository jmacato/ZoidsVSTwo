#include "os_time.h"
extern u32 __OSBusClock;
__asm__(".set __OSBusClock, 0x800000F8");
extern s64 TEA_time_setm;
extern s64 TEA_time_set;

void TEA_SetTimerInit(void)
{
    TEA_time_set = OSGetTime() / (__OSBusClock / 4);
    TEA_time_setm = OSGetTime() / ((__OSBusClock / 4) / 1000);
}


