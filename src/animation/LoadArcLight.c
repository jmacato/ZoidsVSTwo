#pragma readonly_strings on
#include "model_archive.h"
#include "lobj.h"

static inline void HSD_LObjSetNext(HSD_LObj *lobj, HSD_LObj *next)
{
    lobj ? (void)0 : __assert("lobj.h", 301, "lobj");
    lobj->next = next;
}

HSD_LObj *LoadArcLight(ModelArchive *archive)
{
    HSD_LObj *head = 0;
    if (archive->animations->lights != 0) {
        s32 i;
        HSD_LObj *tail = 0;
        s32 dead = 0;
        for (i = 0; archive->animations->lights[i] != 0; i++) {
            HSD_LObj *light = HSD_LObjLoadDesc(archive->animations->lights[i]->light);
            if (archive->animations->lights[i]->animations != 0)
                HSD_LObjAddAnim(light, archive->animations->lights[i]->animations[0]);
            if (tail == 0)
                head = tail = light;
            else {
                HSD_LObjSetNext(tail, light);
                tail = light;
            }
            dead = 0;
        }
        HSD_LObjReqAnimAll(head, 0.0F);
    }
    return head;
}
