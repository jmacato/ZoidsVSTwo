#pragma readonly_strings on
#include "jobj.h"

#define JOBJ_HIDDEN 0x10
#define JOBJ_INSTANCE 0x1000
#define JOBJ_XLU 0x80000
#define JOBJ_ROOT_XLU 0x20000000
#define RENDER_XLU 0x40000000

void stCopyMatRet(HSD_JObj *joint, JointPose *pose)
{
    if (joint != (void *)0 && pose != (void *)0) {
        HSD_DObj *object;
        MaterialPose *saved;
        joint->flags = pose->saved_flags;
        object = HSD_JObjGetDObj(joint);
        saved = pose->materials;
        while (object != 0 && saved != 0) {
            HSD_MObj *material = saved->material;
            object->mobj = material;
            if (material != 0) {
                if (material->mat != 0)
                    material->mat->alpha = saved->alpha;
                material->rendermode = saved->render_mode;
            }
            object = object->next;
            saved = saved->next;
        }
        if (joint->flags & JOBJ_INSTANCE)
            return;
        joint = joint != 0 ? joint->child : 0;
        pose = pose != 0 ? pose->child : 0;
        while (joint != 0 && pose != 0) {
            stCopyMatRet(joint, pose);
            joint = joint->next;
            pose = pose->next;
        }
    }
}
