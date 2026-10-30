#include "aobj.h"

extern void HSD_FObjRemoveAll(struct HSD_FObj *functions);
extern void HSD_JObjUnref(HSD_JObj *joint);
extern void HSD_AObjFree(HSD_AObj *animation);

void HSD_AObjRemove(HSD_AObj *animation)
{
    if (animation != 0) {
        if (animation != 0) {
            if (animation->functions != 0) {
                HSD_FObjRemoveAll(animation->functions);
            }
            animation->functions = 0;
        }
        if (animation != 0) {
            if (animation->joint != 0) {
                HSD_JObjUnref(animation->joint);
            }
            animation->joint = 0;
        }
        HSD_AObjFree(animation);
    }
}
