#include "thp_player.h"

static OSMessageQueue DecodedAudioBufferQueue;

void *PopDecodedAudioBuffer(s32 flags)
{
    void *buf;
    if (OSReceiveMessage(&DecodedAudioBufferQueue, &buf, flags) == 1) {
        return buf;
    }
    return NULL;
}
