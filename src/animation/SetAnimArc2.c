#include "jobj.h"
#include "model_archive.h"

extern void LoadArcAnimModel2(ModelArchive *archive, HSD_JObj *joint, s32 model, s32 selected);

void SetAnimArc2(ModelArchive *archive, s32 model, s32 selected)
{
    if (archive != (void *)0 && archive->model_count > model && archive->models[model] != 0) {
        HSD_JObjRemoveAnimAll(archive->models[model]);
        LoadArcAnimModel2(archive, archive->models[model], model, selected);
    }
}
