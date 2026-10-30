#include "gx_texture_types.h"

s32 calc_doltexture_size(u32 format, s32 width, s32 height)
{
    s32 size;
    switch (format) {
    case GX_TF_I4:
    case GX_TF_C4:
    case GX_TF_CMPR:
        size = ((width + 7) & ~7) / 2 * ((height + 7) & ~7);
        break;
    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_C8:
        size = ((width + 7) & ~7) * ((height + 3) & ~3);
        break;
    case GX_TF_IA8:
    case GX_TF_RGB565:
    case GX_TF_RGB5A3:
    case GX_TF_C14X2:
        size = ((width + 3) & ~3) * ((height + 3) & ~3) * 2;
        break;
    case GX_TF_RGBA8:
        size = ((width + 3) & ~3) * ((height + 3) & ~3) * 2 * 2;
        break;
    default:
        size = 0;
        break;
    }
    return size;
}
