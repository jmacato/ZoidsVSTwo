#include "gx_tev_api.h"
#include "os_font.h"

extern OSFontHeader *FontData;
extern s16 FontSize;
extern s32 sPutFontXY(char *string, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y);

s32 sPutFontXY1(char *string, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, GXColor *color, GXColor *shadow, s32 alpha, u32 flags, u32 align)
{
    s32 width;
    void *image;
    s32 s;
    s32 t;
    char *p;
    f32 widest;
    f32 height;
    f32 line_width;

    if (flags & 1)
        GXSetZMode(1, GX_LESS, 1);
    else
        GXSetZMode(0, GX_ALWAYS, 0);
    if (flags & 2) {
        if (flags & 4)
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_COPY);
        else
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    } else {
        if (flags & 4)
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);
        else
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    }
    GXSetAlphaCompare(GX_GEQUAL, alpha, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, (flags & 0x10) ? GX_CA_ZERO : GX_CA_A1, (flags & 8) ? GX_CA_ZERO : GX_CA_A0,
                    GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, *color);
    GXSetTevColor(GX_TEVREG1, *shadow);
    widest = 0.0f;
    line_width = 0.0f;
    height = 0.0f;
    p = string;
    while (*p != 0) {
        if (*p == '\r') {
            p++;
        } else if (*p == '\n') {
            p++;
            line_width = 0.0f;
            height += FontSize * (FontData->cellHeight / 16.0 * scale_y) / FontData->cellWidth;
        } else {
            p = OSGetFontTexture(p, &image, &s, &t, &width);
            line_width += scale_x * (FontSize * width / 16.0) / FontData->cellWidth;
            if (widest < line_width)
                widest = line_width;
        }
    }
    height += FontSize * (FontData->cellHeight / 16.0 * scale_y) / FontData->cellWidth;
    if (align & 1)
        x -= widest / 2.0f;
    if (align & 2)
        y += height / 2.0f;
    return sPutFontXY(string, x, y, z, scale_x, scale_y);
}
