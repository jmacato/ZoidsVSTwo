/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/robj.c. */
#include "robj.h"
#include "joint_matrix.h"
#include "constraint_math.h"

void HSD_RObjInitAllocData(void)
{
    HSD_ObjAllocInit(&robj_alloc_data, sizeof(HSD_RObj), 4);
    HSD_ObjAllocInit(&rv_alloc_data, sizeof(HSD_Rvalue), 4);
}





HSD_RObj *HSD_RObjGetByType(HSD_RObj *constraint, u32 type, u32 subtype)
{
    s32 active;
    HSD_RObj *curr;

    if (constraint == 0) {
        return 0;
    }

    for (curr = constraint; curr != 0; curr = curr->next) {
        if (curr->flags & ROBJ_ACTIVE) {
            active = 1;
        } else {
            active = 0;
        }

        if (active) {
            if ((curr->flags & ROBJ_TYPE_MASK) == type &&
                (!subtype || subtype == (curr->flags & ROBJ_SUBTYPE_MASK))) {
                return curr;
            }
        }
    }

    return 0;
}

void RObjUpdateFunc(void *object, s32 type, AnimationValue *val)
{
    HSD_RObj *constraint;

    if (object == 0) {
        return;
    }

    if (type != 1) {
        return;
    }

    constraint = (HSD_RObj *)object;
    if (val->scalar >= 0.5) {
        constraint->flags = constraint->flags | ROBJ_ACTIVE;
        return;
    }
    constraint->flags = constraint->flags & ~ROBJ_ACTIVE;
}

static inline void HSD_RObjAnim(HSD_RObj *constraint)
{
    if (constraint == 0) {
        return;
    }

    HSD_AObjInterpretAnim(constraint->aobj, constraint, RObjUpdateFunc);
}

void HSD_RObjAnimAll(HSD_RObj *constraint)
{
    HSD_RObj *curr;

    if (constraint == 0) {
        return;
    }

    for (curr = constraint; curr != 0; curr = curr->next) {
        HSD_RObjAnim(curr);
    }
}

static inline void HSD_RObjRemoveAnimByFlags(HSD_RObj *constraint, u32 flags)
{
    if (constraint == 0) {
        return;
    }

    if (constraint->aobj != 0 && (flags & 0x80) != 0) {
        HSD_AObjRemove(constraint->aobj);
        constraint->aobj = 0;
    }
}

void HSD_RObjRemoveAnimAllByFlags(HSD_RObj *constraint, u32 flags)
{
    if (constraint == 0) {
        return;
    }

    for (; constraint != 0; constraint = constraint->next) {
        HSD_RObjRemoveAnimByFlags(constraint, flags);
    }
}

static inline void HSD_RObjReqAnimByFlags(HSD_RObj *constraint, f32 startframe,
                                          u32 flags)
{
    if (constraint == 0) {
        return;
    }

    if (constraint->aobj != 0 && (flags & 0x80) != 0) {
        HSD_AObjReqAnim(constraint->aobj, startframe);
    }
}

void HSD_RObjReqAnimAllByFlags(HSD_RObj *constraint, f32 startframe, u32 flags)
{
    if (constraint == 0) {
        return;
    }

    for (; constraint != 0; constraint = constraint->next) {
        HSD_RObjReqAnimByFlags(constraint, startframe, flags);
    }
}

void HSD_RObjReqAnimAll(HSD_RObj *constraint, f32 startframe)
{
    HSD_RObjReqAnimAllByFlags(constraint, startframe, ALL_ANIM);
}

static inline void HSD_RObjAddAnim(HSD_RObj *constraint, HSD_RObjAnimJoint *anim)
{
    if (constraint == 0 || anim == 0) {
        return;
    }

    if (constraint->aobj != 0) {
        HSD_AObjRemove(constraint->aobj);
    }
    constraint->aobj = HSD_AObjLoadDesc(anim->aobjdesc);
}

void HSD_RObjAddAnimAll(HSD_RObj *constraint, HSD_RObjAnimJoint *anim)
{
    HSD_RObj *i;
    HSD_RObjAnimJoint *j;

    if (constraint == 0 || anim == 0) {
        return;
    }

    for (i = constraint, j = anim; i != 0 && j != 0; i = i->next, j = j->next) {
        HSD_RObjAddAnim(i, j);
    }
}

static inline u32 HSD_RObjGetConstraintType(HSD_RObj *constraint)
{
    if (constraint == 0) {
        return 0;
    }

    return constraint->flags & ROBJ_SUBTYPE_MASK;
}

int HSD_RObjGetGlobalPosition(HSD_RObj *constraint, int type, Vec3 *p)
{
    Vec3 v = {0, 0, 0};
    HSD_RObj *reference;
    int n = 0;

    if (constraint == 0) {
        return 0;
    }
    for (reference = constraint; reference != 0; reference = reference->next) {
        if (((reference->flags & ROBJ_TYPE_MASK) == REFTYPE_JOBJ ? 1 : 0) != 0) {
            if (((reference->flags & ROBJ_ACTIVE) ? 1 : 0) != 0 &&
                (unsigned)type == HSD_RObjGetConstraintType(reference)) {
                ((reference->u.jobj) ? (void)0 : __assert("robj.c", 498, "rp->u.jobj"));
                setup_matrix(reference->u.jobj);
                n += 1;
                v.x += reference->u.jobj->matrix[0][3];
                v.y += reference->u.jobj->matrix[1][3];
                v.z += reference->u.jobj->matrix[2][3];
            }
        }
    }
    if (n != 0) {
        f32 f = (f32)1.0 / (f32)n;
        p->x = f * v.x;
        p->y = f * v.y;
        p->z = f * v.z;
    }
    return n;
}

void set_dirup_matrix(Vec3 *direction, Vec3 *up, Vec3 *scale, void *object,
                      AnimationUpdate update)
{
    Vec3 side;
    Vec3 v;
    f32 side_scale;
    f32 direction_scale;

    PSVECCrossProduct(direction, up, &side);
    direction_scale = constraint_root(
        1.0F / (1.00000001335e-10f + PSVECDotProduct(direction, direction)));
    PSVECScale(direction, direction, direction_scale);
    side_scale =
        constraint_root(1.0F / (1.00000001335e-10f + PSVECDotProduct(&side, &side)));
    PSVECScale(&side, &side, side_scale);
    PSVECCrossProduct(&side, direction, up);
    v.x = direction->x * scale->x;
    v.y = direction->y * scale->x;
    v.z = direction->z * scale->x;
    update(object, JOBJ_UPDATE_MTX_COL0, (AnimationValue *)&v);
    v.x = up->x * scale->y;
    v.y = up->y * scale->y;
    v.z = up->z * scale->y;
    update(object, JOBJ_UPDATE_MTX_COL1, (AnimationValue *)&v);
    v.x = side.x * scale->z;
    v.y = side.y * scale->z;
    v.z = side.z * scale->z;
    update(object, JOBJ_UPDATE_MTX_COL2, (AnimationValue *)&v);
    update(object, JOBJ_UPDATE_MTX_GET_ROTATION, 0);
}

extern void set_dirup_matrix(Vec3 *direction, Vec3 *up, Vec3 *scale, void *object,
                             AnimationUpdate update);

void resolveCnsDirUp(HSD_RObj *constraint, void *object, AnimationUpdate update)
{
    Vec3 scale = {1.0f, 1.0f, 1.0f};
    Vec3 up = {0.0f, 1.0f, 0.0f};
    Vec3 position;
    Vec3 dir;
    f32 k;

    if (HSD_RObjGetGlobalPosition(constraint, 2, &position) != 0) {
        dir.x = ((HSD_JObj *)object)->matrix[0][3];
        dir.y = ((HSD_JObj *)object)->matrix[1][3];
        dir.z = ((HSD_JObj *)object)->matrix[2][3];
        PSVECSubtract(&position, &dir, &position);
        if (HSD_RObjGetGlobalPosition(constraint, 3, &up) != 0) {
            PSVECSubtract(&up, &dir, &up);
        } else {
            k = 1.0f - PSVECDotProduct(&position, &up);
            if (float_absolute(k) < 1.00000001335e-10f) {
                up.x = 0.0f;
                up.y = 0.0f;
                up.z = 1.0;
            }
        }

        if (((HSD_JObj *)object)->scale_pointer != 0) {
            scale = *((HSD_JObj *)object)->scale_pointer;
        }
        set_dirup_matrix(&position, &up, &scale, object, update);
    }
}

static inline HSD_RObj *inlined_HSD_RObjGetByType(HSD_RObj *constraint, u32 type,
                                                  u32 subtype)
{
    s32 active;
    HSD_RObj *curr;

    if (constraint == 0) {
        return 0;
    }

    for (curr = constraint; curr != 0; curr = curr->next) {
        if (curr->flags & ROBJ_ACTIVE) {
            active = 1;
        } else {
            active = 0;
        }

        if (active) {
            if ((curr->flags & ROBJ_TYPE_MASK) == type &&
                (!subtype || subtype == (curr->flags & ROBJ_SUBTYPE_MASK))) {
                return curr;
            }
        }
    }

    return 0;
}

extern void PSMTXCopy(f32 source[3][4], f32 destination[3][4]);
extern f32 PSVECMag(Vec3 *vector);

static inline f32 (*get_matrix(HSD_JObj *jobj))[4]
{
    if (jobj == 0) {
        __assert("jobj.h", 1148, "jobj");
    }
    HSD_JObjSetupMatrix(jobj);
    return jobj->matrix;
}

static inline f32 orientation_root(f32 value, volatile f32 *rounding)
{
    if (value > 0.0f) {
        f64 estimate = __frsqrte(value);

        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        *rounding = value * estimate;
        return *rounding;
    }
    return value;
}

static inline f32 column_magnitude(f32 matrix[3][4], s32 column, volatile f32 *rounding)
{
    return orientation_root(matrix[2][column] * matrix[2][column] +
                                (matrix[0][column] * matrix[0][column] +
                                 matrix[1][column] * matrix[1][column]),
                            rounding);
}

void resolveCnsOrientation(HSD_RObj *constraint, void *object, AnimationUpdate update)
{
    f32 matrix[3][4];
    f32 factor;
    s32 has_object = object != 0;
    Vec3 vector;
    /* The caller keeps the original stack slot for single-precision rounding. */
    volatile f32 rounding;
    HSD_JObj *jobj;
    s32 axis;
    static s32 type[] = {JOBJ_UPDATE_MTX_COL0, JOBJ_UPDATE_MTX_COL1, JOBJ_UPDATE_MTX_COL2};

    ((has_object) ? (void)0 : __assert("robj.c", 0x276, "obj"));
    constraint = inlined_HSD_RObjGetByType(constraint, REFTYPE_JOBJ, 4);
    if (constraint == 0) {
        return;
    }
    PSMTXCopy(get_matrix(constraint->u.jobj), matrix);
    jobj = object;
    for (axis = 0; axis < 3; axis++) {
        vector.x = matrix[0][axis];
        vector.y = matrix[1][axis];
        vector.z = matrix[2][axis];
        factor = PSVECMag(&vector);
        if (factor > 1e-10f) {
            factor = 1.0f / factor;
        }
        factor *= column_magnitude(jobj->matrix, axis, &rounding);
        vector.x *= factor;
        vector.y *= factor;
        vector.z *= factor;
        update(object, type[axis], (AnimationValue *)&vector);
    }
    update(object, JOBJ_UPDATE_MTX_GET_ROTATION, 0);
}

void HSD_JObjSetupMatrix(HSD_JObj *joint)
{
    if (joint == 0 || !matrix_is_dirty(joint)) {
        return;
    }
    HSD_JObjSetupMatrixSub(joint);
}

extern void HSD_JObjMakeMatrix(HSD_JObj *joint);

static inline s32 RObjHasLimitReftype(HSD_RObj *object)
{
    if ((object->flags & ROBJ_TYPE_MASK) == REFTYPE_LIMIT) {
        return 1;
    }
    return 0;
}

void resolveLimits(HSD_RObj *constraint, void *object, AnimationUpdate update)
{
    HSD_JObj *jobj = (HSD_JObj *)object;
    HSD_RObj *reference;
    s32 update_mtx = 0;

    ((jobj) ? (void)0 : __assert("robj.c", 0x29E, "jobj"));

    reference = constraint;
    while (reference != 0) {
        if (RObjHasLimitReftype(reference)) {
            break;
        }
        reference = reference->next;
    }

    if (reference != 0) {
        for (reference = constraint; reference != 0; reference = reference->next) {
            if (RObjHasLimitReftype(reference)) {
                switch (reference->flags & ROBJ_SUBTYPE_MASK) {
                default:
                    continue;
                case 1:
                    if (jobj->rotation[0] < reference->u.limit) {
                        jobj->rotation[0] = reference->u.limit;
                    }
                    break;

                case 2:
                    if (jobj->rotation[0] > reference->u.limit) {
                        jobj->rotation[0] = reference->u.limit;
                    }
                    break;

                case 3:
                    if (jobj->rotation[1] < reference->u.limit) {
                        jobj->rotation[1] = reference->u.limit;
                    }
                    break;

                case 4:
                    if (jobj->rotation[1] > reference->u.limit) {
                        jobj->rotation[1] = reference->u.limit;
                    }
                    break;

                case 5:
                    if (jobj->rotation[2] < reference->u.limit) {
                        jobj->rotation[2] = reference->u.limit;
                    }
                    break;

                case 6:
                    if (jobj->rotation[2] > reference->u.limit) {
                        jobj->rotation[2] = reference->u.limit;
                    }
                    break;

                case 7:
                    if (jobj->translation.x < reference->u.limit) {
                        jobj->translation.x = reference->u.limit;
                    }
                    break;

                case 8:
                    if (jobj->translation.x > reference->u.limit) {
                        jobj->translation.x = reference->u.limit;
                    }
                    break;

                case 9:
                    if (jobj->translation.y < reference->u.limit) {
                        jobj->translation.y = reference->u.limit;
                    }
                    break;

                case 10:
                    if (jobj->translation.y > reference->u.limit) {
                        jobj->translation.y = reference->u.limit;
                    }
                    break;

                case 11:
                    if (jobj->translation.y < reference->u.limit) {
                        jobj->translation.y = reference->u.limit;
                    }
                    break;

                case 12:
                    if (jobj->translation.y > reference->u.limit) {
                        jobj->translation.y = reference->u.limit;
                    }
                    break;
                }
                update_mtx = 1;
            }
        }
        if (update_mtx) {
            HSD_JObjMakeMatrix(jobj);
        }
    }
}

static inline s32 RObjHasFlags(HSD_RObj *object)
{
    if ((object->flags & ROBJ_TYPE_MASK) == REFTYPE_EXP) {
        return 1;
    }
    return 0;
}
static inline s32 RObjHasFlags2(HSD_RObj *object)
{
    if ((object->flags & ROBJ_ACTIVE) != 0) {
        return 1;
    }
    return 0;
}

void HSD_RObjUpdateAll(HSD_RObj *constraint, void *object, AnimationUpdate update)
{
    HSD_RObj *reference;
    Vec3 vec;

    if (constraint != 0) {
        if (HSD_RObjGetGlobalPosition(constraint, 1, &vec) != 0) {
            update(object, JOBJ_UPDATE_MTX_COL3, (AnimationValue *)&vec);
            update(object, JOBJ_UPDATE_MTX_GET_TRANSLATE, 0);
        }
        resolveCnsDirUp(constraint, object, update);
        resolveCnsOrientation(constraint, object, update);
        resolveLimits(constraint, object, update);

        for (reference = constraint; reference != 0; reference = reference->next) {
            if (RObjHasFlags(reference) && RObjHasFlags2(reference)) {
                expEvaluate(&reference->u.exp, reference->flags & ROBJ_SUBTYPE_MASK, object,
                            update);
            }
        }
    }
}

static inline void reference_joint(HSD_JObj *joint)
{
    s32 has_joint = joint != 0;
    if (has_joint) {
        u16 *count = &joint->individual_reference_count;
        (*count)++;
        ((*count != 0)
             ? (void)0
             : __assert("object.h", 158, "HSD_OBJ(o)->ref_count_individual != 0"));
    }
}

static inline void resolve_reference(HSD_RObj *constraint, HSD_RObjDesc *description)
{
    if (constraint != 0 && description != 0) {
        switch (constraint->flags & ROBJ_TYPE_MASK) {
        case REFTYPE_JOBJ:
            HSD_JObjUnrefThis(constraint->u.jobj);
            constraint->u.jobj =
                HSD_IDGetDataFromTable(0, (u32)description->u.joint, 0);
            ((constraint->u.jobj != 0) ? (void)0
                                       : __assert("robj.c", 0x330, "robj->u.jobj"));
            reference_joint(constraint->u.jobj);
            break;
        case REFTYPE_EXP:
            HSD_RvalueResolveRefsAll(constraint->u.exp.rvalue,
                                     description->u.exp->rvalue);
            break;
        }
    }
}

void HSD_RObjResolveRefsAll(HSD_RObj *constraint, HSD_RObjDesc *description)
{
    for (; constraint != 0 && description != 0;
         constraint = constraint->next, description = description->next) {
        resolve_reference(constraint, description);
    }
}

HSD_RObj *HSD_RObjLoadDesc(HSD_RObjDesc *description)
{
    HSD_RObj *constraint;

    if (description != 0) {
        constraint = HSD_RObjAlloc();
        constraint->next = HSD_RObjLoadDesc(description->next);
        constraint->flags = description->flags;
        switch (constraint->flags & ROBJ_TYPE_MASK) {
        case REFTYPE_JOBJ:
            break;
        case REFTYPE_LIMIT: {
            switch (constraint->flags & ROBJ_SUBTYPE_MASK) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                constraint->u.limit = 0.017453292f * description->u.limit;
                break;
            default:
                constraint->u.limit = description->u.limit;
                break;
            }
        } break;
        case REFTYPE_EXP:
            expLoadDesc(&constraint->u.exp, description->u.exp);
            break;
        case REFTYPE_BYTECODE:
            bcexpLoadDesc(&constraint->u.exp, description->u.bcexp);
            constraint->flags &= ~ROBJ_TYPE_MASK;
            break;
        case REFTYPE_IKHINT:
            constraint->u.ik_hint.bone_length = description->u.ik_hint->bone_length;
            constraint->u.ik_hint.rotate_x = description->u.ik_hint->rotate_x;
            break;
        default:
            HSD_Panic("robj.c", 0x37D, "unexpected type of robj.\n");
            break;
        }
        return constraint;
    }
    return 0;
}

static inline void HSD_RObjRemove(HSD_RObj *constraint)
{

    if (constraint != 0) {
        switch (constraint->flags & ROBJ_TYPE_MASK) {
        case REFTYPE_JOBJ:
            HSD_JObjUnrefThis(constraint->u.jobj);
            break;
        case REFTYPE_EXP:
            HSD_RvalueRemoveAll(constraint->u.exp.rvalue);
            break;
        }
        HSD_AObjRemove(constraint->aobj);
        HSD_RObjFree(constraint);
    }
}

void HSD_RObjRemoveAll(HSD_RObj *constraint)
{
    HSD_RObj *next;

    for (; constraint != 0; constraint = next) {
        next = constraint->next;
        HSD_RObjRemove(constraint);
    }
}

HSD_RObj *HSD_RObjAlloc(void)
{
    HSD_RObj *object = HSD_ObjAlloc(&robj_alloc_data);
    ((object) ? (void)0 : __assert("robj.c", 965, "new"));
    memset(object, 0, sizeof(*object));
    return object;
}

void HSD_RObjFree(HSD_RObj *constraint)
{
    HSD_ObjFree(&robj_alloc_data, constraint);
}

extern u32 HSD_GetNbBits(u32 bits);
extern void HSD_MtxGetRotation(f32 matrix[3][4], Vec3 *rotation);
extern void HSD_MtxGetScale(f32 matrix[3][4], Vec3 *scale);
extern f32 HSD_ByteCodeEval(u8 *code, f32 *arguments, u32 count);

void expEvaluate(HSD_Exp *expression, u32 type, void *object, AnimationUpdate update)
{
    HSD_Rvalue *value;
    HSD_JObj *jobj;
    Vec3 scale;
    float degrees_per_radian;
    float *argument;
    int bit;
    Vec3 rotation;
    s32 count_arguments = expression->nb_args == -1;
    AnimationValue result;
    HSD_RObj *constraint = (HSD_RObj *)object;

    /* This retained SDK assertion string preserves the shared data-pool layout. */
    (void)"(ptr && nitems) || !ptr";
    if (count_arguments) {
        u32 nb_args = 0;
        HSD_Rvalue *value;
        for (value = expression->rvalue; value != 0; value = value->next) {
            nb_args += HSD_GetNbBits(value->flags);
        }
        expression->nb_args = nb_args;
    }
    if (arg_buf == 0) {
        if (arg_buf_size == 0) {
            arg_buf_size = 100;
        }
        arg_buf = HSD_Alloc(arg_buf_size * sizeof(float));
    }
    if (arg_buf_size < expression->nb_args) {
        OSReport("Number of argment of expression exceeds the argument buffer\n"
                 "size. (requested num of arg %d, allocated %d)\n",
                 expression->nb_args, arg_buf_size);
        HSD_Panic("robj.c", 0x41B, "");
    }
    argument = arg_buf;
    degrees_per_radian = 57.29578F;
    for (value = expression->rvalue; value != 0; value = value->next) {
        jobj = value->jobj;
        ((jobj) ? (void)0 : __assert("robj.c", 0x424, "jobj"));
        setup_matrix(value->jobj);
        for (bit = 1; bit && bit <= value->flags; bit <<= 1) {
            switch (value->flags & bit) {
            case 0x1:
                *argument++ = degrees_per_radian * jobj->rotation[0];
                break;
            case 0x2:
                *argument++ = degrees_per_radian * jobj->rotation[1];
                break;
            case 0x4:
                *argument++ = degrees_per_radian * jobj->rotation[2];
                break;
            case 0x8:
                break;
            case 0x10:
                *argument++ = jobj->translation.x;
                break;
            case 0x20:
                *argument++ = jobj->translation.y;
                break;
            case 0x40:
                *argument++ = jobj->translation.z;
                break;
            case 0x80:
                *argument++ = jobj->scale.x;
                break;
            case 0x100:
                *argument++ = jobj->scale.y;
                break;
            case 0x200:
                *argument++ = jobj->scale.z;
                break;
            case 0x400:
            case 0x800:
                break;
            case 0x10000:
                HSD_MtxGetRotation(jobj->matrix, &rotation);
                *argument++ = degrees_per_radian * rotation.x;
                break;
            case 0x20000:
                HSD_MtxGetRotation(jobj->matrix, &rotation);
                *argument++ = degrees_per_radian * rotation.y;
                break;
            case 0x40000:
                HSD_MtxGetRotation(jobj->matrix, &rotation);
                *argument++ = degrees_per_radian * rotation.z;
                break;
            case 0x100000:
                *argument++ = jobj->matrix[0][3];
                break;
            case 0x200000:
                *argument++ = jobj->matrix[1][3];
                break;
            case 0x400000:
                *argument++ = jobj->matrix[2][3];
                break;
            case 0x800000:
                HSD_MtxGetScale(jobj->matrix, &scale);
                *argument++ = scale.x;
                break;
            case 0x1000000:
                HSD_MtxGetScale(jobj->matrix, &scale);
                *argument++ = scale.y;
                break;
            case 0x2000000:
                HSD_MtxGetScale(jobj->matrix, &scale);
                *argument++ = scale.z;
                break;
            }
        }
    }
    if (expression->is_bytecode) {
        result.scalar =
            HSD_ByteCodeEval(expression->expr.bytecode, arg_buf, expression->nb_args);
    } else {
        result.scalar = expression->expr.func(arg_buf);
    }
    if (type - 1 <= 2) {
        result.scalar = result.scalar * 0.017453292F;
    }
    update(object, type, &result);
}

f32 dummy_func(void *unused)
{
    return 0.0f;
}

static inline void HSD_RvalueRemove(HSD_Rvalue *value)
{
    if (value != 0) {
        HSD_JObjUnrefThis(value->jobj);
        HSD_ObjFree(&rv_alloc_data, value);
    }
}

void HSD_RvalueRemoveAll(HSD_Rvalue *value)
{
    HSD_Rvalue *next;

    for (; value != 0; value = next) {
        next = value->next;
        HSD_RvalueRemove(value);
    }
}

extern f32 dummy_func(void *unused);

static inline HSD_Rvalue *HSD_RvalueAlloc(void)
{
    HSD_Rvalue *value = HSD_ObjAlloc(&rv_alloc_data);
    ((value) ? (void)0 : __assert("robj.c", 1157, "rvalue"));
    memset(value, 0, sizeof(HSD_Rvalue));
    return value;
}

static inline HSD_Rvalue *loadRvalue(HSD_RvalueList *descriptions)
{
    HSD_Rvalue *reference;
    HSD_SList rv;

    rv.next = 0;
    reference = (HSD_Rvalue *)&rv;
    if (descriptions == 0) {
        return 0;
    } else {
        for (; descriptions->joint != 0; descriptions++) {
            reference->next = HSD_RvalueAlloc();
            reference->next->flags = descriptions->flags;
            reference = reference->next;
        }
    }
    return (HSD_Rvalue *)rv.next;
}

void expLoadDesc(HSD_Exp *expression, HSD_ExpDesc *desc)
{
    memset(expression, 0, sizeof(HSD_Exp));
    if (desc != 0) {
        if (desc->func != 0) {
            expression->expr.func = desc->func;
        } else {
            expression->expr.func = dummy_func;
        }
        expression->rvalue = loadRvalue(desc->rvalue);
        expression->nb_args = -1;
    }
}

extern f32 dummy_func(void *unused);

void bcexpLoadDesc(HSD_Exp *expression, HSD_ByteCodeExpDesc *desc)
{
    memset(expression, 0, sizeof(HSD_Exp));
    if (desc != 0) {
        if (desc->bytecode != 0) {
            expression->expr.bytecode = desc->bytecode;
        } else {
            expression->expr.bytecode = 0;
        }
        expression->rvalue = loadRvalue(desc->rvalue);
        expression->nb_args = -1;
        expression->is_bytecode = 1;
    }
}

static inline void resolve_value(HSD_Rvalue *value, HSD_RvalueList *description)
{
    if (value != 0 && description != 0) {
        HSD_JObjUnrefThis(value->jobj);
        value->jobj = HSD_IDGetDataFromTable(0, (u32)description->joint, 0);
        ((value->jobj != 0) ? (void)0 : __assert("robj.c", 0x4F2, "rvalue->jobj"));
        reference_joint(value->jobj);
    }
}

void HSD_RvalueResolveRefsAll(HSD_Rvalue *value, HSD_RvalueList *description)
{
    if (description == 0) {
        return;
    }
    for (; value != 0 && description->joint != 0; value = value->next, description++) {
        resolve_value(value, description);
    }
}


