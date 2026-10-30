#include "types.h"

#include "musyx_dsp.h"
#include "musyx_hardware.h"
extern void *dspHrtfHistoryBuffer;
extern u32 (*salMessageCallback)(u32 message, u32 user);
extern void *memset(void *dst, s32 value, u32 size);
extern void DCFlushRangeNoSync(void *address, u32 size);

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/hw_dspctrl.c (CC0-1.0). */
#define NULL 0
#define TRUE 1
#define FALSE 0
extern u8 salNumVoices, salMaxStudioNum, salAuxFrame;
extern u32 dspARAMZeroBuffer;
extern u16 *dspCmdList;
extern s32 *dspSurround;
extern s16 *dspITDBuffer;
extern DSPvoice *dspVoice;
DSPstudioinfo dspStudio[8];
extern u32 aramGetZeroBuffer(void);
extern void DCFlushRange(void *address, u32 length);
extern void DCStoreRangeNoSync(void *address, u32 length);
extern void DCInvalidateRange(void *address, u32 length);
extern void salActivateStudio(u8 studio, unsigned long isMaster, SND_STUDIO_TYPE type);
extern void salInitHRTFBuffer(void);
unsigned long salInitDspCtrl(u8 numVoices, u8 numStudios, unsigned long defaultStudioDPL2) {
  unsigned long i;
  unsigned long j;
  unsigned long itdPtr;
  salNumVoices = numVoices;
  salMaxStudioNum = numStudios;

  dspARAMZeroBuffer = aramGetZeroBuffer();
  if ((dspCmdList = salMalloc(1024 * sizeof(u16)))) {

    if ((dspSurround = salMalloc(160 * sizeof(s32)))) {

      memset(dspSurround, 0, 160 * sizeof(s32));
      DCFlushRange(dspSurround, 160 * sizeof(long));
      if ((dspVoice = salMalloc(salNumVoices * sizeof(DSPvoice)))) {

        if ((dspITDBuffer = salMalloc(salNumVoices * 64))) {

          DCInvalidateRange(dspITDBuffer, salNumVoices * 64);
          itdPtr = (unsigned long)dspITDBuffer;
          for (i = 0; i < salNumVoices; ++i) {

            dspVoice[i].state = DSP_VOICE_STATE_INACTIVE;
            dspVoice[i].postBreak = 0;
            dspVoice[i].startupBreak = 0;
            dspVoice[i].lastUpdate.pitch = 0xff;
            dspVoice[i].lastUpdate.vol = 0xff;
            dspVoice[i].lastUpdate.volA = 0xff;
            dspVoice[i].lastUpdate.volB = 0xff;
            dspVoice[i].pb = salMalloc(sizeof(_PB));
            memset(dspVoice[i].pb, 0, sizeof(_PB));
            dspVoice[i].patchData = salMalloc(0x80);
            dspVoice[i].pb->currHi = ((unsigned long)dspVoice[i].pb >> 16);
            dspVoice[i].pb->currLo = (u16)dspVoice[i].pb;
            dspVoice[i].pb->update.dataHi = ((unsigned long)dspVoice[i].patchData >> 16);
            dspVoice[i].pb->update.dataLo = ((u16)dspVoice[i].patchData);
            dspVoice[i].pb->itd.bufferHi = ((unsigned long)itdPtr >> 16);
            dspVoice[i].pb->itd.bufferLo = ((u16)itdPtr);
            dspVoice[i].itdBuffer = (void *)itdPtr;
            itdPtr += 0x40;
            dspVoice[i].virtualSampleID = 0xFFFFFFFF;
            DCStoreRangeNoSync(dspVoice[i].pb, sizeof(_PB));
            for (j = 0; j < 5; ++j) {
              dspVoice[i].changed[j] = 0;
            }
          }

          for (i = 0; i < salMaxStudioNum; ++i) {

            dspStudio[i].state = DSP_STUDIO_STATE_INACTIVE;
            if (!(dspStudio[i].spb = (_SPB *)salMalloc(sizeof(_SPB)))) {
              return FALSE;
            }
            if (!(dspStudio[i].main[0] = (void *)salMalloc(0x3c00))) {
              return FALSE;
            }
            memset(dspStudio[i].main[0], 0, 0x3c00);
            DCFlushRangeNoSync(dspStudio[i].main[0], 0x3c00);
            dspStudio[i].main[1] = dspStudio[i].main[0] + 0x1e0;
            dspStudio[i].auxA[0] = dspStudio[i].main[1] + 0x1e0;
            dspStudio[i].auxA[1] = dspStudio[i].auxA[0] + 0x1e0;
            dspStudio[i].auxA[2] = dspStudio[i].auxA[1] + 0x1e0;
            dspStudio[i].auxB[0] = dspStudio[i].auxA[2] + 0x1e0;
            dspStudio[i].auxB[1] = dspStudio[i].auxB[0] + 0x1e0;
            dspStudio[i].auxB[2] = dspStudio[i].auxB[1] + 0x1e0;
            memset(dspStudio[i].spb, 0, sizeof(_SPB));
            dspStudio[i].hostDPopSum.l = dspStudio[i].hostDPopSum.r = dspStudio[i].hostDPopSum.s =
                0;
            dspStudio[i].hostDPopSum.lA = dspStudio[i].hostDPopSum.rA =
                dspStudio[i].hostDPopSum.sA = 0;
            dspStudio[i].hostDPopSum.lB = dspStudio[i].hostDPopSum.rB =
                dspStudio[i].hostDPopSum.sB = 0;
            DCFlushRangeNoSync(dspStudio[i].spb, sizeof(_SPB));
          }

          salActivateStudio(
              0, 1, defaultStudioDPL2 != FALSE ? SND_STUDIO_TYPE_DPL2 : SND_STUDIO_TYPE_STD);

          if (!(dspHrtfHistoryBuffer = salMalloc(0x100))) {
            return FALSE;
          }
          salInitHRTFBuffer();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

void salInitHRTFBuffer(void)
{
    memset(dspHrtfHistoryBuffer, 0, 0x100);
    DCFlushRangeNoSync(dspHrtfHistoryBuffer, 0x100);
}

extern DSPstudioinfo dspStudio[8];


#define HW_MESSAGE_VOICE_KILL 1
#define HW_MESSAGE_VIRTUAL_SAMPLE_START 2
typedef struct SNDADPCMinfo {
    u16 numCoef;
    u8 initialPS, loopPS;
    s16 loopY0, loopY1;
    s16 coefTab[8][2];
} SNDADPCMinfo;
typedef struct DSPADPCMblock {
    s16 Y0, Y1;
    u8 PS, reserved;
} DSPADPCMblock;
typedef struct DSPADPCMplusInfo {
    u16 numCoef;
    u8 initialPS, loopPS;
    s16 loopY0, loopY1;
    s16 coefTab[8][2];
    DSPADPCMblock blk[1];
} DSPADPCMplusInfo;
u32 adsrSetup(ADSR_VARS *adsr);
u32 adsrStartRelease(ADSR_VARS *adsr, u32 rtime);
u32 adsrHandle(ADSR_VARS *adsr, u16 *start, u16 *delta);
#define HW_MESSAGE_VIRTUAL_SAMPLE_END 3
#define HW_MESSAGE_SAMPLE_END 0
extern unsigned long __OSBusClock : 0x800000f8;
extern u8 salFrame;
extern u32 dspHRTFOn;
extern u16 *dspCmdCurBase, *dspCmdPtr, *dspCmdMaxPtr, *dspCmdLastLoad, *dspCmdLastBase;
extern u16 dspCmdLastSize, dspCmdFirstSize;
void salDeactivateVoice(DSPvoice *voice);
u32 salSynthSendMessage(DSPvoice *voice,u32 message);
enum DSPCommand {
    DSP_CMD_SETUP = 0,
    DSP_CMD_DL_AND_VOL_MIX = 1,
    DSP_CMD_PB_ADDR = 2,
    DSP_CMD_PROCESS = 3,
    DSP_CMD_MIX_AUXA = 4,
    DSP_CMD_MIX_AUXB = 5,
    DSP_CMD_UPLOAD_LRS = 6,
    DSP_CMD_MIX_AUXB_NOWRITE = 9,
    DSP_CMD_MORE = 13,
    DSP_CMD_OUTPUT = 14,
    DSP_CMD_END = 15,
    DSP_CMD_MIX_AUXB_LR = 16,
    DSP_CMD_SET_OPPOSITE_LR = 17
};
static u16 dspSRCCycles[3][3] = {
    {2990, 2990, 1115},
    {3300, 3300, 1115},
    {3700, 3700, 1115},
};
static const u16 dspMixerCycles[32] = {
    1470, 2940, 2940, 4410, 2230, 4460, 4460, 6690, 2470, 4940, 4940, 7410, 3735, 7470, 7470, 11205,
    2940, 3386, 2940, 3386, 2940, 3386, 2940, 3386, 4940, 5687, 4940, 5687, 4940, 5687, 4940, 5687,
};

static inline void AddDpop(s32 *sum, s16 delta)
{
    *sum += (int)delta;
    *sum = (*sum > 0x7fffff) ? 0x7fffff : (*sum < -0x7fffff ? -0x7fffff : *sum);
}

#pragma force_active on
static u32 salCheckVolErrorAndResetDelta(u16 *dsp_vol, u16 *dsp_delta, u16 *last_vol,
                                        u16 targetVol, u16 *resetFlags, u16 resetMask)
{
    s16 d;
    s16 x;
    if (targetVol != *last_vol) {
        d = (s16)targetVol - (s16)*last_vol;
        if ((s16)d >= 32 && (s16)d < 160) {
            x = (s16)d >> 5;
            if ((s16)x < 5) {
                resetFlags[x] |= resetMask;
            }
            *dsp_delta = 1;
            *last_vol += (x << 5);
            return 1;
        }
        if (-32 >= (s16)d && -160 < (s16)d) {
            x = -(s16)d >> 5;
            if (x < 5) {
                resetFlags[x] |= resetMask;
            }
            *dsp_delta = 0xFFFF;
            *last_vol -= x << 5;
            return 1;
        }
        if (targetVol == 0 && (s16)d > -32) {
            *dsp_vol = *last_vol = 0;
        }
    }
    *dsp_delta = 0;
    return 0;
}

static void HandleDepopVoice(DSPstudioinfo *stp, DSPvoice *dsp_vptr)
{
    _PB *pb;
    dsp_vptr->postBreak = 0;
    dsp_vptr->pb->state = DSP_PB_STATE_STOPPED;
    pb = dsp_vptr->pb;
    AddDpop(&stp->hostDPopSum.l, pb->dpop.aL);
    AddDpop(&stp->hostDPopSum.r, pb->dpop.aR);
    if ((pb->mixerCtrl & 0x04) != 0) {
        AddDpop(&stp->hostDPopSum.s, pb->dpop.aS);
    }
    if ((pb->mixerCtrl & 0x01) != 0) {
        AddDpop(&stp->hostDPopSum.lA, pb->dpop.aAuxAL);
        AddDpop(&stp->hostDPopSum.rA, pb->dpop.aAuxAR);
        if ((pb->mixerCtrl & 0x14) != 0) {
            AddDpop(&stp->hostDPopSum.sA, pb->dpop.aAuxAS);
        }
    }
    if ((pb->mixerCtrl & 0x12) != 0) {
        AddDpop(&stp->hostDPopSum.lB, pb->dpop.aAuxBL);
        AddDpop(&stp->hostDPopSum.rB, pb->dpop.aAuxBR);
        if ((pb->mixerCtrl & 0x4) != 0) {
            AddDpop(&stp->hostDPopSum.sB, pb->dpop.aAuxBS);
        }
    }
}

static void SortVoices(DSPvoice **voices, long l, long r)
{
    long i;
    long last;
    DSPvoice *tmp;
    if (l >= r) {
        return;
    }
    tmp = voices[l];
    voices[l] = voices[(l + r) / 2];
    voices[(l + r) / 2] = tmp;
    last = l;
    i = l + 1;
    for (; i <= r; ++i) {
        if (voices[i]->prio < voices[l]->prio) {
            last += 1;
            tmp = voices[last];
            voices[last] = voices[i];
            voices[i] = tmp;
        }
    }
    tmp = voices[l];
    voices[l] = voices[last];
    voices[last] = tmp;
    SortVoices(voices, l, last - 1);
    SortVoices(voices, last + 1, r);
}
#pragma force_active reset

static inline void sal_setup_dspvol(u16 *dsp_delta, u16 *last_vol, u16 vol) {
  *dsp_delta = ((s16)vol - (s16)*last_vol) / 160;
  *last_vol += (s16)*dsp_delta * 160;
}
static inline void sal_update_hostplayinfo(DSPvoice *dsp_vptr) {
  u32 old_lo;
  u32 pitch;
  if (dsp_vptr->smp_info.loopLength != 0) {
    return;
  }
  if (dsp_vptr->pb->srcSelect != 2) {
    pitch = dsp_vptr->playInfo.pitch << 5;
  } else {
    pitch = 0x200000;
  }
  old_lo = dsp_vptr->playInfo.posLo;
  dsp_vptr->playInfo.posLo += pitch * 0x10000;
  if (old_lo > dsp_vptr->playInfo.posLo) {
    dsp_vptr->playInfo.posHi += (pitch >> 16) + 1;
  } else {
    dsp_vptr->playInfo.posHi += (pitch >> 16);
  }
}
static inline void DoDepopFade(long *dspStart, s16 *dspDelta, s32 *hostSum) {
  if (*hostSum <= -160) {
    *dspDelta = (*hostSum <= -3200) ? 0x14 : (s16)(-*hostSum / 160);
  } else if (*hostSum >= 160) {
    *dspDelta = (*hostSum >= 3200) ? -0x14 : (s16)(-*hostSum / 160);
  } else {
    *dspDelta = 0;
  }
  *dspStart = *hostSum;
  *hostSum += *dspDelta * 160;
}
void salBuildCommandList(s16 *dest, unsigned long nsDelay) {
  static const u16 pbOffsets[9] = {10, 12, 24, 14, 16, 26, 18, 20, 22};
  static DSPvoice *voices[64];
  u8 s;
  u8 mix_start;
  u8 st;
  u8 st1;
  u8 getAuxFrame;
  u8 i;
  u16 rampResetOffsetFlags[5];
  DSPvoice *dsp_vptr;
  DSPvoice *next_dsp_vptr;
  unsigned long tmp_addr;
  unsigned long addr;
  unsigned long base;
  unsigned long in;
  unsigned long voiceNum;
  unsigned long cyclesUsed;
  u16 *pptr;
  u16 *pend;
  u16 adsr_start;
  u16 adsr_delta;
  u16 old_adsr_delta;
  signed long current_delta;
  signed long v;
  _PB *pb;
  _PB *last_pb;
  unsigned long VoiceDone;
  unsigned long needsDelta;
  unsigned long newVoice;
  _SPB *spb;
  DSPstudioinfo *stp;
  unsigned long procVoiceFlag;
  dspCmdCurBase = dspCmdPtr = dspCmdList;
  dspCmdMaxPtr = dspCmdPtr + 0xC0;
  dspCmdLastLoad = NULL;
  if (nsDelay < 200) {
    cyclesUsed = 10430;
  } else {
    cyclesUsed = ((nsDelay - 200) * ((__OSBusClock / 400) / 5000)) + 10430;
  }
  if (dspHRTFOn != FALSE) {
    cyclesUsed += 45000;
  }
  rampResetOffsetFlags[0] = 0;
  for (st = 0; st < salMaxStudioNum; st++) {
    if (dspStudio[st].state == DSP_STUDIO_STATE_ACTIVE) {
      stp = &dspStudio[st];
      for (dsp_vptr = stp->voiceRoot; dsp_vptr; dsp_vptr = next_dsp_vptr) {
        next_dsp_vptr = dsp_vptr->next;
        if ((dsp_vptr->postBreak != 0) || ((dsp_vptr->changed[0] & 0x20) != 0)) {
          HandleDepopVoice(stp, dsp_vptr);
          if (dsp_vptr->virtualSampleID != -1) {
            salSynthSendMessage(dsp_vptr, HW_MESSAGE_VIRTUAL_SAMPLE_END);
          }
          if ((dsp_vptr->state != DSP_VOICE_STATE_STARTING) || (dsp_vptr->startupBreak != 0)) {
            salDeactivateVoice(dsp_vptr);
            dsp_vptr->startupBreak = 0;
          }
        }
      }
      dsp_vptr = stp->alienVoiceRoot;
      while (dsp_vptr) {
        HandleDepopVoice(stp, dsp_vptr);
        dsp_vptr = dsp_vptr->nextAlien;
      }
      stp->alienVoiceRoot = NULL;
      if ((dspCmdPtr + 3) > (dspCmdMaxPtr - 4)) {
        u16 size;
        dspCmdPtr[0] = DSP_CMD_MORE;
        dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
        dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
        size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
        if (dspCmdLastLoad) {
          dspCmdLastLoad[3] = size;
          DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
        } else {
          dspCmdFirstSize = size;
        }
        dspCmdLastLoad = dspCmdPtr;
        dspCmdLastSize = size;
        dspCmdLastBase = dspCmdCurBase;
        dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
        dspCmdMaxPtr = dspCmdPtr + 0xC0;
      }
      dspCmdPtr[0] = DSP_CMD_SETUP;
      dspCmdPtr[1] = (unsigned long)stp->spb >> 16;
      dspCmdPtr[2] = (unsigned long)stp->spb;
      dspCmdPtr += 3;
      cyclesUsed += 0x2C62;
      for (in = 0; in < stp->numInputs; in++) {
        if ((dspCmdPtr + 6) > (dspCmdMaxPtr - 4)) {
          u16 size;
          dspCmdPtr[0] = DSP_CMD_MORE;
          dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
          dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
          size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
          if (dspCmdLastLoad) {
            dspCmdLastLoad[3] = size;
            DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
          } else {
            dspCmdFirstSize = size;
          }
          dspCmdLastLoad = dspCmdPtr;
          dspCmdLastSize = size;
          dspCmdLastBase = dspCmdCurBase;
          dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
          dspCmdMaxPtr = dspCmdPtr + 0xC0;
        }
        dspCmdPtr[0] = DSP_CMD_DL_AND_VOL_MIX;
        dspCmdPtr[1] = (unsigned long)dspStudio[stp->in[in].studio].main[salFrame ^ 1] >> 16;
        dspCmdPtr[2] = (unsigned long)dspStudio[stp->in[in].studio].main[salFrame ^ 1];
        dspCmdPtr[3] = stp->in[in].vol;
        dspCmdPtr[4] = stp->in[in].volA;
        dspCmdPtr[5] = stp->in[in].volB;
        dspCmdPtr += 6;
        cyclesUsed += 0x294D;
      }
      last_pb = NULL;
      for (v = 0, dsp_vptr = stp->voiceRoot; dsp_vptr; v++, dsp_vptr = dsp_vptr->next) {
        voices[v] = dsp_vptr;
      }
      voiceNum = (int)v;
      SortVoices(voices, 0, voiceNum - 1);
      procVoiceFlag = 0;
      for (v = voiceNum; v > 0; v--) {
        dsp_vptr = voices[v - 1];
        if (dsp_vptr->state != DSP_VOICE_STATE_INACTIVE) {
          pb = dsp_vptr->pb;
          for (s = 1; s < 5; s++) {
            rampResetOffsetFlags[s] = 0;
          }
          if (dsp_vptr->state == DSP_VOICE_STATE_STARTING) {
            dsp_vptr->virtualSampleID = -1;
            dsp_vptr->pb->ve.currentDelta = 0x8000;
            if (adsrSetup(&dsp_vptr->adsr) != 0) {
              salSynthSendMessage(dsp_vptr, HW_MESSAGE_SAMPLE_END);
              salDeactivateVoice(dsp_vptr);
              continue;
            }
            dsp_vptr->virtualSampleID = -1;
            switch (dsp_vptr->smp_info.compType) {
            case SAMPLE_TYPE_ADPCM_VIRTUAL:
              dsp_vptr->vSampleInfo.loopBufferLength = 0;
              dsp_vptr->virtualSampleID =
                  salSynthSendMessage(dsp_vptr, HW_MESSAGE_VIRTUAL_SAMPLE_START);
              if (dsp_vptr->vSampleInfo.loopBufferLength == 0) {
                salSynthSendMessage(dsp_vptr, HW_MESSAGE_VOICE_KILL);
                salDeactivateVoice(dsp_vptr);
                continue;
              }
              break;
            }
            pb->src.currentAddressFrac = 0;
            pb->src.last_samples[0] = 0;
            pb->src.last_samples[1] = 0;
            pb->src.last_samples[2] = 0;
            pb->src.last_samples[3] = 0;
            if ((dsp_vptr->flags & 0x80000000) != 0) {
              memset(dsp_vptr->itdBuffer, 0, 0x40);
              DCFlushRange(dsp_vptr->itdBuffer, 0x40);
              pb->itd.targetShiftL = dsp_vptr->itdShiftL;
              pb->itd.shiftL = dsp_vptr->itdShiftL;
              pb->itd.targetShiftR = dsp_vptr->itdShiftR;
              pb->itd.shiftR = dsp_vptr->itdShiftR;
              pb->itd.flag = 1;
            } else {
              pb->itd.flag = 0;
            }
            switch (dsp_vptr->smp_info.compType) {
            case SAMPLE_TYPE_ADPCM:
            case SAMPLE_TYPE_ADPCM_STREAM:
            case SAMPLE_TYPE_ADPCM_VIRTUAL: {
              SNDADPCMinfo *adpcmInfo;
              u8 i;
              pb->addr.format = 0;
              pb->adpcm.gain = 0;
              adpcmInfo = dsp_vptr->smp_info.extra;
              pb->adpcm.yn2 = 0;
              pb->adpcm.yn1 = 0;
              pb->adpcm.pred_scale = adpcmInfo->initialPS;
              for (i = 0; i < 8; i++) {
                pb->adpcm.a[i][0] = adpcmInfo->coefTab[i][0];
                pb->adpcm.a[i][1] = adpcmInfo->coefTab[i][1];
              }
              addr = (base = (unsigned long)dsp_vptr->smp_info.address * 2) + 2;
              dsp_vptr->playInfo.posHi = dsp_vptr->playInfo.posLo = 0;
              if ((dsp_vptr->smp_info.compType == SAMPLE_TYPE_ADPCM_STREAM) ||
                  (dsp_vptr->smp_info.compType == SAMPLE_TYPE_ADPCM_VIRTUAL)) {
                pb->loopType = 1;
              } else {
                pb->adpcmLoop.loop_yn2 = adpcmInfo->loopY0;
                pb->adpcmLoop.loop_yn1 = adpcmInfo->loopY1;
                pb->adpcmLoop.loop_pred_scale = adpcmInfo->loopPS;
                pb->loopType = 0;
              }
            } break;
            case SAMPLE_TYPE_ADPCM_PLUS: {
              DSPADPCMplusInfo *adpcmInfo;
              unsigned long offset;
              u8 i;
              pb->addr.format = 0;
              pb->adpcm.gain = 0;
              offset = (dsp_vptr->smp_info.offset + 0xD) / 14;
              adpcmInfo = dsp_vptr->smp_info.extra;
              pb->adpcm.yn2 = adpcmInfo->blk[offset].Y0;
              pb->adpcm.yn1 = adpcmInfo->blk[offset].Y1;
              pb->adpcm.pred_scale = adpcmInfo->blk[offset].PS;
              pb->adpcmLoop.loop_yn2 = adpcmInfo->loopY0;
              pb->adpcmLoop.loop_yn1 = adpcmInfo->loopY1;
              pb->adpcmLoop.loop_pred_scale = adpcmInfo->loopPS;
              for (i = 0; i < 8; i++) {
                pb->adpcm.a[i][0] = adpcmInfo->coefTab[i][0];
                pb->adpcm.a[i][1] = adpcmInfo->coefTab[i][1];
              }
              addr = (base = (unsigned long)dsp_vptr->smp_info.address * 2) + offset * 16 + 2;
              dsp_vptr->playInfo.posHi = offset * 0xE;
              dsp_vptr->playInfo.posLo = 0;
            } break;
            case SAMPLE_TYPE_PCM8: {
              u8 i;
              pb->addr.format = 0x19;
              pb->adpcm.gain = 0x100;
              for (i = 0; i < 8; i++) {
                pb->adpcm.a[i][0] = 0;
                pb->adpcm.a[i][1] = 0;
              }
              addr = (unsigned long)dsp_vptr->smp_info.offset + (base = (unsigned long)dsp_vptr->smp_info.address);
              dsp_vptr->playInfo.posHi = dsp_vptr->smp_info.offset;
              dsp_vptr->playInfo.posLo = 0;
            } break;
            case SAMPLE_TYPE_PCM16:
            {
              u8 i;
              pb->addr.format = 0xA;
              pb->adpcm.gain = 0x800;
              for (i = 0; i < 8; i++) {
                pb->adpcm.a[i][0] = 0;
                pb->adpcm.a[i][1] = 0;
              }
              addr = dsp_vptr->smp_info.offset + (base = (unsigned long)dsp_vptr->smp_info.address >> 1);
              dsp_vptr->playInfo.posHi = dsp_vptr->smp_info.offset;
              dsp_vptr->playInfo.posLo = 0;
            } break;
            default:

              break;
            }
            pb->addr.currentAddressHi = addr >> 0x10;
            pb->addr.currentAddressLo = addr;
            dsp_vptr->currentAddr = addr;
            if (dsp_vptr->smp_info.loopLength != 0) {
              unsigned long endAddr;
              unsigned long loopAddr;
              pb->addr.loopFlag = 1;
              switch (dsp_vptr->smp_info.compType) {
              case SAMPLE_TYPE_ADPCM:
              case SAMPLE_TYPE_ADPCM_PLUS:
              case SAMPLE_TYPE_ADPCM_STREAM: {
                unsigned long bn;
                unsigned long bo;
                bn = dsp_vptr->smp_info.loop / 14;
                bo = dsp_vptr->smp_info.loop - (bn * 0xE);
                loopAddr = base + bn * 16 + 2 + bo;
                endAddr = dsp_vptr->smp_info.loop + dsp_vptr->smp_info.loopLength - 1;
                bn = endAddr / 14;
                bo = endAddr - (bn * 0xE);
                endAddr = base + bn * 16 + 2 + bo;
              } break;
              case SAMPLE_TYPE_ADPCM_VIRTUAL: {
                unsigned long bn;
                unsigned long bo;
                loopAddr = ((unsigned long)dsp_vptr->vSampleInfo.loopBufferAddr * 2) + 2;
                endAddr = dsp_vptr->smp_info.loop + dsp_vptr->smp_info.loopLength - 1;
                bn = endAddr / 14;
                bo = endAddr - (bn * 0xE);
                endAddr = base + bn * 16 + 2 + bo;
                dsp_vptr->vSampleInfo.inLoopBuffer = 0;
              } break;
              case SAMPLE_TYPE_PCM16:
              case SAMPLE_TYPE_PCM8:
              default:
                loopAddr = base + dsp_vptr->smp_info.loop;
                endAddr = base + dsp_vptr->smp_info.loop + dsp_vptr->smp_info.loopLength - 1;
                break;
              }
              pb->addr.loopAddressHi = loopAddr >> 16;
              pb->addr.loopAddressLo = loopAddr;
              pb->addr.endAddressHi = endAddr >> 16;
              pb->addr.endAddressLo = endAddr;
              pb->streamLoopCnt = 0;
            } else {
              unsigned long zeroAddr;
              pb->addr.loopFlag = 0;
              switch (dsp_vptr->smp_info.compType) {
              case SAMPLE_TYPE_ADPCM:
              case SAMPLE_TYPE_ADPCM_PLUS:
              case SAMPLE_TYPE_ADPCM_STREAM:
              case SAMPLE_TYPE_ADPCM_VIRTUAL: {
                unsigned long bn;
                unsigned long bo;
                bn = dsp_vptr->smp_info.length / 14;
                bo = dsp_vptr->smp_info.length - (bn * 0xE);
                tmp_addr = base + bn * 16 + 2 + bo;
                zeroAddr = (dspARAMZeroBuffer * 2) + 2;
              } break;
              case SAMPLE_TYPE_PCM8:
                tmp_addr = base + dsp_vptr->smp_info.length;
                zeroAddr = dspARAMZeroBuffer;
                break;
              case SAMPLE_TYPE_PCM16:
                tmp_addr = base + dsp_vptr->smp_info.length;
                zeroAddr = dspARAMZeroBuffer >> 1;
                break;
              }
              pb->addr.loopAddressHi = zeroAddr >> 16;
              pb->addr.loopAddressLo = zeroAddr;
              pb->addr.endAddressHi = tmp_addr >> 16;
              pb->addr.endAddressLo = tmp_addr;
            }
            pb->srcSelect = dsp_vptr->srcTypeSelect;
            pb->coefSelect = dsp_vptr->srcCoefSelect;
            pb->state =
                (mix_start = dsp_vptr->singleOffset) ? DSP_PB_STATE_STOPPED : DSP_PB_STATE_PLAYING;
            pb->mix.vL = dsp_vptr->lastVolL = dsp_vptr->volL;
            pb->mix.vR = dsp_vptr->lastVolR = dsp_vptr->volR;
            pb->mix.vS = dsp_vptr->lastVolS = dsp_vptr->volS;
            pb->mix.vAuxAL = dsp_vptr->lastVolLa = dsp_vptr->volLa;
            pb->mix.vAuxAR = dsp_vptr->lastVolRa = dsp_vptr->volRa;
            pb->mix.vAuxAS = dsp_vptr->lastVolSa = dsp_vptr->volSa;
            pb->mixerCtrl = (pb->mix.vAuxAS | (pb->mix.vAuxAL | pb->mix.vAuxAR)) != 0 ? 1 : 0;
            pb->mix.vAuxBL = dsp_vptr->lastVolLb = dsp_vptr->volLb;
            pb->mix.vAuxBR = dsp_vptr->lastVolRb = dsp_vptr->volRb;
            pb->mix.vAuxBS = dsp_vptr->lastVolSb = dsp_vptr->volSb;
            pb->mix.vDeltaL = 0;
            pb->mix.vDeltaR = 0;
            pb->mix.vDeltaS = 0;
            pb->mix.vDeltaAuxAL = 0;
            pb->mix.vDeltaAuxAR = 0;
            pb->mix.vDeltaAuxAS = 0;
            pb->mix.vDeltaAuxBL = 0;
            pb->mix.vDeltaAuxBR = 0;
            pb->mix.vDeltaAuxBS = 0;
            if (stp->type == SND_STUDIO_TYPE_STD) {
              if ((pb->mix.vAuxBS | (pb->mix.vAuxBL | pb->mix.vAuxBR)) != 0) {
                pb->mixerCtrl |= 2;
              }
              if ((pb->mix.vAuxBS | (pb->mix.vS | pb->mix.vAuxAS)) != 0) {
                pb->mixerCtrl |= 4;
              }
            } else if ((pb->mix.vAuxAS | (pb->mix.vAuxBL | pb->mix.vAuxBR)) != 0) {
              pb->mixerCtrl |= 0x10;
            }
            dsp_vptr->state = DSP_VOICE_STATE_PLAYING;
            newVoice = 1;
            goto process_voice_frame;
          }
          if ((dsp_vptr->smp_info.compType == SAMPLE_TYPE_ADPCM_STREAM) ||
              (dsp_vptr->smp_info.compType == SAMPLE_TYPE_ADPCM_VIRTUAL)) {
            pb->adpcmLoop.loop_pred_scale = dsp_vptr->streamLoopPS;
            if ((dsp_vptr->smp_info.compType == SAMPLE_TYPE_ADPCM_VIRTUAL) &&
                (dsp_vptr->vSampleInfo.inLoopBuffer == 0) && (pb->streamLoopCnt != 0)) {
              unsigned long bn;
              unsigned long bo;
              bn = (dsp_vptr->vSampleInfo.loopBufferLength - 1) / 14;
              bo = (dsp_vptr->vSampleInfo.loopBufferLength - 1) - (bn * 14);
              tmp_addr = ((unsigned long)dsp_vptr->vSampleInfo.loopBufferAddr * 2) + bn * 16 + 2 + bo;
              dsp_vptr->smp_info.address = (u32)dsp_vptr->vSampleInfo.loopBufferAddr;
              pb->addr.endAddressHi = tmp_addr >> 0x10;
              pb->addr.endAddressLo = tmp_addr;
              dsp_vptr->vSampleInfo.inLoopBuffer = 1;
            }
          }
          if ((dsp_vptr->smp_info.loopLength == 0) &&
              (dsp_vptr->playInfo.posHi >= dsp_vptr->smp_info.length)) {
            salSynthSendMessage(dsp_vptr, HW_MESSAGE_SAMPLE_END);
            salDeactivateVoice(dsp_vptr);
            continue;
          }
          if (((dsp_vptr->changed[0] & 0x10) != 0) && (adsrSetup(&dsp_vptr->adsr) != 0)) {
            salSynthSendMessage(dsp_vptr, HW_MESSAGE_SAMPLE_END);
            salDeactivateVoice(dsp_vptr);
            continue;
          }
          if ((dsp_vptr->changed[0] & 1) != 0) {
            sal_setup_dspvol(&pb->mix.vDeltaL, &dsp_vptr->lastVolL, dsp_vptr->volL);
            sal_setup_dspvol(&pb->mix.vDeltaR, &dsp_vptr->lastVolR, dsp_vptr->volR);
            sal_setup_dspvol(&pb->mix.vDeltaS, &dsp_vptr->lastVolS, dsp_vptr->volS);
            needsDelta = 1;
          } else {
            needsDelta =
                salCheckVolErrorAndResetDelta(&pb->mix.vL, &pb->mix.vDeltaL, &dsp_vptr->lastVolL,
                                              dsp_vptr->volL, rampResetOffsetFlags, 1);
            needsDelta |=
                salCheckVolErrorAndResetDelta(&pb->mix.vR, &pb->mix.vDeltaR, &dsp_vptr->lastVolR,
                                              dsp_vptr->volR, rampResetOffsetFlags, 2);
            needsDelta |=
                salCheckVolErrorAndResetDelta(&pb->mix.vS, &pb->mix.vDeltaS, &dsp_vptr->lastVolS,
                                              dsp_vptr->volS, rampResetOffsetFlags, 4);
          }
          if ((dsp_vptr->changed[0] & 2) != 0) {
            sal_setup_dspvol(&pb->mix.vDeltaAuxAL, &dsp_vptr->lastVolLa, dsp_vptr->volLa);
            sal_setup_dspvol(&pb->mix.vDeltaAuxAR, &dsp_vptr->lastVolRa, dsp_vptr->volRa);
            sal_setup_dspvol(&pb->mix.vDeltaAuxAS, &dsp_vptr->lastVolSa, dsp_vptr->volSa);
            if ((pb->mix.vDeltaAuxAS | (pb->mix.vDeltaAuxAL | pb->mix.vDeltaAuxAR)) != 0) {
              pb->mixerCtrl |= 1;
              needsDelta = 1;
            } else if ((pb->mix.vAuxAS | (pb->mix.vAuxAL | pb->mix.vAuxAR)) != 0) {
              pb->mixerCtrl |= 1;
            } else {
              pb->mixerCtrl &= ~1;
            }
          }
          else if ((pb->mixerCtrl & 1) != 0) {
            unsigned long localNeedsDelta;
            localNeedsDelta = salCheckVolErrorAndResetDelta(&pb->mix.vAuxAL, &pb->mix.vDeltaAuxAL,
                                                            &dsp_vptr->lastVolLa, dsp_vptr->volLa,
                                                            rampResetOffsetFlags, 8);
            localNeedsDelta |= salCheckVolErrorAndResetDelta(&pb->mix.vAuxAR, &pb->mix.vDeltaAuxAR,
                                                             &dsp_vptr->lastVolRa, dsp_vptr->volRa,
                                                             rampResetOffsetFlags, 0x10);
            localNeedsDelta |= salCheckVolErrorAndResetDelta(&pb->mix.vAuxAS, &pb->mix.vDeltaAuxAS,
                                                             &dsp_vptr->lastVolSa, dsp_vptr->volSa,
                                                             rampResetOffsetFlags, 0x20);
            if ((localNeedsDelta | (pb->mix.vAuxAS | (pb->mix.vAuxAL | pb->mix.vAuxAR))) == 0) {
              pb->mixerCtrl &= ~1;
            } else {
              needsDelta = 1;
            }
          } else {
            pb->mix.vDeltaAuxAL = 0;
            pb->mix.vDeltaAuxAR = 0;
            pb->mix.vDeltaAuxAS = 0;
          }
          if ((dsp_vptr->changed[0] & 4) != 0) {
            if (stp->type == SND_STUDIO_TYPE_STD) {
              sal_setup_dspvol(&pb->mix.vDeltaAuxBL, &dsp_vptr->lastVolLb, dsp_vptr->volLb);
              sal_setup_dspvol(&pb->mix.vDeltaAuxBR, &dsp_vptr->lastVolRb, dsp_vptr->volRb);
              sal_setup_dspvol(&pb->mix.vDeltaAuxBS, &dsp_vptr->lastVolSb, dsp_vptr->volSb);
              if ((pb->mix.vDeltaAuxBS | (pb->mix.vDeltaAuxBL | pb->mix.vDeltaAuxBR)) != 0) {
                pb->mixerCtrl |= 2;
                needsDelta = 1;
              } else if ((pb->mix.vAuxBS | (pb->mix.vAuxBL | pb->mix.vAuxBR)) != 0) {
                pb->mixerCtrl |= 2;
              } else {
                pb->mixerCtrl &= ~2;
              }
            } else {
              sal_setup_dspvol(&pb->mix.vDeltaAuxBL, &dsp_vptr->lastVolLb, dsp_vptr->volLb);
              sal_setup_dspvol(&pb->mix.vDeltaAuxBR, &dsp_vptr->lastVolRb, dsp_vptr->volRb);
              if ((pb->mix.vDeltaAuxBL | pb->mix.vDeltaAuxBR) != 0) {
                pb->mixerCtrl |= 0x10;
                needsDelta = 1;
              } else if ((pb->mix.vAuxBL | pb->mix.vAuxBR) != 0) {
                pb->mixerCtrl |= 0x10;
              } else {
                pb->mixerCtrl &= ~0x10;
              }
            }
          } else if (stp->type == SND_STUDIO_TYPE_STD) {
            if ((pb->mixerCtrl & 2) != 0) {
              unsigned long localNeedsDelta;
              localNeedsDelta = salCheckVolErrorAndResetDelta(&pb->mix.vAuxBL, &pb->mix.vDeltaAuxBL,
                                                              &dsp_vptr->lastVolLb, dsp_vptr->volLb,
                                                              rampResetOffsetFlags, 0x40);
              localNeedsDelta |= salCheckVolErrorAndResetDelta(
                  &pb->mix.vAuxBR, &pb->mix.vDeltaAuxBR, &dsp_vptr->lastVolRb, dsp_vptr->volRb,
                  rampResetOffsetFlags, 0x80);
              localNeedsDelta |= salCheckVolErrorAndResetDelta(
                  &pb->mix.vAuxBS, &pb->mix.vDeltaAuxBS, &dsp_vptr->lastVolSb, dsp_vptr->volSb,
                  rampResetOffsetFlags, 0x100);
              if ((localNeedsDelta | (pb->mix.vAuxBS | (pb->mix.vAuxBL | pb->mix.vAuxBR))) == 0) {
                pb->mixerCtrl &= ~2;
              } else {
                needsDelta = 1;
              }
            } else {
              pb->mix.vDeltaAuxBL = 0;
              pb->mix.vDeltaAuxBR = 0;
              pb->mix.vDeltaAuxBS = 0;
            }
          } else if ((pb->mixerCtrl &
                      0x10) != 0) {
            unsigned long localNeedsDelta;
            localNeedsDelta = salCheckVolErrorAndResetDelta(&pb->mix.vAuxBL, &pb->mix.vDeltaAuxBL,
                                                            &dsp_vptr->lastVolLb, dsp_vptr->volLb,
                                                            rampResetOffsetFlags, 0x40);
            localNeedsDelta |= salCheckVolErrorAndResetDelta(&pb->mix.vAuxBR, &pb->mix.vDeltaAuxBR,
                                                             &dsp_vptr->lastVolRb, dsp_vptr->volRb,
                                                             rampResetOffsetFlags, 0x80);
            if ((localNeedsDelta | (pb->mix.vAuxBL | pb->mix.vAuxBR)) == 0) {
              if ((pb->mix.vAuxAS | pb->mix.vDeltaAuxAS) == 0) {
                pb->mixerCtrl &= ~0x10;
              }
            } else {
              needsDelta = 1;
            }
          } else {
            pb->mix.vDeltaAuxBL = 0;
            pb->mix.vDeltaAuxBR = 0;
          }
          if (needsDelta != 0) {
            pb->mixerCtrl |= 8;
          } else {
            pb->mixerCtrl &= ~8;
          }
          if (stp->type == SND_STUDIO_TYPE_STD) {
            if ((pb->mix.vS != 0) || (pb->mix.vDeltaS != 0) || (pb->mix.vAuxAS != 0) ||
                (pb->mix.vDeltaAuxAS != 0) || (pb->mix.vAuxBS != 0) || (pb->mix.vDeltaAuxBS != 0)) {
              pb->mixerCtrl |= 4;
            } else {
              pb->mixerCtrl &= ~4;
            }
          }
          if ((dsp_vptr->changed[0] & 0x200) != 0) {
            pb->itd.targetShiftL = dsp_vptr->itdShiftL;
            pb->itd.targetShiftR = dsp_vptr->itdShiftR;
          }
          if ((dsp_vptr->changed[0] & 0x100) != 0) {
            pb->srcSelect = dsp_vptr->srcTypeSelect;
          }
          if ((dsp_vptr->changed[0] & 0x80) != 0) {
            pb->coefSelect = dsp_vptr->srcCoefSelect;
          }
          mix_start = 0;
          newVoice = 0;
          dsp_vptr->currentAddr = (pb->addr.currentAddressHi << 0x10) | pb->addr.currentAddressLo;
        process_voice_frame:
          if ((dsp_vptr->changed[mix_start] & 0x40) != 0) {
            adsrRelease(&dsp_vptr->adsr);
          }
          if ((dsp_vptr->changed[mix_start] & 8) != 0) {
            pb->src.ratioHi = dsp_vptr->pitch[mix_start] >> 0x10;
            pb->src.ratioLo = dsp_vptr->pitch[mix_start];
            dsp_vptr->playInfo.pitch = dsp_vptr->pitch[mix_start];
          }
          VoiceDone = adsrHandle(&dsp_vptr->adsr, &pb->ve.currentVolume, &pb->ve.currentDelta);
          old_adsr_delta = pb->ve.currentDelta;
          for (s = 0; s < 5; s++) {
            pb->update.updNum[s] = 0;
          }
          pptr = dsp_vptr->patchData;
          pend = (u16 *)((unsigned long)dsp_vptr->patchData + 0x80);
          if (mix_start != 0) {

            pptr[0] = 7;
            pptr[1] = 1;
            pptr += 2;
            pb->update.updNum[mix_start]++;
          }
          sal_update_hostplayinfo(dsp_vptr);
          for (s = mix_start + 1; s < 5; s++) {
            if (VoiceDone != 0) {

              pptr[0] = 7;
              pptr[1] = 0;
              pptr += 2;
              pb->update.updNum[s]++;
              salSynthSendMessage(dsp_vptr, HW_MESSAGE_SAMPLE_END);
              salDeactivateVoice(dsp_vptr);
              break;
            } else {
              if (rampResetOffsetFlags[s] != 0) {
                for (i = 0; i < 9; i++) {
                  if (((1 << i) & rampResetOffsetFlags[s]) != 0) {

                    pptr[0] = ((u16*)pbOffsets)[i];
                    pptr[1] = 0;
                    pptr += 2;
                    pb->update.updNum[s]++;
                  }
                }
              }
              if ((dsp_vptr->changed[s] & 0x20) != 0) {
                adsrStartRelease(&dsp_vptr->adsr, 10);
                dsp_vptr->postBreak = 1;
              } else if (dsp_vptr->postBreak == 0) {
                if ((dsp_vptr->changed[s] & 0x40) != 0) {
                  adsrRelease(&dsp_vptr->adsr);
                }
                if ((dsp_vptr->changed[s] & 8) != 0) {

                  pptr[0] = 0x53;
                  pptr[1] = dsp_vptr->pitch[s] >> 16;
                  pptr[2] = 0x54;
                  pptr[3] = dsp_vptr->pitch[s];
                  pptr += 4;
                  pb->update.updNum[s] += 2;
                  dsp_vptr->playInfo.pitch = dsp_vptr->pitch[s];
                }
              }
              current_delta = dsp_vptr->adsr.delta;
              VoiceDone = adsrHandle(&dsp_vptr->adsr, &adsr_start, &adsr_delta);
              if (old_adsr_delta == adsr_delta) {
                if (current_delta != 0) {

                  pptr[0] = 0x32;
                  pptr[1] = adsr_start;
                  pptr += 2;
                  pb->update.updNum[s]++;
                }
              } else {

                pptr[0] = 0x32;
                pptr[1] = adsr_start;
                pptr[2] = 0x33;
                pptr[3] = adsr_delta;
                pptr += 4;
                pb->update.updNum[s] += 2;
                old_adsr_delta = adsr_delta;
              }
              sal_update_hostplayinfo(dsp_vptr);
            }
          }
          if (VoiceDone != 0) {
            salSynthSendMessage(dsp_vptr, HW_MESSAGE_SAMPLE_END);
            salDeactivateVoice(dsp_vptr);
          }
          DCStoreRangeNoSync(dsp_vptr->patchData, (unsigned long)pptr - (unsigned long)dsp_vptr->patchData);
          cyclesUsed += dspMixerCycles[pb->mixerCtrl] + 0x4FE;
          switch (pb->src.ratioHi) {
          case 0:
          case 1:
            cyclesUsed += dspSRCCycles[pb->src.ratioHi][pb->srcSelect];
            break;
          default:
            cyclesUsed += dspSRCCycles[2][pb->srcSelect];
            break;
          }
          for (s = 0; s < 5; s++) {
            cyclesUsed += pb->update.updNum[s] * 4;
          }
          if (cyclesUsed > (__OSBusClock / 400)) {
            if ((newVoice == 0) && (VoiceDone == 0)) {
              HandleDepopVoice(stp, dsp_vptr);
            }
            salDeactivateVoice(dsp_vptr);
            salSynthSendMessage(dsp_vptr, HW_MESSAGE_VOICE_KILL);
            for (v = v - 1; v > 0; v--) {
              if (voices[v - 1]->state == DSP_VOICE_STATE_PLAYING) {
                HandleDepopVoice(stp, voices[v - 1]);
              }
              salDeactivateVoice(voices[v - 1]);
              salSynthSendMessage(voices[v - 1], HW_MESSAGE_VOICE_KILL);
            }
            for (st1 = st + 1; st1 < salMaxStudioNum; st1++) {
              if (dspStudio[st1].state == DSP_STUDIO_STATE_ACTIVE) {
                for (dsp_vptr = dspStudio[st1].voiceRoot; dsp_vptr; dsp_vptr = next_dsp_vptr) {
                  next_dsp_vptr = dsp_vptr->next;
                  if (dsp_vptr->state == DSP_VOICE_STATE_PLAYING) {
                    HandleDepopVoice(&dspStudio[st1], dsp_vptr);
                  }
                  salDeactivateVoice(dsp_vptr);
                  salSynthSendMessage(dsp_vptr, HW_MESSAGE_VOICE_KILL);
                }
              }
            }
            break;
          } else {
            if (!last_pb) {
              if ((dspCmdPtr + 3) > (dspCmdMaxPtr - 4)) {
                u16 size;
                dspCmdPtr[0] = DSP_CMD_MORE;
                dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
                dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
                size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
                if (dspCmdLastLoad) {
                  dspCmdLastLoad[3] = size;
                  DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
                } else {
                  dspCmdFirstSize = size;
                }
                dspCmdLastLoad = dspCmdPtr;
                dspCmdLastSize = size;
                dspCmdLastBase = dspCmdCurBase;
                dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
                dspCmdMaxPtr = dspCmdPtr + 0xC0;
              }
              dspCmdPtr[0] = DSP_CMD_PB_ADDR;
              dspCmdPtr[1] = (unsigned long)pb >> 0x10;
              dspCmdPtr[2] = (unsigned long)pb;
              dspCmdPtr += 3;
              procVoiceFlag = 1;
            } else {
              last_pb->nextHi = (unsigned long)pb >> 16;
              last_pb->nextLo = (unsigned long)pb;
              procVoiceFlag = 1;
              DCFlushRangeNoSync(last_pb, sizeof(_PB));
            }
            last_pb = pb;
          }
        }
      }
      if (procVoiceFlag != 0) {
        if ((dspCmdPtr + 1) > (dspCmdMaxPtr - 4)) {
          u16 size;
          dspCmdPtr[0] = DSP_CMD_MORE;
          dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
          dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
          size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
          if (dspCmdLastLoad) {
            dspCmdLastLoad[3] = size;
            DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
          } else {
            dspCmdFirstSize = size;
          }
          dspCmdLastLoad = dspCmdPtr;
          dspCmdLastSize = size;
          dspCmdLastBase = dspCmdCurBase;
          dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
          dspCmdMaxPtr = dspCmdPtr + 0xC0;
        }
        *dspCmdPtr++ = DSP_CMD_PROCESS;
      }
      if (last_pb) {
        last_pb->nextHi = 0;
        last_pb->nextLo = 0;
        DCFlushRangeNoSync(last_pb, sizeof(_PB));
      }
      getAuxFrame = (salAuxFrame + 1) % 3;
      if (stp->auxAHandler) {
        if ((dspCmdPtr + 5) > (dspCmdMaxPtr - 4)) {
          u16 size;
          dspCmdPtr[0] = DSP_CMD_MORE;
          dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
          dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
          size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
          if (dspCmdLastLoad) {
            dspCmdLastLoad[3] = size;
            DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
          } else {
            dspCmdFirstSize = size;
          }
          dspCmdLastLoad = dspCmdPtr;
          dspCmdLastSize = size;
          dspCmdLastBase = dspCmdCurBase;
          dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
          dspCmdMaxPtr = dspCmdPtr + 0xC0;
        }
        dspCmdPtr[0] = DSP_CMD_MIX_AUXA;
        dspCmdPtr[1] = (unsigned long)stp->auxA[salAuxFrame] >> 16;
        dspCmdPtr[2] = (unsigned long)stp->auxA[salAuxFrame];
        dspCmdPtr[3] = (unsigned long)stp->auxA[getAuxFrame] >> 16;
        dspCmdPtr[4] = (unsigned long)stp->auxA[getAuxFrame];
        dspCmdPtr += 5;
      }
      if (stp->type == SND_STUDIO_TYPE_STD) {
        if (stp->auxBHandler) {
          if ((dspCmdPtr + 5) > (dspCmdMaxPtr - 4)) {
            u16 size;
            dspCmdPtr[0] = DSP_CMD_MORE;
            dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
            dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
            size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
            if (dspCmdLastLoad) {
              dspCmdLastLoad[3] = size;
              DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
            } else {
              dspCmdFirstSize = size;
            }
            dspCmdLastLoad = dspCmdPtr;
            dspCmdLastSize = size;
            dspCmdLastBase = dspCmdCurBase;
            dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
            dspCmdMaxPtr = dspCmdPtr + 0xC0;
          }
          dspCmdPtr[0] = DSP_CMD_MIX_AUXB;
          dspCmdPtr[1] = (unsigned long)stp->auxB[salAuxFrame] >> 16;
          dspCmdPtr[2] = (unsigned long)stp->auxB[salAuxFrame];
          dspCmdPtr[3] = (unsigned long)stp->auxB[getAuxFrame] >> 16;
          dspCmdPtr[4] = (unsigned long)stp->auxB[getAuxFrame];
          dspCmdPtr += 5;
        }
      } else {
        if ((dspCmdPtr + 5) > (dspCmdMaxPtr - 4)) {
          u16 size;
          dspCmdPtr[0] = DSP_CMD_MORE;
          dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
          dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
          size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
          if (dspCmdLastLoad) {
            dspCmdLastLoad[3] = size;
            DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
          } else {
            dspCmdFirstSize = size;
          }
          dspCmdLastLoad = dspCmdPtr;
          dspCmdLastSize = size;
          dspCmdLastBase = dspCmdCurBase;
          dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
          dspCmdMaxPtr = dspCmdPtr + 0xC0;
        }
        dspCmdPtr[0] = DSP_CMD_MIX_AUXB_LR;
        dspCmdPtr[1] = (unsigned long)stp->auxB[salFrame] >> 16;
        dspCmdPtr[2] = (unsigned long)stp->auxB[salFrame];
        dspCmdPtr[3] = (unsigned long)stp->auxB[salFrame ^ 1] >> 16;
        dspCmdPtr[4] = (unsigned long)stp->auxB[salFrame ^ 1];
        dspCmdPtr += 5;
      }
      if ((dspCmdPtr + 3) > (dspCmdMaxPtr - 4)) {
        u16 size;
        dspCmdPtr[0] = DSP_CMD_MORE;
        dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
        dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
        size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
        if (dspCmdLastLoad) {
          dspCmdLastLoad[3] = size;
          DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
        } else {
          dspCmdFirstSize = size;
        }
        dspCmdLastLoad = dspCmdPtr;
        dspCmdLastSize = size;
        dspCmdLastBase = dspCmdCurBase;
        dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
        dspCmdMaxPtr = dspCmdPtr + 0xC0;
      }
      dspCmdPtr[0] = DSP_CMD_UPLOAD_LRS;
      dspCmdPtr[1] = (unsigned long)stp->main[salFrame] >> 16;
      dspCmdPtr[2] = (unsigned long)stp->main[salFrame];
      dspCmdPtr += 3;
      spb = stp->spb;
      DoDepopFade((long *)&spb->dpopLHi, (s16 *)&spb->dpopLDelta, &stp->hostDPopSum.l);
      DoDepopFade((long *)&spb->dpopRHi, (s16 *)&spb->dpopRDelta, &stp->hostDPopSum.r);
      DoDepopFade((long *)&spb->dpopSHi, (s16 *)&spb->dpopSDelta, &stp->hostDPopSum.s);
      DoDepopFade((long *)&spb->dpopALHi, (s16 *)&spb->dpopALDelta, &stp->hostDPopSum.lA);
      DoDepopFade((long *)&spb->dpopARHi, (s16 *)&spb->dpopARDelta, &stp->hostDPopSum.rA);
      DoDepopFade((long *)&spb->dpopASHi, (s16 *)&spb->dpopASDelta, &stp->hostDPopSum.sA);
      DoDepopFade((long *)&spb->dpopBLHi, (s16 *)&spb->dpopBLDelta, &stp->hostDPopSum.lB);
      DoDepopFade((long *)&spb->dpopBRHi, (s16 *)&spb->dpopBRDelta, &stp->hostDPopSum.rB);
      DoDepopFade((long *)&spb->dpopBSHi, (s16 *)&spb->dpopBSDelta, &stp->hostDPopSum.sB);
      DCFlushRangeNoSync(spb, sizeof(_SPB));
    }
  }
  if ((dspCmdPtr + 3) > (dspCmdMaxPtr - 4)) {
    u16 size;
    dspCmdPtr[0] = DSP_CMD_MORE;
    dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
    dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
    size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
    if (dspCmdLastLoad) {
      dspCmdLastLoad[3] = size;
      DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
    } else {
      dspCmdFirstSize = size;
    }
    dspCmdLastLoad = dspCmdPtr;
    dspCmdLastSize = size;
    dspCmdLastBase = dspCmdCurBase;
    dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
    dspCmdMaxPtr = dspCmdPtr + 0xC0;
  }
  dspCmdPtr[0] = DSP_CMD_SET_OPPOSITE_LR;
  dspCmdPtr[1] = (unsigned long)dspSurround >> 16;
  dspCmdPtr[2] = (unsigned long)dspSurround;
  dspCmdPtr += 3;
  for (st = 0; st < salMaxStudioNum; st++) {
    if ((dspStudio[st].state == DSP_STUDIO_STATE_ACTIVE) && (dspStudio[st].isMaster != 0)) {
      if ((dspCmdPtr + 3) > (dspCmdMaxPtr - 4)) {
        u16 size;
        dspCmdPtr[0] = DSP_CMD_MORE;
        dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
        dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
        size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
        if (dspCmdLastLoad) {
          dspCmdLastLoad[3] = size;
          DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
        } else {
          dspCmdFirstSize = size;
        }
        dspCmdLastLoad = dspCmdPtr;
        dspCmdLastSize = size;
        dspCmdLastBase = dspCmdCurBase;
        dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
        dspCmdMaxPtr = dspCmdPtr + 0xC0;
      }
      dspCmdPtr[0] = DSP_CMD_MIX_AUXB_NOWRITE;
      dspCmdPtr[1] = (unsigned long)dspStudio[st].main[salFrame] >> 16;
      dspCmdPtr[2] = (unsigned long)dspStudio[st].main[salFrame];
      dspCmdPtr += 3;
    }
  }
  if ((dspCmdPtr + 5) > (dspCmdMaxPtr - 4)) {
    u16 size;
    dspCmdPtr[0] = DSP_CMD_MORE;
    dspCmdPtr[1] = (unsigned long)dspCmdMaxPtr >> 16;
    dspCmdPtr[2] = (unsigned long)dspCmdMaxPtr;
    size = (((unsigned long)(dspCmdPtr + 4) - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
    if (dspCmdLastLoad) {
      dspCmdLastLoad[3] = size;
      DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
    } else {
      dspCmdFirstSize = size;
    }
    dspCmdLastLoad = dspCmdPtr;
    dspCmdLastSize = size;
    dspCmdLastBase = dspCmdCurBase;
    dspCmdCurBase = dspCmdPtr = dspCmdMaxPtr;
    dspCmdMaxPtr = dspCmdPtr + 0xC0;
  }
  {
    u16 size;
    dspCmdPtr[0] = DSP_CMD_OUTPUT;
    dspCmdPtr[1] = (unsigned long)dspSurround >> 16;
    dspCmdPtr[2] = (unsigned long)dspSurround;
    dspCmdPtr[3] = (unsigned long)dest >> 16;
    dspCmdPtr[4] = (unsigned long)dest;
    dspCmdPtr += 5;
    *dspCmdPtr++ = DSP_CMD_END;
    size = (((unsigned long)dspCmdPtr - (unsigned long)dspCmdCurBase) + 3) & 0xfffc;
    if (dspCmdLastLoad) {
      dspCmdLastLoad[3] = size;
      DCStoreRangeNoSync(dspCmdLastBase, dspCmdLastSize);
    } else {
      dspCmdFirstSize = size;
    }
  }
  DCStoreRangeNoSync(dspCmdCurBase, (unsigned long)dspCmdPtr - (unsigned long)dspCmdCurBase);
}

void salDeactivateStudio(u8 studio)
{
    dspStudio[studio].state = 0;
}

u32 salSynthSendMessage(DSPvoice *voice, u32 message)
{
    if (salMessageCallback == 0)
        return 0;
    return salMessageCallback(message, voice->mesgCallBackUserValue);
}

void salDeactivateVoice(DSPvoice *dsp_vptr);

void salActivateVoice(DSPvoice *dsp_vptr, u8 studio)
{
    if (dsp_vptr->state != DSP_VOICE_STATE_INACTIVE) {
        salDeactivateVoice(dsp_vptr);
        dsp_vptr->changed[0] |= 0x20;
    }
    dsp_vptr->postBreak = 0;
    if ((dsp_vptr->next = dspStudio[studio].voiceRoot) != NULL) {
        dsp_vptr->next->prev = dsp_vptr;
    }
    dsp_vptr->prev = NULL;
    dspStudio[studio].voiceRoot = dsp_vptr;
    dsp_vptr->startupBreak = 0;
    dsp_vptr->state = DSP_VOICE_STATE_STARTING;
    dsp_vptr->studio = studio;
}

void salDeactivateVoice(DSPvoice *voice)
{
    if (voice->state == 0)
        return;
    if (voice->prev != 0)
        voice->prev->next = voice->next;
    else
        dspStudio[voice->studio].voiceRoot = voice->next;
    if (voice->next != 0)
        voice->next->prev = voice->prev;
    voice->state = 0;
}


void salActivateStudio(u8 studio, unsigned long isMaster, SND_STUDIO_TYPE type) {
  memset(dspStudio[studio].main[0], 0, 0x3c00);
  DCFlushRangeNoSync(dspStudio[studio].main[0], 0x3c00);
  memset(dspStudio[studio].spb, 0, sizeof(_SPB));
  dspStudio[studio].hostDPopSum.l = dspStudio[studio].hostDPopSum.r =
      dspStudio[studio].hostDPopSum.s = 0;
  dspStudio[studio].hostDPopSum.lA = dspStudio[studio].hostDPopSum.rA =
      dspStudio[studio].hostDPopSum.sA = 0;
  dspStudio[studio].hostDPopSum.lB = dspStudio[studio].hostDPopSum.rB =
      dspStudio[studio].hostDPopSum.sB = 0;
  DCFlushRangeNoSync(dspStudio[studio].spb, sizeof(_SPB));
  memset(dspStudio[studio].auxA[0], 0, 0x780);
  DCFlushRangeNoSync(dspStudio[studio].auxA[0], 0x780);
  memset(dspStudio[studio].auxB[0], 0, 0x780);
  DCFlushRangeNoSync(dspStudio[studio].auxB[0], 0x780);
  dspStudio[studio].voiceRoot = NULL;
  dspStudio[studio].alienVoiceRoot = NULL;
  dspStudio[studio].state = DSP_STUDIO_STATE_ACTIVE;
  dspStudio[studio].isMaster = isMaster;
  dspStudio[studio].numInputs = 0;
  dspStudio[studio].type = type;
  dspStudio[studio].auxAHandler = dspStudio[studio].auxBHandler = NULL;
}
void salHandleAuxProcessing() {
  u8 st;
  s32 *work;
  DSPstudioinfo *sp;
  SND_AUX_INFO info;
  sp = dspStudio;
  for (st = 0; st < salMaxStudioNum; ++st, sp++) {
    if (sp->state != DSP_STUDIO_STATE_ACTIVE) {
      continue;
    }
    if (sp->auxAHandler != NULL) {
      work = sp->auxA[(salAuxFrame + 2) % 3];
      info.data.bufferUpdate.left = work;
      info.data.bufferUpdate.right = work + 0xa0;
      info.data.bufferUpdate.surround = work + 0x140;
      sp->auxAHandler(0, &info, sp->auxAUser);
      DCFlushRangeNoSync(work, 0x780);
    }
    if (sp->type == 0 && sp->auxBHandler != 0) {
      work = sp->auxB[(salAuxFrame + 2) % 3];
      info.data.bufferUpdate.left = work;
      info.data.bufferUpdate.right = work + 0xa0;
      info.data.bufferUpdate.surround = work + 0x140;
      sp->auxBHandler(0, &info, sp->auxBUser);
      DCFlushRangeNoSync(work, 0x780);
    }
  }
}

unsigned long salAddStudioInput(DSPstudioinfo *stp, SND_STUDIO_INPUT *desc)
{
    if (stp->numInputs < 7) {
        stp->in[stp->numInputs].studio = desc->srcStudio;
        stp->in[stp->numInputs].vol = ((u16)desc->vol << 8) | ((u16)desc->vol << 1);
        stp->in[stp->numInputs].volA = ((u16)desc->volA << 8) | ((u16)desc->volA << 1);
        stp->in[stp->numInputs].volB = ((u16)desc->volB << 8) | ((u16)desc->volB << 1);
        stp->in[stp->numInputs].desc = desc;
        ++stp->numInputs;
        return 1;
    }
    return 0;
}

unsigned long salRemoveStudioInput(DSPstudioinfo *stp, SND_STUDIO_INPUT *desc)
{
    long i;
    for (i = 0; i < stp->numInputs; ++i) {
        if (stp->in[i].desc == desc) {
            for (; i <= stp->numInputs - 2; ++i) {
                stp->in[i] = stp->in[i + 1];
            }
            --stp->numInputs;
            return 1;
        }
    }
    return 0;
}

