#include "audio.h"
#include "dvd_file.h"
#include "os_interrupt.h"
#include "os_error.h"
#include "os_memory.h"

typedef struct AudioSFXStack {
    struct AudioSFXStack *next;
    void *pool;
    void *project;
    void *directory;
} AudioSFXStack;

extern AudioSFXStack *hsd_AudioSFXStackTop;
extern s32 hsd_AudioSFXLoadBusyFlag;
extern void *hsd_AudioSFXLoadSam;
extern void *hsd_AudioSFXLoadSdi;
extern void *hsd_AudioSFXLoadPro;
extern u16 hsd_AudioSFXLoadGid;
extern void *hsd_AudioSFXLoadPoo;
extern AudioAllocate hsd_AudioMallocProc;
extern AudioFree hsd_AudioFreeProc;
extern void __assert(const char *file, u32 line, const char *condition);

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

#pragma force_active on
static void HSD_AudioSFXLoadCallback(s32 result, DVDFileInfo *file)
{
    AudioSFXStack *entry;
    s32 previous = OSEnableInterrupts();
    sndPushGroup(hsd_AudioSFXLoadPro, hsd_AudioSFXLoadGid, hsd_AudioSFXLoadSam,
                 hsd_AudioSFXLoadSdi, hsd_AudioSFXLoadPoo);
    OSRestoreInterrupts(previous);
    hsd_AudioFreeProc(hsd_AudioSFXLoadSam);
    entry = audio_allocate(sizeof(AudioSFXStack));
    entry->pool = hsd_AudioSFXLoadPoo;
    entry->project = hsd_AudioSFXLoadPro;
    entry->directory = hsd_AudioSFXLoadSdi;
    entry->next = hsd_AudioSFXStackTop;
    hsd_AudioSFXStackTop = entry;
    hsd_AudioSFXLoadBusyFlag = 0;
}
#pragma force_active off
