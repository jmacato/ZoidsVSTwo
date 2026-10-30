#include "os_font.h"
#include "gx_attr.h"
#include "gx_geometry.h"

extern void *LastSheet;
extern OSFontHeader *FontData;
extern s16 FontSize;
extern s16 FontSpace;
extern void LoadSheet(void *image, GXTexMapID id);
extern void DrawFontChar(s32 x, s32 y, s32 z, s32 s, s32 t, s32 width);

s32 sPutFont(char *string, s32 x, s32 y, s32 z)
{
    s32 width;
    void *image;
    s32 s;
    s32 t;
    s32 pos;
    /* The original copies this magenta color to the stack without reading it. */
    GXColor color = { 0xFF, 0x00, 0xFF, 0xC8 };

    LastSheet = 0;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 4);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    x <<= 4;
    y <<= 4;
    z <<= 4;
    pos = 0;
    while (*string != 0) {
        if (*string == '\n') {
            pos = 0;
            string++;
            y += FontData->leading << 4;
        } else {
            string = OSGetFontTexture(string, &image, &s, &t, &width);
            LoadSheet(image, GX_TEXMAP0);
            DrawFontChar(x + pos, y, z, s, t, width);
            pos += FontSize * width / FontData->cellWidth + FontSpace;
        }
    }
    return (pos + 15) / 16;
}
