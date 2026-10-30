/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/jobj.c. */
#include "jobj.h"
#define NULL 0
#define true 1
#define false 0
typedef s32 bool;
typedef void (*DPCtlCallback)(int, int, int, HSD_JObj *);
#define HSD_ASSERT(line, condition)                                                    \
    do {                                                                               \
        if (!(condition))                                                              \
            __assert("jobj.c", line, #condition);                                      \
    } while (0)
#define HSD_JOBJ_INFO(o) ((HSD_JObjInfo *)(o))
#define HSD_JOBJ(o) ((HSD_JObj *)(o))
#define HSD_JOBJ_METHOD(o) ((HSD_JObjInfo *)((o)->class_info))
#define HSD_CLASS_INFO(o) ((HSD_ClassInfo *)(o))
#define HSD_OBJECT_PARENT_INFO(o) ((o)->parent.head.parent)
#define HSD_OBJECT_METHOD(o) (((HSD_Class *)(o))->class_info)
#define aobj animation
#define robj render_object
#define fobj functions
#define obj_type object_type
#define TYPE_JOBJ 12
#define HSD_JObjMtxIsDirty matrix_is_dirty
#define HSD_JObjSetupMatrix setup_matrix
#define HSD_JObjSetMtxDirty(jobj)                                                      \
    do {                                                                               \
        if ((jobj) != NULL && !matrix_is_dirty(jobj))                                  \
            HSD_JObjSetMtxDirtySub(jobj);                                              \
    } while (0)
#define next_p(p) ((p) ? (p)->next : NULL)
static void JObjInfoInit(void);
HSD_JObjInfo hsdJObj = {{{JObjInfoInit}}};
static HSD_ClassInfo *default_class;
static HSD_SList *ufc_callbacks;
static void (*dptcl_callback)(int, int, int, HSD_JObj *);
static void (*jsound_callback)(s32);
static void (*ptcltgt_callback)(HSD_JObj *, s32);
static HSD_JObj *current_jobj;
extern void HSD_RObjRemoveAnimAllByFlags(HSD_RObj *, u32);
extern void HSD_RObjReqAnimAllByFlags(HSD_RObj *, f32, u32);
extern void HSD_RObjAddAnimAll(HSD_RObj *, HSD_RObjAnimJoint *);
extern void HSD_RObjAnimAll(HSD_RObj *);
extern void HSD_AObjInitEndCallBack(void);
extern void HSD_AObjInvokeCallBacks(void);
#define JOBJ_BILLBOARD_FIELD 0xE00
#define JOBJ_BILLBOARD 0x200
#define JOBJ_VBILLBOARD 0x400
#define JOBJ_HBILLBOARD 0x600
#define JOBJ_RBILLBOARD 0x800
#define JOBJ_PBILLBOARD 0x2000

#define JOBJ_SKELETON (1 << 0)
#define JOBJ_SKELETON_ROOT (1 << 1)
#define JOBJ_ENVELOPE_MODEL (1 << 2)
#define JOBJ_CLASSICAL_SCALE (1 << 3)
#define JOBJ_HIDDEN (1 << 4)
#define JOBJ_PTCL (1 << 5)
#define JOBJ_MTX_DIRTY (1 << 6)
#define JOBJ_LIGHTING (1 << 7)
#define JOBJ_TEXGEN (1 << 8)
#define JOBJ_INSTANCE (1 << 12)
#define JOBJ_SPLINE (1 << 14)
#define JOBJ_FLIP_IK (1 << 15)
#define JOBJ_SPECULAR (1 << 16)
#define JOBJ_USE_QUATERNION (1 << 17)
#define JOBJ_OPA (1 << 18)
#define JOBJ_XLU (1 << 19)
#define JOBJ_TEXEDGE (1 << 20)
#define JOBJ_NULL_OBJ (0 << 21)
#define JOBJ_JOINT1 (1 << 21)
#define JOBJ_JOINT2 (2 << 21)
#define JOBJ_JOINT (3 << 21)
#define JOBJ_EFFECTOR (3 << 21)
#define JOBJ_USER_DEF_MTX (1 << 23)
#define JOBJ_MTX_INDEP_PARENT (1 << 24)
#define JOBJ_MTX_INDEP_SRT (1 << 25)
#define JOBJ_UNK_B26 (1 << 26)
#define JOBJ_UNK_B27 (1 << 27)
#define JOBJ_ROOT_OPA (1 << 28)
#define JOBJ_ROOT_XLU (1 << 29)
#define JOBJ_ROOT_TEXEDGE (1 << 30)

#define JOBJ_ROOT_MASK (JOBJ_ROOT_OPA | JOBJ_ROOT_TEXEDGE | JOBJ_ROOT_XLU)
#define JOBJ_TRSP_SHIFT 18
#define JOBJ_PTCL_BANK_MASK 0x3F
#define JOBJ_PTCL_OFFSET_SHIFT 6
#define JOBJ_PTCL_OFFSET_MASK 0xFFFFFF

#define union_type_ptcl(o) ((o)->flags & JOBJ_PTCL ? true : false)
#define union_type_spline(o) ((o)->flags & JOBJ_SPLINE ? true : false)
#define union_type_dobj(o) ((o)->flags & (JOBJ_PTCL | JOBJ_SPLINE) ? false : true)

#include "id.h"
#include "cobj.h"
#define HSD_IDKey u32
#define scl scale_pointer
#define mtx matrix
#define translate translation
#define MTXMultVec PSMTXMultVec
#define hsd_obj joint
#define envelopemtx envelope_matrix
#define robjdesc render_desc
#define id object_id
#define MTXConcat PSMTXConcat
#define HSD_JObjGetViewingMtxPtrDirect(cobj) ((cobj)->view_mtx)
#define HSD_CObjGetViewingMtxPtrDirect(cobj) ((cobj)->view_mtx)
#define HSD_ASSERTREPORT(line, condition, message)                                     \
    do {                                                                               \
        if (!(condition)) {                                                            \
            OSReport(message);                                                         \
            __assert("jobj.c", line, #condition);                                      \
        }                                                                              \
    } while (0)
#define JOINT_HEADER_ASSERT(line, condition)                                           \
    do {                                                                               \
        if (!(condition))                                                              \
            __assert("jobj.h", line, #condition);                                      \
    } while (0)
typedef void (*ufc_callback)(HSD_JObj *, s32, f32);
extern void OSReport(const char *, ...);
extern void HSD_Panic(const char *, u32, const char *);
extern void *memcpy(void *, const void *, u32);
extern HSD_JObj *HSD_JObjLoadJoint(HSD_Joint *);
extern void HSD_JObjResolveRefsAll(HSD_JObj *, HSD_Joint *);
extern HSD_RObj *HSD_RObjLoadDesc(HSD_RObjDesc *);
extern void HSD_RObjResolveRefsAll(HSD_RObj *, HSD_RObjDesc *);
extern void HSD_RObjRemoveAll(HSD_RObj *);
extern HSD_RObj *HSD_RObjGetByType(HSD_RObj *, u32, s32);
extern void HSD_RObjUpdateAll(HSD_RObj *, void *, AnimationUpdate);
extern HSD_JObj *HSD_JObjReparent(HSD_JObj *, HSD_JObj *);
extern void HSD_JObjMakePositionMtx(HSD_JObj *, Mtx, Mtx);
extern void HSD_JObjDispSub(HSD_JObj *, Mtx, Mtx, u32, u32);
extern void HSD_IDInsertToTable(HSD_IDTable *, u32, void *);
extern void HSD_IDRemoveByIDFromTable(HSD_IDTable *, u32);
extern void splArcLengthPoint(Vec3 *, void *, f32);
extern void resolveIKJoint1(HSD_JObj *);
extern void resolveIKJoint2(HSD_JObj *);
extern f32 PSVECDotProduct(Vec3 *, Vec3 *);
extern void PSVECScale(Vec3 *, Vec3 *, f32);
extern void PSVECAdd(Vec3 *, Vec3 *, Vec3 *);
#define VECScale PSVECScale
#define VECDotProduct PSVECDotProduct
#define VECAdd PSVECAdd
static inline s32 ref_DEC(void *o)
{
    s32 result;
    if ((result = (((HSD_Obj *)o)->reference_count == (u16)-1)))
        return result;
    return ((HSD_Obj *)o)->reference_count-- == 0;
}
static inline s32 ref_CNT(void *o)
{
    return ((HSD_Obj *)o)->reference_count == (u16)-1 ? -1
                                                      : ((HSD_Obj *)o)->reference_count;
}
static inline s32 iref_CNT(void *o)
{
    return ((HSD_Obj *)o)->individual_reference_count;
}
static inline s32 iref_DEC(void *o)
{
    s32 result;
    if ((result = (((HSD_Obj *)o)->individual_reference_count == 0)))
        return result;
    ((HSD_Obj *)o)->individual_reference_count -= 1;
    return ((HSD_Obj *)o)->individual_reference_count == 0;
}
static inline void ref_INC(void *o)
{
    if (o) {
        ++((HSD_Obj *)o)->reference_count;
        if (((HSD_Obj *)o)->reference_count == (u16)-1)
            __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF");
    }
}
static inline void iref_INC(void *o)
{
    ++((HSD_Obj *)o)->individual_reference_count;
    if (!((HSD_Obj *)o)->individual_reference_count)
        __assert("object.h", 158, "HSD_OBJ(o)->ref_count_individual != 0");
}
#define HSD_JObjRef ref_INC
static inline void hsdDelete(void *o)
{
    if (o) {
        HSD_OBJECT_METHOD(o)->release((HSD_Class *)o);
        HSD_OBJECT_METHOD(o)->destroy((HSD_Class *)o);
    }
}
#define HSD_A_J_ROTX 1
#define HSD_A_J_ROTY 2
#define HSD_A_J_ROTZ 3
#define HSD_A_J_PATH 4
#define HSD_A_J_TRAX 5
#define HSD_A_J_TRAY 6
#define HSD_A_J_TRAZ 7
#define HSD_A_J_SCAX 8
#define HSD_A_J_SCAY 9
#define HSD_A_J_SCAZ 10
#define HSD_A_J_NODE 11
#define HSD_A_J_BRANCH 12
#define HSD_A_J_SETBYTE0 20
#define HSD_A_J_SETFLOAT0 30
#define HSD_A_J_SETBYTE1 21
#define HSD_A_J_SETFLOAT1 31
#define HSD_A_J_SETBYTE2 22
#define HSD_A_J_SETFLOAT2 32
#define HSD_A_J_SETBYTE3 23
#define HSD_A_J_SETFLOAT3 33
#define HSD_A_J_SETBYTE4 24
#define HSD_A_J_SETFLOAT4 34
#define HSD_A_J_SETBYTE5 25
#define HSD_A_J_SETFLOAT5 35
#define HSD_A_J_SETBYTE6 26
#define HSD_A_J_SETFLOAT6 36
#define HSD_A_J_SETBYTE7 27
#define HSD_A_J_SETFLOAT7 37
#define HSD_A_J_SETBYTE8 28
#define HSD_A_J_SETFLOAT8 38
#define HSD_A_J_SETBYTE9 29
#define HSD_A_J_SETFLOAT9 39
/* Macro expansion preserves the original recursive inlining depth. */
#define update_joint_flags(object, bits, operation)                                    \
    do {                                                                               \
        if ((object) != NULL) {                                                        \
            if (((object)->flags ^ (bits)) & JOBJ_CLASSICAL_SCALE) {                   \
                if ((object) != NULL && !matrix_is_dirty(object))                      \
                    HSD_JObjSetMtxDirtySub(object);                                    \
            }                                                                          \
            (object)->flags operation(bits);                                           \
        }                                                                              \
    } while (0)

static inline bool has_scl(HSD_JObj *jobj)
{
    bool result = false;
    if (jobj != NULL && jobj->scl != NULL) {
        result = true;
    }
    return result;
}
void HSD_JObjCheckDepend(HSD_JObj *jobj)
{
    (void)"jobj->parent";
    if (jobj == NULL) {
        return;
    }

    switch (HSD_JObjMtxIsDirty(jobj)) {
    case false:
        if ((jobj->flags & JOBJ_USER_DEF_MTX)) {
            if (!(jobj->flags & JOBJ_MTX_INDEP_PARENT) && jobj->parent != NULL &&
                HSD_JObjMtxIsDirty(jobj->parent)) {
                jobj->flags |= JOBJ_MTX_DIRTY;
            }
        } else if ((jobj->parent != NULL && (jobj->parent->flags & JOBJ_MTX_DIRTY)) ||
                   (jobj->flags & JOBJ_EFFECTOR) == JOBJ_JOINT1 ||
                   (jobj->flags & JOBJ_EFFECTOR) == JOBJ_JOINT2 ||
                   (jobj->flags & JOBJ_EFFECTOR) == JOBJ_EFFECTOR ||
                   jobj->robj != NULL) {
            jobj->flags |= JOBJ_MTX_DIRTY;
        }
        break;
    }
}

void HSD_JObjMakeMatrix(HSD_JObj *jobj)
{
    Vec3 *scl;

    HSD_JObjSetupMatrix(jobj->parent);
    if (jobj->flags & JOBJ_CLASSICAL_SCALE) {
        if (jobj->parent != NULL && jobj->parent->scl != NULL) {
            if (jobj->scl == NULL) {
                jobj->scl = HSD_VecAlloc();
            }
            *jobj->scl = *jobj->parent->scl;
        } else {
            if (jobj->scl != NULL) {
                HSD_VecFree(jobj->scl);
                jobj->scl = NULL;
            }
        }
    } else {
        if (jobj->scl == NULL) {
            jobj->scl = HSD_VecAlloc();
        }
        if (jobj->parent != NULL && jobj->parent->scl != NULL) {
            jobj->scl->x = jobj->scale.x * jobj->parent->scl->x;
            jobj->scl->y = jobj->scale.y * jobj->parent->scl->y;
            jobj->scl->z = jobj->scale.z * jobj->parent->scl->z;
        } else {
            *jobj->scl = jobj->scale;
        }
    }
    if (jobj->flags & JOBJ_USE_QUATERNION) {
        if (has_scl(jobj->parent)) {
            scl = jobj->parent->scl;
        } else {
            scl = NULL;
        }
        HSD_MtxSRTQuat(jobj->mtx, &jobj->scale, (Quaternion *)jobj->rotation,
                       &jobj->translate, scl);
    } else {
        if (has_scl(jobj->parent)) {
            scl = jobj->parent->scl;
        } else {
            scl = NULL;
        }
        HSD_MtxSRT(jobj->mtx, &jobj->scale, (Vec3 *)jobj->rotation, &jobj->translate,
                   scl);
    }
    if (jobj->parent != NULL) {
        PSMTXConcat(jobj->parent->mtx, jobj->mtx, jobj->mtx);
    }
    if (jobj->aobj != NULL && jobj->aobj->hsd_obj != NULL) {
        Vec3 vec;
        HSD_JObj *aobj_jobj = (HSD_JObj *)jobj->aobj->hsd_obj;
        HSD_JObjSetupMatrix((HSD_JObj *)jobj->aobj->hsd_obj);
        MTXMultVec(aobj_jobj->mtx, &jobj->translate, &vec);
        jobj->mtx[0][3] = vec.x;
        jobj->mtx[1][3] = vec.y;
        jobj->mtx[2][3] = vec.z;
    }
}

void HSD_JObjRemoveAnimByFlags(HSD_JObj *jobj, u32 flags)
{
    if (jobj != NULL) {
        if (flags & 1) {
            HSD_AObjRemove(jobj->aobj);
            jobj->aobj = NULL;
        }
        if (union_type_dobj(jobj)) {
            HSD_DObjRemoveAnimAllByFlags(jobj->display_object, flags);
        }
        HSD_RObjRemoveAnimAllByFlags(jobj->robj, flags);
    }
}

void HSD_JObjRemoveAnimAllByFlags(HSD_JObj *jobj, u32 flags)
{
    if (jobj != NULL) {
        HSD_JObjRemoveAnimByFlags(jobj, flags);
        if (!(jobj->flags & JOBJ_INSTANCE)) {
            HSD_JObj *child = jobj->child;
            while (child != NULL) {
                HSD_JObjRemoveAnimAllByFlags(child, flags);
                child = child->next;
            }
        }
    }
}

void HSD_JObjRemoveAnim(HSD_JObj *jobj)
{
    HSD_JObjRemoveAnimByFlags(jobj, ALL_ANIM);
}

void HSD_JObjRemoveAnimAll(HSD_JObj *jobj)
{
    HSD_JObjRemoveAnimAllByFlags(jobj, ALL_ANIM);
}

void HSD_JObjReqAnimByFlags(HSD_JObj *jobj, u32 flags, f32 frame)
{
    bool has_dobj;
    if (jobj != NULL) {
        if (flags & 1) {
            HSD_AObjReqAnim(jobj->aobj, frame);
        }
        if (jobj->flags & (JOBJ_PTCL | JOBJ_SPLINE)) {
            has_dobj = false;
        } else {
            has_dobj = true;
        }
        if (has_dobj) {
            HSD_DObjReqAnimAllByFlags(jobj->display_object, frame, flags);
        }
        HSD_RObjReqAnimAllByFlags(jobj->robj, frame, flags);
    }
}

void HSD_JObjReqAnimAllByFlags(HSD_JObj *jobj, u32 flags, f32 frame)
{
    if (jobj != NULL) {
        HSD_JObjReqAnimByFlags(jobj, flags, frame);
        if (!(jobj->flags & JOBJ_INSTANCE)) {
            HSD_JObj *child = jobj->child;
            while (child != NULL) {
                HSD_JObjReqAnimAllByFlags(child, flags, frame);
                child = child->next;
            }
        }
    }
}

void HSD_JObjReqAnimAll(HSD_JObj *jobj, f32 frame)
{
    HSD_JObjReqAnimAllByFlags(jobj, ALL_ANIM, frame);
}

void HSD_JObjReqAnim(HSD_JObj *jobj, f32 frame)
{
    HSD_JObjReqAnimByFlags(jobj, ALL_ANIM, frame);
}

#pragma push
#pragma force_active on
static void JObjSortAnim(HSD_AObj *aobj)
{
    HSD_FObj *fobj;
    HSD_FObj **fobj_ptr;

    if (aobj == NULL || aobj->fobj == NULL) {
        return;
    }
    for (fobj_ptr = &aobj->fobj; *fobj_ptr != NULL; fobj_ptr = &fobj->next) {
        fobj = *fobj_ptr;
        if (fobj->obj_type == TYPE_JOBJ) {
            *fobj_ptr = fobj->next;
            fobj->next = aobj->fobj;
            aobj->fobj = fobj;
            break;
        }
    }
}
#pragma pop

void HSD_JObjAddAnim(HSD_JObj *jobj, HSD_AnimJoint *an_joint,
                     HSD_MatAnimJoint *mat_joint, HSD_ShapeAnimJoint *sh_joint)
{
    if (jobj != NULL) {
        if (an_joint != NULL) {
            if (jobj->aobj != NULL) {
                HSD_AObjRemove(jobj->aobj);
            }
            jobj->aobj = HSD_AObjLoadDesc(an_joint->aobjdesc);
            JObjSortAnim(jobj->aobj);
            HSD_RObjAddAnimAll(jobj->robj, an_joint->robj_anim);
            if (an_joint->flags & 1) {
                HSD_JObjSetFlags(jobj, JOBJ_CLASSICAL_SCALE);
            } else {
                HSD_JObjClearFlags(jobj, JOBJ_CLASSICAL_SCALE);
            }
        }
        if (union_type_dobj(jobj)) {
            HSD_DObjAddAnimAll(jobj->display_object,
                               mat_joint != NULL ? mat_joint->matanim : NULL,
                               sh_joint != NULL ? sh_joint->shapeanimdobj : NULL);
        }
    }
}

void HSD_JObjAddAnimAll(HSD_JObj *jobj, HSD_AnimJoint *ajoint, HSD_MatAnimJoint *mjoint,
                        HSD_ShapeAnimJoint *sjoint)
{
    HSD_JObj *jp;
    HSD_AnimJoint *aj;
    HSD_MatAnimJoint *mj;
    HSD_ShapeAnimJoint *sj;

    if (jobj != NULL) {
        HSD_JObjAddAnim(jobj, ajoint, mjoint, sjoint);
        if (!(jobj->flags & JOBJ_INSTANCE)) {
            jp = jobj->child;
            aj = ajoint != NULL ? ajoint->child : NULL;
            mj = mjoint != NULL ? mjoint->child : NULL;
            sj = sjoint != NULL ? sjoint->child : NULL;
            while (jp != NULL) {
                HSD_JObjAddAnimAll(jp, aj, mj, sj);
                jp = jp->next;
                aj = aj != NULL ? aj->next : NULL;
                mj = mj != NULL ? mj->next : NULL;
                sj = sj != NULL ? sj->next : NULL;
            }
        }
    }
}

static inline void HSD_JObjSetRotationX(HSD_JObj *jobj, f32 x)
{
    JOINT_HEADER_ASSERT(676, jobj);
    JOINT_HEADER_ASSERT(677, !(jobj->flags & JOBJ_USE_QUATERNION));
    jobj->rotation[0] = x;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}
static inline void HSD_JObjSetRotationY(HSD_JObj *jobj, f32 y)
{
    JOINT_HEADER_ASSERT(696, jobj);
    JOINT_HEADER_ASSERT(697, !(jobj->flags & JOBJ_USE_QUATERNION));
    jobj->rotation[1] = y;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}
static inline void HSD_JObjSetRotationZ(HSD_JObj *jobj, f32 z)
{
    JOINT_HEADER_ASSERT(716, jobj);
    JOINT_HEADER_ASSERT(717, !(jobj->flags & JOBJ_USE_QUATERNION));
    jobj->rotation[2] = z;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}
static inline void HSD_JObjSetScaleX(HSD_JObj *jobj, f32 x)
{
    JOINT_HEADER_ASSERT(805, jobj);
    jobj->scale.x = x;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}
static inline void HSD_JObjSetScaleY(HSD_JObj *jobj, f32 y)
{
    JOINT_HEADER_ASSERT(819, jobj);
    jobj->scale.y = y;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}
static inline void HSD_JObjSetScaleZ(HSD_JObj *jobj, f32 z)
{
    JOINT_HEADER_ASSERT(833, jobj);
    jobj->scale.z = z;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}
static inline void HSD_JObjSetTranslateX(HSD_JObj *jobj, f32 x)
{
    JOINT_HEADER_ASSERT(952, jobj);
    jobj->translate.x = x;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}
static inline void HSD_JObjSetTranslateY(HSD_JObj *jobj, f32 y)
{
    JOINT_HEADER_ASSERT(966, jobj);
    jobj->translate.y = y;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}
static inline void HSD_JObjSetTranslateZ(HSD_JObj *jobj, f32 z)
{
    JOINT_HEADER_ASSERT(980, jobj);
    jobj->translate.z = z;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}
#pragma push
#pragma force_active on
static void JObjUpdateFunc(void *obj, s32 type, AnimationValue *val)
{
    HSD_JObj *jobj = obj;
    Vec3 p;
    HSD_JObj *jp;
    HSD_RObj *robj;
    Mtx mtx;

    if (jobj != NULL) {
        switch (type) {
        case HSD_A_J_PATH:
            if (val->scalar < 0.0L) {
                val->scalar = 0.0F;
            }
            if (1.0L < val->scalar) {
                val->scalar = 1.0F;
            }
            HSD_ASSERT(590, jobj->aobj);
            jp = (HSD_JObj *)jobj->aobj->hsd_obj;
            HSD_ASSERT(592, jp);
            if (!jp->display_object)
                __assert("jobj.c", 593, "jp->u.spline");
            splArcLengthPoint(&p, jp->display_object, val->scalar);
            HSD_JObjSetTranslateX(jobj, p.x);
            HSD_JObjSetTranslateY(jobj, p.y);
            HSD_JObjSetTranslateZ(jobj, p.z);
            break;
        case HSD_A_J_ROTX:
            if (jobj->flags & JOBJ_JOINT1) {
                robj = HSD_RObjGetByType(jobj->robj, REFTYPE_IKHINT, 0);
                if (robj != NULL) {
                    robj->u.ik_hint.rotate_x = val->scalar;
                }
            }
            HSD_JObjSetRotationX(jobj, val->scalar);
            break;
        case HSD_A_J_ROTY:
            HSD_JObjSetRotationY(jobj, val->scalar);
            break;
        case HSD_A_J_ROTZ:
            HSD_JObjSetRotationZ(jobj, val->scalar);
            break;
        case HSD_A_J_TRAX:
            HSD_JObjSetTranslateX(jobj, val->scalar);
            break;
        case HSD_A_J_TRAY:
            HSD_JObjSetTranslateY(jobj, val->scalar);
            break;
        case HSD_A_J_TRAZ:
            HSD_JObjSetTranslateZ(jobj, val->scalar);
            break;
        case HSD_A_J_SCAX:
            HSD_JObjSetScaleX(jobj, val->scalar);
            break;
        case HSD_A_J_SCAY:
            HSD_JObjSetScaleY(jobj, val->scalar);
            break;
        case HSD_A_J_SCAZ:
            HSD_JObjSetScaleZ(jobj, val->scalar);
            break;
        case HSD_A_J_BRANCH:
            if (val->scalar > 0.5) {
                HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
            } else {
                HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
            }
            break;
        case HSD_A_J_NODE:
            if (val->scalar > 0.5) {
                HSD_JObjClearFlags(jobj, JOBJ_HIDDEN);
            } else {
                HSD_JObjSetFlags(jobj, JOBJ_HIDDEN);
            }
            break;
        case HSD_A_J_SETBYTE0:
        case HSD_A_J_SETBYTE1:
        case HSD_A_J_SETBYTE2:
        case HSD_A_J_SETBYTE3:
        case HSD_A_J_SETBYTE4:
        case HSD_A_J_SETBYTE5:
        case HSD_A_J_SETBYTE6:
        case HSD_A_J_SETBYTE7:
        case HSD_A_J_SETBYTE8:
        case HSD_A_J_SETBYTE9: {
            HSD_SList *callback_node = ufc_callbacks;
            while (callback_node != NULL) {
                ((ufc_callback)callback_node->data)(jobj, type, (f32)val->integer);
                callback_node = callback_node->next;
            }
            break;
        }
        case HSD_A_J_SETFLOAT0:
        case HSD_A_J_SETFLOAT1:
        case HSD_A_J_SETFLOAT2:
        case HSD_A_J_SETFLOAT3:
        case HSD_A_J_SETFLOAT4:
        case HSD_A_J_SETFLOAT5:
        case HSD_A_J_SETFLOAT6:
        case HSD_A_J_SETFLOAT7:
        case HSD_A_J_SETFLOAT8:
        case HSD_A_J_SETFLOAT9: {
            HSD_SList *callback_node = ufc_callbacks;
            while (callback_node != NULL) {
                ((ufc_callback)callback_node->data)(jobj, type, val->scalar);
                callback_node = callback_node->next;
            }
            break;
        }
        case JOBJ_UPDATE_PTCL: {
            s32 iv = val->integer;
            s32 bank = iv & JOBJ_PTCL_BANK_MASK;
            s32 offset = iv >> JOBJ_PTCL_OFFSET_SHIFT & JOBJ_PTCL_OFFSET_MASK;
            if (dptcl_callback != NULL) {
                dptcl_callback(0, bank, offset, jobj);
            }
        } break;
        case JOBJ_UPDATE_SOUND:
            if (jsound_callback != NULL) {
                jsound_callback(val->integer);
            }
            break;
        case JOBJ_UPDATE_PTCL_TARGET:
            if (ptcltgt_callback != NULL) {
                ptcltgt_callback(jobj, val->integer);
            }
            break;
        case JOBJ_UPDATE_MTX_COL0:
            jobj->mtx[0][0] = val->vector.x;
            jobj->mtx[1][0] = val->vector.y;
            jobj->mtx[2][0] = val->vector.z;
            break;
        case JOBJ_UPDATE_MTX_COL1:
            jobj->mtx[0][1] = val->vector.x;
            jobj->mtx[1][1] = val->vector.y;
            jobj->mtx[2][1] = val->vector.z;
            break;
        case JOBJ_UPDATE_MTX_COL2:
            jobj->mtx[0][2] = val->vector.x;
            jobj->mtx[1][2] = val->vector.y;
            jobj->mtx[2][2] = val->vector.z;
            break;
        case JOBJ_UPDATE_MTX_COL3:
            jobj->mtx[0][3] = val->vector.x;
            jobj->mtx[1][3] = val->vector.y;
            jobj->mtx[2][3] = val->vector.z;
            break;
        case JOBJ_UPDATE_MTX_GET_SRT:
        case JOBJ_UPDATE_MTX_GET_ROTATION:
        case JOBJ_UPDATE_MTX_GET_TRANSLATE:
        case JOBJ_UPDATE_MTX_GET_SCALE:
            if (jobj->parent != NULL) {
                HSD_MtxInverseConcat(jobj->parent->mtx, jobj->mtx, mtx);
            } else {
                PSMTXCopy(jobj->mtx, mtx);
            }
            /* The u32 casts keep the original unsigned compares. */
            if ((u32)type == JOBJ_UPDATE_MTX_GET_SRT ||
                (u32)type == JOBJ_UPDATE_MTX_GET_TRANSLATE) {
                HSD_MtxGetTranslate(mtx, &jobj->translate);
            }
            if (type == JOBJ_UPDATE_MTX_GET_SRT || type == JOBJ_UPDATE_MTX_GET_ROTATION) {
                HSD_MtxGetRotation(mtx, (Vec3 *)jobj->rotation);
            }
            if ((u32)type == JOBJ_UPDATE_MTX_GET_SRT ||
                (u32)type == JOBJ_UPDATE_MTX_GET_SCALE) {
                HSD_MtxGetScale(mtx, &jobj->scale);
            }
            break;
        }
    }
}
#pragma pop

void HSD_JObjAnim(HSD_JObj *jobj)
{
    if (jobj != NULL) {
        HSD_JObjCheckDepend(jobj);
        HSD_AObjInterpretAnim(jobj->aobj, jobj, HSD_JOBJ_METHOD(jobj)->update);
        HSD_RObjAnimAll(jobj->robj);
        if (union_type_dobj(jobj)) {
            HSD_DObjAnimAll(jobj->display_object);
        }
    }
}

#pragma push
#pragma force_active on
static void JObjAnimAll(HSD_JObj *jobj)
{
    HSD_JObj *child;
    if (jobj != NULL) {
        HSD_JObjAnim(jobj);
        if (!(jobj->flags & JOBJ_INSTANCE)) {
            child = jobj->child;
            while (child != NULL) {
                JObjAnimAll(child);
                child = child->next;
            }
        }
    }
}
#pragma pop

void HSD_JObjAnimAll(HSD_JObj *jobj)
{
    if (jobj != NULL) {
        HSD_AObjInitEndCallBack();
        JObjAnimAll(jobj);
        HSD_AObjInvokeCallBacks();
    }
}

static inline MtxPtr camera_viewing_matrix(HSD_CObj *cobj)
{
    return cobj->view_mtx;
}
void HSD_JObjDispAll(HSD_JObj *jobj, Mtx vmtx, u32 flags, u32 rendermode)
{
    if (jobj != NULL) {
        if (jobj->flags & JOBJ_INSTANCE) {
            if (!(jobj->flags & JOBJ_HIDDEN)) {
                Mtx matrix;
                s32 has_view;
                s32 has_camera;
                HSD_CObj *cobj;
                HSD_JObjSetupMatrix(jobj);
                HSD_JObjSetupMatrix(jobj->child);
                PSMTXInverse(jobj->child->matrix, matrix);
                PSMTXConcat(jobj->matrix, matrix, matrix);
                has_view = vmtx != NULL;
                if (has_view)
                    PSMTXConcat(vmtx, matrix, matrix);
                else {
                    cobj = HSD_CObjGetCurrent();
                    has_camera = cobj != NULL;
                    if (has_camera)
                        PSMTXConcat(camera_viewing_matrix(cobj), matrix, matrix);
                }
                HSD_JObjDispAll(jobj->child, matrix, flags, rendermode);
            }
        } else {
            if (jobj->flags & (flags << JOBJ_TRSP_SHIFT))
                HSD_JObjDisp(jobj, vmtx, flags, rendermode);
            if (jobj->flags & (flags << 28)) {
                jobj = jobj->child;
                while (jobj != NULL) {
                    HSD_JObjDispAll(jobj, vmtx, flags, rendermode);
                    jobj = jobj->next;
                }
            }
        }
    }
    (void)"hsdIsDescendantOf(info, &hsdJObj)";
}

static inline HSD_JObj *JObjLoadJointSub(HSD_Joint *joint, HSD_JObj *parent)
{
    HSD_JObj *jobj;
    HSD_ClassInfo *info;
    if (joint == NULL) {
        return NULL;
    }
    if (joint->name == NULL || !(info = hsdSearchClassInfo(joint->name))) {
        jobj = HSD_JObjAlloc();
    } else {
        jobj = hsdNew(info);
        HSD_ASSERT(981, jobj);
    }
    HSD_JOBJ_METHOD(jobj)->load(jobj, joint, parent);
    return jobj;
}

#pragma push
#pragma force_active on
static s32 JObjLoad(HSD_JObj *jobj, HSD_Joint *joint, HSD_JObj *parent)
{
    if (!(joint->flags & JOBJ_INSTANCE)) {
        jobj->child = JObjLoadJointSub(joint->child, jobj);
    }
    jobj->next = JObjLoadJointSub(joint->next, parent);
    jobj->parent = parent;
    jobj->flags |= joint->flags;
    if (union_type_spline(jobj)) {
        jobj->display_object = joint->u.spline;
    } else if (union_type_ptcl(jobj)) {
        HSD_SList *slist;
        jobj->display_object = joint->u.ptcl;
        slist = joint->u.ptcl;
        while (slist != NULL) {
            *(u32 *)&slist->data |= 0x80000000;
            slist = slist->next;
        }
    } else {
        jobj->display_object = HSD_DObjLoadDesc(joint->u.dobjdesc);
    }
    jobj->robj = HSD_RObjLoadDesc(joint->robjdesc);
    jobj->rotation[0] = joint->rotation.x;
    jobj->rotation[1] = joint->rotation.y;
    jobj->rotation[2] = joint->rotation.z;
    jobj->scale = joint->scale;
    jobj->translate = joint->position;
    PSMTXIdentity(jobj->mtx);
    jobj->scl = NULL;
    if (joint->mtx != NULL) {
        jobj->envelopemtx = HSD_MtxAlloc();
        memcpy(jobj->envelopemtx, joint->mtx, sizeof(Mtx));
    }
    HSD_IDInsertToTable(NULL, (HSD_IDKey)joint, jobj);
    jobj->id = (HSD_IDKey)joint;
    return 0;
}
#pragma pop

HSD_JObj *HSD_JObjLoadJoint(HSD_Joint *joint)
{
    HSD_JObj *jobj = JObjLoadJointSub(joint, 0);
    HSD_JObjResolveRefsAll(jobj, joint);
    (void)"jobj_root";
    (void)"jobj_root == NULL";
    return jobj;
}

void HSD_JObjResolveRefs(HSD_JObj *jobj, HSD_Joint *joint)
{
    u32 instance;

    if (jobj == NULL || joint == NULL) {
        return;
    }

    HSD_RObjResolveRefsAll(jobj->robj, joint->robjdesc);
    instance = jobj->flags & JOBJ_INSTANCE;
    if (instance) {
        HSD_JObjUnref(jobj->child);
        jobj->child = HSD_IDGetDataFromTable(NULL, (HSD_IDKey)joint->child, NULL);
        HSD_ASSERT(1106, jobj->child);
        HSD_JObjRef(jobj->child);
    }
    if (union_type_dobj(jobj)) {
        HSD_DObjResolveRefsAll(jobj->display_object, joint->u.dobjdesc);
    }
}

void HSD_JObjResolveRefsAll(HSD_JObj *jobj, HSD_Joint *joint)
{
    while (jobj != NULL && joint != NULL) {
        HSD_JObjResolveRefs(jobj, joint);
        if (!(jobj->flags & JOBJ_INSTANCE)) {
            HSD_JObjResolveRefsAll(jobj->child, joint->child);
        }
        jobj = jobj->next;
        joint = joint->next;
    }
}

void HSD_JObjUnref(HSD_JObj *jobj)
{
    if (jobj != NULL && ref_DEC(jobj)) {
        if (iref_CNT(jobj) - 1 < 0) {
            hsdDelete(jobj);
        } else {
            iref_INC(jobj);
            HSD_JOBJ_METHOD(jobj)->release_child(jobj);
            if (iref_DEC(jobj)) {
                hsdDelete(jobj);
            }
        }
    }
}

void HSD_JObjUnrefThis(HSD_JObj *jobj)
{
    if (jobj != NULL && iref_DEC(jobj) && ref_CNT(jobj) < 0) {
        hsdDelete(jobj);
    }
}

#pragma push
#pragma pool_data off
void HSD_JObjRemoveAll(HSD_JObj *jobj)
{
    HSD_JObj *prev;
    HSD_JObj *next;

    if (jobj == NULL) {
        return;
    }
    if (jobj->parent != NULL) {
        prev = HSD_JObjGetPrev(jobj);
        if (prev != NULL) {
            prev->next = NULL;
        } else {
            jobj->parent->child = NULL;
        }
    }
    while (jobj != NULL) {
        next = jobj->next;
        jobj->parent = NULL;
        jobj->next = NULL;
        HSD_JObjUnref(jobj);
        jobj = next;
    }
    (void)"child->next == NULL";
}
#pragma pop

void RecalcParentTrspBits(HSD_JObj *jobj)
{
    while (jobj != NULL) {
        HSD_JObj *child = jobj->child;
        u32 flags = ~JOBJ_ROOT_MASK;
        while (child != NULL) {
            flags |= (child->flags | child->flags << 10) & JOBJ_ROOT_MASK;
            child = child->next;
        }
        if (!(jobj->flags & ~flags)) {
            break;
        }
        jobj->flags &= flags;
        jobj = jobj->next;
    }
}

static inline void UpdateParentTrspBits(HSD_JObj *jobj, HSD_JObj *child)
{
    u32 flags = (child->flags | (child->flags << 10)) & JOBJ_ROOT_MASK;
    while (jobj != NULL) {
        if (!(flags & ~jobj->flags)) {
            break;
        }
        jobj->flags |= flags;
        jobj = jobj->parent;
    }
}

void HSD_JObjAddChild(HSD_JObj *jobj, HSD_JObj *child)
{
    HSD_JObj *last;

    if (jobj == NULL || child == NULL) {
        return;
    }
    HSD_ASSERTREPORT(1340, child->parent == NULL, "child should be a orphan.\n");
    HSD_ASSERTREPORT(1341, child->next == NULL, "child should not have siblings");
    if (jobj->child == NULL) {
        jobj->child = child;
    } else {
        HSD_ASSERT(1347, !(jobj->flags & JOBJ_INSTANCE));
        last = jobj->child;
        while (last->next != NULL) {
            HSD_ASSERT(1350, last != child);
            last = last->next;
        }
        last->next = child;
    }
    child->parent = jobj;
    UpdateParentTrspBits(jobj, child);
}

HSD_JObj *HSD_JObjGetPrev(HSD_JObj *jobj)
{
    HSD_JObj *cur;

    if (jobj == NULL || jobj->parent == NULL) {
        return NULL;
    }
    if (jobj == jobj->parent->child) {
        return NULL;
    }
    cur = jobj->parent->child;
    while (cur != NULL) {
        if (cur->next == jobj) {
            return cur;
        }
        cur = cur->next;
    }
    HSD_Panic("jobj.c", 1506,
              "can not find specified jobj. maybe jobj tree is broken.\n");
    return NULL;
}

HSD_DObj *HSD_JObjGetDObj(HSD_JObj *jobj)
{
    if (jobj == NULL || !union_type_dobj(jobj)) {
        return NULL;
    }
    return jobj->display_object;
}

void HSD_JObjSetFlags(HSD_JObj *jobj, u32 flags)
{
    if (jobj != NULL) {
        if ((jobj->flags ^ flags) & JOBJ_CLASSICAL_SCALE) {
            if (jobj != NULL && !HSD_JObjMtxIsDirty(jobj)) {
                HSD_JObjSetMtxDirtySub(jobj);
            }
        }
        jobj->flags |= flags;
    }
}

#pragma push
#pragma inline_max_auto_size(1000)
void HSD_JObjSetFlagsAll(HSD_JObj *jobj, u32 flags)
{
    if (jobj != NULL) {
        update_joint_flags(jobj, flags, |=);
        if (!(jobj->flags & JOBJ_INSTANCE)) {
            HSD_JObj *i;
            for (i = jobj->child; i != NULL; i = i->next) {
                HSD_JObjSetFlagsAll(i, flags);
            }
        }
    }
}
#pragma pop

void HSD_JObjClearFlags(HSD_JObj *jobj, u32 flags)
{
    if (jobj != NULL) {
        if ((jobj->flags ^ flags) & JOBJ_CLASSICAL_SCALE) {
            if (jobj != NULL && !HSD_JObjMtxIsDirty(jobj)) {
                HSD_JObjSetMtxDirtySub(jobj);
            }
        }
        jobj->flags &= ~flags;
    }
}

#pragma push
#pragma inline_max_auto_size(1000)
void HSD_JObjClearFlagsAll(HSD_JObj *jobj, u32 flags)
{
    if (jobj != NULL) {
        update_joint_flags(jobj, flags, &= ~);
        if (!(jobj->flags & JOBJ_INSTANCE)) {
            HSD_JObj *i;
            for (i = jobj->child; i != NULL; i = i->next) {
                HSD_JObjClearFlagsAll(i, flags);
            }
        }
    }
}
#pragma pop

HSD_JObj *HSD_JObjAlloc(void)
{

    HSD_JObj *jobj = hsdNew(default_class != NULL ? default_class : &hsdJObj.parent);
    HSD_ASSERT(1993, jobj);
    return jobj;
}

void HSD_JObjSetCurrent(HSD_JObj *jobj)
{
    HSD_JObjRef(jobj);
    HSD_JObjUnref(current_jobj);
    current_jobj = jobj;
}

HSD_JObj *HSD_JObjGetCurrent(void)
{
    return current_jobj;
}

void HSD_JObjSetMtxDirtySub(HSD_JObj *jobj)
{
    jobj->flags |= JOBJ_MTX_DIRTY;
    if (!(jobj->flags & JOBJ_INSTANCE)) {
        HSD_JObj *child = jobj->child;
        while (child != NULL) {
            if (!(child->flags & JOBJ_MTX_INDEP_PARENT)) {
                if (!HSD_JObjMtxIsDirty(child)) {
                    HSD_JObjSetMtxDirtySub(child);
                }
            }
            child = child->next;
        }
    }
}

void HSD_JObjSetDPtclCallback(DPCtlCallback cb)
{
    dptcl_callback = cb;
}

void HSD_JObjSetPtclTargetCallback(void (*callback)(HSD_JObj *, s32))
{
    ptcltgt_callback = callback;
}

#pragma push
#pragma force_active on
static int JObjInit(HSD_Class *o)
{
    int status = HSD_OBJECT_PARENT_INFO(&hsdJObj)->initialize(o);
    if (status >= 0) {
        HSD_JObj *jobj = (HSD_JObj *)o;
        status = 0;
        jobj->flags = JOBJ_MTX_DIRTY;
        jobj->scale.x = 1.0F;
        jobj->scale.y = 1.0F;
        jobj->scale.z = 1.0F;
    }
    return status;
}
#pragma pop

#pragma push
#pragma force_active on
static inline HSD_JObj *joint_reparent(HSD_JObj *jobj, HSD_JObj *parent)
{
    HSD_JObj *next;

    if (jobj == NULL) {
        return NULL;
    }
    next = jobj->next;
    if (jobj->parent != NULL) {
        if (jobj->parent->child == jobj) {
            jobj->parent->child = next;
        } else {
            HSD_JObj *prev = HSD_JObjGetPrev(jobj);
            HSD_ASSERT(1381, prev);
            prev->next = next;
        }
        RecalcParentTrspBits(jobj->parent);
        jobj->parent = NULL;
    }
    jobj->next = NULL;
    HSD_JObjAddChild(parent, jobj);
    return next;
}

static void JObjReleaseChild(HSD_JObj *jobj)
{
    HSD_JObj *child;
    if ((child = jobj->child) != NULL) {
        if (jobj->flags & JOBJ_INSTANCE) {
            HSD_JObjUnref(child);
        } else {
            child->parent = NULL;
            HSD_JObjRemoveAll(jobj->child);
        }
        jobj->child = NULL;
    }
    if (jobj->parent != NULL) {
        joint_reparent(jobj, NULL);
    }
    if (union_type_dobj(jobj)) {
        if (jobj->display_object != NULL) {
            HSD_DObjRemoveAll(jobj->display_object);
            jobj->display_object = NULL;
        }
    }
    if (jobj->robj != NULL) {
        HSD_RObjRemoveAll(jobj->robj);
        jobj->robj = NULL;
    }
    if (jobj->aobj != NULL) {
        HSD_AObjRemove(jobj->aobj);
        jobj->aobj = NULL;
    }
}
#pragma pop

#pragma push
#pragma force_active on
static void JObjRelease(HSD_Class *o)
{
    HSD_JObj *jobj = (HSD_JObj *)o;
    HSD_JOBJ_METHOD(jobj)->release_child(jobj);

    if (HSD_IDGetDataFromTable(NULL, jobj->id, NULL) == jobj) {
        HSD_IDKey id = jobj->id;
        HSD_IDRemoveByIDFromTable(NULL, id);
    }
    if (jobj->scl != NULL) {
        HSD_VecFree(jobj->scl);
    }
    if (jobj->envelopemtx != NULL) {
        HSD_MtxFree(jobj->envelopemtx);
    }
    HSD_OBJECT_PARENT_INFO(&hsdJObj)->release(o);
}
#pragma pop

#pragma push
#pragma force_active on
static void JObjAmnesia(HSD_ClassInfo *info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    if (info == HSD_CLASS_INFO(&hsdJObj)) {
        ufc_callbacks = NULL;
        current_jobj = NULL;
    }
    HSD_OBJECT_PARENT_INFO(&hsdJObj)->forget(info);
}
#pragma pop

#pragma push
#pragma force_active on
static void JObjInfoInit(void)
{
    (void)"jobj[%d,%d]";
    (void)"SKELETON_ROOT ";
    (void)"SKELETON ";
    (void)"ENVELOPE_MODEL ";
    (void)"EFFECTOR ";
    (void)"  rot(L): ";
    (void)"  sca(L): ";
    (void)"  tra(L): ";
    (void)"  rot(G): ";
    (void)"  sca(G): ";
    (void)"  tra(G): ";

    hsdInitClassInfo(HSD_CLASS_INFO(&hsdJObj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_jobj", sizeof(HSD_JObjInfo),
                     sizeof(HSD_JObj));
    HSD_CLASS_INFO(&hsdJObj)->initialize = JObjInit;
    HSD_CLASS_INFO(&hsdJObj)->release = JObjRelease;
    HSD_CLASS_INFO(&hsdJObj)->forget = JObjAmnesia;
    HSD_JOBJ_INFO(&hsdJObj)->make_mtx = HSD_JObjMakeMatrix;
    HSD_JOBJ_INFO(&hsdJObj)->make_pmtx = HSD_JObjMakePositionMtx;
    HSD_JOBJ_INFO(&hsdJObj)->disp = HSD_JObjDispSub;
    HSD_JOBJ_INFO(&hsdJObj)->load = JObjLoad;
    HSD_JOBJ_INFO(&hsdJObj)->release_child = JObjReleaseChild;
    hsdJObj.update = JObjUpdateFunc;
}
#pragma pop

extern f64 __frsqrte(f64);
/* Three Newton steps and a volatile store preserve the original rounding. */
static inline f32 joint_square_root(f32 value)
{
    if (value > 0.0F) {
        f64 estimate = __frsqrte(value);
        volatile f32 result;
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        result = value * estimate;
        return result;
    }
    return value;
}
void HSD_JObjSetupMatrixSub(HSD_JObj *jobj)
{
    Vec3 end_position;
    Vec3 parent_position;
    Vec3 direction;
    HSD_JObj *parent;
    HSD_RObj *robj;
    f32 x_scale;

    HSD_JOBJ_METHOD(jobj)->make_mtx(jobj);
    jobj->flags &= ~JOBJ_MTX_DIRTY;
    if (!(jobj->flags & JOBJ_USER_DEF_MTX)) {
        switch (jobj->flags & JOBJ_JOINT) {
        case JOBJ_JOINT1:
            resolveIKJoint1(jobj);
            break;
        case JOBJ_JOINT2:
            resolveIKJoint2(jobj);
            break;
        case JOBJ_EFFECTOR:
            parent = jobj->parent;
            x_scale = 1.0F;
            if (parent != NULL) {
                robj = HSD_RObjGetByType(parent->robj, REFTYPE_IKHINT, 0);
                if (robj != NULL) {
                    parent_position.x = parent->mtx[0][3];
                    parent_position.y = parent->mtx[1][3];
                    parent_position.z = parent->mtx[2][3];
                    direction.x = parent->mtx[0][0];
                    direction.y = parent->mtx[1][0];
                    direction.z = parent->mtx[2][0];
                    VECScale(&direction, &direction,
                             joint_square_root(1.0F /
                                               (1e-10F + VECDotProduct(&direction, &direction))));
                    if (parent->scl != NULL) {
                        x_scale = parent->scl->x;
                    }
                    VECScale(&direction, &direction, robj->u.ik_hint.bone_length * x_scale);
                    VECAdd(&parent_position, &direction, &end_position);
                    jobj->mtx[0][3] = end_position.x;
                    jobj->mtx[1][3] = end_position.y;
                    jobj->mtx[2][3] = end_position.z;
                }
            }
            break;
        default:
            if (jobj->robj != NULL && jobj != NULL && jobj->robj != NULL) {
                HSD_RObjUpdateAll(jobj->robj, jobj, HSD_JOBJ_METHOD(jobj)->update);
                if (HSD_JObjMtxIsDirty(jobj)) {
                    HSD_JOBJ_METHOD(jobj)->make_mtx(jobj);
                    jobj->flags &= ~JOBJ_MTX_DIRTY;
                }
            }
            break;
        }
        jobj->flags &= ~JOBJ_MTX_DIRTY;
    }
}
extern void PSVECSubtract(Vec3 *, Vec3 *, Vec3 *);
extern void PSVECCrossProduct(Vec3 *, Vec3 *, Vec3 *);
extern void PSMTXRotAxisRad(Mtx, Vec3 *, f32);
extern f32 acosf(f32);
#define VECSubtract PSVECSubtract
#define VECCrossProduct PSVECCrossProduct
#define VECNormalize PSVECNormalize
#define M_PI 3.14159265358979323846
static inline HSD_JObj *jobj_get_joint2(HSD_JObj *jobj)
{
    while (jobj != NULL) {
        if ((jobj->flags & JOBJ_EFFECTOR) == JOBJ_JOINT2) {
            return jobj;
        }
        jobj = jobj->next;
    }
    return NULL;
}
static inline HSD_JObj *jobj_get_effector(HSD_JObj *jobj)
{
    while (jobj != NULL) {
        if ((jobj->flags & JOBJ_EFFECTOR) == JOBJ_EFFECTOR) {
            return jobj;
        }
        jobj = jobj->next;
    }
    return NULL;
}
static inline HSD_JObj *jobj_get_effector_checked(HSD_JObj *eff)
{
    eff = jobj_get_effector(eff);
    HSD_ASSERT(2071, eff);
    if (HSD_RObjGetByType(eff->robj, REFTYPE_JOBJ, 1) != NULL) {
        return eff;
    } else {
        return NULL;
    }
}

void resolveIKJoint2(HSD_JObj *jobj)
{
    Vec3 model_scale = {1.0F, 1.0F, 1.0F};
    Vec3 end_position;
    Vec3 parent_position;
    Vec3 direction;
    Vec3 up_axis;
    Vec3 side_axis;
    Mtx rotation_matrix;
    Vec3 parent_axis;
    Vec3 rotation_axis;

    HSD_JObj *effector;
    f32 dot;
    f32 angle;
    f32 parent_scale;
    f32 inverse_direction_length;
    f32 inverse_up_length;
    s32 limited;
    s32 flip_ik;
    HSD_RObj *lower_limit;
    HSD_RObj *upper_limit;
    HSD_RObj *robj;

    parent_scale = 1.0F;
    effector = jobj_get_effector_checked(jobj->child);
    if (effector == NULL || jobj->parent == NULL) {
        return;
    }
    if (jobj->scl != NULL) {
        model_scale = *jobj->scl;
    }
    {
        MtxPtr mtx = jobj->parent->mtx;
        parent_position.x = mtx[0][3];
        parent_position.y = mtx[1][3];
        parent_position.z = mtx[2][3];
    }
    {
        MtxPtr mtx = jobj->parent->mtx;
        direction.x = mtx[0][0];
        direction.y = mtx[1][0];
        direction.z = mtx[2][0];
    }
    inverse_direction_length =
        joint_square_root(1.0F / (1e-10F + VECDotProduct(&direction, &direction)));
    VECScale(&direction, &direction, inverse_direction_length);
    if (jobj->parent->scl != NULL) {
        parent_scale = jobj->parent->scl->x;
    }
    robj = HSD_RObjGetByType(jobj->parent->robj, REFTYPE_IKHINT, 0);
    HSD_ASSERT(2287, robj);
    VECScale(&direction, &direction, robj->u.ik_hint.bone_length * parent_scale);
    VECAdd(&parent_position, &direction, &end_position);
    VECSubtract(&effector->translate, &end_position, &direction);
    VECScale(
        &direction, &direction,
        joint_square_root(1.0F / (1e-10F + VECDotProduct(&direction, &direction))));
    lower_limit = HSD_RObjGetByType(jobj->robj, REFTYPE_LIMIT, 5);
    upper_limit = HSD_RObjGetByType(jobj->robj, REFTYPE_LIMIT, 6);
    if ((lower_limit != NULL) || (upper_limit != NULL)) {
        limited = 0;
        robj = HSD_RObjGetByType(jobj->robj, REFTYPE_IKHINT, 0);
        HSD_ASSERT(2321, robj);
        flip_ik = robj->flags & 4 ? 1 : 0;
        {
            MtxPtr mtx = jobj->parent->mtx;
            parent_axis.x = mtx[0][0];
            parent_axis.y = mtx[1][0];
            parent_axis.z = mtx[2][0];
        }
        VECNormalize(&parent_axis, &parent_axis);
        dot = VECDotProduct(&parent_axis, &direction);
        if (dot >= 1.0F) {
            angle = 0.0F;
        } else if (dot <= -1.0F) {
            angle = M_PI;
        } else {
            angle = acosf(dot);
        }
        if (flip_ik == 0) {
            angle = -angle;
        }
        if (lower_limit != NULL && angle < lower_limit->u.limit) {
            angle = lower_limit->u.limit;
            limited = 1;
        } else if (upper_limit != NULL) {
            if (upper_limit->u.limit < angle) {
                angle = upper_limit->u.limit;
                limited = 1;
            }
        }
        if (limited != 0) {
            {
                MtxPtr mtx = jobj->parent->mtx;
                rotation_axis.x = mtx[0][2];
                rotation_axis.y = mtx[1][2];
                rotation_axis.z = mtx[2][2];
            }
            PSMTXRotAxisRad(rotation_matrix, &rotation_axis, angle);
            MTXMultVec(rotation_matrix, &parent_axis, &direction);
        }
    }
    {
        MtxPtr mtx = jobj->parent->mtx;
        side_axis.x = mtx[0][2];
        side_axis.y = mtx[1][2];
        side_axis.z = mtx[2][2];
    }
    VECCrossProduct(&side_axis, &direction, &up_axis);
    inverse_up_length =
        joint_square_root(1.0F / (1e-10F + VECDotProduct(&up_axis, &up_axis)));
    VECScale(&up_axis, &up_axis, inverse_up_length);
    VECCrossProduct(&direction, &up_axis, &side_axis);
    jobj->mtx[0][0] = direction.x * model_scale.x;
    jobj->mtx[1][0] = direction.y * model_scale.x;
    jobj->mtx[2][0] = direction.z * model_scale.x;
    jobj->mtx[0][1] = up_axis.x * model_scale.y;
    jobj->mtx[1][1] = up_axis.y * model_scale.y;
    jobj->mtx[2][1] = up_axis.z * model_scale.y;
    jobj->mtx[0][2] = side_axis.x * model_scale.z;
    jobj->mtx[1][2] = side_axis.y * model_scale.z;
    jobj->mtx[2][2] = side_axis.z * model_scale.z;
    jobj->mtx[0][3] = end_position.x;
    jobj->mtx[1][3] = end_position.y;
    jobj->mtx[2][3] = end_position.z;
}
