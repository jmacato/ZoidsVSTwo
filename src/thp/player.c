/* Reference: https://github.com/doldecomp/sms/blob/main/libs/THPPlayer/src/THPPlayer.c.
 */
#include "thp_player.h"
#include "initialize.h"
#include "runtime_string.h"
#include "gx_framebuffer.h"
#include "vi.h"
#include "dvd.h"
extern void DCInvalidateRange(void *addr, u32 nBytes);
#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define ALIGN_NEXT(a, n) (((a) + (n) - 1) & ~((n) - 1))
static u32 WorkBuffer[16];
static OSMessageQueue PrepareReadyQueue;
static OSMessageQueue UsedTextureSetQueue;
static OSMessage UsedTextureSetMessage[3];
THPPlayer ActivePlayer;
static BOOL Initialized;
static void *PrepareReadyMessage;
static void (*OldVIPostCallback)(u32);
extern int THPInit(void);
s32 THPPlayerDrawCurrentFrame(GXRenderModeObj *rmode, u32 x, u32 y, u32 polygonW,
                              u32 polygonH)
{
    s32 frame;
    if (ActivePlayer.open && ActivePlayer.state != THP_PLAYER_STOP &&
        ActivePlayer.dispTextureSet != NULL) {
        THPGXYuv2RgbSetup(rmode);
        THPGXYuv2RgbDraw(ActivePlayer.dispTextureSet->ytexture,
                         ActivePlayer.dispTextureSet->utexture,
                         ActivePlayer.dispTextureSet->vtexture, x, y,
                         ActivePlayer.videoInfo.xSize, ActivePlayer.videoInfo.ySize,
                         polygonW, polygonH);
        THPGXRestore();
        frame =
            (ActivePlayer.dispTextureSet->frameNumber + ActivePlayer.initReadFrame) %
            ActivePlayer.header.numFrames;
        return frame;
    }
    return -1;
}

static void *PopUsedTextureSet(void);
static void PushUsedTextureSet(OSMessage);
BOOL THPPlayerInit(void)
{
    memset(&ActivePlayer, 0, sizeof(THPPlayer));
    LCEnable();
    OSInitMessageQueue(&UsedTextureSetQueue, UsedTextureSetMessage, 3);
    if (THPInit()) {
        Initialized = TRUE;
        return TRUE;
    }
    return FALSE;
}
void THPPlayerQuit(void)
{
    LCDisable();
    Initialized = FALSE;
}

u32 THPPlayerCalcNeedMemory(void)
{
    u32 size;
    if (ActivePlayer.open) {
        if (ActivePlayer.onMemory)
            size = ALIGN_NEXT(ActivePlayer.header.movieDataSize, 32);
        else
            size = ALIGN_NEXT(ActivePlayer.header.bufsize, 32) * 10;
        size += ALIGN_NEXT(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize, 32) * 3;
        size += ALIGN_NEXT(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize / 4, 32) * 3;
        size += ALIGN_NEXT(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize / 4, 32) * 3;
        if (ActivePlayer.audioExist)
            size += ALIGN_NEXT(ActivePlayer.header.audioMaxSamples * 4, 32) * 3;
        size += 0x1000;
        return size;
    }
    return 0;
}

BOOL THPPlayerSetBuffer(u8 *buffer)
{
    unsigned long i;
    u8 *ptr;
    u32 ysize;
    u32 uvsize;

    if (ActivePlayer.open && ActivePlayer.state == THP_PLAYER_STOP) {
        ptr = buffer;
        if (ActivePlayer.onMemory) {
            ActivePlayer.movieData = ptr;
            ptr += ActivePlayer.header.movieDataSize;
        } else {
            for (i = 0; i < ARRAY_COUNT(ActivePlayer.readBuffer); i++) {
                ActivePlayer.readBuffer[i].ptr = ptr;
                ptr += ALIGN_NEXT(ActivePlayer.header.bufsize, 32);
            }
        }

        ysize =
            ALIGN_NEXT(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize, 32);
        uvsize = ALIGN_NEXT(
            ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize / 4, 32);

        for (i = 0; i < ARRAY_COUNT(ActivePlayer.textureSet); i++) {
            ActivePlayer.textureSet[i].ytexture = ptr;

            DCInvalidateRange(ptr, ysize);
            ptr += ysize;

            ActivePlayer.textureSet[i].utexture = ptr;
            DCInvalidateRange(ptr, uvsize);
            ptr += uvsize;

            ActivePlayer.textureSet[i].vtexture = ptr;
            DCInvalidateRange(ptr, uvsize);
            ptr += uvsize;
        }

        if (ActivePlayer.audioExist) {
            for (i = 0; i < ARRAY_COUNT(ActivePlayer.audioBuffer); i++) {
                ActivePlayer.audioBuffer[i].buffer = (s16 *)ptr;
                ActivePlayer.audioBuffer[i].curPtr = (s16 *)ptr;
                ActivePlayer.audioBuffer[i].validSample = 0;
                ptr += ALIGN_NEXT(ActivePlayer.header.audioMaxSamples * 4, 32);
            }
        }

        ActivePlayer.thpWork = ptr;
        return TRUE;
    }

    return FALSE;
}

static void InitAllMessageQueue(void)
{
    s32 i;
    THPReadBuffer *readBuffer;
    THPTextureSet *textureSet;
    THPAudioBuffer *audioBuffer;

    if (!ActivePlayer.onMemory) {
        for (i = 0; i < 10; i++) {
            readBuffer = &ActivePlayer.readBuffer[i];
            PushFreeReadBuffer(readBuffer);
        }
    }
    for (i = 0; i < 3; i++) {
        textureSet = &ActivePlayer.textureSet[i];
        PushFreeTextureSet(textureSet);
    }
    if (ActivePlayer.audioExist) {
        for (i = 0; i < 3; i++) {
            audioBuffer = &ActivePlayer.audioBuffer[i];
            PushFreeAudioBuffer(audioBuffer);
        }
    }
    OSInitMessageQueue(&PrepareReadyQueue, &PrepareReadyMessage, 1);
}

static BOOL WaitUntilPrepare()
{
    OSMessage msg;

    OSReceiveMessage(&PrepareReadyQueue, &msg, OS_MESSAGE_BLOCK);

    if ((BOOL)(s32)msg) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void PrepareReady(BOOL msg)
{
    OSSendMessage(&PrepareReadyQueue, (OSMessage)(s32)msg, OS_MESSAGE_BLOCK);
}

BOOL THPPlayerGetVideoInfo(THPVideoInfo *videoInfo)
{
    if (ActivePlayer.open) {
        memcpy(videoInfo, &ActivePlayer.videoInfo, sizeof(THPVideoInfo));
        return TRUE;
    }

    return FALSE;
}

s32 THPPlayerGetState()
{
    return ActivePlayer.state;
}


static void PushUsedTextureSet(OSMessage msg)
{
    OSSendMessage(&UsedTextureSetQueue, msg, OS_MESSAGE_NOBLOCK);
}

static OSMessage PopUsedTextureSet()
{
    OSMessage msg;
    if (OSReceiveMessage(&UsedTextureSetQueue, &msg, OS_MESSAGE_NOBLOCK) == 1)
        return msg;

    return NULL;
}

extern void ReadThreadCancel(void);
extern void VideoDecodeThreadCancel(void);
extern void AudioDecodeThreadCancel(void);
static BOOL ProperTimingForStart(void);
static BOOL ProperTimingForGettingNextFrame(void);
BOOL THPPlayerOpen(const char *fileName, BOOL onMemory)
{
    s32 offset;
    s32 i;

    if (Initialized == FALSE) {
        return FALSE;
    }

    if (ActivePlayer.open) {
        return FALSE;
    }

    memset(&ActivePlayer.videoInfo, 0, sizeof(THPVideoInfo));
    memset(&ActivePlayer.audioInfo, 0, sizeof(THPAudioInfo));

    if (DVDOpen((char *)fileName, &ActivePlayer.fileInfo) == FALSE) {
        return FALSE;
    }

    if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 64, 0, 2) < 0) {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    memcpy(&ActivePlayer.header.magic, WorkBuffer, sizeof(THPHeader));

    if (strcmp(ActivePlayer.header.magic, "THP") != 0) {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    if (ActivePlayer.header.version != 0x10000) {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    offset = ActivePlayer.header.compInfoDataOffsets;

    if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 32, offset, 2) < 0) {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    memcpy(&ActivePlayer.compInfo, WorkBuffer, sizeof(THPFrameCompInfo));
    offset += sizeof(THPFrameCompInfo);
    ActivePlayer.audioExist = 0;

    for (i = 0; i < ActivePlayer.compInfo.numComponents; i++) {
        switch (ActivePlayer.compInfo.frameComp[i]) {
        case 0: {
            if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 32, offset, 2) < 0) {
                DVDClose(&ActivePlayer.fileInfo);
                return FALSE;
            }

            memcpy(&ActivePlayer.videoInfo, WorkBuffer, sizeof(THPVideoInfo));

            offset += sizeof(THPVideoInfo);
            break;
        }
        case 1: {
            if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 32, offset, 2) < 0) {
                DVDClose(&ActivePlayer.fileInfo);
                return FALSE;
            }
            memcpy(&ActivePlayer.audioInfo, WorkBuffer, sizeof(THPAudioInfo));
            offset += sizeof(THPAudioInfo);
            ActivePlayer.audioExist = 1;
            break;
        }

        default:
            return FALSE;
        }
    }

    ActivePlayer.state = ActivePlayer.internalState = THP_PLAYER_STOP;
    ActivePlayer.playFlag = 0;
    ActivePlayer.onMemory = onMemory;
    ActivePlayer.open = TRUE;

    return TRUE;
}

BOOL THPPlayerClose()
{
    if (ActivePlayer.open && ActivePlayer.state == THP_PLAYER_STOP) {
        ActivePlayer.open = FALSE;
        DVDClose(&ActivePlayer.fileInfo);
        return TRUE;
    }

    return FALSE;
}

BOOL THPPlayerPlay()
{
    if (ActivePlayer.open && (ActivePlayer.state == THP_PLAYER_PREPARE || ActivePlayer.state == THP_PLAYER_PAUSE)) {
        ActivePlayer.state = THP_PLAYER_PLAY;
        ActivePlayer.prevCount = 0;
        ActivePlayer.curCount = 0;
        ActivePlayer.retaceCount = -1;
        return TRUE;
    }
    return FALSE;
}

#pragma dont_inline on
void THPPlayerStop()
{
    register void *texture;

    if (ActivePlayer.open && ActivePlayer.state != THP_PLAYER_STOP) {
        ActivePlayer.state = ActivePlayer.internalState = THP_PLAYER_STOP;
        VISetPostRetraceCallback(OldVIPostCallback);
        if (ActivePlayer.onMemory == 0) {
            DVDCancel(&ActivePlayer.fileInfo.cb);
            ReadThreadCancel();
        }

        VideoDecodeThreadCancel();
        if (ActivePlayer.audioExist) {
            AudioDecodeThreadCancel();
        }

        while ((texture = PopUsedTextureSet()) != (void *)0) {
        }

        ActivePlayer.dvdError = 0;
        ActivePlayer.videoError = 0;
    }
}

#pragma dont_inline reset
static BOOL ProperTimingForStart()
{
    if (ActivePlayer.playFlag & THP_PLAY_SYNC_FIELD0) {
        if (VIGetNextField() == 0)
            return TRUE;
    } else if (ActivePlayer.playFlag & THP_PLAY_SYNC_FIELD1) {
        if (VIGetNextField() == 1)
            return TRUE;
    } else
        return TRUE;

    return FALSE;
}

static BOOL ProperTimingForGettingNextFrame()
{
    if ((ActivePlayer.playFlag & THP_PLAY_SYNC_FIELD0)) {
        if (VIGetNextField() == 0) {
            return TRUE;
        }
    } else if ((ActivePlayer.playFlag & THP_PLAY_SYNC_FIELD1)) {
        if (VIGetNextField() == 1) {
            return TRUE;
        }
    } else {
        s32 frameRate = ActivePlayer.header.frameRate * 100.0f;
        ActivePlayer.curCount = ActivePlayer.retaceCount * frameRate / 5994;

        if (ActivePlayer.prevCount != ActivePlayer.curCount) {
            ActivePlayer.prevCount = ActivePlayer.curCount;
            return TRUE;
        }
    }

    return FALSE;
}

#pragma dont_inline on
static void PlayControl(u32 retraceCnt)
{
    THPTextureSet *decodedTexture;
    s32 difference;
    s32 frame;
    if (OldVIPostCallback != NULL)
        OldVIPostCallback(retraceCnt);
    decodedTexture = (THPTextureSet *)-1;
    if (ActivePlayer.open && ActivePlayer.state == THP_PLAYER_PLAY) {
        if (ActivePlayer.dvdError || ActivePlayer.videoError) {
            ActivePlayer.state = ActivePlayer.internalState = THP_PLAYER_ERROR;
            return;
        }
        if (ActivePlayer.retaceCount == 0 && ActivePlayer.internalState == THP_PLAYER_STOP)
            ActivePlayer.internalState = THP_PLAYER_PLAY;
        ++ActivePlayer.retaceCount;
        if (ActivePlayer.internalState == THP_PLAYER_STOP || ActivePlayer.internalState == THP_PLAYER_PAUSE) {
            if (ProperTimingForStart()) {
                if (ActivePlayer.audioExist) {
                    difference =
                        ActivePlayer.curVideoNumber - ActivePlayer.curAudioNumber;
                    if (difference <= 1) {
                        decodedTexture = (THPTextureSet *)PopDecodedTextureSet(0);
                        if (ActivePlayer.videoDecodeCount > difference)
                            ActivePlayer.videoDecodeCount--;
                    } else
                        ActivePlayer.internalState = THP_PLAYER_PLAY;
                } else {
                    decodedTexture = (THPTextureSet *)PopDecodedTextureSet(0);
                    ActivePlayer.internalState = THP_PLAYER_PLAY;
                }
            } else
                ActivePlayer.retaceCount = -1;
        } else {
            if (ProperTimingForGettingNextFrame()) {
                if (ActivePlayer.audioExist) {
                    difference =
                        ActivePlayer.curVideoNumber - ActivePlayer.curAudioNumber;
                    if (difference <= 1) {
                        decodedTexture = (THPTextureSet *)PopDecodedTextureSet(0);
                        if (ActivePlayer.videoDecodeCount > difference)
                            ActivePlayer.videoDecodeCount--;
                    }
                } else
                    decodedTexture = (THPTextureSet *)PopDecodedTextureSet(0);
            }
        }
        if (decodedTexture != NULL && decodedTexture != (THPTextureSet *)-1) {
            ActivePlayer.curVideoNumber = decodedTexture->frameNumber;
            if (ActivePlayer.dispTextureSet != NULL)
                PushUsedTextureSet(ActivePlayer.dispTextureSet);
            ActivePlayer.dispTextureSet = decodedTexture;
        }
        if ((ActivePlayer.playFlag & THP_PLAY_LOOP) == 0) {
            if (ActivePlayer.audioExist) {
                frame = (ActivePlayer.curAudioNumber + ActivePlayer.initReadFrame) %
                        ActivePlayer.header.numFrames;
                if (frame == ActivePlayer.header.numFrames - 1 &&
                    ActivePlayer.playAudioBuffer == (void *)0) {
                    frame = (ActivePlayer.curVideoNumber + ActivePlayer.initReadFrame) %
                            ActivePlayer.header.numFrames;
                    if (frame == ActivePlayer.header.numFrames - 1 &&
                        decodedTexture == (void *)0) {
                        ActivePlayer.state = ActivePlayer.internalState = THP_PLAYER_PLAYED;
                    }
                }
            } else {
                frame = (ActivePlayer.curVideoNumber + ActivePlayer.initReadFrame) %
                        ActivePlayer.header.numFrames;
                if (frame == ActivePlayer.header.numFrames - 1 &&
                    decodedTexture == (void *)0) {
                    ActivePlayer.state = ActivePlayer.internalState = THP_PLAYER_PLAYED;
                }
            }
        }
    }
}

extern void InitAllMessageQueue(void);
extern BOOL CreateReadThread(OSPriority);
extern BOOL CreateVideoDecodeThread(OSPriority, void *);
extern BOOL CreateAudioDecodeThread(OSPriority, void *);
extern void ReadThreadStart(void);
extern void VideoDecodeThreadStart(void);
extern void AudioDecodeThreadStart(void);

#pragma dont_inline on
BOOL THPPlayerPrepare(s32 frame, s32 flag)
{
    u8 *threadData;
    u32 read_offset;
    if (ActivePlayer.open && ActivePlayer.state == THP_PLAYER_STOP) {
        if (frame > 0) {
            if (ActivePlayer.header.offsetDataOffsets == 0)
                return FALSE;

            if (ActivePlayer.header.numFrames > frame) {
                read_offset = ActivePlayer.header.offsetDataOffsets + (frame - 1) * 4;
                if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 0x20, read_offset,
                                2) < 0)
                    return FALSE;

                ActivePlayer.initOffset =
                    ActivePlayer.header.movieDataOffsets + WorkBuffer[0];
                ActivePlayer.initReadFrame = frame;
                ActivePlayer.initReadSize = WorkBuffer[1] - WorkBuffer[0];
            } else {
                return FALSE;
            }
        } else {
            ActivePlayer.initOffset = ActivePlayer.header.movieDataOffsets;
            ActivePlayer.initReadSize = ActivePlayer.header.firstFrameSize;
            ActivePlayer.initReadFrame = frame;
        }

        ActivePlayer.playFlag = flag;
        ActivePlayer.videoDecodeCount = 0;

        if (ActivePlayer.onMemory) {
            if (DVDReadPrio(&ActivePlayer.fileInfo, ActivePlayer.movieData,
                            ActivePlayer.header.movieDataSize,
                            ActivePlayer.header.movieDataOffsets, 2) < 0) {
                return FALSE;
            }

            threadData = ActivePlayer.movieData + ActivePlayer.initOffset -
                         ActivePlayer.header.movieDataOffsets;
            CreateVideoDecodeThread(20, threadData);

            if (ActivePlayer.audioExist)
                CreateAudioDecodeThread(12, threadData);
        } else {
            CreateVideoDecodeThread(20, 0);
            if (ActivePlayer.audioExist)
                CreateAudioDecodeThread(12, NULL);
            CreateReadThread(8);
        }

        InitAllMessageQueue();
        VideoDecodeThreadStart();

        if (ActivePlayer.audioExist)
            AudioDecodeThreadStart();
        if (ActivePlayer.onMemory == 0)
            ReadThreadStart();

        if (!WaitUntilPrepare())
            return FALSE;

        ActivePlayer.state = THP_PLAYER_PREPARE;
        ActivePlayer.internalState = THP_PLAYER_STOP;
        ActivePlayer.dispTextureSet = (THPTextureSet *)NULL;
        ActivePlayer.playAudioBuffer = (THPAudioBuffer *)NULL;
        ActivePlayer.curVideoNumber = 0;
        ActivePlayer.curAudioNumber = 0;

        OldVIPostCallback = VISetPostRetraceCallback(PlayControl);

        return TRUE;
    }

    return FALSE;
}

static u16 VolumeTable[128] = {
    0,     2,     8,     18,    32,    50,    73,    99,    130,   164,   203,   245,
    292,   343,   398,   457,   520,   587,   658,   733,   812,   895,   983,   1074,
    1170,  1269,  1373,  1481,  1592,  1708,  1828,  1952,  2080,  2212,  2348,  2488,
    2632,  2781,  2933,  3090,  3250,  3415,  3583,  3756,  3933,  4114,  4298,  4487,
    4680,  4877,  5079,  5284,  5493,  5706,  5924,  6145,  6371,  6600,  6834,  7072,
    7313,  7559,  7809,  8063,  8321,  8583,  8849,  9119,  9394,  9672,  9954,  10241,
    10531, 10826, 11125, 11427, 11734, 12045, 12360, 12679, 13002, 13329, 13660, 13995,
    14335, 14678, 15025, 15377, 15732, 16092, 16456, 16823, 17195, 17571, 17951, 18335,
    18723, 19115, 19511, 19911, 20316, 20724, 21136, 21553, 21974, 22398, 22827, 23260,
    23696, 24137, 24582, 25031, 25484, 25941, 26402, 26868, 27337, 27810, 28288, 28769,
    29255, 29744, 30238, 30736, 31238, 31744, 32254, 32768};

u32 THPPlayerAudioOutput(s16 *destination, u32 sample, s32 volume)
{
    s32 mix;
    s16 *dest;
    s16 *src;
    u32 i;
    u32 n;
    u16 vol;
    u32 remain;
    u32 total;
    if (volume < 0)
        volume = 0;
    if (volume > 127)
        volume = 127;
    vol = VolumeTable[volume];
    if (ActivePlayer.open && ActivePlayer.internalState == 2 && ActivePlayer.audioExist) {
        remain = sample;
        total = 0;
        dest = destination;
        for (;;) {
            if (ActivePlayer.playAudioBuffer == (void *)0) {
                if ((ActivePlayer.playAudioBuffer = PopDecodedAudioBuffer(0)) == (void *)0)
                    break;
                ActivePlayer.curAudioNumber = ActivePlayer.playAudioBuffer->frameNumber;
            }
            if (ActivePlayer.playAudioBuffer->validSample == 0)
                continue;
            if (ActivePlayer.playAudioBuffer->validSample >= remain)
                n = remain;
            else
                n = ActivePlayer.playAudioBuffer->validSample;
            src = ActivePlayer.playAudioBuffer->curPtr;
            for (i = 0; i < n * 2; i++) {
                mix = *dest + ((vol * *src) >> 15);
                if (mix < -32767)
                    mix = -32767;
                if (mix > 0x8000)
                    mix = 0x8000;
                *dest = mix;
                dest++;
                src++;
            }
            total += n;
            remain -= n;
            ActivePlayer.playAudioBuffer->validSample -= n;
            ActivePlayer.playAudioBuffer->curPtr = src;
            if (ActivePlayer.playAudioBuffer->validSample == 0) {
                PushFreeAudioBuffer(ActivePlayer.playAudioBuffer);
                ActivePlayer.playAudioBuffer = PopDecodedAudioBuffer(0);
                if (ActivePlayer.playAudioBuffer)
                    ActivePlayer.curAudioNumber = ActivePlayer.playAudioBuffer->frameNumber;
            }
            if (remain == 0)
                break;
        }
        return total;
    }
    return 0;
}
