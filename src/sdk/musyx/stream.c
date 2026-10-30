#include "musyx_synth.h"
#include "musyx_hardware.h"

typedef struct STREAM_ADPCM_INFO {
    u16 numCoef;
    u8 initialPS, loopPS;
    s16 loopY0, loopY1;
    s16 coefTab[8][2];
} STREAM_ADPCM_INFO;
typedef struct STREAM_INFO {
    unsigned long id, flags;
    u8 state, type, reserved0A[2];
    unsigned long (*callback)(void *first, unsigned long firstLength, void *second, unsigned long secondLength, unsigned long user);
    s16 *buffer;
    unsigned long size, bytes, last;
    STREAM_ADPCM_INFO adpcmInfo;
    volatile unsigned long voice;
    unsigned long user, frequency;
    u8 priority, volume, pan, surroundPan, auxA, auxB;
    u8 defaultPan, defaultSurroundPan;
    u8 studio, hwStreamHandle, reserved5E[2];
    unsigned long nextStreamHandle;
} STREAM_INFO;
typedef char StreamInfoSizeCheck[sizeof(STREAM_INFO) == 0x64 ? 1 : -1];
typedef char StreamAdpcmInfoSizeCheck[sizeof(STREAM_ADPCM_INFO) == 0x28 ? 1 : -1];
static STREAM_INFO streamInfo[64];
static u8 streamCallCnt;
static u8 streamCallDelay;
static unsigned long nextPublicID;
extern unsigned long synthFlags;
extern void voiceUnblock(unsigned long index);
extern void hwSetVolume(unsigned long index, u8 mode, f32 volume, unsigned long pan, unsigned long surroundPan, f32 auxA, f32 auxB);
void streamCorrectLoops(void)
{
}
void streamInit(void)
{
    s32 index;
    streamCallCnt = 0;
    streamCallDelay = 3;
    for (index = 0; index < synthInfo.voice_count; index++)
        streamInfo[index].state = 0;
    nextPublicID = 0;
}
void streamKill(unsigned long index)
{
    STREAM_INFO *stream = &streamInfo[index];
    switch (stream->state) {
    case 1:
    case 2:
        if (stream->state == 2)
            voiceUnblock(stream->voice);
        stream->state = 3;
        stream->callback(0, 0, 0, 0, stream->user);
        break;
    }
}
void streamOutputModeChanged(void)
{
    unsigned long index;
    hwDisableIrq();
    for (index = 0; index < synthInfo.voice_count; index++) {
        STREAM_INFO *stream = &streamInfo[index];
        if (stream->state != 0) {
            stream->pan = stream->defaultPan;
            stream->surroundPan = stream->defaultSurroundPan;
            if (synthFlags & 1) {
                stream->pan = 64;
                stream->surroundPan = 0;
            } else if (!(synthFlags & 2)) {
                stream->surroundPan = 0;
            }
            if (stream->state != 3)
                hwSetVolume(stream->voice, 0, stream->volume * (1.0f / 127.0f),
                            stream->pan << 16, stream->surroundPan << 16,
                            stream->auxA * (1.0f / 127.0f), stream->auxB * (1.0f / 127.0f));
        }
    }
    hwEnableIrq();
}

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/stream.c (CC0-1.0). */
#include "musyx_sample.h"
#define NULL 0
#define STREAM_STATE_STARTING 1
#define STREAM_STATE_PLAYING 2
#define STREAM_TYPE_PCM16 0
#define STREAM_TYPE_ADPCM 1
#define SND_STREAM_MANUALARAMUPD 0x20000
#define SND_STREAM_ADPCM_BLKSIZE 14
#define SND_STREAM_ADPCM_BLKBYTES 8
extern unsigned long hwGetStreamPlayBuffer(u8 handle);
extern void DCInvalidateRange(void *buffer, unsigned long length);
extern void hwInitSamplePlayback(unsigned long voice, u16 sample, void *info, unsigned long adsr, unsigned long priority, unsigned long callback, unsigned long src, u8 itd);
extern void hwSetPitch(unsigned long voice, u16 pitch);
extern void hwStart(unsigned long voice, u8 studio);
extern void hwFlushStream(void *buffer, unsigned long offset, unsigned long bytes, u8 handle, void (*callback)(unsigned long user), unsigned long user);
extern unsigned long hwGetPos(unsigned long voice);
extern void hwSetStreamLoopPS(unsigned long voice, u8 ps);
/* The SDK uses a macro here. An inline function changes caller stack allocation. */
#define OSCachedToUncached(pointer) ((void *)((u8 *)(pointer) + 0x40000000))
void streamHandle(void) {
  unsigned long i;
  unsigned long cpos;
  unsigned long len;
  SAMPLE_RESULT newsmp;
  STREAM_INFO *si;
  float f;
  unsigned long v;
  if (streamCallCnt != 0) {
    --streamCallCnt;
    return;
  }
  streamCallCnt = streamCallDelay;
  si = &streamInfo[0];
  for (i = 0; i < synthInfo.voice_count; ++i, si++) {
    switch (si->state) {
    case STREAM_STATE_STARTING:
      newsmp.info = si->frequency | 0x40000000;
      newsmp.address = hwGetStreamPlayBuffer(si->hwStreamHandle);
      newsmp.offset = 0;
      newsmp.length = si->size;
      newsmp.loop = 0;
      newsmp.loopLength = si->size;
      si->adpcmInfo.loopPS = si->adpcmInfo.initialPS = *(u8 *)si->buffer;
      DCInvalidateRange(si->buffer, 1);
      switch (si->type) {
      case STREAM_TYPE_PCM16:
        newsmp.compType = SAMPLE_TYPE_PCM16;
        break;
      case STREAM_TYPE_ADPCM:
        newsmp.extra = &si->adpcmInfo;
        newsmp.compType = SAMPLE_TYPE_ADPCM_STREAM;
        break;
      }
      v = si->voice;
      hwInitSamplePlayback(v, -1, &newsmp, 1, -1, synthVoice[v].id, 1, 1);
      f = (si->frequency / (float)synthInfo.mixFrq);
      hwSetPitch(si->voice, f * 4096.f);
      hwSetVolume(si->voice, 0, si->volume * (1 / 127.f), (si->pan << 16), (si->surroundPan << 16),
                  si->auxA * (1 / 127.f), si->auxB * (1 / 127.f));
      hwStart(si->voice, si->studio);
      si->state = STREAM_STATE_PLAYING;
      if (!(si->flags & SND_STREAM_MANUALARAMUPD)) {
        hwFlushStream(si->buffer, 0, si->bytes, si->hwStreamHandle, NULL, 0);
      }
      break;
    case STREAM_STATE_PLAYING: {
      cpos = hwGetPos(si->voice);
      if (si->type == STREAM_TYPE_ADPCM) {
        cpos = (cpos / SND_STREAM_ADPCM_BLKSIZE) * SND_STREAM_ADPCM_BLKSIZE;
      }
      if (si->last != cpos) {
        if (si->last < cpos) {
          switch (si->type) {
          case STREAM_TYPE_PCM16: {
            if ((len = si->callback(si->buffer + si->last, cpos - si->last, NULL, 0,
                                          si->user)) != 0 &&
                si->state == STREAM_STATE_PLAYING) {
              cpos = (si->last + len) % si->size;
              if (!(si->flags & SND_STREAM_MANUALARAMUPD)) {
                if (cpos != 0) {
                  hwFlushStream(si->buffer, si->last * 2, (cpos - si->last) * 2, si->hwStreamHandle,
                                NULL, 0);
                } else {
                  hwFlushStream(si->buffer, si->last * 2, (si->size - si->last) * 2,
                                si->hwStreamHandle, NULL, 0);
                }
              }
              si->last = cpos;
            }
          } break;
          case STREAM_TYPE_ADPCM: {
            unsigned long off = (si->last / SND_STREAM_ADPCM_BLKSIZE) * SND_STREAM_ADPCM_BLKBYTES;
            if ((len = si->callback((void *)((unsigned long)si->buffer + off), cpos - si->last, NULL,
                                          0, si->user)) != 0 &&
                si->state == STREAM_STATE_PLAYING) {
              cpos = (si->last + len) % si->size;
              if (!(si->flags & SND_STREAM_MANUALARAMUPD)) {
                if (cpos != 0) {
                  hwFlushStream(
                      si->buffer, off,
                      ((cpos + 13) / SND_STREAM_ADPCM_BLKSIZE) * SND_STREAM_ADPCM_BLKBYTES - off,
                      si->hwStreamHandle, NULL, 0);
                } else {
                  hwFlushStream(si->buffer, off, (si->bytes) - off, si->hwStreamHandle, NULL, 0);
                }
              }
              si->last = cpos;
            }
          } break;
          }
        } else if (cpos == 0) {
          switch (si->type) {
          case STREAM_TYPE_PCM16:
            if ((len = si->callback(si->buffer + si->last, si->size - si->last, NULL, 0,
                                          si->user)) &&
                si->state == STREAM_STATE_PLAYING) {
              cpos = (si->last + len) % si->size;
              if (!(si->flags & SND_STREAM_MANUALARAMUPD)) {
                if (cpos == 0) {
                  hwFlushStream(si->buffer, si->last * 2, si->bytes - (si->last * 2),
                                si->hwStreamHandle, NULL, 0);
                } else {
                  hwFlushStream(si->buffer, si->last * 2, (cpos - si->last) * 2, si->hwStreamHandle,
                                NULL, 0);
                }
              }
              si->last = cpos;
            }
            break;
          case STREAM_TYPE_ADPCM: {
            unsigned long off = ((si->last / SND_STREAM_ADPCM_BLKSIZE) * SND_STREAM_ADPCM_BLKBYTES);
            if ((len = si->callback((void *)((unsigned long)si->buffer + off), si->size - si->last,
                                          NULL, 0, si->user)) &&
                si->state == STREAM_STATE_PLAYING) {
              cpos = (si->last + len) % si->size;
              if (!(si->flags & SND_STREAM_MANUALARAMUPD)) {
                if (cpos == 0) {
                  hwFlushStream(si->buffer, off, si->bytes - off, si->hwStreamHandle, NULL, 0);
                } else {
                  hwFlushStream(
                      si->buffer, off,
                      ((cpos + 13) / SND_STREAM_ADPCM_BLKSIZE) * SND_STREAM_ADPCM_BLKBYTES - off,
                      si->hwStreamHandle, NULL, 0);
                }
              }
              si->last = cpos;
            }
          } break;
          }
        } else {
          switch (si->type) {
          case STREAM_TYPE_PCM16:
            if ((len = si->callback(si->buffer + si->last, si->size - si->last, si->buffer,
                                          cpos, si->user)) &&
                si->state == STREAM_STATE_PLAYING) {
              cpos = (si->last + len) % si->size;
              if (!(si->flags & SND_STREAM_MANUALARAMUPD)) {
                if (len > si->size - si->last) {
                  hwFlushStream(si->buffer, si->last * 2, (si->bytes - si->last * 2),
                                si->hwStreamHandle, NULL, 0);
                  hwFlushStream(si->buffer, 0, cpos * 2, si->hwStreamHandle, NULL, 0);
                } else if (cpos == 0) {
                  hwFlushStream(si->buffer, si->last * 2, (si->bytes - si->last * 2),
                                si->hwStreamHandle, NULL, 0);
                } else {
                  hwFlushStream(si->buffer, si->last * 2, (cpos - si->last) * 2, si->hwStreamHandle,
                                NULL, 0);
                }
              }
              si->last = cpos;
            }
            break;
          case STREAM_TYPE_ADPCM: {
            unsigned long off = (si->last / SND_STREAM_ADPCM_BLKSIZE) * SND_STREAM_ADPCM_BLKBYTES;
            if ((len = si->callback((void *)((unsigned long)si->buffer + off), si->size - si->last,
                                          si->buffer, cpos, si->user)) &&
                si->state == STREAM_STATE_PLAYING) {
              cpos = (si->last + len) % si->size;
              if (!(si->flags & SND_STREAM_MANUALARAMUPD)) {
                if (len > si->size - si->last) {
                  hwFlushStream(si->buffer, off, si->bytes - off, si->hwStreamHandle, NULL, 0);
                  hwFlushStream(si->buffer, 0, (cpos / SND_STREAM_ADPCM_BLKSIZE) << 3,
                                si->hwStreamHandle, NULL, 0);
                } else if (cpos == 0) {
                  hwFlushStream(si->buffer, off, si->bytes - off, si->hwStreamHandle, NULL, 0);
                } else {
                  hwFlushStream(
                      si->buffer, off,
                      ((cpos + 13) / SND_STREAM_ADPCM_BLKSIZE) * SND_STREAM_ADPCM_BLKBYTES - off,
                      si->hwStreamHandle, NULL, 0);
                }
              }
              si->last = cpos;
            }
          } break;
          }
        }
        if (si->state == STREAM_STATE_PLAYING && !(si->flags & SND_STREAM_MANUALARAMUPD) &&
            si->type == STREAM_TYPE_ADPCM) {
          hwSetStreamLoopPS(si->voice, *(unsigned long *)OSCachedToUncached(si->buffer) >> 24);
        }
      }
    } break;
    }
  }
}
