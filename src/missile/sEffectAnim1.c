#include "missile.h"
#include "jobj.h"

#define NULL ((void *)0)

extern s32 getEndF(HSD_JObj *joint);

void sEffectAnim1(Effect *effect)
{
    if (effect != NULL && effect->root != NULL) {
        HSD_JObjAnimAll(effect->root);
        if (getEndF(effect->root) != 0)
            sEffectPurge(effect);
    }
}
