#include "gx_tev.h"
#include "joint.h"
#include "os_font.h"
#include "cobj.h"

extern void *LastSheet;
extern void sPutFontRectPrologue(GXColor color, GXColor shadow, s32 alpha, u32 flags);
extern void sPutFontRectEpilogue(void);
extern void DrawFontChar00(char *string, HSD_JObj *joint, f32 scale, f32 limit);

static inline f32 absf(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

void sPutFontRect0(char *string, HSD_JObj *top_left, HSD_JObj *bottom_right, GXColor color, GXColor shadow,
                   s32 alpha, u32 flags, s32 mode)
{

    s32 width;
    void *image;
    s32 s;
    s32 t;
    Vec3 eye;
    char *p;
    s32 widest;
    s32 line_width;
    s32 height;
    f32 scale_x;
    f32 scale_y;
    s32 limit;

    if (top_left != 0 && bottom_right != 0 && string != 0 && *string != 0) {
        widest = 0;
        height = 0;
        line_width = 0;
        LastSheet = 0;
        p = string;
        while (*p != 0) {
            if (*p == '\n') {
                p++;
            } else if (*p == '\r') {
                line_width = 0;
                p++;
                height += 24;
            } else {
                p = OSGetFontTexture(p, &image, &s, &t, &width);
                line_width += width;
                if (widest < line_width)
                    widest = line_width;
            }
        }
        height += 24;
        if (widest != 0) {
            /* MWCC removes the unused high word, then schedules this block again. */
            scale_x = (bottom_right->matrix[0][3] - top_left->matrix[0][3]) / (f32)(s32)(s64)widest;
            scale_y = (top_left->matrix[1][3] - bottom_right->matrix[1][3]) / (f32)height;
            if (scale_x > scale_y)
                scale_x = scale_y;
            limit = absf(bottom_right->matrix[0][3] - top_left->matrix[0][3]);
            {
                s32 sPutFontRectPrologue_alpha = alpha;
                sPutFontRectPrologue(color, shadow, sPutFontRectPrologue_alpha, flags);
            }
            HSD_CObjGetEyePosition(HSD_CObjGetCurrent(), &eye);
            DrawFontChar00(string, top_left, scale_x, mode != 0 ? limit : 0);
            sPutFontRectEpilogue();
        }
    }
}
