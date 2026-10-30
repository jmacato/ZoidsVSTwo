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

void sEffectDispAll(void)
{
    s32 i;
    s32 j;
    s32 k;
    Effect *effect;

    for (i = 0; i < 100; i++) {
        effect = (Effect *)((u8 *)sEffect + i * 0x424);
        if (effect->root == NULL)
            continue;
        if (effect->mode == 2 && effect->source != 0) {
            for (j = 0; j < 3; j++) {
                for (k = 0; k < 4; k++) {
                    effect->root->matrix[j][k] = effect->source->matrix[j][k];
                }
                effect->position[j] = effect->source->matrix[j][3];
            }
            sGlobal2Local(effect->root);
        }
        HSD_JObjSetTranslateX(effect->root, effect->position[0]);
        HSD_JObjSetTranslateY(effect->root, effect->position[1]);
        HSD_JObjSetTranslateZ(effect->root, effect->position[2]);
        HSD_JObjDispAll(effect->root, 0, 7, 0);
    }
}
