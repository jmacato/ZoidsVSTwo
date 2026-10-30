#ifndef ZOIDS_ROBJ_H
#define ZOIDS_ROBJ_H

#include "aobj.h"
#include "objalloc.h"
#include "id.h"
#include "list.h"

/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/robj.h. */
#define ROBJ_TYPE_MASK 0x70000000
#define REFTYPE_EXP 0x00000000
#define REFTYPE_JOBJ 0x10000000
#define REFTYPE_LIMIT 0x20000000
#define REFTYPE_BYTECODE 0x30000000
#define REFTYPE_IKHINT 0x40000000
#define ROBJ_ACTIVE 0x80000000
#define ROBJ_SUBTYPE_MASK 0x0FFFFFFF

typedef struct HSD_Rvalue {
    struct HSD_Rvalue *next;
    u32 flags;
    HSD_JObj *jobj;
} HSD_Rvalue;

typedef struct HSD_RvalueList {
    u32 flags;
    HSD_JObjDesc *joint;
} HSD_RvalueList;

typedef struct HSD_IKHint {
    f32 bone_length;
    f32 rotate_x;
} HSD_IKHint;

typedef struct HSD_Exp {
    union {
        f32 (*func)(void *);
        u8 *bytecode;
    } expr;
    HSD_Rvalue *rvalue;
    u32 nb_args;
    u8 is_bytecode;
} HSD_Exp;

typedef struct HSD_ExpDesc {
    f32 (*func)(void *);
    HSD_RvalueList *rvalue;
} HSD_ExpDesc;

typedef struct HSD_ByteCodeExpDesc {
    u8 *bytecode;
    HSD_RvalueList *rvalue;
} HSD_ByteCodeExpDesc;

typedef struct HSD_RObj {
    struct HSD_RObj *next;
    u32 flags;
    union {
        HSD_JObj *jobj;
        HSD_Exp exp;
        f32 limit;
        HSD_IKHint ik_hint;
    } u;
    HSD_AObj *aobj;
} HSD_RObj;

typedef struct HSD_RObjDesc {
    struct HSD_RObjDesc *next;
    u32 flags;
    union {
        HSD_ExpDesc *exp;
        HSD_ByteCodeExpDesc *bcexp;
        HSD_IKHint *ik_hint;
        HSD_JObjDesc *joint;
        f32 limit;
    } u;
} HSD_RObjDesc;

typedef struct HSD_RObjAnimJoint {
    struct HSD_RObjAnimJoint *next;
    HSD_AObjDesc *aobjdesc;
} HSD_RObjAnimJoint;

extern HSD_ObjAllocData robj_alloc_data;
extern HSD_ObjAllocData rv_alloc_data;
extern f32 *arg_buf;
extern u32 arg_buf_size;
extern void HSD_ObjAllocInit(HSD_ObjAllocData *data, u32 size, u32 alignment);
extern void *HSD_ObjAlloc(HSD_ObjAllocData *data);
extern void HSD_ObjFree(HSD_ObjAllocData *data, void *object);
extern void HSD_AObjRemove(HSD_AObj *animation);
extern HSD_AObj *HSD_AObjLoadDesc(HSD_AObjDesc *description);
extern void HSD_AObjReqAnim(HSD_AObj *animation, f32 frame);
extern void HSD_AObjInterpretAnim(HSD_AObj *animation, void *object,
                                  AnimationUpdate update);
extern void HSD_JObjUnrefThis(HSD_JObj *joint);
extern void HSD_JObjSetupMatrix(HSD_JObj *joint);
extern HSD_RObj *HSD_RObjAlloc(void);
extern void HSD_RObjFree(HSD_RObj *object);
extern void HSD_RvalueRemoveAll(HSD_Rvalue *values);
extern void HSD_RvalueResolveRefsAll(HSD_Rvalue *value, HSD_RvalueList *list);
extern s32 HSD_RObjGetGlobalPosition(HSD_RObj *object, s32 type, Vec3 *position);
extern void RObjUpdateFunc(void *object, s32 type, AnimationValue *value);
extern void expLoadDesc(HSD_Exp *expression, HSD_ExpDesc *description);
extern void bcexpLoadDesc(HSD_Exp *expression, HSD_ByteCodeExpDesc *description);
extern void expEvaluate(HSD_Exp *expression, u32 type, void *object,
                        AnimationUpdate update);
extern void resolveCnsDirUp(HSD_RObj *object, void *joint, AnimationUpdate update);
extern void resolveCnsOrientation(HSD_RObj *object, void *joint,
                                  AnimationUpdate update);
extern void resolveLimits(HSD_RObj *object, void *joint, AnimationUpdate update);
extern void HSD_Panic(const char *file, u32 line, const char *message);
extern void OSReport(const char *format, ...);

#endif
