#ifndef ZOIDS_VI_H
#define ZOIDS_VI_H

#include "types.h"

#define VI_TVMODE(format, interlace) (((format) << 2) + (interlace))

#define VI_INTERLACE 0
#define VI_NON_INTERLACE 1
#define VI_PROGRESSIVE 2

#define VI_NTSC 0
#define VI_PAL 1
#define VI_MPAL 2
#define VI_DEBUG 3
#define VI_DEBUG_PAL 4
#define VI_EURGB60 5

typedef enum {
    VI_TVMODE_NTSC_INT = VI_TVMODE(VI_NTSC, VI_INTERLACE),
    VI_TVMODE_NTSC_DS = VI_TVMODE(VI_NTSC, VI_NON_INTERLACE),
    VI_TVMODE_NTSC_PROG = VI_TVMODE(VI_NTSC, VI_PROGRESSIVE),
    VI_TVMODE_PAL_INT = VI_TVMODE(VI_PAL, VI_INTERLACE),
    VI_TVMODE_PAL_DS = VI_TVMODE(VI_PAL, VI_NON_INTERLACE),
    VI_TVMODE_EURGB60_INT = VI_TVMODE(VI_EURGB60, VI_INTERLACE),
    VI_TVMODE_EURGB60_DS = VI_TVMODE(VI_EURGB60, VI_NON_INTERLACE),
    VI_TVMODE_MPAL_INT = VI_TVMODE(VI_MPAL, VI_INTERLACE),
    VI_TVMODE_MPAL_DS = VI_TVMODE(VI_MPAL, VI_NON_INTERLACE),
    VI_TVMODE_DEBUG_INT = VI_TVMODE(VI_DEBUG, VI_INTERLACE),
    VI_TVMODE_DEBUG_PAL_INT = VI_TVMODE(VI_DEBUG_PAL, VI_INTERLACE),
    VI_TVMODE_DEBUG_PAL_DS = VI_TVMODE(VI_DEBUG_PAL, VI_NON_INTERLACE)
} VITVMode;

typedef enum { VI_XFBMODE_SF = 0, VI_XFBMODE_DF } VIXFBMode;

typedef void (*VIRetraceCallback)(u32 retraceCount);

#define VI_TVMODE_3 3
struct GXRenderModeObj;
extern void __VIInit(VITVMode mode);
extern void VIInit(void);
extern void VIConfigure(struct GXRenderModeObj *mode);
extern void VIWaitForRetrace(void);
extern VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback callback);
extern VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback);
extern void VIFlush(void);
extern void VISetNextFrameBuffer(void *buffer);
extern void VISetBlack(s32 black);
extern u32 VIGetRetraceCount(void);
extern u32 VIGetNextField(void);
extern u32 VIGetCurrentLine(void);
extern u32 VIGetTvFormat(void);
#endif
