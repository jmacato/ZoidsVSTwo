#include "missile.h"

void zdEffectPurge(struct ZoidState *zoid)
{
    s32 i;
    Effect *effect;

    if (zoid != 0) {
        for (i = 0; i < 100; i++) {
            effect = (Effect *)((u8 *)sEffect + i * 0x424);
            if (effect->mode > 1 && effect->zoid == zoid) {
                sEffectPurge(effect);
                effect->root = 0;
            }
        }
    }
}
