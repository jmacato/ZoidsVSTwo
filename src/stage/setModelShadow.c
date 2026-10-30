#include "joint.h"

extern void setModelShadow0(HSD_JObj *joint);

#pragma dont_inline on
void setModelShadow(HSD_JObj *joint)
{
    if (joint != (void *)0) {
        setModelShadow0(joint);
        for (joint = joint->child; joint != 0; joint = joint->next)
            setModelShadow(joint);
    }
}
#pragma dont_inline reset
