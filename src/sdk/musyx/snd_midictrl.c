#include "musyx_synth.h"
#include "musyx_midictrl.h"

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/snd_midictrl.c (CC0-1.0). */




typedef struct CHANNEL_DEFAULTS {
    u8 pBendRange;
} CHANNEL_DEFAULTS;

static u8 midi_lastNote[8][16];
static u8 fx_lastNote[64];
static u8 midi_ctrl[8][16][134];
static u8 fx_ctrl[64][134];
static u32 inpGlobalMIDIDirtyFlags[8][16];
static CHANNEL_DEFAULTS inpChannelDefaults[8][16];
static CHANNEL_DEFAULTS inpFXChannelDefaults[64];
extern u16 _GetInputValue(SYNTH_VOICE *voice, CTRL_DEST *input, u8 midi, u8 midiSet);


#define NULL 0
#pragma auto_inline on
#define SND_STUDIO_MAXNUM 8
extern const u8 inpColdMIDIDefaults[134], inpWarmMIDIDefaults[134];
extern CTRL_DEST inpAuxA[8][4], inpAuxB[8][4];
extern void *memcpy(void *dest, const void *source, u32 size);

void inpSetGlobalMIDIDirtyFlag(u8 channel, u8 set, u32 flag)
{
    inpGlobalMIDIDirtyFlags[set][channel] |= flag;
}

CHANNEL_DEFAULTS *inpGetChannelDefaults(u8 midi, u8 midiSet)
{
    if (midiSet == 0xFF)
        return &inpFXChannelDefaults[midi];
    return &inpChannelDefaults[midiSet][midi];
}

void inpResetChannelDefaults(u8 midi, u8 midiSet)
{
    CHANNEL_DEFAULTS *defaults = midiSet != 0xFF ? &inpChannelDefaults[midiSet][midi]
                                                  : &inpFXChannelDefaults[midi];
    defaults->pBendRange = 2;
}


void inpResetMidiCtrl(u8 ch, u8 set, u32 coldReset) {
  u8 *values;
  u8 *dest;
  u32 i;
  values = coldReset ? (u8 *)inpColdMIDIDefaults : (u8 *)inpWarmMIDIDefaults;
  dest = set != 0xFF ? midi_ctrl[set][ch] : fx_ctrl[ch];
  if (coldReset) {
    memcpy(dest, values, 134);
  } else {
    for (i = 0; i < 134; ++i) {
      if (values[i] != 0xFF) {
        dest[i] = values[i];
      }
    }
  }
  inpSetMidiLastNote(ch, set, 0xFF);
}
void inpSetMidiLastNote(u8 midi, u8 midiSet, u8 key)
{
    if (midiSet != 0xFF)
        midi_lastNote[midiSet][midi] = key;
    else
        fx_lastNote[midi] = key;
}

u8 inpGetMidiLastNote(u8 midi, u8 midiSet)
{
    if (midiSet != 0xFF)
        return midi_lastNote[midiSet][midi];
    return fx_lastNote[midi];
}

u16 inpGetVolume(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x1))
        return voice->inpVolume.oldValue;
    voice->midiDirtyFlags &= ~0x1;
    return _GetInputValue(voice, &voice->inpVolume, voice->midi, voice->midiSet);
}

u16 inpGetPanning(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x2))
        return voice->inpPanning.oldValue;
    voice->midiDirtyFlags &= ~0x2;
    return _GetInputValue(voice, &voice->inpPanning, voice->midi, voice->midiSet);
}

u16 inpGetSurPanning(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x4))
        return voice->inpSurPanning.oldValue;
    voice->midiDirtyFlags &= ~0x4;
    return _GetInputValue(voice, &voice->inpSurPanning, voice->midi, voice->midiSet);
}

u16 inpGetPitchBend(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x8))
        return voice->inpPitchBend.oldValue;
    voice->midiDirtyFlags &= ~0x8;
    return _GetInputValue(voice, &voice->inpPitchBend, voice->midi, voice->midiSet);
}

u16 inpGetDoppler(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x10))
        return voice->inpDoppler.oldValue;
    voice->midiDirtyFlags &= ~0x10;
    return _GetInputValue(voice, &voice->inpDoppler, voice->midi, voice->midiSet);
}

u16 inpGetModulation(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x20))
        return voice->inpModulation.oldValue;
    voice->midiDirtyFlags &= ~0x20;
    return _GetInputValue(voice, &voice->inpModulation, voice->midi, voice->midiSet);
}

u16 inpGetPedal(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x40))
        return voice->inpPedal.oldValue;
    voice->midiDirtyFlags &= ~0x40;
    return _GetInputValue(voice, &voice->inpPedal, voice->midi, voice->midiSet);
}

u16 inpGetPreAuxA(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x100))
        return voice->inpPreAuxA.oldValue;
    voice->midiDirtyFlags &= ~0x100;
    return _GetInputValue(voice, &voice->inpPreAuxA, voice->midi, voice->midiSet);
}

u16 inpGetReverb(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x200))
        return voice->inpReverb.oldValue;
    voice->midiDirtyFlags &= ~0x200;
    return _GetInputValue(voice, &voice->inpReverb, voice->midi, voice->midiSet);
}

u16 inpGetPreAuxB(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x400))
        return voice->inpPreAuxB.oldValue;
    voice->midiDirtyFlags &= ~0x400;
    return _GetInputValue(voice, &voice->inpPreAuxB, voice->midi, voice->midiSet);
}

u16 inpGetPostAuxB(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x800))
        return voice->inpPostAuxB.oldValue;
    voice->midiDirtyFlags &= ~0x800;
    return _GetInputValue(voice, &voice->inpPostAuxB, voice->midi, voice->midiSet);
}

u16 inpGetTremolo(SYNTH_VOICE *voice)
{
    if (!(voice->midiDirtyFlags & 0x1000))
        return voice->inpTremolo.oldValue;
    voice->midiDirtyFlags &= ~0x1000;
    return _GetInputValue(voice, &voice->inpTremolo, voice->midi, voice->midiSet);
}


#pragma auto_inline on

static inline u32 inpResetGlobalMIDIDirtyFlag(u8 chan, u8 midiSet, u32 flag) {
  u32 ret;
  if ((ret = (flag & inpGlobalMIDIDirtyFlags[midiSet][chan]) != 0) != 0) {
    inpGlobalMIDIDirtyFlags[midiSet][chan] &= ~flag;
  }
  return ret;
}

static inline u16 GetGlobalInputValue(CTRL_DEST *inp, u32 dirtyMask, u8 midi, u8 midiSet) {
  if (!inpResetGlobalMIDIDirtyFlag(midi, midiSet, dirtyMask)) {
    return inp->oldValue;
  }
  return _GetInputValue(NULL, inp, midi, midiSet);
}

u16 inpGetAuxA(u8 studio, u8 index, u8 midi, u8 midiSet) {
  static u32 dirtyMask[4] = {0x80000001, 0x80000002, 0x80000004, 0x80000008};
  return GetGlobalInputValue(&inpAuxA[studio][index], dirtyMask[index], midi, midiSet);
}

u16 inpGetAuxB(u8 studio, u8 index, u8 midi, u8 midiSet) {
  static u32 dirtyMask[4] = {0x80000010, 0x80000020, 0x80000040, 0x80000080};
  return GetGlobalInputValue(&inpAuxB[studio][index], dirtyMask[index], midi, midiSet);
}
#pragma auto_inline off
u8 inpTranslateExCtrl(u8 control)
{
    switch (control) {
    case 128: return 128;
    case 129: return 130;
    case 130: return 160;
    case 131: return 161;
    case 132: return 131;
    case 133: return 132;
    case 134: return 162;
    case 135: return 163;
    case 136: return 164;
    default: return control;
    }
}

u16 inpGetExCtrl(SYNTH_VOICE *voice, u8 control)
{
    switch (inpTranslateExCtrl(control)) {
    case 160:
        return voice->lfo[0].value * 2 + 0x2000;
    case 161:
        return voice->lfo[1].value * 2 + 0x2000;
    default:
        return voice->midi != 0xFF ? inpGetMidiCtrl(control, voice->midi, voice->midiSet) : 0;
    }
}

void inpSetExCtrl(SYNTH_VOICE *voice, u8 control, s16 value)
{
    s16 clipped = value < 0 ? 0 : value > 0x3FFF ? 0x3FFF : value;
    u8 translated = inpTranslateExCtrl(control);

    switch (translated) {
    case 160:
    case 161:
        break;
    default:
        if (voice->midi != 0xFF)
            inpSetMidiCtrl14(control, voice->midi, voice->midiSet, clipped);
    }
}

void inpAddCtrl(CTRL_DEST *dest, u8 ctrl, s32 scale, u8 comb, u32 isVar) {
  u8 n;
  if (comb == 0) {
    dest->numSource = 0;
  }
  if (dest->numSource < 4) {
    n = dest->numSource++;
    if (isVar == 0) {
      ctrl = inpTranslateExCtrl(ctrl);
    } else {
      comb |= 0x10;
    }
    dest->source[n].midiCtrl = ctrl;
    dest->source[n].combine = comb;
    dest->source[n].scale = scale;
  }
}

void inpFXCopyCtrl(u8 ctrl, SYNTH_VOICE *dvoice, SYNTH_VOICE *svoice) {
  u8 di;
  u8 si;
  di = dvoice->id;
  si = svoice->id;
  if (ctrl < 64) {
    fx_ctrl[di][ctrl & 31] = fx_ctrl[si][ctrl & 31];
    fx_ctrl[di][(ctrl & 31) + 32] = fx_ctrl[si][(ctrl & 31) + 32];
  } else if (ctrl == 128 || ctrl == 129) {
    fx_ctrl[di][ctrl & 254] = fx_ctrl[si][ctrl & 254];
    fx_ctrl[di][(ctrl & 254) + 1] = fx_ctrl[si][(ctrl & 254) + 1];
  } else if (ctrl == 132 || ctrl == 133) {
    fx_ctrl[di][ctrl & 254] = fx_ctrl[si][ctrl & 254];
    fx_ctrl[di][(ctrl & 254) + 1] = fx_ctrl[si][(ctrl & 254) + 1];
  } else {
    fx_ctrl[di][ctrl] = fx_ctrl[si][ctrl];
  }
}

u16 inpGetMidiCtrl(u8 ctrl, u8 channel, u8 set) {
  if (channel != 0xff) {
    if (set != 0xff) {
      if (ctrl < 0x40) {
        return midi_ctrl[set][channel][ctrl & 0x1f] << 7 |
               midi_ctrl[set][channel][(ctrl & 0x1f) + 0x20];
      }
      if (ctrl < 0x46) {
        return midi_ctrl[set][channel][ctrl] < 0x40 ? 0 : 0x3fff;
      }
      if (ctrl >= 0x60 && ctrl < 0x66) {
        return 0;
      }
      if ((ctrl == 0x80) || (ctrl == 0x81)) {
        return midi_ctrl[set][channel][ctrl & 0xfe] << 7 |
               midi_ctrl[set][channel][(ctrl & 0xfe) + 1];
      }
      if ((ctrl == 0x84) || (ctrl == 0x85)) {
        return midi_ctrl[set][channel][ctrl & 0xfe] << 7 |
               midi_ctrl[set][channel][(ctrl & 0xfe) + 1];
      }
      return midi_ctrl[set][channel][ctrl] << 7;
    }
    if (ctrl < 0x40) {
      return fx_ctrl[channel][ctrl & 0x1f] << 7 | fx_ctrl[channel][(ctrl & 0x1f) + 0x20];
    }
    if (ctrl < 0x46) {
      return fx_ctrl[channel][ctrl] < 0x40 ? 0 : 0x3fff;
    }
    if (ctrl >= 0x60 && ctrl < 0x66) {
      return 0;
    }
    if ((ctrl == 0x80) || (ctrl == 0x81)) {
      return fx_ctrl[channel][ctrl & 0xfe] << 7 | fx_ctrl[channel][(ctrl & 0xfe) + 1];
    }
    if ((ctrl == 0x84) || (ctrl == 0x85)) {
      return fx_ctrl[channel][ctrl & 0xfe] << 7 | fx_ctrl[channel][(ctrl & 0xfe) + 1];
    }
    return fx_ctrl[channel][ctrl] << 7;
  }
  return 0;
}

void inpSetMidiCtrl14(u8 ctrl, u8 channel, u8 set, u16 value) {
  if (channel == 0xFF) {
    return;
  }
  if (ctrl < 64) {
    inpSetMidiCtrl(ctrl & 31, channel, set, value >> 7);
    inpSetMidiCtrl((ctrl & 31) + 32, channel, set, value & 0x7f);
  } else if (ctrl == 128 || ctrl == 129) {
    inpSetMidiCtrl(ctrl & 254, channel, set, value >> 7);
    inpSetMidiCtrl((ctrl & 254) + 1, channel, set, value & 0x7f);
  } else if (ctrl == 132 || ctrl == 133) {
    inpSetMidiCtrl(ctrl & 254, channel, set, value >> 7);
    inpSetMidiCtrl((ctrl & 254) + 1, channel, set, value & 0x7f);
  } else {
    inpSetMidiCtrl(ctrl, channel, set, value >> 7);
  }
}

extern void synthKeyStateUpdate(SYNTH_VOICE *voice);
#pragma auto_inline on

static inline void inpSetRPNHi(u8 set, u8 channel, u8 value) {
  u16 rpn;
  u32 i;
  u8 range;
  rpn = (midi_ctrl[set][channel][100]) | (midi_ctrl[set][channel][101] << 8);
  switch (rpn) {
  case 0:
    range = value > 24 ? 24 : value;
    inpChannelDefaults[set][channel].pBendRange = range;
    for (i = 0; i < synthInfo.voice_count; ++i) {
      if (set == synthVoice[i].midiSet && channel == synthVoice[i].midi) {
        synthVoice[i].pbUpperKeyRange = range;
        synthVoice[i].pbLowerKeyRange = range;
      }
    }
    break;
  default:
    break;
  }
}

static inline void inpSetRPNLo(u8 set, u8 channel, u8 value) {
}

static inline void inpSetRPNDec(u8 set, u8 channel) {
  u16 rpn;
  u32 i;
  u8 range;
  rpn = (midi_ctrl[set][channel][100]) | (midi_ctrl[set][channel][101] << 8);
  switch (rpn) {
  case 0:
    range = inpChannelDefaults[set][channel].pBendRange;
    if (range != 0) {
      --range;
    }
    inpChannelDefaults[set][channel].pBendRange = range;
    for (i = 0; i < synthInfo.voice_count; ++i) {
      if (set == synthVoice[i].midiSet && channel == synthVoice[i].midi) {
        synthVoice[i].pbLowerKeyRange = synthVoice[i].pbUpperKeyRange = range;
      }
    }
    break;
  }
}

static inline void inpSetRPNInc(u8 set, u8 channel) {
  u16 rpn;
  u32 i;
  u8 range;
  rpn = (midi_ctrl[set][channel][100]) | (midi_ctrl[set][channel][101] << 8);
  switch (rpn) {
  case 0:
    range = inpChannelDefaults[set][channel].pBendRange;
    if (range < 24) {
      ++range;
    }
    inpChannelDefaults[set][channel].pBendRange = range;
    for (i = 0; i < synthInfo.voice_count; ++i) {
      if (set == synthVoice[i].midiSet && channel == synthVoice[i].midi) {
        synthVoice[i].pbLowerKeyRange = synthVoice[i].pbUpperKeyRange = range;
      }
    }
    break;
  }
}

void inpSetMidiCtrl(u8 ctrl, u8 channel, u8 set, u8 value) {
  u32 i;
  if (channel == 0xFF) {
    return;
  }
  if (set != 0xFF) {
    switch (ctrl) {
    case 6:
      inpSetRPNHi(set, channel, value);
      break;
    case 38:
      inpSetRPNLo(set, channel, value);
      break;
    case 96:
      inpSetRPNDec(set, channel);
      break;
    case 97:
      inpSetRPNInc(set, channel);
      break;
    }
    midi_ctrl[set][channel][ctrl] = (value & 0x7f);
    for (i = 0; i < synthInfo.voice_count; ++i) {
      if (set == synthVoice[i].midiSet && channel == synthVoice[i].midi) {
        synthVoice[i].midiDirtyFlags = 0x1fff;
        synthKeyStateUpdate(&synthVoice[i]);
      }
    }
    inpGlobalMIDIDirtyFlags[set][channel] = 0xff;
  } else {
    switch (ctrl) {
    case 6:
      inpSetRPNHi(set, channel, value);
      break;
    case 38:
      inpSetRPNLo(set, channel, value);
      break;
    case 96:
      inpSetRPNDec(set, channel);
      break;
    case 97:
      inpSetRPNInc(set, channel);
      break;
    }
    fx_ctrl[channel][ctrl] = value & 0x7f;
    for (i = 0; i < synthInfo.voice_count; ++i) {
      if (set == synthVoice[i].midiSet && channel == synthVoice[i].midi) {
        synthVoice[i].midiDirtyFlags = 0x1fff;
        synthKeyStateUpdate(&synthVoice[i]);
      }
    }
  }
}

#define NULL 0
#define TRUE 1
#define FALSE 0
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define CLAMP(value, min, max) ((value) > (max) ? (max) : (value) < (min) ? (min) : (value))
#define CLAMP_INV(value, min, max) ((value) < (min) ? (min) : (value) > (max) ? (max) : (value))
extern s16 varGet(SYNTH_VOICE *voice, u32 ctrl, u8 index);
#pragma auto_inline off
u16 _GetInputValue(SYNTH_VOICE *svoice, CTRL_DEST *inp, u8 midi, u8 midiSet) {
  u32 i;
  u32 value;
  u8 ctrl;
  signed long tmp;
  signed long vtmp;
  u32 sign;
  value = 0;
  for (i = 0; i < inp->numSource; ++i) {
    if (inp->source[i].combine & 0x10) {
      tmp = (svoice != NULL ? varGet(svoice, 0, inp->source[i].midiCtrl) : 0);
      goto block_18;
    }
    ctrl = inp->source[i].midiCtrl;
    if (ctrl == 128 || ctrl == 1 || ctrl == 10 || ctrl == 160 || ctrl == 161 || ctrl == 131) {
      switch (ctrl) {
      case 160:
      case 161:
        if (svoice != NULL) {
          tmp = svoice->lfo[ctrl - 160].value << 1;
          svoice->lfoUsedByInput[ctrl - 160] = 1;
        } else {
          tmp = 0;
        }
        break;
      default:
        tmp = inpGetMidiCtrl(ctrl, midi, midiSet) - 0x2000;
        break;
      }
    block_18:
      tmp = (tmp * (inp->source[i].scale >> 1)) >> 15;
      tmp = tmp < -0x2000 ? -0x2000 : tmp > 0x1FFF ? 0x1FFF : tmp;
      switch (inp->source[i].combine & 15) {
      case 0:
        value = tmp + 0x2000;
        sign = TRUE;
        break;
      case 1:
        if (sign != FALSE) {
          vtmp = (value + tmp);
          vtmp -= 0x2000;
          value = CLAMP_INV(vtmp, -0x2000, 0x1FFF) + 0x2000;
        } else {
          vtmp = value + tmp;
          value = CLAMP(vtmp, 0, 0x3FFF);
        }
        break;
      case 2:
        if (sign != FALSE) {
          vtmp = (s32)((value - 0x2000) * tmp) >> 13;
        } else {
          vtmp = (tmp * value) >> 13;
          sign = TRUE;
        }
        value = CLAMP_INV(vtmp, -0x2000, 0x1FFF) + 0x2000;
        break;
      case 3:
        if (sign != FALSE) {
          vtmp = (value - 0x2000) - tmp;
          value = CLAMP_INV(vtmp, -0x2000, 0x1FFF) + 0x2000;
        } else {
          vtmp = value - tmp;
          value = CLAMP(vtmp, 0, 0x3FFF);
        }
        break;
      }
    } else {
      switch (ctrl) {
      case 162:
        if (svoice != NULL) {
          tmp = svoice->orgNote << 7;
        } else {
          tmp = 0;
        }
        break;
      case 163:
        if (svoice != NULL) {
          tmp = svoice->originalVolume >> 9;
        } else {
          tmp = 0;
        }
        break;
      case 164:
        if (svoice != NULL) {
          tmp = (synthRealTime - svoice->macStartTime) >> 8;
          if (tmp > 0x3fff) {
            tmp = 0x3fff;
          }
          svoice->timeUsedByInput = 1;
        } else {
          tmp = 0;
        }
        break;
      default:
        tmp = inpGetMidiCtrl(ctrl, midi, midiSet);
        break;
      }
      tmp = (tmp * (inp->source[i].scale >> 1)) >> 15;
      if (tmp > 0x3FFF) {
        tmp = 0x3FFF;
      }
      switch (inp->source[i].combine & 0xF) {
      case 0:
        value = tmp;
        sign = FALSE;
        break;
      case 1:
        if (sign != FALSE) {
          vtmp = (value + tmp);
          vtmp -= 0x2000;
          value = CLAMP_INV(vtmp, -0x2000, 0x1FFF) + 0x2000;
        } else {
          value += tmp;
          value = MIN(value, 0x3FFF);
        }
        break;
      case 2:
        if (sign != FALSE) {
          vtmp = (s32)(tmp * (value - 0x2000)) >> 14;
          value = CLAMP_INV(vtmp, -0x2000, 0x1FFF) + 0x2000;
        } else {
          value = ((value * tmp) >> 0xE);
          value = MIN(value, 0x3FFF);
        }
        break;
      case 3:
        if (sign != FALSE) {
          vtmp = (value - 0x2000) - tmp;
          value = CLAMP_INV(vtmp, -0x2000, 0x1FFF) + 0x2000;
        } else {
          vtmp = value - tmp;
          value = CLAMP(vtmp, 0, 0x3FFF);
        }
        break;
      }
    }
  }
  inp->oldValue = value;
  return value;
}

#pragma auto_inline on

static inline void inpResetGlobalMIDIDirtyFlags() {
  u32 i, j;
  for (i = 0; i < 8; ++i) {
    for (j = 0; j < 16; ++j) {
      inpGlobalMIDIDirtyFlags[i][j] = 0xFF;
    }
  }
}

void inpInit(SYNTH_VOICE *svoice) {
  u32 i;
  u32 s;
  if (svoice != NULL) {
    svoice->inpVolume.source[0].midiCtrl = 7;
    svoice->inpVolume.source[0].combine = 0;
    svoice->inpVolume.source[0].scale = 0x10000;
    svoice->inpVolume.source[1].midiCtrl = 11;
    svoice->inpVolume.source[1].combine = 2;
    svoice->inpVolume.source[1].scale = 0x10000;
    svoice->inpVolume.numSource = 2;
    svoice->inpPanning.source[0].midiCtrl = 10;
    svoice->inpPanning.source[0].combine = 0;
    svoice->inpPanning.source[0].scale = 0x10000;
    svoice->inpPanning.numSource = 1;
    svoice->inpSurPanning.source[0].midiCtrl = 131;
    svoice->inpSurPanning.source[0].combine = 0;
    svoice->inpSurPanning.source[0].scale = 0x10000;
    svoice->inpSurPanning.numSource = 1;
    svoice->inpPitchBend.source[0].midiCtrl = 128;
    svoice->inpPitchBend.source[0].combine = 0;
    svoice->inpPitchBend.source[0].scale = 0x10000;
    svoice->inpPitchBend.numSource = 1;
    svoice->inpModulation.source[0].midiCtrl = 1;
    svoice->inpModulation.source[0].combine = 0;
    svoice->inpModulation.source[0].scale = 0x10000;
    svoice->inpModulation.numSource = 1;
    svoice->inpPedal.source[0].midiCtrl = 64;
    svoice->inpPedal.source[0].combine = 0;
    svoice->inpPedal.source[0].scale = 0x10000;
    svoice->inpPedal.numSource = 1;
    svoice->inpPortamento.source[0].midiCtrl = 65;
    svoice->inpPortamento.source[0].combine = 0;
    svoice->inpPortamento.source[0].scale = 0x10000;
    svoice->inpPortamento.numSource = 1;
    svoice->inpPreAuxA.numSource = 0;
    svoice->inpReverb.source[0].midiCtrl = 91;
    svoice->inpReverb.source[0].combine = 0;
    svoice->inpReverb.source[0].scale = 0x10000;
    svoice->inpReverb.numSource = 1;
    svoice->inpPreAuxB.numSource = 0;
    svoice->inpPostAuxB.source[0].midiCtrl = 93;
    svoice->inpPostAuxB.source[0].combine = 0;
    svoice->inpPostAuxB.source[0].scale = 0x10000;
    svoice->inpPostAuxB.numSource = 1;
    svoice->inpDoppler.source[0].midiCtrl = 132;
    svoice->inpDoppler.source[0].combine = 0;
    svoice->inpDoppler.source[0].scale = 0x10000;
    svoice->inpDoppler.numSource = 1;
    svoice->inpTremolo.numSource = 0;
    svoice->midiDirtyFlags = 0x1fff;
    svoice->lfoUsedByInput[0] = 0;
    svoice->lfoUsedByInput[1] = 0;
    svoice->timeUsedByInput = 0;
  } else {
    for (s = 0; s < SND_STUDIO_MAXNUM; ++s) {
      for (i = 0; i < 4; ++i) {
        inpAuxA[s][i].numSource = 0;
        inpAuxB[s][i].numSource = 0;
      }
    }
    inpResetGlobalMIDIDirtyFlags();
  }
}
