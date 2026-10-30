#include "joint.h"

extern s32 grdCamChkR(HSD_JObj *joint);
extern void HSD_JObjDisp(HSD_JObj *joint, void *mtx, u32 flags, u32 rendermode);
extern void grdDispSubXLUS(HSD_JObj *joint);

void grdDispSubXLU(HSD_JObj *joint)
{
    for (; joint != 0; joint = joint->next) {
        if (grdCamChkR(joint) != 0) {
            HSD_JObjDisp(joint, 0, HSD_TRSP_XLU, 0);
            HSD_JObjDisp(joint, 0, HSD_TRSP_TEXEDGE, 0);
            if (joint->child != 0)
                grdDispSubXLUS(joint->child);
        }
    }
}
