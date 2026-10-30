#include "model_archive.h"
#include "archive.h"
#pragma readonly_strings on

extern s32 sprintf(char *, const char *, ...);
extern void OSReport(const char *format, ...);
#pragma dont_inline on

typedef struct SJModel {
    HSD_JObjDesc *joint;
} SJModel;

typedef struct SJAnim {
    SJModel **models;
} SJAnim;

typedef struct SJZoid {
    char name[0x30];
    void *archive;
    SJAnim *animations;
    u8 pad_038[0x8];
    HSD_JObj *joint;
} SJZoid;

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

static HSD_JObj *searchJoint(SJZoid *zoid, const char *part)
{
    char name[256];
    HSD_JObjDesc *description;
    HSD_JObjDesc *root;

    if (*part == 0)
        return 0;
    sprintf(name, "%s_%s", zoid->name, part);
    description = (HSD_JObjDesc *)HSD_ArchiveGetPublicAddress((struct HSD_Archive *)zoid->archive, name);
    if (description == (void *)0) {
        OSReport("joint error [%s] \n ============", name);
        return 0;
    }
    root = zoid->animations->models[0]->joint;
    return search_PARTS(zoid->joint, root, description);
}
