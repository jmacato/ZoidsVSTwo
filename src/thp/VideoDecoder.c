#include "thp_player.h"
extern void VideoDecode(THPReadBuffer *readBuffer);

void *VideoDecoder(void *arg)
{
    THPReadBuffer *thpBuffer;
    BOOL enable;
    s32 remaining;

    while (TRUE) {
        if (ActivePlayer.audioExist) {
            for (; ActivePlayer.videoDecodeCount < 0;) {
                thpBuffer = (THPReadBuffer *)PopReadedBuffer2();
                remaining = ((thpBuffer->frameNumber + ActivePlayer.initReadFrame) %
                             ActivePlayer.header.numFrames);
                if (remaining == (ActivePlayer.header.numFrames - 1) &&
                    (ActivePlayer.playFlag & THP_PLAY_LOOP) == 0)
                    VideoDecode(thpBuffer);

                PushFreeReadBuffer(thpBuffer);
                enable = OSDisableInterrupts();
                ActivePlayer.videoDecodeCount++;
                OSRestoreInterrupts(enable);
            }
        }

        if (ActivePlayer.audioExist)
            thpBuffer = (THPReadBuffer *)PopReadedBuffer2();
        else
            thpBuffer = (THPReadBuffer *)PopReadedBuffer();

        VideoDecode(thpBuffer);
        PushFreeReadBuffer(thpBuffer);
    }
}
