extern "C" {
#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"

#include "joint.h"
#include "zoid_collision.h"
#include "model_archive.h"
typedef struct WeaponMount2 {
    s32 active;
    u8 pad[0x34];
} WeaponMount2;
typedef struct ZoidState {
    u8 pad000[0x20];
    s32 type;
    s32 person;
    u8 pad028[0x40 - 0x28];
    void *joint;
    u8 pad044[0x14C - 0x44];
    s32 health;
    s32 max_health;
    u8 pad154[0x12FC - 0x154];
    s32 weapon_mount;
    u8 pad1300[0x1304 - 0x1300];
    s32 selected_weapon_mount;
    u8 pad1308[0x130C - 0x1308];
    WeaponMount2 weapon_mounts[10];
    u8 pad153C[0x1B68 - 0x153C];
    f32 position[3];
    u8 pad1B74[0x1BC0 - 0x1B74];
    f32 heading;
    u8 pad1BC4[0x1C2C - 0x1BC4];
    u8 us_pad_1C2C[0xC];
    u8 pad1BC4_us[0x1CB0 - 0x1C2C];
    s32 field_1CB0;
    s32 action;
    u8 pad1CB8[0x1D64 - 0x1CB8];
    s32 death_started;
    u8 pad1D68[0x1D6C - 0x1D68];
    s32 field_1D6C;
    u8 pad1D70[0x1D74 - 0x1D70];
    s32 field_1D74;
    s32 rumble_timer;
    u8 pad1D7C[0x1D7D - 0x1D7C];
    u8 slot;
    u8 pad1D7E[0x1DC4 - 0x1D7E];
    s32 kills;
    s32 field_1DC8;
    s32 field_1DCC;
    u8 pad1DD0[0x1F2C - 0x1DD0];
    void *anim_data;
    u8 pad1F30[0x1F8C - 0x1F30];
} ZoidState;
extern ZoidState Zoid[16];
extern void OSReport(const char *fmt, ...);
extern void *OSGetArenaLo(void);
extern void *OSGetArenaHi(void);
extern u32 OSGetPhysicalMemSize(void);
extern s32 OSCreateHeap(void *lo, void *hi);
extern void *OSInitAlloc(void *lo, void *hi, s32 heaps);
extern void OSSetArenaLo(void *lo);
typedef struct GameMode3 {
    s32 f00;
    s32 game_type;
    s32 f08;
    s32 win;
    s32 f14;
    s32 f18;
    s32 map;
    s32 f24;
    s32 f28;
    s32 f2C;
    s32 f30;
    s32 f34;
    s32 f38;
    s32 f3C;
    s32 f40;
    s32 f44;
    s32 f48;
    s32 f4C;
} GameMode3;
extern u8 IveSaveFlag[6800];
extern s32 bitTBL[14];
extern s32 gameTime;
extern s32 suddenDeath(ZoidState *a, ZoidState *b);
typedef struct FixData {
    u32 sum;
    s32 size;
    u8 rest[1];
} FixData;
extern u8 IveSave[61200];
extern void ZeroMemory(void *p, s32 n);
extern void MemCpy(void *d, void *s, s32 n);
extern void camVoiceOn(s32 chara, s32 vc);
extern void zdVoiceSc(ZoidState *z, s32 no);
extern u8 *sPad1;
typedef struct F1 {
    f32 v;
} F1;
typedef struct VIX {
    u8 pad[0x48];
    F1 f48;
} VIX;
extern VIX HSD_VIData;
typedef struct FogG3 { u8 pad[0x18]; F1 color; } FogG3;
extern void debugFileUnPurgeList(void);
extern void ascPrologue(void);
extern void ascEpilogue(void);
extern void *CopyJoint2(void *p, s32 n);
extern void ZoidPrologue(void);
extern void GameZoidInit(void);
extern void sWeaponPrologue(void);
extern void sWeaponEpilogue(void);
extern void zDispCursorPrologue(void);
extern void zDispCursorEpilogue(void);
extern void HSD_StateInvalidate(s32 n);
extern void HSD_FogSet(void *fog);
extern void HSD_SetZSortMode(s32 a, s32 b);
extern void HSD_VISetBlack(s32 n);
extern void ttlMain(void);
extern void loadMapInfo(void);
extern s32 HSD_Randi(s32 n);
extern void *rdMapInfo(s32 a, s32 b);
extern void VIWaitForRetrace(void);
extern void Epilogue(void);
extern void allRank(void);
extern void waitV(s32 n);
extern void resetSkipOp(void);
extern void ZoidEpilogue(void);
extern void jobjUnrefAll(void *p);
extern void sReadPre(void);
extern void chkVS(void);
extern void HSD_AudioStreamStop(s32 n);
extern void HSD_AudioSFXKeyOffAll(void);
extern void TEA_SoundSfxPop(void);
extern void game0(void);
extern void batFlag(void);
extern void *jobjFilter[5];
extern ModelArchive *dspFilter;
extern s32 sCamNN;
extern s32 sCamNN2;
extern FogG3 *fogG;
extern s32 sPadRep;
extern s32 sZoidHG;
extern s32 dspLoadF;
extern u8 TeaSoundSfxLoadNum;
typedef struct MgI3 {
    u8 pad0[0x2aa4];
    s32 kind;
    s32 condition;
    u8 pad2AAC[0x2ad4 - 0x2aac];
    s32 f2AD4;
    u8 pad1[0x307c - 0x2ad8];
    s16 id;
    s16 health;
    s16 kills;
    s16 x;
    s16 z;
    s16 radius;
    s16 type;
    s16 f308A;
    s16 hold_x;
    s16 hold_z;
    s16 hold_radius;
    s8 f3092;
    u8 pad3[0x321c - 0x3093];
    s8 f321C;
} MgI3;
extern MgI3 mgI;
extern MgI3 *mgP;
extern s32 usrHeap;
extern void TeaBgm(s16 no);
extern s32 SeOn(s32 no);
extern s32 calcRank(s32 v);
extern u32 OSGetResetCode(void);
extern void OSSetSaveRegion(s32 lo, s32 hi);
extern void fixSave(void);
extern s32 fixSt;
extern s32 fixEd;
extern void init_alloc(s32 n);
extern void init_2(void);
extern void sFileInit(void);
extern void configClear(void);
extern void configGet(void);
extern s32 fixGet(void);
extern void HSD_Init(void);
extern void sPadInit(void);
extern void sPadAll(void);
extern s32 sFontInit(void);
extern void debug11(void);
extern void HSD_PadInit(s32 n, void *q, s32 a, s32 b);
extern void TEA_SoundInit(void);
extern void TEA_SoundDefLoad(void);
extern void TEA_defstatusload(void);
extern void TEA_mcInit(void);
extern u32 OSGetSoundMode(void);
extern void sndOutputMode(s32 n);
extern void sCameraInit(void);
extern void initPtcl(void);
extern void readPtcl(char *name);
extern void forgetPtcl(void);
extern void TEA_AudioSeqStop(void);
extern void TEA_AudioSeqFree(void);
extern void HSD_GetVersion(s32 *a, s32 *b, s32 *c, s32 *d);
extern u8 GXNtsc480IntDf[0x3c];
extern ModelArchive *menu_saveloadiconp;
extern ModelArchive *menu_yesnop;
extern void mainLoop(void);
extern s32 HSD_SetInitParameter(s32 id, ...);
extern void *memcpy(void *d, const void *s, u32 n);
#include "os_time.h"
typedef struct RM3 {
    u8 b[0x3c];
} RM3;
typedef struct Cam3 {
    u8 pad0[8];
    void *lights;
    void *shadow;
    u8 pad1[0x1c4 - 0x10];
    ZoidState *zoid;
    u8 pad2[0x1f0 - 0x1c8];
    f32 v1f0[3];
    u8 pad3[0x208 - 0x1fc];
    f32 v208[3];
    f32 v214[3];
    u8 pad4[0x244 - 0x220];
    void *cobj;
    u8 pad5[0x2f9 - 0x248];
    s8 f2F9;
    s8 f2FA;
    s8 f2FB;
    u8 pad6[0x590 - 0x2fc];
} Cam3;
extern Cam3 sCamera[4];
extern Cam3 *camTmp;
extern s32 offShadow;
extern void dummyFunc(char *s);
extern void sCameraCtrl(Cam3 *c);
extern void ZoidAI(void);
extern void camOnStart(Cam3 *c);
extern void sCameraON(Cam3 *c);
extern void camOnWin(Cam3 *c);
extern void camOnDie(Cam3 *c);
extern void camOnScope(Cam3 *c);
extern void gameStartZoid(void);
extern s32 isMsgHelp(void);
extern void clearMsgHelpPage(void);
extern s32 isMsgHelpPage(s32 p);
extern void purgeMsgHelp(void);
extern void ZoidLen(void);
extern void ZoidAllAnim(void);
extern void ZoidAllMat(void);
extern void ZoidAllMove(void);
extern void grdObjAnim(void);
extern void grdObjTest(void);
extern void sMissileMove(void);
extern void psInterpretParticles(s32 n);
extern void psExecGenerator(s32 n);
extern void ZoidSearchTop(void);
extern void ZoidAllPutXZ(void);
extern void sWeaponAnimAll(void);
extern void ZoidWeaponActLoop(void);
extern void sLsrMove(void);
extern void ZoidZdCoObjBodyAll(void);
extern void ZoidCoObjAll(void);
extern void ZoidZdAll(void);
extern void sWeaponMoveAll(void);
extern void sEffectAnimAll(void);
extern void ZoidAllSmoke(void);
extern void sCameraShadowAll(void);
extern void sDispAnim(void);
extern s32 gameEnd(void);

typedef struct C4 { u8 r, g, b, a; } C4;
typedef struct Ctl3 { u8 pad[0x140]; s32 w[16]; } Ctl3;
typedef struct SkyS { u8 pad[0x30]; void *lights; u8 pad34[0x3c - 0x34]; HSD_JObj *models[1]; } SkyS;
extern SkyS *Sky;
extern s32 VsMode;
extern s32 VsTime;
extern s32 VsTimeTBL[8];
extern s32 sCamNS2;
extern s32 vsyncN;
extern u8 *sPad2;
extern u8 *sPad3;
extern u8 *sPad4;
extern u8 sPad[4][0x88];
extern s16 TEA_SEVOL;
extern void sMissileEpilogue(void);
extern void sEffectEpilogue(void);
extern s32 HSD_Rand(void);
extern s32 sprintf(char *s, const char *fmt, ...);
extern s32 fexistU(char *path, s32 mode);
extern void putErr(char *s);
extern void LoadStageNow(s32 n);
extern void ZoidInitAll(void);
extern void ZoidCalcAll(void);
extern void ZoidVoiceScript(void);
extern void zdSearchPosPoly(ZoidState *z);
extern void zdOnGround(ZoidState *z);
extern void zdCoStartPos(ZoidState *z);
extern void zdWeaponChg(ZoidState *z);
extern void ZoidAmbientSetSkyAll(void);
extern void aiPrologue(s32 n);
extern void aiEpilogue(s32 n);
extern void sCameraPurge(void);
extern void sCameraSetQuat(Cam3 *c, s32 n);
extern s32 ZoidInit(ZoidState *z, s32 id);
extern void sCameraTargetZoid(Cam3 *c, ZoidState *z);
extern void sDispLoadParts(s32 a, s32 b);
extern void sCameraSetHalf2(Cam3 *c, s32 n);
extern void sCameraSetHalf(Cam3 *c, s32 n);
extern void sCameraSetNormal(Cam3 *c);
extern void sCameraSetShadowAll(void);
extern void ZoidSetAnim(ZoidState *z, s32 n);
extern void zdLinierTm(ZoidState *zoid, s32 frames);
extern f32 zdColPosY(f32 *point, u16 *selector);
extern void zdSetXYZ(ZoidState *zoid);
extern void sMakeMatrix(void *joint);
extern void sEffectPrologue(void);
extern void sMissilePrologue(void);
extern void sLsrPrologue(void);
extern void sLsrEpilogue(void);
extern void IntSePrologue(void);
extern void IntSeClear(void);
extern void IntSeClose(void);
extern u32 VIGetRetraceCount(void);
extern void vcStart(void);
extern s32 vcEnd(char *s);
extern s32 gameWin(void);
extern void motWinSet(void);
extern s32 HSD_GetHeap(void);
extern void *OSAllocFromHeap(s32 heap, u32 size);
extern void OSFreeToHeap(s32 heap, void *p);
extern void HSD_Free(void *p);
extern void drawAll(void);
extern void sPutFontAllNn(void);
extern void HSD_VICopyXFB(s32 n);
extern void HSD_VISetXFBDrawDone(void);
extern void sDispPauseProc(void);
extern void setBpTBL(void);
extern s32 putBP(void);
extern void TEA_VoiceOn(s32 a, s32 b);
extern s32 TEA_SeKeyOn(u16 id, u8 vol, u8 a, u32 b);
extern void sMissilePurgeAll(void);
extern void sDispPurgeParts(void);
extern void sWeaponUnMountAll(ZoidState *z);
extern void ZoidPurgeAll(void);
extern void PurgeStageData(void);
extern void psKillAllGenerator(void);
extern void psKillAllParticle(void);
extern s32 gClear(void);

s32 calcEndVsMan(void);
s32 sortEndVs4p(void);
void game1(void);
s32 putBP(void);
void voiceSet(s32 who);
void setBpTBL(void);
void game0(void);
extern s32 aram_block[16];
extern GameMode3 GameMode;
extern s32 bp4pNm[10];
extern s32 vNowTBL[80];
extern s32 bp[12];
extern ModelArchive *dspLoad;
extern s32 normLineDEBUG;
extern s32 ctrlZoid;
extern s32 AreaIn;
extern s32 AreaIn2;
extern s32 sPage;
extern s32 vNow;
extern s32 vNowO;
extern s32 drawTime;
extern s32 drawTimeG;
extern s32 vOn44;
extern FixData *fix;
extern s32 lastGame;
extern u32 usrLO;
extern s32 gametime;
typedef struct BpTm2 { s32 a; s32 b; } BpTm2;
extern BpTm2 bpTm;
extern s32 pause;
extern s32 gameContinueTm;
extern s32 gameEndTm;
extern s32 gameEndF;
extern s32 gameStartTm;
extern s32 gameStartF;
extern s32 winF;
extern s32 grdcH2TBL;
extern s32 grdcD2TBL;
extern s32 grdcPolygon;
extern s32 grdcMat;
extern s32 grdcNormal;
extern s32 grdcVtx;
extern s32 grdcEndZ;
extern s32 grdcEndX;
extern s32 grdcDiffZ;
extern s32 grdcDiffX;
extern s32 grdcStartZ;
extern s32 grdcStartX;
extern s32 grdcDivZ;
extern s32 grdcDivX;
extern s32 grdcNn;
extern s32 grdcMn;
extern s32 grdcVn;
extern s32 grdcPn;
extern s32 grdcCol;
extern void HSD_PerfInitStat(void);
extern void HSD_PerfSetStartTime(void);
extern void HSD_PerfSetCPUTime(void);
extern void HSD_PerfSetDrawTime(void);
extern void HSD_PerfSetTotalTime(void);
#include "perf.h"
extern void draw1(s32 first);
extern s32 grdObjAlive(s32 index);
extern f64 __frsqrte(f64);
extern s32 nowShadow;
extern void sCameraLenZoid(Cam3 *c);
extern void sCameraMatrix(Cam3 *c);
extern void sDisp(Cam3 *c);
extern void HSD_ShadowSetActive(void *shadow, s32 on);
extern void HSD_StartRender(s32 pass);
extern void HSD_EndRender(void);
extern s32 HSD_CObjSetCurrent(void *cobj);
extern void HSD_CObjEndCurrent(void);
extern void HSD_LObjSetCurrentAll(void *lobj);
extern void HSD_LObjDeleteCurrentAll(void *lobj);
extern void HSD_LObjSetupInit(void *cobj);
extern void HSD_JObjDispAll(HSD_JObj *jobj, void *mtx, s32 trsp_mask, s32 rendermode);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *jobj);
extern void __assert(const char *file, s32 line, const char *expr);
extern void sMissileLight(void);
extern void grdDisp(void);
extern void grdDispXLU(void);
extern void sdZoidAll(void);
extern void ZoidTransMode(void);
extern void ZoidAllDisp3(void);
extern void ZoidAllDisp(void);
extern void ZoidAllDispXLU(void);
extern void ZoidAllDBG(void);
extern void psDispParticles(s32 mask, s32 pass);
extern void sLsrDisp(void);
extern void sWeaponDispAll(void);
extern void sMissileDisp(void);
extern void sEffectDispAll(void);
extern void GXSetZMode(u8 compare, s32 func, u8 update);
extern void putErrInt(void);
extern void sPutFontAllN(void);
void main(void);
#pragma force_active on
s32 aram_block[16];
GameMode3 GameMode;
s32 bp[12];
static u64 str_1070[32];
static u64 str0_1087[32];
static u64 str1_1088[32];
static u64 strS_1089[32];
static u64 strF0_1090[32];
static u64 strF1_1091[32];
static u16 str_1121[25];
static u32 fixt[6820];
static const char lit_371[15] = "tex/1_B_KA.DTX";
namespace { __declspec(section ".rodata") extern const char lit_902[21] = "chara =%d , vc =%d \n"; }
static const u32 lit_1047[7] = {
    0x000001E0, 0x000001E0, 0x00000384, 0x00000384, 0x00000384, 0x000001E0, 0x000001E0, 
};
static const char lit_1308[20] = "#\x83Q\x81[\x83\x80\x83X\x83^\x81[\x83g!! \n";
static const char lit_1309[17] = "stage/st%02d.dat";
static const char lit_1310[21] = "stage/skymap%02d.dat";
static const char lit_1311[20] = "stage not found %d ";
static const char lit_1314[59] = "wp act no set     ---------------------------------------\n";
static const char lit_1315[52] = "game0 ---- start voice----------------------------\n";
static const char lit_1316[62] = "game0 ---- start voice end----------------------------------\n";
static const char lit_1317[51] = "game0 ---- init----------------------------------\n";
static const char lit_1318[60] = "main loop start   ----------------------------------------\n";
static const char lit_1319[13] = "gameEndF=%d\n";
static const char lit_1321[10] = "C %8f d%d";
static const char lit_1322[10] = "D %8f v%d";
static const char lit_1323[34] = "mem Sd%2dM%4dK,Us%2dM%4dK byte(s)";
static const char lit_1324[25] = "gameEndF=9 tm=%d pA=%s \n";
namespace { __declspec(section ".rodata") extern const char lit_1338[26] = "StFixTbl size=%d tbl=%d \n"; }
#pragma force_active off
void batFlag(void)
{
    s32 n;
    s32 idx;
    s32 bit;
    if (GameMode.game_type == 1 && winF == 1) {
        n = GameMode.f24 + GameMode.map * 3;
        idx = n / 8 + 10;
        bit = n % 8;
        (IveSaveFlag + idx)[0x1a2c] |= bitTBL[bit];
        OSReport("clear battle %d=====================flag[%d]=%02X \n", n, idx, (s8)(IveSaveFlag + idx)[0x1a2c]);
    }
    (void)n; (void)n; (void)n; (void)idx; (void)idx;
}
#undef lit_1352
#define lit_1352 "clear battle %d=====================flag[%d]=%02X \n"
namespace { extern const f32 lit_1354[1]; }
void mainLoop(void)
{
    ModelArchive *arc;
    ModelArchive *arc2;
    s32 k;
    F1 b;
    F1 a = *(const F1 *)lit_1354;
    b = a;
    HSD_VIData.f48 = b;
    OSReport("game loop start ================================\n================================\n===========================\n=====================================\n");
    arc = LoadArc2("debug/c_.dat");
    if (arc != 0) {
        PurgeArc2(arc);
    }
    debugFileUnPurgeList();
    TeaBgm(-1);
    ascPrologue();
    dspFilter = LoadArc2("filter/filter.dat");
    jobjFilter[0] = CopyJoint2(dspFilter, 0);
    jobjFilter[1] = CopyJoint2(dspFilter, 0);
    jobjFilter[2] = CopyJoint2(dspFilter, 0);
    ZoidPrologue();
    GameZoidInit();
    sCamNN = 1;
    sCamNN2 = 2;
    sWeaponPrologue();
    zDispCursorPrologue();
    sZoidHG = 1;
    HSD_SetZSortMode(0, 0);
    HSD_StateInvalidate(-1);
    sZoidHG = 1;
    lastGame = 0;
    OSReport("GameMode.gametime=%d\n", GameMode.f30);
    debugFileUnPurgeList();
    if (fogG != 0) {
        HSD_FogSet(0);
    }
    sPadRep = 1;
    ttlMain();
    if (GameMode.game_type == 9) {
        GameMode.f44 = 1;
    } else {
        GameMode.f44 = 0;
    }
    sPadRep = 0;
    debugFileUnPurgeList();
    TeaBgm(-1);
    debugFileUnPurgeList();
    HSD_VISetBlack(0);
    OSReport("type=%d , stg=%d \n", GameMode.game_type, GameMode.map);
    if (GameMode.game_type == 1) {
        loadMapInfo();
    }
    if (GameMode.game_type == 9) {
        for (k = 0; rdMapInfo(2, HSD_Randi(100)) == (void *)0; k++) {
        }
    }
    GameMode.f34 = 0;
    HSD_SetZSortMode(1, 1);
    zDispCursorEpilogue();
    TeaBgm(-1);
    HSD_StateInvalidate(-1);
    sZoidHG = 0;
    if (GameMode.game_type == 4) {
        if (GameMode.map == 100) {
            lastGame = 1;
        }
        OSReport("epilogue?---------------\n");
    }
    OSReport("GameStartMain\n");
    arc2 = LoadArc2("debug/c_.dat");
    if (arc2 != 0) {
        PurgeArc2(arc2);
    }
    debugFileUnPurgeList();
    VIWaitForRetrace();
    VIWaitForRetrace();
    VIWaitForRetrace();
    if (lastGame != 0) {
        Epilogue();
        OSReport("the end of the mission \n");
    } else {
        TeaBgm(mgI.f3092);
        dspLoadF = 1;
        game0();
        batFlag();
    }
    if (GameMode.game_type == 1 && GameMode.map == 11 && winF == 1) {
        Epilogue();
        GameMode.win = 2;
    }
    if (GameMode.game_type == 2 || GameMode.game_type == 3) {
        dspLoadF = 1;
    }
    waitV(3);
    GameMode.f38 = 0;
    if (GameMode.game_type == 9) {
        resetSkipOp();
    }
    if (GameMode.game_type == 9 && GameMode.f44 == 2) {
        GameMode.f38 = 1;
    }
    GameMode.f44 = 0;
    TeaBgm(-1);
    waitV(3);
    sWeaponEpilogue();
    ZoidEpilogue();
    jobjUnrefAll(jobjFilter[2]);
    jobjFilter[2] = 0;
    jobjUnrefAll(jobjFilter[1]);
    jobjFilter[1] = 0;
    jobjUnrefAll(jobjFilter[0]);
    jobjFilter[0] = 0;
    PurgeArc2(dspFilter);
    dspFilter = 0;
    ascEpilogue();
    HSD_VISetBlack(0);
    sReadPre();
    chkVS();
    HSD_AudioStreamStop(0);
    OSReport("stream off \n");
    sReadPre();
    TeaBgm(-1);
    OSReport("audio off \n");
    sReadPre();
    HSD_AudioSFXKeyOffAll();
    OSReport("key off\n");
    waitV(3);
    if (TeaSoundSfxLoadNum > 1) {
        TEA_SoundSfxPop();
    }
    OSReport("--------- main loop end ---------\n");
}
#undef lit_1399
#define lit_1399 "game loop start ================================\n================================\n===========================\n=====================================\n"
#undef lit_1400
#define lit_1400 "debug/c_.dat"
#undef lit_1401
#define lit_1401 "filter/filter.dat"
#undef lit_1402
#define lit_1402 "GameMode.gametime=%d\n"
#undef lit_1403
#define lit_1403 "type=%d , stg=%d \n"
#undef lit_1404
#define lit_1404 "epilogue?---------------\n"
#undef lit_1405
#define lit_1405 "GameStartMain\n"
#undef lit_1406
#define lit_1406 "the end of the mission \n"
#undef lit_1407
#define lit_1407 "stream off \n"
#undef lit_1408
#define lit_1408 "audio off \n"
#undef lit_1409
#define lit_1409 "key off\n"
#undef lit_1410
#define lit_1410 "--------- main loop end ---------\n"
void init_alloc(s32 heaps)
{
    void *lo = OSGetArenaLo();
    void *hi = OSGetArenaHi();
    lo = OSInitAlloc(lo, hi, heaps);
    OSSetArenaLo(lo);
    OSReport("mem arena %dk \n", ((s32)hi - (s32)lo) / 1024);
    usrLO = (u32)lo;
}
#undef lit_1412
#define lit_1412 "mem arena %dk \n"

void init_2(void)
{
    u8 *lo;
    u8 *hi;
    u8 *t;
    lo = (u8 *)(((u32)OSGetArenaLo() + 0x1f) & ~0x1f);
    if ((u32)lo == usrLO) {
        OSReport("memory arena error   lo==usrLO \n");
    }
    hi = (u8 *)OSGetArenaHi();
    if (OSGetPhysicalMemSize() == 0x3000000) {
        t = hi; t -= 0x1800000;
        if (t > lo) hi = t;
    }
    usrHeap = OSCreateHeap(lo, hi);
    if (usrHeap == -1) {
        OSReport("keep alloc arena error usrHeap.....................\n");
    } else {
        OSReport("memomry arena heap ......................%dk\n", (hi - lo) / 1024);
    }
    (void)lo; (void)hi; (void)t; (void)t;
}
#undef lit_1423
#define lit_1423 "memory arena error   lo==usrLO \n"
#undef lit_1424
#define lit_1424 "keep alloc arena error usrHeap.....................\n"
#undef lit_1425
#define lit_1425 "memomry arena heap ......................%dk\n"
#pragma force_active on
static const char lit_1432[70] = "-------------------------------------test---------------------------\n";
#pragma force_active off
namespace { extern const f32 lit_1453[1]; }
extern void putUSA(void);
void main(void)
{
    s32 d9c = 0;
    s32 d98 = 0;
    s32 d94 = 0;
    s32 d90 = 0;
    s32 d8c = 0;
    f32 d88 = 0.0f;
    s32 d84 = 3;
    RM3 rm;
    OSCalendarTime ct;
    s32 ver0, ver1, ver2, ver3;
    F1 b;
    u32 n;
    s32 buffers_result;
    s32 mode_result;
    s32 heap_result;
    static s32 queue[130];
    if (OSGetResetCode() == 0x80000000) {
        OSSetSaveRegion(fixSt, fixEd);
        fixSave();
    }
    init_alloc(2);
    sPadRep = 0;
    memcpy(&rm, GXNtsc480IntDf, 0x3c);
    buffers_result = HSD_SetInitParameter(1, 2);
    (void)buffers_result;
    mode_result = HSD_SetInitParameter(4, &rm);
    (void)mode_result;
    heap_result = HSD_SetInitParameter(5, 0x8c0000);
    (void)heap_result;
    HSD_Init();
    init_2();
    sFileInit();
    configClear();
    if ((u16)OSGetResetCode() == 0x8800) {
        GameMode.f38 = 1;
    }
    if (OSGetResetCode() == 0x80000000) {
        GameMode.f38 = 1;
    }
    if (GameMode.f38 == 1) {
        if (fixGet() == 0) {
            OSReport("fixGet error\n");
        } else {
            OSReport("fixGet OK \n");
            configGet();
        }
        GameMode.f38 = 1;
        GameMode.game_type = 0;
    }
    OSReport("resetcode=%X \n", OSGetResetCode());
    GameMode.f48 = GameMode.f48 + 1;
    n = GameMode.f48;
    n &= 0xffff;
    goto c1;
b1:
    n -= 100000;
c1:
    if (n > 100000) goto b1;
    goto c2;
b2:
    n--;
    HSD_Randi(10);
c2:
    if (n != 0) goto b2;
    F1 a = *(const F1 *)lit_1453;
    b = a;
    HSD_VIData.f48 = b;
    fogG = 0;
    HSD_FogSet(0);
    if (OSGetPhysicalMemSize() == 0x3000000) {
        OSReport("dev mem alloc \n");
    }
    HSD_GetVersion(&ver0, &ver1, &ver2, &ver3);
    OSReport("ver %d %d %d %d \n", ver0, ver1, ver2, ver3);
    OSTicksToCalendarTime(OSGetTime(), &ct);
    OSReport("NowTime %d/%d/%d %02d:%02d:%02d \n", ct.year, ct.mon, ct.mday, ct.hour, ct.min, ct.sec);
    debug11();
    HSD_PadInit(10, queue, 0, 0);
    sPadInit();
    VIWaitForRetrace();
    sPadAll();
    VIWaitForRetrace();
    sPadAll();
    HSD_VISetBlack(0);
    VIWaitForRetrace();
    sPadAll();
    VIWaitForRetrace();
    sPadAll();
    sFontInit();
    OSReport("--------------test-----------ok--------\n");
    if (GameMode.f38 == 0)
        putUSA();
    debug11();
    debug11();
    debug11();
    dspLoad = LoadArc2("menu/loading/loading.dat");
    debug11();
    debug11();
    debug11();
    dspLoadF = 1;
    OSReport("load ok=================\n");
    sReadPre();
    debug11();
    TEA_SoundInit();
    OSReport("\n sound init \n");
    debug11();
    sReadPre();
    TEA_SoundDefLoad();
    debug11();
    OSReport("\n end sound def load  \n");
    debug11();
    configGet();
    sReadPre();
    TEA_defstatusload();
    TEA_mcInit();
    if (OSGetSoundMode() == 0) {
        OSReport("mono !!--------------------------\n");
        sndOutputMode(0);
    }
    n = 0;
    if (n != 0) {
        sFontInit();
        dspLoad = LoadArc2("menu/loading/loading.dat");
        TEA_SoundDefLoad();
    }
    sCameraInit();
    menu_saveloadiconp = LoadArc2("menu/saveload/mc/icon.dat");
    OSReport("icon ok\n");
    menu_yesnop = LoadArc2("menu/caution/caution.dat");
    dspLoadF = 0;
    while (1) {
        initPtcl();
        readPtcl("ptcl/Zoid_eff");
        mainLoop();
        waitV(3);
        forgetPtcl();
        OSReport("forget partcle \n");
        HSD_AudioSFXKeyOffAll();
        TeaBgm(-1);
        waitV(3);
        TEA_AudioSeqStop();
        TEA_AudioSeqFree();
        if (TeaSoundSfxLoadNum > 1) {
            TEA_SoundSfxPop();
        }
        if (GameMode.game_type == 1 && GameMode.win == 0) {
            GameMode.f38 = 2;
        }
    }
}
#undef lit_1482
#define lit_1482 "fixGet error\n"
#undef lit_1483
#define lit_1483 "fixGet OK \n"
#undef lit_1484
#define lit_1484 "resetcode=%X \n"
#undef lit_1485
#define lit_1485 "dev mem alloc \n"
#undef lit_1486
#define lit_1486 "ver %d %d %d %d \n"
#undef lit_1487
#define lit_1487 "NowTime %d/%d/%d %02d:%02d:%02d \n"
#undef lit_1488
#define lit_1488 "--------------test-----------ok--------\n"
#undef lit_1489
#define lit_1489 "menu/loading/loading.dat"
#undef lit_1490
#define lit_1490 "load ok=================\n"
#undef lit_1491
#define lit_1491 "\n sound init \n"
#undef lit_1492
#define lit_1492 "\n end sound def load  \n"
#undef lit_1493
#define lit_1493 "mono !!--------------------------\n"
#undef lit_1494
#define lit_1494 "menu/saveload/mc/icon.dat"
#undef lit_1495
#define lit_1495 "icon ok\n"
#undef lit_1496
#define lit_1496 "menu/caution/caution.dat"
#undef lit_1497
#define lit_1497 "ptcl/Zoid_eff"
#undef lit_1498
#define lit_1498 "forget partcle \n"
s32 calcEndBattle(void)
{
    ZoidState *first = &Zoid[0];
    s32 player_slot = first->slot;
    s32 i;
    s32 opponent_alive = 0;

    for (i = 0; i < 16; i++) {
        ZoidState *zoid = &Zoid[i];
        if (zoid->joint != 0 && zoid->slot != player_slot) {
            if (zoid->death_started == 0) {
                opponent_alive = 1;
            }
        }
    }
    if (first->death_started == 0) {
        if (opponent_alive != 0) {
            return 0;
        }
        return 1;
    }
    if (opponent_alive != 0) {
        return 3;
    }
    return 2;
}
f32 calcPer(ZoidState *zoid)
{
    f32 max_health = zoid->max_health;
    f32 health = zoid->health;

    if (0.0f == health) {
        return 0.0f;
    }
    if (0.0f == max_health) {
        return 100.0f;
    }
    return (100.0f * health) / max_health;
}

s32 suddenDeath(ZoidState *a, ZoidState *b)
{
    f32 per_a;
    f32 per_b;

    if (gameTime != 0) {
        return 0;
    }
    per_a = calcPer(a);
    per_b = calcPer(b);
    if (per_a == per_b) {
        return 2;
    }
    if (per_a > per_b) {
        return 1;
    }
    return 3;
}

s32 calcEndVsMan(void)
{
    s32 flags = 0;
    ZoidState *z0 = &Zoid[0];
    ZoidState *z1 = &Zoid[1];
    if (GameMode.f3C == -1) {
        if (z0->death_started == 0 && z0->field_1D74 != 0) {
            flags |= 1;
        }
        if (z1->death_started == 0 && z1->field_1D74 != 0) {
            flags |= 2;
        }
        if (flags != 0) {
            if (flags & 1) {
                GameMode.f3C = 0;
            }
            if (flags & 2) {
                GameMode.f3C = 1;
            }
            if (flags == 3) {
                GameMode.f3C = 99;
            }
        }
    }
    if (GameMode.f3C != -1) {
        if (GameMode.f3C == 0) {
            return 1;
        }
        if (GameMode.f3C == 1) {
            return 3;
        }
        return 2;
    }
    if (z0->death_started == 0) {
        if (z1->death_started != 0) {
            return 1;
        }
    } else {
        if (z1->death_started == 0) {
            return 3;
        }
        return 2;
    }
    return suddenDeath(z0, z1);
}
#pragma dont_inline off
static inline f32 sqrtf(f32 x)
{
    volatile f32 y;

    if (x > 0.0f) {
        f64 guess = __frsqrte((f64)x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        y = (f32)(x * guess);
        return y;
    }
    return x;
}

s32 calcEndMission(void)
{
    ZoidState *other;
    s32 i;
    s32 slot;
    s32 alive;
    ZoidState *zoid;
    ZoidState *partner;
    ZoidState *first;
    f32 dx;
    f32 dz;
    f32 dist;

    first = &Zoid[0];
    zoid = first;
    if ((partner = &Zoid[2])->joint == 0)
        partner = 0;
    slot = first->slot;
    if (first->death_started != 0)
        return 3;
    switch (mgI.condition) {
    case 3:
        if (partner != 0 && partner->slot == slot && partner->death_started != 0)
            return 3;
        break;
    case 4:
        for (i = 1; i < 16; i++) {
            other = &Zoid[i];
            if (other->joint != 0 && other->slot == slot && other->type == mgI.type && other->death_started != 0)
                return 3;
        }
    case 5:
        for (i = 1; i < 16; i++) {
            other = &Zoid[i];
            if (other->joint != 0 && other->slot != slot && other->type == mgI.type)
                break;
        }
        if (i == 16)
            break;
        dx = other->position[0] - mgI.hold_x;
        dz = other->position[2] - mgI.hold_z;
        dist = sqrtf(dx * dx + dz * dz);
        if (dist < mgI.hold_radius)
            AreaIn2 = 1;
        if (AreaIn2 != 0)
            return 3;
        break;
    }
    switch (mgI.kind) {
    case 0:
        for (i = 0, alive = 0; i < 16; i++) {
            other = &Zoid[i];
            if (other->joint != 0 && other->slot != slot && other->death_started == 0)
                alive++;
        }
        if (alive == 0)
            return 1;
        break;
    case 1:
        if (grdObjAlive(mgI.id) == 0)
            return 1;
        break;
    case 2:
        for (i = 1, alive = 0; i < 16; i++) {
            other = &Zoid[i];
            if (other->joint != 0 && other->slot != slot && other->type == mgI.id) {
                alive++;
                if (other->health <= mgI.health)
                    return 1;
            }
        }
        break;
    case 7:
        for (i = 1; i < 16; i++) {
            zoid = &Zoid[i];
            if (zoid->joint != 0 && zoid->slot == slot && zoid->type == mgI.id)
                break;
        }
        if (i == 16)
            break;
    case 3:
        dx = zoid->position[0] - mgI.x;
        dz = zoid->position[2] - mgI.z;
        dist = sqrtf(dx * dx + dz * dz);
        if (dist < mgI.radius)
            AreaIn = 1;
        if (AreaIn != 0)
            return 1;
        break;
    case 4:
        if (first->kills >= mgI.kills)
            return 1;
        break;
    case 5:
        if (gameTime == 0)
            return 1;
        break;
    case 6:
        for (i = 1, alive = 0; i < 16; i++) {
            other = &Zoid[i];
            if (other->joint != 0 && other->slot != slot && other->type == mgI.id && other->death_started == 0)
                alive++;
        }
        if (alive == 0)
            return 1;
        break;
    }
    return 0;
}
#pragma dont_inline on
s32 calcEnd(void)
{
    switch (GameMode.game_type) {
    case 1:
        return calcEndBattle();
    case 2:
        return calcEndVsMan();
    case 3:
        return calcEndVsMan();
    case 4:
        return calcEndMission();
    default:
        return calcEndBattle();
    }
}

s32 gameEnd(void)
{
    s32 result;

    result = calcEnd();
    if (gameStartF == 0) {
        if (gameTime > 0) {
            gameTime -= 1;
        }
    }
    if (gameTime == 0) {
        return 1;
    }
    if (result != 0) {
        return 1;
    }
    return 0;
}
s32 gameWin(void)
{
    s32 result = 0;

    result = calcEnd();
    if (result == 0) {
        return 2;
    }
    return result;
}
void game1(void)
{
    s32 i;
    ZoidState *z;
    ZoidState *zoid;
    camTmp = sCamera;
    if (gameEndF >= 4) {
        for (i = 0; i < 16; i++) {
            z = &Zoid[i];
            if (z->joint != (void *)0) {
                z->rumble_timer = 0;
            }
        }
    }
    if (gameEndF <= 3 && gameStartF == 0) {
        for (i = 0; i < 4; i++) {
            sCameraCtrl(&sCamera[i]);
        }
        ZoidAI();
    }
    if (gameEndF <= 5) {
        for (i = 0; i < 4; i++) {
            camTmp = &sCamera[i];
            if (gameStartF == 0) {
                if (camTmp->f2F9) {
                    camOnWin(camTmp);
                } else if (camTmp->f2FA) {
                    camOnScope(camTmp);
                } else {
                    sCameraON(camTmp);
                }
            } else {
                camOnStart(camTmp);
            }
        }
    }
    if (gameEndF <= 3) {
        switch (gameStartF) {
        case 0:
            break;
        case 1:
            gameStartTm++;
            if (gameStartTm == 0x3c) {
                zdVoiceSc(Zoid, 2);
            }
            if (gameStartTm > 0x12c) {
                gameStartTm = 0;
                gameStartF = 0;
                for (i = 0; i < 4; i++) {
                    if (sCamera[i].cobj != 0) {
                        zoid = sCamera[i].zoid;
                        if (zoid != 0 && zoid->joint != 0) {
                            zdLinierTm(zoid, 0x78);
                            ZoidSetAnim(zoid, 1);
                            zdLinierTm(zoid, 0x78);
                            zoid->action = 0;
                            zoid->field_1CB0 = 0;
                        }
                    }
                }
            }
            break;
        }
        camTmp = sCamera;
        ZoidLen();
        ZoidAllAnim();
        ZoidAllMat();
        ZoidAllMove();
        grdObjAnim();
        grdObjTest();
        sMissileMove();
        psInterpretParticles(0);
        psExecGenerator(0);
        ZoidSearchTop();
        ZoidAllPutXZ();
        ZoidAllMat();
        sWeaponAnimAll();
        ZoidWeaponActLoop();
        sLsrMove();
        ZoidZdCoObjBodyAll();
        ZoidCoObjAll();
        ZoidZdAll();
        sWeaponMoveAll();
        sEffectAnimAll();
        ZoidAllSmoke();
        sCameraShadowAll();
        sDispAnim();
        if (gameEnd() != 0 && gameEndF == 0) {
            gameEndF = 1;
        }
        if (gameEndF != 0) {
            if ((u32)gameEndTm == 1) {
                if (GameMode.game_type == 1 || GameMode.game_type == 2 || GameMode.game_type == 3) {
                    SeOn(0x74);
                }
            }
            gameEndTm++;
            if ((u32)gameEndTm > 0xb4) {
                if (gameEndF == 1) {
                    gameEndF = 2;
                    TeaBgm(-1);
                }
            } else if (Zoid[0].field_1D6C != 0) {
                gameEndTm = 0x12c;
            }
        } else {
            gameEndTm = 0;
        }
    }
}
#define JOBJ_MTX_INDEP_SRT 0x02000000
#define JOBJ_USER_DEF_MTX 0x800000
#define JOBJ_MTX_DIRTY 0x40
#pragma dont_inline off
static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 605, "jobj");
    return !(jobj->flags & JOBJ_USER_DEF_MTX) && (jobj->flags & JOBJ_MTX_DIRTY);
}

static inline void HSD_JObjSetMtxDirty(HSD_JObj *jobj)
{
    if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj))
        HSD_JObjSetMtxDirtySub(jobj);
}

static inline void HSD_JObjSetTranslateX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 952, "jobj");
    jobj->translation.x = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjSetTranslateY(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 966, "jobj");
    jobj->translation.y = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjSetTranslateZ(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 980, "jobj");
    jobj->translation.z = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

void draw1(s32 first_camera)
{
    s32 flag;

    if (camTmp->cobj == (void *)0) {
        return;
    }
    sCameraLenZoid(camTmp);
    HSD_SetZSortMode(1, 1);
    if (nowShadow == 0) {
        HSD_ShadowSetActive(camTmp->shadow, 1);
    }
    HSD_StartRender(first_camera ? 3 : 0);
    if (HSD_CObjSetCurrent(camTmp->cobj)) {
        flag = 1;
        HSD_LObjSetCurrentAll(camTmp->lights);
        HSD_LObjSetupInit(camTmp->cobj);
        sCameraMatrix(camTmp);
        HSD_FogSet(0);
        HSD_JObjSetTranslateX(Sky->models[0], camTmp->v1f0[0]);
        HSD_JObjSetTranslateY(Sky->models[0], camTmp->v1f0[1]);
        HSD_JObjSetTranslateZ(Sky->models[0], camTmp->v1f0[2]);
        HSD_JObjDispAll(Sky->models[0], 0, HSD_TRSP_ALL, 0);
        if (fogG) {
            HSD_FogSet(fogG);
        }
        if (flag != 0 && GameMode.f08 == 0) {
            HSD_LObjDeleteCurrentAll(camTmp->lights);
            HSD_LObjSetCurrentAll(camTmp->lights);
            sMissileLight();
            HSD_LObjSetupInit(camTmp->cobj);
        }
        grdDisp();
        sdZoidAll();
        ZoidTransMode();
        if (fogG) {
            HSD_FogSet(0);
        }
        ZoidAllDisp3();
        if (fogG) {
            HSD_FogSet(fogG);
        }
        ZoidAllDisp();
        psDispParticles(0xFFFF, 0);
        if (fogG) {
            HSD_FogSet(0);
        }
        if (fogG) {
            HSD_FogSet(fogG);
        }
        sLsrDisp();
        if (fogG) {
            HSD_FogSet(0);
        }
        if (fogG) {
            HSD_FogSet(fogG);
        }
        if (fogG) {
            HSD_FogSet(fogG);
        }
        grdDispXLU();
        ZoidAllDispXLU();
        if (fogG) {
            HSD_FogSet(0);
        }
        if (fogG) {
            HSD_FogSet(0);
        }
        if (normLineDEBUG) {
            ZoidAllDBG();
        }
        if (fogG) {
            HSD_FogSet(fogG);
        }
        HSD_CObjEndCurrent();
        if (fogG) {
            HSD_FogSet(0);
        }
        HSD_LObjDeleteCurrentAll(camTmp->lights);
        if (fogG) {
            HSD_FogSet(0);
        }
    }
    HSD_SetZSortMode(0, 0);
    if (fogG) {
        HSD_FogSet(0);
    }
    if (HSD_CObjSetCurrent(camTmp->cobj)) {
        HSD_LObjSetCurrentAll(camTmp->lights);
        HSD_LObjSetupInit(camTmp->cobj);
        if (fogG) {
            HSD_FogSet(fogG);
        }
        sWeaponDispAll();
        if (fogG) {
            HSD_FogSet(0);
        }
        HSD_CObjEndCurrent();
        HSD_LObjDeleteCurrentAll(camTmp->lights);
    }
    HSD_SetZSortMode(1, 1);
    if (fogG) {
        HSD_FogSet(0);
    }
    if (HSD_CObjSetCurrent(camTmp->cobj)) {
        HSD_LObjSetCurrentAll(camTmp->lights);
        HSD_LObjSetupInit(camTmp->cobj);
        if (fogG) {
            HSD_FogSet(0);
        }
        sMissileDisp();
        sEffectDispAll();
        psDispParticles(0xFFFF, 1);
        psDispParticles(0xFFFF, 2);
        HSD_CObjEndCurrent();
    }
    if (fogG) {
        HSD_FogSet(0);
    }
    HSD_EndRender();
    HSD_StateInvalidate(-1);
    HSD_StartRender(first_camera ? 3 : 0);
    if (fogG) {
        HSD_FogSet(0);
    }
    GXSetZMode(1, 7, 0);
    sDisp(camTmp);
    HSD_StateInvalidate(-1);
    if (fogG) {
        HSD_FogSet(0);
    }
    putErrInt();
    sPutFontAllN();
    HSD_StateInvalidate(-1);
    HSD_EndRender();
    HSD_ShadowSetActive(camTmp->shadow, 0);
}
#pragma dont_inline on
#undef lit_819
#define lit_819 "jobj.h"
#undef lit_820
#define lit_820 "jobj"
s32 putBP(void)
{
    s32 calcRank_v;

    ZoidState *z2;
    s32 fin;
    s32 cnt;
    s32 i;
    s32 j;
    s32 s1;
    ZoidState *z0;
    s32 s2;
    s32 rank;
    z0 = Zoid;
    z2 = &Zoid[2];
    if (z2->joint == 0) {
        z2 = 0;
    }
    if (z2 != 0 && z2->max_health == 0) {
        z2 = 0;
    }
    fin = 0;
    if (bpTm.a == 0) {
        return 0;
    }
    if (bpTm.a < 0x3c) {
        bpTm.a = bpTm.a + 1;
        GameMode.f40 = 0;
        bpTm.b = 0;
    } else {
        i = 0;
        cnt = 0;
        for (; i < 5; i++) {
            for (j = 0; j < 5; j++) {
                if ((bp + 6)[i] < bp[i]) {
                    (bp + 6)[i]++;
                    if (*(s8 *)(sPad1 + 0x42) != 0) {
                        (bp + 6)[i] = bp[i];
                    }
                    cnt++;
                }
            }
        }
        if (cnt == 0) {
            fin = 1;
        }
    }
    if (fin != 0) {
        bpTm.b++;
        if (bpTm.b > 0x3c && GameMode.f40 == 0) {
            if (z0->max_health <= 0) {
                s1 = 100;
            } else {
                s1 = z0->health * 100 / z0->max_health;
            }
            if (z2 != 0) {
                s2 = z2->health * 100 / z2->max_health;
            } else {
                s2 = 100;
            }
            calcRank_v = (s32)((s1 + s2) / 2.0);
            rank = 5;
            if (calcRank_v >= 40) {
                rank = 4;
            }
            if (calcRank_v >= 60) {
                rank = 3;
            }
            if (calcRank_v >= 80) {
                rank = 2;
            }
            if (calcRank_v >= 90) {
                rank = 1;
            }
            GameMode.f40 = rank;
        }
        if (bpTm.b > 0x12c) {
            return 1;
        }
    }
    return 0;
}
namespace { extern const char lit_902[21]; }
void voiceSet(s32 who)
{
    s32 vc;
    s32 i;
    ZoidState *z;
    if (vOn44 == 0) {
        if (GameMode.game_type == 4) {
            TeaBgm(who == 1 ? 9 : 10);
            if ((s32)IveSave[0x1a6e] == 1) {
                i = 42;
            } else {
                i = 41;
            }
            if (who <= 1) {
                vc = 83;
            } else {
                vc = 84;
            }
            camVoiceOn(i, vc);
            OSReport(lit_902, i, vc);
        }
        if (GameMode.game_type == 1 || GameMode.game_type == 3) {
            TeaBgm(who == 1 ? 9 : 10);
            if (who == 1) {
                zdVoiceSc(&Zoid[0], 7);
            } else {
                for (i = 1; i < 16; i++) {
                    z = &Zoid[i];
                    if (z->joint == 0 || (u8)z->slot == Zoid[0].slot) {
                        continue;
                    }
                    break;
                }
                if (i != 16) {
                    zdVoiceSc(&Zoid[i], 7);
                }
            }
        }
        if (GameMode.game_type == 2) {
            TeaBgm(9);
        }
        vOn44 = 1;
    }
}
void zdOnGround(ZoidState *zoid)
{
    s32 i;
    f32 v[3];
    if (zoid->joint != 0) {
        zoid->position[1] = 100.0f + zdColPosY(zoid->position, 0);
        zoid->position[1] = 1.0f + zdColPosY(zoid->position, 0);
        for (i = 0; i < 3; i++) {
            v[i] = zoid->position[i];
        }
        zoid->position[1] = 100.0f;
        v[1] = v[1] - 100.0f;
        zoid->position[1] = 0.1 + zdColPosY(zoid->position, 0);
        zdSetXYZ(zoid);
        sMakeMatrix(zoid->joint);
    }
}
typedef struct Tbl7 { s32 v[7]; } Tbl7;
#pragma dont_inline off
static inline void setClear(F1 color)
{
    HSD_VIData.f48 = color;
}
#define clearBlack() setClear((F1){0.0f})
void game0(void)
{
    u32 frameCnt;
    s32 drawCnt;
    s32 skip;
    s32 drawn;
    u32 last;
    s32 waitCnt;
    u32 tick0;
    u32 prev;
    u32 tick;
    s32 drawFps;
    s32 waitFps;
    s32 stage;
    s32 bpcnt;
    s32 skip0;
    s32 prevbp;
    s32 cnt5;
    char path[80];
    s32 ok;
    s32 k;
    s32 t;
    Ctl3 *ctrl ; struct ZoidState *zoid;
    s32 f;
    s32 cnt;
    Ctl3 *ctrl2;
    s32 vt;
    s32 d84;
    s32 d80;
    s32 i;
    s32 j ; struct ZoidState *z;
    Cam3 *cam;
    ZoidState *cz;
    s32 idx;
    s32 i2;
    static s32 bak[1] = {-1};

    skip0 = 0;
    skip = 0;
    drawn = 0;
    gameEndTm = 0;
    gameEndF = 0;
    sMissileEpilogue();
    sEffectEpilogue();
    OSReport(lit_1308);
    vOn44 = 0;
    stage = HSD_Rand() % 17 + 1;
    if (GameMode.game_type == 1) {
        stage = mgI.f2AD4;
        GameMode.f28 = stage;
    }
    if (GameMode.game_type == 4) {
        stage = mgI.f2AD4;
        GameMode.f28 = stage;
    }
    stage = mgI.f2AD4;
    GameMode.f28 = stage;
    stage = GameMode.f28;
    ok = 1;
    sprintf(path, lit_1309, stage);
    if (fexistU(path, 0) == 0) {
        ok = 0;
    }
    sprintf(path, lit_1310, stage);
    if (fexistU(path, 0) == 0) {
        ok = 0;
    }
    if (ok == 0) {
        sprintf(path, lit_1311, stage);
        putErr(path);
        OSReport("%s \n", path);
        stage = 3;
    }
    LoadStageNow(stage);
    if (fogG != 0) {
        setClear(fogG->color);
    }
    ZoidInitAll();
    ZoidCalcAll();
    ZoidVoiceScript();
    for (i = 1; i < 16; i++) {
        Zoid[i].slot = i & 1;
    }
    for (i = 0; i < 16; i++) {
        j = 0;
        z = &Zoid[i];
        if (z->joint != (void *)0) {
            ctrl = (Ctl3 *)z->anim_data;
            zdSearchPosPoly(z);
            if (0.0f == z->heading) {
                z->heading = 0.0349065848f;
            }
            zdOnGround(z);
            zdCoStartPos(z);
            zdCoStartPos(z);
            for (k = 0; k < 9; k++) {
                zdWeaponChg(z);
                if (z->selected_weapon_mount == -1) {
                    break;
                }
                t = ctrl->w[z->selected_weapon_mount];
                if (t == 2 || t == 2) {
                } else {
                    break;
                }
            }
        }
    }
    ZoidAmbientSetSkyAll();
    for (i = 0; i < 16; i++) {
        aiPrologue(i);
    }
    sCameraPurge();
    if (GameMode.game_type == 2) {
        sCamNN = 2;
    } else {
        sCamNN = 1;
    }
    if (sCamNN == 2) {
        sCamNS2 = VsMode == 0;
        if (sCamNS2 != 0) {
            sCameraSetHalf2(&sCamera[0], 1);
            sCameraSetHalf2(&sCamera[1], 0);
        } else {
            sCameraSetHalf(&sCamera[0], 1);
            sCameraSetHalf(&sCamera[1], 0);
        }
        sCamera[0].lights = Sky->lights;
        sCamera[1].lights = Sky->lights;
        sCameraTargetZoid(&sCamera[0], &Zoid[0]);
        sCameraTargetZoid(&sCamera[1], &Zoid[1]);
        sDispLoadParts(2, 0);
    } else {
        sCameraSetNormal(&sCamera[0]);
        sCamera[0].lights = Sky->lights;
        sCameraTargetZoid(&sCamera[0], &Zoid[0]);
        sDispLoadParts(1, 0);
    }
    for (i = 0; i < 4; i++) {
        cam = &sCamera[i];
        if (cam->cobj != (void *)0 && (cz = cam->zoid) != (void *)0) {
            for (j = 0; j < 3; j++) {
                cam->v1f0[j] = 100.0f + cz->position[j];
                cam->v208[j] = 100.0f + cz->position[j];
                cam->v214[j] = 100.0f + cz->position[j];
            }
        }
    }
    gameStartF = 1;
    if (GameMode.game_type == 9) {
        gameStartF = 0;
    }
    gameStartTm = 0;
    sCameraSetShadowAll();
    HSD_VISetBlack(0);
    ZoidSetAnim(&Zoid[0], 1);
    ZoidSetAnim(&Zoid[1], 1);
    ZoidSetAnim(&Zoid[2], 1);
    ZoidSetAnim(&Zoid[3], 1);
    ZoidSetAnim(&Zoid[4], 1);
    ZoidSetAnim(&Zoid[5], 1);
    ZoidSetAnim(&Zoid[6], 1);
    ZoidSetAnim(&Zoid[7], 1);
    sEffectPrologue();
    sMissilePrologue();
    sLsrPrologue();
    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint != 0) {
            ctrl2 = (Ctl3 *)zoid->anim_data;
            zoid->weapon_mount = -1;
            idx = 9;
            for (cnt = 0; cnt < 10; cnt++, idx--) {
                if (idx < 0) {
                    idx = 9;
                }
                if (zoid->weapon_mounts[idx].active != 0) {
                    if (ctrl2 == 0) {
                        break;
                    }
                    f = ctrl2->w[idx];
                    if (f != 2) {
                        break;
                    }
                }
            }
            zoid->selected_weapon_mount = idx;
        }
    }
    OSReport(lit_1314);
    tick0 = VIGetRetraceCount();
    prev = tick0;
    tick = tick0;
    winF = 0;
    GameMode.f3C = -1;
    frameCnt = 0;
    drawCnt = 0;
    OSReport(lit_1315);
    camVoiceOn(Zoid[0].person, 2);
    OSReport(lit_1316);
    gameTime = 36180;
    if (GameMode.game_type == 2 || GameMode.game_type == 3) {
        vt = VsTimeTBL[VsTime];
        if (vt == -1) {
            gameTime = -1;
        } else {
            gameTime = vt * 60 * 60 + 59;
        }
    }
    if (GameMode.game_type == 4 || GameMode.game_type == 1) {
        gameTime = mgI.f308A * 60;
        if (gameTime == 0) {
            gameTime = 36180;
        } else {
            gameTime = gameTime + 59;
        }
    }
    if (GameMode.game_type == 9) {
        gameTime = -1;
    }
    OSReport(lit_1317);
    GameMode.win = 0;
    gameEndF = 0;
    GameMode.f30 = 0;
    gametime = 0;
    bpTm.a = 0;
    bpTm.b = 0;
    AreaIn = 0;
    AreaIn2 = 0;
    skip0 = 0;
    bpcnt = 0;
    cnt5 = 0;
    prevbp = 0;
    gameContinueTm = 660;
    IntSePrologue();
    dspLoadF = 0;
    OSReport(lit_1318);
    while (1) {
        if (GameMode.f44 == 2) {
            break;
        }
        if (GameMode.f44 == 1 && (gametime > 3600 || gameEndF != 0)) {
            break;
        }
        HSD_PerfInitStat();
        HSD_PerfSetStartTime();
        vsyncN++;
        sPadAll();
        camTmp = sCamera;
        game1();
        if (bak[0] != gameEndF) {
            OSReport(lit_1319, gameEndF);
        }
        bak[0] = gameEndF;
        if (gameEndF == 0 && gameStartF == 0) {
            gametime++;
        } else {
            Zoid[0].rumble_timer = 0;
            Zoid[1].rumble_timer = 0;
            *(u8 *)(sPad1 + 0x84) = 0;
            *(u8 *)(sPad2 + 0x84) = 0;
        }
        j = ~ ~ bak[8];
        i2 = ~ ~ j;
        j = ~gameEndF;
        i = j + i2;
        if (gameEndF == 5) {
            Tbl7 tbl = *(const Tbl7 *)lit_1047;
            cnt5++;
            if (cnt5 > tbl.v[GameMode.game_type]) {
                gameEndF = 6;
            }
            if (GameMode.game_type == 2 || GameMode.game_type == 3 || GameMode.game_type == 4) {
                if (cnt5 > 120) {
                    j = 0;
                    if (*(s8 *)(sPad1 + 0x5d) != 0) {
                        gameEndF = 6;
                    }
                    if (GameMode.game_type == 2) {
                        if (*(s8 *)(sPad2 + 0x5d) != 0) {
                            gameEndF = 6;
                        }
                    }
                }
            }
        }
        if (gameEndF == 4) {
            winF = gameWin();
            gameEndF = 5;
            voiceSet(winF);
            if (winF == 1) {
                Zoid[0].field_1CB0 = 50;
                sCamera[0].f2F9 = 1;
            }
            if (winF == 3 && GameMode.game_type == 2) {
                Zoid[1].field_1CB0 = 50;
                sCamera[1].f2F9 = 1;
            }
        }
        HSD_PerfSetCPUTime();
        camTmp = sCamera;
        prev = tick;
        tick = VIGetRetraceCount();
        if (normLineDEBUG != 0) {
            s32 d;
            s32 e;
            C4 c1 = {0, 255, 0, 255};
            C4 c2 = {100, 255, 100, 200};
            C4 c3 = {255, 100, 100, 255};
            d = tick - prev;
            if (d < 0) {
                d = 0;
            }
            if (d > 10) {
                d = 10;
            }
            e = d * 4;
            if (e > 20) {
                e = 20;
            }
            for (i = 0; i < e; i++) {
                ((char *)str_1070)[i] = ' ';
            }
            ((char *)str_1070)[e] = 0;
        }
        if (normLineDEBUG != 0) {
            s32 n;
            s32 m;
            s32 nm;
            s32 a0 = 0;
            s32 a1 = -220;
            s32 a2 = -600;
            C4 e1 = {0, 255, 0, 255};
            C4 e2 = {200, 255, 100, 100};
            C4 e3 = {255, 200, 100, 100};
            C4 e4 = {255, 0, 0, 100};
            C4 e5 = {0, 0, 0, 0};
            C4 e6 = {0, 0, 0, 255};
            f32 y;
            f32 x;
            f32 scale;
            scale = 20.0f;
            y = HSD_PerfLastStat.draw_time;
            x = HSD_PerfLastStat.cpu_time;
            n = x * scale;
            m = y * scale;
            if (n > 20) {
                n = 20;
            }
            if (m > 20) {
                m = 20;
            }
            nm = n + m;
            if (nm > 20) {
                nm = 20;
            }
            for (i2 = 0; i2 < n; i2++) {
                ((char *)str0_1087)[i2] = ' ';
            }
            ((char *)str0_1087)[i2] = 0;
            for (i2 = 0; i2 < m; i2++) {
                ((char *)str1_1088)[i2] = ' ';
            }
            ((char *)str1_1088)[i2] = 0;
            for (i2 = 0; i2 < (s32)scale; i2++) {
                ((char *)strS_1089)[i2] = ' ';
            }
            ((char *)strS_1089)[i2] = 0;
            sprintf((char *)strF0_1090, lit_1321, x, drawFps);
            sprintf((char *)strF1_1091, lit_1322, y, waitFps);
        }
        {
            void *ptrs[1024];
            C4 c1 = {200, 255, 200, 255};
            C4 c2 = {0, 0, 0, 50};
            C4 c3 = {0, 0, 255, 255};
            C4 c4 = {255, 0, 0, 100};
            s32 m1;
            s32 m2;
            s32 flag;
            s32 lim;
            s32 p;
            flag = 0;
            lim = 1024;
            for (p = 0; p < lim; p++) {
                ptrs[p] = 0;
            }
            for (p = 0; p < lim; p++) {
                ptrs[p] = OSAllocFromHeap(HSD_GetHeap(), 0x10000);
                if (ptrs[p] == (void *)0) {
                    break;
                }
            }
            m1 = p << 6;
            for (p = 0; p < lim; p++) {
                if (ptrs[p] != 0) {
                    HSD_Free(ptrs[p]);
                }
            }
            if (m1 >= 0x6000) {
                m1 -= 0x6000;
            }
            if (m1 <= 0x400) {
                flag = 1;
            }
            for (p = 0; p < lim; p++) {
                ptrs[p] = 0;
            }
            for (p = 0; p < lim; p++) {
                ptrs[p] = OSAllocFromHeap(usrHeap, 0x10000);
                if (ptrs[p] == (void *)0) {
                    break;
                }
            }
            m2 = p << 6;
            for (p = 0; p < lim; p++) {
                if (ptrs[p] != 0) {
                    OSFreeToHeap(usrHeap, ptrs[p]);
                }
            }
            if (m2 >= 0x6000) {
                m2 -= 0x6000;
            }
            if (m2 <= 0x2000) {
                flag = 1;
            }
            sprintf((char *)str_1121, lit_1323, m1 / 1024, m1 & 0x3ff, m2 / 1024, m2 & 0x3ff);
        }
        pause = 0;
        for (;;) {
            s32 draw;
            u32 now;
            frameCnt++;
            if (frameCnt >= 60) {
                drawFps = drawCnt;
                frameCnt = 0;
                drawCnt = 0;
                waitFps = waitCnt;
                waitCnt = 0;
            }
            now = VIGetRetraceCount();
            if (skip > 100) {
                skip--;
            }
            draw = 0;
            if (drawn > 2) {
                draw = 1;
            }
            if (now - last < 2 || draw != 0) {
                for (i = 0; i < 4; i++) {
                    camTmp = &sCamera[i];
                    draw1(i == 0);
                }
                sPutFontAllNn();
                HSD_VICopyXFB(0);
                HSD_VISetXFBDrawDone();
                drawn = 0;
                drawCnt++;
            } else {
                skip = 200;
                drawn++;
            }
            if (VIGetRetraceCount() - last < 2) {
                waitCnt++;
                VIWaitForRetrace();
            }
            IntSeClear();
            last = now;
            HSD_PerfSetDrawTime();
            HSD_PerfSetTotalTime();
            if (gameEndF < 1 && gameStartF == 0) {
                s32 oldpause;
                oldpause = pause;
                if (*(s8 *)(sPad1 + 0x63) != 0) {
                    if (pause == 0) {
                        pause = 1;
                    } else if (pause == 1) {
                        pause = 0;
                    }
                }
                if (GameMode.game_type == 2 && *(s8 *)(sPad2 + 0x63) != 0) {
                    if (pause == 0) {
                        pause = 2;
                    } else if (pause == 2) {
                        pause = 0;
                    }
                }
                if (oldpause != pause) {
                    SeOn(0x6c);
                }
            }
            if (pause != 0) {
                HSD_AudioSFXKeyOffAll();
                sDispPauseProc();
                HSD_PerfInitStat();
                HSD_PerfSetStartTime();
                vsyncN++;
                Zoid[0].rumble_timer = 0;
                Zoid[1].rumble_timer = 0;
                *(u8 *)(sPad1 + 0x84) = 0;
                *(u8 *)(sPad2 + 0x84) = 0;
                *(u8 *)(sPad3 + 0x84) = 0;
                *(u8 *)(sPad4 + 0x84) = 0;
                Zoid[0].rumble_timer = 0;
                Zoid[1].rumble_timer = 0;
                sPadAll();
                HSD_PerfSetCPUTime();
                continue;
            }
            break;
        }
        (void)(tick + 1);
        i = gameEndF + (j = 0);
        if (gameEndF == 12) {
            break;
        }
        if (gameEndF == 8) {
            HSD_AudioSFXKeyOffAll();
            IntSeClose();
            switch (GameMode.game_type) {
            case 1:
                if (winF == 2 || winF == 3) {
                    *(u8 *)(sPad1 + 0x5d) = 0;
                    gameContinueTm = 660;
                    gameEndF = 9;
                    GameMode.win = 3;
                    sPadAll();
                } else {
                    gameEndF = 11;
                    GameMode.win = 2;
                    d84 = 0;
                    d80 = 0;
                    for (i = 0; i < 6; i++) {
                        bp[i] = 0;
                        (bp + 6)[i] = 0;
                    }
                    if (gameTime > 0) {
                        bp[0] = gameTime / 60 * 10;
                    }
                    if (bp[0] > 9999) {
                        bp[0] = 9999;
                    }
                    bp[1] = Zoid[0].health;
                    if (Zoid[2].joint != 0) {
                        bp[2] = Zoid[2].health;
                    } else {
                        bp[2] = Zoid[0].max_health;
                    }
                    bp[3] = Zoid[0].field_1DC8;
                    bp[4] = Zoid[0].field_1DCC;
                    bp[5] = 0;
                }
                break;
            case 4:
                gameEndF = 12;
                GameMode.win = winF == 1 ? 2 : 3;
                break;
            default:
                gameEndF = 12;
                GameMode.win = winF == 1 ? 2 : 3;
                break;
            }
        }
        if (gameEndF == 9) {
            OSReport(lit_1324, gameContinueTm, *(s8 *)(sPad1 + 0x5d) ? "On" : "Of");
            gameContinueTm--;
            if (gameContinueTm <= 0) {
                gameContinueTm = 0;
                gameEndF = 12;
            }
            if (gameContinueTm % 60 == 0 && gameContinueTm != 0) {
                SeOn(gameContinueTm <= 240 ? 0x91 : 0x90);
            }
            if (*(s8 *)(sPad1 + 0x59) != 0 || *(s8 *)(sPad1 + 0x5a) != 0) {
                s32 old = GameMode.win;
                if (*(s8 *)(sPad1 + 0x59) != 0) {
                    GameMode.win = 3;
                } else {
                    GameMode.win = 0;
                }
                if (old != GameMode.win) {
                    SeOn(0x6f);
                }
            }
            if (*(s8 *)(sPad1 + 0x5d) != 0) {
                gameContinueTm = 0;
                gameEndF = 10;
                SeOn(0x10);
            }
        }
        if (gameEndF == 11) {
            if (GameMode.game_type == 1) {
                if (GameMode.win == 2) {
                    if (putBP()) {
                        if (GameMode.map != 11) {
                            TEA_VoiceOn(15, 97);
                        }
                        gameEndF = 13;
                    } else {
                        bpcnt++;
                        if (bpcnt > 15 && prevbp != bp[11]) {
                            bpcnt = 0;
                            TEA_SeKeyOn(0x21f, (u8)TEA_SEVOL, 64, 64);
                            OSReport("b");
                            prevbp = bp[11];
                        }
                    }
                    GameMode.f34 = bp[11];
                } else {
                    gameEndF = 12;
                }
            } else {
                gameEndF = 12;
            }
        }
        if (gameEndF == 13) {
            bpTm.b++;
            if (bpTm.b > 720) {
                gameEndF = 12;
            }
        }
    }
    HSD_FogSet(0);
    clearBlack();
    IntSeClose();
    VIWaitForRetrace();
    IntSeClear();
    IntSeClose();
    VIWaitForRetrace();
    IntSeClear();
    VIWaitForRetrace();
    VIWaitForRetrace();
    VIWaitForRetrace();
    GameMode.f30 = gametime / 60;
    *(u8 *)(sPad1 + 0x84) = 0;
    *(u8 *)(sPad2 + 0x84) = 0;
    *(u8 *)(sPad3 + 0x84) = 0;
    *(u8 *)(sPad4 + 0x84) = 0;
    for (i = 0; i < 16; i++) {
        aiEpilogue(i);
    }
    sEffectEpilogue();
    sLsrEpilogue();
    sMissilePurgeAll();
    sMissileEpilogue();
    sDispPurgeParts();
    sCameraPurge();
    for (i = 0; i < 16; i++) {
        ZoidState *z2;
        z2 = &Zoid[i];
        if (z2->joint != (void *)0) {
            sWeaponUnMountAll(z2);
        }
    }
    ZoidPurgeAll();
    PurgeStageData();
    HSD_AudioSFXKeyOffAll();
    psKillAllGenerator();
    psKillAllParticle();
    clearBlack();
    fogG = 0;
    HSD_FogSet(0);
    (void)(d84 + 1);
    TeaBgm(-1);
    gClear();
}
#undef lit_1312
#define lit_1312 "%s \n"
#undef lit_1325
#define lit_1325 "On"
#undef lit_1326
#define lit_1326 "Of"
#undef lit_1327
#define lit_1327 "b"
namespace { extern const char lit_1338[26]; }
void fixPut(void)
{
    s32 size;
    s32 total;
    s32 sum;
    u8 *base;

    size = 0x48;
    total = 0;
    total += size;
    fix = (FixData *)fixSt;
    MemCpy(&GameMode, (u8 *)fix + 8, 0x48);
    size = 0x1a90;
    total += size;
    MemCpy(IveSaveFlag, (u8 *)fix + 0x50, 0x1a90);
    total += size;
    MemCpy(IveSave, (u8 *)fix + 0x1ae0, 0x1a90);
    total += size;
    MemCpy(IveSave + 0x1a90, (u8 *)fix + 0x3570, 0x1a90);
    total += size;
    MemCpy(IveSave + 0x6a40, (u8 *)fix + 0x5000, 0x1a90);
    fix->size = total;
    if ((u32)total >= 0x6a90) {
        OSReport(lit_1338, 0x6a90, total);
    }
    size = 0;
    sum = 0;
    base = (u8 *)fix + 4;
    for (; size < total; size++) {
        sum += base[size];
    }
    fix->sum = sum;
}
s32 fixGet(void)
{
    s32 i = 0x48;
    s32 sum;
    s32 size = 0;
    u8 *p;
    i = 0;
    sum = 0;
    fix = (FixData *)fixt;
    size = fix->size;
    p = (u8 *)fix + 4;
    for (; i < size; i++) {
        sum += p[i];
    }
    ZeroMemory(&GameMode, 0x48);
    ZeroMemory(IveSaveFlag, 0x1a90);
    ZeroMemory(IveSave, 0x1a90);
    ZeroMemory(IveSave + 0x1a90, 0x1a90);
    ZeroMemory(IveSave + 0x3520, 0x1a90);
    ZeroMemory(IveSave + 0x4fb0, 0x1a90);
    ZeroMemory(IveSave + 0x6a40, 0x1a90);
    if (fix->sum != sum) {
        return 0;
    }
    MemCpy((u8 *)fix + 8, &GameMode, 0x48);
    MemCpy((u8 *)fix + 0x50, IveSaveFlag, 0x1a90);
    MemCpy((u8 *)fix + 0x1ae0, IveSave, 0x1a90);
    MemCpy((u8 *)fix + 0x3570, IveSave + 0x1a90, 0x1a90);
    MemCpy((u8 *)fix + 0x5000, IveSave + 0x6a40, 0x1a90);
    return 1;
}
void fixSave(void)
{
    MemCpy((void *)fixSt, fixt, 0x6a90);
    fix = (FixData *)fixSt;
    fix->sum = -1;
}
#pragma force_active on
ModelArchive *dspLoad;
s32 normLineDEBUG;
s32 ctrlZoid;
s32 AreaIn;
s32 AreaIn2;
s32 vOn44;
FixData *fix;
s32 lastGame;
namespace { __declspec(section ".sdata2") extern const f32 lit_1354[1] = { 0.0f }; }
namespace { __declspec(section ".sdata2") extern const f32 lit_1453[1] = { 0.0f }; }
#pragma force_active off
}
