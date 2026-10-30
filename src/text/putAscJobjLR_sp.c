#include "game_text.h"

f32 putAscJobjLR_sp(HSD_JObj *joint, s32 font, char *text, s32 right_align, f32 spacing)
{
    if (joint == (void *)0) {
        return 0.0f;
    }
    return putAscMtxLR_sp(joint->matrix, font, text, right_align, spacing);
}
