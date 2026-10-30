#include "tobj.h"

void texScrollMtx(HSD_TObj *texture, f32 matrix[3][4])
{
    s32 i;
    s32 j;
    if (texture != (void *)0) {
        texture->wrap_s = GX_REPEAT;
        texture->wrap_t = GX_REPEAT;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 4; j++)
                texture->mtx[i][j] = matrix[i][j];
        }
    }
}
