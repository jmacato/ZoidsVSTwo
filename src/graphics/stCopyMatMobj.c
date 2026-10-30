#include "jobj.h"

#define JOBJ_INSTANCE 0x1000

void stCopyMatMobj(HSD_JObj *joint, JointPose *pose)
{
    HSD_DObj *object;
    MaterialPose *saved;

    if (joint != (void *)0 && pose != (void *)0) {
        pose->saved_flags = joint->flags;
        object = HSD_JObjGetDObj(joint);
        saved = pose->materials;
        while (object != 0 && saved != 0) {
            HSD_MObj *material = object->mobj;
            HSD_Material *values;
            saved->material = material;
            saved->alpha = 1.0F;
            if (material != 0)
                saved->render_mode = material->rendermode;
            else
                saved->render_mode = 0;
            values = 0;
            if (material != 0)
                values = material->mat;
            if (values != 0)
                saved->alpha = values->alpha;
            object = object != 0 ? object->next : 0;
            saved = saved->next;
        }
        if (joint->flags & JOBJ_INSTANCE)
            return;
        joint = joint != 0 ? joint->child : 0;
        pose = pose != 0 ? pose->child : 0;
        while (joint != 0 && pose != 0) {
            stCopyMatMobj(joint, pose);
            joint = joint != 0 ? joint->next : 0;
            pose = pose != 0 ? pose->next : 0;
        }
    }
}
