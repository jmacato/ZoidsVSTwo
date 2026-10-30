#include "jobj.h"
#pragma dont_inline on

HSD_JObj *jobjWithAobj(HSD_JObj *joint)
{
    if (joint == (void *)0)
        return 0;
    if (joint->animation != 0)
        return joint;
    for (joint = joint != 0 ? joint->child : 0; joint != 0;
         joint = joint != 0 ? joint->next : 0) {
        HSD_JObj *found = jobjWithAobj(joint);
        if (found != 0)
            return found;
    }
    return 0;
}
