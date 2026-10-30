#include "missile.h"
#include "jobj.h"

#pragma readonly_strings on

#define NULL ((void *)0)
#define JOBJ_MTX_INDEP_SRT 0x02000000
#define JOBJ_USER_DEF_MTX 0x800000
#define JOBJ_MTX_DIRTY 0x40

extern void sGlobal2Local(HSD_JObj *joint);
extern void sMakeMatrix(HSD_JObj *joint);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *joint);

#pragma auto_inline off
static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 605, "jobj");
    return !(jobj->flags & 0x00800000) && (jobj->flags & 0x40);
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

void sEffectFollow(Effect *effect)
{
    s32 n;
    HSD_JObj *joint;
    s32 i;
    HSD_JObj *previous;
    f32 *source;
    s32 rate;
    s32 type;

    if (effect->source == NULL)
        return;
    if (effect->joints[0].joint == NULL)
        return;
    type = effect->type;
    if (type <= 0)
        return;
    for (n = 0; n < 3; n++)
        effect->history[0][n] = effect->root->matrix[n][3];
    rate = Tea_effectdat[type].trace_rate;
    if (rate <= 0)
        rate = 1;
    if ((rate = 30 / rate)) {
        (void)0;
    }
    for (i = 59; i > 0; i--) {
        for (n = 0; n < 3; n++)
            effect->history[i][n] = effect->history[i - 1][n];
    }
    for (i = 17; i > 0; i--) {
        joint = effect->joints[i].joint;
        if (joint != NULL) {
            source = effect->history[i * 3];
            for (n = 0; n < 3; n++)
                joint->matrix[n][3] = source[n];
        }
    }
    previous = effect->root;
    joint = effect->joints[0].joint;
    if (previous != NULL && joint != NULL) {
        for (n = 0; n < 3; n++) {
            if (previous->matrix[n][3] != joint->matrix[n][3])
                break;
        }
        if (n == 3)
            joint->matrix[0][3] += 0.02;
        sGlobal2Local(joint);
    }
    for (i = 1; i < 18; i++) {
        previous = effect->joints[i - 1].joint;
        joint = effect->joints[i].joint;
        if (previous != NULL && joint != NULL) {
            for (n = 0; n < 3; n++) {
                if (previous->matrix[n][3] != joint->matrix[n][3])
                    break;
            }
            if (n == 3)
                joint->matrix[0][3] += 0.02;
            HSD_JObjSetScaleX(joint, 1.0f);
            sGlobal2Local(joint);
        }
    }
    sMakeMatrix(effect->root);
}
