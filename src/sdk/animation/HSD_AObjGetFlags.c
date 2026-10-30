#include "aobj.h"

u32 HSD_AObjGetFlags(HSD_AObj *animation)
{
    if (animation != 0) {
        return animation->flags;
    }
    return 0;
}
