#ifndef ZOIDS_MUSYX_ADSR_H
#define ZOIDS_MUSYX_ADSR_H
#include "types.h"

#define ADSR_MODE_LINEAR 0
#define ADSR_MODE_DLS 1

#define ADSR_STATE_START 0
#define ADSR_STATE_ATTACK 1
#define ADSR_STATE_DECAY 2
#define ADSR_STATE_SUSTAIN 3
#define ADSR_STATE_RELEASE 4

typedef struct ADSR_INFO {
    union {
        struct { s32 atime, dtime; u16 slevel, rtime; s32 ascale, dscale; } dls;
        struct { u16 atime, dtime, slevel, rtime; } linear;
    } data;
} ADSR_INFO;

typedef struct ADSR_VARS {
    u8 mode;
    u8 state;
    u16 reserved02;
    u32 count;
    s32 volume;
    s32 index;
    s32 delta;
    u32 attackTime;
    u32 decayTime;
    u16 sustain;
    u16 reserved1E;
    u32 releaseTime;
    u16 cutOff;
    u8 attackMode;
    u8 reserved27;
} ADSR_VARS;
typedef char AdsrVarsSizeCheck[sizeof(ADSR_VARS) == 0x28 ? 1 : -1];
extern u32 adsrRelease(ADSR_VARS *adsr);
#endif
