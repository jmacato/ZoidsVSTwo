#ifndef ZOIDS_MODEL_ARCHIVE_H
#define ZOIDS_MODEL_ARCHIVE_H

#include "joint.h"

typedef struct ModelAnimation {
    HSD_JObjDesc *joint;
    struct HSD_AnimJoint **joint_animations;
    struct HSD_MatAnimJoint **material_animations;
    struct HSD_ShapeAnimJoint **shape_animations;
} ModelAnimation;

typedef struct ModelLightAnimation {
    struct HSD_LightDesc *light;
    struct HSD_LightAnim **animations;
} ModelLightAnimation;

/* "scene_data" public root of a model archive */
typedef struct ModelAnimationTable {
    ModelAnimation **models;
    union HSD_CObjDesc **camera;
    ModelLightAnimation **lights;
    struct HSD_FogDesc **fog;
} ModelAnimationTable;

typedef struct ModelArchive {
    char name[0x20];
    struct HSD_Archive *model_data;
    ModelAnimationTable *animations;
    void *owned_data;
    struct HSD_CObj *camera;
    struct HSD_LObj *lights;
    struct HSD_Fog *fog;
    s32 model_count;
    /* The loader allocates one extra pointer per model after this entry. */
    HSD_JObj *models[1];
} ModelArchive;

extern ModelArchive *LoadArc2(char *name);
extern void PurgeArc2(ModelArchive *archive);
extern void SetAnimArc2(ModelArchive *archive, s32 model, s32 selected);

#endif
