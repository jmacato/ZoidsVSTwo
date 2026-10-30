#include "model_archive.h"
#include "ground_object.h"

extern ModelArchive *GroundObj[6];
extern void LoadArcAnimModel2(ModelArchive *, HSD_JObj *, s32, s32);

void grdObjSetAnim(GroundObject *object, s32 animation)
{
    LoadArcAnimModel2(GroundObj[object->archive_slot], object->joint, 0, animation);
    object->animation = animation;
}
