#include "aobj.h"

void HSD_AObjSetEndFrame(HSD_AObj *animation, f32 value)
{
    if (animation != 0) {
        animation->end_frame = value;
    }
}
