/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/lobj.c. */
#include "lobj.h"
#include "cobj.h"
#define NULL 0
#define true 1
#define false 0
#define MTXMultVec PSMTXMultVec
#define HSD_LOBJ(o) ((HSD_LObj *)(o))
#define HSD_LOBJ_INFO(i) ((HSD_LObjInfo *)(i))
#define HSD_CLASS_INFO(i) ((HSD_ClassInfo *)(i))
#define HSD_OBJECT_METHOD(o) (((HSD_Class *)(o))->class_info)
#define HSD_LOBJ_METHOD(o) ((HSD_LObjInfo *)HSD_OBJECT_METHOD(o))
#define HSD_OBJECT_PARENT_INFO(i) ((i)->parent.head.parent)
#define HSD_ASSERT(line, condition)                                                    \
    do {                                                                               \
        if ((condition) == 0)                                                          \
            __assert("lobj.c", line, #condition);                                      \
    } while (0)
static void LObjInfoInit(void);
static void LObjUpdateFunc(void *, s32, AnimationValue *);
HSD_LObjInfo hsdLObj = {{LObjInfoInit}};
static HSD_LObjInfo *default_class;
static HSD_SList *current_lights;
static HSD_LObj *active_lights[MAX_GXLIGHT];
static s32 nb_active_lights;
static s32 lightmask_diffuse;
static s32 lightmask_specular;
static s32 lightmask_attnfunc;
static s32 lightmask_alpha;
static inline s32 decrement_reference(void *object)
{
    s32 result;
    u16 *count = &((HSD_Obj *)object)->reference_count;
    if ((result = (*count == (u16)-1)))
        return result;
    return (*count)-- == 0;
}
#define ref_DEC(o) decrement_reference(o)
static inline void increment_reference(HSD_Obj *object)
{
    if (object != 0) {
        object->reference_count++;
        if (object->reference_count == 65535)
            __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF");
    }
}
#define ref_INC(o) increment_reference((HSD_Obj *)(o))
static inline void HSD_WObjUnref(HSD_WObj *object)
{
    if (object != NULL && ref_DEC(object)) {
        if (object != NULL) {
            HSD_OBJECT_METHOD(object)->release((HSD_Class *)object);
            HSD_OBJECT_METHOD(object)->destroy((HSD_Class *)object);
        }
    }
}
static inline u8 HSD_LObjGetPriority(HSD_LObj *lobj)
{
    if (!lobj)
        __assert("lobj.h", 355, "lobj");
    return lobj->priority;
}

static inline u32 HSD_LObjGetFlags(HSD_LObj *lobj)
{
    return lobj ? lobj->flags : 0;
}

static inline void HSD_LObjSetFlags(HSD_LObj *lobj, u32 flags)
{
    if (lobj == NULL) {
        return;
    }
    lobj->flags |= flags;
}

static inline u32 HSD_LObjGetType(HSD_LObj *lobj)
{
    return lobj->flags & LOBJ_TYPE_MASK;
}

static inline void HSD_LObjSetActive(HSD_LObj *lobj)
{
    int idx;

    if (HSD_LObjGetType(lobj) == LOBJ_AMBIENT) {
        idx = MAX_GXLIGHT - 1;
        if (active_lights[idx]) {
            return;
        }
    } else {
        idx = nb_active_lights++;
    }
    active_lights[idx] = lobj;
    lobj->id = HSD_Index2LightID(idx);
}

static inline void HSD_LObjClearActive(void)
{
    int i;

    for (i = 0; i < MAX_GXLIGHT; i++) {
        active_lights[i] = NULL;
    }
    nb_active_lights = 0;
}

static inline void HSD_LObjAnim(HSD_LObj *lobj)
{
    if (lobj != NULL) {
        HSD_AObjInterpretAnim(lobj->aobj, lobj, HSD_LOBJ_METHOD(lobj)->update);
        HSD_WObjInterpretAnim(HSD_LObjGetPositionWObj(lobj));
        HSD_WObjInterpretAnim(HSD_LObjGetInterestWObj(lobj));
    }
}

static inline void HSD_LObjReqAnim(HSD_LObj *lobj, f32 startframe)
{
    if (lobj == NULL) {
        return;
    }

    HSD_AObjReqAnim(lobj->aobj, startframe);
    HSD_WObjReqAnim(HSD_LObjGetPositionWObj(lobj), startframe);
    HSD_WObjReqAnim(HSD_LObjGetInterestWObj(lobj), startframe);
}

static inline void setup_diffuse_lightobj(HSD_LObj *lobj)
{
    int diffuse;

    GXInitLightColor(&lobj->lightobj, lobj->color);
    lobj->hw_color = lobj->color;
    lobj->flags |= LOBJ_DIFF_DIRTY;

    switch (HSD_LObjGetType(lobj)) {
    case LOBJ_SPOT:
    case LOBJ_POINT:
        lightmask_attnfunc |= lobj->id;
        break;
    case LOBJ_INFINITE:
        break;
    default:
        HSD_ASSERT(643, 0);
    }

    diffuse = lobj->flags & LOBJ_DIFFUSE;
    if (diffuse) {
        lightmask_diffuse |= lobj->id;
    }
    if (lobj->flags & LOBJ_ALPHA) {
        lightmask_alpha |= lobj->id;
    }
}

static inline void setup_infinite_lightobj(HSD_LObj *lobj, MtxPtr vmtx)
{
    Vec3 lpos;

    HSD_LObjGetPosition(lobj, &lpos);
    lpos.x *= 1048576.0F;
    lpos.y *= 1048576.0F;
    lpos.z *= 1048576.0F;
    MTXMultVec(vmtx, &lpos, &lpos);
    if (lobj->flags & LOBJ_DIFFUSE) {
        GXInitLightPos(&lobj->lightobj, lpos.x, lpos.y, lpos.z);
        GXInitLightAttn(&lobj->lightobj, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    }
    if (lobj->flags & LOBJ_SPECULAR) {
        GXInitLightPos(&lobj->spec_lightobj, lpos.x, lpos.y, lpos.z);
    }
}

static inline void HSD_LObjUnrefThis(HSD_LObj *lobj)
{
    if (lobj != NULL && ref_DEC(lobj)) {
        if (lobj != NULL) {
            HSD_OBJECT_METHOD(lobj)->release((HSD_Class *)lobj);
            HSD_OBJECT_METHOD(lobj)->destroy((HSD_Class *)lobj);
        }
    }
}

static inline void LObjRemoveAll(void)
{
    int i;
    for (i = 0; i < MAX_GXLIGHT; i++) {
        active_lights[i] = NULL;
    }
    nb_active_lights = 0;
    while (current_lights != NULL) {
        HSD_LObjUnrefThis(current_lights->data);
        current_lights = HSD_SListRemove(current_lights);
    }
}

static inline void HSD_LObjSetColor(HSD_LObj *lobj, GXColor color)
{
    lobj->color = color;
}

static inline void HSD_LObjSetSpot(HSD_LObj *lobj, f32 cutoff, s32 point_func)
{
    if (lobj != NULL) {
        lobj->u.point.cutoff = cutoff;
        lobj->u.point.point_func = point_func;
    }
}

static inline void HSD_LObjSetDistAttn(HSD_LObj *lobj, f32 ref_dist, f32 ref_br,
                                       s32 dist_func)
{
    if (lobj != NULL) {
        lobj->u.point.ref_dist = ref_dist;
        lobj->u.point.ref_br = ref_br;
        lobj->u.point.dist_func = dist_func;
    }
}

static inline void HSD_LObjSetAttnA(HSD_LObj *lobj, f32 a0, f32 a1, f32 a2)
{
    if (lobj == NULL) {
        return;
    }

    lobj->u.attn.a0 = a0;
    lobj->u.attn.a1 = a1;
    lobj->u.attn.a2 = a2;
}

static inline void HSD_LObjSetAttnK(HSD_LObj *lobj, f32 k0, f32 k1, f32 k2)
{
    if (lobj == NULL) {
        return;
    }

    lobj->u.attn.k0 = k0;
    lobj->u.attn.k1 = k1;
    lobj->u.attn.k2 = k2;
}

static inline void HSD_LObjSetAttn(HSD_LObj *lobj, f32 a0, f32 a1, f32 a2, f32 k0,
                                   f32 k1, f32 k2)
{
    HSD_LObjSetAttnA(lobj, a0, a1, a2);
    HSD_LObjSetAttnK(lobj, k0, k1, k2);
}

static inline HSD_LObjInfo *HSD_LObjGetDefaultClass(void)
{
    return default_class ? default_class : &hsdLObj;
}

static inline HSD_LObj *HSD_LObjAlloc(void)
{
    HSD_LObj *new = hsdNew(&HSD_LObjGetDefaultClass()->parent);
    new ? (void)0 : __assert("lobj.c", 1467, "new");
    return new;
}

static inline void HSD_LObjSetPositionWObj(HSD_LObj *lobj, HSD_WObj *wobj)
{
    if (lobj == NULL) {
        return;
    }

    HSD_WObjUnref(lobj->position);
    lobj->position = wobj;
}

static inline void HSD_LObjSetInterestWObj(HSD_LObj *lobj, HSD_WObj *wobj)
{
    if (lobj == NULL) {
        return;
    }

    HSD_WObjUnref(lobj->interest);
    lobj->interest = wobj;
}

GXLightID HSD_LObjGetLightMaskDiffuse(void)
{
    return lightmask_diffuse;
}

s32 HSD_LObjGetLightMaskAttnFunc(void)
{
    return lightmask_attnfunc;
}

s32 HSD_LObjGetLightMaskAlpha(void)
{
    return lightmask_alpha;
}

s32 HSD_LObjGetLightMaskSpecular(void)
{
    return lightmask_specular;
}

s32 HSD_LObjGetNbActive(void)
{
    return nb_active_lights;
}

HSD_LObj *HSD_LObjGetActiveByID(GXLightID id)
{
    s32 idx = HSD_LightID2Index(id);
    if (0 <= idx && idx < MAX_GXLIGHT) {
        return active_lights[idx];
    } else {
        return NULL;
    }
}

HSD_LObj *HSD_LObjGetActiveByIndex(s32 idx)
{
    if (0 <= idx && idx < MAX_GXLIGHT - 1) {
        return active_lights[idx];
    } else {
        return NULL;
    }
}

static void LObjUpdateFunc(void *obj, s32 type, AnimationValue *val)
{
    HSD_LObj *lobj = obj;

    if (lobj == NULL) {
        return;
    }

    switch (type) {
    case HSD_A_L_VIS:
        if (val->scalar >= 0.5) {
            lobj->flags &= ~LOBJ_HIDDEN;
        } else {
            lobj->flags |= LOBJ_HIDDEN;
        }
        break;
    case HSD_A_L_A0:
    case HSD_A_L_CUTOFF:
        if (lobj->flags & LOBJ_RAW_PARAM) {
            lobj->u.attn.a0 = val->scalar;
        } else {
            lobj->u.spot.cutoff = val->scalar;
        }
        break;
    case HSD_A_L_A1:
    case HSD_A_L_REFDIST:
        if (lobj->flags & LOBJ_RAW_PARAM) {
            lobj->u.attn.a1 = val->scalar;
        } else {
            lobj->u.spot.ref_dist = val->scalar;
        }
        break;
    case HSD_A_L_A2:
    case HSD_A_L_REFBRIGHT:
        if (lobj->flags & LOBJ_RAW_PARAM) {
            lobj->u.attn.a2 = val->scalar;
        } else {
            lobj->u.spot.ref_br = val->scalar;
        }
        break;
    case HSD_A_L_K0:
        if (lobj->flags & LOBJ_RAW_PARAM) {
            lobj->u.attn.k0 = val->scalar;
        }
        break;
    case HSD_A_L_K1:
        if (lobj->flags & LOBJ_RAW_PARAM) {
            lobj->u.attn.k1 = val->scalar;
        }
        break;
    case HSD_A_L_K2:
        if (lobj->flags & LOBJ_RAW_PARAM) {
            lobj->u.attn.k2 = val->scalar;
        }
        break;
    case HSD_A_L_LITC_R:
        lobj->color.r = 255.0 * val->scalar;
        break;
    case HSD_A_L_LITC_G:
        lobj->color.g = 255.0 * val->scalar;
        break;
    case HSD_A_L_LITC_B:
        lobj->color.b = 255.0 * val->scalar;
        break;
    case HSD_A_L_LITC_A:
        lobj->color.a = 255.0 * val->scalar;
        break;
    }
}

void HSD_LObjAnimAll(HSD_LObj *lobj)
{
    HSD_LObj *lp;

    if (lobj == NULL) {
        return;
    }

    for (lp = lobj; lp; lp = lp->next) {
        HSD_LObjAnim(lp);
    }
}

void HSD_LObjReqAnimAll(HSD_LObj *lobj, f32 startframe)
{
    HSD_LObj *lp;

    if (lobj == NULL) {
        return;
    }

    for (lp = lobj; lp; lp = lp->next) {
        HSD_LObjReqAnim(lp, startframe);
    }
}

void HSD_LObjGetLightVector(HSD_LObj *lobj, Vec3 *dir)
{
    Vec3 position = {0.0F, 0.0F, 0.0F};
    Vec3 interest = {0.0F, 0.0F, 0.0F};

    if (lobj == NULL) {
        return;
    }

    HSD_LObjGetPosition(lobj, &position);
    HSD_LObjGetInterest(lobj, &interest);
    PSVECSubtract(&interest, &position, dir);
    PSVECNormalize(dir, dir);
}

void HSD_LObjSetup(HSD_LObj *lobj, GXColor color, f32 shininess)
{
    f32 k0 = shininess;

    if (lobj->flags & LOBJ_HIDDEN || HSD_LObjGetType(lobj) == LOBJ_AMBIENT) {
        return;
    }

    if ((lobj->flags & (LOBJ_DIFFUSE | LOBJ_ALPHA)) != 0) {
        if (lobj->hw_color.r != color.r || lobj->hw_color.g != color.g ||
            lobj->hw_color.b != color.b || lobj->hw_color.a != color.a) {
            GXInitLightColor(&lobj->lightobj, color);
            lobj->hw_color = color;
            lobj->flags |= LOBJ_DIFF_DIRTY;
        }

        if (lobj->flags & LOBJ_DIFF_DIRTY) {
            GXLoadLightObjImm(&lobj->lightobj, lobj->id);
            lobj->flags &= ~LOBJ_DIFF_DIRTY;
        }
    }

    if (lobj->spec_id != GX_LIGHT_NULL) {
        if (lobj->shininess != shininess) {
            lobj->shininess = shininess;
            k0 *= 0.5F;
            GXInitLightAttn(&lobj->spec_lightobj, 0.0F, 0.0F, 1.0F, k0, 0.0F,
                            1.0F - k0);
            lobj->flags |= LOBJ_SPEC_DIRTY;
        }

        if (lobj->flags & LOBJ_SPEC_DIRTY) {
            GXLoadLightObjImm(&lobj->spec_lightobj, lobj->spec_id);
            lobj->flags &= ~LOBJ_SPEC_DIRTY;
        }
    }
}

void HSD_LObjSetupSpecularInit(Mtx pmtx)
{
    int i;
    s32 num;
    Vec3 cdir;
    Vec3 jpos;

    jpos.x = pmtx[0][3];
    jpos.y = pmtx[1][3];
    jpos.z = pmtx[2][3];
    PSVECNormalize(&jpos, &cdir);

    num = HSD_LObjGetNbActive();
    for (i = 0; i < num; i++) {
        Vec3 half, ldir;
        HSD_LObj *lobj = HSD_LObjGetActiveByIndex(i);

        if (lobj->spec_id == GX_LIGHT_NULL) {
            continue;
        }

        switch (HSD_LObjGetType(lobj)) {
        case LOBJ_POINT:
        case LOBJ_SPOT:
            PSVECSubtract(&jpos, &lobj->lvec, &ldir);
            PSVECNormalize(&ldir, &ldir);
            PSVECAdd(&ldir, &cdir, &half);
            break;

        case LOBJ_INFINITE:
            PSVECAdd(&lobj->lvec, &cdir, &half);
            break;

        default:
            HSD_ASSERT(619, 0);
        }
        PSVECNormalize(&half, &half);
        GXInitLightDir(&lobj->spec_lightobj, half.x, half.y, half.z);
        lobj->flags |= LOBJ_SPEC_DIRTY;
    }
}

static void setup_spec_lightobj(HSD_LObj *lobj, Mtx mtx, s32 spec_id)
{
    f32 x;

    lobj->spec_id = spec_id;
    if (spec_id != 0) {
        GXInitLightColor(&lobj->spec_lightobj, lobj->color);
        lobj->shininess = 50.0F;

        /* Retain the original floating-point register allocation. */
        x = x = lobj->shininess;

        x *= 0.5F;
        GXInitLightAttn(&lobj->spec_lightobj, 0.0F, 0.0F, 1.0F, x, 0.0F, 1.0F - x);
        switch (HSD_LObjGetType(lobj)) {
        case LOBJ_POINT:
        case LOBJ_SPOT:
            HSD_LObjGetPosition(lobj, &lobj->lvec);
            MTXMultVec(mtx, &lobj->lvec, &lobj->lvec);
            break;
        case LOBJ_INFINITE:
            HSD_LObjGetLightVector(lobj, &lobj->lvec);
            PSMTXMultVecSR(mtx, &lobj->lvec, &lobj->lvec);
            PSVECNormalize(&lobj->lvec, &lobj->lvec);
            break;
        default:
            HSD_ASSERT(683, 0);
        }
        lobj->flags |= LOBJ_SPEC_DIRTY;
        lightmask_specular |= spec_id;
    }
}

static void setup_point_lightobj(HSD_LObj *lobj, Mtx mtx)
{
    Vec3 lpos;

    GXInitLightColor(&lobj->lightobj, lobj->color);
    lobj->hw_color = lobj->color;
    HSD_LObjGetPosition(lobj, &lpos);
    MTXMultVec(mtx, &lpos, &lpos);
    GXInitLightPos(&lobj->lightobj, lpos.x, lpos.y, lpos.z);
    GXInitLightPos(&lobj->spec_lightobj, lpos.x, lpos.y, lpos.z);
    if (lobj->flags & LOBJ_RAW_PARAM) {
        GXInitLightAttn(&lobj->lightobj, 1.0F, 0.0F, 0.0F, lobj->u.attn.k0,
                        lobj->u.attn.k1, lobj->u.attn.k2);
    } else {
        f32 ref_br = lobj->u.spot.ref_br;
        f32 ref_dist = lobj->u.spot.ref_dist;
        s32 dist_func = lobj->u.spot.dist_func;
        GXInitLightDistAttn(&lobj->lightobj, ref_dist, ref_br, dist_func);
        GXInitLightSpot(&lobj->lightobj, 0.0F, 0);
        GXInitLightDistAttn(&lobj->spec_lightobj, ref_dist, ref_br, dist_func);
    }
}

static void setup_spot_lightobj(HSD_LObj *lobj, Mtx mtx)
{
    Vec3 lpos;
    Vec3 ldir;

    HSD_LObjGetPosition(lobj, &lpos);
    MTXMultVec(mtx, &lpos, &lpos);
    HSD_LObjGetLightVector(lobj, &ldir);
    PSMTXMultVecSR(mtx, &ldir, &ldir);
    PSVECNormalize(&ldir, &ldir);
    GXInitLightPos(&lobj->lightobj, lpos.x, lpos.y, lpos.z);
    GXInitLightPos(&lobj->spec_lightobj, lpos.x, lpos.y, lpos.z);
    GXInitLightDir(&lobj->lightobj, ldir.x, ldir.y, ldir.z);
    if (lobj->flags & LOBJ_RAW_PARAM) {
        GXInitLightAttn(&lobj->lightobj, lobj->u.attn.a0, lobj->u.attn.a1,
                        lobj->u.attn.a2, lobj->u.attn.k0, lobj->u.attn.k1,
                        lobj->u.attn.k2);
    } else {
        f32 ref_br = lobj->u.point.ref_br;
        f32 ref_dist = lobj->u.point.ref_dist;
        f32 cutoff = lobj->u.point.cutoff;
        s32 spot_func = lobj->u.spot.spot_func;
        s32 dist_func = lobj->u.spot.dist_func;

        GXInitLightDistAttn(&lobj->lightobj, ref_dist, ref_br, dist_func);
        GXInitLightDistAttn(&lobj->spec_lightobj, ref_dist, ref_br, dist_func);
        GXInitLightSpot(&lobj->lightobj, cutoff, spot_func);
    }
}

void HSD_LObjSetupInit(HSD_CObj *cobj)
{
    MtxPtr vmtx;
    int i, num, idx;
    HSD_SList *list;
    idx = 0;
    lightmask_diffuse = GX_LIGHT_NULL;
    lightmask_specular = GX_LIGHT_NULL;
    lightmask_attnfunc = GX_LIGHT_NULL;
    lightmask_alpha = GX_LIGHT_NULL;
    vmtx = cobj->view_mtx;
    HSD_LObjClearActive();
    for (list = current_lights; idx < MAX_GXLIGHT - 1 && list; list = list->next) {
        HSD_LObj *lobj = list->data;
        u32 flags, type;
        int ref_type;
        if (lobj == NULL)
            continue;
        flags = lobj->flags;
        if (flags & LOBJ_HIDDEN)
            continue;
        type = HSD_LObjGetType(lobj);
        ref_type = flags & (LOBJ_DIFFUSE | LOBJ_SPECULAR | LOBJ_ALPHA);
        if (ref_type == 0)
            continue;
        HSD_LObjSetActive(lobj);
        idx = HSD_LObjGetNbActive();
        lobj->spec_id = GX_LIGHT_NULL;
        switch (type) {
        case LOBJ_INFINITE:
            setup_infinite_lightobj(lobj, vmtx);
            break;
        case LOBJ_POINT:
            setup_point_lightobj(lobj, vmtx);
            break;
        case LOBJ_SPOT:
            setup_spot_lightobj(lobj, vmtx);
            break;
        case LOBJ_AMBIENT:
            continue;
        }
        if (ref_type & (LOBJ_DIFFUSE | LOBJ_ALPHA)) {
            setup_diffuse_lightobj(lobj);
        } else if (ref_type & LOBJ_SPECULAR) {
            setup_spec_lightobj(lobj, vmtx, lobj->id);
        }
    }
    if (!HSD_LObjGetActiveByID(GX_MAX_LIGHT)) {
        for (; list; list = list->next) {
            HSD_LObj *lobj = list->data;
            if (lobj != NULL && !(lobj->flags & LOBJ_HIDDEN) &&
                HSD_LObjGetType(lobj) == LOBJ_AMBIENT &&
                (lobj->flags & (LOBJ_DIFFUSE | LOBJ_ALPHA))) {
                HSD_LObjSetActive(lobj);
                break;
            }
        }
    }
    num = HSD_LObjGetNbActive();
    for (i = 0; idx < MAX_GXLIGHT - 1 && i < num; i++) {
        HSD_LObj *lobj;
        if (!(lobj = HSD_LObjGetActiveByIndex(i)))
            continue;
        if ((lobj->flags & LOBJ_SPECULAR) &&
            (lobj->flags & (LOBJ_DIFFUSE | LOBJ_ALPHA))) {
            setup_spec_lightobj(lobj, vmtx, HSD_Index2LightID(idx++));
        }
    }
    for (i = 0; i < num; i++) {
        HSD_LObj *lobj = HSD_LObjGetActiveByIndex(i);
        if (lobj != NULL)
            HSD_LObjSetup(lobj, lobj->color, lobj->shininess);
    }
}

void HSD_LObjAddCurrent(HSD_LObj *lobj)
{
    HSD_SList *node;
    HSD_SList **p;

    if (lobj != NULL) {
        node = current_lights;
        while (node != NULL) {
            if (node->data == lobj) {
                HSD_LObjDeleteCurrent(lobj);
                break;
            }
            node = node->next;
        }
        ref_INC(lobj);
        for (p = &current_lights; *p != NULL; p = &(*p)->next) {
            u8 priority = HSD_LObjGetPriority(lobj);
            if (HSD_LObjGetPriority((*p)->data) > priority) {
                break;
            }
        }
        *p = HSD_SListPrepend(*p, lobj);
    }
}

void HSD_LObjDeleteCurrent(HSD_LObj *lobj)
{
    if (lobj != NULL) {
        HSD_SList **p;
        for (p = &current_lights; *p != NULL; p = &(*p)->next) {
            if ((*p)->data == lobj) {
                int i;
                for (i = 0; i < MAX_GXLIGHT; i++) {
                    if (lobj == active_lights[i]) {
                        active_lights[i] = NULL;
                    }
                }
                *p = HSD_SListRemove(*p);
                HSD_LObjUnrefThis(lobj);
                return;
            }
        }
    }
}

void HSD_LObjDeleteCurrentAll(HSD_LObj *lobj)
{
    if (lobj != NULL) {
        while (lobj != NULL) {
            HSD_LObjDeleteCurrent(lobj);
            lobj = lobj->next;
        }
        return;
    }
    LObjRemoveAll();
}

static inline void LObjReplaceAll(HSD_LObj *lobj)
{
    int i;
    HSD_LObj *cur;
    for (i = 0; i < MAX_GXLIGHT; i++) {
        active_lights[i] = NULL;
    }
    nb_active_lights = 0;
    while (current_lights != NULL) {
        HSD_LObj *light = current_lights->data;
        s32 has_light = light != NULL;
        if (has_light && ref_DEC(light)) {
            if (light != NULL) {
                HSD_OBJECT_METHOD(light)->release((HSD_Class *)light);
                HSD_OBJECT_METHOD(light)->destroy((HSD_Class *)light);
            }
        }
        current_lights = HSD_SListRemove(current_lights);
    }
    for (cur = lobj; cur != NULL; cur = cur->next) {
        HSD_LObjAddCurrent(cur);
    }
}

void HSD_LObjSetCurrentAll(HSD_LObj *lobj)
{
    LObjReplaceAll(lobj);
}

HSD_LObj *HSD_LObjGetCurrentByType(u16 flags)
{
    HSD_SList *cur = current_lights;
    u32 type = flags & LOBJ_TYPE_MASK;
    while (cur != NULL) {
        HSD_LObj *lobj = cur->data;
        if (type == (lobj->flags & LOBJ_TYPE_MASK)) {
            return lobj;
        }
        cur = cur->next;
    }
    return NULL;
}

u32 HSD_LightID2Index(GXLightID id)
{
    u32 index;
    switch (id) {
    case GX_LIGHT0:
        index = 0;
        break;
    case GX_LIGHT1:
        index = 1;
        break;
    case GX_LIGHT2:
        index = 2;
        break;
    case GX_LIGHT3:
        index = 3;
        break;
    case GX_LIGHT4:
        index = 4;
        break;
    case GX_LIGHT5:
        index = 5;
        break;
    case GX_LIGHT6:
        index = 6;
        break;
    case GX_LIGHT7:
        index = 7;
        break;
    case GX_MAX_LIGHT:
        index = 8;
        break;
    default:
        HSD_ASSERT(1159, 0);
        break;
    }
    return index;
}

s32 HSD_Index2LightID(u32 index)
{
    switch (index) {
    case 0:
        return GX_LIGHT0;
    case 1:
        return GX_LIGHT1;
    case 2:
        return GX_LIGHT2;
    case 3:
        return GX_LIGHT3;
    case 4:
        return GX_LIGHT4;
    case 5:
        return GX_LIGHT5;
    case 6:
        return GX_LIGHT6;
    case 7:
        return GX_LIGHT7;
    case 8:
        return GX_MAX_LIGHT;
    default:
        return GX_LIGHT_NULL;
    }
}

void HSD_LObjRemoveAll(HSD_LObj *lobj)
{
    HSD_LObj *next;
    HSD_LObj *cur;

    cur = lobj;
    while (cur != NULL) {
        next = cur->next;
        HSD_LObjDeleteCurrent(cur);
        HSD_LObjUnrefThis(cur);
        cur = next;
    }
}

void HSD_LObjSetPosition(HSD_LObj *lobj, Vec3 *position)
{
    HSD_ASSERT(1358, lobj);
    if (lobj->position == NULL) {
        lobj->position = HSD_WObjAlloc();
        HSD_ASSERT(1361, lobj->position);
    }
    HSD_WObjSetPosition(lobj->position, position);
}

s32 HSD_LObjGetPosition(HSD_LObj *lobj, Vec3 *position)
{
    /* Retained SDK strings preserve the original shared data-pool layout. */
    (void)"lobj->interest";
    (void)"hsdIsDescendantOf(info, &hsdLObj)";
    if (lobj != NULL && lobj->position != NULL) {
        HSD_WObjGetPosition(lobj->position, position);
        return true;
    }
    return false;
}

s32 HSD_LObjGetInterest(HSD_LObj *lobj, Vec3 *interest)
{
    if (lobj != NULL && lobj->interest != NULL) {
        HSD_WObjGetPosition(lobj->interest, interest);
        return true;
    }
    return false;
}

HSD_WObj *HSD_LObjGetPositionWObj(HSD_LObj *lobj)
{
    return lobj ? lobj->position : NULL;
}

HSD_WObj *HSD_LObjGetInterestWObj(HSD_LObj *lobj)
{
    return lobj ? lobj->interest : NULL;
}

static int LObjLoad(HSD_LObj *lobj, HSD_LightDesc *ldesc)
{
    HSD_LObjSetColor(lobj, ldesc->color);
    HSD_LObjSetFlags(lobj, ldesc->flags);
    switch (ldesc->flags & LOBJ_TYPE_MASK) {
    case LOBJ_AMBIENT:
        break;
    case LOBJ_INFINITE:
        HSD_LObjSetPositionWObj(lobj, HSD_WObjLoadDesc(ldesc->position));
        break;
    case LOBJ_POINT:
        HSD_LObjSetPositionWObj(lobj, HSD_WObjLoadDesc(ldesc->position));
        if (ldesc->attnflags & LOBJ_LIGHT_ATTN) {
            HSD_LObjSetFlags(lobj, LOBJ_RAW_PARAM);
            HSD_LObjSetAttnK(lobj, ldesc->u.attn->k0, ldesc->u.attn->k1,
                             ldesc->u.attn->k2);
        } else {
            HSD_LObjSetDistAttn(lobj, ldesc->u.point->ref_dist, ldesc->u.point->ref_br,
                                ldesc->u.point->dist_func);
        }
        break;
    case LOBJ_SPOT:
        HSD_LObjSetPositionWObj(lobj, HSD_WObjLoadDesc(ldesc->position));
        HSD_LObjSetInterestWObj(lobj, HSD_WObjLoadDesc(ldesc->interest));
        if (ldesc->attnflags != 0) {
            HSD_LObjSetFlags(lobj, LOBJ_RAW_PARAM);
            HSD_LObjSetAttn(lobj, ldesc->u.attn->a0, ldesc->u.attn->a1,
                            ldesc->u.attn->a2, ldesc->u.attn->k0, ldesc->u.attn->k1,
                            ldesc->u.attn->k2);
        } else {
            HSD_LObjSetDistAttn(lobj, ldesc->u.spot->ref_dist, ldesc->u.spot->ref_br,
                                ldesc->u.spot->dist_func);
            HSD_LObjSetSpot(lobj, ldesc->u.spot->cutoff, ldesc->u.spot->spot_func);
        }
        break;
    default:
        OSReport("unexpected lightdesc flags (%x)\n", ldesc->flags);
        HSD_Panic("lobj.c", 1599, "");
        break;
    }
    return 0;
}

HSD_LObj *HSD_LObjLoadDesc(HSD_LightDesc *ldesc)
{
    HSD_LObj *top, **p = &top;

    for (; ldesc; ldesc = ldesc->next) {
        HSD_ClassInfo *info;

        if (!ldesc->class_name || !(info = hsdSearchClassInfo(ldesc->class_name))) {
            *p = HSD_LObjAlloc();
        } else {
            *p = hsdNew(info);
            *p ? (void)0 : __assert("lobj.c", 1633, "*p");
        }
        HSD_LOBJ_METHOD(*p)->load(*p, ldesc);
        p = &(*p)->next;
    }
    *p = NULL;

    return top;
}

void HSD_LObjAddAnim(HSD_LObj *lobj, HSD_LightAnim *lanim)
{
    if (lobj == NULL) {
        return;
    }

    if (lanim != NULL) {
        if (lobj->aobj) {
            HSD_AObjRemove(lobj->aobj);
        }
        lobj->aobj = HSD_AObjLoadDesc(lanim->aobjdesc);
        HSD_WObjAddAnim(HSD_LObjGetPositionWObj(lobj), lanim->position_anim);
        HSD_WObjAddAnim(HSD_LObjGetInterestWObj(lobj), lanim->interest_anim);
    }
}

static void LObjRelease(HSD_Class *o)
{
    HSD_LObj *lobj = HSD_LOBJ(o);

    HSD_AObjRemove(lobj->aobj);
    HSD_WObjUnref(HSD_LObjGetPositionWObj(lobj));
    HSD_WObjUnref(HSD_LObjGetInterestWObj(lobj));

    HSD_OBJECT_PARENT_INFO(&hsdLObj)->release(o);
}

static void LObjAmnesia(HSD_ClassInfo *info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    if (info == HSD_CLASS_INFO(&hsdLObj)) {
        current_lights = NULL;
    }
    HSD_OBJECT_PARENT_INFO(&hsdLObj)->forget(info);
}

static void LObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdLObj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_lobj", sizeof(HSD_LObjInfo),
                     sizeof(HSD_LObj));
    HSD_CLASS_INFO(&hsdLObj)->release = LObjRelease;
    HSD_CLASS_INFO(&hsdLObj)->forget = LObjAmnesia;
    HSD_LOBJ_INFO(&hsdLObj)->load = LObjLoad;
    hsdLObj.update = LObjUpdateFunc;
}
