#pragma readonly_strings on
#include "jobj.h"

#define JOBJ_HIDDEN 0x10
#define JOBJ_INSTANCE 0x1000
#define JOBJ_XLU 0x80000
#define JOBJ_ROOT_XLU 0x20000000
#define RENDER_XLU 0x40000000

void stMatSet(HSD_JObj *joint, JointPose *pose, f32 alpha)
{
    HSD_DObj *object = HSD_JObjGetDObj(joint);
    MaterialPose *saved = pose->materials;
    if (object == (void *)0)
        return;
    while (object != 0) {
        HSD_MObj *material = object != 0 ? object->mobj : 0;
        if (material != (void *)0) {
            f32 original_alpha = saved->alpha;
            if (alpha == 1.0)
                material->rendermode = saved->render_mode;
            else
                material->rendermode |= RENDER_XLU;
            if (material->mat != (void *)0)
                material->mat->alpha = alpha * original_alpha;
        }
        object = object != 0 ? object->next : 0;
        saved = saved->next;
    }
}
