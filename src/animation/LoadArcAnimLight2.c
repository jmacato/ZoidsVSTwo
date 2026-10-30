#include "model_archive.h"
#include "lobj.h"
#pragma dont_inline on

void LoadArcAnimLight2(ModelArchive *archive, HSD_LObj *light, s32 selected)
{
    ModelAnimationTable *anim;
    ModelLightAnimation **models;
    if (archive != (void *)0 && (anim = archive->animations) != (void *)0 &&
        (models = archive->animations->lights) != (void *)0 && light != (void *)0) {
        register HSD_LObj *current = light;
        register HSD_LObj *next;
        s32 i;
        for (i = 0; models[i] != 0; i++) {
            SetAnimLight(models[i], current, selected);
            if (current == (void *)0) {
                next = (void *)0;
            } else {
                next = current->next;
            }
            current = next;
        }
    }
    anim = 0;
}
