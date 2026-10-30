#ifndef ZOIDS_AUDIO_H
#define ZOIDS_AUDIO_H

#include "types.h"

typedef void *(*AudioAllocate)(u32 size);
typedef void (*AudioFree)(void *memory);

extern void HSD_AudioSetHooks(AudioAllocate allocate, AudioFree free);
extern void HSD_AudioInit(u8 voices, u8 music, u8 effects, u32 aram_size);
extern u32 HSD_AudioSFXLoadCheck(void);
extern void HSD_AudioSFXPop(void);
extern u32 HSD_AudioSFXStart(u16 effect, u8 volume, u8 pan);
extern u32 HSD_AudioSFXStartEx(u16 effect, u8 volume, u8 pan, u8 studio);
extern void HSD_AudioSFXKeyOff(u32 voice);
extern void HSD_AudioSFXKeyOffAll(void);
extern void HSD_AudioSFXPanning(u32 voice, u8 pan);
extern void HSD_AudioSFXSurroundPanning(u32 voice, u8 pan);
extern void HSD_AudioSFXVolume(u32 voice, u8 volume);
extern u32 HSD_AudioSFXCheck(u32 voice);
extern u32 HSD_AudioSeqPlayCheck(void);
extern u32 HSD_AudioSeqPlayGetSeqId(void);
extern void HSD_AudioSeqStop(u32 sequence);
extern void HSD_AudioSeqVolume(u8 volume, u16 time, u32 sequence, u8 mode);
extern void HSD_AudioSeqFree(u32 sequence);
extern void HSD_AudioStreamStart(const char *name, u32 offset);
extern void HSD_AudioStreamStop(void);
extern void HSD_AudioStreamStopAtEnd(void);
extern void HSD_AudioStreamVolume(s32 volume);
extern void HSD_AudioStreamSetVolume(s32 volume);
extern void HSD_AudioStreamUpdateVolume(void);

#endif
