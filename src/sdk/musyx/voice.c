#include "musyx_synth.h"
#include "musyx_hardware.h"
typedef struct VOICE_LIST {
    u8 prev, next;
    u16 state;
} VOICE_LIST;
typedef struct PRIORITY_LIST {
    u16 next, prev;
} PRIORITY_LIST;
VOICE_LIST voiceList[64];
VOICE_LIST voicePrioSortVoices[64];
u8 voicePrioSortVoicesRoot[256];
PRIORITY_LIST voicePrioSortRootList[256];
extern u16 voicePrioSortRootListRoot;
extern u8 voiceListRoot, voiceListInsert;
extern u8 voiceFxRunning, voiceMusicRunning;
extern u8 sndActive;
extern VID_LIST* vidRoot;
extern VID_LIST* vidFree;
extern u32 vidCurrentId;
extern VID_LIST* get_vidlist(u32 vid);
extern void voiceResetLastStarted(SYNTH_VOICE* voice);
static VID_LIST vidList[128];
static u8 synth_last_started[8][16];
static u8 synth_last_fxstarted[64];
extern void macMakeInactive(SYNTH_VOICE* voice, s32 state);
extern void vidRemoveVoiceReferences(SYNTH_VOICE* voice);
extern u32 vidGetInternalId(u32 vid);
extern void streamKill(u32 index);
extern void voiceRemovePriority(SYNTH_VOICE* voice);

void vidInit(void)
{
    u32 index;
    VID_LIST* list = vidList;
    VID_LIST* prev = 0;
    vidFree = vidList;
    vidCurrentId = 0;
    vidRoot = 0;
    for (index = 0; index < 128; index++) {
        list->prev = prev;
        if (prev != 0)
            prev->next = list;
        prev = list++;
    }
    prev->next = 0;
}
void voiceInitLastStarted(void)
{
    s32 set, channel;

    for (set = 0; set < 8; set++)
        for (channel = 0; channel < 16; channel++)
            synth_last_started[set][channel] = 0xFF;
    for (channel = 0; channel < 64; channel++)
        synth_last_fxstarted[channel] = 0xFF;
}

VID_LIST* get_vidlist(u32 vid)
{
    VID_LIST* list = vidRoot;
    while (list != 0) {
        if (list->vid == vid)
            return list;
        if (list->vid > vid)
            break;
        list = list->next;
    }
    return 0;
}

u32 vidMakeRoot(SYNTH_VOICE* voice)
{
    voice->vidMasterList = voice->vidList;
    return voice->vidList->vid;
}

u32 vidGetInternalId(u32 vid)
{
    VID_LIST* list;
    if (vid != -1) {
        if ((list = get_vidlist(vid)) != 0)
            return list->root;
    }
    return -1;
}

u32 voiceIsLastStarted(SYNTH_VOICE* voice)
{
    if (voice->id != 0xFFFFFFFF && voice->midi != 0xFF) {
        u8 index = (u8)voice->id;
        if (voice->midiSet == 0xFF) {
            if (synth_last_fxstarted[index] == index)
                return 1;
        } else if (index == synth_last_started[voice->midiSet][voice->midi]) {
            return 1;
        }
    }
    return 0;
}

void voiceSetLastStarted(SYNTH_VOICE* voice)
{
    if (voice->id != 0xFFFFFFFF && voice->midi != 0xFF) {
        u8 index = (u8)voice->id;
        if (voice->midiSet == 0xFF)
            synth_last_fxstarted[index] = index;
        else
            synth_last_started[voice->midiSet][voice->midi] = index;
    }
}

#pragma auto_inline off
#pragma auto_inline off
void voiceResetLastStarted(SYNTH_VOICE* voice)
{
    if (voice->id != 0xFFFFFFFF && voice->midi != 0xFF) {
        u8 index = (u8)voice->id;
        if (voice->midiSet == 0xFF) {
            if (synth_last_fxstarted[index] == index)
                synth_last_fxstarted[index] = 0xFF;
        } else if (index == synth_last_started[voice->midiSet][voice->midi]) {
            synth_last_started[voice->midiSet][voice->midi] = 0xFF;
        }
    }
}

#pragma auto_inline off
u32 vidMakeNew(SYNTH_VOICE* voice, u32 master)
{
    u32 id;
    VID_LIST* next;
    VID_LIST* prev;
    VID_LIST* list;
    do {
        id = vidCurrentId++;
    } while (id == 0xFFFFFFFF);
    next = vidRoot;
    prev = 0;
    while (next) {
        if (next->vid > id)
            break;
        if (next->vid == id) {
            do {
                id = vidCurrentId++;
            } while (id == 0xFFFFFFFF);
        }
        prev = next;
        next = next->next;
    }
    if ((list = vidFree) == 0)
        return 0xFFFFFFFF;
    if ((vidFree = vidFree->next) != 0)
        vidFree->prev = 0;
    if (!prev)
        vidRoot = list;
    else
        prev->next = list;
    list->prev = prev;
    list->next = next;
    if (next)
        next->prev = list;
    list->vid = id;
    list->root = voice->id;
    voice->vidMasterList = master ? list : 0;
    voice->vidList = list;
    return master ? id : voice->id;
}

void vidRemoveVoiceReferences(SYNTH_VOICE* voice)
{
    if (voice->id != 0xFFFFFFFF) {
        voiceResetLastStarted(voice);
        if (voice->prevID != 0xFFFFFFFF) {
            synthVoice[(u8)voice->prevID].nextID = voice->nextID;
            if (voice->nextID != 0xFFFFFFFF)
                synthVoice[(u8)voice->nextID].prevID = voice->prevID;
            if (voice->vidList->prev)
                voice->vidList->prev->next = voice->vidList->next;
            else
                vidRoot = voice->vidList->next;
            if (voice->vidList->next)
                voice->vidList->next->prev = voice->vidList->prev;
            voice->vidList->next = vidFree;
            if (vidFree)
                vidFree->prev = voice->vidList;
            voice->vidList->prev = 0;
            vidFree = voice->vidList;
            voice->vidList = 0;
        } else if (voice->nextID != 0xFFFFFFFF) {
            voice->vidList->root = voice->nextID;
            synthVoice[(u8)voice->nextID].prevID = 0xFFFFFFFF;
            synthVoice[(u8)voice->nextID].vidMasterList = voice->vidMasterList;
            if (voice->vidList != voice->vidMasterList) {
                if (voice->vidList->prev)
                    voice->vidList->prev->next = voice->vidList->next;
                else
                    vidRoot = voice->vidList->next;
                if (voice->vidList->next)
                    voice->vidList->next->prev = voice->vidList->prev;
                voice->vidList->next = vidFree;
                if (vidFree)
                    vidFree->prev = voice->vidList;
                voice->vidList->prev = 0;
                vidFree = voice->vidList;
                voice->vidList = 0;
            }
            voice->vidList = 0;
            voice->vidMasterList = 0;
        } else if (voice->vidList != voice->vidMasterList) {
            if (voice->vidList->prev)
                voice->vidList->prev->next = voice->vidList->next;
            else
                vidRoot = voice->vidList->next;
            if (voice->vidList->next)
                voice->vidList->next->prev = voice->vidList->prev;
            voice->vidList->next = vidFree;
            if (vidFree)
                vidFree->prev = voice->vidList;
            voice->vidList->prev = 0;
            vidFree = voice->vidList;
            voice->vidList = 0;
            if (voice->vidMasterList->prev)
                voice->vidMasterList->prev->next = voice->vidMasterList->next;
            else
                vidRoot = voice->vidMasterList->next;
            if (voice->vidMasterList->next)
                voice->vidMasterList->next->prev = voice->vidMasterList->prev;
            voice->vidMasterList->next = vidFree;
            if (vidFree)
                vidFree->prev = voice->vidMasterList;
            voice->vidMasterList->prev = 0;
            vidFree = voice->vidMasterList;
            voice->vidMasterList = 0;
        } else {
            if (voice->vidList->prev)
                voice->vidList->prev->next = voice->vidList->next;
            else
                vidRoot = voice->vidList->next;
            if (voice->vidList->next)
                voice->vidList->next->prev = voice->vidList->prev;
            voice->vidList->next = vidFree;
            if (vidFree)
                vidFree->prev = voice->vidList;
            voice->vidList->prev = 0;
            vidFree = voice->vidList;
            voice->vidList = 0;
            voice->vidMasterList = 0;
        }
    }
}

void voiceRemovePriority(SYNTH_VOICE* voice)
{
    VOICE_LIST* list = &voicePrioSortVoices[(u8)voice->id];
    if (list->state == 1) {
        if (list->prev != 0xFF)
            voicePrioSortVoices[list->prev].next = list->next;
        else
            voicePrioSortVoicesRoot[voice->priority] = list->next;
        if (list->next != 0xFF) {
            voicePrioSortVoices[list->next].prev = list->prev;
        } else if (list->prev == 0xFF) {
            PRIORITY_LIST* priority = &voicePrioSortRootList[voice->priority];
            if (priority->prev != 0xFFFF)
                voicePrioSortRootList[priority->prev].next = priority->next;
            else
                voicePrioSortRootListRoot = priority->next;
            if (priority->next != 0xFFFF)
                voicePrioSortRootList[priority->next].prev = priority->prev;
        }
        list->state = 0;
    }
}
void voiceFree(SYNTH_VOICE* voice)
{
    u32 index;
    VOICE_LIST* list;
    macMakeInactive(voice, 2);
    voiceRemovePriority(voice);
    voice->macro = 0;
    voice->priority = 0;
    index = (u8)voice->id;
    list = &voiceList[index];
    if (list->state == 0) {
        list->state = 1;
        if (voiceListRoot != 0xFF) {
            list->next = 0xFF;
            list->prev = voiceListInsert;
            voiceList[voiceListInsert].next = index;
        } else {
            list->next = 0xFF;
            list->prev = 0xFF;
            voiceListRoot = index;
        }
        voiceListInsert = index;
        if (voice->isFX)
            voiceFxRunning--;
        else
            voiceMusicRunning--;
    }
    voice->id = 0xFFFFFFFF;
}
void voiceUnblock(u32 index)
{
    if (index != 0xFFFFFFFF) {
        if (hwIsActive(index))
            hwBreak(index);
        synthVoice[index].id = index;
        voiceFree(&synthVoice[index]);
        synthVoice[index].blocked = 0;
    }
}

#pragma auto_inline on
u32 voiceKillSound(u32 vid)
{
    SYNTH_VOICE* voice;
    u32 result = 0xFFFFFFFF;
    u32 next;
    u32 index;
    if (sndActive) {
        vid = vidGetInternalId(vid);
        while (vid != 0xFFFFFFFF) {
            index = (u8)vid;
            voice = &synthVoice[index];
            next = voice->nextID;
            if (vid == voice->id) {
                if (voice->macro) {
                    vidRemoveVoiceReferences(voice);
                    voice->cFlags &= ~(CFLAG_STARTING | CFLAG_HW_BREAK);
                    voice->priorityTime = 0;
                    voiceFree(voice);
                }
                if (voice->blocked)
                    streamKill(index);
                hwBreak(index);
                result = 0;
            }
            vid = next;
        }
    }
    return result;
}

void voiceKill(unsigned long index)
{
    SYNTH_VOICE *voice = &synthVoice[index];
    if (voice->macro) {
        vidRemoveVoiceReferences(voice);
        voice->cFlags &= ~(CFLAG_STARTING | CFLAG_HW_BREAK);
        voice->priorityTime = 0;
        voiceFree(voice);
    }
    if (voice->blocked)
        streamKill(index);
    hwBreak(index);
}

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/synthvoice.c (CC0-1.0). */
#define NULL 0
void synthKillVoicesByMacroReferences(u16 *ref) {
  unsigned long i;
  u16 id;
  for (i = 0; i < synthInfo.voice_count; ++i) {
    if (synthVoice[i].macro == NULL && synthVoice[i].blocked == 0) {
      voiceKill(i);
    }
  }
  while (*ref != 0xFFFF) {
    if ((*ref & 0x8000)) {
      id = *ref & 0x3fff;
      while (id <= ref[1]) {
        for (i = 0; i < synthInfo.voice_count; ++i) {
          if (synthVoice[i].macro != NULL && id == synthVoice[i].macroId) {
            voiceKill(i);
          }
        }
        ++id;
      }
      ref += 2;
    } else {
      for (i = 0; i < synthInfo.voice_count; ++i) {
        if (synthVoice[i].macro != NULL && *ref == synthVoice[i].macroId) {
          voiceKill(i);
        }
      }
      ++ref;
    }
  }
}

#define FALSE 0
extern u8 synthIdleWaitActive;


u32 voiceAllocate(u8 priority, u8 maxVoices,
                  u16 allocId,
                  u8 fxFlag) {
  s32 i;
  s32 num;
  s32 voice;
  u16 p;
  u32 type_alloc;
  VOICE_LIST *sfv;
  if (!synthIdleWaitActive) {
    if (fxFlag) {
      type_alloc = (voiceFxRunning >= synthInfo.maxSFX && synthInfo.voice_count > synthInfo.maxSFX);
      if (synthInfo.maxSFX <= maxVoices) {
        goto _skip_alloc;
      }
      goto _do_alloc;
    } else {
      type_alloc =
          (voiceMusicRunning >= synthInfo.maxMusic && synthInfo.voice_count > synthInfo.maxMusic);
      if (synthInfo.maxMusic <= maxVoices) {
        goto _skip_alloc;
      }
    _do_alloc:
      num = 0;
      voice = -1;
      p = voicePrioSortRootListRoot;
      while (p != 0xFFFF && priority >= p && voice == -1) {
        for (i = voicePrioSortVoicesRoot[p]; i != 0xff; i = voicePrioSortVoices[i].next) {
          if (synthVoice[i].blocked)
            continue;
          if (allocId != synthVoice[i].allocId)
            continue;
          ++num;
          if (!type_alloc || fxFlag == synthVoice[i].isFX) {
            if ((synthVoice[i].cFlags & CFLAG_STARTING))
              continue;
            if (voice != -1) {
              if (synthVoice[i].priorityTime < synthVoice[voice].priorityTime)
                voice = i;
            } else
              voice = i;
          }
        }
        p = voicePrioSortRootList[p].next;
      }
    }
    if (num < maxVoices) {
      while (p != 0xffff && num < maxVoices) {
        i = voicePrioSortVoicesRoot[p];
        while (i != 0xff) {
          if (!synthVoice[i].blocked) {
            if (allocId == synthVoice[i].allocId) {
              num++;
            }
          }
          i = voicePrioSortVoices[i].next;
        }
        p = voicePrioSortRootList[p].next;
      }
      if (num < maxVoices) {
      _skip_alloc:
        voice = -1;
        if (voiceListRoot != 0xff && type_alloc == 0) {
          voice = voiceListRoot;
          goto _update;
        }
        if (priority < voicePrioSortRootListRoot) {
          return -1;
        }
        p = voicePrioSortRootListRoot;
        while (p != 0xFFFF && priority >= p && voice == -1) {
          for (i = voicePrioSortVoicesRoot[p]; i != 0xff; i = voicePrioSortVoices[i].next) {
            if (synthVoice[i].blocked != 0)
              continue;
            if (!type_alloc || fxFlag == synthVoice[i].isFX) {
              if ((synthVoice[i].cFlags & CFLAG_STARTING))
                continue;
              if (voice != -1) {
                if (synthVoice[voice].priorityTime > synthVoice[i].priorityTime)
                  voice = i;
              } else
                voice = i;
            }
          }
          p = voicePrioSortRootList[p].next;
        }
        if (voice == -1) {
          return 0xffffffff;
        }
      _update:
        if (synthVoice[voice].priority > priority) {
          goto _fail;
        }
      }
    }
    if (voice == -1) {
      goto _fail;
    }
    if (voiceList[voice].state == 1) {
      sfv = voiceList + voice;
      i = sfv->prev;
      if (i != 0xff) {
        voiceList[i].next = sfv->next;
      } else {
        voiceListRoot = sfv->next;
      }
      i = sfv->next;
      if (i != 0xff) {
        voiceList[i].prev = sfv->prev;
      }
      if (voice == voiceListInsert) {
        voiceListInsert = sfv->prev;
      }
      sfv->state = 0;
    } else if (synthVoice[voice].isFX) {
      voiceFxRunning--;
    } else {
      voiceMusicRunning--;
    }
    if (fxFlag != FALSE) {
      ++voiceFxRunning;
    } else {
      ++voiceMusicRunning;
    }
    return voice;
  }
_fail:
  return -1;
}

static inline void voiceInitFreeList() {
  u32 i;
  for (i = 0; i < synthInfo.voice_count; ++i) {
    voiceList[i].prev = i - 1;
    voiceList[i].next = i + 1;
    voiceList[i].state = 1;
  }
  voiceList[0].prev = 0xff;
  voiceList[synthInfo.voice_count - 1].next = 0xff;
  voiceListRoot = 0;
  voiceListInsert = synthInfo.voice_count - 1;
}

static inline void voiceInitPrioSort() {
  u32 i;
  for (i = 0; i < synthInfo.voice_count; ++i) {
    voicePrioSortVoices[i].state = 0;
  }
  for (i = 0; i < 256; ++i) {
    voicePrioSortVoicesRoot[i] = 0xff;
  }
  voicePrioSortRootListRoot = 0xffff;
}

void synthInitAllocationAids() {
  voiceInitFreeList();
  voiceInitPrioSort();
  voiceFxRunning = 0;
  voiceMusicRunning = 0;
}



#pragma auto_inline off
void voiceSetPriority(SYNTH_VOICE *svoice, u8 prio) {
  u16 li;               
  VOICE_LIST *vps; 
  u16 i;                
  unsigned long v;                
  v = svoice->id & 0xFF;
  vps = &voicePrioSortVoices[v];
  if (vps->state == 1) {
    if (svoice->priority == prio) {
      return;
    }

    voiceRemovePriority(svoice);
  }

  vps->state = 1;
  vps->prev = 0xff;
  if ((vps->next = voicePrioSortVoicesRoot[prio]) != 0xFF) {
    voicePrioSortVoices[voicePrioSortVoicesRoot[prio]].prev = v;
  } else if (voicePrioSortRootListRoot != 0xFFFF) {
    if (prio >= voicePrioSortRootListRoot) {
      for (i = voicePrioSortRootListRoot; i != 0xFFFF; i = voicePrioSortRootList[i].next) {
        if ((u16)i > prio) {
          break;
        }
        li = i;
      }

      voicePrioSortRootList[li].next = (u16)prio;
      voicePrioSortRootList[prio].prev = li;
      voicePrioSortRootList[prio].next = i;
      if (i != 0xFFFF) {
        voicePrioSortRootList[i].prev = prio;
      }

    } else {
      voicePrioSortRootList[prio].next = voicePrioSortRootListRoot;
      voicePrioSortRootList[prio].prev = 0xFFFF;
      voicePrioSortRootList[voicePrioSortRootListRoot].prev = prio;
      voicePrioSortRootListRoot = prio;
    }
  } else {
    voicePrioSortRootList[prio].next = 0xFFFF;
    voicePrioSortRootList[prio].prev = 0xFFFF;
    voicePrioSortRootListRoot = prio;
  }

  voicePrioSortVoicesRoot[prio] = v;
  svoice->priority = prio;
  hwSetPriority(svoice->id & 0xFF, ((unsigned long)prio << 24) | (svoice->priorityTime >> 15));
}
