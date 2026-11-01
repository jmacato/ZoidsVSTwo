#include "model_archive.h"
#include "lobj.h"

#pragma dont_inline on
void SetAnimLight(ModelLightAnimation *model, HSD_LObj *light, s32 selected)
{
    s32 i;
    if (model != 0) {
        if (model->animations == 0) {
            OSReport("light anim TBL non \n");
            return;
        }
        for (i = 0; i < selected + 1; i++) {
            if (model->animations[i] == (void *)0) {
                OSReport("light non anim  req=%d(in %d) \n", selected, i);
                return;
            }
        }
        HSD_LObjAddAnim(light, model->animations[selected]);
        HSD_LObjReqAnimAll(light, 0.0F);
    }
}
