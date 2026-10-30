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
#define JOBJ_UNK_B18 (1 << 18)
#define JOBJ_UNK_B19 (1 << 19)
#define JOBJ_UNK_B20 (1 << 20)
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
extern void PSVECSubtract(Vec3 *, Vec3 *, Vec3 *);
extern void PSVECCrossProduct(Vec3 *, Vec3 *, Vec3 *);
extern void PSMTXRotAxisRad(Mtx, Vec3 *, f32);
#define VECSubtract PSVECSubtract
#define VECCrossProduct PSVECCrossProduct
extern f64 __frsqrte(f64);
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

void resolveIKJoint1(HSD_JObj *jobj)
{
    Vec3 scale = {1.0F, 1.0F, 1.0F};
    f32 len1;
    s32 flip = 0;
    f32 len2 = 0.0F;
    HSD_JObj *joint2 = jobj_get_joint2(jobj->child);
    Vec3 origin = {0.0F, 0.0F, 0.0F};
    u8 unused[12];
    Vec3 dir;
    Vec3 diff;
    Vec3 side;
    Vec3 up;
    Vec3 axis;
    Vec3 pole;
    Vec3 normal;
    Mtx rot;
    HSD_JObj *effector;
    HSD_RObj *robj;
    HSD_IKHint *hint;
    HSD_JObj *parent;
    f32 dist2;
    f32 roll;
    f32 len1sq;
    f32 len2sq;
    f32 d;
    f32 h;
    f32 along;
    f32 inv0;
    f32 inv1;
    f32 inv2;

    if (jobj->scl != NULL) {
        scale = *jobj->scl;
    }
    robj = HSD_RObjGetByType(jobj->robj, REFTYPE_IKHINT, 0);
    HSD_ASSERT(2118, robj);
    hint = &robj->u.ik_hint;
    roll = hint->rotate_x;
    len1 = hint->bone_length * scale.x;
    if (joint2 != NULL) {
        robj = HSD_RObjGetByType(joint2->robj, REFTYPE_IKHINT, 0);
        HSD_ASSERT(2129, robj);
        len2 = robj->u.ik_hint.bone_length * joint2->scale.x * scale.x;
        flip = robj->flags & 4 ? 1 : 0;
        effector = jobj_get_effector_checked(joint2->child);
    } else {
        effector = jobj_get_effector_checked(jobj->child);
    }
    if (effector != NULL) {
        if (HSD_RObjGetByType(jobj->robj, REFTYPE_JOBJ, 3) == NULL && jobj != NULL) {
            if (jobj->robj != NULL) {
                HSD_RObjUpdateAll(jobj->robj, jobj, HSD_JOBJ_METHOD(jobj)->update);
                if (HSD_JObjMtxIsDirty(jobj)) {
                    HSD_JOBJ_METHOD(jobj)->make_mtx(jobj);
                    jobj->flags &= ~JOBJ_MTX_DIRTY;
                }
            }
        }
        parent = jobj->parent;
        if (parent != NULL) {
            HSD_MtxGetTranslate(parent->mtx, &origin);
        }
        HSD_RObjGetGlobalPosition(effector->robj, 1, &effector->translate);
        VECSubtract(&effector->translate, &origin, &diff);
        dist2 = VECDotProduct(&diff, &diff);
        if (dist2 > 1e-8F) {
            axis = diff;
            if (HSD_RObjGetGlobalPosition(jobj->robj, 3, &pole)) {
                VECSubtract(&pole, &origin, &pole);
                if (roll != 0.0F) {
                    PSMTXRotAxisRad(rot, &axis, roll);
                    MTXMultVec(rot, &pole, &pole);
                }
                VECCrossProduct(&axis, &pole, &normal);
                VECCrossProduct(&normal, &axis, &pole);
            } else {
                normal.x = jobj->mtx[0][2];
                normal.y = jobj->mtx[1][2];
                normal.z = jobj->mtx[2][2];
                VECCrossProduct(&normal, &axis, &pole);
                VECCrossProduct(&axis, &pole, &normal);
            }
            inv0 = joint_square_root(1.0F / (1e-10F + VECDotProduct(&normal, &normal)));
            VECScale(&normal, &side, inv0);
            inv1 = joint_square_root(1.0F / (1e-10F + VECDotProduct(&pole, &pole)));
            VECScale(&pole, &up, inv1);
            len1sq = len1 * len1;
            len2sq = len2 * len2;
            d = len1sq - len2sq;
            d = 0.25F * (((2.0F * (len1sq + len2sq)) - dist2) - ((d * d) / dist2));
            h = d;
            if (d < 0.0F) {
                h = 0.0F;
            }
            d = (len1sq - h) / dist2;
            along = d * joint_square_root(1.0F / (1e-10F + d));
            len2 = h * joint_square_root(1.0F / (1e-10F + h));
        } else {
            along = 0.0F;
            len2 = len1;
        }
        if (flip != 0) {
            len2 = -len2;
        }
        if ((len2sq - h) < dist2) {
            VECScale(&diff, &dir, along);
        } else {
            VECScale(&diff, &dir, -along);
        }
        VECScale(&up, &pole, len2);
        VECAdd(&dir, &pole, &dir);
        inv2 = joint_square_root(1.0F / (1e-10F + VECDotProduct(&dir, &dir)));
        VECScale(&dir, &dir, inv2);
        jobj->mtx[0][0] = dir.x * scale.x;
        jobj->mtx[1][0] = dir.y * scale.x;
        jobj->mtx[2][0] = dir.z * scale.x;
        VECCrossProduct(&side, &dir, &pole);
        jobj->mtx[0][1] = pole.x * scale.y;
        jobj->mtx[1][1] = pole.y * scale.y;
        jobj->mtx[2][1] = pole.z * scale.y;
        jobj->mtx[0][2] = side.x * scale.z;
        jobj->mtx[1][2] = side.y * scale.z;
        jobj->mtx[2][2] = side.z * scale.z;
        jobj->mtx[0][3] = origin.x;
        jobj->mtx[1][3] = origin.y;
        jobj->mtx[2][3] = origin.z;
    }
}
