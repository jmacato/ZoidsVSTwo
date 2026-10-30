#include "game_text.h"

f32 putAscJobj(HSD_JObj *joint, s32 font, char *text)
{
    if (joint == (void *)0) {
        return 0.0f;
    }
    return putAscMtxLR(joint->matrix, font, text, 0);
}
