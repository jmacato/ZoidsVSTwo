#include "gx_tev.h"

extern void sPutFontAllPrologue2(void);
extern void sPutFontAllEpilogue(void);
extern void sPutFontXY1(char *text, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, GXColor color,
                        GXColor shadow, s32 alpha, s32 flags, s32 align);

void sPutStringXYC2(char *text, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, GXColor color, GXColor shadow,
                    s32 alpha, s32 flags, s32 align)
{
    sPutFontAllPrologue2();
    sPutFontXY1(text, x, y, z, scale_x, scale_y, color, shadow, alpha, flags, align);
    sPutFontAllEpilogue();
}
