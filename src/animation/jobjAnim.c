#include "jobj.h"

extern s32 getEndF(HSD_JObj *joint);

#pragma dont_inline on
s32 jobjAnim(HSD_JObj *joint)
{
    if (joint == (void *)0)
        return 0;
    HSD_JObjAnimAll(joint);
    return getEndF(joint);
}
