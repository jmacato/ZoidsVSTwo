#include "perf.h"

void HSD_PerfCountEnvelopeBlending(s32 count)
{
    if (count >= 32)
        __assert("perf.c", 164, "n < 32");
    HSD_PerfCurrentStat.envelope_blending[count]++;
}
