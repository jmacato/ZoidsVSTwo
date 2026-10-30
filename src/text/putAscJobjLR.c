#include "game_text.h"

f32 putAscJobjLR(HSD_JObj *joint, s32 font, char *text, s32 right_align)
{
    if (joint == (void *)0) {
        return 0.0f;
    }
    return putAscMtxLR(joint->matrix, font, text, right_align);
}
