#include "os_font.h"
#include "gx_state.h"
#include "gx_geometry.h"

extern OSFontHeader *FontData;
extern s16 FontSize;

static inline void GXPosition3s16(s16 x, s16 y, s16 z)
{
    GXWGFifo.s16 = x;
    GXWGFifo.s16 = y;
    GXWGFifo.s16 = z;
}

static inline void GXTexCoord2s16(s16 s, s16 t)
{
    GXWGFifo.s16 = s;
    GXWGFifo.s16 = t;
}

void DrawFontChar(s32 x, s32 y, s32 z, s32 s, s32 t, s32 width)
{
    s16 x1 = (s16)x;
    /* Both conversions preserve MWCC's second sign extension. */
    s16 x2 = (s16)((s32)(u32)x1 + FontSize * width / FontData->cellWidth);
    s16 y1 = (s16)y;
    s16 y2 = (s16)(y - FontData->cellHeight * FontSize / FontData->cellWidth);
    s16 s1 = (s16)s;
    s16 t1 = (s16)t;
    s16 s2 = (s16)(s + width);
    s16 t2 = (s16)(t + FontData->cellHeight);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(x1, y1, (s16)z);
    GXTexCoord2s16(s1, t1);
    GXPosition3s16(x2, y1, (s16)z);
    GXTexCoord2s16(s2, t1);
    GXPosition3s16(x2, y2, (s16)z);
    GXTexCoord2s16(s2, t2);
    GXPosition3s16(x1, y2, (s16)z);
    GXTexCoord2s16(s1, t2);
}
