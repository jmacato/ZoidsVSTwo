#include "jobj.h"
#include "model_archive.h"

extern void SetAnimModel(ModelAnimation *model, HSD_JObj *joint, s32 selected);

void LoadArcAnimModel2(ModelArchive *archive, HSD_JObj *joint, s32 model, s32 selected)
{
    ModelAnimationTable *table;
    ModelAnimation **models;
    if (archive != (void *)0) {
        table = archive->animations;
        if (table != (void *)0) {
            models = table->models;
            if (archive->model_count > model) {
                if (models != (void *)0 && joint != (void *)0) {
                    SetAnimModel(models[model], joint, selected);
                }
            }
        }
    }
}
