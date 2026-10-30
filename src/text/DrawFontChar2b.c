#include "os_font.h"
#include "gx_state.h"
#include "gx_geometry.h"

extern OSFontHeader *FontData;
extern s16 FontSize;

static inline void GXPosition3f32(f32 x, f32 y, f32 z)
{
    GXWGFifo.f32 = x;
    GXWGFifo.f32 = y;
    GXWGFifo.f32 = z;
}

static inline void GXTexCoord2s16(s16 s, s16 t)
{
    GXWGFifo.s16 = s;
    GXWGFifo.s16 = t;
}

void DrawFontChar2b(f32 x, s32 s, s32 t, s32 width, f32 y, f32 z, f32 scale)
{
    s16 s1;
    s16 t1;
    s16 s2;
    s16 t2;
    f32 x2;
    f32 px;
    f32 py;
    f32 y2;

    py = y;
    px = x;
    s1 = s;
    t1 = t;
    s2 = s + width;
    x2 = px + (f32)width * scale;
    y2 = y - scale * (f32)(FontData->cellHeight * FontSize / 16 / FontData->cellWidth);

    t2 = t + FontData->cellHeight;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(px, py, z);
    GXTexCoord2s16(s1, t1);
    GXPosition3f32(x2, py, z);
    GXTexCoord2s16(s2, t1);
    GXPosition3f32(x2, y2, z);
    GXTexCoord2s16(s2, t2);
    GXPosition3f32(px, y2, z);
    GXTexCoord2s16(s1, t2);
}
