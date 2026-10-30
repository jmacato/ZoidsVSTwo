#include "aobj.h"

void HSD_AObjSetRewindFrame(HSD_AObj *animation, f32 value)
{
    if (animation != 0) {
        animation->rewind_frame = value;
    }
}
