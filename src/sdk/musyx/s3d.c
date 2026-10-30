#include "types.h"

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/snd3d.c (CC0-1.0). */
#include "constraint_math.h"
#include "musyx_spatial.h"
#include "musyx_hardware.h"
#pragma fp_contract off
#define NULL 0
#define TRUE 1
#define FALSE 0
#define SND_STUDIO_TYPE_STD 0
#define sqrtf constraint_root
typedef unsigned long SND_VOICEID;
typedef u16 SND_FXID;
extern u8 sndActive;
extern unsigned long synthFXStart(u16 fx, u8 volume, u8 pan, u8 studio, unsigned long itd);
extern unsigned long synthSendKeyOff(unsigned long voice);
extern unsigned long synthFXSetCtrl(unsigned long voice, u8 control, u8 value);
extern unsigned long synthFXSetCtrl14(unsigned long voice, u8 control, u16 value);
extern unsigned long sndFXCheck(unsigned long voice);
extern void synthActivateStudio(u8 studio, unsigned long master, unsigned long type);
extern void synthDeactivateStudio(u8 studio);

typedef struct SND_PARAMETER {
  u8 ctrl;
  union _paraData {
    u8 value7;
    u16 value14;
  } paraData;
} SND_PARAMETER;
typedef struct SND_PARAMETER_INFO {
  u8 numPara;
  SND_PARAMETER* paraArray;
} SND_PARAMETER_INFO;
typedef struct SND_STUDIO_INPUT {
  u8 vol;
  u8 volA;
  u8 volB;
  u8 srcStudio;
} SND_STUDIO_INPUT;
typedef struct SND_ROOM {
  struct SND_ROOM* next;
  struct SND_ROOM* prev;
  unsigned long flags;
  SND_FVECTOR pos;
  f32 distance;
  u8 studio;
  void (*activateReverb)(u8 studio,
                         void* para);
  void (*deActivateReverb)(u8 studio);
  void* user;
  unsigned long curMVol;
} SND_ROOM;
typedef struct SND_DOOR {
  struct SND_DOOR* next;
  struct SND_DOOR* prev;
  SND_FVECTOR pos;
  f32 open;
  f32 dampen;
  u8 fxVol;
  u8 destStudio;
  SND_ROOM* a;
  SND_ROOM* b;
  unsigned long flags;
  s16 filterCoef[4];
  SND_STUDIO_INPUT input;
} SND_DOOR;
typedef struct SND_LISTENER {
  struct SND_LISTENER* next;
  struct SND_LISTENER* prev;
  SND_ROOM* room;
  unsigned long flags;
  SND_FVECTOR pos;
  f32 volPosOff;
  SND_FVECTOR dir;
  SND_FVECTOR heading;
  SND_FVECTOR right;
  SND_FVECTOR up;
  SND_FMATRIX mat;
  f32 surroundDisFront;
  f32 surroundDisBack;
  f32 soundSpeed;
  f32 vol;
} SND_LISTENER;
typedef struct SND_EMITTER {
  struct SND_EMITTER* next;
  struct SND_EMITTER* prev;
  SND_ROOM* room;
  SND_PARAMETER_INFO* paraInfo;
  unsigned long flags;
  SND_FVECTOR pos;
  SND_FVECTOR dir;
  f32 maxDis;
  f32 maxVol;
  f32 minVol;
  f32 volPush;
  SND_VOICEID vid;
  unsigned long group;
  SND_FXID fxid;
  u8 studio;
  u8 maxVoices;
  u16 VolLevelCnt;
  f32 fade;
} SND_EMITTER;
typedef struct FX_TAB {
  u16 id;
  u16 macro;
  u8 maxVoices;
  u8 priority;
  u8 volume;
  u8 panning;
  u8 key;
  u8 vGroup;
} FX_TAB;
typedef struct START_LIST {
  struct START_LIST *next;
  f32 vol;
  f32 xPan;
  f32 yPan;
  f32 zPan;
  f32 pitch;
  SND_EMITTER *em;
} START_LIST;
typedef struct RUN_LIST {
  struct RUN_LIST *next;
  f32 vol;
  SND_EMITTER *em;
} RUN_LIST;
typedef struct START_GROUP {
  unsigned long id;
  struct START_LIST *list;
  struct RUN_LIST *running;
  unsigned short numRunning;
} START_GROUP;











static u8 s3dCallCnt;
static SND_EMITTER *s3dEmitterRoot;
static SND_LISTENER *s3dListenerRoot;
static SND_ROOM *s3dRoomRoot;
static SND_DOOR *s3dDoorRoot;
static unsigned long snd_used_studios;
static u8 snd_base_studio, snd_max_studios, s3dUseMaxVoices;

void s3dInit(unsigned long flags)
{
    s3dEmitterRoot = 0;
    s3dListenerRoot = 0;
    s3dRoomRoot = 0;
    s3dDoorRoot = 0;
    snd_used_studios = 0;
    snd_base_studio = 1;
    snd_max_studios = 3;
    s3dCallCnt = 0;
    s3dUseMaxVoices = (flags & 2) != 0;
}




typedef char SpatialEmitterSizeCheck[sizeof(SND_EMITTER) == 80 ? 1 : -1];
typedef char SpatialListenerSizeCheck[sizeof(SND_LISTENER) == 144 ? 1 : -1];
typedef char SpatialRoomSizeCheck[sizeof(SND_ROOM) == 48 ? 1 : -1];
typedef char SpatialDoorSizeCheck[sizeof(SND_DOOR) == 56 ? 1 : -1];
extern unsigned long synthAddStudioInput(u8 studio, SND_STUDIO_INPUT *input);
extern unsigned long synthRemoveStudioInput(u8 studio, SND_STUDIO_INPUT *input);
extern void salApplyMatrix(const SND_FMATRIX *matrix, const SND_FVECTOR *vector, SND_FVECTOR *output);
extern f32 salNormalizeVector(SND_FVECTOR *vector);
static void EmitterShutdown(SND_EMITTER *em);
static inline unsigned long sndRemoveEmitter(SND_EMITTER *em) {
  if (sndActive) {
    hwDisableIrq();
    if (em->flags & 0x10000) {
      EmitterShutdown(em);
    }
    hwEnableIrq();
    return TRUE;
  }
  return FALSE;
}
extern u8 synthFXGetMaxVoices(u16 fx);
static void CalcEmitter(SND_EMITTER *, f32 *, f32 *, f32 *, f32 *, f32 *);
static void SetFXParameters(SND_EMITTER *, f32, f32, f32, f32, f32);
/* The unused SDK helper retains the original temporary emitter in the BSS pool. */
static inline SND_VOICEID AddEmitter(SND_EMITTER *em_buffer, SND_FVECTOR *pos, SND_FVECTOR *dir,
                              f32 maxDis, f32 comp, unsigned long flags, u16 fxid, unsigned long groupid, u8 maxVol,
                              u8 minVol, SND_ROOM *room, SND_PARAMETER_INFO *para, u8 studio) {
  static SND_EMITTER tmp_em;
  SND_EMITTER *em;
  f32 xPan;
  f32 yPan;
  f32 zPan;
  f32 cvol;
  f32 pitch;
  hwDisableIrq();
  em = em_buffer == NULL ? &tmp_em : em_buffer;
  em->flags = flags;
  em->pos = *pos;
  em->dir = *dir;
  em->maxDis = maxDis;
  em->fxid = fxid;
  em->maxVol = maxVol * (1.f / 127.f);
  em->minVol = minVol * (1.f / 127.f);
  em->volPush = comp;
  em->group = groupid;
  em->room = room;
  em->studio = studio;
  if (em_buffer == NULL) {
    if (em->room != NULL && em->room->studio == 0xFF) {
      hwEnableIrq();
      return -1;
    }
    CalcEmitter(em, &cvol, &pitch, &xPan, &yPan, &zPan);
    if (cvol == 0.f) {
      hwEnableIrq();
      return -1;
    } else {
      em->vid = synthFXStart(em->fxid, 127, 64, em->room != NULL ? em->room->studio : em->studio,
                             (em->flags & 0x10) != 0);
      if (em->vid == -1) {
        hwEnableIrq();
        return -1;
      }
      SetFXParameters(em, cvol, xPan, yPan, zPan, pitch);
      hwEnableIrq();
      return em->vid;
    }
  } else {
    if ((em->next = s3dEmitterRoot) != NULL) {
      s3dEmitterRoot->prev = em;
    }
    em->prev = NULL;
    s3dEmitterRoot = em;
    em->paraInfo = para;
    em->vid = -1;
    em->VolLevelCnt = 0;
    em->flags |= 0x30000;
    em->maxVoices = synthFXGetMaxVoices(em->fxid);
  }
  hwEnableIrq();
  return -1;
}
static START_GROUP startGroup[64];
static START_LIST startListNum[64];
static RUN_LIST runList[64];
static u8 startGroupNum, startListNumnum, runListNum;

static inline u8 clip127(u8 v) {
  if (v > 0x7f) {
    return 0x7f;
  }
  return v;
}
static inline u16 clip3FFF(
    unsigned long v
) {
  if (v > 0x3fff) {
    return 0x3fff;
  }
  return v;
}
static inline void CalcDoorParameters(SND_DOOR *door) {
  f32 f;
  f32 v;
  v = door->open;
  f = (1.f - door->open) * door->dampen;
  door->input.volA = door->fxVol * v;
  door->input.volB = 0;
  door->input.vol = v * 127.f;
}
static inline void ClearStartList() {
  startGroupNum = 0;
  startListNumnum = 0;
  runListNum = 0;
}
#pragma force_active on
static void UpdateRoomDistances() {
  struct SND_ROOM *r;
  struct SND_LISTENER *li;
  float distance;
  unsigned long n;
  struct SND_FVECTOR d;
  for (n = 0, li = s3dListenerRoot; li != NULL; li = li->next, n++)
    ;
  if (n != 0) {
    for (r = s3dRoomRoot; r != NULL; r = r->next) {
      if (r->studio != 0xFF) {
        distance = 0.f;
        for (li = s3dListenerRoot; li != NULL; li = li->next) {
          d.x = r->pos.x - li->pos.x;
          d.y = r->pos.y - li->pos.y;
          d.z = r->pos.z - li->pos.z;
          distance += d.x * d.x + d.y * d.y + d.z * d.z;
        }
        r->distance = distance / n;
      }
    }
  }
}
static void CheckRoomStatus() {
  SND_LISTENER *li;
  SND_EMITTER *em;
  SND_ROOM *r;
  SND_ROOM *max_room;
  SND_ROOM *room;
  SND_FVECTOR d;
  f32 distance;
  f32 maxDis;
  unsigned long li_num;
  unsigned long i;
  unsigned long mask;
  u8 has_listener;
  UpdateRoomDistances();
  for (li_num = 0, li = s3dListenerRoot; li != NULL; li = li->next, li_num++)
    ;
  if (li_num != 0) {
    for (room = s3dRoomRoot; room != NULL; room = room->next) {
      if (room->studio == 0xff) {
        distance = 0.f;
        for (li = s3dListenerRoot; li != NULL; li = li->next) {
          d.x = room->pos.x - li->pos.x;
          d.y = room->pos.y - li->pos.y;
          d.z = room->pos.z - li->pos.z;
          distance += d.x * d.x + d.y * d.y + d.z * d.z;
        }
        distance = distance / li_num;
        has_listener = FALSE;
        for (li = s3dListenerRoot; li != NULL; li = li->next) {
          if (li->room == room) {
            has_listener = TRUE;
            break;
          }
        }
        mask = ~(-1 << snd_max_studios);
        if (mask != (snd_used_studios & mask)) {
          for (i = 0; i < snd_max_studios; ++i) {
            if (!(snd_used_studios & (1 << i))) {
              break;
            }
          }
          snd_used_studios |= (1 << i);
          room->studio = i + snd_base_studio;
        } else {
          maxDis = -1.f;
          for (r = s3dRoomRoot; r != NULL; r = r->next) {
            if (r->studio != 0xFF && maxDis < r->distance) {
              maxDis = r->distance;
              max_room = r;
            }
          }
          if (has_listener || maxDis > distance) {
            for (em = s3dEmitterRoot; em != NULL; em = em->next) {
              if (em->room == max_room) {
                synthSendKeyOff(em->vid);
                em->flags |= 0x80000;
                em->vid = -1;
              }
            }
            if (max_room->deActivateReverb != NULL) {
              max_room->deActivateReverb(max_room->studio);
            }
            synthDeactivateStudio(max_room->studio);
            room->studio = max_room->studio;
            max_room->studio = 0xff;
            max_room->flags = 0;
          } else {
            continue;
          }
        }
        room->distance = distance;
        room->curMVol = has_listener ? 0x7f0000 : 0;
        if (room->curMVol * 1.2014794e-07f >= 0.5) {
          synthActivateStudio(room->studio, TRUE, SND_STUDIO_TYPE_STD);
        } else {
          synthActivateStudio(room->studio, FALSE, SND_STUDIO_TYPE_STD);
        }
        if (room->activateReverb != NULL) {
          room->activateReverb(room->studio, room->user);
        }
      } else {
        if (room->flags & 0x80000000) {
          room->curMVol += 0x40000;
          if (room->curMVol >= 0x7F0000) {
            room->curMVol = 0x7F0000;
            room->flags &= ~0x80000000;
          }
          if (room->curMVol * 1.2014794e-07f >= 0.5) {
            synthActivateStudio(room->studio, TRUE, SND_STUDIO_TYPE_STD);
          } else {
            synthActivateStudio(room->studio, FALSE, SND_STUDIO_TYPE_STD);
          }
        }
        if ((room->flags & 0x40000000) != 0) {
          room->curMVol = room->curMVol - 0x40000;
          if ((int)room->curMVol >= 0) {
            room->curMVol = 0;
            room->flags &= ~0x40000000;
          }
          if (room->curMVol * 1.2014794e-07f >= 0.5) {
            synthActivateStudio(room->studio, TRUE, SND_STUDIO_TYPE_STD);
          } else {
            synthActivateStudio(room->studio, FALSE, SND_STUDIO_TYPE_STD);
          }
        }
      }
    }
  }
}
static void CheckDoorStatus() {
  SND_DOOR *door;
  for (door = s3dDoorRoot; door != NULL; door = door->next) {
    if (!(door->flags & 0x80000000)) {
      if (door->a->studio != 0xFF) {
        if (door->b->studio != 0xFF) {
          CalcDoorParameters(door);
          if (door->flags & 1) {
            door->input.srcStudio = door->b->studio;
            synthAddStudioInput(door->a->studio, &door->input);
          } else {
            door->input.srcStudio = door->a->studio;
            synthAddStudioInput(door->b->studio, &door->input);
          }
          door->flags |= 0x80000000;
        }
      }
    } else if (door->a->studio == 0xFF || door->b->studio == 0xFF) {
      if ((door->a->studio != 0xFF && door->a->studio == door->destStudio) ||
          (door->b->studio != 0xFF && door->b->studio == door->destStudio)) {
        synthRemoveStudioInput(door->destStudio, &door->input);
      }
      door->flags &= ~0x80000000;
    } else {
      CalcDoorParameters(door);
    }
  }
}
static void CalcEmitter(SND_EMITTER *em, f32 *vol, f32 *doppler, f32 *xPan, f32 *yPan, f32 *zPan
) {
  SND_LISTENER *li;
  SND_FVECTOR d;
  SND_FVECTOR v;
  SND_FVECTOR p;
  f32 relspeed;
  f32 distance;
  f32 new_distance;
  f32 ft;
  f32 vd;
  SND_FVECTOR pan;
  unsigned long n;
  ft = 1.f / 60.f;
  *vol = 0.f;
  *doppler = 1.f;
  pan.x = pan.y = pan.z = 0.f;
  for (n = 0, li = s3dListenerRoot; li != NULL; li = li->next, n++) {
    d.x = em->pos.x - (li->pos.x + li->heading.x * li->volPosOff);
    d.y = em->pos.y - (li->pos.y + li->heading.y * li->volPosOff);
    d.z = em->pos.z - (li->pos.z + li->heading.z * li->volPosOff);
    distance = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
    if (em->maxDis >= distance) {
      vd = distance / em->maxDis;
      if (em->volPush >= 0.f) {
        *vol +=
            li->vol * (em->minVol + (em->maxVol - em->minVol) *
                                        (1.f - ((1.f - em->volPush) * vd + em->volPush * vd * vd)));
      } else {
        *vol +=
            li->vol * (em->minVol + (em->maxVol - em->minVol) *
                                        (1.f - ((em->volPush + 1.f) * vd -
                                                em->volPush * (1.f - (1.f - vd) * (1.f - vd)))));
      }
      if (em->flags & 0x80000) {
        continue;
      }
      if ((em->flags & 0x8) || (li->flags & 1)) {
        v.x = li->dir.x - em->dir.x;
        v.y = li->dir.y - em->dir.y;
        v.z = li->dir.z - em->dir.z;
        relspeed = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
        if (relspeed > 0.f) {
          d.x = (em->pos.x + em->dir.x * ft) - (li->pos.x + li->dir.x * ft);
          d.y = (em->pos.y + em->dir.y * ft) - (li->pos.y + li->dir.y * ft);
          d.z = (em->pos.z + em->dir.z * ft) - (li->pos.z + li->dir.z * ft);
          new_distance = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
          if (new_distance < distance) {
            *doppler = li->soundSpeed / (li->soundSpeed - relspeed);
          } else {
            *doppler = li->soundSpeed / (li->soundSpeed + relspeed);
          }
        }
      }
      if (distance != 0.f) {
        salApplyMatrix(&li->mat, &em->pos, &p);
        if (p.z <= 0.f) {
          pan.z += -li->surroundDisFront < p.z ? -p.z / li->surroundDisFront : 1.f;
        } else {
          pan.z += li->surroundDisBack > p.z ? -p.z / li->surroundDisBack : -1.f;
        }
        if (p.x != 0.f || p.y != 0.f || p.z != 0.f) {
          salNormalizeVector(&p);
        }
        pan.x += p.x;
        pan.y -= p.y;
      }
    }
  }
  if (n != 0) {
    *xPan = pan.x / n;
    *yPan = pan.y / n;
    *zPan = pan.z / n;
  }
}
static void SetFXParameters(SND_EMITTER *const em, f32 vol, f32 xPan, f32 yPan, f32 zPan,
                            f32 doppler
) {
  SND_VOICEID vid;
  u8 i;
  SND_PARAMETER *pPtr;
  vid = em->vid;
  if ((em->flags & 0x100000) != 0) {
    synthFXSetCtrl(vid, 7, clip127((em->fade * vol) * 127.f));
  } else {
    synthFXSetCtrl(vid, 7, clip127(vol * 127.f));
  }
  synthFXSetCtrl(vid, 10, clip127((1.f + xPan) * 64.f));
  synthFXSetCtrl(vid, 131, clip127((1.f - zPan) * 64.f));
  synthFXSetCtrl14(vid, 132, clip3FFF(doppler * 8192.0f));
  if (em->paraInfo != NULL) {
    pPtr = em->paraInfo->paraArray;
    for (i = 0; i < em->paraInfo->numPara; ++i) {
      if (pPtr->ctrl < 0x40 || pPtr->ctrl == 0x80 || pPtr->ctrl == 0x84) {
        synthFXSetCtrl14(vid, pPtr->ctrl, (pPtr->paraData).value14);
      } else {
        synthFXSetCtrl(vid, pPtr->ctrl, (pPtr->paraData).value7);
      }
      ++pPtr;
    }
  }
}
static void EmitterShutdown(SND_EMITTER *em) {
  if (em->next != NULL) {
    em->next->prev = em->prev;
  }
  if (em->prev != NULL) {
    em->prev->next = em->next;
  } else {
    s3dEmitterRoot = em->next;
  }
  em->flags &= 0xFFFF;
  if (em->vid != -1) {
    synthSendKeyOff(em->vid);
  }
}
void s3dKillEmitterByFXID(FX_TAB *fxTab, unsigned long num) {
  struct SND_EMITTER *em;
  struct SND_EMITTER *nem;
  unsigned long j;
  for (em = s3dEmitterRoot; em != NULL; em = nem) {
    nem = em->next;
    for (j = 0; j < num; ++j) {
      if (em->fxid == fxTab[j].id) {
        sndRemoveEmitter(em);
        break;
      }
    }
  }
}
void AddRunningEmitter(SND_EMITTER *em, f32 vol) {
  long i;
  RUN_LIST *rl;
  RUN_LIST *lrl;
  for (i = 0; i < startGroupNum; ++i) {
    if (em->group == startGroup[i].id) {
      break;
    }
  }
  if (i == startGroupNum) {
    startGroup[i].list = NULL;
    startGroup[i].running = NULL;
    startGroup[i].numRunning = 0;
    startGroup[i].id = em->group;
    ++startGroupNum;
  }
  ++startGroup[i].numRunning;
  lrl = NULL;
  for (rl = startGroup[i].running; rl != NULL; rl = rl->next) {
    if (rl->vol > vol) {
      break;
    }
    lrl = rl;
  }
  if (lrl == NULL) {
    startGroup[i].running = &runList[runListNum];
  } else {
    lrl->next = &runList[runListNum];
  }
  runList[runListNum].next = rl;
  runList[runListNum].em = em;
  runList[runListNum++].vol = vol;
}
static unsigned long AddStartingEmitter(SND_EMITTER *em, f32 vol, f32 xPan, f32 yPan, f32 zPan, f32 pitch
) {
  long i;
  START_LIST *sl;
  for (i = 0; i < startGroupNum; ++i) {
    if (em->group == startGroup[i].id) {
      break;
    }
  }
  if (i == startGroupNum) {
    if (startGroupNum == 64) {
      return FALSE;
    }
    startGroup[i].list = NULL;
    startGroup[i].running = NULL;
    startGroup[i].numRunning = 0;
    startGroup[i].id = em->group;
    ++startGroupNum;
  }
  if (startListNumnum == 64) {
    return FALSE;
  }
  if ((sl = startGroup[i].list) != NULL) {
    for (; sl->next != NULL; sl = sl->next) {
      if (sl->vol < vol) {
        break;
      }
    }
    startListNum[startListNumnum].next = sl->next;
    sl->next = &startListNum[startListNumnum];
  } else {
    startListNum[startListNumnum].next = startGroup[i].list;
    startGroup[i].list = &startListNum[startListNumnum];
  }
  startListNum[startListNumnum].em = em;
  startListNum[startListNumnum].pitch = pitch;
  startListNum[startListNumnum].xPan = xPan;
  startListNum[startListNumnum].yPan = yPan;
  startListNum[startListNumnum].zPan = zPan;
  startListNum[startListNumnum++].vol = vol;
  return TRUE;
}
static void StartContinousEmitters() {
    long i;
    START_LIST *sl;
    SND_EMITTER *em;
    f32 dv;
    for (i = 0; i < startGroupNum; ++i) {
      for (sl = startGroup[i].list; sl != NULL; sl = sl->next) {
        if ((startGroup[i].running != NULL) &&
            !(((s3dUseMaxVoices != '\0' && ((startGroup[i].id & 0x80000000) != 0)) &&
               (startGroup[i].numRunning < startGroup[i].list->em->maxVoices)))) {
          dv = sl->vol - (startGroup[i].running)->vol;
          if (dv <= 0.08f) {
            continue;
          } else if (dv <= 0.15f) {
            if (++sl->em->VolLevelCnt < 20) {
              continue;
            }
          } else {
            sl->em->VolLevelCnt = 0;
          }
        }
        em = sl->em;
        if (em->room != NULL && em->room->studio == 0xFF) {
          goto set_flags;
        }
        if (
            (em->vid =
                 synthFXStart(em->fxid, 127, 64, em->room != NULL ? em->room->studio : em->studio,
                              (em->flags & 0x10) != 0))
            == -1) {
        set_flags:
          if (!(em->flags & 0x2)) {
            em->flags |= 0x40000;
            em->flags &= ~0x20000;
          }
        } else {
          if (!(em->flags & 0x20)) {
            em->flags |= 0x100000;
            em->fade = 0.f;
          } else {
            em->fade = 1.f;
          }
          SetFXParameters(em, sl->vol, sl->xPan, sl->yPan, sl->zPan, sl->pitch);
          em->flags &= ~0x20000;
          ++startGroup[i].numRunning;
          if (startGroup[i].running != NULL) {
            startGroup[i].running = startGroup[i].running->next;
          }
        }
      }
    }
}
void s3dHandle() {
  SND_EMITTER *em;
  SND_EMITTER *nem;
  f32 vol;
  f32 xPan;
  f32 yPan;
  f32 zPan;
  f32 pitch;
  if (s3dCallCnt != 0) {
    --s3dCallCnt;
    return;
  }
  s3dCallCnt = 3;
  ClearStartList();
  em = s3dEmitterRoot;
  for (; em != NULL; em = nem) {
    nem = em->next;
    if ((em->flags & 0x40000) != 0) {
      EmitterShutdown(em);
      continue;
    }
    if ((em->flags & 0x20001) != 0) {
      CalcEmitter(em, &vol, &pitch, &xPan, &yPan, &zPan);
    }
    if (!(em->flags & 0x80000)) {
      if (em->flags & 0x20000) {
        if (vol == 0.f && em->flags & 0x4) {
          em->flags |= 0x80000;
          em->flags &= ~0x20000;
          goto found_emitter;
        }
        else if (vol == 0.f && em->flags & 0x40) {
          EmitterShutdown(em);
          continue;
        }
        if (em->flags & 1) {
          if (AddStartingEmitter(em, vol, xPan, yPan, zPan, pitch
                                 )) {
            continue;
          }
        }
        else if (em->room == NULL || em->room->studio != 0xFF) {
          if ((em->vid =
                   synthFXStart(em->fxid, 127, 64, em->room != NULL ? em->room->studio : em->studio,
                                (em->flags & 0x10) != 0)) == -1) {
          start_failed:
            if (!(em->flags & 2)) {
              em->flags |= 0x40000;
              em->flags &= ~0x20000;
            } else {
              continue;
            }
          }
        } else {
          goto start_failed;
        }
      } else if ((em->vid = sndFXCheck(em->vid)) == -1) {
        if ((em->flags & 2)) {
          em->flags |= 0x20000;
        } else {
          em->flags |= 0x40000;
        }
      }
    found_emitter:
      if (em->vid != -1) {
        if ((em->flags & 1) != 0) {
          AddRunningEmitter(em, vol);
        }
        if ((vol == 0.f) && ((em->flags & 4) != 0)) {
          synthSendKeyOff(em->vid);
          em->vid = 0xffffffff;
          if ((em->flags & 2)) {
            em->flags |= 0x80000;
          } else {
            em->flags |= 0x40000;
          }
        } else {
          SetFXParameters(em, vol, xPan, yPan, zPan, pitch);
        }
      }
      if ((em->flags & 0x100000) != 0) {
        em->fade += .3f;
        if (em->fade >= 1.f) {
          em->flags &= ~0x100000;
        }
      }
    } else if (
        (em->room == NULL || (em->room != NULL && em->room->studio != 0xff)) &&
        vol != 0.f) {
      em->flags &= ~0x80000;
      em->flags |= 0x20000;
    }
  }
  StartContinousEmitters();
  CheckRoomStatus();
  CheckDoorStatus();
}
#pragma force_active reset
