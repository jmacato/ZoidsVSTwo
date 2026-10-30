#pragma auto_inline off
#include "joint.h"
#include "gx_tev.h"
#include "dobj.h"

extern HSD_DObj *HSD_JObjGetDObj(HSD_JObj *joint);

static inline HSD_MObj *HSD_DObjGetMObj(HSD_DObj *dobj)
{
    return dobj ? dobj->mobj : 0;
}

void jobjAmbient(HSD_JObj *joint, GXColor *color)
{
    register HSD_Material *mat;
    HSD_DObj *dobj;
    HSD_MObj *mobj;
    GXColor child;

    for (; joint != 0; joint = joint ? joint->next : 0) {
        for (dobj = HSD_JObjGetDObj(joint); dobj != 0; dobj = dobj->next) {
            mobj = HSD_DObjGetMObj(dobj);
            if (mobj != 0 && (mat = mobj->mat) != 0) {
                mat->ambient.r = color->r;
                mat->ambient.g = color->g;
                mat->ambient.b = color->b;
                mat->ambient.a = color->a;
            }
        }
        jobjAmbient(joint->child, (child = *color, &child));
    }
}
