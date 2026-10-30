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

void grdDispSubXLUS(HSD_JObj *joint)
{
    f32 scale_y;
    s32 marker;
    s32 result;

    for (; joint != 0; joint = HSD_JObjGetNext(joint)) {
        scale_y = HSD_JObjGetScaleY(joint);
        marker = scale_y;
        /* 1 is plain ground; 2..6 place map_obj01..05 (GroundObjLoadSub2) */
        if (marker == 1) {
            result = grdCamChkS(joint);
            HSD_JObjDispAll(joint, 0, HSD_TRSP_XLU, 0);
            HSD_JObjDispAll(joint, 0, HSD_TRSP_TEXEDGE, 0);
        }
    }
}
