#include "ai.h"
#include "thp_player.h"

static s16 SoundBuffer[2][480];
static s32 SoundBufferNumber;
AIDCallback oldaudiocallback;
static void AudioCallback(void);
extern void *memset(void *, s32, u32);

void AudioInit(void)
{
    AIInit(NULL);
    oldaudiocallback = AIRegisterDMACallback(AudioCallback);
    memset(SoundBuffer[SoundBufferNumber], 0, sizeof(SoundBuffer[0]));
    DCFlushRange(SoundBuffer[SoundBufferNumber], sizeof(SoundBuffer[0]));
    AIInitDMA((u32)SoundBuffer[SoundBufferNumber], sizeof(SoundBuffer[0]));
    AIStartDMA();
}

void THP_AudioEnd(void)
{
    AIRegisterDMACallback(oldaudiocallback);
}

void AudioCallback(void);
