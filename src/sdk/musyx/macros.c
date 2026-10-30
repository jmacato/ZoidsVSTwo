#include "musyx_synth.h"
#include "musyx_midictrl.h"
#include "musyx_hardware.h"

extern s32 synthGlobalVariable[16];
extern void macHandleActive(SYNTH_VOICE *voice);

extern SYNTH_VOICE *macActiveMacroRoot;
extern SYNTH_VOICE *macTimeQueueRoot;
extern u64 macRealTime;
extern void synthForceLowPrecisionUpdate(SYNTH_VOICE *voice);
extern void macMakeActive(SYNTH_VOICE *voice);
extern void macMakeInactive(SYNTH_VOICE *voice, s32 state);
extern u16 sndRand(void);
extern void sndConvertMs(u32 *time);
extern void sndConvertTicks(u32 *time, void *voice);
extern void synthInitPortamento(SYNTH_VOICE *voice);

typedef struct MSTEP { u32 para[2]; } MSTEP;
extern signed long varGet32(SYNTH_VOICE *voice, u32 type, u8 variable);
extern void varSet32(SYNTH_VOICE *voice, u32 type, u8 variable, s32 value);
static inline void varSet(SYNTH_VOICE *svoice, u32 ctrl, u8 index, s16 v) { varSet32(svoice, ctrl, index, v); }
#pragma auto_inline on
signed long varGet32(SYNTH_VOICE *voice, u32 type, u8 variable)
{
    if (type != 0)
        return inpGetExCtrl(voice, variable);
    variable &= 31;
    return variable < 16 ? voice->variable[variable] : synthGlobalVariable[variable - 16];
}

#pragma force_active on
s16 varGet(SYNTH_VOICE *voice, u32 type, u8 variable)
{
    return varGet32(voice, type, variable);
}

static void mcmdVarCalculation(SYNTH_VOICE *svoice, MSTEP *cstep, u8 op) {
  s16 s1;
  s16 s2;
  s32 t;
  s1 = varGet(svoice, (u8)(cstep->para[0] >> 24), cstep->para[1]);
  if (op == 4) {
    s2 = cstep->para[1] >> 8;
  } else {
    s2 = varGet(svoice, (u8)(cstep->para[1] >> 8), cstep->para[1] >> 16);
  }
  switch (op) {
  case 4:
  case 0:
    t = (s1 + s2);
    break;
  case 1:
    t = (s1 - s2);
    break;
  case 2:
    t = (s1 * s2);
    break;
  case 3:
    t = s2 != 0 ? (s1 / s2) : 0;
    break;
  }
  varSet(svoice, (u8)(cstep->para[0] >> 8), (u8)(cstep->para[0] >> 0x10),
         (t < -0x8000 ? -0x8000
          : t > 0x7FFF ? 0x7FFF
                       : t));
}
#pragma force_active reset

void varSet32(SYNTH_VOICE *voice, u32 type, u8 variable, s32 value)
{
    if (type) {
        inpSetExCtrl(voice, variable, (s16)value);
    } else {
        variable &= 31;
        if (variable < 16)
            voice->variable[variable] = value;
        else
            synthGlobalVariable[variable - 16] = value;
    }
}

#pragma auto_inline off

void macSetExternalKeyoff(SYNTH_VOICE *voice)
{
    voice->cFlags |= CFLAG_EXTERNAL_KEYOFF;
    if (voice->macro != 0) {
        if (!(voice->cFlags & CFLAG_PEDAL)) {
            u32 trapped;
            if (voice->trapEnabled[0] && voice->trapMacro[0]) {
                voice->macroStep = voice->trapStep[0];
                voice->macro = voice->trapMacro[0];
                voice->trapMacro[0] = 0;
                macMakeActive(voice);
                trapped = 1;
            } else {
                trapped = 0;
            }
            if (!trapped && (voice->cFlags & CFLAG_WAIT_KEYOFF))
                macMakeActive(voice);
        } else {
            voice->cFlags |= CFLAG_DEFERRED_KEYOFF;
        }
    }
}
void macSetPedalState(SYNTH_VOICE *voice, u32 state)
{
    if (state) {
        voice->cFlags |= CFLAG_PEDAL;
    } else {
        if (voice->macro && (voice->cFlags & CFLAG_DEFERRED_KEYOFF)) {
            u32 trapped;
            if (voice->trapEnabled[0] && voice->trapMacro[0]) {
                voice->macroStep = voice->trapStep[0];
                voice->macro = voice->trapMacro[0];
                voice->trapMacro[0] = 0;
                macMakeActive(voice);
                trapped = 1;
            } else {
                trapped = 0;
            }
            if (!trapped && (voice->cFlags & CFLAG_WAIT_KEYOFF))
                macMakeActive(voice);
        }
        voice->cFlags &= ~(CFLAG_DEFERRED_KEYOFF | CFLAG_PEDAL);
    }
}

void macInit(void)
{
    u32 index;
    macActiveMacroRoot = 0;
    macTimeQueueRoot = 0;
    macRealTime = 0;
    for (index = 0; index < synthInfo.voice_count; index++) {
        synthVoice[index].macro = 0;
        synthVoice[index].macroState = 2;
        synthVoice[index].loop = 0;
    }
}
void macSampleEndNotify(SYNTH_VOICE *voice)
{
    if (voice->macroState == 1) {
        u32 trapped;
        if (voice->trapEnabled[0] && voice->trapMacro[1]) {
            voice->macroStep = voice->trapStep[1];
            voice->macro = voice->trapMacro[1];
            voice->trapMacro[1] = 0;
            macMakeActive(voice);
            trapped = 1;
        } else {
            trapped = 0;
        }
        if (!trapped && (voice->cFlags & CFLAG_WAIT_SAMPLE_END))
            macMakeActive(voice);
    }
}
#pragma force_active on

static void TimeQueueAdd(SYNTH_VOICE *voice)
{
    SYNTH_VOICE *next = macTimeQueueRoot;
    SYNTH_VOICE *prev = 0;
    while (next && next->waitTime < voice->waitTime) {
        prev = next;
        next = next->timeNext;
    }
    if (!next) {
        if (!prev) {
            macTimeQueueRoot = voice;
            voice->timeNext = 0;
            voice->timePrev = 0;
        } else {
            prev->timeNext = voice;
            voice->timePrev = prev;
            voice->timeNext = 0;
        }
    } else {
        voice->timeNext = next;
        if ((voice->timePrev = next->timePrev) != 0)
            next->timePrev->timeNext = voice;
        else
            macTimeQueueRoot = voice;
        next->timePrev = voice;
    }
}
#pragma force_active reset

void macHandle(u32 delta)
{
    SYNTH_VOICE *voice = macTimeQueueRoot;
    while (voice != 0 && voice->waitTime <= macRealTime) {
        u64 time = voice->waitTime;
        SYNTH_VOICE *next = voice->timeNext;
        macMakeActive(voice);
        voice->macroTime = time;
        voice = next;
    }
    voice = macActiveMacroRoot;
    while (voice != 0) {
        u32 enabled = voice->trapEnabled[0] ? voice->trapMacro[1] != 0 : 0;
        if (enabled && !(voice->cFlags & CFLAG_HW_START_PENDING) && !hwIsActive((u8)voice->id)) {
            if (voice->trapEnabled[0] && voice->trapMacro[1]) {
                voice->macroStep = voice->trapStep[1];
                voice->macro = voice->trapMacro[1];
                voice->trapMacro[1] = 0;
                macMakeActive(voice);
            }
        }
        macHandleActive(voice);
        voice = voice->macroNext;
    }
    macRealTime += delta;
}

u32 mcmdWait(SYNTH_VOICE *voice, const u32 *step)
{
    u32 time;
    if ((time = step[1] >> 16) != 0) {
        u32 milliseconds;
        if ((step[0] >> 8) & 1) {
            if (voice->cFlags & CFLAG_EXTERNAL_KEYOFF) {
                if (!(voice->cFlags & CFLAG_PEDAL))
                    return 0;
                voice->cFlags |= CFLAG_DEFERRED_KEYOFF;
            }
            voice->cFlags |= CFLAG_WAIT_KEYOFF;
        } else {
            voice->cFlags &= ~CFLAG_WAIT_KEYOFF;
        }
        if ((step[0] >> 24) & 1) {
            if (!(voice->cFlags & CFLAG_HW_START_PENDING) && !hwIsActive((u8)voice->id))
                return 0;
            voice->cFlags |= CFLAG_WAIT_SAMPLE_END;
        } else {
            voice->cFlags &= ~CFLAG_WAIT_SAMPLE_END;
        }
        if ((step[0] >> 16) & 1)
            time = sndRand() % time;
        if (time != 0xFFFF) {
            if ((milliseconds = ((step[1] >> 8) & 1) != 0) != 0)
                sndConvertMs((u32 *)&time);
            else
                sndConvertTicks(&time, voice);
            if (milliseconds) {
                if (step[1] & 1)
                    voice->waitTime = voice->macStartTime + time;
                else
                    voice->waitTime = macRealTime + time;
            } else {
                if (step[1] & 1)
                    voice->waitTime = time;
                else
                    voice->waitTime = voice->macroTime + time;
            }
            if (voice->waitTime <= macRealTime) {
                voice->macroTime = voice->waitTime;
                voice->waitTime = 0;
            }
        } else {
            voice->waitTime = 0xFFFFFFFFFFFFFFFFULL;
        }
        if (voice->waitTime != 0) {
            if (voice->waitTime != 0xFFFFFFFFFFFFFFFFULL)
                TimeQueueAdd(voice);
            macMakeInactive(voice, 1);
            return 1;
        }
    }
    return 0;
}

void DoPanningSetup(SYNTH_VOICE *voice, const u32 *step, u8 lane)
{
    s32 time = step[0] >> 16;
    s32 change;
    voice->panTime[lane] = time;
    sndConvertMs(&voice->panTime[lane]);
    change = (s8)step[1] * 0x10000;
    voice->panCurrent[lane] = ((step[0] >> 8) & 0xFF) << 16;
    voice->panTarget[lane] = voice->panCurrent[lane] + change;
    if (voice->panTime[lane])
        voice->panDelta[lane] = change / time;
    else
        voice->panDelta[lane] = change;
    voice->cFlags |= CFLAG_PAN_UPDATE;
}

void mcmdLoop(SYNTH_VOICE *voice, const u32 *step)
{
    if (voice->loop == 0) {
        if ((step[0] >> 16) & 1)
            voice->loop = sndRand() % (s32)(step[1] >> 16);
        else
            voice->loop = step[1] >> 16;
        if (voice->loop != 0xFFFF)
            voice->loop++;
        else
            goto repeat;
    } else if (voice->loop == 0xFFFF) {
        goto repeat;
    }
    if (--voice->loop == 0)
        return;
repeat:
    if (((step[0] >> 8) & 1) && (voice->cFlags & (CFLAG_PEDAL | CFLAG_EXTERNAL_KEYOFF)) == CFLAG_EXTERNAL_KEYOFF) {
        voice->loop = 0;
        return;
    }
    if (((step[0] >> 24) & 1) && !(voice->cFlags & CFLAG_HW_START_PENDING) && !hwIsActive((u8)voice->id)) {
        voice->loop = 0;
        return;
    }
    voice->macroStep = (MSTEP *)voice->macro + (step[1] & 0xFFFF);
}

void mcmdPortamento(SYNTH_VOICE *voice, const u32 *step)
{
    u32 time;
    voice->portamento_type = step[0] >> 16;
    time = step[1] >> 16;
    if ((step[1] >> 8) & 1)
        sndConvertMs((u32 *)&time);
    else
        sndConvertTicks(&time, voice);
    voice->portDuration = time;
    switch ((step[0] >> 8) & 0xFF) {
    case 0:
        if (voice->midi != 0xFF)
            inpSetMidiCtrl(SND_MIDICTRL_PORTAMENTO, voice->midi, voice->midiSet, 0);
        voice->cFlags &= ~CFLAG_PORTAMENTO;
        break;
    case 1:
        if (voice->midi != 0xFF)
            inpSetMidiCtrl(SND_MIDICTRL_PORTAMENTO, voice->midi, voice->midiSet, 127);
    enable:
        if (!(voice->cFlags & CFLAG_PORTAMENTO))
            synthInitPortamento(voice);
        voice->cFlags |= CFLAG_PORTAMENTO;
        break;
    case 2:
        if (voice->midi != 0xFF && inpGetMidiCtrl(SND_MIDICTRL_PORTAMENTO, voice->midi, voice->midiSet) > 0x1F80)
            goto enable;
        break;
    }
}

void mcmdVibrato(SYNTH_VOICE *voice, const u32 *step)
{
    u32 time;
    s8 key;
    s8 cents;
    if ((step[0] >> 24) & 3)
        voice->cFlags |= CFLAG_VIBRATO_MODWHEEL;
    else
        voice->cFlags &= ~CFLAG_VIBRATO_MODWHEEL;
    time = step[1] >> 16;
    if ((step[1] >> 8) & 1)
        sndConvertMs((u32 *)&time);
    else
        sndConvertTicks(&time, voice);
    if (time) {
        voice->cFlags |= CFLAG_VIBRATO;
        voice->vibratoPeriod = time;
        key = step[0] >> 8;
        cents = step[0] >> 16;
        if (key < 0) {
            if (cents < 0)
                voice->vibratoCentRange = -cents;
            else
                voice->vibratoCentRange = cents;
            voice->vibratoKeyRange = -key;
            voice->vibratoTime = voice->vibratoPeriod >> 1;
        } else {
            if (cents < 0) {
                if (key == 0) {
                    voice->vibratoCentRange = -cents;
                    voice->vibratoTime = voice->vibratoPeriod >> 1;
                } else {
                    voice->vibratoCentRange = 100 - cents;
                    voice->vibratoTime = 0;
                    key--;
                }
            } else {
                voice->vibratoCentRange = cents;
                voice->vibratoTime = 0;
            }
            voice->vibratoKeyRange = key;
        }
    } else {
        voice->cFlags &= ~CFLAG_VIBRATO;
    }
}

#define NULL 0

#pragma auto_inline on
extern u32 macStart(u16 id, u8 priority, u8 maxVoices, u16 allocationId, u8 key, u8 volume, u8 pan, u8 midi, u8 set, u8 section, u16 step, u16 track, u8 newVID, u8 group, u8 studio, u32 itd);
extern void *dataGetCurve(u16 id);
extern u32 sndConvert2Ms(u32 time);
extern void synthFXCloneMidiSetup(SYNTH_VOICE *dest, SYNTH_VOICE *source);
extern u32 voiceIsLastStarted(SYNTH_VOICE *voice);
extern void inpAddCtrl(CTRL_DEST *dest, u8 control, s32 scale, u8 combination, u32 variableSource);
extern void inpSetGlobalMIDIDirtyFlag(u8 midi, u8 set, u32 flags);
extern void voiceKill(u32 index);

static inline u32 mcmdSetKey(SYNTH_VOICE *svoice, MSTEP *cstep) {
  svoice->curNote = (u8)(cstep->para[0] >> 8) & 0x7f;
  svoice->curDetune = (s8)(cstep->para[0] >> 0x10);
  if (voiceIsLastStarted(svoice) != 0) {
    inpSetMidiLastNote(svoice->midi, svoice->midiSet, svoice->curNote & 0xff);
  }
  cstep->para[0] = 4;
  return mcmdWait(svoice, cstep->para);
}
#pragma force_active on
static void mcmdPlayMacro(SYNTH_VOICE *svoice, MSTEP *cstep) {
  s32 key;
  u32 new_child;
  key = ((u32)svoice->orgNote + (s8)(u8)(cstep->para[0] >> 8));
  key = (key < 0) ? 0 : key > 0x7f ? 0x7f : key;
  if (svoice->isFX != 0) {
    key |= 0x80;
  }
  svoice->blocked = 1;
  new_child = macStart((u16)(cstep->para[0] >> 0x10), (u8)(cstep->para[1] >> 0x10),
                       (u8)(cstep->para[1] >> 0x18), svoice->allocId, key,
                       (u8)(svoice->soundVolume >> 0x10), (u8)(svoice->panCurrent[0] >> 0x10), svoice->midi,
                       svoice->midiSet, svoice->section, (u16)cstep->para[1], (u16)svoice->track, 0,
                       svoice->vGroup, svoice->studio, svoice->itdMode == 0);
  svoice->blocked = 0;
  if (new_child != 0xFFFFFFFF) {
    svoice->lastVID = synthVoice[(u8)new_child].vidList->vid;
    synthVoice[(u8)new_child].prevID = svoice->id;
    if (svoice->nextID != -1) {
      synthVoice[(u8)new_child].nextID = svoice->nextID;
      synthVoice[(u8)svoice->nextID].prevID = new_child;
    }
    svoice->nextID = new_child;
    if (svoice->isFX != 0) {
      synthFXCloneMidiSetup(&synthVoice[(u8)new_child], svoice);
    }
  } else {
    svoice->lastVID = 0xFFFFFFFF;
  }
}

static void DoSetPitch(SYNTH_VOICE *svoice) {
  u32 f;
  u32 of;
  u32 i;
  u32 frq;
  u32 ofrq;
  u32 no;
  s32 key;
  s32 oKey;
  static u16 kf[13] = {
      4096, 4339, 4597, 4871, 5160, 5467, 5792, 6137, 6502, 6888, 7298, 7732, 8192,
  };
  frq = svoice->playbackFrequency & 0xFFFFFF;
  ofrq = svoice->sampleInfo & 0xFFFFFF;
  if (ofrq == frq) {
    svoice->curNote = (u8)(svoice->sampleInfo >> 24);
    svoice->curDetune = 0;
  } else if (ofrq < frq) {
    f = (frq << 12) / ofrq;
    of = f >> 12;
    for (no = 0; no < 11; no++) {
      if (of < (1 << (no + 1))) {
        break;
      }
    }
    f /= (1 << no);
    for (i = 11;; i--) {
      if (f > kf[i]) {
        break;
      }
    }
    svoice->curNote = (svoice->sampleInfo >> 24) + (no * 12) + i;
    svoice->curDetune = ((f - kf[i]) * 100) / (kf[i + 1] - kf[i]);
  } else {
    f = (ofrq << 12) / frq;
    of = f >> 12;
    for (no = 0; no < 11; no++) {
      if (of < (1 << (no + 1))) {
        break;
      }
    }
    f /= (1 << no);
    for (i = 11;; i--) {
      if (f > kf[i]) {
        break;
      }
    }
    if ((key = i + (no * 12)) > (oKey = (svoice->sampleInfo >> 24))) {
      svoice->curNote = svoice->curDetune = 0;
    } else {
      svoice->curNote = (oKey & 0xFF) - key;
      svoice->curDetune = ((kf[i] - f) * 100) / (kf[i + 1] - kf[i]);
    }
  }
}

static u32 TranslateVolume(u32 volume, u16 curve) {
  u8 *ptr;
  u32 vlow;
  u32 vhigh;
  s32 d;
  if (curve != 0xFFFF) {
    if ((ptr = (u8 *)dataGetCurve(curve))) {
      vhigh = (volume >> 16) & 0xFFFF;
      vlow = volume & 0xFFFF;
      if (vhigh < 0x7f) {
        d = vlow * (ptr[vhigh + 1] - ptr[vhigh]);
        volume = d + ((u32)ptr[vhigh] << 16);
      } else {
        volume = ptr[vhigh] << 16;
      }
    }
  }
  return volume;
}

static void DoEnvelopeCalculation(SYNTH_VOICE *svoice, MSTEP *cstep, s32 start_vol) {
  u32 tvol;
  u32 time;
  s32 mstime;
  u16 curve;
  time = (u16)(cstep->para[1] >> 16);
  if ((u8)(cstep->para[1] >> 8) & 1) {
    sndConvertMs((u32 *)&time);
  } else {
    sndConvertTicks(&time, svoice);
  }
  mstime = sndConvert2Ms(time);
  if (mstime == 0) {
    mstime = 1;
  }
  tvol = (svoice->soundVolume * (u8)(cstep->para[0] >> 8) >> 7);
  tvol += (u8)(cstep->para[0] >> 16) << 16;
  if (tvol > 0x7f0000) {
    tvol = 0x7f0000;
  }
  curve = (u16)(u8)(cstep->para[0] >> 0x18);
  curve |= (((u16)(u8)cstep->para[1]) << 8);
  tvol = TranslateVolume(tvol, curve);
  svoice->envTarget = tvol;
  svoice->envCurrent = start_vol;
  svoice->envDelta = (s32)(tvol - start_vol) / mstime;
  svoice->soundVolume = start_vol;
  svoice->cFlags |= CFLAG_ENVELOPE;
}

static void mcmdRandomKey(SYNTH_VOICE *svoice , MSTEP *cstep ) {
  u8 k1;
  u8 k2;
  u8 t;
  long i1;
  long i2;
  u8 detune;
  if (!(u8)(cstep->para[1] >> 8)) {
    k1 = (cstep->para[0] >> 8);
    k2 = (cstep->para[0] >> 24);
    if (k1 > k2) {
      t = k1;
      k1 = k2;
      k2 = t;
    }
  } else {
    i1 = svoice->curNote - (u8)(cstep->para[0] >> 8);
    i2 = svoice->curNote + (u8)(cstep->para[0] >> 24);
    k1 = i1 < 0 ? 0 : i1 > 127 ? 127 : i1;
    k2 = i2 < 0 ? 0 : i2 > 127 ? 127 : i2;
  }
  if ((u8)cstep->para[1]) {
    detune = (sndRand() % 201) - 100;
  } else {
    detune = (u8)(cstep->para[0] >> 16);
  }
  cstep->para[0] = ((u8)detune << 16) | 0x19 | ((k1 + (sndRand() % ((k2 - k1) + 1))) * 0x100);
  cstep->para[1] = 0;
  mcmdSetKey(svoice, cstep);
}

static void SelectSource(SYNTH_VOICE *svoice, CTRL_DEST *dest, MSTEP *cstep, u64 tstflag,
                         u32 dirtyFlag) {
  u8 comb;
  long scale;
  if (!(svoice->cFlags & tstflag)) {
    comb = 0;
    svoice->cFlags |= tstflag;
  } else {
    comb = (u8)cstep->para[1];
  }
  scale = ((s16)(cstep->para[0] >> 16) << 16) / 100;
  if (scale < 0) {
    scale -= ((s8)(cstep->para[1] >> 0x10) << 8) / 100;
  } else {
    scale += ((s8)(cstep->para[1] >> 0x10) << 8) / 100;
  }
  inpAddCtrl(dest, (u8)(cstep->para[0] >> 8), scale, comb, (u8)(cstep->para[1] >> 8) != 0);
  if ((dirtyFlag & 0x80000000) != 0) {
    inpSetGlobalMIDIDirtyFlag(svoice->midi, svoice->midiSet, dirtyFlag);
  } else {
    svoice->midiDirtyFlags |= dirtyFlag;
  }
}

static void mcmdSetKeyGroup(SYNTH_VOICE *svoice, MSTEP *cstep) {
  u32 i;
  u8 kg;
  u32 kill;
  svoice->keyGroup = 0;
  kg = (u8)(cstep->para[0] >> 8);
  kill = (u8)(cstep->para[0] >> 0x10) != 0;
  if (kg) {
    for (i = 0; i < synthInfo.voice_count; ++i) {
      if (synthVoice[i].macro != NULL && (synthVoice[i].cFlags & CFLAG_STARTING) == 0 &&
          kg == synthVoice[i].keyGroup) {
        if (!kill) {
          macSetExternalKeyoff(&synthVoice[i]);
        } else {
          voiceKill(i);
        }
      }
    }
    svoice->keyGroup = kg;
  }
}

#pragma auto_inline off

#pragma auto_inline on

#pragma force_active reset
extern u32 vidGetInternalId(u32 vid);
extern void (*synthMessageCallback)(u32 voice, s32 message);
static inline u32 ExecuteTrap(SYNTH_VOICE *svoice, u8 trapType) {
  if (svoice->trapEnabled[0] != 0 && svoice->trapMacro[trapType] != NULL) {
    svoice->macroStep = svoice->trapStep[trapType];
    svoice->macro = svoice->trapMacro[trapType];
    svoice->trapMacro[trapType] = NULL;
    macMakeActive(svoice);
    return 1;
  }
  return 0;
}
static inline u32 macPostMessage(u32 vid, s32 mesg) {
  SYNTH_VOICE *sv;
  if ((vid = vidGetInternalId(vid)) != -1 && (sv = &synthVoice[vid & 0xFF])->messageCount < 4) {
    ++sv->messageCount;
    sv->messageQueue[sv->messageWrite] = mesg;
    sv->messageWrite = (sv->messageWrite + 1) & 3;
    ExecuteTrap(sv, 2);
    return 1;
  }
  return 0;
}
#pragma force_active on
static void mcmdSendMessage(SYNTH_VOICE *svoice, MSTEP *cstep) {
  u8 i;
  signed long mesg;
  u16 macro;
  mesg = varGet32(svoice, 0, (u8)(cstep->para[1] >> 8));
  if (!(u8)(cstep->para[0] >> 8)) {
    macro = (u16)(cstep->para[0] >> 16);
    if (macro != 0xFFFF) {
      for (i = 0; i < synthInfo.voice_count; ++i) {
        if (synthVoice[i].macro != NULL && macro == synthVoice[i].macroId) {
          macPostMessage(synthVoice[i].vidList->vid, mesg);
        }
      }
    } else if (synthMessageCallback != NULL) {
      synthMessageCallback(svoice->vidList->vid, mesg);
    }
  } else {
    macPostMessage(varGet32(svoice, 0, (u8)cstep->para[1]), mesg);
  }
}

#pragma force_active reset

#define NULL 0
#define MAC_STATE_RUNNABLE 0
#define MAC_STATE_STOPPED 2
#pragma auto_inline on
#pragma force_active on
static void UnYieldMacro(SYNTH_VOICE *voice, u32 noUpdate) {
  if (voice->waitTime != 0) {
    if (voice->waitTime != -1) {
      if (voice->timePrev == NULL) {
        macTimeQueueRoot = voice->timeNext;
      } else {
        voice->timePrev->timeNext = voice->timeNext;
      }
      if (voice->timeNext) {
        voice->timeNext->timePrev = voice->timePrev;
      }
    }
    if (!noUpdate) {
      synthForceLowPrecisionUpdate(voice);
    }
    voice->waitTime = 0;
    voice->macroTime = macRealTime;
    voice->cFlags &= ~(CFLAG_WAIT_SAMPLE_END | CFLAG_WAIT_KEYOFF);
  }
}

inline void macMakeActive(SYNTH_VOICE *voice) {
  if (voice->macroState == MAC_STATE_RUNNABLE) {
    return;
  }
  UnYieldMacro(voice, 0);
  if ((voice->macroNext = macActiveMacroRoot)) {
    macActiveMacroRoot->macroPrev = voice;
  }
  voice->macroPrev = NULL;
  macActiveMacroRoot = voice;
  voice->macroState = MAC_STATE_RUNNABLE;
}

inline void macMakeInactive(SYNTH_VOICE *voice, s32 newState) {
  if (voice->macroState == newState) {
    return;
  }
  if (voice->macroState == MAC_STATE_RUNNABLE) {
    if (voice->macroPrev == NULL) {
      macActiveMacroRoot = voice->macroNext;
    } else {
      voice->macroPrev->macroNext = voice->macroNext;
    }
    if (voice->macroNext != NULL) {
      voice->macroNext->macroPrev = voice->macroPrev;
    }
  }
  if (newState == MAC_STATE_STOPPED) {
    UnYieldMacro(voice, 1);
  }
  voice->macroState = newState;
}
#pragma force_active reset
/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/synthmacros.c (CC0-1.0). */
#define NULL 0
#define MAC_STATE_STOPPED 2
extern void *dataGetMacro(u16 id);
extern u16 seqGetMIDIPriority(u8 set, u8 midi);
extern u32 voiceAllocate(u8 priority, u8 maxVoices, u16 allocationId, u8 isFX);
extern void vidRemoveVoiceReferences(SYNTH_VOICE *voice);
extern void voiceFree(SYNTH_VOICE *voice);
extern void inpResetMidiCtrl(u8 midi, u8 set, u32 coldReset);
extern void inpResetChannelDefaults(u8 midi, u8 set);
extern void voiceSetPriority(SYNTH_VOICE *voice, u8 priority);
extern u32 vidMakeNew(SYNTH_VOICE *voice, u32 master);
#pragma auto_inline on
u32 macStart(u16 macid, u8 priority, u8 maxVoices,
             u16 allocId,
             u8 key, u8 vol, u8 panning, u8 midi, u8 midiSet, u8 section, u16 step, u16 trackid,
             u8 new_vid, u8 vGroup, u8 studio, u32 itd) {
  u32 voice;
  u32 vid;
  MSTEP *addr;
  SYNTH_VOICE *svoice;
  u16 seqPrio;
  if ((addr = dataGetMacro(macid))) {
    if (!(key & 0x80) && (seqPrio = seqGetMIDIPriority(midiSet, midi)) != 0xFFFF) {
      priority = seqPrio;
    }
    if ((voice = voiceAllocate(priority, maxVoices, allocId, (key & 0x80) ? 1 : 0)) != -1) {
      svoice = &synthVoice[voice];
      vidRemoveVoiceReferences(svoice);
      macMakeInactive(svoice, MAC_STATE_STOPPED);
      svoice->cFlags = (svoice->cFlags & CFLAG_HW_KEYON) | CFLAG_STARTING;
      if (hwIsActive(voice)) {
        svoice->cFlags |= CFLAG_HW_BREAK;
      }
      svoice->waitTime = 0;
      if ((key & 0x80) != 0) {
        svoice->isFX = 01;
        key &= 0x7f;
        inpResetMidiCtrl(voice, 0xff, 1);
        inpResetChannelDefaults(voice, 0xff);
        svoice->setupMidi = voice;
        svoice->setupMidiSet = 0xff;
        svoice->setupSection = 0;
      } else {
        svoice->isFX = 0;
        svoice->setupMidi = midi;
        svoice->setupMidiSet = midiSet;
        svoice->setupSection = section;
      }
      svoice->macroId = macid;
      svoice->allocId = allocId;
      svoice->priorityTime = 0x75300000;
      svoice->ageSpeed = 0x400;
      svoice->macro = addr;
      svoice->macroStep = addr + step;
      svoice->orgNote = key;
      svoice->curNote = key;
      svoice->curDetune = 0;
      svoice->setupVolume = vol;
      svoice->setupPan = panning;
      svoice->setupTrack = trackid;
      svoice->callStackEntryNum = 0;
      svoice->callStackIndex = 0;
      svoice->nextID = -1;
      svoice->prevID = -1;
      svoice->lastVID = -1;
      svoice->setupVGroup = vGroup;
      svoice->setupStudio = studio;
      svoice->setupITD = itd != 0 ? 0 : 1;
      svoice->messageCount = svoice->messageRead = svoice->messageWrite = 0;
      svoice->id = voice | ((macid << 16) | (key << 8));
      voiceSetPriority(svoice, priority);
      if ((vid = vidMakeNew(svoice, new_vid)) != -1) {
        macMakeActive(svoice);
        return vid;
      }
      if (hwIsActive(voice)) {
        hwBreak(voice);
      }
      voiceFree(svoice);
    }
  }
  return -1;
}


#include "musyx_sample.h"
#define FLT_EPSILON 1.1920928955078125e-7f
#define FALSE 0
#define TRUE 1
extern s32 dataGetSample(u16 id, SAMPLE_RESULT *output);
extern void hwInitSamplePlayback(u32 voice, u16 sampleID, void *sample, u32 defaultADSR, u32 priority, u32 callbackValue, u32 setSRC, u8 itdMode);
extern void synthKeyStateUpdate(void *voice);
extern void hwSetADSR(u32 voice, void *adsr, u8 mode);
extern f32 dspDLSVolTab[];
extern u8 dspScale2IndexTab[];
extern u32 adsrConvertTimeCents(s32 time);
extern u32 adsrSetup(ADSR_VARS *adsr);

#pragma force_active on
static void mcmdStartSample(SYNTH_VOICE *svoice, MSTEP *cstep) {
  static SAMPLE_RESULT newsmp;
  u16 smp;
  smp = cstep->para[0] >> 8;
  if (dataGetSample(smp, &newsmp) != 0) {
    return;
  }
  switch ((u8)(cstep->para[0] >> 0x18)) {
  case 0:
    newsmp.offset = cstep->para[1];
    break;
  case 1:
    newsmp.offset = ((u8)(0x7f - (svoice->soundVolume >> 0x10)) * (u32)cstep->para[1]) / 0x7f;
    ;
    break;
  case 2:
    newsmp.offset = ((u8)((svoice->soundVolume >> 0x10)) * (u32)cstep->para[1]) / 0x7f;
    break;
  default:
    newsmp.offset = 0;
    break;
  }
  if (newsmp.offset >= newsmp.length) {
    newsmp.offset = newsmp.length - 1;
  }
  hwInitSamplePlayback(svoice->id & 0xFF, smp, &newsmp, (svoice->cFlags & CFLAG_ADSR_SET) == 0,
                       ((u32)svoice->priority << 24) | ((u32)svoice->priorityTime >> 15), svoice->id,
                       (svoice->cFlags & CFLAG_SRC_SET) == 0, svoice->itdMode);
  svoice->sampleInfo = newsmp.info;
  if (svoice->playbackFrequency != -1) {
    DoSetPitch(svoice);
  }
  svoice->cFlags |= CFLAG_HW_START_PENDING;
  synthKeyStateUpdate(svoice);
}

static void mcmdSetADSR(SYNTH_VOICE *svoice, MSTEP *cstep) {
  ADSR_INFO adsr;
  ADSR_INFO *adsr_ptr;
  s32 ascale;
  s32 dscale;
  f32 sScale;
  if ((adsr_ptr = dataGetCurve(cstep->para[0] >> 8)) != NULL) {
    if (!(u8)(cstep->para[0] >> 24)) {
      adsr.data.linear.atime = adsr_ptr->data.linear.atime >> 8 | adsr_ptr->data.linear.atime << 8;
      adsr.data.linear.dtime = adsr_ptr->data.linear.dtime >> 8 | adsr_ptr->data.linear.dtime << 8;
      adsr.data.linear.slevel = adsr_ptr->data.linear.slevel >> 8 | adsr_ptr->data.linear.slevel
                                                                        << 8;
      adsr.data.linear.rtime = adsr_ptr->data.linear.rtime >> 8 | adsr_ptr->data.linear.rtime << 8;
      hwSetADSR(svoice->id & 0xFF, &adsr, FALSE);
    } else {
      sScale =
          dspDLSVolTab[(u16)(adsr_ptr->data.dls.slevel >> 8 | adsr_ptr->data.dls.slevel << 8) >> 5];
      adsr.data.dls.atime =
          ((u8 *)&adsr_ptr->data.dls.atime)[0] << 0 | ((u8 *)&adsr_ptr->data.dls.atime)[1] << 8 |
          ((u8 *)&adsr_ptr->data.dls.atime)[2] << 16 | ((u8 *)&adsr_ptr->data.dls.atime)[3] << 24;
      adsr.data.dls.dtime =
          ((u8 *)&adsr_ptr->data.dls.dtime)[0] << 0 | ((u8 *)&adsr_ptr->data.dls.dtime)[1] << 8 |
          ((u8 *)&adsr_ptr->data.dls.dtime)[2] << 16 | ((u8 *)&adsr_ptr->data.dls.dtime)[3] << 24;
      adsr.data.dls.slevel = 4096.f * sScale;
      adsr.data.dls.rtime = adsr_ptr->data.dls.rtime >> 8 | adsr_ptr->data.dls.rtime << 8;
      ascale =
          ((u8 *)&adsr_ptr->data.dls.ascale)[0] << 0 | ((u8 *)&adsr_ptr->data.dls.ascale)[1] << 8 |
          ((u8 *)&adsr_ptr->data.dls.ascale)[2] << 16 | ((u8 *)&adsr_ptr->data.dls.ascale)[3] << 24;
      dscale =
          ((u8 *)&adsr_ptr->data.dls.dscale)[0] << 0 | ((u8 *)&adsr_ptr->data.dls.dscale)[1] << 8 |
          ((u8 *)&adsr_ptr->data.dls.dscale)[2] << 16 | ((u8 *)&adsr_ptr->data.dls.dscale)[3] << 24;
      if (ascale != 0x80000000) {
        adsr.data.dls.atime += (s32)(FLT_EPSILON * svoice->originalVolume * ascale);
      }
      if (dscale != 0x80000000) {
        adsr.data.dls.dtime += (s32)(0.0078125f * svoice->orgNote * dscale);
      }
      hwSetADSR(svoice->id & 0xFF, &adsr, TRUE);
    }
    svoice->cFlags |= CFLAG_ADSR_SET;
  }
}

static void mcmdSetPitchADSR(SYNTH_VOICE *svoice, MSTEP *cstep) {
  ADSR_INFO adsr;
  ADSR_INFO *adsr_ptr;
  u32 sl;
  s32 ascale;
  s32 dscale;
  if ((adsr_ptr = dataGetCurve((cstep->para[0] >> 8))) == NULL) {
    return;
  }
  svoice->pitchAdsrRange = ((s8)cstep->para[1] << 8);
  if (svoice->pitchAdsrRange >= 0) {
    svoice->pitchAdsrRange += ((s16)(s8)(cstep->para[1] >> 8) << 8) / 100;
  } else {
    svoice->pitchAdsrRange -= ((s16)(s8)(cstep->para[1] >> 8) << 8) / 100;
  }
  adsr.data.dls.atime =
      (((u8 *)&adsr_ptr->data.dls.atime)[0] << 0) | (((u8 *)&adsr_ptr->data.dls.atime)[1] << 8) |
      (((u8 *)&adsr_ptr->data.dls.atime)[2] << 16) | (((u8 *)&adsr_ptr->data.dls.atime)[3] << 24);
  adsr.data.dls.dtime =
      (((u8 *)&adsr_ptr->data.dls.dtime)[0] << 0) | (((u8 *)&adsr_ptr->data.dls.dtime)[1] << 8) |
      (((u8 *)&adsr_ptr->data.dls.dtime)[2] << 16) | (((u8 *)&adsr_ptr->data.dls.dtime)[3] << 24);
  adsr.data.dls.slevel = (adsr_ptr->data.dls.slevel >> 8) | (adsr_ptr->data.dls.slevel << 8);
  adsr.data.dls.rtime = (adsr_ptr->data.dls.rtime >> 8) | (adsr_ptr->data.dls.rtime << 8);
  ascale =
      (((u8 *)&adsr_ptr->data.dls.ascale)[0] << 0) | (((u8 *)&adsr_ptr->data.dls.ascale)[1] << 8) |
      (((u8 *)&adsr_ptr->data.dls.ascale)[2] << 16) | (((u8 *)&adsr_ptr->data.dls.ascale)[3] << 24);
  dscale =
      (((u8 *)&adsr_ptr->data.dls.dscale)[0] << 0) | (((u8 *)&adsr_ptr->data.dls.dscale)[1] << 8) |
      (((u8 *)&adsr_ptr->data.dls.dscale)[2] << 16) | (((u8 *)&adsr_ptr->data.dls.dscale)[3] << 24);
  if (ascale != 0x80000000) {
    adsr.data.dls.atime += (s32)((FLT_EPSILON * svoice->originalVolume) * (f32)ascale);
  }
  if (dscale != 0x80000000) {
    adsr.data.dls.dtime += (s32)((0.0078125f * svoice->orgNote) * (f32)dscale);
  }
  svoice->adsr.mode = ADSR_MODE_DLS;
  svoice->adsr.attackMode = 0;
  svoice->adsr.attackTime = adsrConvertTimeCents(adsr.data.dls.atime);
  svoice->adsr.decayTime = adsrConvertTimeCents(adsr.data.dls.dtime);
  if ((sl = adsr.data.dls.slevel >> 2) > 0x3ff) {
    sl = 0x3ff;
  }
  svoice->adsr.sustain = 193 - dspScale2IndexTab[sl];
  svoice->adsr.releaseTime = adsr.data.dls.rtime;
  adsrSetup(&svoice->adsr);
  svoice->cFlags |= CFLAG_PITCH_ADSR;
}
#pragma force_active reset

#pragma force_active reset
#pragma auto_inline on
typedef struct CHANNEL_DEFAULTS { u8 pBendRange; } CHANNEL_DEFAULTS;
static signed long midi2TimeTab[128] = {
    0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110,
    110, 120, 130, 140, 150, 160, 170, 190, 200, 220, 230, 250,
    270, 290, 310, 330, 350, 380, 410, 440, 470, 500, 540, 580,
    620, 660, 710, 760, 820, 880, 940, 1000, 1000, 1100, 1200, 1300,
    1400, 1500, 1600, 1700, 1800, 2000, 2100, 2300, 2400, 2600, 2800, 3000,
    3200, 3500, 3700, 4000, 4300, 4600, 4900, 5300, 5700, 6100, 6500, 7000,
    7500, 8100, 8600, 9300, 9900, 10000, 11000, 12000, 13000, 14000, 15000, 16000,
    17000, 18000, 19000, 21000, 22000, 24000, 26000, 28000, 30000, 32000, 34000, 37000,
    39000, 42000, 45000, 49000, 50000, 55000, 60000, 65000, 70000, 75000, 80000, 85000,
    90000, 95000, 100000, 105000, 110000, 115000, 120000, 125000, 130000, 135000, 140000, 145000,
    150000, 155000, 160000, 165000, 170000, 175000, 180000, 0,
};
extern u8 DebugMacroSteps;
extern void inpInit(SYNTH_VOICE *voice);
extern void voiceSetLastStarted(SYNTH_VOICE *voice);
extern CHANNEL_DEFAULTS *inpGetChannelDefaults(u8 midi, u8 set);
extern void *memset(void *dest, signed long value, unsigned long size);
extern void synthStartSynthJobHandling(SYNTH_VOICE *voice);
extern u16 inpGetModulation(SYNTH_VOICE *voice);
extern unsigned long hwFrq2Pitch(unsigned long frequency);
extern void hwKeyOff(unsigned long index);
extern void hwSetSRCType(unsigned long index, u8 type);
extern void hwSetPolyPhaseFilter(unsigned long index, u8 type);
extern CTRL_DEST inpAuxA[8][4], inpAuxB[8][4];

static inline int SendSingleKeyOff(unsigned long voiceid) {
  unsigned long i;
  if (voiceid != 0xFFFFFFFF) {
    i = voiceid & 0xFF;
    if (voiceid == synthVoice[i].id) {
      macSetExternalKeyoff(&synthVoice[i]);
      return 0;
    }
  }
  return -1;
}

static inline unsigned long mcmdWaitMs(SYNTH_VOICE *svoice, MSTEP *cstep) {
  ((u8 *)&cstep->para[1])[2] = 1;
  return mcmdWait(svoice, cstep->para);
}

static inline unsigned long mcmdEndOfMacro(SYNTH_VOICE *svoice) {
  vidRemoveVoiceReferences(svoice);
  voiceFree(svoice);
  return 1;
}

static inline unsigned long mcmdStop(SYNTH_VOICE *svoice) { return mcmdEndOfMacro(svoice); }

static inline unsigned long mcmdReturn(SYNTH_VOICE *svoice) {
  if (svoice->callStackEntryNum != 0) {
    svoice->macro = svoice->callStack[svoice->callStackIndex].macro;
    svoice->macroStep = svoice->callStack[svoice->callStackIndex].step;
    svoice->callStackIndex = (svoice->callStackIndex - 1) & 3;
    --svoice->callStackEntryNum;
  }
  return 0;
}

static inline void mcmdIfKey(SYNTH_VOICE *svoice, MSTEP *cstep) {
  MSTEP *addr;
  if (svoice->curNote < ((u8)(cstep->para[0] >> 8))) {
    return;
  }
  if ((addr = (MSTEP *)dataGetMacro((cstep->para[0] >> 0x10))) != NULL) {
    svoice->macro = addr;
    svoice->macroStep = addr + (u16)cstep->para[1];
  }
}

static inline void mcmdIfVelocity(SYNTH_VOICE *svoice, MSTEP *cstep) {
  MSTEP *addr;
  if (((u8)(svoice->soundVolume >> 0x10)) < (u8)(cstep->para[0] >> 8)) {
    return;
  }
  if ((addr = (MSTEP *)dataGetMacro(cstep->para[0] >> 0x10))) {
    svoice->macro = addr;
    svoice->macroStep = addr + (u16)cstep->para[1];
  }
}

static inline void mcmdIfModulation(SYNTH_VOICE *svoice, MSTEP *cstep) {
  MSTEP *addr;
  u8 mod;
  if (svoice->midi == 0xff) {
    return;
  }
  mod = inpGetModulation(svoice) >> 7;
  if (mod < (u8)(cstep->para[0] >> 8)) {
    return;
  }
  if ((addr = (MSTEP *)dataGetMacro(cstep->para[0] >> 0x10))) {
    svoice->macro = addr;
    svoice->macroStep = addr + (u16)(cstep->para[1]);
  }
}

static inline void mcmdIfRandom(SYNTH_VOICE *svoice, MSTEP *cstep) {
  MSTEP *addr;
  if ((u8)sndRand() < (u8)(cstep->para[0] >> 8)) {
    return;
  }
  if ((addr = (MSTEP *)dataGetMacro(cstep->para[0] >> 0x10))) {
    svoice->macro = addr;
    svoice->macroStep = addr + (u16)cstep->para[1];
  }
}

static inline unsigned long mcmdGoto(SYNTH_VOICE *svoice, MSTEP *cstep) {
  MSTEP *addr;
  if ((addr = (MSTEP *)dataGetMacro(cstep->para[0] >> 0x10)) != NULL) {
    svoice->macro = addr;
    svoice->macroStep = addr + (u16)cstep->para[1];
    return 0;
  }
  return mcmdEndOfMacro(svoice);
}

static inline unsigned long mcmdGosub(SYNTH_VOICE *svoice, MSTEP *cstep) {
  MSTEP *addr;
  if ((addr = (MSTEP *)dataGetMacro((u16)(cstep->para[0] >> 0x10))) != NULL) {
    svoice->callStackIndex = (svoice->callStackIndex + 1) & 3;
    svoice->callStack[svoice->callStackIndex].macro = svoice->macro;
    svoice->callStack[svoice->callStackIndex].step = svoice->macroStep;
    if (++svoice->callStackEntryNum > 4) {
      svoice->callStackEntryNum = 4;
    }
    svoice->macro = addr;
    svoice->macroStep = addr + (u16)cstep->para[1];
    return 0;
  }
  return mcmdEndOfMacro(svoice);
}

static inline void mcmdTrapEvent(SYNTH_VOICE *svoice, MSTEP *cstep) {
  MSTEP *addr;
  u8 t;
  if ((addr = (MSTEP *)dataGetMacro(cstep->para[0] >> 0x10)) != NULL) {
    t = (u8)(cstep->para[0] >> 8);
    svoice->trapMacro[t] = addr;
    svoice->trapStep[t] = addr + (u16)cstep->para[1];
    svoice->trapEnabled[0] = 1;
    if (t == 0 && (svoice->cFlags & (CFLAG_PEDAL | CFLAG_EXTERNAL_KEYOFF)) == (CFLAG_PEDAL | CFLAG_EXTERNAL_KEYOFF)) {
      svoice->cFlags |= CFLAG_DEFERRED_KEYOFF;
    }
  }
}

static inline void mcmdUntrapEvent(SYNTH_VOICE *svoice, MSTEP *cstep) {
  u8 i;
  svoice->trapMacro[(u8)(cstep->para[0] >> 8)] = 0;
  for (i = 0; i < 3; ++i) {
    if (svoice->trapMacro[i] != NULL) {
      return;
    }
  }
  svoice->trapEnabled[0] = 0;
}

static inline void mcmdSendKeyOff(SYNTH_VOICE *svoice, MSTEP *cstep) {
  unsigned long voiceid;
  unsigned long i;
  voiceid = (svoice->orgNote + (u8)(cstep->para[0] >> 8)) << 8;
  voiceid |= ((u16)(cstep->para[0] >> 16)) << 16;
  for (i = 0; i < synthInfo.voice_count; ++i) {
    if (synthVoice[i].id == (voiceid | i)) {
      SendSingleKeyOff(voiceid | i);
    }
  }
}

static inline void mcmdAddAgeCounter(SYNTH_VOICE *svoice, MSTEP *cstep) {
  s16 step;
  signed long age;
  step = (u16)(cstep->para[0] >> 16);
  age = (svoice->priorityTime >> 15) + step;
  if (age < 0) {
    svoice->priorityTime = 0;
  } else if (age > 0xFFFF) {
    svoice->priorityTime = 0x7fff8000;
  } else {
    svoice->priorityTime = age * 0x8000;
  }
  hwSetPriority(svoice->id & 0xFF, ((unsigned long)svoice->priority << 24) | ((unsigned long)svoice->priorityTime >> 15));
}

static inline void mcmdSetAgeCounter(SYNTH_VOICE *svoice, MSTEP *cstep) {
  svoice->priorityTime = (u16)(cstep->para[0] >> 0x10) << 0xf;
  hwSetPriority(svoice->id & 0xff, (unsigned long)svoice->priority << 0x18 | svoice->priorityTime >> 0xf);
}

static inline void mcmdSetAgeCounterSpeed(SYNTH_VOICE *svoice, MSTEP *cstep) {
  unsigned long time = cstep->para[1];
  if (time != 0) {
    svoice->ageSpeed = (svoice->priorityTime >> 8) / time;
  } else {
    svoice->ageSpeed = 0;
  }
}

static inline void mcmdSetAgeCounterByVolume(SYNTH_VOICE *svoice, MSTEP *cstep) {
  unsigned long age;
  age = (((u8)(svoice->soundVolume >> 16) * (u16)cstep->para[1]) >> 7) + (u16)(cstep->para[0] >> 16);
  svoice->priorityTime = age > 60000 ? 0x75300000 : age * 0x8000;
  hwSetPriority(svoice->id & 0xff, (unsigned long)svoice->priority << 0x18 | svoice->priorityTime >> 0xf);
}

static inline void mcmdAddPriority(SYNTH_VOICE *svoice, MSTEP *cstep) {
  s16 add;
  s16 prio;
  add = (u16)(cstep->para[0] >> 16);
  prio = svoice->priority + add;
  prio = (prio < 0) ? 0 : (prio > 0xFF) ? 0xFF : prio;
  voiceSetPriority(svoice, prio);
}

static inline void mcmdSetPriority(SYNTH_VOICE *svoice, MSTEP *cstep) {
  voiceSetPriority(svoice, cstep->para[0] >> 8);
}

static inline void mcmdSendFlag(MSTEP *cstep) {
  synthGlobalVariable[(u8)(cstep->para[0] >> 8)] = (u8)(cstep->para[0] >> 16);
}

static inline void mcmdSetPitchWheelRange(SYNTH_VOICE *svoice, MSTEP *cstep) {
  svoice->pbLowerKeyRange = (u8)(cstep->para[0] >> 0x10);
  svoice->pbUpperKeyRange = (u8)(cstep->para[0] >> 8);
}

static inline unsigned long mcmdAddKey(SYNTH_VOICE *svoice, MSTEP *cstep) {
  if ((u8)(cstep->para[0] >> 0x18) == 0) {
    svoice->curNote += (s8)(u8)(cstep->para[0] >> 8);
  } else {
    svoice->curNote = (u16)svoice->orgNote + (s16)(s8)(u8)(cstep->para[0] >> 8);
  }
  svoice->curNote = (s16)svoice->curNote < 0 ? 0 : svoice->curNote > 0x7f ? 0x7f : svoice->curNote;
  svoice->curDetune = (s8)(cstep->para[0] >> 0x10);
  if (voiceIsLastStarted(svoice) != 0) {
    inpSetMidiLastNote(svoice->midi, svoice->midiSet, svoice->curNote);
  }
  cstep->para[0] = 4;
  return mcmdWait(svoice, cstep->para);
}

static inline unsigned long mcmdLastKey(SYNTH_VOICE *svoice, MSTEP *cstep) {
  svoice->curNote = svoice->portamento_key + (s8)(u8)(cstep->para[0] >> 8);
  svoice->curNote = (s16)svoice->curNote < 0 ? 0 : svoice->curNote > 0x7f ? 0x7f : svoice->curNote;
  svoice->curDetune = (s8)(cstep->para[0] >> 16);
  if (svoice->midi != 0xFF) {
    inpSetMidiLastNote(svoice->midi, svoice->midiSet, svoice->curNote);
  }
  cstep->para[0] = 4;
  return mcmdWait(svoice, cstep->para);
}

static inline void mcmdStopSample(SYNTH_VOICE *svoice) { hwBreak(svoice->id & 0xFF); }

static inline void mcmdKeyOff(SYNTH_VOICE *svoice) {
  svoice->cFlags |= CFLAG_HW_KEYOFF_PENDING;
  synthKeyStateUpdate(svoice);
}

static inline void mcmdSetMod2Vibrato(SYNTH_VOICE *svoice, MSTEP *cstep) {
  svoice->vibratoModScale = (s8)(cstep->para[0] >> 8) << 8;
  if (svoice->vibratoModScale >= 0) {
    svoice->vibratoModScale += ((s16)(s8)(cstep->para[0] >> 0x10) << 8) / 100;
  } else {
    svoice->vibratoModScale -= ((s16)(s8)(cstep->para[0] >> 0x10) << 8) / 100;
  }
}

static inline void mcmdSetupLFO(SYNTH_VOICE *svoice, MSTEP *cstep) {
  unsigned long time;
  unsigned long phase;
  u8 n;
  n = (u8)(cstep->para[0] >> 8);
  time = (u16)(cstep->para[0] >> 0x10);
  sndConvertMs((u32 *)&time);
  if (svoice->lfo[n].period != 0) {
    phase = (u16)cstep->para[1];
    sndConvertMs((u32 *)&phase);
    svoice->lfo[n].time = phase;
  }
  svoice->lfo[n].period = time;
}

static inline void mcmdSetPitch(SYNTH_VOICE *svoice, MSTEP *cstep) {
  svoice->playbackFrequency = (unsigned long)(cstep->para[0] >> 8);
  svoice->playbackFrequency |= (u8)cstep->para[1];
  if (svoice->sampleInfo != -1) {
    DoSetPitch(svoice);
  }
}

static inline void mcmdSetADSRFromCtrl(SYNTH_VOICE *svoice, MSTEP *cstep) {
  f32 sScale;
  ADSR_INFO adsr;
  sScale = dspDLSVolTab[inpGetMidiCtrl(cstep->para[0] >> 24, svoice->midi, svoice->midiSet) >> 7];
  adsr.data.dls.atime =
      midi2TimeTab[inpGetMidiCtrl(cstep->para[0] >> 8, svoice->midi, svoice->midiSet) >> 7];
  adsr.data.dls.dtime =
      midi2TimeTab[inpGetMidiCtrl(cstep->para[0] >> 16, svoice->midi, svoice->midiSet) >> 7];
  adsr.data.dls.slevel = 193 - dspScale2IndexTab[(unsigned long)(1023.f * sScale)];
  adsr.data.dls.rtime =
      midi2TimeTab[inpGetMidiCtrl(cstep->para[1], svoice->midi, svoice->midiSet) >> 7];
  adsr.data.dls.ascale = -0x80000000;
  adsr.data.dls.dscale = -0x80000000;
  hwSetADSR((u8)svoice->id, &adsr, 2);
  svoice->cFlags |= CFLAG_ADSR_SET;
}

static inline unsigned long mcmdPitchSweep(SYNTH_VOICE *svoice, MSTEP *cstep, int num) {
  signed long delta;
  svoice->sweepOff[num] = 0;
  svoice->sweepNum[num] = (u8)(cstep->para[0] >> 8);
  svoice->sweepCnt[num] = (signed long)svoice->sweepNum[num] << 0x10;
  delta = (int)(short)(cstep->para[0] >> 0x10);
  if (delta >= 0) {
    delta = hwFrq2Pitch(delta);
  } else {
    delta = -hwFrq2Pitch(-delta);
  }
  svoice->sweepAdd[num] = delta << 0x10;
  cstep->para[0] = 0;
  return mcmdWait(svoice, cstep->para);
}

static inline void mcmdSetPanning(SYNTH_VOICE *svoice, MSTEP *cstep) { DoPanningSetup(svoice, cstep->para, 0); }

static inline void mcmdSetSurroundPanning(SYNTH_VOICE *svoice, MSTEP *cstep) {
  DoPanningSetup(svoice, cstep->para, 1);
}

static inline void mcmdSetPianoPanning(SYNTH_VOICE *svoice, MSTEP *cstep) {
  signed long delta;
  signed long scale;
  delta = (svoice->curNote << 16) - ((u8)(cstep->para[0] >> 16) << 16);
  scale = (s8)((u8)(cstep->para[0] >> 8));
  delta = ((delta * scale) >> 7);
  delta += (u8)(cstep->para[0] >> 0x18) << 16;
  delta = delta < 0 ? 0 : delta > 0x7f0000 ? 0x7f0000 : delta;
  svoice->panTarget[0] = delta;
  svoice->panCurrent[0] = delta;
}

static inline void mcmdScaleVolume(SYNTH_VOICE *svoice, MSTEP *cstep) {
  u16 curve;
  u16 scale;
  scale = (u16)(u8)(cstep->para[0] >> 8);
  if ((u8)(cstep->para[1] >> 8) == 0) {
    svoice->soundVolume = (svoice->soundVolume * scale) / 0x7f;
  } else {
    svoice->soundVolume = (svoice->originalVolume * scale) / 0x7f;
  }
  svoice->soundVolume += (u8)(cstep->para[0] >> 16) << 16;
  if (svoice->soundVolume > 0x7f0000) {
    svoice->soundVolume = 0x7f0000;
  }
  curve = (u8)(cstep->para[0] >> 0x18);
  curve |= ((u16)((u8)cstep->para[1]) << 8);
  svoice->soundVolume = TranslateVolume(svoice->soundVolume, curve);
  svoice->cFlags |= CFLAG_VOLUME_UPDATE;
}

static inline void mcmdScaleVolumeDLS(SYNTH_VOICE *svoice, MSTEP *cstep) {
  u16 scale;
  scale = (cstep->para[0] >> 8);
  if ((u8)(cstep->para[0] >> 0x18) == 0) {
    svoice->soundVolume = ((svoice->soundVolume >> 5) * scale) >> 7;
  } else {
    svoice->soundVolume = ((svoice->originalVolume >> 5) * scale) >> 7;
  }
  if (svoice->soundVolume > 0x7f0000) {
    svoice->soundVolume = 0x7f0000;
  }
  svoice->cFlags |= CFLAG_VOLUME_UPDATE;
}

static inline void mcmdEnvelope(SYNTH_VOICE *svoice, MSTEP *cstep) {
  DoEnvelopeCalculation(svoice, cstep, svoice->soundVolume);
}

static inline void mcmdFadeIn(SYNTH_VOICE *svoice, MSTEP *cstep) {
  DoEnvelopeCalculation(svoice, cstep, 0);
}

static inline void mcmdSetPitchbendAfterKeyOff(SYNTH_VOICE *svoice) { svoice->cFlags |= CFLAG_PITCHBEND_AFTER_KEYOFF; }

static inline void mcmdScaleReverb(SYNTH_VOICE *svoice, MSTEP *cstep) {
  svoice->reverbScale = (u8)(cstep->para[0] >> 8);
  svoice->reverbOffset = (u8)(cstep->para[0] >> 0x10);
}

static inline void mcmdVolumeSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpVolume, cstep, 0x80000, 1);
}

static inline void mcmdPanningSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpPanning, cstep, 0x100000, 2);
}

static inline void mcmdPitchWheelSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpPitchBend, cstep, 0x200000, 8);
}

static inline void mcmdModWheelSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpModulation, cstep, 0x400000, 0x20);
}

static inline void mcmdPedalSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpPedal, cstep, 0x2000000, 0x40);
}

static inline void mcmdPortamentoSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpPortamento, cstep, 0x1000000, 0x80);
}

static inline void mcmdReverbSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpReverb, cstep, 0x800000, 0x200);
}

static inline void mcmdPreAuxASelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpPreAuxA, cstep, 0x20000000, 0x100);
}

static inline void mcmdPreAuxBSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpPreAuxB, cstep, 0x40000000, 0x400);
}

static inline void mcmdPostAuxBSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpPostAuxB, cstep, 0x80000000, 0x800);
}

static inline void mcmdSurroundPanningSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpSurPanning, cstep, 0x4000000, 4);
}

static inline void mcmdDopplerSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpDoppler, cstep, 0x8000000, 0x10);
}

static inline void mcmdTremoloSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  SelectSource(svoice, &svoice->inpTremolo, cstep, 0x10000000, 0x1000);
}

static inline void mcmdAuxAFXSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  unsigned long i;
  static u64 mask[4] = {0x100000000, 0x200000000, 0x400000000, 0x800000000};
  static unsigned long dirty[4] = {0x80000001, 0x80000002, 0x80000004, 0x80000008};
  i = (u8)(cstep->para[1] >> 0x18);
  SelectSource(svoice, &inpAuxA[svoice->studio][i], cstep, mask[i], dirty[i]);
}

static inline void mcmdAuxBFXSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  unsigned long i;
  static u64 mask[4] = {0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000};
  static unsigned long dirty[4] = {0x80000010, 0x80000020, 0x80000040, 0x80000080};
  i = (u8)(cstep->para[1] >> 0x18);
  SelectSource(svoice, &inpAuxB[svoice->studio][i], cstep, mask[i], dirty[i]);
}

static inline void mcmdSetVarImmediate(SYNTH_VOICE *svoice, MSTEP *cstep) {
  varSet(svoice, (u8)(cstep->para[0] >> 8), (u8)(cstep->para[0] >> 0x10), (s16)cstep->para[1]);
}

static inline void mcmdIfVarCompare(SYNTH_VOICE *svoice, MSTEP *cstep, u8 cmp) {
  signed long a;
  signed long b;
  u8 result;
  a = varGet32(svoice, (u8)(cstep->para[0] >> 8), (u8)(cstep->para[0] >> 0x10));
  b = varGet32(svoice, (u8)(cstep->para[0] >> 0x18), (u8)cstep->para[1]);
  switch (cmp) {
  case 0:
    result = (a == b);
    break;
  case 1:
    result = (a < b);
    break;
  }
  if ((u8)(cstep->para[1] >> 8) != 0) {
    result = !result;
  }
  if ((u8)result != 0) {
    svoice->macroStep = svoice->macro + (u16)(cstep->para[1] >> 0x10);
  }
}

static inline void mcmdGetMessage(SYNTH_VOICE *svoice, MSTEP *cstep) {
  signed long mesg;
  mesg = 0;
  if (svoice->messageCount != '\0') {
    mesg = svoice->messageQueue[svoice->messageRead];
    svoice->messageRead = (svoice->messageRead + 1) & 3;
    --svoice->messageCount;
  }
  varSet32(svoice, 0, (u8)(cstep->para[0] >> 8), mesg);
}

static inline void mcmdGetVID(SYNTH_VOICE *svoice, MSTEP *cstep) {
  if ((u8)(cstep->para[0] >> 0x10) == 0) {
    varSet32(svoice, 0, (u8)(cstep->para[0] >> 8), svoice->vidList->vid);
  } else {
    varSet32(svoice, 0, (u8)(cstep->para[0] >> 8), svoice->lastVID);
  }
}

static inline void mcmdModeSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  svoice->volumeTable = (u8)(cstep->para[0] >> 8) ? TRUE : FALSE;
  svoice->itdMode = (u8)(cstep->para[0] >> 0x10) ? FALSE : TRUE;
}

static inline void mcmdSRCModeSelect(SYNTH_VOICE *svoice, MSTEP *cstep) {
  hwSetSRCType(svoice->id & 0xff, (u8)(cstep->para[0] >> 8));
  hwSetPolyPhaseFilter(svoice->id & 0xff, (u8)(cstep->para[0] >> 0x10));
  svoice->cFlags |= CFLAG_SRC_SET;
}

static inline void mcmdSetupTremolo(SYNTH_VOICE *svoice, MSTEP *cstep) {
  svoice->tremoloDepth = (cstep->para[0] >> 8);
  svoice->tremoloModScale = cstep->para[1];
  svoice->tremoloScale = 1.f;
}

#pragma force_active on
void macHandleActive(SYNTH_VOICE *svoice) {
  u8 i;
  u8 lastNote;
  unsigned long ex;
  CHANNEL_DEFAULTS *channelDefaults;
  static MSTEP cstep;
  if (svoice->cFlags & (CFLAG_STARTING | CFLAG_HW_BREAK)) {
    if (svoice->cFlags & CFLAG_HW_BREAK) {
      svoice->cFlags &= ~CFLAG_HW_BREAK;
      hwBreak(svoice->id & 0xFF);
    }
    svoice->panCurrent[0] = svoice->panTarget[0] = (unsigned long)(svoice->setupPan) << 16;
    svoice->panCurrent[1] = svoice->panTarget[1] = 0;
    svoice->soundVolume = (unsigned long)(svoice->setupVolume << 16);
    svoice->volumeTable = 0;
    svoice->originalVolume = svoice->soundVolume;
    svoice->midi = svoice->setupMidi;
    svoice->midiSet = svoice->setupMidiSet;
    svoice->section = svoice->setupSection;
    svoice->track = svoice->setupTrack;
    svoice->itdMode = svoice->setupITD;
    svoice->keyGroup = 0;
    svoice->vibratoModScale = 0;
    svoice->tremoloDepth = 0;
    inpInit(svoice);
    if ((lastNote = inpGetMidiLastNote(svoice->midi, svoice->midiSet)) != 0xFF) {
      svoice->portamento_key = lastNote;
    } else {
      svoice->portamento_key = svoice->orgNote;
    }
    inpSetMidiLastNote(svoice->midi, svoice->midiSet, svoice->orgNote);
    voiceSetLastStarted(svoice);
    svoice->vGroup = svoice->setupVGroup;
    svoice->studio = svoice->setupStudio;
    svoice->portTime = 0;
    svoice->portDuration = 25600;
    svoice->portamento_type = 0;
    if (svoice->midi != 0xff) {
      svoice->portLastCtrlState = inpGetMidiCtrl(SND_MIDICTRL_PORTAMENTO, svoice->midi, svoice->midiSet);
    } else {
      svoice->portLastCtrlState = 0;
    }
    channelDefaults = inpGetChannelDefaults(svoice->midi, svoice->midiSet);
    svoice->pbLowerKeyRange = channelDefaults->pBendRange;
    svoice->pbUpperKeyRange = channelDefaults->pBendRange;
    svoice->reverbScale = 128;
    svoice->reverbOffset = 0;
    svoice->loop = 0;
    svoice->sweepNum[0] = 0;
    svoice->sweepNum[1] = 0;
    svoice->sweepOff[0] = 0;
    svoice->sweepOff[1] = 0;
    svoice->lfo[0].period = 0;
    svoice->lfo[0].value = 0;
    svoice->lfo[0].lastValue = 0x7fff;
    svoice->lfo[1].period = 0;
    svoice->lfo[1].value = 0;
    svoice->lfo[1].lastValue = 0x7fff;
    for (i = 0; i < 3; ++i) {
      svoice->trapMacro[i] = NULL;
    }
    svoice->trapEnabled[0] = 0;
    svoice->sampleInfo = -1;
    svoice->playbackFrequency = -1;
    svoice->pbLast = 0x2000;
    svoice->currentOutputVolume = 0;
    svoice->cFlags &= CFLAG_EXTERNAL_KEYOFF;
    svoice->cFlags |= (CFLAG_PAN_UPDATE | CFLAG_VOLUME_UPDATE);
    memset(svoice->variable, 0, sizeof(svoice->variable));
    svoice->macroTime = macRealTime;
    svoice->macStartTime = macRealTime;
    synthStartSynthJobHandling(svoice);
  }
  DebugMacroSteps = 0;
  do {
    if (++DebugMacroSteps > 32) {
      break;
    }
    cstep.para[0] = svoice->macroStep->para[0];
    cstep.para[1] = svoice->macroStep->para[1];
    ++svoice->macroStep;
    ex = 0;
    switch (cstep.para[0] & 0x7f) {
    case 0x0:
      ex = mcmdEndOfMacro(svoice);
      break;
    case 0x1:
      ex = mcmdStop(svoice);
      break;
    case 0x2:
      mcmdIfKey(svoice, &cstep);
      break;
    case 0x3:
      mcmdIfVelocity(svoice, &cstep);
      break;
    case 0x4:
      ex = mcmdWait(svoice, cstep.para);
      break;
    case 0x5:
      mcmdLoop(svoice, cstep.para);
      break;
    case 0x6:
      ex = mcmdGoto(svoice, &cstep);
      break;
    case 0x7:
      ex = mcmdWaitMs(svoice, &cstep);
      break;
    case 0x8:
      mcmdPlayMacro(svoice, &cstep);
      break;
    case 0x9:
      mcmdSendKeyOff(svoice, &cstep);
      break;
    case 0xa:
      mcmdIfModulation(svoice, &cstep);
      break;
    case 0xb:
      mcmdSetPianoPanning(svoice, &cstep);
      break;
    case 0xc:
      mcmdSetADSR(svoice, &cstep);
      break;
    case 0xd:
      mcmdScaleVolume(svoice, &cstep);
      break;
    case 0xe:
      mcmdSetPanning(svoice, &cstep);
      break;
    case 0xf:
      mcmdEnvelope(svoice, &cstep);
      break;
    case 0x10:
      mcmdStartSample(svoice, &cstep);
      break;
    case 0x11:
      mcmdStopSample(svoice);
      break;
    case 0x12:
      mcmdKeyOff(svoice);
      break;
    case 0x13:
      mcmdIfRandom(svoice, &cstep);
      break;
    case 0x14:
      mcmdFadeIn(svoice, &cstep);
      break;
    case 0x15:
      mcmdSetSurroundPanning(svoice, &cstep);
      break;
    case 0x16:
      mcmdSetADSRFromCtrl(svoice, &cstep);
      break;
    case 0x17:
      mcmdRandomKey(svoice, &cstep);
      break;
    case 0x18:
      ex = mcmdAddKey(svoice, &cstep);
      break;
    case 0x19:
      ex = mcmdSetKey(svoice, &cstep);
      break;
    case 0x1a:
      ex = mcmdLastKey(svoice, &cstep);
      break;
    case 0x1b:
      mcmdPortamento(svoice, cstep.para);
      break;
    case 0x1c:
      mcmdVibrato(svoice, cstep.para);
      break;
    case 0x1d:
      ex = mcmdPitchSweep(svoice, &cstep, 0);
      break;
    case 0x1e:
      ex = mcmdPitchSweep(svoice, &cstep, 1);
      break;
    case 0x1f:
      mcmdSetPitch(svoice, &cstep);
      break;
    case 0x20:
      mcmdSetPitchADSR(svoice, &cstep);
      break;
    case 0x21:
      mcmdScaleVolumeDLS(svoice, &cstep);
      break;
    case 0x22:
      mcmdSetMod2Vibrato(svoice, &cstep);
      break;
    case 0x23:
      mcmdSetupTremolo(svoice, &cstep);
      break;
    case 0x24:
      mcmdReturn(svoice);
      break;
    case 0x25:
      ex = mcmdGosub(svoice, &cstep);
      break;
    case 0x28:
      mcmdTrapEvent(svoice, &cstep);
      break;
    case 0x29:
      mcmdUntrapEvent(svoice, &cstep);
      break;
    case 0x2a:
      mcmdSendMessage(svoice, &cstep);
      break;
    case 0x2b:
      mcmdGetMessage(svoice, &cstep);
      break;
    case 0x2c:
      mcmdGetVID(svoice, &cstep);
      break;
    case 0x30:
      mcmdAddAgeCounter(svoice, &cstep);
      break;
    case 0x31:
      mcmdSetAgeCounter(svoice, &cstep);
      break;
    case 0x32:
      mcmdSendFlag(&cstep);
      break;
    case 0x33:
      mcmdSetPitchWheelRange(svoice, &cstep);
      break;
    case 0x34:
      mcmdScaleReverb(svoice, &cstep);
      break;
    case 0x35:
      mcmdSetPitchbendAfterKeyOff(svoice);
      break;
    case 0x36:
      mcmdSetPriority(svoice, &cstep);
      break;
    case 0x37:
      mcmdAddPriority(svoice, &cstep);
      break;
    case 0x38:
      mcmdSetAgeCounterSpeed(svoice, &cstep);
      break;
    case 0x39:
      mcmdSetAgeCounterByVolume(svoice, &cstep);
      break;
    case 0x40:
      mcmdVolumeSelect(svoice, &cstep);
      break;
    case 0x41:
      mcmdPanningSelect(svoice, &cstep);
      break;
    case 0x42:
      mcmdPitchWheelSelect(svoice, &cstep);
      break;
    case 0x43:
      mcmdModWheelSelect(svoice, &cstep);
      break;
    case 0x44:
      mcmdPedalSelect(svoice, &cstep);
      break;
    case 0x45:
      mcmdPortamentoSelect(svoice, &cstep);
      break;
    case 0x46:
      mcmdReverbSelect(svoice, &cstep);
      break;
    case 0x47:
      mcmdSurroundPanningSelect(svoice, &cstep);
      break;
    case 0x48:
      mcmdDopplerSelect(svoice, &cstep);
      break;
    case 0x49:
      mcmdTremoloSelect(svoice, &cstep);
      break;
    case 0x4a:
      mcmdPreAuxASelect(svoice, &cstep);
      break;
    case 0x4b:
      mcmdPreAuxBSelect(svoice, &cstep);
      break;
    case 0x4c:
      mcmdPostAuxBSelect(svoice, &cstep);
      break;
    case 0x4d:
      mcmdAuxAFXSelect(svoice, &cstep);
      break;
    case 0x4e:
      mcmdAuxBFXSelect(svoice, &cstep);
      break;
    case 0x50:
      mcmdSetupLFO(svoice, &cstep);
      break;
    case 0x58:
      mcmdModeSelect(svoice, &cstep);
      break;
    case 0x59:
      mcmdSetKeyGroup(svoice, &cstep);
      break;
    case 0x5a:
      mcmdSRCModeSelect(svoice, &cstep);
      break;
    case 0x60:
      mcmdVarCalculation(svoice, &cstep, 0);
      break;
    case 0x61:
      mcmdVarCalculation(svoice, &cstep, 1);
      break;
    case 0x62:
      mcmdVarCalculation(svoice, &cstep, 2);
      break;
    case 0x63:
      mcmdVarCalculation(svoice, &cstep, 3);
      break;
    case 0x64:
      mcmdVarCalculation(svoice, &cstep, 4);
      break;
    case 0x65:
      mcmdSetVarImmediate(svoice, &cstep);
      break;
    case 0x70:
      mcmdIfVarCompare(svoice, &cstep, 0);
      break;
    case 0x71:
      mcmdIfVarCompare(svoice, &cstep, 1);
      break;
    }
  } while (!ex);
}
#pragma force_active reset
