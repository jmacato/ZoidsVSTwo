/* Loop calculation follows https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/aobj.c. */
#include "aobj.h"

extern s32 nb_deadanim;
extern s32 nb_activeanim;
extern f32 fmodf__Fff(f32 numerator, f32 denominator);
extern void HSD_FObjStopAnimAll(struct HSD_FObj *functions, void *object,
                               AnimationUpdate update, f32 delta);
extern void HSD_FObjInterpretAnimAll(void *functions, void *object,
                                    AnimationUpdate update, f32 delta);

static inline f32 looped_frame(HSD_AObj *animation)
{
    f32 period = animation->end_frame - animation->rewind_frame;
    f32 frame = animation->current_frame - animation->rewind_frame;

    return fmodf__Fff(frame, period) + animation->rewind_frame;
}

void HSD_AObjInterpretAnim(HSD_AObj *animation, void *object, AnimationUpdate update)
{
    f32 delta;

    if (animation == 0 || (animation->flags & AOBJ_NO_ANIM) != 0) {
        return;
    }
    if ((animation->flags & AOBJ_FIRST_PLAY) != 0) {
        animation->flags &= ~AOBJ_FIRST_PLAY;
        delta = 0.0f;
    } else {
        delta = animation->rate;
        animation->current_frame += delta;
    }
    if ((animation->flags & AOBJ_LOOP) != 0 &&
        animation->end_frame <= animation->current_frame) {
        if (animation->rewind_frame < animation->end_frame) {
            HSD_FObjStopAnimAll(animation->functions, object, update, delta);
            animation->current_frame = looped_frame(animation);
            HSD_FObjReqAnimAll(animation->functions, animation->current_frame);
        } else {
            animation->current_frame = animation->end_frame;
        }
        delta = 0.0f;
        animation->flags |= AOBJ_REWINDED;
    } else {
        animation->flags &= ~AOBJ_REWINDED;
    }
    if ((animation->flags & AOBJ_NO_UPDATE) != 0) {
        HSD_FObjInterpretAnimAll(animation->functions, object, 0, delta);
    } else {
        HSD_FObjInterpretAnimAll(animation->functions, object, update, delta);
    }
    if ((animation->flags & AOBJ_LOOP) == 0 &&
        animation->end_frame <= animation->current_frame) {
        if (animation != 0) {
            HSD_FObjStopAnimAll(animation->functions, object, update, animation->rate);
            animation->flags |= AOBJ_NO_ANIM;
        }
    }
    if ((animation->flags & AOBJ_NO_ANIM) != 0) {
        nb_deadanim++;
    } else {
        nb_activeanim++;
    }
}
