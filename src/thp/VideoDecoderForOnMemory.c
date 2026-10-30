#include "thp_player.h"

static OSThread VideoDecodeThread;

extern THPPlayer ActivePlayer;
extern void VideoDecode(THPReadBuffer *readBuffer);

void *VideoDecoderForOnMemory(void *arg)
{
    THPReadBuffer readBuffer;
    s32 readSize;
    s32 frame;
    BOOL enable;
    s32 size;
    s32 remaining;

    readSize = ActivePlayer.initReadSize;
    frame = 0;
    readBuffer.ptr = (u8 *)arg;

    while (TRUE) {
        if (ActivePlayer.audioExist) {
            while (ActivePlayer.videoDecodeCount < 0) {
                enable = OSDisableInterrupts();
                ActivePlayer.videoDecodeCount++;
                OSRestoreInterrupts(enable);
                remaining = (frame + ActivePlayer.initReadFrame) %
                            ActivePlayer.header.numFrames;
                if (remaining == ActivePlayer.header.numFrames - 1) {
                    if ((ActivePlayer.playFlag & THP_PLAY_LOOP) == 0)
                        break;

                    readSize = *(s32 *)readBuffer.ptr;
                    readBuffer.ptr = ActivePlayer.movieData;
                } else {
                    size = *(s32 *)readBuffer.ptr;
                    readBuffer.ptr += readSize;
                    readSize = size;
                }
                frame++;
            }
        }

        readBuffer.frameNumber = frame;

        VideoDecode(&readBuffer);

        remaining =
            (frame + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
        if (remaining == ActivePlayer.header.numFrames - 1) {
            if ((ActivePlayer.playFlag & THP_PLAY_LOOP)) {
                readSize = *(s32 *)readBuffer.ptr;
                readBuffer.ptr = ActivePlayer.movieData;
            } else {
                OSSuspendThread(&VideoDecodeThread);
            }
        } else {
            size = *(s32 *)readBuffer.ptr;
            readBuffer.ptr += readSize;
            readSize = size;
        }

        frame++;
    }
}
