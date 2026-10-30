#include "missile.h"

#define NULL ((void *)0)

extern void sEffectAnim1(Effect *effect);
extern void sEffectMove(Effect *effect);

void sEffectAnimAll(void)
{
    s32 i;
    Effect *effect;

    for (i = 0; i < 100; i++) {
        effect = (Effect *)((u8 *)sEffect + i * 0x424);
        if (effect->root != NULL) {
            sEffectAnim1(effect);
            if (effect->root != NULL)
                sEffectMove(effect);
        }
    }
}
