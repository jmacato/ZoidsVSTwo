#include "model_archive.h"
#include "archive.h"
#pragma readonly_strings on

extern s32 sprintf(char *, const char *, ...);
#pragma dont_inline on

static HSD_JObj *search_PARTS(HSD_JObj *joint, HSD_JObjDesc *description, HSD_JObjDesc *target)
{
    HSD_JObj *match;

    while (joint != 0 && description != 0) {
        if (description == target) {
            return joint;
        }
        if (description->child != 0) {
            match = search_PARTS(joint->child, description->child, target);
            if (match != 0) {
                return match;
            }
        }
        joint = joint->next;
        description = description->next;
    }
    return 0;
}

HSD_JObj *searchJobjJoint(ModelArchive *archive, HSD_JObj *joint, char *name)
{
    char symbol[256];
    HSD_JObjDesc *description;
    HSD_JObj *result;
    if (*name == 0)
        return 0;
    sprintf(symbol, "%s_%s", archive->name, name);
    description = HSD_ArchiveGetPublicAddress(archive->model_data, symbol);
    if (description == (void *)0)
        return 0;
    result = search_PARTS(joint, archive->animations->models[0]->joint, description);
    return result;
}

HSD_JObj *searchArcJoint2(ModelArchive *archive, s32 model, char *name)
{
    char symbol[256];
    HSD_JObjDesc *description;
    HSD_JObj *result;
    if (*name == 0)
        return 0;
    if (archive == (void *)0)
        return 0;
    if (archive->model_count <= model)
        return 0;
    sprintf(symbol, "%s_%s", archive->name, name);
    description = HSD_ArchiveGetPublicAddress(archive->model_data, symbol);
    if (description == (void *)0)
        return 0;
    result = search_PARTS(archive->models[model],
                          archive->animations->models[model]->joint, description);
    return result;
}
