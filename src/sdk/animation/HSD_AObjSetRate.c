#include "aobj.h"

void HSD_AObjSetRate(HSD_AObj *animation, f32 value)
{
    if (animation != 0) {
        animation->rate = value;
    }
}
