#ifndef ZOIDS_AOBJ_H
#define ZOIDS_AOBJ_H

#include "joint.h"

#define AOBJ_REWINDED 0x04000000
#define AOBJ_FIRST_PLAY 0x08000000
#define AOBJ_NO_UPDATE 0x10000000
#define AOBJ_LOOP 0x20000000
#define AOBJ_NO_ANIM 0x40000000

#define ALL_ANIM 0x7FF

typedef struct HSD_AObj {
    u32 flags;
    f32 current_frame;
    f32 rewind_frame;
    f32 end_frame;
    f32 rate;
    struct HSD_FObj *functions;
    HSD_JObj *joint;
} HSD_AObj;

typedef struct HSD_AObjDesc {
    u32 flags;
    f32 end_frame;
    struct HSD_FObjDesc *functions;
    HSD_JObjDesc *joint;
} HSD_AObjDesc;

typedef union AnimationValue {
    f32 scalar;
    s32 integer;
    Vec3 vector;
} AnimationValue;

typedef void (*AnimationUpdate)(void *object, s32 channel, AnimationValue *value);

typedef struct AnimationEndCallback {
    struct AnimationEndCallback *next;
    void (*callback)(void);
} AnimationEndCallback;

extern void HSD_FObjReqAnimAll(struct HSD_FObj *functions, f32 frame);

static inline void request_animation(HSD_AObj *animation, f32 frame)
{
    if (animation != 0) {
        animation->current_frame = frame;
        animation->flags = (animation->flags & ~AOBJ_NO_ANIM) | AOBJ_FIRST_PLAY;
        HSD_FObjReqAnimAll(animation->functions, frame);
    }
}

#endif
