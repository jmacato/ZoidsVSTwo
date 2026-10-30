#include "os_font.h"
#include "gx_attr.h"
#include "gx_geometry.h"

extern void *LastSheet;
extern OSFontHeader *FontData;
extern s16 FontSize;
extern void LoadSheet(void *image, GXTexMapID id);
extern void DrawFontCharXY(f32 x, f32 y, f32 z, s32 s, s32 t, s32 width, f32 sx, f32 sy);

s32 sPutFontXY(char *string, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y)
{
    s32 width;
    void *image;
    s32 s;
    s32 t;
    f32 pos;

    LastSheet = 0;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 4);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    pos = 0.0f;
    while (*string != 0) {
        if (*string == '\r') {
            string++;
        } else if (*string == '\n') {
            string++;
            pos = 0.0f;
            y -= FontSize * (FontData->cellHeight / 16.0 * scale_y) / FontData->cellWidth;
        } else {
            string = OSGetFontTexture(string, &image, &s, &t, &width);
            LoadSheet(image, GX_TEXMAP0);
            DrawFontCharXY(x + pos, y, z, s, t, width, scale_x, scale_y);
            pos += scale_x * (FontSize * width / 16.0) / FontData->cellWidth;
        }
    }
    return (pos + 15.0f) / 16.0f;
}
