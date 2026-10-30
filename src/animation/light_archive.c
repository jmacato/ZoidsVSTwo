#include "model_archive.h"
#include "lobj.h"

static inline void HSD_LObjSetNext(HSD_LObj *lobj, HSD_LObj *next)
{
    if (!lobj)
        __assert("lobj.h", 301, "lobj");
    lobj->next = next;
}



HSD_LObj *LoadArc2Light(ModelArchive *archive, s32 selected)
{
    HSD_LObj *light;
    if (archive == 0)
        return 0;
    if (archive->animations == 0)
        return 0;
    if (archive->animations->lights == 0)
        return 0;
    light = HSD_LObjLoadDesc(archive->animations->lights[selected]->light);
    if (archive->animations->lights[selected]->animations != 0) {
        HSD_LObjAddAnim(light, archive->animations->lights[selected]->animations[0]);
        HSD_LObjReqAnimAll(light, 0.0F);
    }
    return light;
}

#pragma dont_inline on



