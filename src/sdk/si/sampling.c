/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/si/SISamplingRate.c. */
#include "si.h"
#include "os_error.h"
#define VI_NTSC 0
#define VI_PAL 1
#define VI_MPAL 2
#define VI_EURGB60 5
extern volatile u16 __VIRegs[64] : 0xCC002000;
extern u32 VIGetTvFormat(void);
#pragma dont_inline on
static u32 SamplingRate;

typedef struct XY {
    u16 line;
    u8 count;
} XY;

static XY XYNTSC[12] = {
    {263 - 17, 2}, {15, 18}, {30, 9}, {44, 6},  {52, 5},  {65, 4},
    {87, 3},       {87, 3},  {87, 3}, {131, 2}, {131, 2}, {131, 2},
};

static XY XYPAL[12] = {
    {313 - 17, 2}, {15, 21}, {29, 11}, {45, 7},  {52, 6},  {63, 5},
    {78, 4},       {104, 3}, {104, 3}, {104, 3}, {104, 3}, {156, 2},
};

void SISetSamplingRate(u32 msec)
{
    XY *xy;
    s32 enabled;

    if (msec > 11) {
        msec = 11;
    }

    enabled = OSDisableInterrupts();

    SamplingRate = msec;

    switch (VIGetTvFormat()) {
    case VI_NTSC:
    case VI_MPAL:
    case VI_EURGB60:
        xy = XYNTSC;
        break;
    case VI_PAL:
        xy = XYPAL;
        break;
    default:
        OSReport("SISetSamplingRate: unknown TV format. Use default.");
        msec = 0;
        xy = XYNTSC;
        break;
    }

    SISetXY((__VIRegs[54] & 1 ? 2u : 1u) * xy[msec].line, xy[msec].count);
    OSRestoreInterrupts(enabled);
}

void SIRefreshSamplingRate()
{
    SISetSamplingRate(SamplingRate);
}
#pragma dont_inline reset
