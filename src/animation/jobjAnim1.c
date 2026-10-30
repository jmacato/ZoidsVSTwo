#include "jobj.h"

extern s32 getEndF(HSD_JObj *joint);

#pragma dont_inline on
s32 jobjAnim1(HSD_JObj *joint)
{
    if (joint == (void *)0)
        return 0;
    HSD_JObjAnim(joint);
    return getEndF(joint);
}
