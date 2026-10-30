extern "C" {
#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"

typedef struct GXColor3 {
    u8 r, g, b, a;
} GXColor3;
typedef struct RenderMode3 {
    u32 viTVmode;
    u16 fbWidth;
    u16 efbHeight;
    u8 reserved_08[0x34];
} RenderMode3;
typedef struct PadMini3 {
    u8 reserved_00[0x4E];
    s8 f4e;
    u8 reserved_4F[0xE];
    s8 pressed_a;
    s8 pressed_b;
} PadMini3;
extern RenderMode3 GXNtsc480IntDf;
extern PadMini3 *sPad1;
extern s32 OSEnableInterrupts(void);
extern s32 OSRestoreInterrupts(s32 level);
extern void *memset(void *, s32, u32);
extern void DCFlushRange(void *addr, u32 length);
extern void AIInitDMA(u32 start_addr, u32 length);
extern u32 THPPlayerAudioOutput(s16 *, u32, s32);
extern void OSReport(const char *format, ...);
extern void GXSetDispCopyGamma(s32 gamma);
extern void GXSetCopyFilter(u8 aa, const u8 *samples, u8 vertical, const u8 *vfilter);
extern void GXSetCopyClear(GXColor3 color, u32 depth);
extern s32 THPPlayerInit(void);
extern void AudioInit(void);
extern void THP_AudioEnd(void);
extern void chkVS(void);
extern void sPadAll(void);
extern s32 sReadPre(void);
extern void HSD_StartRender(s32 pass);
extern void HSD_EndRender(void);
extern void HSD_VICopyXFB(s32 a);
extern void HSD_VISetXFBDrawDone(void);
extern void VIWaitForRetrace(void);
extern s32 THPPlayerDrawCurrentFrame(RenderMode3 *mode, u32 x, u32 y, u32 w, u32 h);
extern void THPPlayerDrawDone(void);
extern s32 THPPlayerGetState(void);
extern void THPPlayerStop(void);
extern s32 THPPlayerClose(void);
extern void THPPlayerQuit(void);
extern s32 THPPlayerOpen(char *name, s32 flag);
extern s32 THPPlayerGetVideoInfo(u32 *info);
extern s32 THPPlayerCalcNeedMemory(void);
extern s32 THPPlayerSetBuffer(void *buffer);
extern s32 THPPlayerPrepare(s32 offset, s32 loop);
extern s32 THPPlayerPlay(void);
extern void *malloc2(u32 size);
extern void free(void *p);
extern char *strcpy(char *dst, const char *src);

void TEA_THP_PlaySkip(char *path, s32 loop, s32 skip);
void AudioCallback(void);
#pragma force_active on
static s16 SoundBuffer[2][480];
static s32 SoundBufferNumber;
s32 oldaudiocallback;
#pragma force_active off
void TEA_THP_PlaySkip(char *path, s32 loop, s32 skip)
{
    GXColor3 color;
    u32 info[2];
    char name[52];
    s32 playing;
    void *buffer;
    s32 want;
    RenderMode3 *mode;
    s32 dead = 0;
    s32 looping = 0;
    s32 state = 0;
    s32 flag;
    s32 first = 1;
    s32 skipped = 0;
    s32 dead2;
    u32 xoff;
    u32 yoff;

    if (skip == 0)
        loop = 0;
    color.r = 0;
    color.g = 0;
    color.b = 0;
    color.a = 255;
    OSReport("PlayMovieInit:THP\n");
    GXSetDispCopyGamma(0);
    GXSetCopyFilter(0, 0, 0, 0);
    GXSetCopyClear(color, 0xFFFFFF);
    mode = &GXNtsc480IntDf;
    THPPlayerInit();
    AudioInit();
    want = 0;
    playing = 0;
    flag = 0;
    chkVS();
    for (;;) {
        dead = 1;
        dead2 = 1;
        dead2 = 2;
        sPadAll();
        if (sReadPre() != 0)
            skipped = 1;
        HSD_StartRender(0);
        if (playing != 0)
            THPPlayerDrawCurrentFrame(mode, xoff, yoff, info[0], info[1]);
        THPPlayerDrawDone();
        HSD_EndRender();
        HSD_VICopyXFB(0);
        HSD_VISetXFBDrawDone();
        VIWaitForRetrace();
        state = THPPlayerGetState();
        if (state == 5 || state == 3) {
            THPPlayerStop();
            THPPlayerClose();
            free(buffer);
            OSReport("PlayMovieEnd:THP\n");
            break;
        }
        if (first == 1) {
            strcpy(name, path);
            if (loop == 1)
                looping = 1;
            else
                looping = 0;
            want = 1;
            flag = 0;
            first = 0;
        }
        if (skip == 0) {
            sPad1->f4e = 0;
            sPad1->pressed_a = 0;
            sPad1->pressed_b = 0;
        }
        if (sPad1->f4e != 0 || sPad1->pressed_a != 0 || sPad1->pressed_b != 0 || skipped != 0) {
            if (playing != 0) {
                THPPlayerStop();
                THPPlayerClose();
                free(buffer);
                OSReport("PlayMovieStop:THP\n");
            }
            break;
        }
        if (want != 0 && playing != 0) {
            THPPlayerStop();
            THPPlayerClose();
            free(buffer);
            playing = 0;
        }
        if (want != 0 && playing == 0) {
            if (sReadPre() != 0)
                skipped = 1;
            if (THPPlayerOpen(name, flag) == 0)
                return;
            OSReport("file(%s) \n", name);
            THPPlayerGetVideoInfo(info);
            xoff = (mode->fbWidth - info[0]) >> 1;
            yoff = (mode->efbHeight - info[1]) >> 1;
            buffer = malloc2(THPPlayerCalcNeedMemory());
            if (buffer == 0)
                return;
            THPPlayerSetBuffer(buffer);
            if (THPPlayerPrepare(0, looping) == 0)
                return;
            THPPlayerPlay();
            want = 0;
            playing = 1;
        }
    }
    OSReport("PlayMovieQuit:THP\n");
    THPPlayerQuit();
    THP_AudioEnd();
    VIWaitForRetrace();
}

void AudioCallback(void)
{
    s32 level;
    u32 unused;

    level = OSEnableInterrupts();
    SoundBufferNumber ^= 1;
    memset(SoundBuffer[SoundBufferNumber], 0, 0x3C0);
    unused = THPPlayerAudioOutput(SoundBuffer[SoundBufferNumber], 0xF0, 0x7F);
    DCFlushRange(SoundBuffer[SoundBufferNumber], 0x3C0);
    AIInitDMA((u32)SoundBuffer[SoundBufferNumber], 0x3C0);
    OSRestoreInterrupts(level);
}

static const char lit_469[10] = "DemoInit\n";
static const char lit_471[61] = "###########################################################\n";
static const char lit_472[44] = "Push A Button\t : Play the movie with audio\n";
static const char lit_473[58] = "Push B Button\t : Play the movie with audio on the memory\n";
static const char lit_474[43] = "Push X Button\t : Play the interlace movie\n";
static const char lit_475[47] = "Push Y Button\t : Play the movie without audio\n";
static const char lit_476[44] = "Push L Trigger\t: Pause / Restart the movie\n";
static const char lit_477[56] = "Push R Trigger\t: Skip one frame if the movie is paused\n";
static const char lit_478[37] = "Push Start Button : Application end\n";
static const char lit_479[20] = "thpdemo/rebirth.thp";
static const char lit_480[17] = "thpdemo/fish.thp";
static const char lit_481[21] = "thpdemo/fish_int.thp";
static const char lit_482[20] = "thpdemo/fish_no.thp";
static const char lit_484[27] = "Fail to open the thp file\n";
static const char lit_485[17] = "application end\n";
}
