#include "missile.h"
#include "jobj.h"
#include "lobj.h"

#pragma readonly_strings on

#define NULL ((void *)0)
#define JOBJ_MTX_INDEP_SRT 0x02000000
#define JOBJ_USER_DEF_MTX 0x800000
#define JOBJ_MTX_DIRTY 0x40

extern void sGlobal2Local(HSD_JObj *joint);
extern void sMakeMatrix(HSD_JObj *joint);
extern void sEffectFollow(Effect *effect);
extern void sEffectTrace(Effect *effect);
extern void sEffectZoid(Effect *effect);
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

static inline void HSD_JObjSetTranslateX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 952, "jobj");
    jobj->translation.x = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjSetTranslateY(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 966, "jobj");
    jobj->translation.y = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjSetTranslateZ(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 980, "jobj");
    jobj->translation.z = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

void sEffectMove(Effect *effect)
{
    s32 kind;
    s32 type;
    s32 r;
    s32 c;

    if (effect == NULL)
        return;
    if (effect->root == NULL)
        return;
    type = effect->type;
    if (type <= 0)
        return;
    kind = Tea_effectdat[type].kind;
    if (effect->mode == 3 || effect->mode == 2) {
        if (effect->source != 0) {
            for (r = 0; r < 3; r++) {
                for (c = 0; c < 4; c++)
                    effect->root->matrix[r][c] = effect->source->matrix[r][c];
                effect->position[r] = effect->source->matrix[r][3];
            }
            sGlobal2Local(effect->root);
        }
    }
    if (effect->mode == 3)
        kind = EFFECT_KIND_ZOID;
    HSD_JObjSetTranslateX(effect->root, effect->position[0]);
    HSD_JObjSetTranslateY(effect->root, effect->position[1]);
    HSD_JObjSetTranslateZ(effect->root, effect->position[2]);
    sMakeMatrix(effect->root);
    if (effect->joints[0].joint != 0) {
        if (kind == EFFECT_KIND_FOLLOW)
            sEffectFollow(effect);
        if (kind == EFFECT_KIND_TRACE)
            sEffectTrace(effect);
        if (kind == EFFECT_KIND_ZOID)
            sEffectZoid(effect);
    }
}
