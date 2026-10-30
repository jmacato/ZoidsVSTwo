#include "jobj.h"
#include "model_archive.h"
#pragma dont_inline on

extern void SetAnimModel(ModelAnimation *model, HSD_JObj *joint, s32 selected);

void LoadArcAnimModel(ModelArchive *archive, HSD_JObj *joint, s32 model, s32 selected)
{
    register ModelAnimation **models;
    ModelAnimationTable *anim;
    if (archive != (void *)0 && (anim = archive->animations) != (void *)0 &&
        (models = archive->animations->models) != (void *)0 && joint != (void *)0) {
        SetAnimModel(models[model], joint, selected);
    }
    anim = 0;
}
