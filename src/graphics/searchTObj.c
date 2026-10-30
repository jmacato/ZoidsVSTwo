#include "jobj.h"

#pragma dont_inline on
HSD_TObj *searchTObj(HSD_JObj *joint)
{
    HSD_DObj *object;
    HSD_MObj *material;
    HSD_TObj *texture;
    while (joint != 0) {
        object = HSD_JObjGetDObj(joint);
        while (object != 0) {
            material = object != 0 ? object->mobj : 0;
            if (material != 0) {
                texture = material->tobj;
                if (texture != 0)
                    return texture;
            }
            object = object != 0 ? object->next : 0;
        }
        if (joint->child != 0) {
            texture = searchTObj(joint->child);
            if (texture != 0)
                return texture;
        }
        joint = joint->next;
    }
    return 0;
}
