#include "musyx_synth.h"

extern u8 sndActive;
extern void hwExit(void);
extern void dataExit(void);
extern void s3dExit(void);
extern void synthExit(void);



/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/snd_init.c (CC0-1.0). */
extern void dataInitStack(void);
extern void dataInit(unsigned long base, unsigned long aramSize);
extern void seqInit(void);
extern u8 synthIdleWaitActive;
extern void synthInit(unsigned long frequency, unsigned long voices);
extern void streamInit(void);
extern void vsInit(void);
extern void s3dInit(unsigned long flags);
extern signed long hwInit(unsigned long *frequency, u16 voices, u16 studios, unsigned long flags);

static inline signed long DoInit(unsigned long mixFrq, unsigned long aramSize,
                                unsigned long numVoices, unsigned long flags)
{
    unsigned long ret;
    ret = 0;
    dataInitStack();
    dataInit(0, aramSize);
    seqInit();
    synthIdleWaitActive = 0;
    synthInit(mixFrq, numVoices);
    streamInit();
    vsInit();
    s3dInit(flags);
    sndActive = 1;
    return ret;
}

signed long sndInit(u8 voices, u8 music, u8 sfx, u8 studios,
                    unsigned long flags, unsigned long aramSize)
{
    signed long ret;
    unsigned long frq;

    ret = 0;
    sndActive = 0;
    if (voices <= 64) {
        synthInfo.voice_count = voices;
    } else {
        synthInfo.voice_count = 64;
    }
    if (studios <= 8) {
        synthInfo.studioCount = studios;
    } else {
        synthInfo.studioCount = 8;
    }
    synthInfo.maxMusic = music;
    synthInfo.maxSFX = sfx;
    frq = 32000;
    if ((ret = hwInit(&frq, synthInfo.voice_count, synthInfo.studioCount, flags)) == 0) {
        ret = DoInit(32000, aramSize, synthInfo.voice_count, flags);
    }
    return ret;
}
