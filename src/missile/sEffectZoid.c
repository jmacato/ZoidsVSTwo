#include "zoid3.h"
#include "missile.h"

#define NULL ((void *)0)

extern void sGlobal2Local(HSD_JObj *joint);
extern void sMakeMatrix(HSD_JObj *joint);

typedef struct ZoidSlotsV2 {
    u8 pad[0x185C];
    WeaponSlot weapon_slots[30];
} ZoidSlotsV2;

void sEffectZoid(Effect *effect)
{
    ZoidState *zoid;
    HSD_JObj *source;
    HSD_JObj *target;
    s32 i;
    s32 r;
    s32 c;
    s32 type;

    if (effect->source == NULL)
        return;
    type = effect->type;
    if (type <= 0)
        return;
    zoid = effect->zoid;
    if (zoid == NULL)
        return;
    if (zoid->joint == NULL)
        return;
    for (i = 0; i < 17; i++) {
        source = ((ZoidSlotsV2 *)zoid)->weapon_slots[i + 10].joint;
        if (source != NULL) {
            target = effect->joints[i].joint;
            if (target != NULL) {
                for (r = 0; r < 3; r++) {
                    for (c = 0; c < 4; c++) {
                        target->matrix[r][c] = source->matrix[r][c];
                    }
                }
                sGlobal2Local(target);
            }
        }
    }
    sMakeMatrix(effect->root);
}
