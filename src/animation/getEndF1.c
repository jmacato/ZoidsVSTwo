#include "jobj.h"

extern u32 HSD_AObjGetFlags(HSD_AObj *animation);

#pragma dont_inline on
s32 getEndF1(HSD_JObj *joint)
{
    if (joint == (void *)0)
        return 1;
    if (joint->animation != 0) {
        u32 flags = HSD_AObjGetFlags(joint->animation);
        if (flags & AOBJ_REWINDED)
            return 1;
        if (flags & AOBJ_NO_ANIM)
            return 1;
        return 0;
    }
    return 1;
}
