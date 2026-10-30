/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/pobj.c. */
#include "pobj.h"
#include "id.h"
#include "tobj.h"
#include "perf.h"
#define NULL 0
#define POBJ_ANIM 8
#define POBJ_SHAPEANIM 0x1000
#define POBJ_ENVELOPE 0x2000
#define POBJ_SKIN 0
#define SHAPESET_ADDITIVE 2
#define pobj_type(p) ((p)->flags & 0x3000)
#define next_p(p) ((p) ? (p)->next : NULL)
#define HSD_POBJ(o) ((HSD_PObj *)(o))
#define HSD_POBJ_METHOD(o) ((HSD_PObjInfo *)((o)->parent.class_info))
#define HSD_ASSERT(line, condition)                                                    \
    do {                                                                               \
        if (!(condition))                                                              \
            __assert("pobj.c", line, #condition);                                      \
    } while (0)
static void PObjInfoInit(void);
HSD_PObjInfo hsdPObj = {{{PObjInfoInit}}};
static HSD_PObjInfo *default_class;
static f32 (*vertex_buffer)[3];
static f32 (*normal_buffer)[3];
static u32 vertex_buffer_size;
static u32 normal_buffer_size;
static HSD_VtxDescList *prev_vtxdesclist_array;
static HSD_VtxDescList *prev_vtxdesc;
static struct mtx_mark_t {
    void *obj;
    u32 mark;
} mtx_mark[2];
extern void __assert(const char *, u32, const char *);
extern void HSD_Panic(const char *, u32, const char *);
extern void *hsdAllocMemPiece(u32);
extern void *memset(void *, int, u32);
extern void *HSD_Alloc(u32);
extern void HSD_AObjRemove(HSD_AObj *);
extern HSD_AObj *HSD_AObjLoadDesc(HSD_AObjDesc *);
extern void HSD_AObjReqAnim(HSD_AObj *, f32);
extern void HSD_AObjInterpretAnim(HSD_AObj *, void *, AnimationUpdate);
extern void _HSD_NeedCacheInvalidate(u32);

#define HSD_CLASS_INFO(o) ((HSD_ClassInfo *)(o))
#define HSD_POBJ_INFO(o) ((HSD_PObjInfo *)(o))
#define HSD_PARENT_INFO(o) ((o)->parent.head.parent)
#define HSD_MTX_RIGID 1
#define HSD_MTX_ENVELOPE 2
#define JOBJ_LIGHTING 0x80
#define JOBJ_USER_DEF_MTX 0x800000
#define JOBJ_MTX_DIRTY 0x40
#define SHAPESET_AVERAGE 1
#define HSD_DEFAULT_MAX_SHAPE_VERTICES 2000
#define HSD_DEFAULT_MAX_SHAPE_NORMALS 2000
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define pobj_min(x, y) ((x) < (y) ? (x) : (y))
#define HSD_MemAlloc HSD_Alloc
#define MTXConcat PSMTXConcat
#define mtx matrix
#define envelopemtx envelope_matrix
#define GX_VTXFMT0 0
#define GX_OPCODE_MASK 0xF8
#define GX_VAT_MASK 7
#define GX_NOP 0
#define GX_PNMTX0 0
#define GX_PNMTX1 3
#define GX_TEXMTX0 30
#define GX_TEXMTX1 33
#define GX_MTX3x4 0
#define POBJ_CULLFRONT 0x4000
#define POBJ_CULLBACK 0x8000
#define GX_CULL_NONE 0
#define GX_CULL_FRONT 1
#define GX_CULL_BACK 2
typedef s32 GXPrimitive;
typedef s32 GXVtxFmt;
typedef enum PObjSetupFlag {
    SETUP_NONE = 0,
    SETUP_NORMAL = 1,
    SETUP_REFLECTION = 2,
    SETUP_HIGHLIGHT = 4,
    SETUP_NORMAL_PROJECTION = 6,
    SETUP_JOINT0 = 1,
    SETUP_JOINT1 = 2
} PObjSetupFlag;
extern void GXBegin(GXPrimitive, GXVtxFmt, u16);
extern void GXCallDisplayList(void *, u32);
extern void GXSetCurrentMtx(u32);
extern void GXLoadPosMtxImm(Mtx, u32);
extern void GXLoadNrmMtxImm(Mtx, u32);
extern void GXLoadTexMtxImm(Mtx, u32, u32);
extern void OSReport(const char *, ...);
extern void *memcpy(void *, const void *, u32);
extern void HSD_Free(void *);
extern void hsdFreeMemPiece(void *, u32);
extern HSD_ClassInfo hsdClass;
extern HSD_JObj *HSD_JObjGetCurrent(void);
extern void HSD_JObjSetupMatrixSub(HSD_JObj *);
extern void HSD_JObjUnrefThis(HSD_JObj *);
extern u32 PSMTXInvXpose(Mtx, Mtx);
static inline void HSD_MtxInverseTranspose(Mtx source, Mtx destination)
{
    if (!PSMTXInvXpose(source, destination))
        PSMTXCopy(source, destination);
}
extern MtxPtr _HSD_mkEnvelopeModelNodeMtx(HSD_JObj *, Mtx);
extern s32 HSD_Index2PosNrmMtx(s32);
extern void HSD_GetCurrentArena(void **, void **);
extern void HSD_StateSetCullMode(s32);
static inline void fifo_byte(u8 v)
{
    *(volatile u8 *)0xCC008000 = v;
}
static inline void fifo_half(u16 v)
{
    *(volatile u16 *)0xCC008000 = v;
}
static inline void fifo_vec(f32 x, f32 y, f32 z)
{
    *(volatile f32 *)0xCC008000 = x;
    *(volatile f32 *)0xCC008000 = y;
    *(volatile f32 *)0xCC008000 = z;
}
#define GXTexCoord1u8 fifo_byte
#define GXTexCoord1x8 fifo_byte
#define GXColor1x8 fifo_byte
#define GXTexCoord1x16 fifo_half
#define GXColor1x16 fifo_half
#define GXColor1u16 fifo_half
#define GXPosition3f32 fifo_vec
#define GXNormal3f32 fifo_vec
static inline void GXColor3u8(u8 r, u8 g, u8 b)
{
    fifo_byte(r);
    fifo_byte(g);
    fifo_byte(b);
}
static inline void GXColor4u8(u8 r, u8 g, u8 b, u8 a)
{
    fifo_byte(r);
    fifo_byte(g);
    fifo_byte(b);
    fifo_byte(a);
}
/* GX ends a primitive after the vertex count supplied to GXBegin. */
#define GXEnd() ((void)0)
static inline void HSD_PerfCountMtxLoad(void)
{
    HSD_PerfCurrentStat.matrix_load_count++;
}
static inline void HSD_PObjRemoveAnimByFlags(HSD_PObj *pobj, u32 flags)
{
    if (pobj == NULL) {
        return;
    }

    if (flags & POBJ_ANIM) {
        HSD_AObjRemove(pobj->aobj);
        pobj->aobj = NULL;
    }
}

void HSD_PObjRemoveAnimAllByFlags(HSD_PObj *pobj, u32 flags)
{
    HSD_PObj *pp;

    if (pobj == NULL) {
        return;
    }

    for (pp = pobj; pp != NULL; pp = pp->next) {
        HSD_PObjRemoveAnimByFlags(pp, flags);
    }
}

static inline void HSD_PObjAddAnim(HSD_PObj *pobj, HSD_ShapeAnim *shapeanim)
{
    if (!pobj)
        return;
    if (pobj->aobj)
        HSD_AObjRemove(pobj->aobj);
    pobj->aobj = HSD_AObjLoadDesc(shapeanim->aobjdesc);
}

void HSD_PObjAddAnimAll(HSD_PObj *pobj, HSD_ShapeAnim *shapeanim)
{
    HSD_PObj *po;
    HSD_ShapeAnim *sa;

    if (pobj == NULL || shapeanim == NULL) {
        return;
    }

    for (po = pobj, sa = shapeanim; po != NULL; po = po->next, sa = next_p(sa)) {
        HSD_PObjAddAnim(po, sa);
    }
}

static inline void HSD_PObjReqAnimByFlags(HSD_PObj *pobj, f32 startframe, u32 flags)
{
    if (pobj == NULL) {
        return;
    }

    if (flags & POBJ_ANIM) {
        HSD_AObjReqAnim(pobj->aobj, startframe);
    }
}

void HSD_PObjReqAnimAllByFlags(HSD_PObj *pobj, f32 startframe, u32 flags)
{
    HSD_PObj *pp;

    if (pobj == NULL) {
        return;
    }

    for (pp = pobj; pp; pp = pp->next) {
        HSD_PObjReqAnimByFlags(pp, startframe, flags);
    }
}

static inline void ShapeSetSetAnimResult(HSD_ShapeSet *shape_set, u32 type,
                                         AnimationValue *val)
{
    if (shape_set->flags & SHAPESET_ADDITIVE) {
        shape_set->blend.bp[type - 2] = val->scalar;
    } else {
        shape_set->blend.bl = val->scalar;
    }
}

#pragma push
#pragma force_active on
static void PObjUpdateFunc(void *obj, int type, AnimationValue *val)
{
    HSD_PObj *pobj = HSD_POBJ(obj);
    if (pobj == NULL) {
        return;
    }

    if (pobj_type(pobj) == POBJ_SHAPEANIM) {
        ShapeSetSetAnimResult(pobj->u.shape_set, type, val);
    }
}
#pragma pop

static inline void HSD_PObjAnim(HSD_PObj *pobj)
{
    if (pobj)
        HSD_AObjInterpretAnim(pobj->aobj, pobj, HSD_POBJ_METHOD(pobj)->update);
}

void HSD_PObjAnimAll(HSD_PObj *pobj)
{
    HSD_PObj *pp;

    if (pobj != NULL) {
        for (pp = pobj; pp != NULL; pp = pp->next) {
            HSD_PObjAnim(pp);
        }
    }
}

static inline HSD_Envelope *HSD_EnvelopeAlloc(void)
{
    HSD_Envelope *envelope = hsdAllocMemPiece(sizeof(HSD_Envelope));
    s32 has_envelope = envelope != NULL;
    if (!has_envelope)
        __assert("pobj.c", 425, "envelope");
    memset(envelope, 0, sizeof(HSD_Envelope));
    return envelope;
}

#pragma push
#pragma force_active on
static HSD_SList *loadEnvelopeDesc(HSD_EnvelopeDesc **edesc_p)
{
    HSD_SList *list = NULL;
    HSD_SList **list_p = &list;

    if (edesc_p == NULL) {
        return NULL;
    }

    while (*edesc_p) {
        HSD_Envelope *envelope = NULL;
        HSD_Envelope **env_p = &envelope;
        HSD_EnvelopeDesc *edesc = *edesc_p;

        while (edesc->joint) {
            *env_p = HSD_EnvelopeAlloc();
            (*env_p)->weight = edesc->weight;
            env_p = &(*env_p)->next;
            edesc++;
        }

        (*list_p) = HSD_SListAlloc();
        (*list_p)->data = envelope;
        list_p = &(*list_p)->next;
        edesc_p++;
    }
    return list;
}
#pragma pop

#pragma push
#pragma force_active on
static HSD_ShapeSet *loadShapeSetDesc(HSD_ShapeSetDesc *sdesc)
{
    int i;

    HSD_ShapeSet *shape_set = hsdAllocMemPiece(sizeof(HSD_ShapeSet));
    HSD_ASSERT(482, shape_set);
    memset(shape_set, 0, sizeof(HSD_ShapeSet));
    shape_set->flags = sdesc->flags;
    shape_set->nb_shape = sdesc->nb_shape;
    shape_set->nb_vertex_index = sdesc->nb_vertex_index;
    shape_set->vertex_desc = sdesc->vertex_desc;
    shape_set->vertex_idx_list = sdesc->vertex_idx_list;
    shape_set->nb_normal_index = sdesc->nb_normal_index;
    shape_set->normal_desc = sdesc->normal_desc;
    shape_set->normal_idx_list = sdesc->normal_idx_list;
    if (shape_set->flags & SHAPESET_ADDITIVE) {
        shape_set->blend.bp = HSD_Alloc(shape_set->nb_shape * sizeof(f32));
        for (i = 0; i < shape_set->nb_shape; i++) {
            shape_set->blend.bp[i] = 0.0f;
        }
    } else {
        shape_set->blend.bl = 0.0f;
    }
    return shape_set;
}
#pragma pop

#pragma push
#pragma force_active on
static s32 PObjLoad(HSD_PObj *pobj, HSD_PObjDesc *desc)
{
    pobj->next = HSD_PObjLoadDesc(desc->next);
    pobj->verts = desc->verts;
    pobj->flags = desc->flags;
    pobj->n_display = desc->n_display;
    pobj->display = desc->display;
    switch (pobj_type(pobj)) {
    case POBJ_SHAPEANIM:
        pobj->u.shape_set = loadShapeSetDesc(desc->u.shape_set);
        break;

    case POBJ_ENVELOPE:
        pobj->u.envelope_list = loadEnvelopeDesc(desc->u.envelope_p);
        break;

    case POBJ_SKIN:
        break;

    default:
        HSD_Panic("pobj.c", 554, "pobj: unexected type.\n");
    }

    _HSD_NeedCacheInvalidate(1);

    return 0;
}
#pragma pop

HSD_PObj *HSD_PObjLoadDesc(HSD_PObjDesc *pobjdesc)
{
    if (pobjdesc != NULL) {
        HSD_PObj *pobj;
        HSD_ClassInfo *info;

        if (!pobjdesc->class_name ||
            !(info = hsdSearchClassInfo(pobjdesc->class_name))) {
            pobj = HSD_PObjAlloc();
        } else {
            pobj = hsdNew(info);
            HSD_ASSERT(583, pobj);
        }
        HSD_POBJ_METHOD(pobj)->load(pobj, pobjdesc);
        return pobj;
    } else {
        return NULL;
    }
}

static inline void HSD_PObjRemove(HSD_PObj *pobj)
{
    if (pobj) {
        pobj->parent.class_info->release((HSD_Class *)pobj);
        pobj->parent.class_info->destroy((HSD_Class *)pobj);
    }
}

void HSD_PObjRemoveAll(HSD_PObj *pobj)
{
    HSD_PObj *next;

    while (pobj != NULL) {
        next = pobj->next;
        HSD_PObjRemove(pobj);
        pobj = next;
    }
}

HSD_PObj *HSD_PObjAlloc(void)
{
    HSD_PObj *pobj = hsdNew(default_class ? &default_class->parent : &hsdPObj.parent);
    /* MWCC retained this unused assertion string in the original data pool. */
    (void)"hsdIsDescendantOf(info, &hsdPObj)";
    HSD_ASSERT(681, pobj);
    return pobj;
}

static inline void HSD_JObjRefThis(HSD_JObj *jobj)
{
    s32 has_joint = jobj != NULL;
    if (has_joint) {
        u16 *count = &jobj->individual_reference_count;
        ++*count;
        if (!*count)
            __assert("object.h", 158, "HSD_OBJ(o)->ref_count_individual != 0");
    }
}
static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    s32 result;
    if (!jobj)
        __assert("jobj.h", 605, "jobj");
    result = 0;
    if (!(jobj->flags & JOBJ_USER_DEF_MTX) && (jobj->flags & JOBJ_MTX_DIRTY))
        result = 1;
    return result;
}
static inline void HSD_JObjSetupMatrix(HSD_JObj *jobj)
{
    if (!jobj || !HSD_JObjMtxIsDirty(jobj))
        return;
    HSD_JObjSetupMatrixSub(jobj);
}
#pragma push
#pragma force_active on
static void resolveEnvelope(HSD_SList *list, HSD_EnvelopeDesc **edesc_p)
{
    s32 has_list = list != NULL;
    if (!has_list || edesc_p == NULL) {
        return;
    }

    for (; list && *edesc_p; list = list->next, edesc_p++) {
        HSD_Envelope *env = list->data;
        HSD_EnvelopeDesc *edesc = *edesc_p;

        while (env && edesc->joint) {
            HSD_JObjUnrefThis(env->jobj);
            env->jobj = HSD_IDGetDataFromTable(NULL, (u32)edesc->joint, NULL);
            HSD_ASSERT(714, env->jobj);
            HSD_JObjRefThis(env->jobj);
            env = env->next;
            edesc++;
        }
    }
}
#pragma pop

static inline void HSD_PObjResolveRefs(HSD_PObj *pobj, HSD_PObjDesc *pdesc)
{
    if (!pobj || !pdesc) {
        return;
    }

    switch (pobj_type(pobj)) {
    case POBJ_ENVELOPE:
        resolveEnvelope(pobj->u.envelope_list, pdesc->u.envelope_p);
        break;

    case POBJ_SKIN:
        HSD_JObjUnrefThis(pobj->u.jobj);
        pobj->u.jobj = NULL;
        if (pdesc->u.joint != NULL) {
            pobj->u.jobj = HSD_IDGetDataFromTable(NULL, (u32)pdesc->u.joint, NULL);
            HSD_ASSERT(741, pobj->u.jobj);
            HSD_JObjRefThis(pobj->u.jobj);
        }
        break;

    default:
        break;
    }
}

void HSD_PObjResolveRefsAll(HSD_PObj *pobj, HSD_PObjDesc *pdesc)
{
    for (; pobj != NULL && pdesc != NULL; pobj = pobj->next, pdesc = pdesc->next) {
        HSD_PObjResolveRefs(pobj, pdesc);
    }
}

void HSD_ClearVtxDesc(void)
{
    GXClearVtxDesc();
    prev_vtxdesclist_array = 0;
    prev_vtxdesc = 0;
}

#pragma push
#pragma force_active on
static void setupArrayDesc(HSD_VtxDescList *desc_list)
{
    HSD_VtxDescList *desc;

    if (prev_vtxdesclist_array != desc_list) {
        for (desc = desc_list; desc->attr != GX_VA_NULL; desc++) {
            if (desc->attr_type != GX_DIRECT) {
                GXSetArray(desc->attr, desc->vertex, desc->stride);
            }
        }
        prev_vtxdesclist_array = desc_list;
    }
}
#pragma pop

#pragma push
#pragma force_active on
static void setupVtxDesc(HSD_PObj *pobj)
{
    HSD_VtxDescList *desc;

    if (prev_vtxdesc != pobj->verts) {
        GXClearVtxDesc();
        for (desc = pobj->verts; desc->attr != GX_VA_NULL; desc++) {
            GXSetVtxDesc(desc->attr, desc->attr_type);
            switch (desc->attr) {
            case GX_VA_PNMTXIDX:
            case GX_VA_TEX0MTXIDX:
            case GX_VA_TEX1MTXIDX:
            case GX_VA_TEX2MTXIDX:
            case GX_VA_TEX3MTXIDX:
            case GX_VA_TEX4MTXIDX:
            case GX_VA_TEX5MTXIDX:
            case GX_VA_TEX6MTXIDX:
            case GX_VA_TEX7MTXIDX:
                break;
            default:
                GXSetVtxAttrFmt(GX_VTXFMT0, desc->attr, desc->comp_cnt, desc->comp_type,
                                desc->frac);
            }
        }
        prev_vtxdesc = pobj->verts;
    }
}
#pragma pop

#pragma push
#pragma force_active on
static void setupShapeAnimArrayDesc(HSD_VtxDescList *desc_list)
{
    HSD_VtxDescList *desc;

    for (desc = desc_list; desc->attr != GX_VA_NULL; desc++) {
        if (desc->attr_type != GX_DIRECT) {
            switch (desc->attr) {
            case GX_VA_POS:
            case GX_VA_NRM:
            case GX_VA_NBT:
                break;
            default:
                GXSetArray(desc->attr, desc->vertex, desc->stride);
            }
        }
    }
    prev_vtxdesclist_array = NULL;
}
#pragma pop

#pragma push
#pragma force_active on
static void setupShapeAnimVtxDesc(HSD_PObj *pobj)
{
    HSD_VtxDescList *desc;

    GXClearVtxDesc();
    for (desc = pobj->verts; desc->attr != GX_VA_NULL; desc++) {
        switch (desc->attr) {
        case GX_VA_NRM:
        case GX_VA_POS:
        case GX_VA_NBT:
            GXSetVtxDesc(desc->attr, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, desc->attr, desc->comp_cnt, GX_F32, 0);
            break;

        case GX_VA_PNMTXIDX:
        case GX_VA_TEX0MTXIDX:
        case GX_VA_TEX1MTXIDX:
        case GX_VA_TEX2MTXIDX:
        case GX_VA_TEX3MTXIDX:
        case GX_VA_TEX4MTXIDX:
        case GX_VA_TEX5MTXIDX:
        case GX_VA_TEX6MTXIDX:
        case GX_VA_TEX7MTXIDX:
            GXSetVtxDesc(desc->attr, desc->attr_type);
            break;

        default:
            GXSetVtxDesc(desc->attr, desc->attr_type);
            GXSetVtxAttrFmt(GX_VTXFMT0, desc->attr, desc->comp_cnt, desc->comp_type,
                            desc->frac);
        }
    }
    prev_vtxdesc = NULL;
}
#pragma pop

static inline void decode_u8_xyz(void *src_base, f32 dst[3], int scale)
{
    u8 *src = src_base;
    dst[0] = (f32)src[0] / scale;
    dst[1] = (f32)src[1] / scale;
    dst[2] = (f32)src[2] / scale;
}

static inline void decode_s8_xyz(void *src_base, f32 dst[3], int scale)
{
    s8 *src = src_base;
    dst[0] = (f32)src[0] / scale;
    dst[1] = (f32)src[1] / scale;
    dst[2] = (f32)src[2] / scale;
}

static inline void decode_u16_xyz(void *src_base, f32 dst[3], int scale)
{
    u16 *src = src_base;
    dst[0] = (f32)src[0] / scale;
    dst[1] = (f32)src[1] / scale;
    dst[2] = (f32)src[2] / scale;
}

static inline void decode_s16_xyz(void *src_base, f32 dst[3], int scale)
{
    s16 *src = src_base;
    dst[0] = (f32)src[0] / scale;
    dst[1] = (f32)src[1] / scale;
    dst[2] = (f32)src[2] / scale;
}

#pragma push
#pragma force_active on
static void get_shape_vertex_xyz(HSD_ShapeSet *shape_set, int shape_id, int arrayidx,
                                 f32 dst[3])
{
    u8 *index_array = shape_set->vertex_idx_list[shape_id];
    int idx;
    void *src_base;

    if (shape_set->vertex_desc->attr_type == GX_INDEX16) {
        idx = index_array[arrayidx * 2];
        idx = (idx << 8) + index_array[arrayidx * 2 + 1];
    } else {
        idx = index_array[arrayidx];
    }

    HSD_ASSERT(1082, shape_set->vertex_desc->comp_cnt == GX_POS_XYZ);
    src_base =
        ((u8 *)shape_set->vertex_desc->vertex) + idx * shape_set->vertex_desc->stride;

    if (shape_set->vertex_desc->comp_type == GX_F32) {
        memcpy(dst, src_base, sizeof(f32[3]));
    } else {
        int decimal_point = 1 << shape_set->vertex_desc->frac;
        switch (shape_set->vertex_desc->comp_type) {
        case GX_U8:
            decode_u8_xyz(src_base, dst, decimal_point);
            break;

        case GX_S8:
            decode_s8_xyz(src_base, dst, decimal_point);
            break;

        case GX_U16:
            decode_u16_xyz(src_base, dst, decimal_point);
            break;

        case GX_S16:
            decode_s16_xyz(src_base, dst, decimal_point);
            break;

        default:
            HSD_Panic("pobj.c", 1125, "unexpected vertex type.\n");
        }
    }
}
#pragma pop

#pragma push
#pragma force_active on
static void get_shape_normal_xyz(HSD_ShapeSet *shape_set, int shape_id, int arrayidx,
                                 float dst[3])
{
    u8 *index_array = shape_set->normal_idx_list[shape_id];
    int idx;
    void *src_base;

    if (shape_set->normal_desc->attr_type == GX_INDEX16) {
        idx = index_array[arrayidx * 2];
        idx = (idx << 8) + index_array[arrayidx * 2 + 1];
    } else {
        idx = index_array[arrayidx];
    }

    HSD_ASSERT(1145, shape_set->normal_desc->comp_cnt == GX_NRM_XYZ);
    src_base =
        ((u8 *)shape_set->normal_desc->vertex) + idx * shape_set->normal_desc->stride;

    if (shape_set->normal_desc->comp_type == GX_F32) {
        memcpy(dst, src_base, sizeof(f32[3]));
    } else {
        int decimal_point = 1 << shape_set->normal_desc->frac;
        switch (shape_set->normal_desc->comp_type) {
        case GX_U8:
            decode_u8_xyz(src_base, dst, decimal_point);
            break;
        case GX_S8:
            decode_s8_xyz(src_base, dst, decimal_point);
            break;
        case GX_U16:
            decode_u16_xyz(src_base, dst, decimal_point);
            break;
        case GX_S16:
            decode_s16_xyz(src_base, dst, decimal_point);
            break;
        default:
            HSD_Panic("pobj.c", 1188, "unexpected normal type.");
        }
    }
}
#pragma pop

#pragma push
#pragma force_active on
static void get_shape_nbt_xyz(HSD_ShapeSet *shape_set, int shape_id, int arrayidx,
                              float *dst)
{
    u8 *index_array = shape_set->normal_idx_list[shape_id];
    int i, idx;
    void *src_base;

    HSD_ASSERT(1201, shape_set->normal_desc->attr == GX_VA_NBT);

    if (shape_set->normal_desc->attr_type == GX_INDEX16) {
        idx = index_array[arrayidx * 2];
        idx = (idx << 8) + index_array[arrayidx * 2 + 1];
    } else {
        idx = index_array[arrayidx];
    }

    HSD_ASSERT(1210, shape_set->normal_desc->comp_cnt == GX_NRM_XYZ);

    src_base =
        ((u8 *)shape_set->normal_desc->vertex) + idx * shape_set->normal_desc->stride;

    if (shape_set->normal_desc->comp_type == GX_F32) {
        memcpy(dst, src_base, sizeof(f32[9]));
    } else {
        int decimal_point = 1 << shape_set->normal_desc->frac;
        switch (shape_set->normal_desc->comp_type) {
        case GX_U8:
            for (i = 0; i < 9; i++) {
                dst[i] = (float)((u8 *)src_base)[i] / decimal_point;
            }
            break;
        case GX_S8:
            for (i = 0; i < 9; i++) {
                dst[i] = (float)((s8 *)src_base)[i] / decimal_point;
            }
            break;
        case GX_U16:
            for (i = 0; i < 9; i++) {
                dst[i] = (float)((u16 *)src_base)[i] / decimal_point;
            }
            break;
        case GX_S16:
            for (i = 0; i < 9; i++) {
                dst[i] = (float)((s16 *)src_base)[i] / decimal_point;
            }
            break;
        default:
            HSD_Panic("pobj.c", 1241, "unexpected normal type.");
        }
    }
}
#pragma pop

#pragma push
#pragma force_active on
static void interpretShapeAnimDisplayList(HSD_PObj *pobj, float (*vertex)[3],
                                          float (*normal)[3])
{
    u8 *dl = pobj->display;
    int length = pobj->n_display << 5;
    int l;

    for (l = 0; l + 3 < length;) {
        int n = dl[1] << 8 | dl[2];
        int m = 3;
        int i, j;

        if ((dl[0] & GX_OPCODE_MASK) == GX_NOP) {
            break;
        }
        GXBegin((GXPrimitive)(dl[0] & GX_OPCODE_MASK), (GXVtxFmt)(dl[0] & GX_VAT_MASK),
                n);
        for (i = 0; i < n; i++) {
            for (j = 0;; j++) {
                HSD_VtxDescList *desc = &pobj->verts[j];
                if (desc->attr == GX_VA_NULL) {
                    break;
                } else {
                    u16 idx = dl[m++];
                    switch (desc->attr) {
                    case GX_VA_PNMTXIDX:
                    case GX_VA_TEX0MTXIDX:
                    case GX_VA_TEX1MTXIDX:
                    case GX_VA_TEX2MTXIDX:
                    case GX_VA_TEX3MTXIDX:
                    case GX_VA_TEX4MTXIDX:
                    case GX_VA_TEX5MTXIDX:
                    case GX_VA_TEX6MTXIDX:
                    case GX_VA_TEX7MTXIDX:
                        GXTexCoord1u8(idx);
                        break;

                    case GX_VA_POS:
                        if (desc->attr_type == GX_INDEX16) {
                            idx = (idx << 8) | dl[m++];
                        }
                        GXPosition3f32(vertex[idx][0], vertex[idx][1], vertex[idx][2]);
                        break;

                    case GX_VA_NRM:
                        if (desc->attr_type == GX_INDEX16) {
                            idx = (idx << 8) | dl[m++];
                        }
                        GXNormal3f32(normal[idx][0], normal[idx][1], normal[idx][2]);
                        break;

                    case GX_VA_NBT:
                        if (desc->attr_type == GX_INDEX16) {
                            idx = (idx << 8) | dl[m++];
                        }
                        idx *= 3;
                        GXNormal3f32(normal[idx + 0][0], normal[idx + 0][1],
                                     normal[idx + 0][2]);
                        GXNormal3f32(normal[idx + 1][0], normal[idx + 1][1],
                                     normal[idx + 1][2]);
                        GXNormal3f32(normal[idx + 2][0], normal[idx + 2][1],
                                     normal[idx + 2][2]);
                        break;

                    case GX_VA_TEX0:
                    case GX_VA_TEX1:
                    case GX_VA_TEX2:
                    case GX_VA_TEX3:
                    case GX_VA_TEX4:
                    case GX_VA_TEX5:
                    case GX_VA_TEX6:
                    case GX_VA_TEX7:
                        if (desc->attr_type == GX_INDEX16) {
                            idx = (idx << 8) | dl[m++];
                            GXTexCoord1x16(idx);
                        } else {
                            GXTexCoord1x8(idx);
                        }
                        break;

                    case GX_VA_CLR0:
                    case GX_VA_CLR1:
                        if (desc->attr_type == GX_INDEX16) {
                            idx = (idx << 8) | dl[m++];
                            GXColor1x16(idx);
                        } else if (desc->attr_type == GX_INDEX8) {
                            GXColor1x8(idx);
                        } else {
                            switch (desc->comp_type) {
                            case GX_RGB565:
                            case GX_RGBA4:
                                GXColor1u16((idx << 8) | dl[m++]);
                                break;
                            case GX_RGB8:
                            case GX_RGBA6:
                                GXColor3u8(idx, dl[m], dl[m + 1]);
                                m += 2;
                                break;
                            case GX_RGBA8:
                            case GX_RGBX8:
                                GXColor4u8(idx, dl[m], dl[m + 1], dl[m + 2]);
                                m += 3;
                                break;
                            }
                        }
                        break;
                    default:
                        if (desc->attr_type == GX_INDEX16) {
                            idx = (idx << 8) | dl[m++];
                        }
                        OSReport("attr(%d) is not supported by sysdolphin\n",
                                 desc->attr);
                        break;
                    }
                }
            }
        }
        GXEnd();
        l += m;
        dl += m;
    }
}
#pragma pop

#pragma push
#pragma force_active on
static void drawShapeAnim(HSD_PObj *pobj)
{
    HSD_ShapeSet *shape_set = pobj->u.shape_set;
    f32 blend;
    int shape_id, i;
    int blend_nbt;

    if (vertex_buffer_size == 0) {
        vertex_buffer_size = HSD_DEFAULT_MAX_SHAPE_VERTICES;
        vertex_buffer = HSD_MemAlloc(vertex_buffer_size * sizeof(f32[3]));
    }
    HSD_ASSERT(1387, vertex_buffer_size >= shape_set->nb_vertex_index);
    if (shape_set->normal_desc && normal_buffer_size == 0) {
        normal_buffer_size = HSD_DEFAULT_MAX_SHAPE_NORMALS;
        normal_buffer = HSD_MemAlloc(normal_buffer_size * sizeof(f32[3]));
    }

    if (shape_set->normal_desc) {
        if (shape_set->normal_desc->attr == GX_VA_NRM) {
            HSD_ASSERT(1396, normal_buffer_size >= shape_set->nb_normal_index);
            blend_nbt = 0;
        } else {
            HSD_ASSERT(1399, normal_buffer_size >= shape_set->nb_normal_index * 3);
            blend_nbt = 1;
        }
    }

    if (shape_set->flags & SHAPESET_AVERAGE) {
        blend = shape_set->blend.bl;
        shape_id = pobj_min(MAX(0, (int)blend), shape_set->nb_shape - 1);
        blend = pobj_min(MAX(0.0, blend - (f32)shape_id), 1.0f);
        for (i = 0; i < shape_set->nb_vertex_index; i++) {
            f32 s0[3], s1[3];

            get_shape_vertex_xyz(shape_set, shape_id, i, s0);
            get_shape_vertex_xyz(
                shape_set, pobj_min(shape_id + 1, shape_set->nb_shape - 1), i, s1);
            vertex_buffer[i][0] = (s1[0] - s0[0]) * blend + s0[0];
            vertex_buffer[i][1] = (s1[1] - s0[1]) * blend + s0[1];
            vertex_buffer[i][2] = (s1[2] - s0[2]) * blend + s0[2];
        }
        if (shape_set->nb_normal_index) {
            if (blend_nbt) {
                for (i = 0; i < shape_set->nb_normal_index; i++) {
                    f32 s0[9], s1[9];
                    int j, idx = i * 3;

                    get_shape_nbt_xyz(shape_set, shape_id, i, s0);
                    get_shape_nbt_xyz(shape_set,
                                      pobj_min(shape_id + 1, shape_set->nb_shape - 1),
                                      i, s1);
                    for (j = 0; j < 9; j++) {
                        normal_buffer[idx][j] = (s1[j] - s0[j]) * blend + s0[j];
                    }
                }
            } else {
                for (i = 0; i < shape_set->nb_normal_index; i++) {
                    f32 s0[3], s1[3];

                    get_shape_normal_xyz(shape_set, shape_id, i, s0);
                    get_shape_normal_xyz(
                        shape_set, pobj_min(shape_id + 1, shape_set->nb_shape - 1), i,
                        s1);
                    normal_buffer[i][0] = (s1[0] - s0[0]) * blend + s0[0];
                    normal_buffer[i][1] = (s1[1] - s0[1]) * blend + s0[1];
                    normal_buffer[i][2] = (s1[2] - s0[2]) * blend + s0[2];
                }
            }
        }
    } else {
        int j;
        f32 *blend_bp;
        blend_bp = shape_set->blend.bp;
        for (i = 0; i < shape_set->nb_vertex_index; i++) {
            get_shape_vertex_xyz(shape_set, 0, i, vertex_buffer[i]);
            for (j = 0; j < shape_set->nb_shape; j++) {
                f32 b = MAX(0.0, blend_bp[j]);
                f32 s[3];

                get_shape_vertex_xyz(shape_set, j + 1, i, s);
                vertex_buffer[i][0] += s[0] * b;
                vertex_buffer[i][1] += s[1] * b;
                vertex_buffer[i][2] += s[2] * b;
            }
        }
        if (shape_set->nb_normal_index) {
            if (blend_nbt) {
                for (i = 0; i < shape_set->nb_normal_index; i++) {
                    s32 idx = i * 3;
                    get_shape_nbt_xyz(shape_set, 0, i, normal_buffer[idx]);
                    for (j = 0; j < shape_set->nb_shape; j++) {
                        f32 b = MAX(0.0, blend_bp[j]);
                        f32 s[9];
                        int k;

                        get_shape_nbt_xyz(shape_set, j + 1, i, s);
                        for (k = 0; k < 9; k++) {
                            normal_buffer[idx][k] += s[k] * b;
                        }
                    }
                }
            } else {
                for (i = 0; i < shape_set->nb_normal_index; i++) {
                    get_shape_normal_xyz(shape_set, 0, i, normal_buffer[i]);
                    for (j = 0; j < shape_set->nb_shape; j++) {
                        f32 b = MAX(0.0, blend_bp[j]);
                        f32 s[3];

                        get_shape_normal_xyz(shape_set, j + 1, i, s);
                        normal_buffer[i][0] += s[0] * b;
                        normal_buffer[i][1] += s[1] * b;
                        normal_buffer[i][2] += s[2] * b;
                    }
                }
            }
        }
    }
    interpretShapeAnimDisplayList(pobj, vertex_buffer, normal_buffer);
}
#pragma pop

void HSD_PObjClearMtxMark(void *obj, u32 mark)
{
    int i;
    for (i = 0; i < 2; i++) {
        mtx_mark[i].obj = obj;
        mtx_mark[i].mark = mark;
    }
}

static inline PObjSetupFlag GetSetupFlags(HSD_JObj *jobj, u32 rendermode)
{
    PObjSetupFlag flags = SETUP_NONE;
    {
        if (jobj->flags & JOBJ_LIGHTING) {
            flags |= SETUP_NORMAL;
        }

        if (_HSD_TObjGetCurrentByType(NULL, TEX_COORD_REFLECTION)) {
            flags |= SETUP_NORMAL | SETUP_REFLECTION;
        }

        if (_HSD_TObjGetCurrentByType(NULL, TEX_COORD_HILIGHT)) {
            flags |= SETUP_NORMAL | SETUP_HIGHLIGHT;
        }
    }

    return flags;
}

#pragma push
#pragma force_active on
/* This routine references the cache object without a BSS-pool base. */
#pragma pool_data off
static void SetupRigidModelMtx(HSD_PObj *pobj, Mtx vmtx, Mtx pmtx, u32 rendermode)
{
    HSD_JObj *jobj;
    Mtx n;
    PObjSetupFlag flags;

    jobj = HSD_JObjGetCurrent();

    {
        void *obj;
        u32 mark;

        obj = mtx_mark[0].obj;
        mark = mtx_mark[0].mark;
        if (obj == jobj && mark == HSD_MTX_RIGID) {
            return;
        }
    }

    GXSetCurrentMtx(GX_PNMTX0);
    GXLoadPosMtxImm(pmtx, GX_PNMTX0);
    HSD_PerfCountMtxLoad();

    flags = GetSetupFlags(jobj, rendermode);

    if (flags & SETUP_NORMAL) {
        HSD_MtxInverseTranspose(pmtx, n);
        if (jobj->flags & JOBJ_LIGHTING) {
            GXLoadNrmMtxImm(n, GX_PNMTX0);
            HSD_PerfCountMtxLoad();
        }
        if (flags & SETUP_NORMAL_PROJECTION) {
            GXLoadTexMtxImm(n, GX_TEXMTX0, GX_MTX3x4);
            HSD_PerfCountMtxLoad();
        }
    }
}
#pragma pop

#pragma push
#pragma force_active on
static void SetupSharedVtxModelMtx(HSD_PObj *pobj, Mtx vmtx, Mtx pmtx, u32 rendermode)
{
    HSD_JObj *jobj;
    Mtx n0, n1, m;
    PObjSetupFlag flags = SETUP_NONE;

    jobj = HSD_JObjGetCurrent();
    {
        void *obj;
        u32 mark;

        obj = mtx_mark[0].obj;
        mark = mtx_mark[0].mark;
        if (obj != jobj && mark != HSD_MTX_RIGID) {
            flags |= SETUP_JOINT0;
        }

        obj = mtx_mark[1].obj;
        mark = mtx_mark[1].mark;
        if (obj != pobj->u.jobj && mark != HSD_MTX_RIGID) {
            flags |= SETUP_JOINT1;
        }
    }

    if (flags == SETUP_NONE) {
        return;
    }

    flags |= GetSetupFlags(jobj, rendermode);

    /* Preserve the shipped bitwise-OR branches and their register lifetimes. */
    if (flags | SETUP_JOINT0) {
        GXSetCurrentMtx(GX_PNMTX0);
        GXLoadPosMtxImm(pmtx, GX_PNMTX0);
        HSD_PerfCountMtxLoad();

        if (flags & SETUP_NORMAL) {
            HSD_MtxInverseTranspose(pmtx, n0);
            if (jobj->flags & JOBJ_LIGHTING) {
                GXLoadNrmMtxImm(n0, GX_PNMTX0);
                HSD_PerfCountMtxLoad();
            }
            if (flags & SETUP_NORMAL_PROJECTION) {
                GXLoadTexMtxImm(n0, GX_TEXMTX0, GX_MTX3x4);
                HSD_PerfCountMtxLoad();
            }
        }
    }
    if (flags | SETUP_JOINT1) {
        u32 needs_normal;
        HSD_JObjSetupMatrix(pobj->u.jobj);
        PSMTXConcat(vmtx, pobj->u.jobj->mtx, m);
        GXLoadPosMtxImm(m, GX_PNMTX1);
        HSD_PerfCountMtxLoad();

        needs_normal = flags & SETUP_NORMAL;
        if (needs_normal) {
            HSD_MtxInverseTranspose(m, n1);
            if (jobj->flags & JOBJ_LIGHTING) {
                GXLoadNrmMtxImm(n1, GX_PNMTX1);
                HSD_PerfCountMtxLoad();
            }
            if (flags & SETUP_NORMAL_PROJECTION) {
                GXLoadTexMtxImm(n1, GX_TEXMTX1, GX_MTX3x4);
                HSD_PerfCountMtxLoad();
            }
        }
    }
}
#pragma pop

#pragma push
#pragma force_active on
static void SetupEnvelopeModelMtx(HSD_PObj *pobj, Mtx vmtx, Mtx pmtx, u32 rendermode)
{
    HSD_JObj *jobj;
    HSD_SList *list;
    int MtxIdx = 0;
    MtxPtr right;
    Mtx mtx;
    PObjSetupFlag flags = SETUP_NONE;

    jobj = HSD_JObjGetCurrent();
    HSD_PObjClearMtxMark(NULL, HSD_MTX_ENVELOPE);
    flags = GetSetupFlags(jobj, rendermode);
    right = _HSD_mkEnvelopeModelNodeMtx(jobj, mtx);

    for (MtxIdx = 0, list = pobj->u.envelope_list; MtxIdx < 10 && list;
         MtxIdx++, list = list->next) {
        Mtx mtx, tmp;
        MtxPtr mtxp;
        HSD_Envelope *envelope = list->data;
        s32 mtx_no = HSD_Index2PosNrmMtx(MtxIdx);
        int perf = 0;

        HSD_ASSERT(1817, envelope);
        if (envelope->weight >= 1.0f) {
            HSD_JObjSetupMatrix(envelope->jobj);
            if (right) {
                MTXConcat(envelope->jobj->mtx, envelope->jobj->envelopemtx, mtx);
                mtxp = mtx;
            } else {
                mtxp = envelope->jobj->mtx;
            }
        } else {
            mtx[0][0] = mtx[0][1] = mtx[0][2] = mtx[0][3] = mtx[1][0] = mtx[1][1] =
                mtx[1][2] = mtx[1][3] = mtx[2][0] = mtx[2][1] = mtx[2][2] = mtx[2][3] =
                    0.0f;
            while (envelope) {
                HSD_JObj *jp;

                HSD_ASSERT(1837, envelope->jobj);
                jp = envelope->jobj;
                HSD_JObjSetupMatrix(jp);
                HSD_ASSERT(1840, jp->mtx);
                HSD_ASSERT(1841, jp->envelopemtx);

                MTXConcat(jp->mtx, jp->envelopemtx, tmp);
                HSD_MtxScaledAdd(tmp, mtx, mtx, envelope->weight);
                perf++;
                envelope = envelope->next;
            }
            mtxp = mtx;
        }
        HSD_PerfCountEnvelopeBlending(perf);
        if (right) {
            MTXConcat(mtxp, right, mtx);
        }
        MTXConcat(vmtx, mtxp, tmp);
        GXLoadPosMtxImm(tmp, mtx_no);
        HSD_PerfCountMtxLoad();

        if (flags & SETUP_NORMAL) {
            HSD_MtxInverseTranspose(tmp, mtx);
            if (jobj->flags & JOBJ_LIGHTING) {
                GXLoadNrmMtxImm(mtx, mtx_no);
                HSD_PerfCountMtxLoad();
            }
            if (flags & SETUP_NORMAL_PROJECTION) {
                GXLoadTexMtxImm(mtx, HSD_Index2TexMtx(MtxIdx), GX_MTX3x4);
                HSD_PerfCountMtxLoad();
            }
        }
    }
}
#pragma pop

#pragma push
#pragma force_active on
static void PObjSetupMtx(HSD_PObj *pobj, Mtx vmtx, Mtx pmtx, u32 rendermode)
{
    switch (pobj_type(pobj)) {
    case POBJ_SKIN:
        if (!pobj->u.jobj) {
            SetupRigidModelMtx(pobj, vmtx, pmtx, rendermode);
        } else {
            SetupSharedVtxModelMtx(pobj, vmtx, pmtx, rendermode);
        }
        break;
    case POBJ_SHAPEANIM:
        SetupRigidModelMtx(pobj, vmtx, pmtx, rendermode);
        break;
    case POBJ_ENVELOPE:
        SetupEnvelopeModelMtx(pobj, vmtx, pmtx, rendermode);
        break;
    }
}
#pragma pop

static inline void PObjDispSimplePrimitive(HSD_PObj *pobj, u32 rendermode)
{
    setupArrayDesc(pobj->verts);
    setupVtxDesc(pobj);

    GXCallDisplayList(pobj->display, pobj->n_display << 5);
}

static inline void PObjDispShapeAnim(HSD_PObj *pobj, u32 rendermode)
{
    setupShapeAnimArrayDesc(pobj->verts);
    setupShapeAnimVtxDesc(pobj);

    HSD_ASSERT(1914, pobj->u.shape_set);
    drawShapeAnim(pobj);
}

void HSD_PObjDisp(HSD_PObj *pobj, Mtx vmtx, Mtx pmtx, u32 rendermode)
{
    switch (pobj->flags & (POBJ_CULLFRONT | POBJ_CULLBACK)) {
    case 0x0:
        HSD_StateSetCullMode(GX_CULL_NONE);
        break;
    case POBJ_CULLFRONT:
        HSD_StateSetCullMode(GX_CULL_FRONT);
        break;
    case POBJ_CULLBACK:
        HSD_StateSetCullMode(GX_CULL_BACK);
        break;
    case POBJ_CULLFRONT | POBJ_CULLBACK:
        return;
    }

    HSD_POBJ_METHOD(pobj)->setup_mtx(pobj, vmtx, pmtx, rendermode);
    if (pobj_type(pobj) == POBJ_SHAPEANIM) {
        PObjDispShapeAnim(pobj, rendermode);
    } else {
        PObjDispSimplePrimitive(pobj, rendermode);
    }
}

static inline void HSD_EnvelopeFree(HSD_Envelope *env)
{
    if (env != NULL) {
        hsdFreeMemPiece(env, sizeof(HSD_Envelope));
    }
}

static inline void HSD_EnvelopeListFree(HSD_SList *list)
{
    while (list) {
        HSD_Envelope *env = list->data;
        while (env) {
            HSD_Envelope *next = env->next;
            HSD_JObjUnrefThis(env->jobj);
            HSD_EnvelopeFree(env);
            env = next;
        }
        list = HSD_SListRemove(list);
    }
}

static inline void HSD_ShapeSetFree(HSD_ShapeSet *shape_set)
{
    if (!shape_set) {
        return;
    }
    hsdFreeMemPiece(shape_set, sizeof(HSD_ShapeSet));
}

static inline void HSD_ShapeSetRemove(HSD_ShapeSet *shape_set)
{
    if (shape_set == NULL) {
        return;
    }

    if (shape_set->flags & SHAPESET_ADDITIVE) {
        HSD_Free(shape_set->blend.bp);
    }

    HSD_ShapeSetFree(shape_set);
}

#pragma push
#pragma force_active on
static void PObjRelease(HSD_Class *o)
{
    HSD_PObj *pobj = HSD_POBJ(o);

    if (pobj->aobj)
        HSD_AObjRemove(pobj->aobj);
    switch (pobj_type(pobj)) {
    case POBJ_SHAPEANIM:
        HSD_ShapeSetRemove(pobj->u.shape_set);
        break;
    case POBJ_ENVELOPE:
        HSD_EnvelopeListFree(pobj->u.envelope_list);
        break;
    case POBJ_SKIN:
        HSD_JObjUnrefThis(pobj->u.jobj);
        break;
    default:
        break;
    }
    hsdPObj.parent.head.parent->release(o);
}
#pragma pop

#pragma push
#pragma force_active on
static void PObjAmnesia(HSD_ClassInfo *info)
{
    void *low, *high;
    if (info == (HSD_ClassInfo *)default_class)
        default_class = NULL;
    if (info == (HSD_ClassInfo *)&hsdPObj) {
        HSD_GetCurrentArena(&low, &high);
        if (low <= (void *)vertex_buffer && (void *)vertex_buffer < high) {
            vertex_buffer = NULL;
            vertex_buffer_size = 0;
        }
        if (low <= (void *)normal_buffer && (void *)normal_buffer < high) {
            normal_buffer = NULL;
            normal_buffer_size = 0;
        }
        prev_vtxdesclist_array = NULL;
        prev_vtxdesc = NULL;
    }
    hsdPObj.parent.head.parent->forget(info);
}
#pragma pop

#pragma push
#pragma force_active on
static void PObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdPObj), HSD_CLASS_INFO(&hsdClass),
                     "sysdolphin_base_library", "hsd_pobj", sizeof(HSD_PObjInfo),
                     sizeof(HSD_PObj));
    HSD_CLASS_INFO(&hsdPObj)->release = PObjRelease;
    HSD_CLASS_INFO(&hsdPObj)->forget = PObjAmnesia;
    HSD_POBJ_INFO(&hsdPObj)->disp = HSD_PObjDisp;
    HSD_POBJ_INFO(&hsdPObj)->setup_mtx = PObjSetupMtx;
    HSD_POBJ_INFO(&hsdPObj)->load = PObjLoad;
    hsdPObj.update = PObjUpdateFunc;
}
#pragma pop
