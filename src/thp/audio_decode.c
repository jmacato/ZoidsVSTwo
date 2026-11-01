/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/THPPlayer/src/THPAudioDecode.c. */
#include "thp_player.h"

#define STACK_SIZE 4096

static OSThread AudioDecodeThread;
static u8 AudioDecodeThreadStack[STACK_SIZE];
static OSMessageQueue FreeAudioBufferQueue;
static OSMessageQueue DecodedAudioBufferQueue;
static OSMessage FreeAudioBufferMessage[THP_AUDIO_BUFFER_COUNT];
static OSMessage DecodedAudioBufferMessage[THP_AUDIO_BUFFER_COUNT];

static BOOL AudioDecodeThreadCreated;

static void *AudioDecoderForOnMemory(void *arg);
static void *AudioDecoder(void *arg);
void AudioDecode(THPReadBuffer *readBuffer);



BOOL CreateAudioDecodeThread(OSPriority priority, void *ptr)
{
    if (ptr) {
        if (!OSCreateThread(&AudioDecodeThread, AudioDecoderForOnMemory, ptr, AudioDecodeThreadStack + STACK_SIZE, STACK_SIZE, priority, 1)) {
            return 0;
        }
    } else {
        if (!OSCreateThread(&AudioDecodeThread, AudioDecoder, 0, AudioDecodeThreadStack + STACK_SIZE, STACK_SIZE, priority, 1)) {
            return 0;
        }
    }
    OSInitMessageQueue(&FreeAudioBufferQueue, FreeAudioBufferMessage, 3);
    OSInitMessageQueue(&DecodedAudioBufferQueue, DecodedAudioBufferMessage, 3);
    AudioDecodeThreadCreated = 1;
    return 1;
}

void AudioDecodeThreadStart()
{
    if (AudioDecodeThreadCreated)
        OSResumeThread(&AudioDecodeThread);
}

void AudioDecodeThreadCancel()
{
    if (AudioDecodeThreadCreated) {
        OSCancelThread(&AudioDecodeThread);
        AudioDecodeThreadCreated = FALSE;
    }
}

static void *AudioDecoder(void *arg)
{
    THPReadBuffer *buf;
    while (TRUE) {
        buf = (THPReadBuffer *)PopReadedBuffer();
        AudioDecode(buf);
        PushReadedBuffer2(buf);
    }
}

static void *AudioDecoderForOnMemory(void *arg)
{
    s32 readSize;
    THPReadBuffer readBuffer;
    s32 size;
    s32 frame;
    s32 remaining;

    frame = 0;
    readSize = ActivePlayer.initReadSize;
    readBuffer.ptr = (u8 *)arg;

    while (TRUE) {
        readBuffer.frameNumber = frame;
        AudioDecode(&readBuffer);

        remaining =
            (frame + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;

        if (remaining == ActivePlayer.header.numFrames - 1) {
            if ((ActivePlayer.playFlag & THP_PLAY_LOOP)) {
                readSize = *(s32 *)readBuffer.ptr;
                readBuffer.ptr = ActivePlayer.movieData;
            } else {
                OSSuspendThread(&AudioDecodeThread);
            }
        } else {
            size = *(s32 *)readBuffer.ptr;
            readBuffer.ptr += readSize;
            readSize = size;
        }
        frame++;
    }
}

#pragma dont_inline on
void AudioDecode(THPReadBuffer *readBuffer);
#pragma dont_inline off

void *PopFreeAudioBuffer()
{
    void *buf;
    OSReceiveMessage(&FreeAudioBufferQueue, &buf, 1);
    return buf;
}

void PushFreeAudioBuffer(void *buf)
{
    OSSendMessage(&FreeAudioBufferQueue, buf, OS_MESSAGE_NOBLOCK);
}



void PushDecodedAudioBuffer(void *buf)
{
    OSSendMessage(&DecodedAudioBufferQueue, buf, 1);
}
