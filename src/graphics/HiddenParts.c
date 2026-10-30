#include "jobj.h"

#define JOBJ_HIDDEN 0x10

#pragma dont_inline on
void HiddenParts(HSD_JObj *joint, s32 hidden)
{
    if (joint != (void *)0) {
        if (hidden != 0)
            HSD_JObjSetFlags(joint, JOBJ_HIDDEN);
        else
            HSD_JObjClearFlags(joint, JOBJ_HIDDEN);
    }
}
