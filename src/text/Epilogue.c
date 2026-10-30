#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"

#include "model_archive.h"
#include "game_mode.h"
typedef struct PadState3 {
    u8 pad_00[0x5d];
    s8 pressed_a;
} PadState3;
typedef struct LoadColor { u8 r, g, b, a; } LoadColor;
typedef struct LoadVIInfo { u8 pad[0x48]; LoadColor clear_clr; } LoadVIInfo;
extern LoadVIInfo HSD_VIData;
extern ModelArchive *dspBP;
extern PadState3 *sPad1;
extern void *fogG;
extern u8 IveSave[];
extern u8 IveSaveFlag[];
extern void OSReport(const char *format, ...);
extern void sDispLoadPartsBP(void);
extern void sDispPurgePartsBP(void);
extern void HSD_VISetBlack(s32 black);
extern void HSD_SetZSortMode(s32 a, s32 b);
extern void gClear(void);
extern s32 jobjAnim(void *joint);
extern void allRank0(s32 rank);
extern void sPadAll(void);
extern s32 sReadPre(void);
extern void waitV(s32 frames);
typedef struct GameZoidEntry3 {
    s16 id;
    u8 pad[0x1e];
} GameZoidEntry3;
typedef struct ZoidS3 {
    u8 pad_00[0x34];
    void *anim_table;
    u8 pad_38[8];
    void *joint;
} ZoidS3;
typedef struct AObj3 {
    u32 flags;
    f32 current_frame;
    f32 rewind_frame;
    f32 end_frame;
} AObj3;
extern GameZoidEntry3 GameZoid[16];
extern ZoidS3 Zoid;
extern ModelArchive *dspFilter;
extern void debug11(void);
extern void TeaBgm(s16 track);
extern void TeaMenuBGM(s32 track);
extern ModelArchive *LoadArc2(char *name);
extern void PurgeArc2(ModelArchive *archive);
extern void ZoidInit1(void *zoid, s32 a, s32 b);
extern void *malloc2(u32 size);
extern void clearAscii(void);
extern void *sReadFileLenU(char *name, s32 mode, s32 *length);
extern u8 *cpyCr(u8 *source, char *destination);
extern void LoadArcAnimLight2(ModelArchive *archive, struct HSD_LObj *light, s32 selected);
extern AObj3 *getAnimAobj(void *joint);
extern void sAddAnimAll(void *joint, void *table, s32 selected);
extern void ZoidSetAnim(void *zoid, s32 anim);
extern void *HSD_JObjAlloc(void);
extern void HSD_JObjUnref(void *jobj);
extern void closeLoop(void);
extern void psKillAllGenerator(void);
extern void psKillAllParticle(void);
extern void sWeaponUnMountAll(void *zoid);
extern void ZoidPurge(void *zoid);
extern void HSD_StateInvalidate(s32 state);
extern void free(void *memory);
extern void __assert(const char *file, u32 line, const char *expr);


extern ModelArchive *textPos;
extern ModelArchive *ending;
extern u8 *staff;
extern s32 staffLen;
extern s32 cOn;
extern s32 fOn;
extern s32 dOn;
extern u8 *sStr;
extern s32 strNextTm;
extern s32 strConfNext;
extern void *j0;
extern void *j1;
extern s32 strConfNextAll;
extern s32 strConfUp;
extern s32 strConfAll;

#pragma force_active on
static const char lit_438[37] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
#pragma force_active off

void Epilogue(void)
{
    s32 i;
    u8 *p;
    AObj3 *aobj;
    char buf[0x100];

    OSReport("epilogue start\n");
    debug11();
    cOn = 0;
    fOn = 0;
    dOn = 1;
    if (GameZoid[0].id < 0) {
        GameZoid[0].id = 3;
    }
    if (GameZoid[0].id > 160) {
        GameZoid[0].id = 3;
    }
    HSD_SetZSortMode(0, 0);
    TeaBgm(-1);
    TeaMenuBGM(7);
    textPos = LoadArc2("epilogue/credit.dat");
    if (textPos == 0) {
        OSReport("textPos credit.dat not found .\n");
    }
    fogG = 0;
    ending = LoadArc2("epilogue/ending.dat");
    if (ending == 0) {
        OSReport("ending.dat not found\n");
    } else {
        fogG = ending->fog;
    }
    ZoidInit1(&Zoid, 0, 0);
    sStr = (u8 *)malloc2(0xd20);
    if (sStr == 0) {
        OSReport("StStr out of memory.\n");
    } else {
        clearAscii();
        for (i = 0; i < 0x28; i++) {
            sStr[i * 0x54] = 0;
        }
    }
    staff = (u8 *)sReadFileLenU("epilogue/epilogue.txt", 0, &staffLen);
    if (staff != 0) {
        if (staffLen <= 0) {
            staffLen = 1;
        }
        staff[staffLen - 1] = 0;
        strConfAll = 0;
        p = staff;
        while (*p != 0) {
            p = cpyCr(p, buf);
            strConfAll++;
        }
        OSReport("%d line(s) \n", strConfAll);
    } else {
        OSReport("staff text not found.\n");
    }
    strNextTm = 0;
    if (ending != 0) {
        OSReport("SetEndingAnime ");
        SetAnimArc2(ending, 0, 1);
        jobjAnim(ending->models[0]);
        SetAnimArc2(ending, 1, 1);
        jobjAnim(ending->models[1]);
        SetAnimArc2(ending, 2, 1);
        jobjAnim(ending->models[2]);
        SetAnimArc2(ending, 5, 1);
        jobjAnim(ending->models[5]);
        OSReport("SetEndingLight ");
        LoadArcAnimLight2(ending, ending->lights, 1);
        OSReport("GetEndingAobj ");
        aobj = getAnimAobj(ending->models[2]);
        OSReport("AobjIs[%s] ", aobj ? "on" : "non");
        if (aobj != 0) {
            aobj ? (void)0 : __assert("aobj.h", 0xab, "aobj");
            strConfNextAll = (s32)aobj->end_frame;
            strConfNext = strConfNextAll / strConfAll;
            OSReport("strn=%d ", strConfNext);
        }
    }
    if (Zoid.joint != 0) {
        OSReport("epi -anim ");
        sAddAnimAll(Zoid.joint, Zoid.anim_table, 4);
        ZoidSetAnim(&Zoid, 5);
    }
    j0 = HSD_JObjAlloc();
    j1 = HSD_JObjAlloc();
    if (dspFilter != 0) {
        SetAnimArc2(dspFilter, 0, 0);
        jobjAnim(dspFilter->models[0]);
    } else {
        OSReport("un Filter ");
    }
    closeLoop();
    TeaBgm(-1);
    HSD_JObjUnref(j1);
    HSD_JObjUnref(j0);
    if (staff != 0) {
        free(staff);
    }
    if (sStr != 0) {
        free(sStr);
    }
    sWeaponUnMountAll(&Zoid);
    if (Zoid.joint != 0) {
        ZoidPurge(&Zoid);
    }
    if (ending != 0) {
        PurgeArc2(ending);
    }
    if (textPos != 0) {
        PurgeArc2(textPos);
    }
    HSD_StateInvalidate(-1);
}
