#include "model_archive.h"
#include "jobj.h"
#pragma dont_inline on

HSD_JObj *CopyJoint2(ModelArchive *archive, s32 model)
{
    HSD_JObjDesc *joint;
    if (archive == (void *)0)
        return 0;
    if (archive->model_data == (void *)0)
        return 0;
    if (archive->animations->models == (void *)0)
        return 0;
    if (model >= archive->model_count)
        return 0;
    joint = archive->animations->models[model]->joint;
    return HSD_JObjLoadJoint(joint);
}
