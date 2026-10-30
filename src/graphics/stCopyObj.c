#include "jobj.h"

#define JOBJ_INSTANCE 0x1000

extern MaterialPose *makeDobj(void);
extern JointPose *makeJobj(void);

void stCopyObj(HSD_JObj *joint, JointPose *pose)
{
    HSD_DObj *object;
    MaterialPose *material;
    MaterialPose *next_material;
    JointPose *child_pose;

    if (joint != (void *)0) {
        object = HSD_JObjGetDObj(joint);
        if (object != 0) {
            material = makeDobj();
            pose->materials = material;
            for (;;) {
                object = object != 0 ? object->next : 0;
                if (object == (void *)0)
                    break;
                next_material = makeDobj();
                material->next = next_material;
                material = next_material;
            }
        }
        if (joint->flags & JOBJ_INSTANCE)
            return;
        joint = joint != 0 ? joint->child : 0;
        if (joint != (void *)0) {
            child_pose = makeJobj();
            pose->child = child_pose;
            pose = child_pose;
            stCopyObj(joint, pose);
            while (joint != 0) {
                joint = joint != 0 ? joint->next : 0;
                if (joint != 0) {
                    child_pose = makeJobj();
                    pose->next = child_pose;
                    pose = child_pose;
                    stCopyObj(joint, pose);
                }
            }
        }
    }
}
