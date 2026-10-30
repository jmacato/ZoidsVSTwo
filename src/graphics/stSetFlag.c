#pragma readonly_strings on
#include "jobj.h"

#define JOBJ_HIDDEN 0x10
#define JOBJ_INSTANCE 0x1000
#define JOBJ_XLU 0x80000
#define JOBJ_ROOT_XLU 0x20000000
#define RENDER_XLU 0x40000000

void stSetFlag(HSD_JObj *joint, JointPose *pose, f32 alpha)
{
    if (joint != (void *)0 && pose != (void *)0) {
        if (alpha == 1.0)
            joint->flags = pose->saved_flags;
        else
            joint->flags |= JOBJ_ROOT_XLU | JOBJ_XLU;
        stMatSet(joint, pose, alpha);
        if (joint->flags & JOBJ_INSTANCE)
            return;
        joint = joint != 0 ? joint->child : 0;
        pose = pose != 0 ? pose->child : 0;
        while (joint != 0 && pose != 0) {
            stSetFlag(joint, pose, alpha);
            joint = joint->next;
            pose = pose->next;
        }
    }
}
