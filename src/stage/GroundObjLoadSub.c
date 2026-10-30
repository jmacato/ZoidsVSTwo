#include "joint.h"

extern void GroundObjLoadSub2(HSD_JObj *joint, s32 stage, s32 usb);

#pragma dont_inline on
void GroundObjLoadSub(HSD_JObj *joint, s32 stage, s32 usb)
{
    if (joint != (void *)0) {
        for (joint = joint != 0 ? joint->child : 0; joint != 0;
             joint = joint != 0 ? joint->next : 0)
            GroundObjLoadSub2(joint, stage, usb);
    }
}
#pragma dont_inline reset
