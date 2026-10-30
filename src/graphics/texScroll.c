#include "tobj.h"

void texScroll(HSD_TObj *texture, f32 x, f32 y, f32 z)
{
    if (texture != (HSD_TObj *)0) {
        texture->wrap_s = GX_REPEAT;
        texture->wrap_t = GX_REPEAT;
        texture->mtx[0][3] = x;
        texture->mtx[1][3] = y;
        texture->mtx[2][3] = z;
    }
}
