#pragma readonly_strings on
#include "joint.h"

#define JOBJ_MTX_INDEP_SRT 0x02000000
#define JOBJ_USER_DEF_MTX 0x800000
#define JOBJ_MTX_DIRTY 0x40

extern void sGlobal2Local(HSD_JObj *joint);
extern void sMakeMatrix(HSD_JObj *joint);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *joint);
extern void __assert(const char *file, s32 line, const char *expr);

#pragma auto_inline off
static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 605, "jobj");
    return !(jobj->flags & JOBJ_USER_DEF_MTX) && (jobj->flags & JOBJ_MTX_DIRTY);
}

#define HSD_JObjSetMtxDirty(jobj) \
    do { \
        if ((jobj) != 0 && !HSD_JObjMtxIsDirty(jobj)) \
            HSD_JObjSetMtxDirtySub(jobj); \
    } while (0)

static inline void HSD_JObjSetScaleX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 805, "jobj");
    jobj->scale.x = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjSetScaleY(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 819, "jobj");
    jobj->scale.y = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjSetScaleZ(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 833, "jobj");
    jobj->scale.z = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline f32 HSD_JObjGetScaleX(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 899, "jobj");
    return jobj->scale.x;
}

static inline f32 HSD_JObjGetScaleY(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 911, "jobj");
    return jobj->scale.y;
}

static inline f32 HSD_JObjGetScaleZ(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 923, "jobj");
    return jobj->scale.z;
}

void jobjCopyPosS(HSD_JObj *source, HSD_JObj *joint)
{
    s32 i;

    if (joint == (void *)0)
        return;
    if (source == (void *)0)
        return;
    for (i = 0; i < 3; i++)
        joint->matrix[i][3] = source->matrix[i][3];
    sGlobal2Local(joint);
    HSD_JObjSetScaleX(joint, HSD_JObjGetScaleX(source));
    HSD_JObjSetScaleY(joint, HSD_JObjGetScaleY(source));
    HSD_JObjSetScaleZ(joint, HSD_JObjGetScaleZ(source));
    HSD_JObjSetMtxDirty(joint);
    sMakeMatrix(joint);
}
