#pragma dont_inline on
#include "jobj.h"
#include "dobj.h"
HSD_MObj *searchMObj(HSD_JObj *joint)
{
    HSD_DObj *object;
    HSD_MObj *material;
    while (joint != 0) {
        object = HSD_JObjGetDObj(joint);
        /* The original repeats this loop if a draw object has no material. */
        while (object != 0) {
            material = object != 0 ? object->mobj : 0;
            if (material != 0)
                return material;
        }
        if (joint->child != 0) {
            material = searchMObj(joint->child);
            if (material != 0)
                return material;
        }
        joint = joint->next;
    }
    return 0;
}
