#include "musyx_synth.h"
#include "musyx_midictrl.h"
#include "musyx_hardware.h"

typedef struct SND_AUX_INFO SND_AUX_INFO;
typedef void (*SND_AUX_CALLBACK)(u8 reason, SND_AUX_INFO *info, void *user);

extern u8 sndActive;
extern unsigned long synthMasterFaderActiveFlags;
extern u32 synthFlags;
extern u8 synthJobTableIndex;
static u32 synthTicksPerSecond[9][16];
static SYNTH_JOB *synthJobTable[32][3];
SYNTH_INFO synthInfo;
SYNTH_MASTER_FADER synthMasterFader[32];
u8 synthTrackVolume[64];
extern void hwDisableHRTF(void);
extern void streamOutputModeChanged(void);
extern void seqStop(unsigned long sequence);
extern void seqPause(u32 sequence);
extern void seqMute(u32 sequence, u32 mask1, u32 mask2);
extern u32 hwAddInput(u8 studio, void *input);
extern u32 hwRemoveInput(u8 studio, void *input);

extern u32 vidGetInternalId(u32 vid);
extern void inpFXCopyCtrl(u8 control, SYNTH_VOICE *dest, SYNTH_VOICE *source);
extern void macSetExternalKeyoff(SYNTH_VOICE *voice);
extern u16 inpGetPedal(SYNTH_VOICE *voice);
extern void macSetPedalState(SYNTH_VOICE *voice, u32 state);
extern void hwStart(u32 index, u8 studio);
extern void hwKeyOff(u32 index);
extern SND_AUX_CALLBACK synthAuxACallback[8];
extern SND_AUX_CALLBACK synthAuxBCallback[8];
extern u8 synthAuxAMIDI[8];
extern u8 synthAuxBMIDI[8];
extern u8 synthITDDefault[8][2];
extern void hwActivateStudio(u8 studio, u32 isMaster, u32 type);
extern void hwDeactivateStudio(u8 studio);
extern u32 voiceKillSound(u32 vid);
extern void hwOff(s32 index);

void synthKeyStateUpdate(void *voice)
{
    synthAddJob(voice, 2, 0);
}

void sndOutputMode(u32 mode)
{
    u32 old = synthFlags;
    u32 voice;

    switch (mode) {
    case 0:
        synthFlags |= 1;
        synthFlags &= ~2;
        hwDisableHRTF();
        break;
    case 1:
        synthFlags &= ~1;
        synthFlags &= ~2;
        hwDisableHRTF();
        break;
    case 2:
        synthFlags &= ~1;
        synthFlags |= 2;
        hwDisableHRTF();
        break;
    }
    if (old != synthFlags) {
        for (voice = 0; voice < synthInfo.voice_count; voice++)
            synthVoice[voice].cFlags |= CFLAG_PAN_UPDATE;
        streamOutputModeChanged();
    }
}

void synthSetMusicVolumeType(u8 group, u8 type)
{
    if (sndActive)
        synthMasterFader[group].type = type;
}



u32 synthAddStudioInput(u8 studio, void *input)
{
    return hwAddInput(studio, input);
}

u32 synthRemoveStudioInput(u8 studio, void *input)
{
    return hwRemoveInput(studio, input);
}

void synthSetBpm(u32 bpm, u8 set, u8 section)
{
    if (set == 0xFF)
        set = 8;
    synthTicksPerSecond[set][section] = ((bpm << 3) * 0x600) / 240;
}



void synthForceLowPrecisionUpdate(SYNTH_VOICE *voice)
{
    synthAddJob(voice, 0, 0);
    synthAddJob(voice, 1, 0);
}

u32 synthGetTicksPerSecond(SYNTH_VOICE *voice)
{
    return synthTicksPerSecond[voice->midiSet == 0xFF ? 8 : voice->midiSet][voice->section];
}

#pragma auto_inline off
void synthAddJob(SYNTH_VOICE *voice, s32 type, u32 delta)
{
    u8 index = ((delta >> 8) + synthJobTableIndex) & 31;
    SYNTH_JOB *job;
    SYNTH_JOB **head;
    SYNTH_JOB **row = synthJobTable[index];

    /* Callers pass only job types 0, 1, and 2. */
    switch (type) {
    case 0:
        job = &voice->jobs[0];
        if (job->queue_index != 0xFF) {
            if (job->queue_index == index)
                return;
            if (job->next)
                job->next->prev = job->prev;
            if (job->prev)
                job->prev->next = job->next;
            else
                synthJobTable[job->queue_index][0] = job->next;
        }
        head = &row[0];
        break;
    case 1:
        job = &voice->jobs[1];
        if (job->queue_index != 0xFF) {
            if (job->queue_index == index)
                return;
            if (job->next)
                job->next->prev = job->prev;
            if (job->prev)
                job->prev->next = job->next;
            else
                synthJobTable[job->queue_index][2] = job->next;
        }
        head = &row[2];
        break;
    case 2:
        job = &voice->jobs[2];
        if (job->queue_index != 0xFF)
            return;
        head = &row[1];
        break;
    }
    job->queue_index = index;
    if ((job->next = *head) != 0)
        (*head)->prev = job;
    job->prev = 0;
    *head = job;
}

void synthStartSynthJobHandling(SYNTH_VOICE *voice)
{
    voice->lastLowCallTime = synthRealTime;
    voice->lastZeroCallTime = synthRealTime;
    synthAddJob(voice, 0, 0);
    synthAddJob(voice, 1, 0);
}

#pragma force_active on
static void HandleJobQueue(SYNTH_JOB **head, void (*handler)(u32))
{
    SYNTH_JOB *job = *head, *next;

    while (job) {
        next = job->next;
        job->queue_index = 0xFF;
        if (!synthVoice[job->voice_index].blocked)
            handler(job->voice_index);
        job = next;
    }
    *head = 0;
}

static inline void HandleFaderTermination(SYNTH_MASTER_FADER *fader)
{
    switch (fader->action) {
    case 1:
        seqStop(fader->sequence);
        break;
    case 2:
        seqPause(fader->sequence);
        break;
    case 3:
        seqMute(fader->sequence, 0, 0);
        break;
    }
}
#pragma force_active reset

u32 synthIsFadeOutActive(u8 group)
{
    if (synthMasterFader[group].type != 4 && (synthMasterFaderActiveFlags & (1U << group))) {
        if (synthMasterFader[group].start > synthMasterFader[group].target)
            return 1;
    }
    return 0;
}

#pragma auto_inline on
void synthInitPortamento(SYNTH_VOICE *voice)
{
    if (!(voice->cFlags & CFLAG_LEGATO)) {
        if (voice->portamento_type == 1) {
            if (!(voice->cFlags & 0x1000ULL))
                voice->portTime = 0;
            else
                voice->portTime = voice->portDuration;
        } else {
            voice->portTime = voice->portDuration;
        }
        voice->portCurPitch = voice->portamento_key << 16;
    }
}
#pragma auto_inline off


u32 synthFXSetCtrl(u32 vid, u8 control, u8 value)
{
    u32 result = 0;
    u32 id = vidGetInternalId(vid);
    while (id != 0xFFFFFFFF) {
        u8 index = id;
        if (id == synthVoice[index].id) {
            if (synthVoice[index].cFlags & CFLAG_STARTING)
                inpSetMidiCtrl(control, index, synthVoice[index].setupMidiSet, value);
            else
                inpSetMidiCtrl(control, index, synthVoice[index].midiSet, value);
            result = 1;
            id = synthVoice[index].nextID;
        } else {
            return result;
        }
    }
    return result;
}
u32 synthFXSetCtrl14(u32 vid, u8 control, u16 value)
{
    u32 result = 0;
    u32 id = vidGetInternalId(vid);
    while (id != 0xFFFFFFFF) {
        u8 index = id;
        if (id == synthVoice[index].id) {
            if (synthVoice[index].cFlags & CFLAG_STARTING)
                inpSetMidiCtrl14(control, index, synthVoice[index].setupMidiSet, value);
            else
                inpSetMidiCtrl14(control, index, synthVoice[index].midiSet, value);
            result = 1;
            id = synthVoice[index].nextID;
        } else {
            return result;
        }
    }
    return result;
}
void synthFXCloneMidiSetup(SYNTH_VOICE *dest, SYNTH_VOICE *source)
{
    inpFXCopyCtrl(7, dest, source);
    inpFXCopyCtrl(10, dest, source);
    inpFXCopyCtrl(91, dest, source);
    inpFXCopyCtrl(128, dest, source);
    inpFXCopyCtrl(132, dest, source);
}
u32 synthSendKeyOff(u32 vid)
{
    u8 index;
    u32 result = 0;
    if (sndActive) {
        u32 id = vidGetInternalId(vid);
        while (id != 0xFFFFFFFF) {
            index = id;
            if (synthVoice[index].id == id) {
                macSetExternalKeyoff(&synthVoice[index]);
                result = 1;
            }
            id = synthVoice[index].nextID;
        }
    }
    return result;
}

#pragma force_active on
static void EventHandler(u32 index)
{
    SYNTH_VOICE *voice = &synthVoice[index];
    if (hwIsActive(index) || voice->macro != 0) {
        macSetPedalState(voice, inpGetPedal(voice) > 0x1F80);
        if (voice->cFlags & CFLAG_HW_START_PENDING) {
            voice->cFlags &= ~CFLAG_HW_START_PENDING;
            voice->cFlags |= CFLAG_HW_KEYON;
            hwStart(index, voice->studio);
        }
        if ((voice->cFlags & (CFLAG_PEDAL | CFLAG_HW_KEYOFF_PENDING | CFLAG_HW_KEYON)) == (CFLAG_HW_KEYOFF_PENDING | CFLAG_HW_KEYON)) {
            voice->cFlags &= ~(CFLAG_HW_KEYOFF_PENDING | CFLAG_HW_KEYON);
            hwKeyOff(index);
            if (voice->cFlags & CFLAG_PITCH_ADSR) {
                if (adsrRelease(&voice->adsr))
                    voice->cFlags &= ~CFLAG_PITCH_ADSR;
            }
        }
    }
    if (voice->timeUsedByInput) {
        voice->timeUsedByInput = 0;
        voice->midiDirtyFlags = 0x1FFF;
    }
}
#pragma force_active reset

void synthActivateStudio(u8 studio, u32 isMaster, u32 type)
{
    hwDisableIrq();
    synthAuxACallback[studio] = 0;
    synthAuxBCallback[studio] = 0;
    synthAuxAMIDI[studio] = 0xFF;
    synthAuxBMIDI[studio] = 0xFF;
    synthITDDefault[studio][1] = 0;
    synthITDDefault[studio][0] = 0;
    hwActivateStudio(studio, isMaster, type);
    hwEnableIrq();
}
void synthDeactivateStudio(u8 studio)
{
    u32 index;
    for (index = 0; index < synthInfo.voice_count; index++) {
        if (synthVoice[index].studio == studio) {
            if (synthVoice[index].id != 0xFFFFFFFF)
                voiceKillSound(synthVoice[index].vidList->vid);
            else if (hwIsActive(index))
                hwOff(index);
        }
    }
    hwDisableIrq();
    synthAuxACallback[studio] = 0;
    synthAuxBCallback[studio] = 0;
    synthAuxAMIDI[studio] = 0xFF;
    synthAuxBMIDI[studio] = 0xFF;
    hwEnableIrq();
    hwDeactivateStudio(studio);
}

extern u32 hwGetVirtualSampleID(u32 index);
extern void vsSampleEndNotify(u32 id);
extern u32 vsSampleStartNotify(u8 index);
extern void macSampleEndNotify(SYNTH_VOICE *voice);
extern void voiceKill(u32 index);
u32 synthHWMessageHandler(u32 message, u32 id)
{
    u32 result = 0;
    switch (message) {
    case 0:
        if (!synthVoice[(u8)id].blocked) {
            vsSampleEndNotify(hwGetVirtualSampleID((u8)id));
            if (synthVoice[(u8)id].id == id)
                macSampleEndNotify(&synthVoice[(u8)id]);
        }
        break;
    case 1:
        voiceKill((u8)id);
        break;
    case 2:
        result = vsSampleStartNotify((u8)id);
        break;
    case 3:
        vsSampleEndNotify(hwGetVirtualSampleID((u8)id));
        break;
    }
    return result;
}


/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/synth.c (CC0-1.0). */
typedef struct KEYMAP { u16 id; s8 transpose; u8 panning; s16 prioOffset; u8 reserved[2]; } KEYMAP;
typedef struct LAYER { u16 id; u8 keyLow, keyHigh; s8 transpose; u8 volume; s16 prioOffset; u8 panning, reserved[3]; } LAYER;
#define SND_ID_ERROR 0xFFFFFFFF
#define TRUE 1
#define FALSE 0
#define NULL 0
#define CLAMP(value, low, high) ((value) > (high) ? (high) : ((value) < (low) ? (low) : (value)))
#define CLAMP_INV(value, low, high) ((value) < (low) ? (low) : ((value) > (high) ? (high) : (value)))
extern void *dataGetKeymap(u16 id);
extern void *dataGetLayer(u16 id, u16 *count);
extern void vidRemoveVoiceReferences(SYNTH_VOICE *voice);
extern u32 vidMakeNew(SYNTH_VOICE *voice, u32 master);
extern u32 vidMakeRoot(SYNTH_VOICE *voice);
extern void voiceSetLastStarted(SYNTH_VOICE *voice);
extern u32 macStart(u16 id, u8 priority, u8 maxVoices, u16 allocationId, u8 key, u8 volume, u8 pan, u8 midi, u8 set, u8 section, u16 step, u16 track, u8 newVID, u8 group, u8 studio, u32 itd);
static u32 StartKeymap(u16 id, s16 priority, u8 maxVoices, u16 allocationId, u8 key, u8 volume, u8 pan, u8 midi, u8 set, u8 section, u16 step, u16 track, u32 newVID, u8 group, u8 studio, u32 itd);

#pragma force_active on
static u32 do_voice_portamento(u8 key, u8 midi, u8 midiSet, u32 isMaster, u32 *rejected) {
  u32 i;
  u32 vid;
  u32 id;
  SYNTH_VOICE *sv;
  SYNTH_VOICE *last_sv;
  u32 legatoVoiceIsStarting;
  legatoVoiceIsStarting = FALSE;
  vid = SND_ID_ERROR;
  for (i = 0, sv = synthVoice; i < synthInfo.voice_count; ++i, sv++) {
    if (
        sv->id != SND_ID_ERROR && sv->midi == midi && sv->midiSet == midiSet) {
      if ((sv->cFlags & CFLAG_STARTING) != 0) {
        legatoVoiceIsStarting = TRUE;
      }
      if ((sv->cFlags & CFLAG_HW_KEYON) != 0 && (sv->cFlags & (CFLAG_PEDAL | CFLAG_EXTERNAL_KEYOFF)) != CFLAG_EXTERNAL_KEYOFF && hwIsActive(i)) {
        if (vid == SND_ID_ERROR && (sv->cFlags & (CFLAG_LEGATO | CFLAG_STARTING)) == (CFLAG_LEGATO | CFLAG_STARTING)) {
          *rejected = TRUE;
          return SND_ID_ERROR;
        }
        last_sv = sv;
        sv->portCurPitch = (sv->curNote * 65536) + (sv->curDetune * 65536) / 100;
        sv->portamento_key = sv->curNote;
        sv->curNote = key + ((sv->curNote & 0xff) - sv->orgNote);
        sv->orgNote = key;
        sv->curDetune = 0;
        sv->portTime = 0;
        sv->cFlags |= CFLAG_LEGATO;
        vidRemoveVoiceReferences(&synthVoice[i]);
        if (vid == SND_ID_ERROR) {
          sv->nextID = SND_ID_ERROR;
          sv->prevID = SND_ID_ERROR;
          vid = vidMakeNew(&synthVoice[i], isMaster);
          id = sv->id;
        } else {
          synthVoice[id & 0xff].nextID = sv->id;
          sv->prevID = id;
          id = sv->id;
          vidMakeNew(&synthVoice[i], FALSE);
        }
      }
    }
  }
  if (vid != SND_ID_ERROR) {
    voiceSetLastStarted(last_sv);
    inpSetMidiLastNote(last_sv->midi, last_sv->midiSet, last_sv->curNote);
    *rejected = FALSE;
  } else {
    *rejected = legatoVoiceIsStarting;
  }
  return vid;
}

static inline u32 check_portamento(u8 key, u8 midi, u8 midiSet, u32 newVID, u32 *vid) {
  u32 rejected;
  if (inpGetMidiCtrl(SND_MIDICTRL_PORTAMENTO, midi, midiSet) > 8064) {
    *vid = do_voice_portamento(key & 0x7f, midi, midiSet, newVID, &rejected);
    return !rejected;
  }
  *vid = 0xFFFFFFFF;
  return 1;
}

static u32 StartLayer(u16 layerID, s16 prio, u8 maxVoices,
                      u16 allocId,
                      u8 key, u8 vol, u8 panning, u8 midi, u8 midiSet, u8 section, u16 step,
                      u16 trackid, u32 vidFlag, u8 vGroup, u8 studio, u32 itd) {
  u16 n;
  u32 vid;
  u32 new_id;
  unsigned long id;
  LAYER *l;
  s32 p;
  s32 k;
  u8 v;
  u8 mKey;
  vid = SND_ID_ERROR;
  if ((l = dataGetLayer(layerID, &n)) == NULL) {
    goto end;
  }
  mKey = key & 0x7f;
  for (; n != 0; --n, l++) {
    if (l->id == 0xffff || l->keyLow > mKey || l->keyHigh < mKey) {
      continue;
    }
    k = mKey + l->transpose;
    k = CLAMP(k, 0, 127);
    if ((l->id & 0xC000) == 0) {
      if (check_portamento(k, midi, midiSet, 0, &new_id)) {
        if (new_id != 0xFFFFFFFF) {
          goto apply_new_id;
        } else {
          goto start_new_id;
        }
      }
      continue;
    }
  start_new_id:
    if ((l->panning & 0x80) == 0) {
      p = l->panning - 0x40;
      p += panning;
      p = CLAMP_INV(p, 0, 0x7f);
    } else {
      p = 0x80;
    }
    v = (vol * l->volume) / 0x7f;
    prio += l->prioOffset;
    prio = CLAMP(prio, 0, 0xff);
    switch (l->id & 0xC000) {
    case 0:
      new_id = macStart(l->id, prio, maxVoices, allocId, k | (key & 0x80), v, p, midi, midiSet,
                        section, step, trackid, 0, vGroup, studio, itd);
      break;
    case 0x4000:
      new_id = StartKeymap(l->id, prio, maxVoices, allocId, k | (key & 0x80), v, p, midi, midiSet,
                           section, step, trackid, 0, vGroup, studio, itd);
      break;
    case 0x8000:
      new_id = StartLayer(l->id, prio, maxVoices, allocId, k | (key & 0x80), v, p, midi, midiSet,
                          section, step, trackid, 0, vGroup, studio, itd);
      break;
    }
    if (new_id != SND_ID_ERROR) {
    apply_new_id:
      if (vid == SND_ID_ERROR) {
        if (vidFlag != 0) {
          vid = vidMakeRoot(&synthVoice[new_id & 0xff]);
        } else {
          vid = new_id;
        }
      } else {
        synthVoice[id & 0xff].nextID = new_id;
        synthVoice[new_id & 0xff].prevID = id;
      }
      id = new_id;
      while (synthVoice[id & 0xff].nextID != SND_ID_ERROR) {
        id = synthVoice[id & 0xff].nextID;
      }
    }
  }
end:
  return vid;
}

static u32 StartKeymap(u16 keymapID, s16 prio, u8 maxVoices,
                       u16 allocId,
                       u8 key, u8 vol, u8 panning, u8 midi, u8 midiSet, u8 section, u16 step,
                       u16 trackid, u32 vidFlag, u8 vGroup, u8 studio, u32 itd) {
  u8 o;
  KEYMAP *keymap;
  s32 p;
  s32 k;
  u32 vid;
  if ((keymap = dataGetKeymap(keymapID)) != NULL) {
    o = key & 0x7f;
    if (keymap[o].id != 0xffff && (keymap[o].id & 0xc000) != 0x4000) {
      if ((keymap[o].panning & 0x80) == 0) {
        p = (keymap[key].panning - 0x40);
        p += panning;
        if (p < 0) {
          panning = 0;
        } else if (p > 0x7f) {
          panning = 0x7f;
        } else {
          panning = p;
        }
      } else {
        panning = 0x80;
      }
      k = (key & 0x7f) + keymap[o].transpose;
      k = CLAMP(k, 0, 127);
      prio += keymap[o].prioOffset;
      prio = CLAMP(prio, 0, 0xff);
      if ((keymap[o].id & 0xc000) == 0) {
        if (!check_portamento(k & 0xff, midi, midiSet, vidFlag, &vid)) {
          return 0xffffffff;
        }
        if (vid != 0xffffffff) {
          return vid;
        }
        return macStart(keymap[o].id, prio, maxVoices, allocId, k | (key & 0x80), vol, panning,
                        midi, midiSet, section, step, trackid, vidFlag, vGroup, studio, itd);
      }
      return StartLayer(keymap[o].id, prio, maxVoices, allocId, k | (key & 0x80), vol, panning,
                        midi, midiSet, section, step, trackid, vidFlag & 0xff, vGroup, studio, itd);
    }
  }
  return SND_ID_ERROR;
}

#pragma auto_inline on
u32 synthStartSound(u16 id, u8 prio, u8 max,
                    u8 key, u8 vol, u8 panning, u8 midi, u8 midiSet, u8 section, u16 step,
                    u16 trackid, u8 vGroup, s16 prioOffset, u8 studio, u32 itd) {
  prio += prioOffset;
  prio = CLAMP(prio, 0, 0xff);
  switch (id & 0xC000) {
  case 0: {
    u32 vid;
    if (!check_portamento(key, midi, midiSet, 1, &vid)) {
      return SND_ID_ERROR;
    }
    if (vid != SND_ID_ERROR) {
      return vid;
    }
    return macStart(id, prio, max,
                    id,
                    key, vol, panning, midi, midiSet, section, step, trackid, 1, vGroup, studio,
                    itd);
  }
  case 0x4000: {
    return StartKeymap(id, prio, max, id, key, vol, panning, midi, midiSet, section, step, trackid,
                       1, vGroup, studio, itd);
  }
  case 0x8000: {
    return StartLayer(id, prio, max, id, key, vol, panning, midi, midiSet, section, step, trackid,
                      1, vGroup, studio, itd);
  }
  default:
    return SND_ID_ERROR;
  }
}
#pragma auto_inline off

#pragma force_active reset

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/synth.c (CC0-1.0). */
#define NULL 0
#define TRUE 1
#define FALSE 0
#define SYNTH_FLAG_MONO 1
#define SYNTH_FLAG_SURROUND 2
#define SYNTH_JOBTYPE_ZERO 1
#define SND_AUX_REASON_PARAMETERUPDATE 1
#define SND_AUX_NUMPARAMETERS 4
#define SND_STUDIO_MAXNUM 8
#define SND_USERMUSIC_VOLGROUPS 0xfa
#define SND_USERFX_VOLGROUPS 0xfb
#define SND_USERALL_VOLGROUPS 0xfc
#define SND_MUSIC_VOLGROUPS 0xfd
#define SND_FX_VOLGROUPS 0xfe
#define SND_ALL_VOLGROUPS 0xff
extern void macHandle(u32 deltaTime);
extern u8 hwGetTimeOffset(void);
extern void hwFrameDone(void);
extern unsigned long synthMasterFaderPauseActiveFlags;
extern u8 synthAuxAMIDISet[8], synthAuxBMIDISet[8];
extern void *synthAuxAUser[8], *synthAuxBUser[8];
extern u8 synthTrackVolume[64];
struct SND_AUX_INFO { union { struct { s32 *left, *right, *surround; } bufferUpdate; struct { u16 para[4]; } parameterUpdate; } data; };
typedef struct FX_TAB { u16 id, macro; u8 maxVoices, priority, volume, panning, key, vGroup; } FX_TAB;
extern FX_TAB *dataGetFX(u16 id);
extern s16 sndSin(u16 angle);
extern u32 adsrHandleLowPrecision(ADSR_VARS *adsr, u16 *start, u16 *delta);
extern u16 inpGetPitchBend(SYNTH_VOICE *voice);
extern u16 inpGetModulation(SYNTH_VOICE *voice);
extern u16 inpGetDoppler(SYNTH_VOICE *voice);
extern u16 inpGetPanning(SYNTH_VOICE *voice);
extern u16 inpGetSurPanning(SYNTH_VOICE *voice);
extern u16 inpGetTremolo(SYNTH_VOICE *voice);
extern u16 inpGetVolume(SYNTH_VOICE *voice);
extern u16 inpGetPreAuxA(SYNTH_VOICE *voice);
extern u16 inpGetReverb(SYNTH_VOICE *voice);
extern u16 inpGetPreAuxB(SYNTH_VOICE *voice);
extern u16 inpGetPostAuxB(SYNTH_VOICE *voice);
extern u16 inpGetAuxA(u8 studio, u8 index, u8 midi, u8 set);
extern u16 inpGetAuxB(u8 studio, u8 index, u8 midi, u8 set);
extern u32 sndGetPitch(u8 key, u32 sample);
extern u16 sndPitchUpOne(u16 pitch);
extern void hwSetPitch(u32 index, u16 pitch);
extern void hwSetVolume(u32 index, u8 table, f32 volume, u32 pan, u32 span, f32 auxA, f32 auxB);
extern void sndConvertMs(unsigned long *time);
#pragma auto_inline on
#pragma fp_contract off
void *synthAuxAUser[8];
SND_AUX_CALLBACK synthAuxACallback[8];
void *synthAuxBUser[8];
SND_AUX_CALLBACK synthAuxBCallback[8];

static inline void UpdateTimeMIDICtrl(SYNTH_VOICE *sv) {
  if (!sv->timeUsedByInput) {
    return;
  }
  sv->timeUsedByInput = 0;
  sv->midiDirtyFlags = 0x1fff;
}

static inline unsigned long apply_portamento(SYNTH_VOICE *svoice, unsigned long ccents, unsigned long deltaTime) {
  unsigned long old_portCurPitch;
  if ((svoice->cFlags & CFLAG_PORTAMENTO) != 0 && (int)((svoice->portDuration - svoice->portTime) >> 8) > 0) {
    old_portCurPitch = svoice->portCurPitch;
    svoice->portCurPitch += (int)deltaTime * ((int)(ccents - svoice->portCurPitch) >> 8) /
                            (int)((svoice->portDuration - svoice->portTime) >> 8);
    if ((old_portCurPitch < ccents && svoice->portCurPitch < ccents) ||
        (old_portCurPitch > ccents && (svoice->portCurPitch > ccents))) {
      ccents = svoice->portCurPitch;
      svoice->portTime += deltaTime;
    } else {
      svoice->portTime = svoice->portDuration;
    }
  }
  return ccents;
}

static inline unsigned long convert_cents(SYNTH_VOICE *svoice, unsigned long ccents) {
  unsigned long curDetune;
  unsigned long cpitch;
  cpitch = sndGetPitch(ccents / 65536, svoice->sampleInfo) * 65536;
  if ((curDetune = ccents & 0xffff) != 0) {
    cpitch += curDetune * ((sndPitchUpOne(cpitch / 65536) & 0xffff) - (cpitch / 65536));
  }
  return cpitch;
}

static inline void SetupFader(SYNTH_MASTER_FADER *smf, u8 volume, unsigned long time, u8 seqMode, unsigned long seqId) {
  smf->action = seqMode;
  smf->sequence = seqId;
  if (time != 0) {
    smf->start = smf->volume;
    smf->target = (f32)volume * (1.f / 127.f);
    smf->time = 1.f;
    smf->deltaTime = 1280.f / (f32)time;
  } else {
    smf->volume = smf->target = (f32)volume * (1.f / 127.f);
    if (smf->sequence != SND_ID_ERROR) {
      HandleFaderTermination(smf);
    }
  }
}

#pragma force_active on

static void LowPrecisionHandler(u32 i) {
  unsigned long j;
  signed long pbend;
  unsigned long ccents;
  unsigned long cpitch;
  u16 Modulation;
  u16 portamento;
  unsigned long lowDeltaTime;
  SYNTH_VOICE *sv;
  unsigned long cntDelta;
  unsigned long addFactor;
  u16 adsr_start;
  u16 adsr_delta;
  signed long vrange;
  signed long voff;
  sv = &synthVoice[i];
  if (!hwIsActive(i) && sv->macro == NULL) {
    goto end;
  }
  lowDeltaTime = synthRealTime - sv->lastLowCallTime;
  sv->lastLowCallTime = synthRealTime;
  for (j = 0; j < 2; ++j) {
    if (sv->lfo[j].period == 0) {
      continue;
    }
    sv->lfo[j].time += lowDeltaTime;
    sv->lfo[j].value =
        sndSin((sv->lfo[j].time % sv->lfo[j].period * 16) / (sv->lfo[j].period / 256));
    if (sv->lfo[j].value != sv->lfo[j].lastValue) {
      sv->lfo[j].lastValue = sv->lfo[j].value;
      if (sv->lfoUsedByInput[j]) {
        sv->lfoUsedByInput[j] = 0;
        sv->midiDirtyFlags |= 0x1fff;
      }
    }
  }
  if ((sv->cFlags & CFLAG_VIBRATO) != 0) {
    sv->vibratoTime += lowDeltaTime;
    sv->vibratoOffset = sndSin((sv->vibratoTime % sv->vibratoPeriod * 16) / (sv->vibratoPeriod / 256));
  }
  if (sv->sweepNum[0] | sv->sweepNum[1]) {
    cntDelta = (lowDeltaTime << 8) >> 4;
    addFactor = (lowDeltaTime << 4) >> 4;
    for (j = 0; j < 2; ++j) {
      if (sv->sweepNum[j] == 0) {
        continue;
      }
      sv->sweepCnt[j] -= cntDelta;
      if (sv->sweepCnt[j] <= 0) {
        sv->sweepCnt[j] = sv->sweepNum[j] << 16;
        sv->sweepOff[j] = 0;
      } else {
        sv->sweepOff[j] += (sv->sweepAdd[j] >> 12) * addFactor;
      }
    }
  }
  for (j = 0; j < 2; ++j) {
    if (sv->panCurrent[j] == sv->panTarget[j]) {
      continue;
    }
    sv->panTime[j] -= lowDeltaTime;
    if ((signed long)sv->panTime[j] <= 0) {
      sv->panCurrent[j] = sv->panTarget[j];
      sv->panTime[j] = 0;
    } else {
      sv->panCurrent[j] = sv->panTarget[j] - (sv->panTime[j] / 256) * sv->panDelta[j];
      sv->panCurrent[j] = CLAMP_INV((signed long)sv->panCurrent[j], 0, 0x7f0000u);
    }
    sv->cFlags |= CFLAG_PAN_UPDATE;
  }
  if ((sv->cFlags & CFLAG_PITCH_ADSR) != 0 &&
      adsrHandleLowPrecision(&sv->adsr, &adsr_start, &adsr_delta)) {
    sv->cFlags &= ~CFLAG_PITCH_ADSR;
  }
  ccents = sv->curNote * 65536 + (sv->curDetune * 65536) / 100;
  if ((sv->cFlags & (CFLAG_PITCHBEND_AFTER_KEYOFF | CFLAG_HW_KEYON)) != 0) {
    if (sv->midi != 0xff) {
      pbend = inpGetPitchBend(sv);
      sv->pbLast = pbend;
      goto pbend_adjust;
    }
  } else {
    pbend = sv->pbLast;
  pbend_adjust:
    if (pbend != 0x2000) {
      pbend -= 0x2000;
      if (pbend < 0) {
        ccents += sv->pbLowerKeyRange * pbend * 8;
      } else {
        ccents += sv->pbUpperKeyRange * pbend * 8;
      }
    }
  }
  if ((sv->cFlags & CFLAG_VIBRATO) != 0) {
    Modulation = inpGetModulation(sv);
    vrange = sv->vibratoKeyRange * 256 + (sv->vibratoCentRange * 256) / 100;
    if (sv->vibratoModScale != 0) {
      vrange += (sv->vibratoModScale * ((Modulation >> 7) & 0x1ff)) >> 7;
    }
    if ((sv->cFlags & CFLAG_VIBRATO_MODWHEEL) != 0) {
      voff = (sv->vibratoOffset * ((Modulation >> 7) & 0x1ff)) >> 7;
    } else {
      voff = sv->vibratoOffset;
    }
    ccents += (vrange * voff) >> 4;
  }
  if (sv->midi != 0xff) {
    portamento = inpGetMidiCtrl(SND_MIDICTRL_PORTAMENTO, sv->midi, sv->midiSet);
    if (portamento != sv->portLastCtrlState || (sv->cFlags & 0x21000) == 0x20000) {
      if (portamento <= 0x1f80) {
        sv->cFlags &= ~CFLAG_PORTAMENTO;
      } else {
        if ((sv->cFlags & CFLAG_PORTAMENTO) == 0) {
          synthInitPortamento(sv);
        }
        sv->cFlags |= CFLAG_PORTAMENTO;
      }
      sv->cFlags |= 0x1000;
      sv->portLastCtrlState = portamento;
    }
  }
  ccents = apply_portamento(sv, ccents, lowDeltaTime);
  if ((sv->cFlags & CFLAG_PITCH_ADSR) != 0) {
    ccents += sv->pitchAdsrRange * (sv->adsr.volume >> 16) >> 7;
  }
  cpitch = convert_cents(sv, ccents);
  cpitch += sv->sweepOff[0] + sv->sweepOff[1];
  cpitch = ((cpitch >> 16) * inpGetDoppler(sv)) >> 13;
  sv->currentPitch = cpitch;
  hwSetPitch(i, cpitch);
  synthAddJob(sv, 0, 0xf00);
end:
  UpdateTimeMIDICtrl(sv);
}

static void ZeroOffsetHandler(u32 i) {
  SYNTH_VOICE *sv;
  unsigned long lowDeltaTime;
  u16 Modulation;
  f32 vol;
  f32 auxa;
  f32 auxb;
  f32 f;
  f32 voiceVol;
  unsigned long volUpdate;
  f32 lfo;
  f32 scale;
  f32 mscale;
  signed long pan;
  f32 preVol;
  f32 postVol;
  sv = &synthVoice[i];
  if (!hwIsActive(i) && sv->macro == NULL) {
    goto end;
  }
  lowDeltaTime = synthRealTime - sv->lastZeroCallTime;
  sv->lastZeroCallTime = synthRealTime;
  if ((sv->cFlags & CFLAG_ENVELOPE) != 0) {
    sv->envCurrent += sv->envDelta * (lowDeltaTime >> 8);
    if (sv->envDelta < 0) {
      if ((signed long)sv->envTarget >= (signed long)sv->envCurrent) {
        sv->envCurrent = sv->envTarget;
        sv->cFlags &= ~CFLAG_ENVELOPE;
      }
    } else if ((signed long)sv->envTarget <= (signed long)sv->envCurrent) {
      sv->envCurrent = sv->envTarget;
      sv->cFlags &= ~CFLAG_ENVELOPE;
    }
    sv->soundVolume = sv->envCurrent;
    volUpdate = TRUE;
  } else {
    volUpdate = (sv->cFlags & CFLAG_VOLUME_UPDATE) != 0;
  }
  sv->cFlags &= ~CFLAG_VOLUME_UPDATE;
  f = synthMasterFader[sv->vGroup].pauseVol * synthMasterFader[sv->vGroup].volume *
      synthMasterFader[sv->isFX ? 22 : 21].volume;
  if (sv->track != 0xff) {
    vol = f * (f32)synthTrackVolume[sv->track] * (1.f / 127.f);
  } else {
    vol = f;
  }
  if (vol != sv->lastVolumeScale) {
    sv->lastVolumeScale = vol;
    volUpdate = TRUE;
  }
  voiceVol = (f32)sv->soundVolume * (1.f / (8192.f * 1016.f));
  if ((sv->tremoloDepth | sv->tremoloModScale) != 0) {
    Modulation = inpGetModulation(sv);
    lfo = (f32)(8192 - ((8192 - ((s16)inpGetTremolo(sv) - 8192)) >> 1)) * (1.f / 8192.f);
    mscale = 1.f - (f32)Modulation * (4096 - sv->tremoloModScale) * 1.490207e-08f;
    scale = (f32)sv->tremoloDepth * mscale * (1.f / 4096.f);
    if (sv->tremoloScale < scale) {
      if ((sv->tremoloScale += 0.2f) > scale) {
        sv->tremoloScale = scale;
      }
    } else if (sv->tremoloScale > scale) {
      if ((sv->tremoloScale -= 0.2f) < scale) {
        sv->tremoloScale = scale;
      }
    }
    voiceVol *= 1.f - lfo * (1.f - sv->tremoloScale);
    volUpdate = TRUE;
  }
  if ((synthFlags & SYNTH_FLAG_MONO) == 0) {
    if ((sv->cFlags & CFLAG_PAN_UPDATE) != 0 || (sv->midiDirtyFlags & 0x6) != 0) {
      sv->cFlags &= ~CFLAG_PAN_UPDATE;
      pan = sv->panCurrent[0] + (inpGetPanning(sv) - 8192) * 0x200;
      sv->lastPan = CLAMP_INV(pan, 0, 0x7f0000);
      if ((synthFlags & SYNTH_FLAG_SURROUND) != 0) {
        if ((sv->lastSurroundPan = sv->panCurrent[1] + inpGetSurPanning(sv) * 512) > 0x7f0000) {
          sv->lastSurroundPan = 0x7f0000;
        }
      } else {
        sv->lastSurroundPan = 0;
      }
      volUpdate = TRUE;
    } else if ((synthFlags & SYNTH_FLAG_SURROUND) == 0) {
      sv->lastSurroundPan = 0;
    }
  } else {
    sv->lastPan = 0x400000;
    sv->lastSurroundPan = 0;
    volUpdate |= (sv->cFlags & CFLAG_PAN_UPDATE) != 0;
    sv->cFlags &= ~CFLAG_PAN_UPDATE;
  }
  if (volUpdate || (sv->midiDirtyFlags & 0xf01) != 0) {
    preVol = voiceVol;
    postVol = voiceVol * vol * (f32)inpGetVolume(sv) * (1.f / 16383.f);
    auxa = ((f32)sv->reverbOffset * (1.f / 127.f)) +
           ((preVol * (f32)inpGetPreAuxA(sv) * (1.f / 16383.f)) +
            ((f32)sv->reverbScale *
             (postVol * (f32)inpGetReverb(sv) * (1.f / 16383.f)) * (1.f / 127.f)));
    auxb = (preVol * (f32)inpGetPreAuxB(sv) * (1.f / 16383.f)) +
           (postVol * (f32)inpGetPostAuxB(sv) * (1.f / 16383.f));
    sv->currentOutputVolume = (u16)(postVol * 32767.f);
    hwSetVolume(i, sv->volumeTable, postVol, sv->lastPan, sv->lastSurroundPan, auxa, auxb);
  }
  if (sv->priorityTime != 0) {
    if ((signed long)(sv->priorityTime -= sv->ageSpeed * lowDeltaTime) < 0) {
      sv->priorityTime = 0;
    }
    hwSetPriority(i, sv->priority << 24 | sv->priorityTime >> 15);
  }
  synthAddJob(sv, SYNTH_JOBTYPE_ZERO, (5 - hwGetTimeOffset()) * 256);
end:
  UpdateTimeMIDICtrl(sv);
}

#pragma force_active reset

static inline void HandleVoices(void) {
 SYNTH_JOB **table = synthJobTable[synthJobTableIndex];
 HandleJobQueue(&table[0], LowPrecisionHandler);
 HandleJobQueue(&table[1], EventHandler);
 HandleJobQueue(&table[2], ZeroOffsetHandler);
 synthJobTableIndex = (synthJobTableIndex + 1) & 31;
}

void synthHandle(unsigned long deltaTime) {
  unsigned long i;
  unsigned long s;
  SYNTH_MASTER_FADER *smf;
  unsigned long testFlag;
  if (synthInfo.numSamples == 0) {
    return;
  }
  macHandle(deltaTime);
  HandleVoices();
  if (hwGetTimeOffset() == 0) {
    if ((synthMasterFaderActiveFlags | synthMasterFaderPauseActiveFlags) != 0) {
      for (i = 0, smf = synthMasterFader, testFlag = 1; i < 32; ++i, ++smf, testFlag <<= 1) {
        if ((synthMasterFaderActiveFlags & testFlag) != 0) {
          smf->volume = smf->target - smf->time * (smf->target - smf->start);
          if ((smf->time -= smf->deltaTime) <= 0.f) {
            smf->volume = smf->target;
            HandleFaderTermination(smf);
            if ((synthMasterFaderActiveFlags &= ~testFlag) == 0 &&
                synthMasterFaderPauseActiveFlags == 0) {
              break;
            }
          }
        }
        if ((synthMasterFaderPauseActiveFlags & testFlag) != 0) {
          smf->pauseVol = smf->pauseTarget - smf->pauseTime * (smf->pauseTarget - smf->pauseStart);
          if ((smf->pauseTime -= smf->pauseDeltaTime) <= 0.f) {
            smf->pauseVol = smf->pauseTarget;
            if ((synthMasterFaderPauseActiveFlags &= ~testFlag) == 0 &&
                synthMasterFaderActiveFlags == 0) {
              break;
            }
          }
        }
      }
    }
    for (s = 0; s < SND_STUDIO_MAXNUM; ++s) {
      if (synthAuxAMIDI[s] != 0xff) {
        SND_AUX_INFO info;
        for (i = 0; i < SND_AUX_NUMPARAMETERS; ++i) {
          info.data.parameterUpdate.para[i] =
              inpGetAuxA(s, i, synthAuxAMIDI[s], synthAuxAMIDISet[s]);
        }
        synthAuxACallback[s](SND_AUX_REASON_PARAMETERUPDATE, &info, synthAuxAUser[s]);
      }
      if (synthAuxBMIDI[s] != 0xff) {
        SND_AUX_INFO info;
        for (i = 0; i < SND_AUX_NUMPARAMETERS; ++i) {
          info.data.parameterUpdate.para[i] =
              inpGetAuxB(s, i, synthAuxBMIDI[s], synthAuxBMIDISet[s]);
        }
        synthAuxBCallback[s](SND_AUX_REASON_PARAMETERUPDATE, &info, synthAuxBUser[s]);
      }
    }
  }
  hwFrameDone();
  synthRealTime += deltaTime;
}

void synthVolume(u8 volume, u16 time, u8 vGroup, u8 seqMode, unsigned long seqId) {
  unsigned long ltime;
  unsigned long i;
  u8 type;
  SYNTH_MASTER_FADER *smf;
  if ((ltime = time) != 0) {
    sndConvertMs(&ltime);
  }
  switch (vGroup) {
  case SND_ALL_VOLGROUPS:
    for (smf = synthMasterFader, i = 0; i < 32; ++i, smf++) {
      if (smf->type == 0 || smf->type == 1) {
        SetupFader(smf, volume, ltime, seqMode, SND_ID_ERROR);
        synthMasterFaderActiveFlags |= 1 << i;
      }
    }
    return;
  case SND_USERALL_VOLGROUPS:
    for (smf = synthMasterFader, i = 0; i < 32; ++i, smf++) {
      if (smf->type == 2 || smf->type == 3) {
        SetupFader(smf, volume, ltime, seqMode, SND_ID_ERROR);
        synthMasterFaderActiveFlags |= 1 << i;
      }
    }
    return;
  case SND_USERMUSIC_VOLGROUPS:
    type = 2;
    goto setup_type;
  case SND_USERFX_VOLGROUPS:
    type = 3;
    goto setup_type;
  case SND_MUSIC_VOLGROUPS:
    type = 0;
    goto setup_type;
  case SND_FX_VOLGROUPS:
    type = 1;
    goto setup_type;
  setup_type:
    for (smf = synthMasterFader, i = 0; i < 32; ++i, smf++) {
      if (smf->type == type) {
        SetupFader(smf, volume, ltime, seqMode, SND_ID_ERROR);
        synthMasterFaderActiveFlags |= 1 << i;
      }
    }
    return;
  default:
    SetupFader(&synthMasterFader[vGroup], volume, ltime, seqMode, seqId);
    synthMasterFaderActiveFlags |= 1 << vGroup;
    return;
  }
}

unsigned long synthFXStart(u16 fid,
                 u8 vol, u8 pan, u8 studio, unsigned long itd) {
  FX_TAB *fx;
  unsigned long v;
  v = 0xFFFFFFFF;
  if ((fx = dataGetFX(fid)) != NULL) {
    if (vol == 0xFF) {
      vol = fx->volume;
    }
    if (pan == 0xFF) {
      pan = fx->panning;
    }
    v = synthStartSound(fx->macro, fx->priority, fx->maxVoices,
                        fx->key | 0x80,
                        vol, pan, 0xFF, 0xFF, 0, 0, 0xFF, fx->vGroup, 0, studio, itd);
  }
  return v;
}

u8 synthITDDefault[8][2];
s32 synthGlobalVariable[16];
extern void (*synthMessageCallback)(u32 voice, s32 message);
extern void *memset(void *dest, int value, unsigned long length);
extern void inpInit(SYNTH_VOICE *voice);
extern void macInit(void);
extern void vidInit(void);
extern void synthInitAllocationAids(void);
extern s32 synthGlobalVariable[16];
extern void voiceInitLastStarted(void);
extern void hwSetMesgCallback(u32 (*callback)(u32 message, u32 voice));
static inline void synthInitJobQueue() {
  u8 i;
  for (i = 0; i < 32; ++i) {
    synthJobTable[i][0] = NULL;
    synthJobTable[i][1] = NULL;
    synthJobTable[i][2] = NULL;
  }
  synthJobTableIndex = 0;
}
void synthInit(unsigned long mixFrq, unsigned long numVoices) {
  unsigned long i;
  synthRealTime = 0;
  synthInfo.mixFrq = mixFrq;
  synthSetBpm(120, 255, 0);
  synthFlags = 0;
  synthMessageCallback = NULL;
  synthVoice = salMalloc(numVoices * sizeof(SYNTH_VOICE));

  memset(synthVoice, 0, numVoices * sizeof(SYNTH_VOICE));
  for (i = 0; i < numVoices; ++i) {
    synthVoice[i].id = 0xffffffff;
    synthVoice[i].cFlags = 0;
    synthVoice[i].priorityTime = 0;
    synthVoice[i].priority = 0;
    synthVoice[i].midi = 0xff;
    synthVoice[i].soundVolume = 0;
    synthVoice[i].volumeTable = 0;
    synthVoice[i].reverbScale = 128;
    synthVoice[i].reverbOffset = 0;
    synthVoice[i].panCurrent[0] = synthVoice[i].panTarget[0] = 0x400000;
    synthVoice[i].panCurrent[1] = synthVoice[i].panTarget[1] = 0;
    synthVoice[i].sweepOff[0] = 0;
    synthVoice[i].sweepOff[1] = 0;
    synthVoice[i].sweepNum[0] = 0;
    synthVoice[i].sweepNum[1] = 0;
    synthVoice[i].blocked = 0;
    synthVoice[i].vGroup = 23;
    synthVoice[i].keyGroup = 0;
    synthVoice[i].itdMode = 1;
    synthVoice[i].lfo[0].period = 0;
    synthVoice[i].lfo[0].value = 0;
    synthVoice[i].lfo[0].lastValue = 0x7fff;
    synthVoice[i].lfo[1].period = 0;
    synthVoice[i].lfo[1].value = 0;
    synthVoice[i].lfo[1].lastValue = 0x7fff;
    synthVoice[i].portTime = 25600;
    synthVoice[i].portamento_type = 0;
    synthVoice[i].studio = 0;
    synthVoice[i].jobs[0].voice_index = i;
    synthVoice[i].jobs[0].queue_index = 0xff;
    synthVoice[i].jobs[1].voice_index = i;
    synthVoice[i].jobs[1].queue_index = 0xff;
    synthVoice[i].jobs[2].voice_index = i;
    synthVoice[i].jobs[2].queue_index = 0xff;
  }
  for (i = 0; i < 32; ++i) {
    synthMasterFader[i].volume = 0.f;
    synthMasterFader[i].pauseVol = 1.f;
    synthMasterFader[i].type = 4;
  }
  synthMasterFaderActiveFlags = 0;
  synthMasterFaderPauseActiveFlags = 0;
  synthMasterFader[31].type = 1;
  for (i = 0; i < 8; ++i) {
    synthMasterFader[i + 23].type = 0;
  }
  synthMasterFader[21].volume = 1.f;
  synthMasterFader[22].volume = 1.f;
  inpInit(0);
  for (i = 0; i < SND_STUDIO_MAXNUM; ++i) {
    synthAuxACallback[i] = NULL;
    synthAuxAMIDI[i] = 0xff;
    synthAuxBCallback[i] = NULL;
    synthAuxBMIDI[i] = 0xff;
    synthITDDefault[i][1] = 0;
    synthITDDefault[i][0] = 0;
  }
  macInit();
  vidInit();
  synthInitAllocationAids();
  for (i = 0; i < 16; ++i) {
    synthGlobalVariable[i] = 0;
  }
  voiceInitLastStarted();
  synthInitJobQueue();
  hwSetMesgCallback(synthHWMessageHandler);
}
