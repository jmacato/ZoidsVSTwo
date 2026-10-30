#include "aobj.h"

void HSD_AObjSetCurrentFrame(HSD_AObj *animation, f32 frame)
{
    if (animation != 0 && (animation->flags & AOBJ_NO_ANIM) == 0) {
        request_animation(animation, frame);
    }
}
