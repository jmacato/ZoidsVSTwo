#ifndef ZOIDS_PERF_H
#define ZOIDS_PERF_H
#include "types.h"
typedef struct HSD_PerfStat {
    f32 cpu_time;
    f32 draw_time;
    f32 total_time;
    u32 matrix_load_count;
    u32 envelope_blending[32];
} HSD_PerfStat;
extern HSD_PerfStat HSD_PerfLastStat;
extern HSD_PerfStat HSD_PerfCurrentStat;
extern void HSD_PerfInitStat(void);
extern void HSD_PerfSetStartTime(void);
extern void HSD_PerfSetCPUTime(void);
extern void HSD_PerfSetDrawTime(void);
extern void HSD_PerfSetTotalTime(void);
extern void HSD_PerfCountEnvelopeBlending(s32 count);
#endif
