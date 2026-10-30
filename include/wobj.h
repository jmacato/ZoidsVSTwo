#ifndef ZOIDS_WOBJ_H
#define ZOIDS_WOBJ_H

#include "aobj.h"
#include "class.h"

/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/wobj.h. */
#define WOBJ_PATH_LOCAL 0x1
#define WOBJ_POSITION_DIRTY 0x2

struct HSD_WObj;
struct HSD_WObjDesc;

typedef struct HSD_WObjInfo {
    HSD_ClassInfo parent;
    s32 (*load)(struct HSD_WObj *object, struct HSD_WObjDesc *description);
    AnimationUpdate update;
} HSD_WObjInfo;

typedef struct HSD_WObj {
    HSD_WObjInfo *class_info;
    u16 reference_count;
    u16 individual_reference_count;
    u32 flags;
    Vec3 position;
    HSD_AObj *animation;
    struct HSD_RObj *render_object;
} HSD_WObj;

typedef struct HSD_WObjDesc {
    const char *class_name;
    Vec3 position;
    struct HSD_RObjDesc *render_description;
} HSD_WObjDesc;

typedef struct HSD_WObjAnim {
    HSD_AObjDesc *animation;
    struct HSD_RObjAnimJoint *render_animation;
} HSD_WObjAnim;

extern void HSD_AObjReqAnim(HSD_AObj *animation, f32 frame);
extern void HSD_AObjRemove(HSD_AObj *animation);
extern HSD_AObj *HSD_AObjLoadDesc(HSD_AObjDesc *description);
extern void HSD_AObjInterpretAnim(HSD_AObj *animation, void *object,
                                  AnimationUpdate update);
extern void HSD_RObjReqAnimAll(struct HSD_RObj *object, f32 frame);
extern void HSD_RObjAddAnimAll(struct HSD_RObj *object,
                               struct HSD_RObjAnimJoint *animation);
extern void HSD_RObjAnimAll(struct HSD_RObj *object);
extern void HSD_RObjRemoveAll(struct HSD_RObj *object);
extern struct HSD_RObj *HSD_RObjLoadDesc(struct HSD_RObjDesc *description);
extern void HSD_RObjResolveRefsAll(struct HSD_RObj *object,
                                   struct HSD_RObjDesc *description);
extern void HSD_WObjSetPosition(HSD_WObj *object, Vec3 *position);
extern void HSD_WObjSetPositionX(HSD_WObj *object, f32 value);
extern void HSD_WObjSetPositionY(HSD_WObj *object, f32 value);
extern void HSD_WObjSetPositionZ(HSD_WObj *object, f32 value);
extern HSD_WObj *HSD_WObjAlloc(void);

#endif
