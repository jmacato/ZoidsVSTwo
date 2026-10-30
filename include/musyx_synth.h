#ifndef ZOIDS_MUSYX_SYNTH_H
#define ZOIDS_MUSYX_SYNTH_H

#include "musyx_voice.h"

typedef struct SYNTH_MASTER_FADER {
    f32 volume, target, start, time, deltaTime;
    f32 pauseVol, pauseTarget, pauseStart, pauseTime, pauseDeltaTime;
    u32 sequence;
    u8 action, type, reserved_2E[2];
} SYNTH_MASTER_FADER;

typedef struct SND_PLAYBACKINFO {
    u32 frq;
    u8 stereo;
    u8 bits;
    s8 deviceName[256];
    s8 versionText[256];
} SND_PLAYBACKINFO;

typedef struct SYNTH_INFO {
    u32 mixFrq;
    u32 numSamples;
    SND_PLAYBACKINFO pbInfo;
    u8 voice_count;
    u8 maxMusic, maxSFX, studioCount;
} SYNTH_INFO;

typedef char SynthMasterFaderSizeCheck[sizeof(SYNTH_MASTER_FADER) == 0x30 ? 1 : -1];
typedef char SynthInfoSizeCheck[sizeof(SYNTH_INFO) == 0x214 ? 1 : -1];

extern SYNTH_VOICE *synthVoice;
extern SYNTH_INFO synthInfo;
extern SYNTH_MASTER_FADER synthMasterFader[32];
extern u64 synthRealTime;

extern void synthAddJob(SYNTH_VOICE *voice, s32 type, u32 delta);

#endif
