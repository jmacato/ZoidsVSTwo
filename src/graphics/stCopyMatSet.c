#include "zoid.h"
#include "dobj.h"

#define JOBJ_INSTANCE 0x1000

extern HSD_DObj *HSD_JObjGetDObj(HSD_JObj *joint);

#pragma dont_inline on
void stCopyMatSet(HSD_JObj *joint, HSD_MObj *material)
{
    HSD_DObj *object;
    if (joint != (void *)0) {
        for (object = HSD_JObjGetDObj(joint); object != 0; object = object->next) {
            object->mobj = material;
        }
        if (joint->flags & JOBJ_INSTANCE)
            return;
        for (joint = joint->child; joint != 0; joint = joint->next) {
            stCopyMatSet(joint, material);
        }
    }
}
