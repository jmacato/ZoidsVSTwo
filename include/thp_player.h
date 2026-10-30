/* Layouts follow
 * https://github.com/doldecomp/sms/tree/main/libs/THPPlayer/include/THPPlayer, with the
 * game's earlier video, audio, and read-buffer layouts. */
#ifndef ZOIDS_THP_PLAYER_H
#define ZOIDS_THP_PLAYER_H
#include "types.h"
#include "os_thread.h"
#include "os_message.h"
#include "os_interrupt.h"
#include "dvd_file.h"
#include "os_cache.h"
typedef int BOOL;
typedef long OSPriority;
typedef void *OSMessage;
#define TRUE 1
#define FALSE 0
#define NULL 0
#define OS_MESSAGE_BLOCK 1
#define OS_MESSAGE_NOBLOCK 0
#define THP_AUDIO_BUFFER_COUNT 3
#define THP_READ_BUFFER_COUNT 10
#define THP_TEXTURE_SET_COUNT 3
/* values of THPPlayer.state and internalState */
#define THP_PLAYER_STOP 0
#define THP_PLAYER_PREPARE 1
#define THP_PLAYER_PLAY 2
#define THP_PLAYER_PLAYED 3
#define THP_PLAYER_PAUSE 4
#define THP_PLAYER_ERROR 5
/* THPPlayer.playFlag bits (flag argument of THPPlayerPrepare) */
#define THP_PLAY_LOOP 1
#define THP_PLAY_SYNC_FIELD0 2
#define THP_PLAY_SYNC_FIELD1 4

typedef struct THPHeader {
    char magic[4];
    u32 version;
    u32 bufsize;
    u32 audioMaxSamples;
    f32 frameRate;
    u32 numFrames;
    u32 firstFrameSize;
    u32 movieDataSize;
    u32 compInfoDataOffsets;
    u32 offsetDataOffsets;
    u32 movieDataOffsets;
    u32 finalFrameDataOffsets;
} THPHeader;

typedef struct THPVideoInfo {
    u32 xSize;
    u32 ySize;
} THPVideoInfo;

typedef struct THPAudioInfo {
    u32 sndChannels;
    u32 sndFrequency;
    u32 sndNumSamples;
} THPAudioInfo;

typedef struct THPFrameCompInfo {
    u32 numComponents;
    u8 frameComp[16];
} THPFrameCompInfo;

typedef struct THPTextureSet {
    u8 *ytexture;
    u8 *utexture;
    u8 *vtexture;
    s32 frameNumber;
} THPTextureSet;

typedef struct THPAudioBuffer {
    s16 *buffer;
    s16 *curPtr;
    u32 validSample;
    s32 frameNumber;
} THPAudioBuffer;

typedef struct THPReadBuffer {
    u8 *ptr;
    s32 frameNumber;
} THPReadBuffer;

typedef struct THPPlayer {
    DVDFileInfo fileInfo;
    THPHeader header;
    THPFrameCompInfo compInfo;
    THPVideoInfo videoInfo;
    THPAudioInfo audioInfo;
    void *thpWork;
    BOOL open;
    u8 state;
    u8 internalState;
    u8 playFlag;
    u8 audioExist;
    s32 dvdError;
    s32 videoError;
    BOOL onMemory;
    u8 *movieData;
    s32 initOffset;
    s32 initReadSize;
    s32 initReadFrame;
    u32 curField;
    s64 retaceCount;
    s32 prevCount;
    s32 curCount;
    s32 videoDecodeCount;
    s32 curVideoNumber;
    s32 curAudioNumber;
    THPTextureSet *dispTextureSet;
    THPAudioBuffer *playAudioBuffer;
    THPReadBuffer readBuffer[10];
    THPTextureSet textureSet[THP_TEXTURE_SET_COUNT];
    THPAudioBuffer audioBuffer[THP_AUDIO_BUFFER_COUNT];
} THPPlayer;

extern THPPlayer ActivePlayer;
struct GXRenderModeObj;
void THPGXRestore(void);
void THPGXYuv2RgbSetup(struct GXRenderModeObj *);
void THPGXYuv2RgbDraw(u8 *, u8 *, u8 *, s16, s16, s16, s16, s16, s16);
s32 THPPlayerDrawCurrentFrame(struct GXRenderModeObj *, u32, u32, u32, u32);
u32 THPPlayerAudioOutput(s16 *, u32, s32);
u32 THPAudioDecode(s16 *, u8 *, s32);
s32 THPVideoDecode(u8 *, u8 *, u8 *, u8 *, void *);
void *PopFreeAudioBuffer(void);
void *PopDecodedAudioBuffer(s32);
void PushFreeAudioBuffer(void *);
void PushDecodedAudioBuffer(void *);
void *PopReadedBuffer(void);
void *PopReadedBuffer2(void);
void *PopFreeReadBuffer(void);
void PushReadedBuffer(void *);
void PushReadedBuffer2(void *);
void PushFreeReadBuffer(void *);
void *PopFreeTextureSet(void);
void *PopDecodedTextureSet(s32);
void PushFreeTextureSet(void *);
void PushDecodedTextureSet(void *);
void PrepareReady(int);
s32 sReadPre(void);

#endif
