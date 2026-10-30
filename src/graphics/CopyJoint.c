#include "model_archive.h"
#include "jobj.h"

#pragma dont_inline on
HSD_JObj *CopyJoint(ModelArchive *archive)
{
    HSD_JObj *joint;
    if (archive->animations->models == (void *)0)
        return 0;
    joint = HSD_JObjLoadJoint(archive->animations->models[0]->joint);
    HSD_JObjAddAnimAll(
        joint,
        archive->animations->models[0]->joint_animations != 0
            ? archive->animations->models[0]->joint_animations[0]
            : 0,
        archive->animations->models[0]->material_animations != 0
            ? archive->animations->models[0]->material_animations[0]
            : 0,
        archive->animations->models[0]->shape_animations != 0
            ? archive->animations->models[0]->shape_animations[0]
            : 0);
    return joint;
}
