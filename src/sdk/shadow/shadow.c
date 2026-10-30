/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/shadow.c. */
#include "shadow.h"
#include "state.h"
#include "perf.h"
#define NULL 0
#define GX_FALSE 0
#define GX_TRUE 1
#define GX_SRC_REG 0
#define GX_LIGHT_NULL 0
#define GX_DF_CLAMP 2
#define GX_AF_NONE 2
#define GX_CS_SCALE_1 0
#define GX_TB_ZERO 0
#define GX_BM_NONE 0
#define GX_BL_SRCALPHA 4
#define GX_BL_INVSRCALPHA 5
#define GX_LO_SET 15
#define GX_ALWAYS 7
#define GX_AOP_AND 0
#define GX_VA_POS 9
#define GX_DIRECT 1
#define GX_VTXFMT0 0
#define GX_POS_XYZ 1
#define GX_F32 4
#define GX_CULL_BACK 2
#define GX_QUADS 0x80
#define GX_TEVSTAGE0 0
#define GX_TEXCOORD_NULL 0xFF
#define GX_TEXMAP_NULL 0xFF
#define RENDER_SHADOW 0x04000000
#define HSD_TRSP_OPA 1
#define HSD_TRSP_TEXEDGE 4
#define HSD_TrspMask u32
#define F32_MAX 3.4028234663852886e38F
#define MTXLightPerspective C_MTXLightPerspective
#define MTXLightFrustum C_MTXLightFrustum
#define MTXLightOrtho C_MTXLightOrtho
#define MTXConcat PSMTXConcat
#define VECSubtract PSVECSubtract
#define VECNormalize PSVECNormalize
#define VECCrossProduct PSVECCrossProduct
#define VECMag PSVECMag
#define VECDotProduct PSVECDotProduct
#define VECScale PSVECScale
#define HSD_ASSERT(line, condition)                                                    \
    do {                                                                               \
        if (!(condition))                                                              \
            __assert("shadow.c", line, #condition);                                    \
    } while (0)
extern Mtx HSD_identityMtx;
static inline MtxPtr HSD_CObjGetViewingMtxPtrDirect(HSD_CObj *cobj)
{
    return cobj->view_mtx;
}
extern void *memset(void *, s32, u32);
extern void HSD_Free(void *);
extern void *HSD_Alloc(u32);
extern void GXSetTexCopySrc(u16, u16, u16, u16);
extern void GXSetTexCopyDst(u16, u16, s32, u8);
extern u32 GXGetTexBufferSize(u16, u16, s32, u8, u8);
extern void GXLoadPosMtxImm(Mtx, u32);
extern void GXSetCurrentMtx(u32);
extern void HSD_ClearVtxDesc(void);
extern void GXBegin(u8, s32, u16);
extern void GXSetScissor(u32, u32, u32, u32);
extern void GXCopyTex(void *, u8);
extern void GXPixModeSync(void);
extern void GXInvalidateTexAll(void);
extern void C_MTXLightPerspective(Mtx, f32, f32, f32, f32, f32, f32);
extern void C_MTXLightFrustum(Mtx, f32, f32, f32, f32, f32, f32, f32, f32, f32);
extern void C_MTXLightOrtho(Mtx, f32, f32, f32, f32, f32, f32, f32, f32);
extern void PSVECSubtract(Vec3 *, Vec3 *, Vec3 *);
extern void PSVECCrossProduct(Vec3 *, Vec3 *, Vec3 *);
extern void PSVECScale(Vec3 *, Vec3 *, f32);
extern f32 PSVECDotProduct(Vec3 *, Vec3 *);
extern f32 atan2f(f32, f32);
static inline void GXPosition2f32(f32 x, f32 y)
{
    *(volatile f32 *)0xCC008000 = x;
    *(volatile f32 *)0xCC008000 = y;
}
#define GXEnd() ((void)0)
#define HSD_PerfCountMtxLoad() (++HSD_PerfCurrentStat.matrix_load_count)
static inline s32 ref_DEC(void *o)
{
    s32 result;
    if ((result = (((HSD_Obj *)o)->reference_count == (u16)-1)))
        return result;
    return ((HSD_Obj *)o)->reference_count-- == 0;
}
static inline void hsdDelete(void *o)
{
    if (o) {
        ((HSD_Class *)o)->class_info->release(o);
        ((HSD_Class *)o)->class_info->destroy(o);
    }
}
static inline void HSD_JObjRef(void *o)
{
    if (o) {
        ++((HSD_Obj *)o)->reference_count;
        if (((HSD_Obj *)o)->reference_count == (u16)-1)
            __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF");
    }
}
static inline f32 fabsf_bitwise(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}
static HSD_ObjAllocData shadow_alloc_data;

static void makeMatrix(HSD_Shadow *shadow);





static inline HSD_TObj *makeShadowTObj(void)
{
    HSD_TObj *shadowTObj;
    shadowTObj = HSD_TObjAlloc();
    shadowTObj->src = 0;
    shadowTObj->wrap_s = 0;
    shadowTObj->wrap_t = 0;
    shadowTObj->flags = TEX_COORD_SHADOW | TEX_LIGHTMAP_SHADOW | TEX_COLORMAP_MODULATE |
                        TEX_ALPHAMAP_PASS;
    shadowTObj->imagedesc = HSD_ImageDescAlloc();
    return shadowTObj;
}





void HSD_ShadowInit(HSD_Shadow *shadow)
{
    HSD_ImageDesc *imagedesc;

    HSD_ASSERT(245, shadow);
    imagedesc = shadow->texture->imagedesc;
    GXSetTexCopySrc(0, 0, imagedesc->width, imagedesc->height);
    GXSetTexCopyDst(imagedesc->width, imagedesc->height, GX_CTF_R4, GX_FALSE);
}

void HSD_ShadowSetSize(HSD_Shadow *shadow, u16 width, u16 height)
{
    u32 size;
    HSD_ImageDesc *idesc;

    HSD_ASSERT(277, shadow);
    HSD_ASSERT(278, width > 0);
    HSD_ASSERT(279, height > 0);

    idesc = shadow->texture->imagedesc;
    if (!idesc->image_ptr || idesc->width != width || idesc->height != height) {
        if (idesc->image_ptr) {
            HSD_Free(idesc->image_ptr);
        }

        size = GXGetTexBufferSize(width, height, GX_TF_I4, GX_FALSE, 0);
        HSD_ASSERT(0x122, size > 0);
        idesc->image_ptr = HSD_Alloc(size);
        idesc->width = width;
        idesc->height = height;

        HSD_CObjSetViewportfx4(shadow->camera, 0, width, 0, height);
        HSD_CObjSetScissorx4(shadow->camera, 0, width, 0, height);
    }
}

static void drawBackgroundRect(HSD_Shadow *shadow)
{
    f32 top, bottom, left, right, near;
    HSD_CObj *cobj = shadow->camera;

    GXLoadPosMtxImm(HSD_identityMtx, GX_PNMTX0);
    HSD_PerfCountMtxLoad();
    GXSetCurrentMtx(GX_PNMTX0);
    HSD_ClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    HSD_StateSetCullMode(GX_CULL_BACK);

    top = HSD_CObjGetTop(cobj);
    bottom = HSD_CObjGetBottom(cobj);
    left = HSD_CObjGetLeft(cobj);
    right = HSD_CObjGetRight(cobj);
    near = HSD_CObjGetNear(cobj);

    top *= 1.20000004768f;
    bottom *= 1.20000004768f;
    left *= 1.20000004768f;
    right *= 1.20000004768f;
    near *= -1.10000002384f;

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(left, top);
    GXPosition2f32(near, right);
    GXPosition2f32(top, near);
    GXPosition2f32(right, bottom);
    GXPosition2f32(near, left);
    GXPosition2f32(bottom, near);
    GXEnd();
}

void HSD_ShadowStartRender(HSD_Shadow *shadow)
{
    HSD_CObj *cobj;
    HSD_ImageDesc *idesc;
    HSD_SList *list;

    HSD_ASSERT(359, shadow);
    HSD_ASSERT(360, shadow->camera);
    HSD_ASSERT(361, shadow->texture);
    HSD_ASSERT(362, shadow->texture->imagedesc);

    list = shadow->objects;
    cobj = shadow->camera;
    idesc = shadow->texture->imagedesc;

    if (list != NULL) {
        HSD_CObjSetCurrent(cobj);
        {
            static HSD_Chan chan = {
                NULL,           GX_COLOR0A0, 0,          {0, 0, 0, 0},
                {0, 0, 0, 255}, GX_FALSE,    GX_SRC_REG, GX_SRC_REG,
                GX_LIGHT_NULL,  GX_DF_CLAMP, GX_AF_NONE, NULL,
            };
            static HSD_TevDesc tev = {
                NULL,
                TEVCONF_MODE,
                GX_TEVSTAGE0,
                GX_TEXCOORD_NULL,
                GX_TEXMAP_NULL,
                GX_COLOR0A0,
                {
                    GX_TEV_ADD,    GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,   GX_CC_RASC,
                    GX_CS_SCALE_1, GX_TB_ZERO, GX_TRUE,    GX_TEVPREV,   GX_TEV_ADD,
                    GX_CA_ZERO,    GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA,   GX_CS_SCALE_1,
                    GX_TB_ZERO,    GX_TRUE,    GX_TEVPREV, GX_TC_LINEAR,
                }};
            static HSD_PEDesc pedesc = {9, // ENABLE_COLOR_UPDATE | BEFORE_TEX,
                                        0,
                                        0,
                                        0,
                                        GX_BM_NONE,
                                        GX_BL_SRCALPHA,
                                        GX_BL_INVSRCALPHA,
                                        GX_LO_SET,
                                        GX_ALWAYS,
                                        GX_ALWAYS,
                                        GX_AOP_AND,
                                        GX_ALWAYS};

            HSD_StateInitTev();
            HSD_SetupTevStageAll(&tev);

            HSD_StateSetNumTexGens();

            HSD_SetupPEMode(0, &pedesc);

            chan.mat_color.r = 255;
            chan.mat_color.g = 255;
            chan.mat_color.b = 255;

            HSD_SetupChannelAll(&chan);
            GXSetScissor(0, 0, idesc->width, idesc->height);

            drawBackgroundRect(shadow);

            chan.mat_color.r = shadow->intensity;
            chan.mat_color.g = shadow->intensity;
            chan.mat_color.b = shadow->intensity;

            HSD_SetupChannelAll(&chan);
            GXSetScissor(2, 2, idesc->width - 4, idesc->height - 4);
        }

        for (list = shadow->objects; list != NULL; list = list->next) {
            HSD_JObjDispAll(list->data, NULL,
                            (HSD_TrspMask)(HSD_TRSP_OPA | HSD_TRSP_TEXEDGE),
                            RENDER_SHADOW);
        }

        HSD_CObjEndCurrent();
    }
}

void HSD_ShadowEndRender(HSD_Shadow *shadow)
{
    HSD_ImageDesc *idesc;

    HSD_ASSERT(501, shadow);

    idesc = shadow->texture->imagedesc;
    if (!idesc->image_ptr) {
        HSD_ShadowSetSize(shadow, idesc->width, idesc->height);
    }

    GXCopyTex(idesc->image_ptr, GX_TRUE);
    GXPixModeSync();

    GXInvalidateTexAll();

    makeMatrix(shadow);
}

void HSD_ShadowSetActive(HSD_Shadow *shadow, int active)
{
    HSD_ImageDesc *idesc;

    HSD_ASSERT(580, shadow);

    if ((shadow->active && active) || (!shadow->active && !active)) {
        return;
    }

    shadow->active = active;
    if (active) {
        idesc = shadow->texture->imagedesc;
        if (!idesc->image_ptr) {
            HSD_ShadowSetSize(shadow, idesc->width, idesc->height);
        }

        HSD_MObjAddShadowTexture(shadow->texture);
    } else {
        HSD_MObjDeleteShadowTexture(shadow->texture);
    }
}

#pragma push
#pragma pool_data off

void HSD_ShadowAddObject(HSD_Shadow *shadow, HSD_JObj *jobj)
{
    HSD_SList *list;
    (void)"distance > 0.0F";

    if (!shadow || !jobj) {
        return;
    }

    for (list = shadow->objects; list; list = list->next) {
        if (list->data == jobj) {
            return;
        }
    }
    shadow->objects = HSD_SListPrepend(shadow->objects, jobj);
    HSD_JObjRef(jobj);
}

#pragma pop

void HSD_ShadowDeleteObject(HSD_Shadow *shadow, HSD_JObj *jobj)
{
    if (!shadow) {
        return;
    }

    if (jobj) {
        HSD_SList **list = &(shadow->objects);
        for (; *list; list = &((*list)->next)) {
            if ((*list)->data == jobj) {
                HSD_JObjUnref(jobj);
                (*list) = HSD_SListRemove(*list);
                return;
            }
        }
    } else {
        while (shadow->objects) {
            HSD_JObjUnref(shadow->objects->data);
            shadow->objects = HSD_SListRemove(shadow->objects);
        }
    }
}

static void makeMatrix(HSD_Shadow *shadow)
{
    Mtx Mprj;

    switch (HSD_CObjGetProjectionType(shadow->camera)) {
    case PROJ_PERSPECTIVE:
        MTXLightPerspective(Mprj, shadow->camera->projection_param.perspective.fov,
                            shadow->camera->projection_param.perspective.aspect,
                            shadow->scaleS, shadow->scaleT, shadow->transS,
                            shadow->transT);
        break;

    case PROJ_FRUSTUM:
        MTXLightFrustum(Mprj, shadow->camera->projection_param.frustum.top,
                        shadow->camera->projection_param.frustum.bottom,
                        shadow->camera->projection_param.frustum.left,
                        shadow->camera->projection_param.frustum.right,
                        shadow->camera->near, shadow->scaleS, shadow->scaleT,
                        shadow->transS, shadow->transT);
        break;

    case PROJ_ORTHO:
        MTXLightOrtho(Mprj, shadow->camera->projection_param.ortho.top,
                      shadow->camera->projection_param.ortho.bottom,
                      shadow->camera->projection_param.ortho.left,
                      shadow->camera->projection_param.ortho.right, shadow->scaleS,
                      shadow->scaleT, shadow->transS, shadow->transT);
        break;

    default:
        HSD_ASSERT(698, 0);
    }

    MTXConcat(Mprj, HSD_CObjGetViewingMtxPtrDirect(shadow->camera),
              shadow->texture->mtx);
}

#define HSD_ASSERT2(line, text, cond)                                                  \
    ((cond) ? ((void)0) : __assert("shadow.c", line, text))

void HSD_ShadowSetViewingRect(HSD_Shadow *shadow, float top, float bottom, float left,
                              float right)
{
    HSD_CObj *cobj;
    float distance;

    HSD_ASSERT(721, shadow);

    cobj = shadow->camera;
    distance = HSD_CObjGetEyeDistance(cobj);
    HSD_ASSERT2(725, "distance > 0.0F", distance > 0.0F);

    switch (HSD_CObjGetProjectionType(cobj)) {
    case PROJ_PERSPECTIVE: {
        float width, height;

        if (fabsf_bitwise(top) > fabsf_bitwise(bottom)) {
            width = fabsf_bitwise(top);
        } else {
            width = fabsf_bitwise(bottom);
        }
        if (fabsf_bitwise(left) > fabsf_bitwise(right)) {
            height = fabsf_bitwise(left);
        } else {
            height = fabsf_bitwise(right);
        }
        HSD_CObjSetAspect(cobj, height / width);
        HSD_CObjSetFov(cobj, atan2f(height, distance));
    } break;

    case PROJ_ORTHO:
        HSD_CObjSetOrtho(cobj, top, bottom, left, right);
        break;

    case PROJ_FRUSTUM: {
        float scale = HSD_CObjGetNear(cobj) / distance;
        HSD_ASSERT(754, scale > 0.0F);
        HSD_CObjSetFrustum(cobj, scale * top, scale * bottom, scale * left,
                           scale * right);
    } break;

    default:
        HSD_ASSERT(762, 0);
    }
}

void HSD_ViewingRectInit(HSD_ViewingRect *rect, Vec3 *position, Vec3 *interest,
                         Vec3 *upvector, int perspective)
{
    Vec3 v;
    HSD_ASSERT(795, rect);

    rect->origin = *position;
    VECSubtract(interest, position, &rect->eye_v);
    VECNormalize(&rect->eye_v, &rect->eye_vn);
    VECNormalize(upvector, &v);
    VECCrossProduct(&rect->eye_vn, &v, &rect->right_v);
    VECCrossProduct(&rect->right_v, &rect->eye_vn, &rect->up_v);
    rect->distance = VECMag(&rect->eye_v);

    rect->top = rect->right = -F32_MAX;
    rect->bottom = rect->left = F32_MAX;
    rect->perspective = perspective;
}

int HSD_ViewingRectCheck(HSD_ViewingRect *rect)
{
    HSD_ASSERT(818, rect);
    return rect->top > rect->bottom && rect->right > rect->left;
}

void HSD_ViewingRectAddRect(HSD_ViewingRect *rect, Vec3 *position, float top,
                            float bottom, float left, float right)
{
    float x, y, dot, scale;
    Vec3 o2p, e2p;

    HSD_ASSERT(855, rect);
    HSD_ASSERT(856, position);

    VECSubtract(position, &rect->origin, &o2p);
    dot = VECDotProduct(&o2p, &rect->eye_vn);
    if (rect->perspective) {
        if (dot <= 0.0F) {
            return;
        }
        scale = rect->distance / dot;
        VECScale(&o2p, &o2p, scale);
        VECSubtract(&o2p, &rect->eye_v, &e2p);
        x = VECDotProduct(&rect->right_v, &e2p);
        y = VECDotProduct(&rect->up_v, &e2p);

        top *= scale;
        bottom *= scale;
        left *= scale;
        right *= scale;
    } else {
        Vec3 tmp;
        VECScale(&rect->eye_vn, &tmp, dot);
        VECSubtract(&o2p, &tmp, &e2p);
        x = VECDotProduct(&rect->right_v, &e2p);
        y = VECDotProduct(&rect->up_v, &e2p);
    }

    if (x + right > rect->right) {
        rect->right = x + right;
    }
    if (x + left < rect->left) {
        rect->left = x + left;
    }
    if (y + top > rect->top) {
        rect->top = y + top;
    }
    if (y + bottom < rect->bottom) {
        rect->bottom = y + bottom;
    }
    (void)"radius > 0.0F";
    (void)"a > 0.0F";
}
