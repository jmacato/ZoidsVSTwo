#ifndef ZOIDS_COBJ_H
#define ZOIDS_COBJ_H
/* Layout follows
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/cobj.h. */
#include "object.h"
#include "wobj.h"
#include "matrix.h"
#include "video.h"
#include "gx_transform.h"
typedef f32 Mtx44[4][4];
typedef struct HSD_CObj HSD_CObj;
typedef struct HSD_CObjInfo HSD_CObjInfo;
typedef struct HSD_CameraDescCommon HSD_CameraDescCommon;
typedef struct HSD_CameraDescFrustum HSD_CameraDescFrustum;
typedef struct HSD_CameraDescPerspective HSD_CameraDescPerspective;
typedef union HSD_CObjDesc HSD_CObjDesc;
#define PROJ_PERSPECTIVE 1
#define PROJ_FRUSTUM 2
#define PROJ_ORTHO 3
#define COBJ_USE_UP_VECTOR 0x1
#define COBJ_MTX_DIRTY 0x40000000
#define COBJ_INV_MTX_DIRTY 0x80000000
typedef struct _Scissor {
    u16 left;
    u16 right;
    u16 top;
    u16 bottom;
} Scissor;

typedef struct _HSD_RectS16 {
    s16 xmin;
    s16 xmax;
    s16 ymin;
    s16 ymax;
} HSD_RectS16;

typedef struct _HSD_RectF32 {
    f32 xmin;
    f32 xmax;
    f32 ymin;
    f32 ymax;
} HSD_RectF32;

struct HSD_CObj {
    /*  +0 */
    HSD_Obj parent;
    /*  +8 */ u32 flags;
    /*  +C */ HSD_RectF32 viewport;
    /* +1C */ Scissor scissor;
    /* +24 */ HSD_WObj *eyepos;
    /* +28 */ HSD_WObj *interest;
    union HSD_CObj_u {
        /* +2C */ f32 roll;
        /* +2C */ Vec3 up;
    } u;
    /* +38 */ f32 near;
    /* +3C */ f32 far;
    union HSD_CObj_projection_param {
        struct HSD_CObj_projection_param_perspective {
            f32 fov;
            f32 aspect;
        } perspective;

        struct HSD_CObj_projection_param_frustum {
            f32 top;
            f32 bottom;
            f32 left;
            f32 right;
        } frustum;

        struct HSD_CObj_projection_param_ortho {
            f32 top;
            f32 bottom;
            f32 left;
            f32 right;
        } ortho;
    } projection_param;
    /* +50 */ u8 projection_type;
    /* +54 */ Mtx view_mtx;
    /* +84 */ HSD_AObj *aobj;
    /* +88 */ Mtx *proj_mtx;
};

struct HSD_CameraDescCommon {
    char *class_name;
    u16 flags;
    u16 projection_type;
    HSD_RectS16 viewport;
    Scissor scissor;
    HSD_WObjDesc *eyepos;
    HSD_WObjDesc *interest;
    f32 roll;
    Vec3 *up_vector;
    f32 nnear;
    f32 ffar;
};

struct HSD_CameraDescFrustum {
    char *class_name;
    u16 flags;
    u16 projection_type;
    HSD_RectS16 viewport;
    Scissor scissor;
    HSD_WObjDesc *eyepos;
    HSD_WObjDesc *interest;
    f32 roll;
    Vec3 *up_vector;
    f32 nnear;
    f32 ffar;
    f32 top;
    f32 bottom;
    f32 left;
    f32 right;
};

struct HSD_CameraDescPerspective {
    char *class_name;
    u16 flags;
    u16 projection_type;
    HSD_RectS16 viewport;
    Scissor scissor;
    HSD_WObjDesc *eyepos;
    HSD_WObjDesc *interest;
    f32 roll;
    Vec3 *up_vector;
    f32 nnear;
    f32 ffar;
    f32 fov;
    f32 aspect;
};

union HSD_CObjDesc {
    char *class_name;
    HSD_CameraDescCommon common;
    HSD_CameraDescFrustum frustum;
    HSD_CameraDescFrustum ortho;
    HSD_CameraDescPerspective perspective;
};

struct HSD_CObjInfo {
    HSD_ClassInfo parent;
    int (*load)(HSD_CObj *cobj, HSD_CObjDesc *desc);
    AnimationUpdate update;
};

struct HSD_CameraAnim {
    HSD_AObjDesc *aobjdesc;
    HSD_WObjAnim *eye_anim;
    HSD_WObjAnim *interest_anim;
};

extern HSD_CObjInfo hsdCObj;
extern s32 HSD_CObjSetCurrent(HSD_CObj *cobj);
extern void HSD_CObjEndCurrent(void);
extern void HSD_CObjGetInterest(HSD_CObj *cobj, Vec3 *interest);
extern void HSD_CObjSetInterest(HSD_CObj *cobj, Vec3 *interest);
extern void HSD_CObjGetEyePosition(HSD_CObj *cobj, Vec3 *position);
extern void HSD_CObjSetEyePosition(HSD_CObj *cobj, Vec3 *position);
extern int HSD_CObjGetEyeVector(HSD_CObj *cobj, Vec3 *eye);
extern float HSD_CObjGetEyeDistance(HSD_CObj *cobj);
extern int HSD_CObjGetUpVector(HSD_CObj *cobj, Vec3 *up);
extern void HSD_CObjSetUpVector(HSD_CObj *cobj, Vec3 *up);
extern void HSD_CObjSetMtxDirty(HSD_CObj *cobj);
extern s32 HSD_CObjMtxIsDirty(HSD_CObj *cobj);
extern void HSD_CObjGetViewingMtx(HSD_CObj *cobj, Mtx mtx);
extern MtxPtr HSD_CObjGetInvViewingMtxPtrDirect(HSD_CObj *cobj);
extern MtxPtr HSD_CObjGetViewingMtxPtr(HSD_CObj *cobj);
extern MtxPtr HSD_CObjGetInvViewingMtxPtr(HSD_CObj *cobj);
extern void HSD_CObjSetRoll(HSD_CObj *cobj, float roll);
extern void HSD_CObjSetFov(HSD_CObj *cobj, float fov);
extern void HSD_CObjSetAspect(HSD_CObj *cobj, float aspect);
extern float HSD_CObjGetTop(HSD_CObj *cobj);
extern float HSD_CObjGetBottom(HSD_CObj *cobj);
extern float HSD_CObjGetLeft(HSD_CObj *cobj);
extern float HSD_CObjGetRight(HSD_CObj *cobj);
extern float HSD_CObjGetNear(HSD_CObj *cobj);
extern void HSD_CObjSetNear(HSD_CObj *cobj, float near);
extern float HSD_CObjGetFar(HSD_CObj *cobj);
extern void HSD_CObjSetFar(HSD_CObj *cobj, float far);
extern void HSD_CObjGetScissor(HSD_CObj *cobj, Scissor *scissor);
extern void HSD_CObjSetScissor(HSD_CObj *cobj, Scissor *scissor);
extern void HSD_CObjSetScissorx4(HSD_CObj *cobj, u16 left, u16 right, u16 top,
                                 u16 bottom);
extern void HSD_CObjSetViewport(HSD_CObj *cobj, HSD_RectS16 *viewport);
extern void HSD_CObjSetViewportfx4(HSD_CObj *cobj, float left, float right, float top,
                                   float bottom);
extern int HSD_CObjGetProjectionType(HSD_CObj *cobj);
extern void HSD_CObjSetProjectionType(HSD_CObj *cobj, u32 proj_type);
extern void HSD_CObjSetFrustum(HSD_CObj *cobj, float top, float bottom, float left,
                               float right);
extern void HSD_CObjSetOrtho(HSD_CObj *cobj, float top, float bottom, float left,
                             float right);
extern void HSD_CObjSetFlags(HSD_CObj *cobj, u32 flags);
extern void HSD_CObjClearFlags(HSD_CObj *cobj, u32 flags);
extern HSD_CObj *HSD_CObjGetCurrent(void);
extern HSD_CObj *HSD_CObjAlloc(void);
extern HSD_CObj *HSD_CObjLoadDesc(HSD_CObjDesc *desc);

#endif
