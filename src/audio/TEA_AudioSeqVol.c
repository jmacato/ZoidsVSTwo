#include "audio.h"

extern void TEA_AudioSeqVolume(u8 volume, u16 time, u8 mode);

void TEA_AudioSeqVol(u8 volume, u16 time)
{
    TEA_AudioSeqVolume(volume, time, 0);
}
