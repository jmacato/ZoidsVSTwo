#include "types.h"

extern void HSD_AudioStreamStop(s32 mode);

void TEA_VoiceOff(void)
{
    HSD_AudioStreamStop(0);
}
