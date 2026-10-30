#include "types.h"

extern void HSD_AudioStreamVolume(s32 volume);
extern void HSD_AudioStreamStart(char *file, s32 a, s32 b);
extern void HSD_AudioStreamStopAtEnd(s32 a);
extern void AISetStreamVolLeft(u8 volume);
extern void AISetStreamVolRight(u8 volume);

void TEA_VoiceFilePlay(char *file, u8 volume, s32 loop)
{
    HSD_AudioStreamVolume(0);
    HSD_AudioStreamStart(file, 0, 0);
    if (loop == 0)
        HSD_AudioStreamStopAtEnd(0);
    HSD_AudioStreamVolume(volume);
    AISetStreamVolLeft(volume);
    AISetStreamVolRight(volume);
}
