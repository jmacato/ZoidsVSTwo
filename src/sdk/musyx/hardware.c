#include "musyx_synth.h"
#include "musyx_sample.h"
#include "musyx_dsp.h"
#include "musyx_hardware.h"

extern DSPvoice *dspVoice;
extern u8 salTimeOffset;
extern u32 (*salMessageCallback)(u32, u32);
extern u32 dspHRTFOn;
extern DSPstudioinfo dspStudio[8];
extern void hwExitIrq(void);
extern void salExitDsp(void);
extern void salExitDspCtrl(void);
extern void salExitAi(void);
extern void salActivateVoice(DSPvoice *voice, u8 studio);
extern void salDeactivateVoice(DSPvoice *voice);
extern u32 salAddStudioInput(DSPstudioinfo *studio, void *input);
extern u32 salRemoveStudioInput(DSPstudioinfo *studio, void *input);
extern void salActivateStudio(u8 studio, u32 isMaster, u32 type);
extern void salDeactivateStudio(u8 studio);
extern u32 aramGetStreamBufferAddress(u8 id, u32 *len);
extern void aramInit(u32 length);
extern void aramExit(void);
extern void aramSyncTransferQueue(void);

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/hardware.c (CC0-1.0). */
extern u8 sndActive, salFrame, salAuxFrame, salNumVoices;
extern void streamCorrectLoops(void);
extern void hwIRQEnterCritical(void);
extern void hwIRQLeaveCritical(void);
extern void *salAiGetDest(void);
extern void salCtrlDsp(s16 *dest);
extern void salHandleAuxProcessing(void);
extern void hwSetTimeOffset(u8 offset);
extern void seqHandle(unsigned long deltaTime);
extern void synthHandle(unsigned long deltaTime);
extern void s3dHandle(void);
extern void streamHandle(void);
extern void vsSampleUpdates(void);
extern void hwInitIrq(void);
extern unsigned long salInitAi(void (*callback)(void), unsigned long flags, unsigned long *frequency);
extern unsigned long salInitDspCtrl(u8 voices, u8 studios, unsigned long dpl2);
extern unsigned long salInitDsp(unsigned long flags);
extern void salStartAi(void);
static void snd_handle_irq() {
  u8 r;
  u8 i;
  u8 v;
  if (sndActive == 0) {
    return;
  }
  streamCorrectLoops();
  hwIRQEnterCritical();
  salCtrlDsp(salAiGetDest());
  hwIRQLeaveCritical();
  hwIRQEnterCritical();
  salHandleAuxProcessing();
  hwIRQLeaveCritical();
  hwIRQEnterCritical();
  salFrame ^= 1;
  salAuxFrame = (salAuxFrame + 1) % 3;
  for (v = 0; v < salNumVoices; ++v) {
    for (i = 0; i < 5; ++i) {
      dspVoice[v].changed[i] = 0;
    }
  }
  hwIRQLeaveCritical();
  for (r = 0; r < 5; ++r) {
    hwIRQEnterCritical();
    hwSetTimeOffset(r);
    seqHandle(256);
    synthHandle(256);
    hwIRQLeaveCritical();
  }
  hwIRQEnterCritical();
  hwSetTimeOffset(0);
  s3dHandle();
  hwIRQLeaveCritical();
  hwIRQEnterCritical();
  streamHandle();
  hwIRQLeaveCritical();
  hwIRQEnterCritical();
  vsSampleUpdates();
  hwIRQLeaveCritical();
}
signed long hwInit(unsigned long *frq, u16 numVoices, u16 numStudios, unsigned long flags) {

  hwInitIrq();
  salFrame = 0;
  salAuxFrame = 0;
  salMessageCallback = 0;
  if (salInitAi(snd_handle_irq, flags, frq) != 0) {

    if (salInitDspCtrl(numVoices, numStudios, (flags & 1) != 0) != 0) {

      if (salInitDsp(flags)) {

        hwEnableIrq();

        salStartAi();

        return 0;
      }

    }
  }
  return -1;
}



void hwSetTimeOffset(u8 offset)
{
    salTimeOffset = offset;
}

u8 hwGetTimeOffset(void)
{
    return salTimeOffset;
}

u32 hwIsActive(u32 v)
{
    return dspVoice[v].state != DSP_VOICE_STATE_INACTIVE;
}

void hwSetMesgCallback(u32 (*callback)(u32, u32))
{
    salMessageCallback = callback;
}

void hwSetPriority(u32 v, u32 prio)
{
    dspVoice[v].prio = prio;
}

void hwBreak(s32 v)
{
    if (dspVoice[v].state == DSP_VOICE_STATE_STARTING && salTimeOffset == 0)
        dspVoice[v].startupBreak = 1;
    dspVoice[v].changed[salTimeOffset] |= 0x20;
}

void hwSetVirtualSampleLoopBuffer(u32 v, void *start, u32 length)
{
    dspVoice[v].vSampleInfo.loopBufferAddr = start;
    dspVoice[v].vSampleInfo.loopBufferLength = length;
}

u32 hwGetVirtualSampleState(u32 v)
{
    return dspVoice[v].vSampleInfo.inLoopBuffer;
}

u8 hwGetSampleType(u32 v)
{
    return dspVoice[v].smp_info.compType;
}

u16 hwGetSampleID(u32 v)
{
    return dspVoice[v].smp_id;
}

void hwSetStreamLoopPS(u32 v, u8 ps)
{
    dspVoice[v].streamLoopPS = ps;
}

void hwStart(u32 v, u8 studio)
{
    dspVoice[v].singleOffset = salTimeOffset;
    salActivateVoice(&dspVoice[v], studio);
}

void hwKeyOff(u32 v)
{
    dspVoice[v].changed[salTimeOffset] |= 0x40;
}

extern void hwSetSRCType(u32 voice, u8 type);
extern void hwSetPolyPhaseFilter(u32 voice, u8 select);
extern void hwSetITDMode(u32 voice, u8 mode);
void hwInitSamplePlayback(u32 v, u16 smpID, void *newsmp, u32 set_defadsr, u32 prio,
                          u32 callbackUserValue, u32 setSRC, u8 itdMode) {
  unsigned char i;
  unsigned long bf;
  bf = 0;
  for (i = 0; i <= salTimeOffset; ++i) {
    bf |= dspVoice[v].changed[i] & 0x20;
    dspVoice[v].changed[i] = 0;
  }
  dspVoice[v].changed[0] = bf;
  dspVoice[v].prio = prio;
  dspVoice[v].mesgCallBackUserValue = callbackUserValue;
  dspVoice[v].flags = 0;
  dspVoice[v].smp_id = smpID;
  dspVoice[v].smp_info = *(SAMPLE_RESULT *)newsmp;
  if (set_defadsr != 0) {
    dspVoice[v].adsr.mode = ADSR_MODE_LINEAR;
    dspVoice[v].adsr.attackTime = 0;
    dspVoice[v].adsr.decayTime = 0;
    dspVoice[v].adsr.sustain = 0x7FFF;
    dspVoice[v].adsr.releaseTime = 0;
  }
  dspVoice[v].lastUpdate.pitch = 0xff;
  dspVoice[v].lastUpdate.vol = 0xff;
  dspVoice[v].lastUpdate.volA = 0xff;
  dspVoice[v].lastUpdate.volB = 0xff;
  if (setSRC != 0) {
    hwSetSRCType(v, 0);
    hwSetPolyPhaseFilter(v, 1);
  }
  hwSetITDMode(v, itdMode);
}
void hwSetSRCType(u32 v, u8 type)
{
    static u16 dspSRCType[3] = {0, 1, 2};
    DSPvoice *voice = &dspVoice[v];
    voice->srcTypeSelect = dspSRCType[type];
    voice->changed[0] |= 0x100;
}

void hwSetPolyPhaseFilter(u32 v, u8 select)
{
    static u16 dspCoefSel[3] = {0, 1, 2};
    DSPvoice *voice = &dspVoice[v];
    voice->srcCoefSelect = dspCoefSel[select];
    voice->changed[0] |= 0x80;
}

void hwSetITDMode(u32 v, u8 mode)
{
    if (!mode) {
        dspVoice[v].flags |= 0x80000000;
        dspVoice[v].itdShiftL = 16;
        dspVoice[v].itdShiftR = 16;
        return;
    }
    dspVoice[v].flags &= ~0x80000000;
}

void hwOff(s32 v)
{
    salDeactivateVoice(&dspVoice[v]);
}

void hwActivateStudio(u8 studio, u32 isMaster, u32 type)
{
    salActivateStudio(studio, isMaster, type);
}

void hwDeactivateStudio(u8 studio)
{
    salDeactivateStudio(studio);
}

u32 hwAddInput(u8 studio, void *input)
{
    return salAddStudioInput(&dspStudio[studio], input);
}

u32 hwRemoveInput(u8 studio, void *input)
{
    return salRemoveStudioInput(&dspStudio[studio], input);
}

u32 hwGetStreamPlayBuffer(u8 id)
{
    return aramGetStreamBufferAddress(id, 0);
}

void *hwTransAddr(void *samples)
{
    return samples;
}

u32 hwFrq2Pitch(u32 frequency)
{
    return 4096.0f * frequency / synthInfo.mixFrq;
}

void hwInitSampleMem(u32 base, u32 length)
{
    aramInit(length);
}



void hwSyncSampleMem(void)
{
    aramSyncTransferQueue();
}

void hwFrameDone(void)
{
}

void hwDisableHRTF(void)
{
    dspHRTFOn = 0;
}

u32 hwGetVirtualSampleID(u32 v)
{
    if (dspVoice[v].state == DSP_VOICE_STATE_INACTIVE)
        return -1;
    return dspVoice[v].virtualSampleID;
}

u32 hwVoiceInStartup(u32 v)
{
    return dspVoice[v].state == DSP_VOICE_STATE_STARTING;
}

void hwSetPitch(u32 index, u16 pitch)
{
    DSPvoice *voice = &dspVoice[index];
    if (pitch >= 0x4000)
        pitch = 0x3FFF;
    if (voice->lastUpdate.pitch != 0xFF && voice->pitch[voice->lastUpdate.pitch] == (u32)(pitch << 4))
        return;
    voice->pitch[salTimeOffset] = pitch << 4;
    voice->changed[salTimeOffset] |= 8;
    voice->lastUpdate.pitch = salTimeOffset;
}

extern u32 aramStoreData(u32 address, u32 length);
extern void aramRemoveData(void *address, u32 length);

static inline u32 SampleMemorySize(u32 length, u32 format)
{
    switch (format) {
    case SAMPLE_TYPE_ADPCM:
    case SAMPLE_TYPE_ADPCM_PLUS:
    case SAMPLE_TYPE_ADPCM_STREAM:
    case SAMPLE_TYPE_ADPCM_VIRTUAL:
        length = ((length + 13) / 14) * 8;
        break;
    case SAMPLE_TYPE_PCM16:
        length *= 2;
        break;
    }
    return length;
}
void hwSaveSample(SAMPLE_INFO **info, u32 *address)
{
    *address = aramStoreData(*address, SampleMemorySize((*info)->length & 0xFFFFFF, (*info)->length >> 24));
}
void hwRemoveSample(SAMPLE_INFO *info, u32 address)
{
    aramRemoveData((void *)address, SampleMemorySize(info->length & 0xFFFFFF, info->length >> 24));
}

#define SND_STREAM_ADPCM_BLKSIZE 14
extern u8 dspScale2IndexTab[];
extern u32 adsrConvertTimeCents(s32 time);

void hwSetADSR(u32 v, void *_adsr, u8 mode) {
  u32 sl;
  ADSR_INFO *adsr = (ADSR_INFO *)_adsr;
  switch (mode) {
  case 0: {
    dspVoice[v].adsr.mode = ADSR_MODE_LINEAR;
    dspVoice[v].adsr.attackTime = adsr->data.linear.atime;
    dspVoice[v].adsr.decayTime = adsr->data.linear.dtime;
    if ((sl = adsr->data.linear.slevel << 3) > 0x7fff) {
      sl = 0x7fff;
    }
    dspVoice[v].adsr.sustain = sl;
    dspVoice[v].adsr.releaseTime = adsr->data.linear.rtime;
    break;
  }
  case 1:
  case 2:
    dspVoice[v].adsr.mode = ADSR_MODE_DLS;
    dspVoice[v].adsr.attackMode = 0;
    if (mode == 1) {
      dspVoice[v].adsr.attackTime = adsrConvertTimeCents(adsr->data.dls.atime) & 0xFFFF;
      dspVoice[v].adsr.decayTime = adsrConvertTimeCents(adsr->data.dls.dtime) & 0xFFFF;
      sl = adsr->data.dls.slevel >> 2;
      if (sl > 0x3ff) {
        sl = 0x3ff;
      }
      dspVoice[v].adsr.sustain = 193 - dspScale2IndexTab[sl];
    } else {
      dspVoice[v].adsr.attackTime = adsr->data.dls.atime & 0xFFFF;
      dspVoice[v].adsr.decayTime = adsr->data.dls.dtime & 0xFFFF;
      dspVoice[v].adsr.sustain = adsr->data.dls.slevel;
    }
    dspVoice[v].adsr.releaseTime = adsr->data.dls.rtime;
  }
  dspVoice[v].changed[0] |= 0x10;
}

u32 hwGetPos(u32 v) {
  unsigned long pos;
  unsigned long off;
  if (dspVoice[v].state != DSP_VOICE_STATE_PLAYING) {
    return 0;
  }
  switch (dspVoice[v].smp_info.compType) {
  case SAMPLE_TYPE_ADPCM:
  case SAMPLE_TYPE_ADPCM_PLUS:
  case SAMPLE_TYPE_ADPCM_STREAM:
  case SAMPLE_TYPE_ADPCM_VIRTUAL:
    pos = ((dspVoice[v].currentAddr - (u32)dspVoice[v].smp_info.address * 2) / 16) *
          SND_STREAM_ADPCM_BLKSIZE;
    off = dspVoice[v].currentAddr & 0xf;
    if (off >= 2) {
      pos += off - 2;
    }
    break;
  case SAMPLE_TYPE_PCM8:
    pos = dspVoice[v].currentAddr - (u32)dspVoice[v].smp_info.address;
    break;
  case SAMPLE_TYPE_PCM16:
    pos = dspVoice[v].currentAddr - ((u32)dspVoice[v].smp_info.address / 2);
    break;
  }
  return pos;
}

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/hardware.c (CC0-1.0). */
typedef struct SAL_VOLINFO {
    f32 volL, volR, volS, volAuxAL, volAuxAR, volAuxAS, volAuxBL, volAuxBR, volAuxBS;
} SAL_VOLINFO;
extern void salCalcVolume(u8 table, SAL_VOLINFO *info, f32 volume, u32 pan, u32 span, f32 auxA, f32 auxB, u32 itd, u32 dpl2);
static volatile const u16 itdOffTab[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8,
    9, 9, 9, 10, 10, 10, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 15, 15, 15, 16, 16, 17,
    17, 17, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 24, 25,
    25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30,
    31, 31, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
};
static inline void SetupITD(DSPvoice *dsp_vptr, u8 pan) {
  dsp_vptr->itdShiftL = ((u16*)itdOffTab)[pan];
  dsp_vptr->itdShiftR = 32 - itdOffTab[pan];
  dsp_vptr->changed[0] |= 0x200;
}

void hwSetVolume(u32 v, u8 table, float vol, u32 pan, u32 span, float auxa, float auxb) {
  SAL_VOLINFO vi;
  u16 il;
  u16 ir;
  u16 is;
  DSPvoice *dsp_vptr = &dspVoice[v];
  if (vol >= 1.f) {
    vol = 1.f;
  }
  if (auxa >= 1.f) {
    auxa = 1.f;
  }
  if (auxb >= 1.f) {
    auxb = 1.f;
  }
  salCalcVolume(table, &vi, vol, pan, span, auxa, auxb, (dsp_vptr->flags & 0x80000000) != 0,
                dspStudio[dsp_vptr->studio].type == SND_STUDIO_TYPE_DPL2);
  il = 32767.f * vi.volL;
  ir = 32767.f * vi.volR;
  is = 32767.f * vi.volS;
  if (dsp_vptr->lastUpdate.vol == 0xff || dsp_vptr->volL != il || dsp_vptr->volR != ir ||
      dsp_vptr->volS != is) {
    dsp_vptr->volL = il;
    dsp_vptr->volR = ir;
    dsp_vptr->volS = is;
    dsp_vptr->changed[0] |= 1;
    dsp_vptr->lastUpdate.vol = 0;
  }
  il = 32767.f * vi.volAuxAL;
  ir = 32767.f * vi.volAuxAR;
  is = 32767.f * vi.volAuxAS;
  if (dsp_vptr->lastUpdate.volA == 0xff || dsp_vptr->volLa != il || dsp_vptr->volRa != ir ||
      dsp_vptr->volSa != is) {
    dsp_vptr->volLa = il;
    dsp_vptr->volRa = ir;
    dsp_vptr->volSa = is;
    dsp_vptr->changed[0] |= 2;
    dsp_vptr->lastUpdate.volA = 0;
  }
  il = 32767.f * vi.volAuxBL;
  ir = 32767.f * vi.volAuxBR;
  is = 32767.f * vi.volAuxBS;
  if (dsp_vptr->lastUpdate.volB == 0xff || dsp_vptr->volLb != il || dsp_vptr->volRb != ir ||
      dsp_vptr->volSb != is) {
    dsp_vptr->volLb = il;
    dsp_vptr->volRb = ir;
    dsp_vptr->volSb = is;
    dsp_vptr->changed[0] |= 4;
    dsp_vptr->lastUpdate.volB = 0;
  }
  if ((dsp_vptr->flags & 0x80000000)) {
    SetupITD(dsp_vptr, (pan >> 16));
  }
}

extern void DCStoreRange(void *buffer, u32 length);
extern void aramUploadData(u32 source, u32 destination, u32 length, u32 priority,
                           void (*callback)(u32), u32 user);

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/hardware.c (CC0-1.0). */
void hwFlushStream(void *base, u32 offset, u32 bytes, u8 handle,
                   void (*callback)(u32), u32 user)
{
    u32 aram, mram, length;
    aram = aramGetStreamBufferAddress(handle, &length);
    bytes += offset & 31;
    offset &= ~31;
    bytes = (bytes + 31) & ~31;
    mram = (u32)base + offset;
    DCStoreRange((void *)mram, bytes);
    aramUploadData(mram, aram + offset, bytes, 1, callback, user);
}
