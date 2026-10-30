#pragma readonly_strings on
#include "joint.h"

extern void __assert(const char *file, u32 line, const char *condition);
extern f32 grdCamChkS(HSD_JObj *joint);
extern void HSD_JObjDispAll(HSD_JObj *joint, void *mtx, u32 flags, u32 rendermode);

static inline f32 HSD_JObjGetScaleY(HSD_JObj *jobj)
{
    (jobj) ? (void)0 : __assert("jobj.h", 911, "jobj");
    return jobj->scale.y;
}

static inline HSD_JObj *HSD_JObjGetNext(HSD_JObj *jobj)
{
    return jobj ? jobj->next : 0;
}

void grdDispSubS(HSD_JObj *joint)
{
    f32 scale_y;
    s32 marker;
    s32 result;

    for (; joint != 0; joint = HSD_JObjGetNext(joint)) {
        scale_y = HSD_JObjGetScaleY(joint);
        marker = scale_y;
        if (marker == 1) {
            result = grdCamChkS(joint);
            HSD_JObjDispAll(joint, 0, HSD_TRSP_ALL, 0);
        }
    }
}
