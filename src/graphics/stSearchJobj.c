#include "model_archive.h"
#include "jobj.h"

JointPose *stSearchJobj(HSD_JObj *joint, JointPose *pose, HSD_JObj *target)
{
    if (joint == (void *)0)
        return 0;
    if (pose == (void *)0)
        return 0;
    if (joint == target)
        return pose;
    joint = joint != 0 ? joint->child : 0;
    pose = pose->child;
    while (joint != 0 && pose != 0) {
        JointPose *found = stSearchJobj(joint, pose, target);
        if (found != 0)
            return found;
        joint = joint != 0 ? joint->next : 0;
        pose = pose->next;
    }
    return 0;
}
