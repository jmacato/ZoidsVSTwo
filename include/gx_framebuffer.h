#ifndef ZOIDS_GX_FRAMEBUFFER_H
#define ZOIDS_GX_FRAMEBUFFER_H
#include "gx_tev.h"
typedef enum _GXCopyMode {
    GX_COPY_PROGRESSIVE = 0,
    GX_COPY_INTLC_EVEN = 2,
    GX_COPY_INTLC_ODD = 3,
} GXCopyMode;
typedef enum _GXFBClamp {
    GX_CLAMP_NONE = 0,
    GX_CLAMP_TOP = 1,
    GX_CLAMP_BOTTOM = 2,
} GXFBClamp;
typedef enum _GXGamma {
    GX_GM_1_0,
    GX_GM_1_7,
    GX_GM_2_2,
} GXGamma;
extern void GXSetDispCopySrc(u16 left, u16 top, u16 width, u16 height);
extern void GXSetTexCopySrc(u16 left, u16 top, u16 width, u16 height);
extern void GXSetDispCopyDst(u16 width, u16 height);
extern void GXSetTexCopyDst(u16 width, u16 height, s32 format, u8 mipmap);
extern void GXSetDispCopyFrame2Field(GXCopyMode mode);
extern void GXSetCopyClamp(GXFBClamp clamp);
extern u32 GXSetDispCopyYScale(f32 scale);
extern void GXSetCopyClear(GXColor color, u32 depth);
extern void GXSetCopyFilter(u8 aa, const u8 samples[12][2], u8 vertical,
                            const u8 filter[7]);
extern void GXSetDispCopyGamma(GXGamma gamma);
extern void GXCopyDisp(void *destination, u8 clear);
extern void GXCopyTex(void *destination, u8 clear);
extern void GXClearBoundingBox(void);
#endif
