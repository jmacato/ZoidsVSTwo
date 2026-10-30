#include "jobj.h"

typedef struct Effect3 {
    HSD_JObj *root;
    u8 pad_04[0x41C - 4];
    s32 se_playing;
    u32 se_voice;
} Effect3;

extern void jobjUnrefAll(HSD_JObj *joint);

void sEffectPurge(Effect3 *effect)
{
    if (effect != (void *)0 && effect->root != (void *)0) {
        if (effect->se_playing != 0) {
            effect->se_playing = 0;
        }
        jobjUnrefAll(effect->root);
        effect->root = 0;
    }
}
