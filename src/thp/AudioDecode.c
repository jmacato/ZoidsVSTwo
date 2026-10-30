#include "thp_player.h"

extern THPPlayer ActivePlayer;
extern void *PopFreeAudioBuffer();
extern void PushDecodedAudioBuffer(void *buf);

static void AudioDecode(THPReadBuffer *readBuffer)
{
    THPAudioBuffer *audioBuf;
    s32 i;
    u32 *offsets;
    u8 *audioData;
    u32 valid;

    offsets = (u32 *)(readBuffer->ptr + 8);
    audioData = &readBuffer->ptr[ActivePlayer.compInfo.numComponents * 4] + 8;
    audioBuf = (THPAudioBuffer *)PopFreeAudioBuffer();

    for (i = 0; i < ActivePlayer.compInfo.numComponents; i++) {
        switch (ActivePlayer.compInfo.frameComp[i]) {
        case 1: {
            valid = THPAudioDecode(audioBuf->buffer, audioData, 0);
            audioBuf->validSample = valid;
            audioBuf->curPtr = audioBuf->buffer;
            audioBuf->frameNumber = readBuffer->frameNumber;
            PushDecodedAudioBuffer(audioBuf);
            break;
        }
        }

        audioData += *offsets;
        offsets++;
    }
}
