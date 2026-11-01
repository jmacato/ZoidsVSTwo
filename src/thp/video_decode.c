/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/THPPlayer/src/THPVideoDecode.c. */
#include "thp_player.h"

#define STACK_SIZE 4096

static OSThread VideoDecodeThread;
static u8 VideoDecodeThreadStack[STACK_SIZE];
static OSMessageQueue FreeTextureSetQueue;
static OSMessageQueue DecodedTextureSetQueue;
static OSMessage FreeTextureSetMessage[THP_AUDIO_BUFFER_COUNT];
static OSMessage DecodedTextureSetMessage[THP_AUDIO_BUFFER_COUNT];

static BOOL VideoDecodeThreadCreated;
static BOOL First;

static void *VideoDecoderForOnMemory(void *arg);
static void *VideoDecoder(void *arg);
void VideoDecode(THPReadBuffer *readBuffer);



BOOL CreateVideoDecodeThread(OSPriority priority, void *ptr)
{
    if (ptr) {
        if (!OSCreateThread(&VideoDecodeThread, VideoDecoderForOnMemory, ptr, VideoDecodeThreadStack + STACK_SIZE, STACK_SIZE, priority, 1)) {
            return 0;
        }
    } else {
        if (!OSCreateThread(&VideoDecodeThread, VideoDecoder, 0, VideoDecodeThreadStack + STACK_SIZE, STACK_SIZE, priority, 1)) {
            return 0;
        }
    }
    OSInitMessageQueue(&FreeTextureSetQueue, FreeTextureSetMessage, 3);
    OSInitMessageQueue(&DecodedTextureSetQueue, DecodedTextureSetMessage, 3);
    VideoDecodeThreadCreated = 1;
    First = 1;
    return 1;
}

void VideoDecodeThreadStart(void)
{
    if (VideoDecodeThreadCreated)
        OSResumeThread(&VideoDecodeThread);
}

void VideoDecodeThreadCancel(void)
{
    if (VideoDecodeThreadCreated) {
        OSCancelThread(&VideoDecodeThread);
        VideoDecodeThreadCreated = FALSE;
    }
}

void *VideoDecoder(void *arg);

void *VideoDecoderForOnMemory(void *arg);

void VideoDecode(THPReadBuffer *readBuffer)
{
    THPTextureSet *textureSet;
    BOOL enable;
    s32 i;
    u32 *tileOffsets;
    u8 *tile;

    tileOffsets = (u32 *)(readBuffer->ptr + 8);
    tile = &readBuffer->ptr[ActivePlayer.compInfo.numComponents * 4] + 8;
    textureSet = (THPTextureSet *)PopFreeTextureSet();

    for (i = 0; i < ActivePlayer.compInfo.numComponents; i++) {
        switch (ActivePlayer.compInfo.frameComp[i]) {
        case 0: {
            if ((ActivePlayer.videoError =
                     THPVideoDecode(tile, textureSet->ytexture, textureSet->utexture,
                                    textureSet->vtexture, ActivePlayer.thpWork))) {
                if (First) {
                    PrepareReady(FALSE);
                    First = FALSE;
                }
                OSSuspendThread(&VideoDecodeThread);
            }
            textureSet->frameNumber = readBuffer->frameNumber;
            PushDecodedTextureSet(textureSet);
            enable = OSDisableInterrupts();
            ActivePlayer.videoDecodeCount++;
            OSRestoreInterrupts(enable);
        }
        }

        tile += *tileOffsets;
        tileOffsets++;
    }

    if (First) {
        PrepareReady(TRUE);
        First = FALSE;
    }
}

void *PopFreeTextureSet(void)
{
    void *tex;
    OSReceiveMessage(&FreeTextureSetQueue, &tex, 1);
    return tex;
}

void PushFreeTextureSet(void *tex)
{
    OSSendMessage(&FreeTextureSetQueue, tex, 0);
}

void *PopDecodedTextureSet(s32 flags)
{
    void *tex;
    if (OSReceiveMessage(&DecodedTextureSetQueue, &tex, flags) == TRUE) {
        return tex;
    }
    return NULL;
}

void PushDecodedTextureSet(void *tex)
{
    OSSendMessage(&DecodedTextureSetQueue, tex, OS_MESSAGE_BLOCK);
}
