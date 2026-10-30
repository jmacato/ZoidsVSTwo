#include "audio.h"

#pragma dont_inline on
s32 TEA_SeKeyOn(u16 effect, u8 volume, u8 pan, u32 unused)
{
    s32 zero = 0;
    s32 started;
    if (effect == -1)
        return -1;
    started = HSD_AudioSFXStart(effect, volume, pan);
    return -1;
}
