#pragma readonly_strings on
#include "model_archive.h"
#include "archive.h"
#include "jobj.h"
#include "fog.h"

extern u32 strlen(const char *text);
extern char *strcpy(char *dst, const char *src);
extern HSD_Archive *TEA_USBFileArchiveLoad(char *name, void **data);
extern struct HSD_LObj *LoadArcLight(ModelArchive *archive);
extern struct HSD_CObj *HSD_CObjLoadDesc(union HSD_CObjDesc *desc);

s32 LoadArcUSB(ModelArchive *archive, char *name)
{
    char *base;
    char *p;
    s32 length;
    s32 i;
    s32 pad = 0;
    void *data;

    p = name;
    base = name;
    for (; *p != 0; p++) {
        if (*p == '/')
            base = p + 1;
    }
    length = strlen(base);
    strcpy(archive->name, base);
    if (length > 4)
        archive->name[length - 4] = 0;
    archive->owned_data = 0;
    archive->model_data = TEA_USBFileArchiveLoad(name, &data);
    if (archive->model_data != 0)
        archive->owned_data = data;
    HSD_ArchiveLoadDepends(archive->model_data, 0, 0, 0);
    HSD_ArchiveResolveReference(archive->model_data, 0);
    archive->animations = HSD_ArchiveGetPublicAddress(archive->model_data, "scene_data");
    archive->lights = LoadArcLight(archive);
    i = 0;
    if (archive->animations->models != 0) {
        for (; archive->animations->models[i] != 0; i++) {
        }
    }
    archive->model_count = i;
    if (archive->animations->models != 0) {
        archive->models[0] = HSD_JObjLoadJoint((*archive->animations->models)->joint);
        HSD_JObjAddAnimAll(archive->models[0],
                           (*archive->animations->models)->joint_animations ? *(*archive->animations->models)->joint_animations : 0,
                           (*archive->animations->models)->material_animations ? *(*archive->animations->models)->material_animations : 0,
                           (*archive->animations->models)->shape_animations ? *(*archive->animations->models)->shape_animations : 0);
    }
    archive->camera = HSD_CObjLoadDesc(*archive->animations->camera);
    if (archive->animations->fog != 0 && *archive->animations->fog != 0)
        archive->fog = HSD_FogLoadDesc(*archive->animations->fog);
    return 1;
}
