#include "perf.h"

extern s64 start_time;
extern s64 OSGetTime(void);
extern void *memset(void *memory, s32 value, u32 size);
extern void __assert(const char *file, u32 line, const char *condition);
/* The console stores its bus clock at this fixed address. */
#define BUS_CLOCK (*(volatile u32 *)0x800000F8)

HSD_PerfStat HSD_PerfLastStat = {0};
HSD_PerfStat HSD_PerfCurrentStat = {0};
void HSD_PerfInitStat(void)
{
    HSD_PerfLastStat = HSD_PerfCurrentStat;
    memset(&HSD_PerfCurrentStat, 0, sizeof(HSD_PerfStat));
}
void HSD_PerfSetStartTime(void)
{
    start_time = OSGetTime();
}
void HSD_PerfSetCPUTime(void)
{
    HSD_PerfCurrentStat.cpu_time =
        (OSGetTime() - start_time) / (f32)((BUS_CLOCK / 4) / 60);
}
void HSD_PerfSetDrawTime(void)
{
    HSD_PerfCurrentStat.draw_time =
        (OSGetTime() - start_time) / (f32)((BUS_CLOCK / 4) / 60);
}
void HSD_PerfSetTotalTime(void)
{
    HSD_PerfCurrentStat.total_time =
        (OSGetTime() - start_time) / (f32)((BUS_CLOCK / 4) / 60);
}
