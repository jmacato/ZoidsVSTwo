#include "joint.h"
#include "os_font.h"
#include "gx_attr.h"
#include "gx_geometry.h"

extern void *LastSheet;
extern void LoadSheet(void *image, GXTexMapID id);
extern void DrawFontChar2b(s32 s, s32 t, s32 width, f32 x, f32 y, f32 z, f32 scale);
extern f32 fontWX(char *text);

void DrawFontChar00(char *string, HSD_JObj *joint, f32 scale)
{
    s32 width;
    void *image;
    s32 s;
    s32 t;
    GXColor color = { 0xFF, 0x00, 0xFF, 0xC8 };
    register f32 dx;
    f32 x;
    f32 y;
    f32 z;

    x = joint->matrix[0][3];
    y = joint->matrix[1][3];
    z = joint->matrix[2][3];
    LastSheet = 0;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 4);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    dx = 0.0f;
    while (*string != 0) {
        if (*string == '\n') {
            string++;
        } else if (*string == '\r') {
            y -= 24.0f * scale;
            dx = 0.0f;
            string++;
        } else {
            string = OSGetFontTexture(string, &image, &s, &t, &width);
            LoadSheet(image, GX_TEXMAP0);
            DrawFontChar2b(s, t, width, x + dx, y, z, scale);
            dx += width * scale;
        }
    }
    joint = joint;
}
