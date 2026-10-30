#ifndef ZOIDS_JOBJ_H
#define ZOIDS_JOBJ_H
/* Layout follows
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/jobj.h. The
 * shipped class adds the animation callback. */
#include "object.h"
#include "dobj.h"
#include "robj.h"
#include "fobj.h"
#include "joint_matrix.h"
typedef struct HSD_AnimJoint {
    struct HSD_AnimJoint *child, *next;
    HSD_AObjDesc *aobjdesc;
    HSD_RObjAnimJoint *robj_anim;
    u32 flags;
} HSD_AnimJoint;
typedef struct HSD_MatAnimJoint {
    struct HSD_MatAnimJoint *child, *next;
    HSD_MatAnim *matanim;
} HSD_MatAnimJoint;
typedef struct HSD_JObjInfo {
    HSD_ClassInfo parent;
    s32 (*load)(HSD_JObj *, HSD_JObjDesc *, HSD_JObj *);
    void (*make_mtx)(HSD_JObj *);
    void (*make_pmtx)(HSD_JObj *, Mtx, Mtx);
    void (*disp)(HSD_JObj *, Mtx, Mtx, u32, u32);
    void (*release_child)(HSD_JObj *);
    AnimationUpdate update;
} HSD_JObjInfo;
extern HSD_JObjInfo hsdJObj;
extern void HSD_JObjCheckDepend(HSD_JObj *);
extern void HSD_JObjSetFlags(HSD_JObj *, u32);
extern void HSD_JObjClearFlags(HSD_JObj *, u32);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *);
extern HSD_JObj *HSD_JObjGetCurrent(void);
extern HSD_JObj *HSD_JObjAlloc(void);
extern HSD_DObj *HSD_JObjGetDObj(HSD_JObj *);
extern void HSD_JObjRemoveAnimAllByFlags(HSD_JObj *, u32);
extern void HSD_JObjReqAnimAllByFlags(HSD_JObj *, u32, f32);
extern void HSD_JObjAddAnimAll(HSD_JObj *, HSD_AnimJoint *, HSD_MatAnimJoint *,
                               HSD_ShapeAnimJoint *);
extern void HSD_JObjAnimAll(HSD_JObj *);
extern void HSD_JObjMakeMatrix(HSD_JObj *);
extern void HSD_JObjRemoveAnimByFlags(HSD_JObj *, u32);
extern void HSD_JObjRemoveAnim(HSD_JObj *);
extern void HSD_JObjRemoveAnimAll(HSD_JObj *);
extern void HSD_JObjReqAnimByFlags(HSD_JObj *, u32, f32);
extern void HSD_JObjReqAnimAll(HSD_JObj *, f32);
extern void HSD_JObjReqAnim(HSD_JObj *, f32);
extern void HSD_JObjAddAnim(HSD_JObj *, HSD_AnimJoint *, HSD_MatAnimJoint *,
                            HSD_ShapeAnimJoint *);
extern void HSD_JObjAnim(HSD_JObj *);
extern HSD_JObj *HSD_JObjGetPrev(HSD_JObj *);
extern void HSD_JObjSetFlagsAll(HSD_JObj *, u32);
extern void HSD_JObjClearFlagsAll(HSD_JObj *, u32);
extern void HSD_JObjSetDPtclCallback(void (*)(int, int, int, HSD_JObj *));
extern void HSD_JObjSetPtclTargetCallback(void (*)(HSD_JObj *, s32));
extern HSD_JObj *HSD_JObjLoadJoint(HSD_JObjDesc *);
extern void HSD_JObjResolveRefs(HSD_JObj *, HSD_JObjDesc *);
extern void HSD_JObjResolveRefsAll(HSD_JObj *, HSD_JObjDesc *);
extern void HSD_JObjUnref(HSD_JObj *);
extern void HSD_JObjUnrefThis(HSD_JObj *);
extern void HSD_JObjRemoveAll(HSD_JObj *);
extern void HSD_JObjAddChild(HSD_JObj *, HSD_JObj *);
extern void HSD_JObjSetCurrent(HSD_JObj *);
extern void HSD_JObjDisp(HSD_JObj *, Mtx, u32, u32);
extern void HSD_JObjDispAll(HSD_JObj *, Mtx, u32, u32);
#endif
