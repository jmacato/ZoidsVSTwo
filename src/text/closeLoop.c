#include "zoid3.h"
#include "model_archive.h"
#include "game_pad.h"
#include "video.h"
#include "lobj.h"
#include "gx_tev.h"

#pragma auto_inline off
#pragma readonly_strings on

typedef struct AsciiEntry {
    char text[80];
    f32 scroll_y;
} AsciiEntry;

typedef struct PadStateVS3 {
    u8 reserved_00[0x63];
    s8 pressed_start;
} PadStateVS3;

extern AsciiEntry *sStr;
extern ModelArchive *textPos, *ending, *dspFilter;
extern u8 *staff;
extern s32 strNextTm, strConfNext;
extern s32 fOn, cOn, dOn;
extern PadState *sPad1;
extern u8 *cpyCr(u8 *source, char *destination);
extern void setAscii(char *text);
extern void SetAnimArc2(ModelArchive *archive, s32 model, s32 selected);
extern s32 jobjAnim(HSD_JObj *joint);
extern void ZoidAnim(ZoidState *zoid);
extern void ZoidToFly(ZoidState *zoid);
extern void HSD_LObjAnimAll(HSD_LObj *lobj);
extern void closeDispAll(void);
extern void clearAscii(void);
extern void sPadAll(void);
extern void psInterpretParticles(s32 arg);
extern void psExecGenerator(s32 arg);
extern void psDispParticles(s32 mask, s32 layer);
extern void HSD_VISetBlack(int black);
extern void HSD_SetZSortMode(s32 a, s32 b);

static inline void SetClearColor(GXColor c)
{
    HSD_VIData.current.vi.clear_clr = c;
}

void closeLoop(void)
{
    ZoidState *zoid;
    char text[256];
    s32 i;
    s32 running;
    u8 *line;

    if (sStr != 0 && textPos != 0 && ending != 0 && (zoid = Zoid, zoid->joint != 0) && staff != 0) {
        SetClearColor((GXColor){0, 0, 0, 0});
        HSD_VISetBlack(0);
        HSD_SetZSortMode(1, 1);
        line = staff;
        running = 1;
        do {
            if (*line != 0)
                strNextTm++;
            if (strNextTm >= strConfNext) {
                strNextTm = 0;
                line = cpyCr(line, text);
                setAscii(text);
                if (*line == 0) {
                    SetAnimArc2(dspFilter, 0, 3);
                    fOn = 1;
                }
            }
            if (*line == 0 && jobjAnim(dspFilter->models[0]) != 0) {
                SetAnimArc2(dspFilter, 0, 4);
                jobjAnim(dspFilter->models[0]);
                fOn = 1;
            }
            jobjAnim(ending->models[0]);
            jobjAnim(ending->models[1]);
            if (jobjAnim(ending->models[2]) != 0)
                running = 0;
            jobjAnim(ending->models[5]);
            ZoidAnim(zoid);
            ZoidToFly(zoid);
            HSD_LObjAnimAll(ending->lights);
            closeDispAll();
            sPadAll();
            if (((PadStateVS3 *)sPad1)->pressed_start != 0)
                running = 0;
        } while (running != 0);
        clearAscii();
        SetAnimArc2(dspFilter, 0, 5);
        cOn = 1;
        do {
            closeDispAll();
            sPadAll();
        } while (jobjAnim(dspFilter->models[0]) == 0);
        fOn = 0;
        for (i = 0; i < 180; i++) {
            closeDispAll();
            sPadAll();
        }
        fOn = 1;
        SetAnimArc2(dspFilter, 0, 3);
        jobjAnim(dspFilter->models[0]);
        do {
            closeDispAll();
            sPadAll();
        } while (jobjAnim(dspFilter->models[0]) == 0);
        dOn = 0;
        for (i = 0; i < 60; i++) {
            closeDispAll();
            sPadAll();
        }
    }
}
