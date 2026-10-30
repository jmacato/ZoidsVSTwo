#include "types.h"
#include "musyx_hardware.h"

extern void seqStop(unsigned long sequence);
extern void seqSpeed(u32 sequence, u16 speed);
extern void seqContinue(u32 sequence);
extern void seqMute(u32 sequence, u32 mask1, u32 mask2);
extern u32 synthFXSetCtrl(u32 voice, u8 control, u8 value);
extern u32 synthSendKeyOff(u32 voice);
extern u32 vidGetInternalId(u32 voice);
extern void synthVolume(u8 volume, u16 time, u8 group, u8 seqMode, u32 sequence);

void sndSeqStop(u32 sequence)
{
    hwDisableIrq();
    seqStop(sequence);
    hwEnableIrq();
}

void sndSeqSpeed(u32 sequence, u16 speed)
{
    hwDisableIrq();
    seqSpeed(sequence, speed);
    hwEnableIrq();
}

void sndSeqContinue(u32 sequence)
{
    hwDisableIrq();
    seqContinue(sequence);
    hwEnableIrq();
}

void sndSeqMute(u32 sequence, u32 mask1, u32 mask2)
{
    hwDisableIrq();
    seqMute(sequence, mask1, mask2);
    hwEnableIrq();
}



u32 sndFXKeyOff(u32 voice)
{
    u32 result;
    hwDisableIrq();
    result = synthSendKeyOff(voice);
    hwEnableIrq();
    return result;
}

u32 sndFXCheck(u32 voice)
{
    return vidGetInternalId(voice) != -1 ? voice : -1;
}

void sndVolume(u8 volume, u16 time, u8 group)
{
    hwDisableIrq();
    synthVolume(volume, time, group, 0, -1);
    hwEnableIrq();
}

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/snd_synthapi.c (CC0-1.0). */

extern u8 synthITDDefault[8][2];
extern u32 synthFXStart(u16 fx, u8 volume, u8 pan, u8 studio, u8 itd);
u32 sndFXStartEx(u16 fx, u8 volume, u8 pan, u8 studio)
{
    u32 result;
    hwDisableIrq();
    result = synthFXStart(fx, volume, pan, studio, synthITDDefault[studio][1]);
    hwEnableIrq();
    return result;
}

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/seq_api.c (CC0-1.0). */

extern void seqVolume(u8 volume, u16 time, u32 sequence, u8 mode);
void sndSeqVolume(u8 volume, u16 time, u32 sequence, u8 mode)
{
    hwDisableIrq();
    seqVolume(volume, time, sequence, mode);
    hwEnableIrq();
}
