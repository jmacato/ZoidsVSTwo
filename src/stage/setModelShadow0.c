#include "joint.h"
#include "dobj.h"

#define RENDER_SHADOW (1 << 26)

extern HSD_DObj *HSD_JObjGetDObj(HSD_JObj *joint);

void setModelShadow0(HSD_JObj *joint)
{
    HSD_DObj *dobj;
    HSD_MObj *mobj;

    for (dobj = HSD_JObjGetDObj(joint); dobj != 0; dobj = dobj ? dobj->next : 0) {
        mobj = dobj ? dobj->mobj : 0;
        mobj->rendermode |= RENDER_SHADOW;
    }
}
