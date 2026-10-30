#include "audio.h"

extern u32 TeaSeqId;

#pragma dont_inline on
void TEA_AudioSeqVolume(u8 volume, u16 time, u8 mode)
{
    u32 value = volume;
    s32 sequence;
    if (TeaSeqId != (u32)-1)
        HSD_AudioSeqVolume(value, time, TeaSeqId, mode);
    for (sequence = 0; sequence < 10000; sequence++) {
        HSD_AudioSeqVolume(value, time, sequence, mode);
    }
}
#pragma dont_inline reset
