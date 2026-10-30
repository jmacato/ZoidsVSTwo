#include "types.h"
#include "musyx_midictrl.h"

typedef struct NOTE {
    struct NOTE *next;
    struct NOTE *prev;
    u32 id;
    s32 endTime;
    u8 section;
    u8 timeIndex;
    u8 reserved12[2];
} NOTE;
typedef struct SEQ_CROSSFADE {
    u32 seqId1;
    u16 time1;
    u32 seqId2;
    u16 time2;
    void *arr2;
    u16 gid2, sid2;
    u8 volume, studio2;
    u32 trackMute[2];
    u16 speed;
    u8 flags;
} SEQ_CROSSFADE;
typedef struct SND_SEQVOLDEF { u8 track, volGroup; } SND_SEQVOLDEF;
typedef struct SND_PLAYPARA {
    u32 flags, trackMute[2];
    u16 speed;
    struct { u16 time; u8 target; } volume;
    u8 numSeqVolDef;
    SND_SEQVOLDEF *seqVolDef;
    u8 numFaded;
    u8 *faded;
} SND_PLAYPARA;
typedef struct MIDI_CHANNEL_SETUP { u8 program, volume, panning, reverb, chorus; } MIDI_CHANNEL_SETUP;
typedef struct MIDISETUP { u16 songId, reserved; MIDI_CHANNEL_SETUP channel[16]; } MIDISETUP;
typedef struct SEQ_TICKS {
    u32 low;
    s32 high;
} SEQ_TICKS;
typedef struct PAGE { u16 macro; u8 prio, maxVoices, index, reserved; } PAGE;
typedef struct PRG_STATE { u16 macId; u8 priority, maxVoices; } PRG_STATE;
typedef struct SEQ_PATTERN { u32 headerLen, pitchBend, modulation, noteData; } SEQ_PATTERN;
typedef struct ARR {
    u32 trackTable, patternTable, trackMidiTable, masterTrack, info;
    u32 loopPoint[16];
    u32 trackSectionTable;
} ARR;
typedef struct TENTRY {
    u32 time;
    u8 prgChange, velocity, reserved[2];
    u16 pattern;
    s8 transpose, velocityAdd;
} TENTRY;
typedef struct TRACK { TENTRY *base, *addr; } TRACK;
typedef struct NOTE_DATA { u16 time; u8 key, velocity; u16 length; } NOTE_DATA;
typedef struct SEQ_STREAM {
    u8 *nextAddr;
    u16 value;
    s16 nextDelta;
    u32 nextTime;
} SEQ_STREAM;
typedef struct CPAT {
    u32 lTime, baseTime;
    NOTE_DATA *addr;
    TENTRY *patternInfo;
    SEQ_STREAM pitchBend, modulation;
    u8 midi;
} CPAT;
typedef struct SEQ_EVENT {
    struct SEQ_EVENT *next, *prev;
    u32 time;
    union {
        TENTRY *trackAddr;
        struct { NOTE_DATA *addr; CPAT *base; } pattern;
    } info;
    u8 type, trackId;
} SEQ_EVENT;
typedef struct MTRACK_DATA { volatile u32 time; u32 bpm; } MTRACK_DATA;
typedef struct SEQ_SECTION {
    MTRACK_DATA *masterBase;
    MTRACK_DATA *masterAddress;
    u32 bpm;
    SEQ_TICKS tickDelta[2];
    SEQ_EVENT *eventRoot;
    SEQ_TICKS time[2];
    u8 timeIndex;
    u8 reserved31;
    u16 speed;
    u16 loopCount;
    u8 loopDisable;
    u8 reserved37;
} SEQ_SECTION;
typedef struct SEQ_INSTANCE {
    struct SEQ_INSTANCE *next;
    struct SEQ_INSTANCE *prev;
    u8 state;
    u8 index;
    u16 groupID;
    u32 publicId;
    PAGE *normtab;
    u8 normTrans[128];
    PAGE *drumtab;
    u8 drumTrans[128];
    ARR *arrangement;
    u32 trackMute[2];
    TRACK track[64];
    u8 trackVolGroup[64];
    CPAT pattern[64];
    NOTE *noteUsed[2];
    NOTE *noteKeyOff;
    PRG_STATE prgState[16];
    u8 defVGroup;
    u8 reservedEB1[3];
    SEQ_CROSSFADE syncCrossInfo;
    u32 *syncSeqIdPtr;
    u8 syncActive;
    u8 defaultStudio;
    u8 keyOffCheck;
    u8 reservedEE3;
    SEQ_EVENT events[64];
    u8 *trackSectionTable;
    SEQ_SECTION section[16];
} SEQ_INSTANCE;
typedef char NoteSizeCheck[sizeof(NOTE) == 20 ? 1 : -1];
typedef char SeqInstanceSizeCheck[sizeof(SEQ_INSTANCE) == 0x1868 ? 1 : -1];
#pragma force_active on
static NOTE seqNote[256];
SEQ_INSTANCE seqInstance[8];
u16 seqMIDIPriority[8][16];
static SEQ_INSTANCE *cseq;
static NOTE *noteFree;
static u32 curSeqId;
static u8 curFadeOutState;
static u32 seq_next_id;
SEQ_INSTANCE *seqFreeRoot = 0;
SEQ_INSTANCE *seqPausedRoot = 0;
SEQ_INSTANCE *seqActiveRoot = 0;
#pragma force_active reset
u32 sndSeqPlayEx(u16 group, u16 song, void *arrangement, SND_PLAYPARA *para, u8 studio);
extern u32 seqPlaySong(u16 group, u16 song, void *arrangement, SND_PLAYPARA *para, u8 irq, u8 studio);
extern void synthVolume(u8 volume, u16 time, u8 group, u8 mode, u32 sequence);

void seqFreeKeyOffNote(NOTE *note)
{
    if (note->next != 0)
        note->next->prev = note->prev;
    if (note->prev != 0)
        note->prev->next = note->next;
    else
        cseq->noteKeyOff = note->next;
    if ((note->next = noteFree) != 0)
        noteFree->prev = note;
    note->prev = 0;
    noteFree = note;
}






/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/seq.c (CC0-1.0). */
static inline void ClearNotes(void)
{
    NOTE *previous = 0;
    s32 index;
    noteFree = &seqNote[0];
    for (index = 0; index < 256; index++) {
        seqNote[index].prev = previous;
        if (previous)
            previous->next = &seqNote[index];
        previous = &seqNote[index];
    }
    previous->next = 0;
}
static void ResetNotes(SEQ_INSTANCE *instance);
u32 seqGetPrivateId(u32 sequence)
{
    SEQ_INSTANCE *instance;
    for (instance = seqActiveRoot; instance; instance = instance->next) {
        if (instance->publicId == (sequence & 0x7FFFFFFF))
            return instance->index | (sequence & 0x80000000);
    }
    for (instance = seqPausedRoot; instance; instance = instance->next) {
        if (instance->publicId == (sequence & 0x7FFFFFFF))
            return instance->index | (sequence & 0x80000000);
    }
    return 0xFFFFFFFF;
}
void seqVolume(u8 volume, u16 time, u32 sequence, u8 mode)
{
    unsigned long index;
    u32 publicId;
    publicId = sequence;
    sequence = seqGetPrivateId(sequence);
    if (sequence == 0xFFFFFFFF)
        return;
    if (!(sequence & 0x80000000)) {
        synthVolume(volume, time, seqInstance[sequence].defVGroup, mode, publicId);
        for (index = 0; index < 64; index++) {
            if (seqInstance[sequence].trackVolGroup[index] != seqInstance[sequence].defVGroup)
                synthVolume(volume, time, seqInstance[sequence].trackVolGroup[index], 0, 0xFFFFFFFF);
        }
    } else {
        sequence &= 0x7FFFFFFF;
        switch (mode & 15) {
        case 0:
            seqInstance[sequence].syncCrossInfo.volume = volume;
            break;
        case 1:
            seqInstance[sequence].syncSeqIdPtr = 0;
            break;
        case 2:
            seqInstance[sequence].syncCrossInfo.flags |= 8;
            seqInstance[sequence].syncCrossInfo.volume = volume;
            break;
        case 3:
            seqInstance[sequence].syncCrossInfo.flags |= 0x80;
            seqInstance[sequence].syncCrossInfo.volume = volume;
            break;
        }
    }
}
void seqInit(void)
{
    unsigned long index;
    unsigned long channel;
    seqActiveRoot = 0;
    seqPausedRoot = 0;
    for (index = 0; index < 8; index++) {
        if (index == 0) {
            seqFreeRoot = &seqInstance[index];
            seqInstance[index].prev = 0;
        } else {
            seqInstance[index - 1].next = &seqInstance[index];
            seqInstance[index].prev = &seqInstance[index - 1];
        }
        seqInstance[index].index = index;
        seqInstance[index].state = 0;
        for (channel = 0; channel < 16; channel++)
            seqMIDIPriority[index][channel] = 0xFFFF;
    }
    seqInstance[index - 1].next = 0;
    ClearNotes();
    seq_next_id = 0;
}


extern u32 synthSendKeyOff(u32 id);
extern u32 voiceKillSound(u32 id);
static inline void KillNotes(SEQ_INSTANCE *instance)
{
    NOTE *note;
    unsigned long index;
    for (index = 0; index < 2; index++) {
        for (note = instance->noteUsed[index]; note; note = note->next)
            voiceKillSound(note->id);
    }
    for (note = instance->noteKeyOff; note; note = note->next)
        voiceKillSound(note->id);
}
#pragma force_active on
static NOTE *AllocateNote(u32 endTime, u8 section)
{
    NOTE *note;
    NOTE *current;
    NOTE *previous;
    if ((note = noteFree) != 0) {
        if ((noteFree = note->next) != 0)
            noteFree->prev = 0;
        note->endTime = endTime;
        note->section = section;
        note->timeIndex = cseq->section[section].timeIndex;
        previous = 0;
        for (current = cseq->noteUsed[note->timeIndex]; current; previous = current, current = current->next) {
            if (current->endTime > note->endTime) {
                note->next = current;
                note->prev = previous;
                if (previous)
                    previous->next = note;
                else
                    cseq->noteUsed[note->timeIndex] = note;
                current->prev = note;
                return note;
            }
        }
        note->prev = previous;
        if (previous)
            previous->next = note;
        else
            cseq->noteUsed[note->timeIndex] = note;
        note->next = 0;
    }
    return note;
}
static u32 HandleNotes(void)
{
    NOTE *note;
    unsigned long index;
    for (index = 0; index < 2; index++) {
        if ((note = cseq->noteUsed[index]) != 0) {
            while (note->endTime <= cseq->section[note->section].time[index].high) {
                synthSendKeyOff(note->id);
                if ((cseq->noteUsed[index] = note->next) != 0)
                    cseq->noteUsed[index]->prev = 0;
                if ((note->next = cseq->noteKeyOff) != 0)
                    cseq->noteKeyOff->prev = note;
                cseq->noteKeyOff = note;
                if ((note = cseq->noteUsed[index]) == 0)
                    break;
            }
        }
    }
    return cseq->noteUsed[0] != 0 || cseq->noteUsed[1] != 0;
}
static void KeyOffNotes(void)
{
    NOTE *note;
    NOTE *next;
    unsigned long index;
    for (index = 0; index < 2; index++) {
        note = cseq->noteUsed[index];
        while (note) {
            next = note->next;
            synthSendKeyOff(note->id);
            if ((cseq->noteUsed[index] = note->next) != 0)
                cseq->noteUsed[index]->prev = 0;
            if ((note->next = cseq->noteKeyOff) != 0)
                cseq->noteKeyOff->prev = note;
            cseq->noteKeyOff = note;
            note = next;
        }
    }
}
static u32 GetPublicId(u32 sequence)
{
    u32 id;
    SEQ_INSTANCE *instance;
    do {
        id = seq_next_id++;
        seq_next_id &= 0x7FFFFFFF;
        for (instance = seqActiveRoot; instance; instance = instance->next) {
            if (instance->publicId == id) {
                id = 0xFFFFFFFF;
                break;
            }
        }
        for (instance = seqPausedRoot; instance; instance = instance->next) {
            if (instance->publicId == id) {
                id = 0xFFFFFFFF;
                break;
            }
        }
    } while (id == 0xFFFFFFFF);
    seqInstance[sequence].publicId = id;
    return id;
}
#pragma force_active reset
#define TRUE 1
#define FALSE 0
#define SND_ID_ERROR 0xFFFFFFFF
#define ARR_GET(arr, offset) ((void *)((offset) + (u32)(arr)))
#define ARR_GET_TYPE(arr, offset, type) ((type)ARR_GET(arr, offset))
#define CLAMP(value, low, high) ((value) > (high) ? (high) : ((value) < (low) ? (low) : (value)))
#define SEQ_STATE_PLAYING 1
#define SEQ_STATE_FREE 0
#define SND_SEQ_ERROR_ID 0xFFFFFFFF
#define SND_SEQ_CROSSFADE_ID 0x80000000
#define SND_PAUSEVOL_NORMAL 127
#define SND_PLAYPARA_TRACKMUTE 1
#define SND_PLAYPARA_SPEED 2
#define SND_PLAYPARA_VOLUME 4
#define SND_PLAYPARA_SEQVOLDEF 8
#define SND_PLAYPARA_PAUSE 16
#define SND_SEQVOL_CONTINUE 0
#define SND_SEQVOL_STOP 1
#define SND_SEQVOL_PAUSE 2
#define SND_SEQVOL_MUTE 3
#define SND_CROSSFADE_PAUSE 1
#define SND_CROSSFADE_CONTINUE 2
#define SND_CROSSFADE_SYNC 4
#define SND_CROSSFADE_PAUSENEW 8
#define SND_CROSSFADE_TRACKMUTE 16
#define SND_CROSSFADE_SPEED 32
#define SND_CROSSFADE_MUTE 64
#define SND_CROSSFADE_MUTENEW 128
extern void synthSetMusicVolumeType(u8 group, u8 type);
extern void inpResetChannelDefaults(u8 midi, u8 set);
extern u8 synthTrackVolume[64];
extern u32 synthIsFadeOutActive(u8 group);
extern u32 sndFXCheck(u32 voice);
extern void sndSeqVolume(u8 volume, u16 time, u32 sequence, u8 mode);
extern void sndSeqContinue(u32 sequence);
extern void sndSeqMute(u32 sequence, u32 mask1, u32 mask2);
extern void sndSeqSpeed(u32 sequence, u16 speed);





extern void synthSetBpm(u32 bpm, u8 set, u8 section);
extern void inpResetMidiCtrl(u8 midi, u8 set, u32 coldReset);
void StartPause(SEQ_INSTANCE *instance);
static void InitTrackEvents(void);
static inline void BuildTransTab(u8 *tab, PAGE *page) {
  u8 i;

  for (i = 0; i < 128; ++i) {
    tab[i] = 0xff;
  }

  for (i = 0; page->index != 0xFF; ++i, page++) {
    tab[page->index] = i;
  }
}
static inline void DoPrgChange(SEQ_INSTANCE *seq, u8 prg, u8 midi) {
  seqMIDIPriority[curSeqId][midi] = 0xFFFF;
  if (midi != 9) {
    prg = seq->normTrans[prg];
    if (prg != 0xFF) {
      seq->prgState[midi].macId = seq->normtab[prg].macro;
      seq->prgState[midi].priority = seq->normtab[prg].prio;
      seq->prgState[midi].maxVoices = seq->normtab[prg].maxVoices;
    }
  } else {
    prg = seq->drumTrans[prg];
    if (prg != 0xFF) {
    seq->prgState[midi].macId = seq->drumtab[prg].macro;
    seq->prgState[midi].priority = seq->drumtab[prg].prio;
    seq->prgState[midi].maxVoices = seq->drumtab[prg].maxVoices;
    }
  }
}
u32 seqStartPlay(PAGE *norm, PAGE *drum, MIDISETUP *midiSetup, u32 *song, SND_PLAYPARA *para,
                 u8 studio, u16 sgid) {
  ARR *arr;
  u32 *tracktab;
  long i;
  SEQ_INSTANCE *nseq;
  SEQ_INSTANCE *oldCSeq;
  u32 seqId;
  u32 bpm;

  if ((nseq = seqFreeRoot) == 0) {
    return SND_ID_ERROR;
  }
  if ((seqFreeRoot = nseq->next) != 0) {
    seqFreeRoot->prev = 0;
  }
  if ((nseq->next = seqActiveRoot) != 0) {
    seqActiveRoot->prev = nseq;
  }

  nseq->prev = 0;
  seqActiveRoot = nseq;
  nseq->state = SEQ_STATE_PLAYING;
  for (i = 0; i < 16; ++i) {
    nseq->section[i].eventRoot = 0;
  }

  seqId = nseq->index;
  nseq->syncActive = FALSE;
  nseq->normtab = norm;
  nseq->drumtab = drum;
  nseq->arrangement = (ARR *)song;
  nseq->groupID = sgid;
  BuildTransTab(nseq->normTrans, nseq->normtab);
  BuildTransTab(nseq->drumTrans, nseq->drumtab);
  nseq->defVGroup = seqId + 23;
  for (i = 0; i < 64; ++i) {
    nseq->trackVolGroup[i] = nseq->defVGroup;
  }

  nseq->defaultStudio = studio;
  if (para == 0) {
    nseq->trackMute[0] = -1;
    nseq->trackMute[1] = -1;
    for (i = 0; i < 16; ++i) {
      nseq->section[i].speed = 256;
    }

    synthVolume(SND_PAUSEVOL_NORMAL, 0, nseq->defVGroup, 0, SND_ID_ERROR);
  } else {
    if (para->flags & SND_PLAYPARA_TRACKMUTE) {
      nseq->trackMute[0] = para->trackMute[0];
      nseq->trackMute[1] = para->trackMute[1];
    } else {
      nseq->trackMute[0] = -1;
      nseq->trackMute[1] = -1;
    }

    if (para->flags & SND_PLAYPARA_SPEED) {
      for (i = 0; i < 16; ++i) {
        nseq->section[i].speed = para->speed;
      }
    } else {
      for (i = 0; i < 16; ++i) {
        nseq->section[i].speed = 256;
      }
    }

    if (para->flags & SND_PLAYPARA_SEQVOLDEF) {
      for (i = 0; i < para->numSeqVolDef; ++i) {
        nseq->trackVolGroup[para->seqVolDef[i].track] = para->seqVolDef[i].volGroup;
        synthSetMusicVolumeType(para->seqVolDef[i].volGroup, 0);
      }
    }

    if (para->flags & SND_PLAYPARA_VOLUME) {
      synthVolume(para->volume.target, para->volume.time, nseq->defVGroup, SND_SEQVOL_CONTINUE,
                  SND_SEQ_ERROR_ID);

      for (i = 0; i < para->numFaded; ++i) {
        synthVolume(para->volume.target, para->volume.time, para->faded[i], SND_SEQVOL_CONTINUE,
                    SND_SEQ_ERROR_ID);
      }
    }
  }

  arr = (ARR *)song;
  if (arr->info & 0x80000000) {
    nseq->trackSectionTable = ARR_GET(arr, arr->trackSectionTable);
  } else {
    nseq->trackSectionTable = 0;
  }

  bpm = arr->info & 0x0fffffff;

  if (!(arr->info & 0x40000000)) {
    bpm <<= 10;
  }

  for (i = 0; i < 16; ++i) {
    nseq->section[i].bpm = bpm;
    synthSetBpm(bpm >> 10, seqId, i);

    if (arr->masterTrack != 0) {
      nseq->section[i].masterBase = ARR_GET(arr, arr->masterTrack);
      nseq->section[i].masterAddress = nseq->section[i].masterBase;
    } else {
      nseq->section[i].masterBase = 0;
    }

    nseq->section[i].loopDisable = FALSE;
    nseq->section[i].loopCount = 0;
  }

  tracktab = ARR_GET(arr, arr->trackTable);
  for (i = 0; i < 64; ++i) {
    synthTrackVolume[i] = SND_PAUSEVOL_NORMAL;
    nseq->pattern[i].addr = 0;
    if (tracktab[i] != 0) {
      nseq->track[i].addr = nseq->track[i].base = ARR_GET(arr, tracktab[i]);
    } else {
      nseq->track[i].addr = nseq->track[i].base = 0;
    }
  }

  nseq->noteUsed[0] = 0;
  nseq->noteUsed[1] = 0;
  nseq->noteKeyOff = 0;

  for (i = 0; i < 16; ++i) {
    inpResetMidiCtrl(i, seqId, 1);
  }
  for (i = 0; i < 16; ++i) {
    nseq->prgState[i].macId = 0xffff;
  }
  for (i = 0; i < 16; ++i) {
    inpResetChannelDefaults(i, seqId);
  }
  if (midiSetup != 0) {
    for (i = 0; i < 16; ++i) {
      DoPrgChange(nseq, midiSetup->channel[i].program, i);
      inpSetMidiCtrl(SND_MIDICTRL_VOLUME, i, seqId, midiSetup->channel[i].volume);
      inpSetMidiCtrl(SND_MIDICTRL_PANNING, i, seqId, midiSetup->channel[i].panning);
      inpSetMidiCtrl(SND_MIDICTRL_REVERB, i, seqId, midiSetup->channel[i].reverb);
      inpSetMidiCtrl(SND_MIDICTRL_CHORUS, i, seqId, midiSetup->channel[i].chorus);
    }
  }
  for (i = 0; i < 16; ++i) {
    seqMIDIPriority[seqId][i] = 0xffff;
  }
  for (i = 0; i < 16; ++i) {
    nseq->section[i].time[0].high = 0;
    nseq->section[i].time[0].low = 0;
    nseq->section[i].time[1].high = 0;
    nseq->section[i].time[1].low = 0;
    nseq->section[i].timeIndex = 0;
  }
  nseq->keyOffCheck = 0;

  if (para != 0 && (para->flags & SND_PLAYPARA_PAUSE) != 0) {
    StartPause(nseq);
  }

  oldCSeq = cseq;
  cseq = nseq;
  InitTrackEvents();
  cseq = oldCSeq;
  seqId = GetPublicId(seqId);
  return seqId;
}
void StartPause(SEQ_INSTANCE *instance)
{
    if (instance->prev != 0)
        instance->prev->next = instance->next;
    else
        seqActiveRoot = instance->next;
    if (instance->next != 0)
        instance->next->prev = instance->prev;
    if ((instance->next = seqPausedRoot) != 0)
        seqPausedRoot->prev = instance;
    instance->prev = 0;
    seqPausedRoot = instance;
    instance->state = 2;
}

void seqPause(u32 sequence)
{
    SEQ_INSTANCE *instance;
    sequence = seqGetPrivateId(sequence);
    if (sequence == 0xFFFFFFFF)
        return;
    if (!(sequence & 0x80000000)) {
        instance = &seqInstance[sequence];
        if (instance->state == 1) {
            StartPause(instance);
            KillNotes(instance);
            ResetNotes(instance);
        }
    } else {
        instance = &seqInstance[sequence & 0x7FFFFFFF];
        if (instance->state != 0)
            instance->syncCrossInfo.flags |= 8;
    }
}
void seqStop(unsigned long sequence)
{
    SEQ_INSTANCE *instance;
    if ((sequence = seqGetPrivateId(sequence)) == 0xFFFFFFFF)
        return;
    if (!(sequence & 0x80000000)) {
        instance = &seqInstance[sequence];
        switch (instance->state) {
        case 1:
            if (instance->prev)
                instance->prev->next = instance->next;
            else
                seqActiveRoot = instance->next;
            KillNotes(&seqInstance[sequence]);
            ResetNotes(&seqInstance[sequence]);
            break;
        case 2:
            if (instance->prev)
                instance->prev->next = instance->next;
            else
                seqPausedRoot = instance->next;
            break;
        }
        if (instance->next)
            instance->next->prev = instance->prev;
        instance->state = 0;
        if (seqFreeRoot)
            seqFreeRoot->prev = instance;
        instance->next = seqFreeRoot;
        instance->prev = 0;
        seqFreeRoot = instance;
    } else {
        instance = &seqInstance[sequence & 0x7FFFFFFF];
        if (instance->state != 0)
            instance->syncSeqIdPtr = 0;
    }
}
void seqContinue(u32 sequence)
{
    SEQ_INSTANCE *instance;
    sequence = seqGetPrivateId(sequence);
    if (!(sequence & 0x80000000)) {
        instance = &seqInstance[sequence];
        if (instance->state == 2) {
            if (instance->prev)
                instance->prev->next = instance->next;
            else
                seqPausedRoot = instance->next;
            if (instance->next)
                instance->next->prev = instance->prev;
            if ((instance->next = seqActiveRoot) != 0)
                seqActiveRoot->prev = instance;
            instance->prev = 0;
            seqActiveRoot = instance;
            instance->state = 1;
        }
    } else {
        seqInstance[sequence & 0x7FFFFFFF].syncCrossInfo.flags &= ~8;
    }
}
void seqMute(u32 sequence, u32 mask1, u32 mask2)
{
    sequence = seqGetPrivateId(sequence);
    if (sequence == 0xFFFFFFFF)
        return;
    if (!(sequence & 0x80000000)) {
        seqInstance[sequence].trackMute[0] = mask1;
        seqInstance[sequence].trackMute[1] = mask2;
    } else {
        sequence &= 0x7FFFFFFF;
        seqInstance[sequence].syncCrossInfo.flags |= 0x10;
        seqInstance[sequence].syncCrossInfo.trackMute[0] = mask1;
        seqInstance[sequence].syncCrossInfo.trackMute[1] = mask2;
    }
}
void seqKillInstancesByGroupID(u16 group)
{
    SEQ_INSTANCE *instance;
    for (instance = seqActiveRoot; instance; instance = instance->next) {
        if (instance->groupID == group)
            seqStop(instance->publicId);
    }
    for (instance = seqPausedRoot; instance; instance = instance->next) {
        if (instance->groupID == group)
            seqStop(instance->publicId);
    }
}
void seqSpeed(u32 sequence, u16 speed)
{
    unsigned long index;
    sequence = seqGetPrivateId(sequence);
    if (!(sequence & 0x80000000)) {
        for (index = 0; index < 16; index++)
            seqInstance[sequence].section[index].speed = speed;
    } else {
        sequence &= 0x7FFFFFFF;
        seqInstance[sequence].syncCrossInfo.flags |= 0x20;
        seqInstance[sequence].syncCrossInfo.speed = speed;
    }
}
#pragma force_active on
static void ResetNotes(SEQ_INSTANCE *instance)
{
    NOTE *note;
    unsigned long index;
    for (index = 0; index < 2; index++) {
        if ((note = instance->noteUsed[index]) != 0) {
            for (; note->next; note = note->next)
                ;
            if (noteFree) {
                note->next = noteFree;
                noteFree->prev = note;
            }
            noteFree = instance->noteUsed[index];
            instance->noteUsed[index] = 0;
        }
    }
    if ((note = instance->noteKeyOff) != 0) {
        for (; note->next; note = note->next)
            ;
        if (noteFree) {
            note->next = noteFree;
            noteFree->prev = note;
        }
        noteFree = instance->noteKeyOff;
        instance->noteKeyOff = 0;
    }
}
#pragma force_active reset

static SEQ_EVENT *GenerateNextTrackEvent(u8 trackId);
static void InsertGlobalEvent(SEQ_SECTION *section, SEQ_EVENT *event);
#pragma force_active on
static u8 *GetStreamValue(u8 *stream, u16 *deltaTime, s16 *deltaData) {
  u8 b1;
  u8 b2;
  s16 v;

  b1 = stream[0];
  b2 = stream[1];
  if (b1 == 0x80 && b2 == 0) {
    return 0;
  }

  if ((b1 & 0x80) != 0) {
    *deltaTime = ((b1 & 0x7f) << 8) | b2;
    stream += 2;
  } else {
    *deltaTime = b1;
    stream += 1;
  }

  b1 = stream[0];
  b2 = stream[1];
  if ((b1 & 0x80) != 0) {
    v = ((b1 & 0x7f) << 8) | b2;
    v <<= 1;
    v >>= 1;
    *deltaData = v;
    stream += 2;
  } else {
    v = b1;
    v <<= 9;
    v >>= 9;
    *deltaData = v;
    stream += 1;
  }

  return stream;
}

static void InitTrackEvents() {
  u32 i;
  SEQ_EVENT *ev;

  if (cseq->trackSectionTable == 0) {
    for (i = 0; i < 0x40; i += 1) {
      if ((ev = GenerateNextTrackEvent(i)) != 0) {
        InsertGlobalEvent(cseq->section, ev);
      }
    }
  } else {
    for (i = 0; i < 0x40; i += 1) {
      if ((ev = GenerateNextTrackEvent(i)) != 0) {
        InsertGlobalEvent(cseq->section + cseq->trackSectionTable[i], ev);
      }
    }
  }
}

static void InitTrackEventsSection(u8 secIndex) {
  u32 i;
  SEQ_EVENT *ev;

  if (cseq->trackSectionTable == 0) {
    for (i = 0; i < 64; i += 1) {
      if ((ev = GenerateNextTrackEvent(i)) != 0) {
        InsertGlobalEvent(cseq->section, ev);
      }
    }
  } else {
    for (i = 0; i < 64; i += 1) {
      if ((secIndex == cseq->trackSectionTable[i]) && (ev = GenerateNextTrackEvent(i)) != 0) {
        InsertGlobalEvent(cseq->section + secIndex, ev);
      }
    }
  }
}

static SEQ_EVENT *GenerateNextTrackEvent(u8 trackId) {
  TRACK *track;
  CPAT *pattern;
  SEQ_EVENT *ev;
  u32 patternTime;
  u32 pitchTime;
  u32 modTime;

  track = &cseq->track[trackId];
  pattern = &cseq->pattern[trackId];

  if (track->addr != 0) {
    ev = &cseq->events[trackId];
    ev->trackId = trackId;
    ev->info.pattern.base = pattern;

    if (pattern->addr == 0) {
    null_pattern_addr:
      if (track->addr->pattern == 0xffff) {
        track->addr = 0;
        return 0;
      }

      if (track->addr->pattern == 0xfffe) {
        if (cseq->trackSectionTable == 0) {
          if (cseq->section[0].loopDisable) {
            track->addr = 0;
            return 0;
          }
        } else if (cseq->section[cseq->trackSectionTable[trackId]].loopDisable) {
          track->addr = 0;
          return 0;
        }

        ev->type = 3;
        ev->time = track->addr->time;

        track->addr = &track->base[*((u16 *)&track->addr->transpose)];
        return ev;
      }

      ev->type = 4;
      ev->time = track->addr->time;
      ev->info.trackAddr = track->addr;
      ++track->addr;
      return ev;
    }

    pitchTime = pattern->pitchBend.nextTime;
    modTime = pattern->modulation.nextTime;

  loop:
    patternTime = pattern->addr->time + pattern->lTime;
    if (patternTime >= pitchTime) {
      goto use_pitch_time;
    }
    if (patternTime >= modTime) {
      goto use_mod_time;
    }
    if (pattern->addr->key == 0xff && pattern->addr->velocity == 0xff) {
      pattern->addr = 0;
      goto null_pattern_addr;
    }

    ev->info.trackAddr = (TENTRY *)pattern->addr;
    pattern->lTime = patternTime;

    if ((pattern->addr->key & 0x80) != 0) {
      pattern->addr = (NOTE_DATA *)((u8 *)pattern->addr + 4);
      goto use_pattern_time;
    }
    if ((pattern->addr->key | pattern->addr->velocity) == 0) {
      pattern->addr = (NOTE_DATA *)((u8 *)pattern->addr + 4);
      goto loop;
    }
    ++pattern->addr;

  use_pattern_time:
    ev->type = 0;
    ev->time = patternTime + pattern->baseTime;
    goto end;

  use_pitch_time:
    if (pitchTime < modTime) {
      ev->time = pitchTime + pattern->baseTime;
      ev->type = 2;
      goto end;
    }

  use_mod_time:
    ev->time = modTime + pattern->baseTime;
    ev->type = 1;

  end:
    return ev;
  }

  return 0;
}

static void InsertGlobalEvent(SEQ_SECTION *section, SEQ_EVENT *event) {
  SEQ_EVENT *el;
  SEQ_EVENT *last_el;

  last_el = 0;
  el = section->eventRoot;
  for (; el != 0; last_el = el, el = el->next) {
    if (el->time > event->time) {
      event->next = el;
      event->prev = last_el;
      if (last_el != 0) {
        last_el->next = event;
      } else {
        section->eventRoot = event;
      }
      el->prev = event;
      return;
    }
  }

  event->prev = last_el;
  if (last_el != 0) {
    last_el->next = event;
  } else {
    section->eventRoot = event;
  }
  event->next = 0;
}

static void HandleMasterTrack(u8 secIndex) {
  SEQ_SECTION *section;

  section = &cseq->section[secIndex];
  if (section->masterBase != 0) {
    while (section->masterAddress->time != -1) {
      if (section->masterAddress->time > section->time[section->timeIndex].high) {
        break;
      }

      if ((cseq->arrangement->info & 0x40000000) != 0) {
        synthSetBpm((section->bpm = section->masterAddress->bpm) >> 10, curSeqId, secIndex);
      } else {
        synthSetBpm(section->masterAddress->bpm, curSeqId, secIndex);
        section->bpm = section->masterAddress->bpm << 10;
      }

      ++section->masterAddress;
    }
  }
}
#pragma force_active reset

extern void seqCrossFade(SEQ_CROSSFADE *info, u32 *sequence, u8 irq);
extern u32 synthStartSound(u16 id, u8 priority, u8 maxVoices, u8 key, u8 volume, u8 pan, u8 midi, u8 set, u8 section, u16 step, u16 track, u8 group, s16 priorityOffset, u8 studio, u32 itd);
extern u8 synthITDDefault[8][2];
/* Reference: https://github.com/doldecomp/melee/blob/master/src/MSL/math.h. */
extern float __fabsf(float value);
static inline float fmodf(float numerator, float denominator)
{
    s64 quotient;
    if (__fabsf(denominator) > __fabsf(numerator))
        return numerator;
    quotient = numerator / denominator;
    return numerator - denominator * quotient;
}
extern float floorf(float value);

#pragma force_active on
static inline void FreeNote(struct NOTE *n) {
  if (n->next != 0) {
    n->next->prev = n->prev;
  }

  if (n->prev != 0) {
    n->prev->next = n->next;
  } else {
    cseq->noteUsed[n->timeIndex] = n->next;
  }

  if ((n->next = noteFree) != 0) {
    noteFree->prev = n;
  }

  n->prev = 0;
  noteFree = n;
}



static inline void InitStream(SEQ_STREAM *stream, u32 streamDataOffset) {
  u16 delta;
  if (streamDataOffset != 0) {
    if ((stream->nextAddr = GetStreamValue(ARR_GET(cseq->arrangement, streamDataOffset), &delta,
                                           &stream->nextDelta)) != 0) {
      stream->nextTime = delta;
    } else {
      stream->nextTime = 0x7fffffff;
    }
  } else {
    stream->nextTime = 0x7fffffff;
  }
}

static inline u16 HandleStream(SEQ_STREAM *stream) {
  u16 delta;
  stream->value += stream->nextDelta;
  if (stream->nextAddr != 0) {
    if ((stream->nextAddr = GetStreamValue(stream->nextAddr, &delta, &stream->nextDelta)) != 0) {
      stream->nextTime += delta;
    } else {
      stream->nextTime = 0x7fffffff;
    }
  } else {
    stream->nextTime = 0x7fffffff;
  }
  return stream->value;
}

static inline void SetTickDelta(SEQ_SECTION *section, u32 deltaTime) {
  float tickDelta = (float)section->bpm * (float)deltaTime * (1.f / (40960000.f));
  tickDelta *= section->speed * (1.f / 256.f);

  section->tickDelta[section->timeIndex].low = fmodf(tickDelta * 65536.f, 65536.f);
  section->tickDelta[section->timeIndex].high = floorf(tickDelta);
}

static inline SEQ_EVENT *GetGlobalEvent(SEQ_SECTION *section) {
  SEQ_EVENT *ev;
  if ((ev = section->eventRoot) != 0 && ((section->eventRoot = ev->next) != 0)) {
    section->eventRoot->prev = 0;
  }
  return ev;
}

static inline u32 GetNextEventTime(SEQ_SECTION *section) {
  if (section->eventRoot == 0) {
    return 0;
  }
  return section->eventRoot->time;
}

static inline void RewindMTrack(u8 secIndex, u32 deltaTime) {
  if (cseq->section[secIndex].masterBase == 0) {
    return;
  }
  cseq->section[secIndex].masterAddress = cseq->section[secIndex].masterBase;
  HandleMasterTrack(secIndex);
  SetTickDelta(cseq->section + secIndex, deltaTime);
}

static SEQ_EVENT *HandleEvent(SEQ_EVENT *event, u8 secIndex, u32 *loopFlag) {
  CPAT *pa;
  NOTE_DATA *pe;
  s32 velocity;
  s32 key;
  u8 midi;
  u16 macId;
  NOTE *note;
  TENTRY *tEntry;
  CPAT *pattern;
  u32 *pTab;
  SEQ_PATTERN *pptr;

  switch (event->type) {
  case 4:
    tEntry = event->info.trackAddr;
    pattern = &cseq->pattern[event->trackId];
    pTab = ARR_GET(cseq->arrangement, cseq->arrangement->patternTable);
    pptr = ARR_GET(cseq->arrangement, pTab[tEntry->pattern]);
    pattern->addr = (NOTE_DATA *)&pptr->noteData;
    pattern->lTime = 0;
    pattern->baseTime = tEntry->time;
    pattern->patternInfo = tEntry;
    InitStream(&pattern->pitchBend, pptr->pitchBend);
    pattern->pitchBend.value = 0x2000;
    InitStream(&pattern->modulation, pptr->modulation);
    pattern->modulation.value = 0;
    pattern->midi = ARR_GET_TYPE(cseq->arrangement, cseq->arrangement->trackMidiTable, u8 *)[event->trackId];
    if (tEntry->prgChange != 0xff) {
      DoPrgChange(cseq, tEntry->prgChange, pattern->midi);
    }
    if (tEntry->velocity != 0xff) {
      inpSetMidiCtrl(SND_MIDICTRL_VOLUME, pattern->midi, curSeqId, tEntry->velocity);
    }
    break;

  case 0:
    pe = event->info.pattern.addr;
    pa = event->info.pattern.base;
    key = pe->key;
    velocity = pe->velocity;
    midi = pa->midi;

    if ((key & 0x80) != 0) {
      switch (velocity) {
      case 0:
        DoPrgChange(cseq, key & 0x7f, midi);
        break;
      case 1:
        inpSetMidiCtrl(0x82 /* TODO SND_MIDICTRL_? */, midi, curSeqId, key & 0x7f);
        break;
      default:
        if ((velocity & 0x80) != 0x80) {
          break;
        }
        switch (velocity & 0x7f) {
        case 0x68:
          if (cseq->syncActive) {
            seqCrossFade(&cseq->syncCrossInfo, cseq->syncSeqIdPtr, TRUE);
            cseq->syncActive = FALSE;
          }
          break;
        case 0x69:
          seqMIDIPriority[curSeqId][midi] = key & 0x7f;
          break;
        case 0x6a:
          seqMIDIPriority[curSeqId][midi] = (key & 0x7f) + 0x80;
          break;
        case 0x79:
          inpResetMidiCtrl(midi, curSeqId, FALSE);
          break;
        case 0x7b:
          KeyOffNotes();
          break;
        default:
          inpSetMidiCtrl(velocity & 0x7f, midi, curSeqId, key & 0x7f);
          break;
        }
      }
      break;
    }

    if ((cseq->trackMute[event->trackId / 32] & (1 << (event->trackId & 0x1f))) != 0) {
      if ((macId = cseq->prgState[midi].macId) != 0xffff) {
        key += pa->patternInfo->transpose;
        key = CLAMP(key, 0, 0x7f);

        velocity += pa->patternInfo->velocityAdd;
        velocity = CLAMP(velocity, 0, 0x7f);

        if ((note = AllocateNote(event->time + pe->length, secIndex)) != 0) {
          if ((note->id = synthStartSound(
                   macId, cseq->prgState[midi].priority, cseq->prgState[midi].maxVoices,
                   key, velocity, 64, midi, curSeqId, secIndex, 0, event->trackId,
                   cseq->trackVolGroup[event->trackId], curFadeOutState ? -1 : 0, cseq->defaultStudio,
                   synthITDDefault[cseq->defaultStudio][0])) == SND_ID_ERROR) {
            FreeNote(note);
          }
        }
      }
    }
    break;

  case 2:
    pa = event->info.pattern.base;
    inpSetMidiCtrl14(SND_MIDICTRL_PITCHBEND, pa->midi, curSeqId, HandleStream(&pa->pitchBend));
    break;

  case 1:
    pa = event->info.pattern.base;
    inpSetMidiCtrl14(SND_MIDICTRL_MODULATION, pa->midi, curSeqId, HandleStream(&pa->modulation));
    break;

  case 3:
    *loopFlag |= 1;
    return 0;
  }

  return GenerateNextTrackEvent(event->trackId);
}

/* Keep the original MSL remainder product rounded before subtraction. */
#pragma fp_contract off
static u32 HandleTrackEvents(u8 secIndex, u32 deltaTime) {
  SEQ_EVENT *ev;
  u32 loopFlag;
  SEQ_SECTION *section;

  section = &cseq->section[secIndex];
  loopFlag = FALSE;

  while (GetNextEventTime(section) <= section->time[section->timeIndex].high) {
    if ((ev = GetGlobalEvent(section)) == 0) {
      if (!loopFlag) {
        return FALSE;
      }

      loopFlag = FALSE;
      section->timeIndex ^= 1;
      section->time[section->timeIndex].high = cseq->arrangement->loopPoint[secIndex];
      section->time[section->timeIndex].low = section->time[section->timeIndex ^ 1].low;
      RewindMTrack(secIndex, deltaTime);
      section->loopCount += 1;
      InitTrackEventsSection(secIndex);
      continue;
    }

    if ((ev = HandleEvent(ev, secIndex, &loopFlag)) != 0) {
      InsertGlobalEvent(section, ev);
    }
  }

  return TRUE;
}
#pragma force_active reset

static inline void HandleKeyOffNotes() {
  NOTE *n;
  NOTE *nn;
  if (!cseq->keyOffCheck) {
    n = cseq->noteKeyOff;
    while (n != 0) {
      nn = n->next;
      if (n->id != SND_ID_ERROR && sndFXCheck(n->id) == SND_ID_ERROR) {
        seqFreeKeyOffNote(n);
      }

      n = nn;
    }
  }

  cseq->keyOffCheck = (cseq->keyOffCheck + 1) % 5;
}

void seqHandle(u32 deltaTime) {
  u32 x;
  u32 i;
  u32 j;
  u32 eventsActive;
  u32 notesActive;
  SEQ_INSTANCE *si;
  SEQ_INSTANCE *nextSi;

  if (deltaTime == 0) {
    return;
  }

  si = seqActiveRoot;
  while (si != 0) {
    nextSi = si->next;
    cseq = si;
    curSeqId = si->index;
    curFadeOutState = synthIsFadeOutActive(si->defVGroup);

    if (cseq->trackSectionTable == 0) {
      HandleMasterTrack(0);
      SetTickDelta(cseq->section, deltaTime);
      eventsActive = HandleTrackEvents(0, deltaTime);
      notesActive = HandleNotes();
      HandleKeyOffNotes();

      for (i = 0; i < 2; ++i) {
        x = cseq->section[0].time[i].low + cseq->section[0].tickDelta[i].low;
        cseq->section[0].time[i].low = x & 0xffff;
        x >>= 16;
        cseq->section[0].time[i].high += x + cseq->section[0].tickDelta[i].high;
      }
    } else {
      eventsActive = 0;
      for (i = 0; i < 16; ++i) {
        HandleMasterTrack(i);
        SetTickDelta(&cseq->section[i], deltaTime);
        eventsActive |= HandleTrackEvents(i, deltaTime);
      }
      notesActive = HandleNotes();
      HandleKeyOffNotes();

      for (i = 0; i < 16; ++i) {
        for (j = 0; j < 2; ++j) {
          x = cseq->section[i].time[j].low + cseq->section[i].tickDelta[j].low;
          cseq->section[i].time[j].low = x & 0xffff;
          x >>= 16;
          cseq->section[i].time[j].high += x + cseq->section[i].tickDelta[j].high;
        }
      }
    }

    if (eventsActive == 0 && notesActive == 0) {
      if (si->prev != 0) {
        si->prev->next = nextSi;
      } else {
        seqActiveRoot = nextSi;
      }
      if (nextSi != 0) {
        nextSi->prev = si->prev;
      }
      ResetNotes(si);
      si->state = SEQ_STATE_FREE;
      si->prev = 0;
      if ((si->next = seqFreeRoot) != 0) {
        seqFreeRoot->prev = si;
      }
      seqFreeRoot = si;
    }
    si = nextSi;
  }
}

void seqCrossFade(SEQ_CROSSFADE *ci, u32 *new_seqId, u8 irq_call) {
  SND_PLAYPARA pp;
  u32 seqId;
  u16 time;

  seqId = seqGetPrivateId(ci->seqId1);

  if ((ci->flags & SND_CROSSFADE_SYNC) != 0) {
    seqInstance[seqId].syncCrossInfo = *ci;
    seqInstance[seqId].syncActive = TRUE;
    seqInstance[seqId].syncSeqIdPtr = new_seqId;
    seqInstance[seqId].syncCrossInfo.flags &= ~SND_CROSSFADE_SYNC;
    *new_seqId = ci->seqId1 | SND_SEQ_CROSSFADE_ID;
    return;
  }

  if (irq_call) {
    time = ci->time1 < 5 ? 5 : ci->time1;
    if ((ci->flags & SND_CROSSFADE_PAUSE) != 0) {
      seqVolume(0, time, ci->seqId1, SND_SEQVOL_PAUSE);
    } else if ((ci->flags & SND_CROSSFADE_MUTE) != 0) {
      seqVolume(0, time, ci->seqId1, SND_SEQVOL_MUTE);
    } else {
      seqVolume(0, time, ci->seqId1, SND_SEQVOL_STOP);
    }
  } else {
    if ((ci->flags & SND_CROSSFADE_PAUSE) != 0) {
      sndSeqVolume(0, ci->time1, ci->seqId1, SND_SEQVOL_PAUSE);
    } else if ((ci->flags & SND_CROSSFADE_MUTE) != 0) {
      sndSeqVolume(0, ci->time1, ci->seqId1, SND_SEQVOL_MUTE);
    } else {
      sndSeqVolume(0, ci->time1, ci->seqId1, SND_SEQVOL_STOP);
    }
  }

  if (new_seqId == 0) {
    return;
  }

  if ((ci->flags & SND_CROSSFADE_CONTINUE) != 0) {
    if (seqGetPrivateId(ci->seqId2) != SND_SEQ_ERROR_ID) {
      if (irq_call) {
        seqContinue(ci->seqId2);
        seqVolume(ci->volume, ci->time2, ci->seqId2, SND_SEQVOL_CONTINUE);
        if ((ci->flags & SND_CROSSFADE_TRACKMUTE) != 0) {
          seqMute(ci->seqId2, ci->trackMute[0], ci->trackMute[1]);
        }
        if ((ci->flags & SND_CROSSFADE_SPEED) != 0) {
          seqSpeed(ci->seqId2, ci->speed);
        }
      } else {
        sndSeqContinue(ci->seqId2);
        sndSeqVolume(ci->volume, ci->time2, ci->seqId2, SND_SEQVOL_CONTINUE);
        if ((ci->flags & SND_CROSSFADE_TRACKMUTE) != 0) {
          sndSeqMute(ci->seqId2, ci->trackMute[0], ci->trackMute[1]);
        }
        if ((ci->flags & SND_CROSSFADE_SPEED) != 0) {
          sndSeqSpeed(ci->seqId2, ci->speed);
        }
      }
      *new_seqId = ci->seqId2;
    } else {
      *new_seqId = SND_SEQ_ERROR_ID;
    }
  } else {
    pp.flags = SND_PLAYPARA_VOLUME;
    if ((ci->flags & SND_CROSSFADE_PAUSENEW) != 0) {
      pp.flags |= SND_PLAYPARA_PAUSE;
    }
    if ((ci->flags & SND_CROSSFADE_SPEED) != 0) {
      pp.flags |= SND_PLAYPARA_SPEED;
      pp.speed = ci->speed;
    }
    if ((ci->flags & SND_CROSSFADE_TRACKMUTE) != 0) {
      pp.flags |= SND_PLAYPARA_TRACKMUTE;
      pp.trackMute[0] = ci->trackMute[0];
      pp.trackMute[1] = ci->trackMute[1];
    }
    pp.volume.time = ci->time2;
    pp.volume.target = ci->volume;
    pp.numFaded = 0;
    if (irq_call != 0) {
      if ((*new_seqId = seqPlaySong(ci->gid2, ci->sid2, ci->arr2, &pp, TRUE, ci->studio2)) !=
              SND_SEQ_ERROR_ID &&
          (ci->flags & SND_CROSSFADE_MUTENEW) != 0) {
        seqMute(*new_seqId, 0, 0);
      }
    } else {
      if ((*new_seqId = sndSeqPlayEx(ci->gid2, ci->sid2, ci->arr2, &pp, ci->studio2)) !=
              SND_SEQ_ERROR_ID &&
          (ci->flags & SND_CROSSFADE_MUTENEW) != 0) {
        sndSeqMute(*new_seqId, 0, 0);
      }
    }
  }
}
u32 sndSeqPlayEx(u16 group, u16 song, void *arrangement, SND_PLAYPARA *para, u8 studio)
{
    return seqPlaySong(group, song, arrangement, para, 0, studio);
}
#pragma fp_contract reset
u16 seqGetMIDIPriority(u8 set, u8 channel)
{
    return seqMIDIPriority[set][channel];
}
