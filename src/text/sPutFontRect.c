#include "gx_tev.h"
#include "joint.h"

extern void sPutFontRect0(char *string, HSD_JObj *top_left, HSD_JObj *bottom_right, GXColor color, GXColor shadow,
                          s32 alpha, u32 flags, s32 mode);

void sPutFontRect(char *string, HSD_JObj *top_left, HSD_JObj *bottom_right, GXColor color, GXColor shadow,
                  s32 alpha, u32 flags)
{
    sPutFontRect0(string, top_left, bottom_right, color, shadow, alpha, flags, 0);
}
