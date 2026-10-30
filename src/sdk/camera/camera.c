/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/cobj.c. */
#include "cobj.h"
extern void CObjInfoInit(void);
HSD_CObjInfo hsdCObj = {{{CObjInfoInit}}};
#define NULL 0
#define true 1
#define false 0
#define FLT_MIN 1.17549435e-38F
#define HSD_COBJ(o) ((HSD_CObj *)(o))
#define HSD_CLASS_INFO(i) ((HSD_ClassInfo *)(i))
#define HSD_OBJECT_PARENT_INFO(i) ((i)->parent.head.parent)
#define HSD_OBJECT_METHOD(o) (((HSD_Class *)(o))->class_info)
#define HSD_COBJ_METHOD(o) ((HSD_CObjInfo *)HSD_OBJECT_METHOD(o))
#define HSD_ASSERT(line, condition)                                                    \
    do {                                                                               \
        if ((condition) == 0)                                                          \
            __assert("cobj.c", line, #condition);                                      \
    } while (0)
#define DegToRad(a) ((a) * 0.01745329252F)
#define MTXPerspective C_MTXPerspective
#define MTXFrustum C_MTXFrustum
#define MTXOrtho C_MTXOrtho
#define VECSubtract PSVECSubtract
#define VECMag PSVECMag

/* Shared SDK state remains at its original addresses. */
extern s32 scale_width;
extern s32 scale_height;
extern HSD_ClassInfo *default_class;
extern HSD_CObj *current;
extern void __assert(const char *, u32, const char *);
extern f32 tanf(f32);
extern void OSReport(const char *format, ...);
extern f32 PSVECDotProduct(Vec3 *, Vec3 *);
extern f32 atan2f(f32, f32);
static inline GXRenderModeObj *HSD_VIGetRenderMode(void)
{
    return &HSD_VIData.current.vi.rmode;
}
extern void GXSetViewport(f32, f32, f32, f32, f32, f32);
extern void GXSetViewportJitter(f32, f32, f32, f32, f32, f32, u32);
extern void GXSetScissor(u32, u32, u32, u32);
extern void GXSetProjection(Mtx44, GXProjectionType);
extern u32 VIGetNextField(void);
extern void C_MTXPerspective(Mtx44, f32, f32, f32, f32);
extern void C_MTXFrustum(Mtx44, f32, f32, f32, f32, f32, f32);
extern void C_MTXOrtho(Mtx44, f32, f32, f32, f32, f32, f32);
extern void C_MTXLookAt(Mtx, Vec3 *, Vec3 *, Vec3 *);
extern void PSMTXCopy(Mtx, Mtx);
extern s32 PSMTXInverse(Mtx, Mtx);
extern Mtx *HSD_MtxAlloc(void);
extern void HSD_MtxFree(Mtx *);
extern void HSD_WObjGetPosition(HSD_WObj *, Vec3 *);
extern void _HSD_ZListSort(void);
extern void _HSD_ZListDisp(void);

static void CObjUpdateFunc(void *obj, int type, AnimationValue *val);
static GXProjectionType makeProjectionMtx(HSD_CObj *cobj, Mtx44 mtx);
static s32 setupNormalCamera(HSD_CObj *cobj);
static s32 setupTopHalfCamera(HSD_CObj *cobj);
static s32 setupBottomHalfCamera(HSD_CObj *cobj);
static int CObjInit(HSD_Class *o);
static void CObjAmnesia(HSD_ClassInfo *info);

extern void _HSD_ZListClear(void);
extern void HSD_Panic(const char *, u32, const char *);

#pragma push
#pragma force_active on
static void CObjUpdateFunc(void *obj, int type, AnimationValue *val)
{
    HSD_CObj *cobj = obj;
    Vec3 vec;
    (void)"ref_width > 0";
    (void)"ref_height > 0";

    if (cobj == NULL) {
        return;
    }

    switch (type) {
    case 1:
        HSD_CObjGetEyePosition(cobj, &vec);
        vec.x = val->scalar;
        HSD_CObjSetEyePosition(cobj, &vec);
        break;
    case 2:
        HSD_CObjGetEyePosition(cobj, &vec);
        vec.y = val->scalar;
        HSD_CObjSetEyePosition(cobj, &vec);
        break;
    case 3:
        HSD_CObjGetEyePosition(cobj, &vec);
        vec.z = val->scalar;
        HSD_CObjSetEyePosition(cobj, &vec);
        break;
    case 5:
        HSD_CObjGetInterest(cobj, &vec);
        vec.x = val->scalar;
        HSD_CObjSetInterest(cobj, &vec);
        break;
    case 6:
        HSD_CObjGetInterest(cobj, &vec);
        vec.x = val->scalar;
        HSD_CObjSetInterest(cobj, &vec);
        break;
    case 7:
        HSD_CObjGetInterest(cobj, &vec);
        vec.x = val->scalar;
        HSD_CObjSetInterest(cobj, &vec);
        break;
    case 9:
        HSD_CObjSetRoll(cobj, val->scalar);
        break;
    case 10:
        HSD_CObjSetFov(cobj, val->scalar);
        break;
    case 11:
        HSD_CObjSetNear(cobj, val->scalar);
        break;
    case 12:
        HSD_CObjSetFar(cobj, val->scalar);
        break;
    }
}
#pragma pop

s32 HSD_CObjSetCurrent(HSD_CObj *cobj)
{
    HSD_RenderPass render_pass;
    s32 result;
    Mtx44 mtx;
    Vec3 interest, up_vec, eyepos;
    f32 up_x;
    f32 up_y;
    f32 up_z;

    if (cobj == NULL) {
        return false;
    }
    render_pass = HSD_GetCurrentRenderPass();
    _HSD_ZListClear();
    current = cobj;
    switch (render_pass) {
    case HSD_RP_OFFSCREEN:

        GXSetViewport(cobj->viewport.xmin, cobj->viewport.ymin,
                      cobj->viewport.xmax - cobj->viewport.xmin,
                      cobj->viewport.ymax - cobj->viewport.ymin, 0.0f, 1.0f);
        GXSetScissor(cobj->scissor.left, cobj->scissor.top,
                     cobj->scissor.right - cobj->scissor.left,
                     cobj->scissor.bottom - cobj->scissor.top);
        GXSetProjection(mtx, makeProjectionMtx(cobj, mtx));

        result = true;
        break;
    case HSD_RP_SCREEN:
        result = setupNormalCamera(cobj);
        break;
    case HSD_RP_TOPHALF:
        result = setupTopHalfCamera(cobj);
        break;
    case HSD_RP_BOTTOMHALF:
        result = setupBottomHalfCamera(cobj);
        break;
    default:
        HSD_Panic("cobj.c", 663, "unkown type of render pass.\n");
        return false;
    }
    if (!result) {
        return false;
    } else {

        if (!(cobj->flags & 2) && HSD_CObjMtxIsDirty(cobj)) {
            HSD_CObjGetEyePosition(cobj, &eyepos);
            if (!HSD_CObjGetUpVector(cobj, &up_vec)) {
                up_x = 0.0F;
                up_vec.x = up_x;
                up_y = 1.0F;
                up_vec.y = up_y;
                up_z = 0.0F;
                up_vec.z = up_z;
            }
            HSD_CObjGetInterest(cobj, &interest);
            C_MTXLookAt(cobj->view_mtx, &eyepos, &up_vec, &interest);
            cobj->eyepos->flags &= ~WOBJ_POSITION_DIRTY;
            cobj->interest->flags &= ~WOBJ_POSITION_DIRTY;
            HSD_CObjClearFlags(cobj, COBJ_MTX_DIRTY);
            HSD_CObjSetFlags(cobj, COBJ_INV_MTX_DIRTY);
        }

        return true;
    }
}

extern f64 __frsqrte(f64);
extern void PSVECSubtract(Vec3 *, Vec3 *, Vec3 *);
extern void PSMTXRotAxisRad(Mtx, Vec3 *, f32);
extern void PSMTXMultVecSR(Mtx, Vec3 *, Vec3 *);
static int vec_normalize_check(Vec3 *, Vec3 *);

static inline HSD_WObj *HSD_CObjGetInterestWObj(HSD_CObj *cobj)
{
    cobj ? (void)0 : __assert("cobj.c", 700, "cobj");
    return cobj->interest;
}
static inline HSD_WObj *HSD_CObjGetEyePositionWObj(HSD_CObj *cobj)
{
    cobj ? (void)0 : __assert("cobj.c", 724, "cobj");
    return cobj->eyepos;
}

static inline void camera_eye_position(HSD_CObj *cobj, Vec3 *position)
{
    cobj ? (void)0 : __assert("cobj.c", 772, "cobj");
    cobj ? (void)0 : __assert("cobj.c", 724, "cobj");
    HSD_WObjGetPosition(cobj->eyepos, position);
}

static inline void camera_interest_position(HSD_CObj *cobj, Vec3 *interest)
{
    cobj ? (void)0 : __assert("cobj.c", 748, "cobj");
    cobj ? (void)0 : __assert("cobj.c", 700, "cobj");
    HSD_WObjGetPosition(cobj->interest, interest);
}

static inline f32 camera_vector_x(Vec3 *vector)
{
    return vector->x;
}

static inline f64 camera_abs_y(Vec3 *vector)
{
    return __fabsf(vector->y);
}

/* Volatile stores preserve the original single-precision rounding. */
static inline f32 camera_root_into(f32 value, volatile f32 *storage)
{
    if (value > 0.0F) {
        f64 estimate = __frsqrte(value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        *storage = value * estimate;
        value = *storage;
    }
    return value;
}

static inline s32 decrement_reference(void *object)
{
    s32 result;
    u16 *count = &((HSD_Obj *)object)->reference_count;
    if ((result = (*count == (u16)-1)))
        return result;
    return (*count)-- == 0;
}

static inline void HSD_WObjUnref(HSD_WObj *object)
{
    if (object != NULL && decrement_reference(object)) {
        if (object != NULL) {
            HSD_OBJECT_METHOD(object)->release((HSD_Class *)object);
            HSD_OBJECT_METHOD(object)->destroy((HSD_Class *)object);
        }
    }
}

/* MWCC needs different local orders for inline and standalone roll conversion. */
static inline int camera_roll_to_up(HSD_CObj *cobj, Vec3 *up, float roll)
{
    s32 have_eye;
    volatile f32 root_values[2];
    Vec3 position, interest;
    Vec3 eye;
    Vec3 v0;
    Vec3 v1;
    Mtx m;

    if (!cobj || !cobj->eyepos || !cobj->interest)
        have_eye = 0;
    else {
        camera_eye_position(cobj, &position);
        camera_interest_position(cobj, &interest);
        PSVECSubtract(&interest, &position, &eye);
        have_eye = !vec_normalize_check(&eye, &eye);
    }
    if (!have_eye) {
        return 0;
    }
    if (1.0 - camera_abs_y(&eye) < 0.0001) {
        v0.x = camera_root_into(eye.y * eye.y + eye.z * eye.z, &root_values[1]);
        v0.y = eye.y * (-camera_vector_x(&eye) / v0.x);
        v0.z = eye.z * (-eye.x / v0.x);
    } else {
        v0.y = camera_root_into(eye.x * eye.x + eye.z * eye.z, &root_values[0]);
        v0.x = eye.x * (-eye.y / v0.y);
        v0.z = eye.z * (-eye.y / v0.y);
    }
    PSMTXRotAxisRad(m, &eye, -roll);
    PSMTXMultVecSR(m, &v0, &v1);
    PSVECNormalize(&v1, up);
    return 1;
}

#pragma push
#pragma force_active on
static int roll2upvec(HSD_CObj *cobj, Vec3 *up, float roll)
{
    s32 have_eye;
    Vec3 eye;
    Vec3 v0;
    Vec3 v1;
    Mtx m;
    Vec3 interest, position;
    volatile f32 root_values[2];

    if (!cobj || !cobj->eyepos || !cobj->interest)
        have_eye = 0;
    else {
        camera_eye_position(cobj, &position);
        camera_interest_position(cobj, &interest);
        PSVECSubtract(&interest, &position, &eye);
        have_eye = !vec_normalize_check(&eye, &eye);
    }
    if (!have_eye) {
        return 0;
    }
    if (1.0 - camera_abs_y(&eye) < 0.0001) {
        v0.x = camera_root_into(eye.y * eye.y + eye.z * eye.z, &root_values[1]);
        v0.y = eye.y * (-camera_vector_x(&eye) / v0.x);
        v0.z = eye.z * (-eye.x / v0.x);
    } else {
        v0.y = camera_root_into(eye.x * eye.x + eye.z * eye.z, &root_values[0]);
        v0.x = eye.x * (-eye.y / v0.y);
        v0.z = eye.z * (-eye.y / v0.y);
    }
    PSMTXRotAxisRad(m, &eye, -roll);
    PSMTXMultVecSR(m, &v0, &v1);
    PSVECNormalize(&v1, up);
    return 1;
}
#pragma pop

#pragma push
#pragma force_active on
int HSD_CObjGetUpVector(HSD_CObj *cobj, Vec3 *up)
{
    if (!cobj || !up)
        return 0;
    if (cobj->flags & COBJ_USE_UP_VECTOR) {
        *up = cobj->u.up;
        return 1;
    }
    return camera_roll_to_up(cobj, up, cobj->u.roll);
}
#pragma pop

#pragma push
#pragma force_active on
static void CObjRelease(HSD_Class *object)
{
    HSD_WObj *eyepos;
    HSD_WObj *interest;
    HSD_CObj *cobj = (HSD_CObj *)object;
    HSD_AObjRemove(cobj->aobj);
    eyepos = HSD_CObjGetEyePositionWObj(cobj);
    HSD_WObjUnref(eyepos);
    interest = HSD_CObjGetInterestWObj(cobj);
    HSD_WObjUnref(interest);
    if (cobj->proj_mtx)
        HSD_MtxFree(cobj->proj_mtx);
    hsdCObj.parent.head.parent->release(object);
}
#pragma pop



#pragma push
#pragma force_active on
static GXProjectionType makeProjectionMtx(HSD_CObj *cobj, Mtx44 mtx)
{
    GXProjectionType projection_type;
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        projection_type = GX_PERSPECTIVE;
        MTXPerspective(mtx, cobj->projection_param.perspective.fov,
                       cobj->projection_param.perspective.aspect, cobj->near,
                       cobj->far);
        break;
    case PROJ_FRUSTUM:
        projection_type = GX_PERSPECTIVE;
        MTXFrustum(mtx, cobj->projection_param.perspective.fov,
                   cobj->projection_param.perspective.aspect,
                   cobj->projection_param.frustum.left,
                   cobj->projection_param.frustum.right, cobj->near, cobj->far);
        break;
    case PROJ_ORTHO:
        projection_type = GX_ORTHOGRAPHIC;
        MTXOrtho(mtx, cobj->projection_param.perspective.fov,
                 cobj->projection_param.perspective.aspect,
                 cobj->projection_param.frustum.left,
                 cobj->projection_param.frustum.right, cobj->near, cobj->far);
        break;
    }
    return projection_type;
}
#pragma pop

#pragma push
#pragma force_active on
static s32 setupNormalCamera(HSD_CObj *cobj)
{
    GXProjectionType projection_type;
    Mtx44 p;

    f64 x_scale;
    f64 y_scale;

    f32 top;
    f32 bottom;
    f32 left;
    f32 right;

    f32 width;
    f32 height;

    GXRenderModeObj *rmode = HSD_VIGetRenderMode();

    x_scale = (f64)rmode->fbWidth / (f64)scale_width;
    y_scale = (f64)rmode->efbHeight / (f64)scale_height;

    left = cobj->viewport.xmin * x_scale;
    right = cobj->viewport.xmax * x_scale;
    top = cobj->viewport.ymin * y_scale;
    bottom = cobj->viewport.ymax * y_scale;

    width = right - left;
    height = bottom - top;

    if (rmode->field_rendering) {
        GXSetViewportJitter(left, top, width, height, 0.0f, 1.0f, VIGetNextField());
    } else {
        GXSetViewport(left, top, width, height, 0.0f, 1.0f);
    }

    left = cobj->scissor.left * x_scale;
    right = cobj->scissor.right * x_scale;
    top = cobj->scissor.top * y_scale;
    bottom = cobj->scissor.bottom * y_scale;
    width = right - left;
    height = bottom - top;
    GXSetScissor(left, top, width, height);

    projection_type = makeProjectionMtx(cobj, p);
    GXSetProjection(p, projection_type);

    return true;
}
#pragma pop

#pragma push
#pragma force_active on
static s32 setupTopHalfCamera(HSD_CObj *cobj)
{
    GXProjectionType projection_type;
    Mtx44 p;

    f32 h_scale;
    f32 t;
    f32 b;
    f32 w;

    f32 top;
    f32 bottom;
    f32 left;
    f32 right;

    f32 width;
    f32 height;

    GXRenderModeObj *rmode = HSD_VIGetRenderMode();

    if (cobj->viewport.ymin >= rmode->efbHeight) {
        return 0;
    }
    left = cobj->viewport.xmin;
    right = cobj->viewport.xmax;
    top = cobj->viewport.ymin;
    /* Retain the original floating-point register allocation. */
    bottom = bottom = cobj->viewport.ymax;
    bottom = bottom < rmode->efbHeight ? bottom : rmode->efbHeight;
    width = right - left;
    height = bottom - top;
    GXSetScissor(left, top, width, height);
    top = cobj->viewport.ymin;
    bottom = cobj->viewport.ymax;
    left = cobj->viewport.xmin;
    right = cobj->viewport.xmax;
    height = (bottom < rmode->efbHeight ? bottom : rmode->efbHeight) - top;
    h_scale = height / (bottom - top);
    width = right - left;

    GXSetViewport(left, top, width, height, 0.0f, 1.0f);

    {
        int unused_2[1];

        switch (cobj->projection_type) {
        case PROJ_PERSPECTIVE:
            projection_type = GX_PERSPECTIVE;
            t = cobj->near *
                tanf(DegToRad(0.5 * cobj->projection_param.perspective.fov));
            w = t * cobj->projection_param.perspective.aspect;
            b = t * -(2.0f * h_scale - 1.0f);
            MTXFrustum(p, t, b, -w, w, cobj->near, cobj->far);
            break;
        case PROJ_FRUSTUM:
            projection_type = GX_PERSPECTIVE;
            MTXFrustum(p, cobj->projection_param.perspective.fov,
                       -(h_scale * (cobj->projection_param.perspective.fov -
                                    cobj->projection_param.perspective.aspect) -
                         cobj->projection_param.perspective.fov),
                       cobj->projection_param.frustum.left,
                       cobj->projection_param.frustum.right, cobj->near, cobj->far);
            break;
        case PROJ_ORTHO:
            projection_type = GX_ORTHOGRAPHIC;
            MTXOrtho(p, cobj->projection_param.perspective.fov,
                     -(h_scale * (cobj->projection_param.perspective.fov -
                                  cobj->projection_param.perspective.aspect) -
                       cobj->projection_param.perspective.fov),
                     cobj->projection_param.frustum.left,
                     cobj->projection_param.frustum.right, cobj->near, cobj->far);
            break;
        }
    }

    GXSetProjection(p, projection_type);

    return true;
}
#pragma pop

#pragma push
#pragma force_active on
static s32 setupBottomHalfCamera(HSD_CObj *cobj)
{
    GXProjectionType projection_type;
    Mtx44 p;

    f32 top, bottom;
    f32 left, right;
    f32 width, height, hscale;
    f32 t, b, w, h;
    u32 screen_top;

    GXRenderModeObj *rmode = HSD_VIGetRenderMode();

    screen_top = rmode->efbHeight - 8;

    if (cobj->viewport.ymax < screen_top) {
        return 0;
    }

    top = cobj->scissor.top;
    left = cobj->scissor.left;
    right = cobj->scissor.right;
    top =
        (cobj->scissor.top > screen_top ? cobj->scissor.top : screen_top) - screen_top;
    bottom = cobj->scissor.bottom - screen_top;
    width = right - left;
    height = bottom - top;
    GXSetScissor(left, top, width, height);

    top = cobj->viewport.ymin;
    left = cobj->viewport.xmin;
    right = cobj->viewport.xmax;
    top = (top > screen_top ? top : screen_top);
    bottom = cobj->viewport.ymax;
    width = right - left;
    height = bottom - cobj->viewport.ymin;

    {
        int unused;
        t = top - screen_top;
        h = (bottom - screen_top) - t;

        hscale = h / height;

        GXSetViewport(left, t, width, h, 0.0f, 1.0f);
    }

    {
        switch (cobj->projection_type) {
        case PROJ_PERSPECTIVE:
            projection_type = GX_PERSPECTIVE;
            b = cobj->near *
                tanf(DegToRad(0.5 * cobj->projection_param.perspective.fov));
            w = b * cobj->projection_param.perspective.aspect;
            t = b * (2.0f * hscale + -1.0f);
            MTXFrustum(p, t, -b, -w, w, cobj->near, cobj->far);
            break;
        case PROJ_FRUSTUM:
            projection_type = GX_PERSPECTIVE;
            h = (hscale * (cobj->projection_param.perspective.fov -
                           cobj->projection_param.perspective.aspect) +
                 cobj->projection_param.perspective.aspect);
            MTXFrustum(p, h, cobj->projection_param.perspective.aspect,
                       cobj->projection_param.frustum.left,
                       cobj->projection_param.frustum.right, cobj->near, cobj->far);
            break;
        case PROJ_ORTHO:
            projection_type = GX_ORTHOGRAPHIC;
            MTXOrtho(p,
                     (hscale * (cobj->projection_param.perspective.fov -
                                cobj->projection_param.perspective.aspect) +
                      cobj->projection_param.perspective.aspect),
                     cobj->projection_param.perspective.aspect,
                     cobj->projection_param.frustum.left,
                     cobj->projection_param.frustum.right, cobj->near, cobj->far);
            break;
        }
    }

    GXSetProjection(p, projection_type);

    return true;
}
#pragma pop

void HSD_CObjEndCurrent(void)
{
    _HSD_ZListSort();
    _HSD_ZListDisp();
}

void HSD_CObjGetInterest(HSD_CObj *cobj, Vec3 *interest)
{
    HSD_ASSERT(748, cobj);
    HSD_WObjGetPosition(HSD_CObjGetInterestWObj(cobj), interest);
}

void HSD_CObjSetInterest(HSD_CObj *cobj, Vec3 *interest)
{
    HSD_ASSERT(760, cobj);
    HSD_WObjSetPosition(HSD_CObjGetInterestWObj(cobj), interest);
}

void HSD_CObjGetEyePosition(HSD_CObj *cobj, Vec3 *position)
{
    HSD_ASSERT(772, cobj);
    HSD_WObjGetPosition(HSD_CObjGetEyePositionWObj(cobj), position);
}

void HSD_CObjSetEyePosition(HSD_CObj *cobj, Vec3 *position)
{
    HSD_ASSERT(784, cobj);
    HSD_WObjSetPosition(HSD_CObjGetEyePositionWObj(cobj), position);
}

float HSD_CObjGetEyeDistance(HSD_CObj *cobj)
{
    Vec3 position;
    Vec3 interest;
    Vec3 look_vector;

    if (cobj == NULL) {
        return 0.0f;
    }
    HSD_ASSERT(831, cobj->eyepos);
    HSD_ASSERT(832, cobj->interest);
    HSD_CObjGetEyePosition(cobj, &position);
    HSD_CObjGetInterest(cobj, &interest);
    VECSubtract(&interest, &position, &look_vector);
    return VECMag(&look_vector);
}

static inline f32 camera_absolute(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

#pragma push
#pragma force_active on
static int vec_normalize_check(Vec3 *src, Vec3 *dst)
{
    if (!src || !dst)
        return -1;
    if (camera_absolute(src->x) <= FLT_MIN && camera_absolute(src->y) <= FLT_MIN &&
        camera_absolute(src->z) <= FLT_MIN)
        return -1;
    PSVECNormalize(src, dst);
    return 0;
}
#pragma pop

int HSD_CObjGetEyeVector(HSD_CObj *cobj, Vec3 *eye)
{
    Vec3 position, interest;
    if (!cobj || !cobj->eyepos || !cobj->interest)
        return 0;
    HSD_CObjGetEyePosition(cobj, &position);
    HSD_CObjGetInterest(cobj, &interest);
    PSVECSubtract(&interest, &position, eye);
    return !vec_normalize_check(eye, eye);
}

static inline f32 cobj_get_up_x(Vec3 *up)
{
    return up->x;
}

static inline Vec3 *cobj_origin(void)
{
    static Vec3 orig = {0.0F, 0.0F, 0.0F};
    return &orig;
}

static inline Vec3 *cobj_y_axis(void)
{
    static Vec3 uy = {0.0F, 1.0F, 0.0F};
    return &uy;
}

static inline f32 upvec2roll(HSD_CObj *cobj, Vec3 *up)
{
    Vec3 local_up;
    Vec3 eye;
    Mtx m;
    if (!HSD_CObjGetEyeVector(cobj, &eye))
        return 0.0F;
    {
        f32 dot = PSVECDotProduct(up, &eye);
        dot = 1.0F - __fabsf(dot);
        if (dot < FLT_MIN)
            return 0.0F;
    }
    C_MTXLookAt(m, cobj_origin(), cobj_y_axis(), &eye);
    PSMTXMultVecSR(m, up, &local_up);
    {
        f32 y = local_up.y;
        f32 negative_x = -local_up.x;
        if (y == 0.0F)
            return negative_x >= 0.0F ? 1.5707963267948966F : -1.5707963267948966F;
        return atan2f(negative_x, y);
    }
}

void HSD_CObjSetUpVector(HSD_CObj *cobj, Vec3 *up)
{
    if (!cobj || !up)
        return;
    if (cobj->flags & COBJ_USE_UP_VECTOR) {
        Vec3 normalized;
        if (vec_normalize_check(up, &normalized)) {
            OSReport("illegal up vector.");
            HSD_ASSERT(976, 0);
        }
        if (cobj->u.up.x != cobj_get_up_x(&normalized) ||
            cobj->u.up.y != normalized.y || cobj->u.up.z != normalized.z) {
            HSD_CObjSetMtxDirty(cobj);
            cobj->u.up = normalized;
        }
    } else {
        HSD_CObjSetRoll(cobj, upvec2roll(cobj, up));
    }
}

void HSD_CObjSetMtxDirty(HSD_CObj *cobj)
{
    cobj->flags |= COBJ_MTX_DIRTY | COBJ_INV_MTX_DIRTY;
}

static inline s32 HSD_WObjMtxIsDirty(HSD_WObj *wobj)
{
    return wobj->flags & WOBJ_POSITION_DIRTY;
}

s32 HSD_CObjMtxIsDirty(HSD_CObj *cobj)
{
    return (cobj->flags & COBJ_MTX_DIRTY) ||
           (cobj->eyepos != NULL && HSD_WObjMtxIsDirty(cobj->eyepos)) ||
           (cobj->interest != NULL && HSD_WObjMtxIsDirty(cobj->interest));
}

void HSD_CObjGetViewingMtx(HSD_CObj *cobj, Mtx mtx)
{
    PSMTXCopy(HSD_CObjGetViewingMtxPtr(cobj), mtx);
}

MtxPtr HSD_CObjGetInvViewingMtxPtrDirect(HSD_CObj *cobj)
{
    if (cobj->flags & COBJ_INV_MTX_DIRTY) {
        if (cobj->proj_mtx == NULL) {
            cobj->proj_mtx = HSD_MtxAlloc();
        }
        PSMTXInverse(cobj->view_mtx, *cobj->proj_mtx);
        HSD_CObjClearFlags(cobj, COBJ_INV_MTX_DIRTY);
    }
    return *cobj->proj_mtx;
}

static inline void sr_eye_position(HSD_CObj *cobj, Vec3 *position)
{
    cobj ? (void)0 : __assert("cobj.c", 772, "cobj");
    cobj ? (void)0 : __assert("cobj.c", 724, "cobj");
    HSD_WObjGetPosition(cobj->eyepos, position);
}

static inline int sr_eye_vector(HSD_CObj *cobj, Vec3 *eye)
{
    Vec3 position, interest;
    if (!cobj || !cobj->eyepos || !cobj->interest)
        return 0;
    sr_eye_position(cobj, &position);
    camera_interest_position(cobj, &interest);
    PSVECSubtract(&interest, &position, eye);
    return !vec_normalize_check(eye, eye);
}

static inline f32 sr_sqrtf(f32 x)
{
    volatile f32 y;
    if (x > 0.0F) {
        f64 guess = __frsqrte(x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        y = x * guess;
        return y;
    }
    return x;
}

static inline int sr_roll2upvec(HSD_CObj *cobj, Vec3 *up, float roll)
{
    Vec3 eye;
    Vec3 v0;
    Vec3 v1;
    Mtx m;

    if (!sr_eye_vector(cobj, &eye))
        return 0;
    if (1.0 - camera_abs_y(&eye) < 0.0001) {
        v0.x = sr_sqrtf(eye.y * eye.y + eye.z * eye.z);
        v0.y = eye.y * (-camera_vector_x(&eye) / v0.x);
        v0.z = eye.z * (-eye.x / v0.x);
    } else {
        v0.y = sr_sqrtf(eye.x * eye.x + eye.z * eye.z);
        v0.x = eye.x * (-eye.y / v0.y);
        v0.z = eye.z * (-eye.y / v0.y);
    }
    PSMTXRotAxisRad(m, &eye, -roll);
    PSMTXMultVecSR(m, &v0, &v1);
    PSVECNormalize(&v1, up);
    return 1;
}

static inline f32 sr_local_roll(Vec3 *local_up)
{
    f32 y = local_up->y;
    f32 negative_x = -local_up->x;
    if (y == 0.0F)
        return negative_x >= 0.0F ? 1.5707963267948966F : -1.5707963267948966F;
    return atan2f(negative_x, y);
}

static inline int sr_parallel(Vec3 *up, Vec3 *eye)
{
    f32 dot = __fabsf(PSVECDotProduct(up, eye));
    dot = 1.0F - dot;
    return dot < FLT_MIN;
}

static inline f32 sr_upvec2roll(HSD_CObj *cobj, Vec3 *up)
{
    Vec3 *local_up_ptr;
    Vec3 local_up;
    Vec3 eye;
    Mtx m;
    if (!HSD_CObjGetEyeVector(cobj, &eye))
        return 0.0F;
    if (sr_parallel(up, &eye))
        return 0.0F;
    C_MTXLookAt(m, cobj_origin(), cobj_y_axis(), &eye);
    PSMTXMultVecSR(m, up, &local_up);
    {
        local_up_ptr = &local_up;
        return sr_local_roll(local_up_ptr);
    }
}

static inline void sr_set_up_vector(HSD_CObj *cobj, Vec3 *up)
{
    if (!cobj || !up)
        return;
    if (cobj->flags & COBJ_USE_UP_VECTOR) {
        Vec3 normalized;
        if (vec_normalize_check(up, &normalized)) {
            OSReport("illegal up vector.");
            HSD_ASSERT(976, 0);
        }
        if (cobj->u.up.x != cobj_get_up_x(&normalized) ||
            cobj->u.up.y != normalized.y || cobj->u.up.z != normalized.z) {
            HSD_CObjSetMtxDirty(cobj);
            cobj->u.up = normalized;
        }
    } else {
        HSD_CObjSetRoll(cobj, sr_upvec2roll(cobj, up));
    }
}

void HSD_CObjSetRoll(HSD_CObj *cobj, float roll)
{
    Vec3 up;

    if (!cobj)
        return;
    if (cobj->flags & COBJ_USE_UP_VECTOR) {
        sr_roll2upvec(cobj, &up, roll);
        sr_set_up_vector(cobj, &up);
    } else {
        if (cobj->u.roll != roll)
            cobj->flags |= COBJ_MTX_DIRTY | COBJ_INV_MTX_DIRTY;
        cobj->u.roll = roll;
    }
}

void HSD_CObjSetFov(HSD_CObj *cobj, float fov)
{
    if (cobj == NULL || cobj->projection_type != PROJ_PERSPECTIVE) {
        return;
    }
    cobj->projection_param.perspective.fov = fov;
}

void HSD_CObjSetAspect(HSD_CObj *cobj, float aspect)
{
    if (cobj == NULL || cobj->projection_type != PROJ_PERSPECTIVE) {
        return;
    }
    cobj->projection_param.perspective.aspect = aspect;
}

float HSD_CObjGetTop(HSD_CObj *cobj)
{
    float result;

    if (cobj == NULL) {
        return 0.0f;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        result =
            cobj->near * tanf(0.5F * DegToRad(cobj->projection_param.perspective.fov));
        break;
    case PROJ_FRUSTUM:
        result = cobj->projection_param.perspective.fov;
        break;
    case PROJ_ORTHO:
        result = cobj->projection_param.perspective.fov;
        break;
    default:
        result = 0.0f;
        break;
    }
    return result;
}

float HSD_CObjGetBottom(HSD_CObj *cobj)
{
    if (cobj == NULL) {
        return 0.0f;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        return -cobj->near *
               tanf(0.5F * DegToRad(cobj->projection_param.perspective.fov));
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.bottom;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.bottom;
    default:
        return 0.0f;
    }
}

float HSD_CObjGetLeft(HSD_CObj *cobj)
{
    if (cobj == NULL) {
        return 0.0f;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        return cobj->projection_param.perspective.aspect *
               (-cobj->near *
                tanf(0.5F * DegToRad(cobj->projection_param.perspective.fov)));
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.left;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.left;
    default:
        return 0.0f;
    }
}

float HSD_CObjGetRight(HSD_CObj *cobj)
{
    if (cobj == NULL) {
        return 0.0f;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        return cobj->projection_param.perspective.aspect *
               (cobj->near *
                tanf(0.5F * DegToRad(cobj->projection_param.perspective.fov)));
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.right;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.right;
    default:
        return 0.0f;
    }
}

float HSD_CObjGetNear(HSD_CObj *cobj)
{
    if (cobj == NULL) {
        return 0.0f;
    }
    return cobj->near;
}

void HSD_CObjSetNear(HSD_CObj *cobj, float near)
{
    if (cobj != NULL) {
        cobj->near = near;
    }
}

float HSD_CObjGetFar(HSD_CObj *cobj)
{
    if (cobj == NULL) {
        return 0.0f;
    }
    return cobj->far;
}

void HSD_CObjSetFar(HSD_CObj *cobj, float far)
{
    if (cobj != NULL) {
        cobj->far = far;
    }
}

void HSD_CObjGetScissor(HSD_CObj *cobj, Scissor *scissor)
{
    if (cobj == NULL) {
        return;
    }
    *scissor = cobj->scissor;
}

void HSD_CObjSetScissor(HSD_CObj *cobj, Scissor *scissor)
{
    if (cobj == NULL) {
        return;
    }
    cobj->scissor = *scissor;
}

void HSD_CObjSetScissorx4(HSD_CObj *cobj, u16 left, u16 right, u16 top, u16 bottom)
{
    if (cobj == NULL) {
        return;
    }
    cobj->scissor.left = left;
    cobj->scissor.right = right;
    cobj->scissor.top = top;
    cobj->scissor.bottom = bottom;
}

void HSD_CObjSetViewport(HSD_CObj *cobj, HSD_RectS16 *viewport)
{
    if (cobj == NULL) {
        return;
    }
    cobj->viewport.xmin = viewport->xmin;
    cobj->viewport.xmax = viewport->xmax;
    cobj->viewport.ymin = viewport->ymin;
    cobj->viewport.ymax = viewport->ymax;
}

void HSD_CObjSetViewportfx4(HSD_CObj *cobj, float left, float right, float top,
                            float bottom)
{
    if (cobj == NULL) {
        return;
    }
    cobj->viewport.xmin = left;
    cobj->viewport.xmax = right;
    cobj->viewport.ymin = top;
    cobj->viewport.ymax = bottom;
}

int HSD_CObjGetProjectionType(HSD_CObj *cobj)
{
    if (cobj == NULL) {
        return PROJ_PERSPECTIVE;
    }
    return cobj->projection_type;
}

void HSD_CObjSetProjectionType(HSD_CObj *cobj, u32 proj_type)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = proj_type;
}

void HSD_CObjSetFrustum(HSD_CObj *cobj, float top, float bottom, float left,
                        float right)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = PROJ_FRUSTUM;
    cobj->projection_param.frustum.top = top;
    cobj->projection_param.frustum.bottom = bottom;
    cobj->projection_param.frustum.left = left;
    cobj->projection_param.frustum.right = right;
}

void HSD_CObjSetOrtho(HSD_CObj *cobj, float top, float bottom, float left, float right)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = PROJ_ORTHO;
    cobj->projection_param.ortho.top = top;
    cobj->projection_param.ortho.bottom = bottom;
    cobj->projection_param.ortho.left = left;
    cobj->projection_param.ortho.right = right;
}

static inline s32 viewing_up_vector(HSD_CObj *cobj, Vec3 *up)
{
    if (!cobj || !up)
        return 0;
    if (cobj->flags & COBJ_USE_UP_VECTOR) {
        *up = cobj->u.up;
        return 1;
    }
    return roll2upvec(cobj, up, cobj->u.roll);
}

static inline void HSD_WObjClearFlags(HSD_WObj *wobj, u32 flags)
{
    wobj->flags &= ~flags;
}

MtxPtr HSD_CObjGetViewingMtxPtr(HSD_CObj *cobj)
{
    Vec3 interest;
    Vec3 up_vec;
    Vec3 eyepos;

    if (!(cobj->flags & 2) && HSD_CObjMtxIsDirty(cobj)) {
        HSD_CObjGetEyePosition(cobj, &eyepos);
        if (!viewing_up_vector(cobj, &up_vec)) {
            up_vec.x = 0.0F;
            up_vec.y = 1.0F;
            up_vec.z = 0.0F;
        }
        HSD_CObjGetInterest(cobj, &interest);
        C_MTXLookAt(cobj->view_mtx, &eyepos, &up_vec, &interest);
        HSD_WObjClearFlags(cobj->eyepos, WOBJ_POSITION_DIRTY);
        HSD_WObjClearFlags(cobj->interest, WOBJ_POSITION_DIRTY);
        HSD_CObjClearFlags(cobj, COBJ_MTX_DIRTY);
        HSD_CObjSetFlags(cobj, COBJ_INV_MTX_DIRTY);
    }
    return cobj->view_mtx;
}

MtxPtr HSD_CObjGetInvViewingMtxPtr(HSD_CObj *cobj)
{
    Vec3 interest;
    Vec3 up_vec;
    Vec3 eyepos;

    if (!(cobj->flags & 2) && HSD_CObjMtxIsDirty(cobj)) {
        HSD_CObjGetEyePosition(cobj, &eyepos);
        if (!viewing_up_vector(cobj, &up_vec)) {
            up_vec.x = 0.0F;
            up_vec.y = 1.0F;
            up_vec.z = 0.0F;
        }
        HSD_CObjGetInterest(cobj, &interest);
        C_MTXLookAt(cobj->view_mtx, &eyepos, &up_vec, &interest);
        HSD_WObjClearFlags(cobj->eyepos, WOBJ_POSITION_DIRTY);
        HSD_WObjClearFlags(cobj->interest, WOBJ_POSITION_DIRTY);
        HSD_CObjClearFlags(cobj, COBJ_MTX_DIRTY);
        HSD_CObjSetFlags(cobj, COBJ_INV_MTX_DIRTY);
    }
    return HSD_CObjGetInvViewingMtxPtrDirect(cobj);
}

void HSD_CObjSetFlags(HSD_CObj *cobj, u32 flags)
{
    if (cobj == NULL) {
        return;
    }
    cobj->flags |= flags;
}

void HSD_CObjClearFlags(HSD_CObj *cobj, u32 flags)
{
    if (cobj == NULL) {
        return;
    }
    cobj->flags &= ~flags;
}

HSD_CObj *HSD_CObjGetCurrent(void)
{
    return current;
}

HSD_CObj *HSD_CObjAlloc(void)
{
    HSD_CObj *cobj = HSD_COBJ(hsdNew(default_class ? default_class : &hsdCObj.parent));
    cobj ? (void)0 : __assert("cobj.c", 1936, "cobj");
    return cobj;
}

HSD_CObj *HSD_CObjLoadDesc(HSD_CObjDesc *desc)
{
    HSD_ClassInfo *info;
    HSD_CObj *cobj;

    if (desc != NULL) {
        if (desc->class_name == NULL ||
            (info = hsdSearchClassInfo(desc->class_name)) == NULL) {
            cobj = HSD_CObjAlloc();
        } else {
            cobj = hsdNew(info);
            cobj ? (void)0 : __assert("cobj.c", 2021, "cobj");
        }
        HSD_COBJ_METHOD(cobj)->load(cobj, desc);
        (void)"hsdIsDescendantOf(info, &hsdCObj)";
        return cobj;
    }
    return NULL;
}

#pragma push
#pragma force_active on
static int CObjInit(HSD_Class *o)
{
    HSD_CObj *cobj;
    int status = HSD_OBJECT_PARENT_INFO(&hsdCObj)->initialize(o);
    if (status < 0) {
        return status;
    }
    cobj = HSD_COBJ(o);
    if (cobj != NULL) {
        HSD_CObjSetMtxDirty(cobj);
    }
    cobj->eyepos = HSD_WObjAlloc();
    cobj->interest = HSD_WObjAlloc();
    return 0;
}
#pragma pop

#pragma push
#pragma force_active on
static void CObjAmnesia(HSD_ClassInfo *info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    if (info == HSD_CLASS_INFO(&hsdCObj)) {
        current = NULL;
    }
    HSD_OBJECT_PARENT_INFO(&hsdCObj)->forget(info);
}
#pragma pop


extern void HSD_WObjInit(HSD_WObj *, HSD_WObjDesc *);

static inline void cl_reset_flags(HSD_CObj *cobj, u32 flags)
{
    if (cobj == NULL)
        return;
    cobj->flags = (cobj->flags & (COBJ_MTX_DIRTY | COBJ_INV_MTX_DIRTY)) | flags;
}

static inline void cl_set_perspective(HSD_CObj *cobj, float fov, float aspect)
{
    if (cobj == NULL)
        return;
    cobj->projection_type = PROJ_PERSPECTIVE;
    cobj->projection_param.perspective.fov = fov;
    cobj->projection_param.perspective.aspect = aspect;
}

static inline void cl_set_roll(HSD_CObj *cobj, float roll)
{
    Vec3 up;

    if (!cobj)
        return;
    if (cobj->flags & COBJ_USE_UP_VECTOR) {
        roll2upvec(cobj, &up, roll);
        HSD_CObjSetUpVector(cobj, &up);
    } else {
        if (cobj->u.roll != roll)
            cobj->flags |= COBJ_MTX_DIRTY | COBJ_INV_MTX_DIRTY;
        cobj->u.roll = roll;
    }
}

/* The loader and roll setter need separate inline forms for their original stack lifetimes. */
static inline f32 cl_upvec2roll(HSD_CObj *cobj, Vec3 *up)
{
    Vec3 local_up;
    Vec3 eye;
    Mtx m;
    if (!HSD_CObjGetEyeVector(cobj, &eye))
        return 0.0F;
    if (sr_parallel(up, &eye))
        return 0.0F;
    C_MTXLookAt(m, cobj_origin(), cobj_y_axis(), &eye);
    PSMTXMultVecSR(m, up, &local_up);
    return sr_local_roll(&local_up);
}

static inline void cl_set_up_vector(HSD_CObj *cobj, Vec3 *up)
{
    if (!cobj || !up)
        return;
    if (cobj->flags & COBJ_USE_UP_VECTOR) {
        Vec3 normalized;
        if (vec_normalize_check(up, &normalized)) {
            OSReport("illegal up vector.");
            HSD_ASSERT(976, 0);
        }
        if (cobj->u.up.x != cobj_get_up_x(&normalized) ||
            cobj->u.up.y != normalized.y || cobj->u.up.z != normalized.z) {
            HSD_CObjSetMtxDirty(cobj);
            cobj->u.up = normalized;
        }
    } else {
        HSD_CObjSetRoll(cobj, cl_upvec2roll(cobj, up));
    }
}

int CObjLoad(HSD_CObj *cobj, HSD_CObjDesc *desc)
{
    static Vec3 y_axis = {0.0F, 1.0F, 0.0F};

    cobj->flags = desc->common.flags;
    cl_reset_flags(cobj, desc->common.flags);
    HSD_CObjSetViewport(cobj, &desc->common.viewport);
    HSD_CObjSetScissor(cobj, &desc->common.scissor);
    HSD_WObjInit(cobj->eyepos, desc->common.eyepos);
    HSD_WObjInit(cobj->interest, desc->common.interest);
    HSD_CObjSetNear(cobj, desc->common.nnear);
    HSD_CObjSetFar(cobj, desc->common.ffar);
    if (desc->common.flags & COBJ_USE_UP_VECTOR) {
        if (desc->common.up_vector != NULL) {
            cl_set_up_vector(cobj, desc->common.up_vector);
        } else {
            cl_set_up_vector(cobj, &y_axis);
        }
    } else {
        cl_set_roll(cobj, desc->common.roll);
    }
    switch (desc->common.projection_type) {
    case PROJ_PERSPECTIVE:
        cl_set_perspective(cobj, desc->perspective.fov, desc->perspective.aspect);
        break;
    case PROJ_ORTHO:
        HSD_CObjSetOrtho(cobj, desc->ortho.top, desc->ortho.bottom, desc->ortho.left,
                         desc->ortho.right);
        break;
    case PROJ_FRUSTUM:
        HSD_CObjSetFrustum(cobj, desc->frustum.top, desc->frustum.bottom,
                           desc->frustum.left, desc->frustum.right);
        break;
    default:
        HSD_ASSERT(1982, 0);
        break;
    }
    return 0;
}

void CObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdCObj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_cobj", sizeof(HSD_CObjInfo),
                     sizeof(HSD_CObj));
    HSD_CLASS_INFO(&hsdCObj)->initialize = CObjInit;
    HSD_CLASS_INFO(&hsdCObj)->release = CObjRelease;
    HSD_CLASS_INFO(&hsdCObj)->forget = CObjAmnesia;
    hsdCObj.load = CObjLoad;
    hsdCObj.update = CObjUpdateFunc;
}
