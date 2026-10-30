#include "audio.h"
#include "musyx_midictrl.h"
typedef struct AudioHooks {
    AudioAllocate allocate;
    AudioFree free;
} AudioHooks;
typedef struct AudioSFXEntry {
    struct AudioSFXEntry *next;
    u32 voice;
    u16 effect;
} AudioSFXEntry;
typedef struct AudioSFXStack {
    struct AudioSFXStack *next;
    void *pool;
    void *project;
    void *directory;
} AudioSFXStack;
typedef struct AudioSequence {
    struct AudioSequence *next;
    u32 id;
    void *data;
} AudioSequence;
static AudioAllocate hsd_AudioMallocProc;
static AudioFree hsd_AudioFreeProc;
/* Explicit zero initializers retain the SDK small-data order. */
AudioSFXStack *hsd_AudioSFXStackTop = 0;
s32 hsd_AudioSFXLoadBusyFlag = 0;
s32 hsd_AudioSeqLoadBusyFlag = 0;
static AudioSequence *seqhead;
s32 hsd_AudioStreamVolumeVal = 0;
extern f32 HSD_AudioOutPutGain;
s32 hsd_AudioSeqParaFlag = 0;
u16 hsd_AudioSeqSid = 0;
u16 hsd_AudioSeqSgid = 0;
void *hsd_AudioSeqSongdata = 0;
void *hsd_AudioSFXLoadSam = 0;
void *hsd_AudioSFXLoadSdi = 0;
void *hsd_AudioSFXLoadPro = 0;
u16 hsd_AudioSFXLoadGid = 0;
void *hsd_AudioSFXLoadPoo = 0;
AudioSFXEntry *HSD_AudioSFXEntryFreeList = 0;
AudioSFXEntry *HSD_AudioSFXEntryHead = 0;
extern void sndSetHooks(AudioHooks *hooks);
extern s32 sndPopGroup(void);
extern u32 sndFXStartEx(u16 effect, u8 volume, u8 pan, u8 studio);
extern s32 sndFXKeyOff(u32 voice);
extern u32 sndFXCheck(u32 voice);
extern s32 sndFXCtrl(u32 voice, u8 control, u8 value);
extern s32 sndSeqStop(u32 sequence);
extern s32 sndSeqVolume(u8 volume, u16 time, u32 sequence, u8 mode);
extern void AISetStreamVolLeft(u8 volume);
extern void AISetStreamVolRight(u8 volume);
extern void __assert(const char *file, u32 line, const char *condition);
typedef struct DVDFileInfo {
    u8 command_block[48];
    u32 start_addr;
    u32 length;
    void (*callback)(s32, struct DVDFileInfo *);
} DVDFileInfo;
typedef struct AudioStream {
    DVDFileInfo file;
    u32 unknown;
    s32 active;
} AudioStream;
typedef struct AudioSequenceParameters {
    u32 words[8];
} AudioSequenceParameters;
static u32 sfxgroupentry[128];
DVDFileInfo hsd_AudioFInfo[4];
AudioSequenceParameters hsd_AudioSeqPara;
DVDFileInfo hsd_AudioSeqFInfo;
extern s32 OSEnableInterrupts(void);
extern s32 OSRestoreInterrupts(s32 enabled);
extern s32 sndPushGroup(void *project, u16 group, void *samples, void *directory,
                        void *pool);
extern u32 sndSeqPlayEx(u16 group, u16 song, void *data,
                        AudioSequenceParameters *parameters, u8 studio);
extern void HSD_Free(void *memory);
extern void *HSD_Alloc(u32 size);
extern s32 sprintf(char *buffer, const char *format, ...);
extern s32 DVDReadAsyncPrio(DVDFileInfo *file, void *buffer, s32 length, s32 offset,
                            void (*callback)(s32, DVDFileInfo *), s32 priority);
extern void OSReport(const char *format, ...);
extern s32 DVDOpen(const char *name, DVDFileInfo *file);
extern s32 DVDClose(DVDFileInfo *file);
extern s32 DVDPrepareStreamAsync(DVDFileInfo *file, u32 length, u32 offset,
                                 void (*callback)(s32, DVDFileInfo *));
extern s32 DVDCancelStream(DVDFileInfo *file);
extern s32 DVDStopStreamAtEndAsync(DVDFileInfo *file,
                                   void (*callback)(s32, DVDFileInfo *));
extern void AISetStreamPlayState(u32 state);
extern u32 ARAlloc(u32 size);
extern s32 sndInit(u8 voices, u8 music, u8 effects, u8 studios, u32 flags,
                   u32 aram_size);
extern void sndVolume(u8 volume, u16 time, u8 group);
extern void AISetDSPSampleRate(u32 rate);
#pragma force_active on
static void HSD_AudioSFXEntryGC(void);
#pragma force_active off
static inline void *audio_allocate(u32 size)
{
    void *p = hsd_AudioMallocProc(size);
    s32 allocated = p != 0;
    if (!allocated) {
        OSReport("audio heap overflow.\n");
        __assert("audio.c", 51, "p");
    }
    return p;
}
static inline void *audio_allocate2(u32 size)
{
    void *p = hsd_AudioMallocProc(size);
    if (p == 0) {
        OSReport("audio heap overflow.\n");
        __assert("audio.c", 51, "p");
    }
    return p;
}
static inline void *audio_load_file(char *path, DVDFileInfo *file)
{
    void *buffer;
    u32 size;

    if (DVDOpen(path, file) == 0) {
        buffer = 0;
    } else {
        size = file->length;
        size = (size + 31) & ~31;
        DVDReadAsyncPrio(file, buffer = audio_allocate(size), size, 0, 0, 2);
    }
    return buffer;
}
extern AudioStream stream;
void HSD_AudioInit(u8 voices, u8 music, u8 effects, u32 aram_size)
{
    s32 i;
    (void)hsd_AudioFInfo;
    ARAlloc(aram_size);
    sndInit(voices, music, effects, 8, 0, aram_size);
    sndVolume(127, 0, 255);
    HSD_AudioSFXEntryHead = audio_allocate((effects + 2) * sizeof(AudioSFXEntry));
    HSD_AudioSFXEntryHead->next = 0;
    HSD_AudioSFXEntryFreeList = HSD_AudioSFXEntryHead + 1;
    for (i = 1; i <= effects; i++)
        HSD_AudioSFXEntryHead[i].next = HSD_AudioSFXEntryHead + i + 1;
    HSD_AudioSFXEntryHead[i].next = 0;
    for (i = 0; i < 128; i++)
        sfxgroupentry[i] = 0;
    AISetDSPSampleRate(0);
}

#pragma force_active on
void HSD_AudioSFXLoadCallback(s32 result, DVDFileInfo *file);

static void HSD_AudioSeqPlayCallback(s32 result, DVDFileInfo *file)
{
    AudioSequence *entry = audio_allocate(sizeof(AudioSequence));
    entry->data = hsd_AudioSeqSongdata;
    if (hsd_AudioSeqParaFlag)
        entry->id = sndSeqPlayEx(hsd_AudioSeqSgid, hsd_AudioSeqSid, entry->data,
                                 &hsd_AudioSeqPara, 0);
    else
        entry->id = sndSeqPlayEx(hsd_AudioSeqSgid, hsd_AudioSeqSid, entry->data, 0, 0);
    entry->next = seqhead;
    seqhead = entry;
    hsd_AudioSeqLoadBusyFlag = 0;
}
#pragma force_active off

AudioStream stream = {0};
void HSD_AudioSetHooks(AudioAllocate allocate, AudioFree free)
{
    AudioHooks hooks;
    hooks.allocate = allocate;
    hsd_AudioMallocProc = allocate;
    hsd_AudioFreeProc = free;
    hooks.free = free;
    sndSetHooks(&hooks);
}
#pragma force_active on
static void HSD_AudioSFXEntryGC(void)
{
    AudioSFXEntry *entry;
    AudioSFXEntry *next;
    entry = HSD_AudioSFXEntryHead->next;
    for (; entry != 0; entry = next) {
        next = entry->next;
        if (sndFXCheck(entry->voice) == 0xFFFFFFFF) {
            AudioSFXEntry *previous = HSD_AudioSFXEntryHead;
            while (previous->next != 0) {
                if (previous->next == entry) {
                    previous->next = entry->next;
                    entry->next = HSD_AudioSFXEntryFreeList;
                    HSD_AudioSFXEntryFreeList = entry;
                    break;
                }
                previous = previous->next;
            }
        }
    }
}
#pragma force_active off
static inline void *audio_load_samples(char *path, DVDFileInfo *file)
{
    void *buffer;
    u32 size;

    if (DVDOpen(path, file) == 0) {
        buffer = 0;
    } else {
        size = file->length;
        size = (size + 31) & ~31;
        buffer = audio_allocate2(size);
        DVDReadAsyncPrio(file, buffer, size, 0, HSD_AudioSFXLoadCallback, 2);
    }
    return buffer;
}
s32 HSD_AudioSFXLoad(char *name, u16 gid)
{
    char path[0xC8];

    if (hsd_AudioSFXLoadBusyFlag == 1) {
        return 0;
    }
    hsd_AudioSFXLoadBusyFlag = 1;
    sprintf(path, "%s.poo", name);
    hsd_AudioSFXLoadPoo = audio_load_file(path, &hsd_AudioFInfo[0]);
    if (hsd_AudioSFXLoadPoo == 0) {
        return 0;
    }
    sprintf(path, "%s.pro", name);
    hsd_AudioSFXLoadPro = audio_load_file(path, &hsd_AudioFInfo[1]);
    if (hsd_AudioSFXLoadPro == 0) {
        return 0;
    }
    sprintf(path, "%s.sdi", name);
    hsd_AudioSFXLoadSdi = audio_load_file(path, &hsd_AudioFInfo[2]);
    if (hsd_AudioSFXLoadSdi == 0) {
        return 0;
    }
    sprintf(path, "%s.sam", name);
    hsd_AudioSFXLoadSam = audio_load_samples(path, &hsd_AudioFInfo[3]);
    if (hsd_AudioSFXLoadSam == 0) {
        return 0;
    }
    hsd_AudioSFXLoadGid = gid;
    return 1;
}


u32 HSD_AudioSFXLoadCheck(void)
{
    return hsd_AudioSFXLoadBusyFlag;
}
void HSD_AudioSFXPop(void)
{
    AudioSFXStack *entry;
    sndPopGroup();
    entry = hsd_AudioSFXStackTop;
    hsd_AudioFreeProc(hsd_AudioSFXStackTop->pool);
    hsd_AudioFreeProc(entry->project);
    hsd_AudioFreeProc(entry->directory);
    hsd_AudioSFXStackTop = entry->next;
    hsd_AudioFreeProc(entry);
}
static inline void audio_register_voice(u16 effect, u32 voice)
{
    AudioSFXEntry *entry;
    if (HSD_AudioSFXEntryFreeList == 0) {
        HSD_AudioSFXEntryGC();
        if (HSD_AudioSFXEntryFreeList == 0)
            return;
    }
    entry = HSD_AudioSFXEntryFreeList;
    HSD_AudioSFXEntryFreeList = entry->next;
    entry->voice = voice;
    entry->effect = effect;
    entry->next = HSD_AudioSFXEntryHead->next;
    HSD_AudioSFXEntryHead->next = entry;
}
u32 HSD_AudioSFXStart(u16 effect, u8 volume, u8 pan)
{
    u32 voice = sndFXStartEx(effect, volume, pan, 0);
    audio_register_voice(effect, voice);
    return voice;
}


void HSD_AudioSFXKeyOffAll(void)
{
    AudioSFXEntry *entry;
    AudioSFXEntry *next;
    entry = HSD_AudioSFXEntryHead->next;
    for (; entry != 0; entry = next) {
        next = entry->next;
        if (sndFXCheck(entry->voice) == 0xFFFFFFFF) {
            AudioSFXEntry *previous = HSD_AudioSFXEntryHead;
            while (previous->next != 0) {
                if (previous->next == entry) {
                    previous->next = entry->next;
                    entry->next = HSD_AudioSFXEntryFreeList;
                    HSD_AudioSFXEntryFreeList = entry;
                    break;
                }
                previous = previous->next;
            }
        } else
            sndFXKeyOff(entry->voice);
    }
}



u32 HSD_AudioSFXCheck(u32 voice)
{
    return sndFXCheck(voice);
}
s32 HSD_AudioSeqPlay(u16 group, u16 song, char *name, AudioSequenceParameters *parameters)
{
    void *buffer;
    u32 size;
    if (hsd_AudioSeqLoadBusyFlag == 1)
        return 0;
    hsd_AudioSeqLoadBusyFlag = 1;
    hsd_AudioSeqSgid = group;
    hsd_AudioSeqSid = song;
    if (parameters != 0) {
        hsd_AudioSeqParaFlag = 1;
        hsd_AudioSeqPara = *parameters;
    } else {
        hsd_AudioSeqParaFlag = 0;
    }
    if (DVDOpen(name, &hsd_AudioSeqFInfo) == 0) {
        buffer = 0;
    } else {
        size = (hsd_AudioSeqFInfo.length + 31) & ~31;
        buffer = audio_allocate(size);
        DVDReadAsyncPrio(&hsd_AudioSeqFInfo, buffer, size, 0, HSD_AudioSeqPlayCallback, 2);
    }
    hsd_AudioSeqSongdata = buffer;
    return 1;
}
u32 HSD_AudioSeqPlayCheck(void)
{
    return hsd_AudioSeqLoadBusyFlag;
}

void HSD_AudioSeqStop(u32 sequence)
{
    sndSeqStop(sequence);
}
void HSD_AudioSeqVolume(u8 volume, u16 time, u32 sequence, u8 mode)
{
    sndSeqVolume(volume, time, sequence, mode);
}
void HSD_AudioSeqFree(u32 sequence)
{
    AudioSequence **slot = &seqhead;
    while (*slot != 0) {
        if ((*slot)->id == sequence) {
            AudioSequence *next;
            hsd_AudioFreeProc((*slot)->data);
            next = (*slot)->next;
            hsd_AudioFreeProc(*slot);
            *slot = next;
            break;
        }
        slot = &(*slot)->next;
    }
}
#pragma dont_inline on
void HSD_AudioStreamSetVolume(s32 volume)
{
    if (hsd_AudioStreamVolumeVal != volume) {
        hsd_AudioStreamVolumeVal = volume;
        HSD_AudioStreamUpdateVolume();
    }
}
#pragma dont_inline off
void HSD_AudioStreamVolume(s32 volume)
{
    HSD_AudioStreamSetVolume(volume);
}
void HSD_AudioStreamUpdateVolume(void)
{
    AISetStreamVolLeft(2.0078740157480315 *
                       (hsd_AudioStreamVolumeVal * HSD_AudioOutPutGain));
    AISetStreamVolRight(2.0078740157480315 *
                        (hsd_AudioStreamVolumeVal * HSD_AudioOutPutGain));
}

void HSD_AudioStreamStart(const char *name, u32 offset)
{
    s32 active = stream.active;
    s32 opened;
    if (active)
        DVDCancelStream(&stream.file);
    opened = DVDOpen(name, &stream.file);
    if (opened) {
        stream.active = 1;
        DVDPrepareStreamAsync(&stream.file, stream.file.length - offset, offset, 0);
        AISetStreamPlayState(1);
    }
}
void HSD_AudioStreamStop(void)
{
    s32 active = 0;
    stream.active = active;
    AISetStreamPlayState(0);
    DVDCancelStream(&stream.file);
    DVDClose(&stream.file);
}
void HSD_AudioStreamStopAtEnd(void)
{
    DVDStopStreamAtEndAsync(&stream.file, 0);
}
