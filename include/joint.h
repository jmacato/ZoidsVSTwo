#ifndef ZOIDS_JOINT_H
#define ZOIDS_JOINT_H

#include "types.h"

typedef struct HSD_JObj {
    struct HSD_ClassInfo *class_info;
    u16 reference_count;
    u16 individual_reference_count;
    struct HSD_JObj *next;
    struct HSD_JObj *parent;
    struct HSD_JObj *child;
    u32 flags;
    void *display_object;
    f32 rotation[4];
    Vec3 scale;
    Vec3 translation;
    f32 matrix[3][4];
    Vec3 *scale_pointer;
    void *envelope_matrix;
    struct HSD_AObj *animation;
    void *render_object;
    u32 object_id;
} HSD_JObj;

/* Link names follow https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/jobj.h. */

typedef struct MaterialPose {
    struct MaterialPose *next;
    struct HSD_MObj *material;
    f32 alpha;
    u32 render_mode;
} MaterialPose;

typedef struct JointPose {
    struct JointPose *next;
    struct JointPose *child;
    MaterialPose *materials;
    f32 rotation[4];
    Vec3 scale;
    Vec3 translation;
    f32 matrix[3][4];
    s32 animation_index;
    u32 saved_flags;
} JointPose;

typedef struct HSD_JObjDesc {
    const char *name;
    u32 flags;
    struct HSD_JObjDesc *child;
    struct HSD_JObjDesc *next;
    union {
        struct HSD_DObjDesc *dobjdesc;
        void *spline;
        struct HSD_SList *ptcl;
    } u;
    Vec3 rotation;
    Vec3 scale;
    Vec3 position;
    f32 (*matrix)[4];
    struct HSD_RObjDesc *render_desc;
} HSD_JObjDesc;

/* trsp_mask argument of HSD_JObjDisp and HSD_JObjDispAll */
#define HSD_TRSP_OPA 1
#define HSD_TRSP_XLU 2
#define HSD_TRSP_TEXEDGE 4
#define HSD_TRSP_ALL 7

/* JObjUpdateFunc track types after HSD_A_J_SETFLOAT9 (39, src/sdk/joint/joint.c) */
#define JOBJ_UPDATE_PTCL 40
#define JOBJ_UPDATE_SOUND 41
#define JOBJ_UPDATE_PTCL_TARGET 42
#define JOBJ_UPDATE_MTX_COL0 50
#define JOBJ_UPDATE_MTX_COL1 51
#define JOBJ_UPDATE_MTX_COL2 52
#define JOBJ_UPDATE_MTX_COL3 53
#define JOBJ_UPDATE_MTX_GET_SRT 54
#define JOBJ_UPDATE_MTX_GET_ROTATION 55
#define JOBJ_UPDATE_MTX_GET_TRANSLATE 56
#define JOBJ_UPDATE_MTX_GET_SCALE 57

#endif
