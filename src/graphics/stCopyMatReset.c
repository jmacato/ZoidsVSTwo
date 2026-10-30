#pragma readonly_strings on
#include "jobj.h"

#define JOBJ_HIDDEN 0x10
#define JOBJ_INSTANCE 0x1000
#define JOBJ_XLU 0x80000
#define JOBJ_ROOT_XLU 0x20000000
#define RENDER_XLU 0x40000000

extern void free(void *memory);

void stCopyMatReset(HSD_JObj *joint, JointPose *pose)
{
    HSD_DObj *object;
    MaterialPose *saved;
    MaterialPose *next;
    if (joint != (void *)0) {
        if (pose != (void *)0) {
            joint->flags = pose->saved_flags;
            pose->saved_flags = 0;
            object = HSD_JObjGetDObj(joint);
            saved = pose->materials;
            pose->materials = 0;
            while (object != 0 && saved != 0) {
                object->mobj = saved->material;
                object = object != 0 ? object->next : 0;
                next = saved->next;
                free(saved);
                saved = next;
            }
            if (joint->flags & JOBJ_INSTANCE)
                return;
            joint = joint != 0 ? joint->child : 0;
            pose = pose != 0 ? pose->child : 0;
            while (joint != 0 && pose != 0) {
                stCopyMatReset(joint, pose);
                joint = joint->next;
                pose = pose->next;
            }
        }
    }
}
