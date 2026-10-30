#include "game_text.h"

f32 putAscMtxLR_sp(f32 matrix[3][4], s32 font, char *text, s32 right_align, f32 spacing)
{
    Vec3 position;
    s32 i;

    for (i = 0; i < 3; i++) {
        ((f32 *)&position)[i] = matrix[i][3];
    }
    return putAscLR_sp(&position, font, text, right_align, spacing);
}
