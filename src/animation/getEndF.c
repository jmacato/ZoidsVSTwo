#include "jobj.h"
#include "aobj.h"

#pragma dont_inline on
s32 getEndF(HSD_JObj *joint)
{
    u32 flags;

    if (joint == (void *)0)
        return 1;
    if (joint->animation != 0) {
        flags = HSD_AObjGetFlags(joint->animation);
        if (flags & AOBJ_REWINDED)
            return 1;
        if (flags & AOBJ_NO_ANIM)
            return 1;
        return 0;
    }
    for (joint = joint->child; joint != 0; joint = joint->next) {
        if (getEndF(joint) != 0)
            return 1;
    }
    return 0;
}
