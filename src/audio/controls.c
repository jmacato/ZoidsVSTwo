#include "audio.h"

extern u32 TeaSeqId;





#pragma dont_inline on

#pragma dont_inline reset



void TEA_AudioSeqFree(void)
{
    s32 sequence;
    if (TeaSeqId != (u32)-1)
        HSD_AudioSeqFree(TeaSeqId);
    for (sequence = 0; sequence < 10000; sequence++) {
        HSD_AudioSeqFree(sequence);
    }
}

void TEA_AudioSeqStop(void)
{
    s32 sequence;
    if (TeaSeqId != (u32)-1)
        HSD_AudioSeqStop(TeaSeqId);
    for (sequence = 0; sequence < 10000; sequence++) {
        HSD_AudioSeqStop(sequence);
    }
}

#pragma dont_inline on
s32 TEA_SeKeyOn(u16 effect, u8 volume, u8 pan, u32 unused);

extern s16 TEA_SEVOL;



void SeRetDemo(void)
{
    TEA_SeKeyOn(0x220, TEA_SEVOL, 64, 64);
}

void SeCancel(void)
{
    TEA_SeKeyOn(0x1AB, TEA_SEVOL, 64, 64);
}





void SeBuzzerC(void)
{
    TEA_SeKeyOn(0x1AD, TEA_SEVOL, 64, 64);
}

void SeLiftStart(void)
{
    TEA_SeKeyOn(0x219, TEA_SEVOL, 64, 64);
}

void SeLiftStop(void)
{
    TEA_SeKeyOn(0x14F, TEA_SEVOL, 64, 64);
}


