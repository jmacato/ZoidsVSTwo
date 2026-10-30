#include "game_text.h"

f32 putAscMtxLR(f32 matrix[3][4], s32 font, char *text, s32 right_align)
{
    return putAscMtxLR_sp(matrix, font, text, right_align, 0.0f);
}
