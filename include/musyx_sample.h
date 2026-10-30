#ifndef ZOIDS_MUSYX_SAMPLE_H
#define ZOIDS_MUSYX_SAMPLE_H
#include "types.h"

#define SAMPLE_TYPE_ADPCM 0
#define SAMPLE_TYPE_ADPCM_PLUS 1
#define SAMPLE_TYPE_PCM16 2
#define SAMPLE_TYPE_PCM8 3
#define SAMPLE_TYPE_ADPCM_STREAM 4
#define SAMPLE_TYPE_ADPCM_VIRTUAL 5
#define SAMPLE_TYPE_PCM16_VIRTUAL 6

typedef struct SAMPLE_INFO {
    u32 info;
    u32 length; /* bits 0-23 sample count, bits 24-31 compType */
    u32 loopOffset;
    u32 loopLength;
    u32 extraOffset;
} SAMPLE_INFO;

typedef struct SAMPLE_RESULT {
    u32 info;
    u32 address;
    void *extra;
    u32 offset, length, loop, loopLength;
    u8 compType;
} SAMPLE_RESULT;

extern void hwSaveSample(SAMPLE_INFO **info, u32 *address);
extern void hwRemoveSample(SAMPLE_INFO *info, u32 address);
#endif
