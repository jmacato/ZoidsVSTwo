#include "jobj.h"

#pragma dont_inline on
HSD_TObj *searchTObjN(HSD_JObj *joint, s32 *index)
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
                while (texture != 0) {
                    if (*index == 0)
                        return texture;
                    --*index;
                    texture = texture->next;
                }
            }
            object = object != 0 ? object->next : 0;
        }
        if (joint->child != 0) {
            texture = searchTObjN(joint->child, index);
            while (texture != 0) {
                if (*index == 0)
                    return texture;
                --*index;
                texture = texture->next;
            }
        }
        joint = joint->next;
    }
    return 0;
}
