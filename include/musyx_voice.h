#ifndef ZOIDS_MUSYX_VOICE_H
#define ZOIDS_MUSYX_VOICE_H

#include "types.h"
#include "musyx_adsr.h"

typedef struct SYNTH_JOB {
    struct SYNTH_JOB *next;
    struct SYNTH_JOB *prev;
    u8 voice_index;
    u8 queue_index;
    u16 reserved_0A;
} SYNTH_JOB;

typedef char SynthJobSizeCheck[sizeof(SYNTH_JOB) == 12 ? 1 : -1];

typedef struct CTRL_SOURCE { u8 midiCtrl, combine; s32 scale; } CTRL_SOURCE;
typedef struct CTRL_DEST { CTRL_SOURCE source[4]; u16 oldValue; u8 numSource; } CTRL_DEST;

typedef struct SYNTH_LFO {
    u32 time, period;
    s16 value, lastValue;
} SYNTH_LFO;

typedef struct VID_LIST {
    struct VID_LIST *next;
    struct VID_LIST *prev;
    u32 vid;
    u32 root;
} VID_LIST;

/* The original records align 64-bit timestamps and flags to four bytes. */
#pragma pack(push, 4)
typedef struct SYNTH_VOICE {
    SYNTH_JOB jobs[3];
    u64 lastLowCallTime;
    u64 lastZeroCallTime;
    struct MSTEP *macro;
    struct MSTEP *macroStep;
    struct SYNTH_VOICE *macroNext;
    struct SYNTH_VOICE *macroPrev;
    struct SYNTH_VOICE *timeNext;
    struct SYNTH_VOICE *timePrev;
    s32 macroState;
    void *trapMacro[3];
    void *trapStep[3];
    u8 trapEnabled[3];
    u8 reserved06B;
    struct { void *macro, *step; } callStack[4];
    u8 callStackEntryNum, callStackIndex, reserved08E[2];
    u64 macStartTime;
    u64 waitTime;
    u64 macroTime;
    u8 timeUsedByInput;
    u8 reserved0A9;
    u16 loop;
    s32 variable[16];
    unsigned long nextID;
    u32 prevID;
    u32 id;
    VID_LIST *vidList;
    VID_LIST *vidMasterList;
    u16 allocId, macroId;
    u8 keyGroup, reserved105[3];
    u32 lastVID;
    u8 priority;
    u8 reserved10D;
    u16 ageSpeed;
    u32 priorityTime;
    u64 cFlags;
    u8 blocked;
    u8 isFX;
    u8 vGroup;
    u8 studio;
    u8 track;
    u8 midi;
    u8 midiSet;
    u8 section;
    u32 sampleInfo;
    u32 playbackFrequency;
    u16 curNote;
    s8 curDetune;
    u8 orgNote;
    u8 portamento_key;
    u8 portamento_type;
    u16 portLastCtrlState;
    u32 portDuration;
    u32 portCurPitch;
    u32 portTime;
    u8 vibratoKeyRange;
    u8 vibratoCentRange;
    u8 reserved142[2];
    u32 vibratoPeriod;
    u32 vibratoTime;
    s32 vibratoOffset;
    s16 vibratoModScale;
    u8 reserved152[2];
    u32 soundVolume, originalVolume;
    f32 lastVolumeScale;
    u32 lastPan, lastSurroundPan;
    f32 tremoloScale;
    u16 tremoloDepth, tremoloModScale;
    u32 panCurrent[2];
    s32 panDelta[2];
    s32 panTarget[2];
    u32 panTime[2];
    u8 reverbScale, reverbOffset, volumeTable, itdMode;
    s32 envDelta;
    u32 envTarget, envCurrent;
    s32 sweepOff[2], sweepAdd[2], sweepCnt[2];
    u8 sweepNum[2], reserved1BA[2];
    SYNTH_LFO lfo[2];
    u8 lfoUsedByInput[2];
    u8 pbLowerKeyRange, pbUpperKeyRange;
    u16 pbLast;
    u8 reserved1DA[2];
    ADSR_VARS adsr;
    s16 pitchAdsrRange;
    u16 currentPitch;
    u8 setupVolume, setupPan, setupMidi;
    u8 setupMidiSet;
    u8 setupSection, setupTrack, setupVGroup, setupStudio, setupITD;
    u8 reserved211[3];
    u32 midiDirtyFlags;
    CTRL_DEST inpVolume;
    CTRL_DEST inpPanning;
    CTRL_DEST inpSurPanning;
    CTRL_DEST inpPitchBend;
    CTRL_DEST inpDoppler;
    CTRL_DEST inpModulation;
    CTRL_DEST inpPedal;
    CTRL_DEST inpPortamento;
    CTRL_DEST inpPreAuxA;
    CTRL_DEST inpReverb;
    CTRL_DEST inpPreAuxB;
    CTRL_DEST inpPostAuxB;
    CTRL_DEST inpTremolo;
    u8 messageCount, messageRead, messageWrite, reserved3EF;
    s32 messageQueue[4];
    u16 currentOutputVolume;
    u8 reserved402[2];
} SYNTH_VOICE;
#pragma pack(pop)

typedef char SynthVoiceSizeCheck[sizeof(SYNTH_VOICE) == 0x404 ? 1 : -1];

/* SYNTH_VOICE.cFlags bits; 0x1000 and the SelectSource input bits stay literal. */
#define CFLAG_HW_BREAK 0x1ULL
#define CFLAG_STARTING 0x2ULL
#define CFLAG_WAIT_KEYOFF 0x4ULL
#define CFLAG_EXTERNAL_KEYOFF 0x8ULL
#define CFLAG_HW_KEYON 0x10ULL
#define CFLAG_HW_START_PENDING 0x20ULL
#define CFLAG_HW_KEYOFF_PENDING 0x80ULL
#define CFLAG_ADSR_SET 0x100ULL
#define CFLAG_PORTAMENTO 0x400ULL
#define CFLAG_VIBRATO 0x2000ULL
#define CFLAG_VIBRATO_MODWHEEL 0x4000ULL
#define CFLAG_ENVELOPE 0x8000ULL
#define CFLAG_PITCHBEND_AFTER_KEYOFF 0x10000ULL
#define CFLAG_LEGATO 0x20000ULL
#define CFLAG_WAIT_SAMPLE_END 0x40000ULL
#define CFLAG_PEDAL 0x10000000000ULL
#define CFLAG_PITCH_ADSR 0x20000000000ULL
#define CFLAG_DEFERRED_KEYOFF 0x40000000000ULL
#define CFLAG_SRC_SET 0x80000000000ULL
#define CFLAG_VOLUME_UPDATE 0x100000000000ULL
#define CFLAG_PAN_UPDATE 0x200000000000ULL

extern u8 inpTranslateExCtrl(u8 control);
extern u16 inpGetExCtrl(SYNTH_VOICE *voice, u8 control);
extern void inpSetExCtrl(SYNTH_VOICE *voice, u8 control, s16 value);

#endif
