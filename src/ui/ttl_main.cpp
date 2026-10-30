extern "C" {
#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"

extern int printf(const char *format, ...);
extern void OSReport(const char *format, ...);
extern s32 sReadLenPtr(char *path, void *buffer, s32 offset, s32 length);
extern void TeaBgm(s16 track);
#define NULL ((void *)0)

typedef struct ZoidWeaponSlot {
    s32 weapon;
    u8 field_04[8];
} ZoidWeaponSlot;

typedef struct ZoidState {
    u8 field_0000[0x28];
    void *field_0028;
    u8 field_002C[0x14];
    struct HSD_JObj *joint;
    u8 field_0044[0x438];
    f32 field_047C;
    u8 field_0480[0x1448];
    ZoidWeaponSlot weapons[28];
    u8 field_1A18[0x200];
    f32 field_1C18[3];
    u8 field_1C24[0x1B8];
    s32 field_1DDC;
    u8 field_1DE0[4];
    s32 field_1DE4;
    u8 field_1DE8[0x118];
    s32 field_1F00;
    s32 field_1F04;
    u8 field_1F08[0x18C];
    void *anim_data;
    u8 field_2098[0x2068];
} ZoidState;

extern ZoidState Zoid[16];
extern void free(void *ptr);
extern void PurgeArc2(void *archive);
extern void ZoidPurge(ZoidState *zoid);
extern void HSD_VIWaitXFBFlush(void);

typedef struct GameModeState {
    s32 field_00;
    s32 game_type;
    s32 field_08;
    s32 win;
    s32 field_14;
    s32 field_18;
    s32 map;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 gametime;
    s32 bp_earned;
    s32 field_38;
    s32 field_3C[5];
} GameModeState;

extern GameModeState GameMode;
extern void *LoadArc2(char *path);
extern void *sReadFileU(char *path, s32 flags);
extern void *binB2(void *pack, s32 index, s32 *size);
extern void *LoadArc2B(void *data, s32 size, char *name);
extern int sprintf(char *buffer, const char *format, ...);

typedef struct GameZoidEntry {
    s16 type;
    s16 mounts[28];
    s8 color;
    s8 mount_colors[28];
    u8 field_57;
    s16 field_58;
    s16 field_5A;
    s16 field_5E;
    s16 field_60[3];
    s8 mount_8;
    s8 mount_9;
} GameZoidEntry;

typedef struct GameSaveRecord {
    s32 field_0000;
    s16 owned_zoids[30];
    u8 owned_zoid_count;
    u8 field_0041;
    s16 owned_parts[450];
    s16 owned_part_count;
    s16 list_parts[450];
    s16 list_flag[450];
    s16 list_count;
    s16 zoid_field_0AD2[30];
    s16 zoid_parts[30][28];
    s16 zoid_field_119E[30];
    s16 zoid_field_11DA[30][3];
    s8 zoid_color[30];
    s8 part_colors[30][28];
    s16 field_15F4;
    s16 zoid_slot;
    s16 field_15F8;
    s16 field_15FA;
    s16 partner_slot;
    s16 field_15FE;
    s16 field_1600;
    s16 field_1602;
    s16 field_1604;
    s16 field_1606;
    u8 field_1608[8];
    s8 person_got[71];
    s8 field_1688;
    u8 field_1689;
    s16 zoid_list[150];
    s8 zoid_list_count;
    s8 zoid_got[150];
    u8 field_1A0F;
    u8 field_1A10;
    s8 persons[71];
    s8 person_count;
    s8 shop_flag[450];
    s16 shop_count;
    s8 system_flags[0x32];
    s32 money;
    s16 map_no;
    u8 single_team;
    s8 field_1C87;
    s8 stage;
    s8 field_1C89;
    s8 next_stage;
    s8 field_1C8B;
    s8 field_1C8C;
    s8 field_1C8D;
    u8 country;
    s8 mount_8;
    s8 mount_9;
    u8 field_1C91;
    s16 save_year;
    s8 save_month;
    s8 save_day;
    u8 field_1C96[6];
    s32 play_time;
    u8 field_1CA0[2];
    u8 cfg_bgm;
    u8 cfg_se;
    u8 cfg_voice;
    u8 cfg_rumble;
    u8 cfg_vstime;
    u8 cfg_vsmode;
    s8 cfg_bplimit;
    u8 field_1CA9[7];
} GameSaveRecord;

extern s32 ZoidInitC(ZoidState *zoid, s32 type, s32 color);
typedef struct HSD_JObj HSD_JObj;
extern HSD_JObj *GetChild(HSD_JObj *joint, s32 a, s32 b, s32 c, s32 d, s32 e);
extern void HSD_JObjDisp(HSD_JObj *joint, void *mtx, u32 flags, u32 rendermode);
extern void batFlag(void);

typedef struct ZoidDataEntry {
    char name[0x34];
    char file[8];
    s32 field_50;
    u8 field_54[4];
    s32 field_58;
    s32 field_5C;
    u8 field_60[6];
    s16 field_66;
    u8 field_68[0xC];
    s16 default_parts[20];
    u8 field_9C[0x10];
    s16 point_flags[12];
    u8 field_C4[0x6DE];
    s16 default_color;
    u8 field_7A4[4];
    s16 sort_key;
    s16 weapon_points[10];
    s16 mount_parts[10];
    u8 fly;
    u8 part_colors[10];
    u8 field_07DD[0x1F];
} ZoidDataEntry;

typedef struct PartDataEntry {
    char name[0x33];
    char short_name[0x1D];
    s32 price;
    s32 field_54;
    u8 field_58[0x10];
    s16 category;
    u8 field_6A[0x4A];
    s16 zoids[12];
    u8 field_CC[0x20];
    s16 color;
    u8 field_EE[0xC];
    s16 rank;
    u8 field_FC[2];
    u8 no_fly;
    u8 field_FF[0x25];
} PartDataEntry;

typedef struct YesNoAnim {
    s16 state;
    s16 anim;
} YesNoAnim;

typedef struct PadState {
    u8 field_00[0x40];
    s8 field_40;
    s8 field_41;
    u8 field_42[3];
    s8 field_45;
    u8 field_46;
    s8 field_47;
    u8 field_48[7];
    s8 field_4F;
    s8 field_50;
    s8 field_51;
    s8 field_52;
    s8 field_53;
    s8 field_54;
    s8 field_55;
    s8 field_56;
    s8 field_57;
    s8 field_58;
    s8 field_59;
    s8 field_5A;
    s8 field_5B;
    s8 field_5C;
    s8 pressed_a;
    s8 field_5E;
    s8 field_5F;
    s8 field_60;
    s8 field_61;
    s8 field_62;
    s8 field_63;
    u8 field_64[8];
    f32 field_6C;
    f32 field_70;
    u8 field_74[0x10];
    s8 rumble;
    s8 previous_rumble;
    u8 field_86[2];
} PadState;

typedef struct BatMapEntry {
    s16 field_00[3];
    s16 map;
    s16 field_08[4];
} BatMapEntry;

typedef struct HSD_LObj HSD_LObj;
typedef f32 Mtx[3][4];

extern s32 menudispyesnoflag;
extern s32 menudispyesnopoint;
extern YesNoAnim menudispyesnoanm;
extern s16 menusaveloadslot;
extern u8 menuZoidBattleModeSingleTeam;
extern PadState *sPad1;
extern s32 flag_saveok;
extern s32 sCamNN, sCamNN2;
extern s32 dspLoadF;
extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
extern HSD_LObj *HSD_LObjLoadDesc(void *desc);
extern void HSD_LObjRemoveAll(HSD_LObj *lobj);
extern void HSD_VISetBlack(s32 black);
extern void memcopy(void *dst, void *src, s32 size);
extern void *memcpy(void *dst, const void *src, u32 size);
extern void menuSetTitle(s16 title);
extern void menuSetTitleButton(s16 button);

extern void sPadAll(void);

extern void menu_sDispStartYesNo(void);
extern void SeRet(void);


extern void EndeFin(void);

extern void debug11(void);
extern s32 TEA_mcFileOldFileExist(s32 channel);
extern void HSD_AudioSFXKeyOffAll(void);
extern void VIWaitForRetrace(void);
extern void ttlMovie(char *path, s32 skip);
extern void gRefresh(void);
extern s16 menufncPushStart(void);
extern void TEA_SoundMasterVol(s32 vol, s32 mode);
extern s32 HSD_Rand(void);
extern s16 menufncPushStartMain(void);
extern s16 menufncZoidBattleNewGameLoadGame(void);
extern s16 menufncZoidMissionEmpireCountry(void);
extern void menuZoidMission_NonSaveData_InitData(void);
extern void menufncZoid_ConfigMenu(void);
extern s16 menufncZoidBattleSingleTeamGame(void);
extern s16 menufncZoidVSGame(void);

extern s32 menufncMainMenu(void);
extern s32 menufncStoryMode01(void);
extern s32 menufncZoidBattle02Hokyu(void);
extern void ttlMenu_sDispMissionHokyu(void);
extern s32 menufncMission_Menu(void);
extern s32 menufncZoidMission_MenuEntry(void);
extern s32 menufncZoidBattle02(void);
extern s32 menufncZoidBattle01Human(void);
extern s32 menufncZoidBattle_BattleMenu(void);
extern s32 menufncZoidBattle_BattleMenuEntry(void);
extern s32 menufncZoidBattle_HumanBattleMenuEntry(void);

extern void ttlMenu_sDispBattleNextYokok(void);
extern s32 menufncZoidBattleEnd(void);
extern void menuZoidVS_NonSaveData_InitData(s16 side);
extern void menuZoidFist_NonSaveData_InitData(s16 side);

extern s32 menufncZoidVsMenu(void);
extern s32 menufncZoidVsMenuEntry(void);
extern s32 menufncZoidVsFistMenu(void);
extern s32 menufncZoidVs4pMenu(void);
extern s32 menufncZoidGalleryMenu(void);
extern void testbitsetmenu_zoid(void);
typedef struct HSD_CObj HSD_CObj;

typedef struct MenuArchive {
    u8 field_00[0x2C];
    HSD_CObj *cobj;
    HSD_LObj *lobj;
    u8 field_34[8];
    HSD_JObj *joints[16];
} MenuArchive;

typedef struct CameraState {
    u8 field_00[8];
    HSD_LObj *lobj;
} CameraState;

extern MenuArchive *menu_yesnop;
extern s32 vsyncN;
extern CameraState sCamera;
extern CameraState *menucam;
extern s32 TXT_Menu_size;
extern void HSD_StartRender(s32 pass);
extern void HSD_EndRender(void);
extern void HSD_SetZSortMode(s32 a, s32 b);
extern s32 HSD_CObjSetCurrent(HSD_CObj *cobj);
extern void HSD_CObjEndCurrent(void);
extern void HSD_LObjSetCurrentAll(HSD_LObj *lobj);
extern void HSD_LObjSetupInit(HSD_CObj *cobj);
extern void GXSetCullMode(s32 mode);
extern void menu_sDispYesNoMenu(void);
extern void TEA_THP_PlaySkip(char *path, s32 loop, s32 skip);
extern void sCameraSetNormal(CameraState *camera);
extern void HSD_VICopyXFB(s32 a);
extern void HSD_VISetXFBDrawDone(void);
extern void sPadInit(void);
extern void sReadPre(void);
extern void menu_MissionDemo(void);
typedef struct GXColor {
    u8 r, g, b, a;
} GXColor;

typedef struct PadButtons {
    u8 field_00[0x4F];
    s8 up;
    s8 down;
    s8 left;
    s8 right;
    u8 field_53[8];
    s8 l;
    s8 r;
    s8 a;
    s8 b;
} PadButtons;

extern PadState *sPad3;
extern s32 menuCursor;
extern s32 menuParticles;
extern void sPutString2(char *text, s32 x, s32 y, s32 z, GXColor color, GXColor shadow, s32 a, s32 b);
extern s32 Tea_atoi(char *text);
extern int puts(const char *s);
extern void zdDispCursorSet(ZoidState *zoid, s32 mount, s32 on);
extern s32 menuretno;

typedef struct PersonDataEntry {
    char name[0x3C];
    s32 order;
    u8 field_40[0x28];
} PersonDataEntry;

extern void SetAnimArc2(MenuArchive *archive, s32 anim, s32 loop);
extern s32 jobjAnim(HSD_JObj *joint);
extern void sMakeMatrix(HSD_JObj *joint);
extern void jobjDisp(HSD_JObj *joint);
extern void sPutFontRect(char *text, HSD_JObj *a, HSD_JObj *b, GXColor color, GXColor shadow, s32 c, s32 d);
extern void infoStrMsg(char *msg);
extern void infoAddMan(s32 person);
extern void infoAddZoid(s32 zoid);
extern int strcmp(const char *a, const char *b);
extern s32 bitTBL[];
extern void menuZoidBattle_SaveData_AddData(void);
extern s32 fexist(char *path);

extern void clearFlagThinkVS(s32 save);
extern void clearFlagThinkFIST(s32 save);
extern void clearFlagThinkG(s32 save);
extern s16 menuwepdispnum;
extern void ZoidWeaponPurge(ZoidState *zoid);
extern void sWeaponMount(s32 part, ZoidState *zoid, s32 mount, s32 slot);
extern void sWeaponColor(s32 weapon, s32 color, s32 a);
extern char *strcpy(char *dst, const char *src);
extern PadState *sPad2;
extern PadState *sPad4;
extern void SeSelectB(void);
extern char menudispyesnostr1[180];
extern char menudispyesnostr2[180];
extern void jobjDispAll(HSD_JObj *joint);
extern void menu_sDispYesNoMenu(void);
extern void jobjCopyPos(HSD_JObj *src, HSD_JObj *dst);
extern f32 putAscJobj(HSD_JObj *joint, s32 font, char *text);
extern void HSD_VIWaitEFBFlush(void);
extern char *MC_FILENAME_MSG;
extern s32 ttlMenuBatSaveLoad1P2P(s32 mode, s32 a, s32 b);
typedef struct HSD_VIStatus {
    u8 field_00[0x48];
    GXColor bg_color;
} HSD_VIStatus;

extern HSD_VIStatus HSD_VIData;
extern s16 menusaveloadmode;
extern void HSD_FogSet(void *fog);

extern void menu_sDispSaveLoadDataWindow(void);
extern void menu_sDispSaveLoadTitle(void);
typedef struct SaveLoadWindow {
    s16 active;
    s16 anim;
} SaveLoadWindow;
extern s32 sSetAnim1(void *archive, s32 model, HSD_JObj *joint, s32 animation);
extern s32 jobjAnim1(HSD_JObj *joint);
extern s32 TEA_mcLoadFile0(s32 slot, s32 file, s32 record);
extern s16 MemCardKeepBlock;
extern void TEA_mcMsgDisp(s32 slot, s32 msg);
extern s32 TEA_mcSaveFile0(s32 slot, s32 file, s32 record);
extern void TEA_mcMsg(s32 slot, s32 msg);
extern void TEA_mcMsgNoChkSlot(s32 slot, s32 msg);
extern s32 ZoidFileBroken;
extern s32 MemCardFreeSize;
extern s32 MemCardFreeDir;
extern s32 TEA_mcFileMountExist(s32 slot, char *name);
extern s32 CARDFreeBlocks(s32 chan, s32 *bytes, s32 *files);
extern s32 MemCardStatus[2];
extern s32 MemCardError;
extern s32 TEA_mcChkSlot(s32 slot);
extern s32 TEA_mcChkSector(s32 slot);
extern s32 TEA_mcChkRest(s32 slot);
extern s32 TEA_mcChkEncoding(s32 slot);
extern s32 TEA_mcFormat(s32 slot);
extern s32 TEA_mcMsgYesNo(s32 slot, s32 msg);
extern void clrYesNoMsg(void);
extern s32 TEA_mcUnMount(s32 slot);
extern s32 TEA_mcChkSlotNoMsg(s32 slot);
extern s32 TEA_mcMountNoChkError(s32 slot);
extern s32 TEA_mcChkBroken(s32 slot);
extern s32 TEA_mcChkIoErr(s32 slot);
extern s32 TEA_mcMsgYesNoNotChkSlot(s32 slot, s32 msg);
extern s32 TEA_mcBadEncFormatEnc(s32 slot);
extern s32 TEA_mcMount(s32 slot);
extern s32 TEA_mcFileDel(void);
extern YesNoAnim menusaveloadDatamainanm;
extern YesNoAnim menusaveloadDataNoanm;
extern s16 menuSLPos[5];
extern s32 menusaveloadDataSelect;
extern void menu_sDispSaveLoadDataNo(s16 no, s16 slot);
extern u32 stagename_mis[73];
typedef struct TitleAnim {
    u8 mode;
    u8 state;
    u8 prev;
    u8 reserved;
} TitleAnim;

extern void HSD_JObjDispAll(HSD_JObj *joint, void *mtx, u32 flags, u32 rendermode);
extern void menu_sDispSaveLoadTitleSubButton(void);
extern void SeConfirm(void);
extern s32 SeOn(s32 se);
extern void menu_sDispBattleSet(void);
extern s32 menuSE_FLAG_ON;
extern s32 menuSE_FLAG_U;
extern s32 menuSE_FLAG_D;
extern s32 menuSE_FLAG_L;
extern s32 menuSE_FLAG_R;
extern void HSD_CObjSetEyePosition(HSD_CObj *cobj, f32 *pos);
extern void *fogG;
extern void sWeaponAnimAll(void);
extern void sWeaponMoveAll(void);
extern void sWeaponDispAll(void);
extern void menuZoidBattleSetStage_PersonSelect(s32 mode);
extern void menu_TITLE_dispEntry(s32 mode);
typedef struct ButtonAnim {
    u8 state;
    u8 cur;
    u8 old;
} ButtonAnim;

extern f32 putAscJobjLR(HSD_JObj *joint, s32 font, char *text, s32 right_align);
extern s32 menuanmend;
extern void menu_sDispBattleFarm(void);
extern s16 TEA_menufnc2p0(s32 a, s32 b, s16 c, s16 d, s32 e, s32 f);
extern s16 menuDetailMode;
extern s32 dispDetailFlag;
extern s32 jobjColor(HSD_JObj *joint, char *name, s32 a);
extern s16 menumode;
extern void menu_sDispBattleShop(void);
extern void menu_sDispBattleShopLButtonZoid(void);

extern void MessageBoxBatShop(char *msg);
extern s32 VolSe, VolBgm, VolVoice, rumble, VsTime, VsMode, VsBpLimit;
extern void menufnc_MapInfo(s32 mode, s32 map);
extern s16 TEA_VOICEVOL, TEA_BGMVOL, TEA_SEVOL;
extern s16 TEA_BGMMAXNUM;
extern s32 rumbleTm;
typedef struct SampleVoice {
    u8 character;
    u8 reserved;
    s16 voice;
} SampleVoice;

extern void TEA_AudioSeqVolume(u8 volume, s32 a, s32 b);
extern void TEA_VoiceOff(void);
extern void TEA_VoiceOn(u8 character, s32 voice);
extern s32 TestVoice;
extern s32 sbT, sbN, crN;
extern void HiddenParts(HSD_JObj *joint, s32 hidden);
extern void ZoidSetAnim(ZoidState *zoid, s32 anim);
extern void setAnimRate(HSD_JObj *joint, f32 rate);
extern void HSD_JObjReqAnimAll(HSD_JObj *joint, f32 frame);
extern s32 aNumTBL, aNumTBL2;
extern MenuArchive *menuvs4, *menuvs4a, *menuvs4b, *menuvs4c, *menuvs4d, *menuvs4e;
extern void menu_sDispBattleSetLButtonAnm(s32 button);
extern void menu_sDispBattleShopLButtonMainAnm(s32 button);
extern void menu_sDispBattleShopLButtonAnm(s32 button);
extern void menu_sDispBattleSetLButtonZoid(void);
extern void menu_sDispBattleSetLButtonReset(void);
extern void menu_sDispBattleSetLButtonExit(void);
extern void menu_sDispBattleSetZoidStatus(void);
extern s32 vsZoidBig1(s32 player);
typedef struct MgState {
    u8 field_0000[0x2AD4];
    s32 map;
    u8 field_2AD8[0x741];
    s8 field_3219;
    s8 field_321A;
    s8 field_321B;
} MgState;

extern MgState *mgP;
extern void menu_sDispStart(void);
extern void *rdMapInfo(s32 mode, s32 map);
extern void StgLoad(s32 stage, s32 usb);
extern s32 vsZoidStage(void);
extern void menu_sDispVsEntry(void);
extern PadState sPad[4];
extern s32 chk22(s32 map);
extern void hokyuu(s32 map);
extern void gClear(void);
extern void SeCancel(void);
extern void ttl_GameZoidSetZoidDefParts(s16 player, s16 zoid);
typedef struct Vs4Anim {
    s32 state;
    s32 anim;
    s32 start;
    s32 end;
} Vs4Anim;
extern void SetMenuMsgAdr(s16 msg, char *dst);
extern void GXInvalidateVtxCache(void);
extern void GXInvalidateTexAll(void);
extern void detPerson(s32 no);
extern s32 chkMapFly(s32 kind);
extern int printf(const char *format, ...);
extern void menu_sDispVsFarm(void);
extern void menu_sDispBattleFarmWeapon(void);
extern void menu_sDispBattleEntryPersonPartner(void);
extern void menu_sDispBattleEntryPersonPartnerZoidPic(void);
extern void menu_sDispBattleEntryPilotZoidList(void);
extern s32 enemyChara[2];
extern void putErr(char *message);
extern void menu_sDispMsgWin(s16 model, s16 part);
extern void menu_sDispBattleMsgArea(void);
extern void menu_sDispBattleShopHaveListDsip(void);
extern void menu_sDispBattleShopLButtonAnimZoidWepOptSub(void);
extern void menu_sDispBattleShopunfold(void);
extern void menu_sDispBattleShopLButtonZoidWepOptSub(void);
extern void menu_sDispBattleShopHaveBp(void);
extern void menu_sDispBattleShopZoidHaveZoidList(void);
extern void menu_sDispBattleShopHavePartsList(void);
extern s32 detPartsn;
extern void flPictureDispParts(s32 part);
extern void menu_sDispSetButton(void);
extern void menu_sDispBattleEntry(void);
extern void menu_sDispBattleFarmBack(void);
extern void menu_sDispBattleFarmZoid(void);
extern void menu_sDispBattleFarmTitle(void);
extern void menu_sDispBattleFarmTitleSubButton(void);
extern void menu_sDispBattleEntryPersonZoidStatus(void);
extern void menu_sDispBattleEntryPersonPartnerZoidSelectFrame(void);
extern void menu_sDispDetailAll(void);
extern void menu_sDispVsFarmPlayer1Icon(void);
extern void menu_sDispVsFarmPlayer2Icon(void);
extern void menu_sDispVsEntryPersonPartnerZoidPic(void);
extern void menu_sDispVsEntryPersonZoidStatus(void);
extern void menu_sDispVsFarmEntryButton(void);
extern void menu_sDispVsFarmStageButton(void);
extern void menu_sDispVsFarmBattleButton(void);
extern void menu_sDispVsFarmMapInfo(void);
extern void menu_sDispMissionDemoMsgArea(void);
extern void menu_sDispMissionDemoSelectMenu(void);
extern GameSaveRecord IveSave[9];
extern GameSaveRecord IveSaveFlag;
extern void menu_sDispBattleHumanEntryPersonPartner(void);
extern void menu_sDispBattleHumanEntryPersonPartnerZoidPic(void);
extern void menu_sDispBattleHumanEntryPersonZoidStatus(void);
extern void menu_sDispBattleHumanEntryPilotZoidList(void);
extern void menu_sDispHumanEntryPersonStatus(void);
extern void menu_sDispBattleHumanEntryPersonPartnerZoidSelectFrame(void);
extern s8 menuZoidBattleStage;
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern void HSD_CObjSetFov(HSD_CObj *cobj, f32 fov);
extern s16 menusetcl[20];
extern s16 flag_VsFarmEntryButton[2];
extern s32 zdCalcFarm(ZoidState *zoid, s32 a, s32 b, s32 c);
extern void menu_sDispVsFistFarm(void);
extern void menu_sDispMisFarm(void);
extern s32 enemyZoid[2];
extern void yokoku2(s32 table);
extern s16 TEA_menufnc4p(s32 pad, s16 *a, s16 *b, s16 c, s16 d, s32 e, s32 f);
extern void SeLiftStart(void);
extern void SeLiftStop(void);
extern void ttl_WeaponAllUnMount(s16 slot);
extern s32 ZoidInit(ZoidState *zoid, s32 type);
extern s32 ZoidColor(ZoidState *zoid, s32 color, s32 a);
extern void menu_sDispMisEntryPersonPartner(void);
extern void menu_sDispMisEntryPersonPartnerZoidPic(void);
extern void menu_sDispMissionEntryPilotZoidList(void);
extern void angLimit(f32 *value);
extern void eyeCamSet(void);
extern void menu_sDispVs4pPersonPic(s32 index);
extern void menu_sDispBattleSetZoidHaveZoidList(void);
extern void menu_sDispBattleSetequipPartsList(void);
extern void menu_sDispBattleSetequipParts(void);
extern void menu_sDispBattleSetHavePartsList(void);
extern void menu_sDispVsEntryLoadButton(void);
extern void menu_sDispVsFarmPlayer1IconEnt(void);
extern void menu_sDispVsEntryPilotZoidList(void);
extern void menu_sDispVsEntryPersonPartnerZoidSelectFrame(void);
extern void menuDispLimitBP(void);
extern u8 mgI[13056];
extern s32 menu_sDispMissionHokyu(u8 *info, MenuArchive *arc, s32 done);
extern void menu_sDispBattleFarmModeButton(s16 a, s16 b);
extern void menu_sDispBattleFarmPersonZoidPartner(void);
extern void menu_sDispMisFarmModeButton(s16 a, s16 b);
extern void menu_sDispVs4pSubMenu(s32 player);
extern GXColor gxWhite;
extern GXColor gxBack;
extern s64 OSGetTime(void);
extern s64 __div2i(s64 a, s64 b);
extern s64 __mod2i(s64 a, s64 b);
extern void zdCalcZoid(ZoidState *zoid, s32 id, s32 flag);
extern s32 testnumedit(s32 x, s32 y, s32 value);
extern void menu_sDispAllHavePartsList(void);
extern void SeWinon(void);
extern void menu_sDispDetailSetting(f32 a, f32 b);
extern void menu_sDispDetailZoidn(f32 a, f32 b);
extern void menu_sDispDetailWeapon(f32 a, f32 b);
extern void menu_sDispDetailOption(f32 a, f32 b);
extern void menu_sDispDetailPerson(f32 a, f32 b);
extern void sLinear1D(f32 *value, f32 target, f32 rate);
extern void menu_sDispStartPushStart(void);
extern void menu_sDispZoidEmpireCountry(void);
extern void menu_sDispZoidBattleNewGameLoadGame(void);
extern void menu_sDispZoidBattleSingleTeamGame(void);
extern void menu_sDispStartMainMenu(void);
extern void menu_sDispZoidVSGame(void);
extern void menu_sDispStartPushLogo(void);
extern void menu_sDispStartWhiteInWhiteOut(void);
extern void sPutFontRectC(char *text, HSD_JObj *a, HSD_JObj *b, GXColor color, GXColor shadow, s32 c, s32 d);
extern void TEA_AudioSeqVol(u8 a, u16 b);
extern s32 menu_MissionDemoScrPlay1(void);
extern void SeRetDemo(void);
extern void ZoidToFly(ZoidState *zoid);
extern s32 getEndF1(HSD_JObj *joint);
extern void *searchTObjN(HSD_JObj *joint, void *p);
extern void texScroll(void *obj, f32 a, f32 b, f32 c);
extern void menu_VsFarmMapLoad(s16 map);
extern void PutParts(s16 a, s16 b, HSD_JObj *c);
extern void __assert(const char *file, s32 line, const char *expr);
extern void HSD_JObjSetMtxDirtySub(void *jobj);
extern void dispBGM(MenuArchive *archive);
extern void dispRumble(MenuArchive *archive);
extern void dispSe(MenuArchive *archive);
extern void dispVoice(MenuArchive *archive);
extern void dispVsTime(MenuArchive *archive);
extern void selectcursolanm(s16 model);
extern s32 TestBgm;
extern u32 VIGetRetraceCount(void);
extern s32 menuzoidequippartslist;
extern s8 menuzoidhavepartslist;
extern s16 menudispshophavezoidlist;
extern void HSD_CObjSetInterest(HSD_CObj *cobj, f32 *interest);
extern void sGlobal2Local(HSD_JObj *joint);
extern void sMakeDirty(HSD_JObj *joint);
extern void zdDispCursor(void);
extern s8 *det_Load(char *path);
extern s32 getEndF(HSD_JObj *joint);
extern s32 sbTO, sbTA, csN, csA, csAO, csOld, crO;
extern u32 TEA_SeOn(u16 se, f32 a, f32 b);
extern s32 getEndFALL(HSD_JObj *joint);
extern void GXGetCullMode(s32 *mode);
extern u8 menumodeanm[20];
extern u8 menufarmanm[20];
extern u8 menufarmanmold[20];
extern s16 menufarmcl[20];
extern s16 menufarmsl[20];
extern s16 menufarmst[20];
extern s16 menufarmmh[20];
extern s16 menufarmmn[20];
extern ButtonAnim menubackanm[20];
extern s8 menufarmmode;
extern s32 flag_VsFarmEntryStageBattleExit;
extern u8 menucuranm[2];
extern s16 flag_VsFarmStageButton[2];
extern s16 flag_VsFarmBattleButton[2];
extern s16 flag_VsFarmExitButton[2];
extern s32 flag_menuvsentzoid[2];
extern s32 flag_menuvsenthuman[2];
extern s8 menuvsmode[2];
typedef struct VsEntryLoadFlag { s16 a; s16 b; } VsEntryLoadFlag;
extern VsEntryLoadFlag flag_VsEntryLoadButton[2];
extern s16 ttl_vs_modeLR;
extern s16 flag_VsFarmPlayer1Icon;
extern s16 flag_VsFarmPlayer2Icon;
extern void *menu1p_map;
extern s32 chkMapBP(s32 player);
extern void resetSkipOp(void);
extern s32 menuYesNo(s32 point, char *msg1, char *msg2);
extern void TEA_setvsync(void);
extern void ttl_SortZoidKind(s16 save, s16 kind);
typedef struct StgS {
    u8 field_00[6];
    s16 scale;
} StgS;
extern StgS Stg[100];
extern void jobjCopyPosS(HSD_JObj *dst, HSD_JObj *src);
extern s32 HSD_Randi(s32 max);
extern void vs4_MenuTop(s32 pad);
extern void vs4_MenuHuman(s32 pad);
extern s16 initvs4pzoid[51];
extern s16 initvs4phuman[60];
extern BatMapEntry menu_MisMapData[60];
extern f32 menubackcampos[3];
extern void VISetBlack(s32 black);
extern void TEA_HotReset(u32 code);
extern void menuZoidBattleGo(void);
extern void menuSortPartsHave(void);
extern s32 menufncZoidBattle_BattleMenuSet(void);
extern s16 dlgpersonno;
extern s16 dlgzoidno;
extern s16 dlgpartsno;
extern void TEA_SetTimerInit(void);
extern void SeBuzzerC(void);
extern s32 menufncZoidBattle_BattleMenuShop(void);
#define lit_5845 "MenuZoidBattle Entry On\n"
#define lit_5842 "Menu CamPos x=%f y=%f z=%f \n"
#define lit_5844 "MenuEnd \n"
#define lit_5846 "MenuZoidBattle Entry Ok\n"
#define lit_5847 "MenuZoidBattle Entry\n"
#define lit_5848 "MenuZoidBattle Farm\n"
extern s32 TEA_mcChkSlotZoid(s32 slot);
extern s32 chkCstmBP(s32 player, s32 slot);

void TeaMenuBGM(s16 setno);
void ttlBanPurgeZoid(void);
void ttlBanPurgePerson(void);
void ttlMenuAllPurge(void);
void ttlMenuAllPurgeMission(void);
void ttlMenuPurgeParts(void);
void ttlMenuLoadParts(s32 kind);
void ttlMenuPurgeBack(void);
void ttlMenuLoadBack(s32 kind);
void ttlMenuLoadDetail(void);
void ttlMenuPurgeDetail(void);
void ttlMenuLoadZoid(void);
void ttlMenuLoadPersonBanner(void);
void ttlMenuLoadZoidBanner(void);
void ttlMenuDispParts(HSD_JObj *joint, s32 a, s32 b, s32 c, s32 d);
void debugmenudraw(void);
void ttlMain_MissionFlagChkSet(void);
void ttlMain_BattleFlagChkSet(void);
void menu_sDispStartYesNo(void);
void ttlMovie(char *path, s32 skip);
s32 TEA_defstatusload(void);
s32 menufncMainMenu(void);
s32 menufncStoryMode01(void);
void testbitsetmenu_zoid(void);
s32 Tea_atoi(char *text);
s32 testnumedit(s32 x, s32 y, s32 value);
void menuPartsWepSetListZoid(s16 category, s16 zoid, s16 point);
void menuPartsShopListZoid(s16 category, s16 zoid);
void menuPartsShopListAttackerCompareZoid(s16 category, s16 zoid);
void menuPartsHaveListZoid(s16 category, s16 zoid);
void menufnczoidmountpointdisp(void);
void menufnczoidmountpointdispset(s8 point);
s32 menufncZoidBattle01Human(void);
s32 menufncZoidBattle02(void);
s32 menufncZoidBattle02Hokyu(void);
s32 menufncZoidBattleEnd(void);
void menuZoidBattleGoPartsOptSet_GameZoid(s16 gz, s16 save, s16 slot, s32 flag);
void infoAddZoid(s32 zoid);
void infoAddMan(s32 person);
void addZoidsCustm(s32 save, s32 zoid);
void ttl_HaveSortHuman(s16 save);
void ttl_HaveAddHuman(s16 save, s16 person);
void addVS_FIST(s32 save);
void addZoids(s32 save, s32 zoid);
s32 Tea_compare(char *a, char *b, s32 length);
s32 ttlChkFlagCopy(s32 src, s32 dst);
s32 ttlChkFlagRank(s32 rank);
void clearFlagThink(s32 save, s32 mode);
void clearFlagThinkG(s32 save);
void clearFlagThinkVS(s32 save);
void clearFlagThinkFIST(s32 save);
void clearFlagThink4P(s32 save);
void iveClear(s32 save);
void menuZoidBattle_NonSaveData_InitData(void);
void menuZoidBattle_SaveData_AddData(void);
void menuZoidMission_EndSaveData_start(void);
void menuZoidVS_NonSaveData_InitData(s16 side);
void menuZoidFist_NonSaveData_InitData(s16 side);
void menuZoidGallery_InitData(void);
void ttl_HaveAddParts(s16 save, s16 part);
void ttl_HaveAddZoid(s16 save, s16 zoid);
s32 ttl_ZoidChkWepPoint(s16 zoid, s16 point);
s32 ttl_ZoidWepPointPermission(s16 zoid, s16 point);
s16 ttl_ZoidRemoveContractWep(s16 save, s16 slot, s16 point);
s16 ttl_ZoidRemoveContractOpt(s16 save, s16 slot);
s16 ttl_ZoidRemoveContractSub(s16 save, s16 slot, s16 sub);
void ttl_IveSaveSetZoidDefParts(s16 save, s16 slot, s16 zoid);
void ttl_WeaponAllUnMount(s16 slot);
void ttl_WeaponAllMount(s16 slot);
s16 ttl_GetMountNo(s16 zoid, s16 slot);
char *GetMenuMsgAdr(s16 msg);
s32 menuYesNoMask(s32 point, char *msg1, char *msg2, s32 pad1, s32 pad2, s32 pad3, s32 pad4);
s32 menuYesNo(s32 point, char *msg1, char *msg2);
void menuYesNoSetStr(char *str1, char *str2);
void menu_sDispSaveLoadMenuSet(s16 mode);
void menu_sDispSaveLoadMenu(void);
void menuZoidPartsCopy(s16 src_save, s16 src_slot, s16 dst_save, s16 dst_slot);
s16 menuSeartchPartsHave(s16 part);
s16 menuSeartchPartsZoid(s16 part, s16 slot);
s16 menuSeartchPartsAllZoid(s16 part);
void menuDelPartsHave(s16 part);
void ttljobjcopyposdispall(HSD_JObj *src, HSD_JObj *dst);
void MessageBox(char *msg);
s32 ttlMemCardChkStartNew(void);
s32 ttlMenuSystemCardLoad(void);
void menu_sDispSaveLoad(void);
void menu_sDispSaveLoadTitle(void);
void menu_sDispSaveLoadDataWindow(void);
void ttlMenuPurgeSaveLoad(void);
void ttlMenuLoadSaveLoad(void);
void ttlMenuKeyClr1P2P(s32 pad1, s32 pad2);
s32 ttlMenuBatSaveLoad(s32 mode);
void ttlMenuSaveDataChk(void);
void ttlMenuSaveDataSetFileName(s16 a, s16 b);
s32 ttlMenuSaveLoadexistenceData0(s32 mode);
s32 ttlMenuSaveLoadexistenceDataStartNew0(void);
s32 ttlMenuSystemMemCardSave(void);
s32 ttlMenuSystemMemCardSaveNoMSg(void);
s32 TeaMemCardChk(void);
void menu_sDispBattleEntry(void);
void menu_sDispBattleEntryPersonPartnerZoidPic(void);
void menu_sDispBattleEntryPersonPartner(void);
void menu_sDispBattleEntryZoidStatusKind(s32 group, s32 kind_index, s32 value);
void menu_sDispBattleEntryPilotZoidList(void);
void menu_sDispBattleEntryPersonListDisp(s16 idx, s16 y);
void menu_sDispBattleEntryZoidListDisp(s16 idx, s16 y);
void menu_sDispBattleEntryPersonPartnerZoidSelectFrame(void);
void menu_BattleSet_EntryPersonPartnerExit(void);
void ttlbatentzoidchange_stagedownup(s16 unmount_slot, s16 zoid_slot);
void menufncZoidBattle_BattleMenuReturn(void);
void menu_TITLE_dispEntry(s32 state);
void ttl_batent_detailsetparam(void);
void menu_sDispBattleSet(void);
void selectcursolanm(s16 model);
void menu_sDispBattleSetHavePartsList(void);
void menu_sDispBattleSetequipPartsList(void);
void menu_sDispBattleSetequipParts(void);
void menu_sDispBattleSetZoidHaveZoidList(void);
void menu_sDispBattleSetunfold(void);
void menu_sDispBattleSetZoidStatusKind(s32 index, s32 value);
void menu_sDispBattleSetLButton(void);
void menu_sDispBattleSetLButtonZoid(void);
void menu_sDispBattleSetLButtonWeapon(void);
void menu_sDispBattleSetLButtonOption(void);
void menu_sDispBattleSetLButtonSubParts(void);
void menu_sDispBattleSetLButtonReset(void);
void menu_sDispBattleSetLButtonExit(void);
void menu_sDispBattleSetLButtonEtc(void);
void menu_sDispBattleSetLButtonAnm(s32 k);
void menuZoidBattleSettingmenubuttonset(void);
void MessageBoxBatNoParts(void);
void MessageBoxBat(char *msg);
void ttl_batset_detailsetparam(void);
void menu_sDispBattleFarm(void);
void menu_sDispMsgWin(s16 index, s16 value);
void menu_sDispBattleFarmMsgWindowHelp(void);
void menu_sDispBattleFarmPersonZoidPartner(void);
void menu_sDispSetButton(void);
void menu_sDispBattleFarmTitle(void);
f32 rPadX(void);
void menu_sDispBattleFarmBack(void);
void eyeCamPos(void);
void menu_sDispBattleFarmWeapon(void);
s32 menufncZoidBattle_BattleMenu(void);
void menu_sDispBattleFarmStageDown(s32 dir);
s32 menu_sDispBattleStageDown(s32 mode);
void menuZoidBattleSetStage_fromnowstage(void);
void menuZoidBattleSetStage_PersonSelect(s32 mode);
void menuseon_up(void);
void menuseon_down(void);
void menuseon_left(void);
void menuseon_right(void);
s16 TEA_menufnc0(s16 *a, s16 *b, s16 n, s16 m, s32 f1, s32 f2);
s16 TEA_menufnc(s16 *a, s16 *b, s16 c, s16 d);
s16 TEA_menufnc2p0(s32 a, s32 b, s16 c, s16 d, s32 e, s32 f);
s16 TEA_menufnc2p(s32 a, s32 b, s16 c, s16 d);
void menuerasewait(s16 index);
void menu_TITLE_disp(s32 mode);
void menuSetTitleButton(s16 button);
void menuHiddenTitle(void);
void ttl_batfarm_msgsetparam(void);
void flPicturePrologue(void);
void flPictureEpilogue(void);
s32 flPictureDisp(char *name);
void flPictureDispParts(s32 part);
void menu_sDispBattleShop(void);
void menu_sDispBattleShopHaveListDsip(void);
void menu_sDispBattleMsgArea(void);
void menu_sDispBattleShopHaveBp(void);
void menu_sDispBattleShopZoidHaveZoidList(void);
void menu_sDispBattleShopLButtonZoid(void);
void menu_sDispBattleShopunfold(void);
void menuZoidBattleShopmenubuttonset(void);
void menu_sDispBattleShopLButton(void);
void menu_sDispBattleShopLButtonZoidWepOptSub(void);
void menu_sDispBattleShopLButtonWeapon(void);
void menu_sDispBattleShopLButtonOption(void);
void menu_sDispBattleShopLButtonSubParts(void);
void menu_sDispBattleShopLButtonAnm(s32 k);
void menu_sDispBattleShopLButtonMainAnm(s32 button);
void menufncShopyesno(s32 buy);
void shopDispParts(s32 slot);
void MessageBoxBatShopNoParts(void);
void MessageBoxBatShopNoBp(void);
void MessageBoxBatShop(char *msg);
void ttl_batshop_detailsetparam(void);
void menu_sDispBattleHumanEntry(void);
void menu_sDispBattleHumanEntryPersonPartnerZoidPic(void);
void menu_sDispBattleHumanEntryPersonPartner(void);
void menu_sDispBattleHumanEntryPilotZoidList(void);
void menu_sDispBattleHumanEntryPersonListDisp(s16 idx, s16 y);
void menu_sDispBattleHumanEntryPersonPartnerZoidSelectFrame(void);
void menu_BattleSet_HumanEntryPersonPartnerExit(void);
void ttl_batenthuman_detailsetparam(void);
void menu_sDispBattleMapInfo(u8 *info, u8 *flags, s32 sel, s32 anim, MenuArchive *obj, MenuArchive **arr);
void menufncZoidBattle_MapInfo(void);
void menufncZoidMission_MapInfo(void);
void loadMapInfo(void);
void menu_sDispBattleNextYokok2(void *data, MenuArchive *archive);
void ttlMenu_sDispBattleNextYokok(void);
void menu_sDispMisFarm(void);
s32 menufncMission_Menu(void);
void menuMissionGo(void);
void menu_TITLE_dispMis(s32 mode);
void MissionLoadInitData(void);
void ttl_misfarm_msgsetparam(void);
void menu_sDispMissionEntry(void);
void menu_sDispMissionEntryPilotZoidList(void);
void menu_sDispMisEntryPersonPartner(void);
void menu_MissionSet_EntryPersonPartnerExit(void);
void ttl_misent_detailsetparam(void);
void menu_sDispMissionDemo(void);
void ttl_demomenumsgset(s16 no);
void ttl_demoflagset(s16 fl, s16 dat);
void menu_sDispMissionDemoMsgCursor(void);
void menu_sDispMissionDemoMsgArea(void);
void menu_sDispMissionDemoBack(void);
void menu_sDispMissionDemoPerson(s16 no);
s16 menuDemoSelectMenu(s16 cur);
void menu_MissionDemo(void);
void menu_MissionDemoLoadScrFile(s16 no);
void hokyuu(s32 map);
s32 chk22(s32 map);
void ttlMenu_sDispMissionHokyu(void);
void menu_sDispStart(void);
void menu_sDispStartBG_Base(void);
void menu_StartScrollBigCoreBlock(void);
void menu_sDispStartPushStart(void);
void menu_sDispStartPushLogo(void);
void menu_sDispStartMainMenu(void);
void menu_sDispZoidBattleNewGameLoadGame(void);
void menu_sDispZoidBattleSingleTeamGame(void);
void menu_sDispZoidVSGame(void);
void menu_sDispZoidEmpireCountry(void);
void menu_sDispStartMsgWin(void);
void menu_sDispStartWhiteInWhiteOut(void);
void menu_BlackInWhite(void);
void menu_BlackInWhiteSet(void);
void menu_WhiteOutBlack(void);
s32 menufncZoidVs01(void);
void menuDispLimitBP(void);
void menu_sDispVsEntry(void);
void menu_sDispVsEntryLoadButton(void);
void menu_sDispVsFarmPlayer1IconEnt(void);
void menu_sDispVsFarmPlayer2IconEnt(void);
void menu_sDispVsEntryPersonPartnerZoidPic(void);
void menu_sDispVsEntryZoidStatusKind(s32 kind, s32 row, s32 value);
void menu_sDispVsEntryPilotZoidList(void);
void menu_sDispVsEntryPersonListDisp(s16 idx, s16 y);
void menu_sDispVsEntryZoidListDisp(s16 idx, s16 row);
void menu_sDispVsEntryPersonPartnerZoidSelectFrame(void);
void bpError(s32 pad);
void menu_TITLE_dispVsEntry(s32 mode);
s32 menufncZoidVsMenuEntry(void);
void ttl_vsent_detailsetparam(s16 side);
void ttl_vsent_setparamdispobjct(void);
void menu_sDispVsFarm(void);
void menu_VsFarmMapLoad(s16 map);
void menu_sDispVsFarmMapInfo(void);
void menu_sDispVsFarmEntryButton(void);
void menu_sDispVsFarmStageButton(void);
void menu_sDispVsFarmBattleButton(void);
void menu_sDispVsFarmExitButton(void);
void menu_sDispVsFarmPlayer1Icon(void);
void menu_sDispVsFarmPlayer2Icon(void);
s32 vsZoidBig1(s32 player);
s32 vsZoidBig(void);
s32 vsZoidStage(void);
s32 nextStage(void);
s32 backStage(void);
void vsMapMaxClear(void);
s32 menufncZoidVsMenu(void);
void menu_TITLE_dispVs(s32 state);
void menu_sDispVsFistFarm(void);
s32 menufncZoidVsFistMenu(void);
void menu_TITLE_dispVsFist(s32 mode);
void menuvs4dispanm_team(s32 player);
void menuvs4setanm_team(s32 player, s32 anim, s32 start, s32 end);
void menuvs4dispanm_cpu(s32 player);
void menuvs4setanm_cpu(s32 player, s32 anim, s32 start, s32 end);
void menuvs4dispanm_SelectFrame(void);
void menuvs4setanm_SelectFrame(s32 on);
void menu_sDispVs4pFarm(void);
void menu_sDispVs4CopyVs4(s32 player);
void menu_sDispVs4pPersonPic(s32 player);
void menu_sDispVs4pButtonTeam(s32 player);
void menu_sDispVs4pButtonCpu(s32 player);
void menu_sDispVs4pSubMenu(s32 index);
void menu_sDispVs4StatusKind(s32 kind, s32 row, s32 value);
void vs4_MenuSub(s32 pad);
void vs4_MenuZoid(s32 pad);
void vs4_MenuComplete(s32 pad);
void vs4_MenuSubCursolSet(s32 idx);
s32 menufncZoidVs4pMenu(void);
s32 menufncZoidVs4pLoadFile(void);
void menu_sDispVsFarmPlayerIcon(s32 player);
void menu_sDispVs4Complete(s32 player);
void vs4PersonDispSmall(s16 person, HSD_JObj *dst);
void vs4ZoidDispSmall(s16 zoid, HSD_JObj *src);
void vs4SelectFrame(HSD_JObj *dst);
void vs4DispSmallListFrame(s32 person);
void vs4DispSmallListFrameZoid(s32 pad);
void menuZoidvs4GoPartsOptSet_GameZoid(s16 player, s16 slot, s16 zoid);
s32 GetVsBpLimitTBL(void);
s32 GetVsBpLimit(void);
void configSet(void);
void configGet(void);
void configPut(void);
void configClear(void);
void configBak(void);
void configBakRet(void);
void voiceSn2i(void);
void voiceSn2a(s32 voice);
s32 voiceSn2c1(s32 kind);
s32 voiceSn2c(void);
void getSubMenuT(void);
void bgm2bgm(void);
void sub0(void);
void sub1(void);
void sub2(void);
void sub3(void);
void sub4(void);
void sub5(void);
void sub6(void);
void sub7(void);
void sub8(void);
void subMenu(void);
void dispBGM(MenuArchive *archive);
void dispVoice(MenuArchive *archive);
void dispSe(MenuArchive *archive);
void dispRumble(MenuArchive *archive);
void dispVsTime(MenuArchive *archive);
void dispVsBpLimit(MenuArchive *archive);
void dispVsMode(MenuArchive *archive);
s32 menu_sDispConfig(MenuArchive *archive, s32 flags);
void detSaveNoSet(s16 no);
void detCustm(s32 no);
void detZoid(s32 no);
void menu_sDispDetailSetting(f32 a, f32 b);
void menu_sDispDetailZoidn(f32 a, f32 b);
void detWeapon(s32 no);
void detOption(s32 no);
void menu_sDispDetailOption(f32 a_, f32 b_);
void detSubparts(s32 no);
void menu_sDispDetailSubparts(f32 a, f32 b);
void detPerson(s32 no);
void menu_sDispDetailPerson(f32 a_, f32 b_);
void menu_sDispDetailAll(void);
void eyeCamSet(void);
void GalleryKey(void);
void menu_sDispBattleGalleryExitAndListBt(void);
void menu_sDispBattleGalleryZoidList(void);
void menu_sDispGallery(void);
void stZoidA(ZoidState *zoid);
extern void ttl_SortZoid(s16 kind);
extern s32 anmN;
extern s32 csO;
extern s16 custmFist[32];
extern s16 custmVs[32];
extern u32 demoPicMan[395];
extern s32 detPersonn;
extern s32 detSaveNo;
extern s32 detZoidCstm;
extern s32 detZoidn;
extern s32 detZoidPerson;
extern Mtx dspMtx;
extern s32 dtMain[16];
extern f32 eyeCam[3];
extern s16 flag_AllHavePartsList;
extern s16 flag_AllHavePartsListold;
extern void *flPicture;
extern char flPictureF[200];
extern s32 flPictureT;
extern GameZoidEntry GameZoid[16];
extern s16 humanFist[60];
extern s16 humanVs[60];
extern GameSaveRecord IveSaveFlagBak;
extern f32 lookCam[3];
extern f32 lookCamY;
extern void *menu_back1p;
extern BatMapEntry menu_BatMapData[37];
extern void *menu_detail1p;
extern void *menu_personp[71];
extern void *menu_saveloadbgp;
extern s32 menu_saveloadiconp;
extern void *menu_saveloadp;
extern s32 menu_SlotDataOn[5];
extern s32 menu_SlotOn[2];
extern s16 flag_AllHavePartsListanm[3];
extern s64 gametimestart;
extern void ttlMenuOption(void);
extern s32 menu_StageUpDownFlag;
extern f32 menu_zoideyecam[3];
extern void *menu_zoidp[150];
extern void *menu1p;
extern f32 menubackcambak[3];
extern f32 menubackintbak[3];
extern f32 menubackintpos[3];
extern void *menudemo[4];
extern s16 menudemobg;
extern s16 menudemobgm;
extern s16 menudemoC;
extern s16 menudemocha[6];
extern s32 menudemocharain;
extern s16 menudemocur;
extern s32 menudemoend;
extern s16 menudemojmp;
extern s16 menudemokeywait;
extern s16 menudemomsg;
extern s16 menudemop;
extern s16 menudemoselect;
extern char menudemoselstr1[80];
extern char menudemoselstr2[80];
extern s32 MenuDemoSkipFlag;
extern s16 menudemovno;
extern SaveLoadWindow menudispslwindow[3];
extern char menudispwinstr[400];
extern u8 menufarmano[12];
extern u8 menufarmjfl[20];
extern u8 menufarmjno[12];
extern s32 menuMax[7];
extern s8 menumodeShop;
extern char menusaveloadcommentname[68];
extern char menusaveloadfilename[30];
extern s32 menusaveloadSlotButtonanm[3];
extern ButtonAnim menusetbtanm[12];
extern s16 menusetmh[20];
extern s16 menusetmn[20];
extern s16 menusetsl[20];
extern s16 menusetst[20];
extern TitleAnim menutitleanm[20];
extern s16 menutitleguide[8];
extern s16 menuwepdispmount[28];
extern char menuyesnoshopmsg[80];
extern u8 menuZoidMountPointDsip[28];
extern f32 menuzoidry;
extern s32 rumble93;
extern SampleVoice samplevoiceno[100];
extern f32 shininess;
extern s32 stage00;
extern s32 Tea_bulletdat[2489];
extern s16 Tea_demomenu[11];
extern u8 Tea_demomsg[2200];
extern s16 Tea_effectdat[1781];
extern PartDataEntry Tea_partsdat[451];
extern PersonDataEntry Tea_persondat[72];
extern ZoidDataEntry Tea_zoiddat[151];
extern char *tst999;
extern HSD_LObj *ttlLight;
extern u8 TXT_Menu_dat[15420];
extern s32 voiceSn[5][20];
extern s32 voiceSn2[20];
extern s32 VolBgm3;
extern s32 VolBgmTBL[11];
extern s32 VolSe3;
extern s32 VolSeTBL[11];
extern s32 VolVoice3;
extern s32 VolVoiceTBL[11];
extern s8 vsman;
extern s32 vsMapMax;
extern s32 VsMode3;
extern s32 VsTime3;
extern u32 VsTimeTBL[5];
extern void *yokoSeTBL;
extern char *strcat(char *destination, const char *source);
#define ZF(z, T, off) (*(T *)((u8 *)(z) + (off)))
typedef struct ZoidStateV2 {
    u8 data[0x1F8C];
} ZoidStateV2;
extern s32 menudemoanmend[2];
extern void ttlMenuLoadYesNo(void);
extern s32 menu_MissionDemoScr_MenuAfter_SetMsg1(void);
extern s16 menudemofase[2];
extern s32 menudemoseldat;
extern s32 menudemoselflg;
extern s16 menudemoseljmp[2];
extern s16 menudemoselmsg[2];
extern void HSD_StateInvalidate(s32 state);
typedef struct VoiceEntry {
    u8 flags;
    s8 kind;
    u16 value;
    s8 choices[4];
} VoiceEntry;
extern void camVoiceNow(s32 a, s32 b);
extern void zdVoiceScript(ZoidState *zoid);
extern void *binB(void *bin, s32 index);
extern s32 zdVoiceScriptCalc(VoiceEntry *entry, ZoidState *zoid, s32 a, s32 b);
#pragma force_active on
static s32 dtMain[16];
static Mtx dspMtx;
void *menu_zoidp[150];
void *menu_personp[71];
GameZoidEntry GameZoid[16];
GameSaveRecord IveSave[9];
GameSaveRecord IveSaveFlag;
GameSaveRecord IveSaveFlagBak;
ZoidDataEntry Tea_zoiddat[151];
PartDataEntry Tea_partsdat[451];
s32 Tea_bulletdat[2489];
PersonDataEntry Tea_persondat[72];
s16 Tea_effectdat[1781];
u8 TXT_Menu_dat[15420];
void *menudemo[4];
u8 menuZoidMountPointDsip[28];
s16 menuwepdispmount[28];
char menudispyesnostr1[180];
char menudispyesnostr2[180];
SaveLoadWindow menudispslwindow[3];
char menudispwinstr[400];
s16 menutitleguide[8];
TitleAnim menutitleanm[20];
s16 menuSLPos[5];
s32 menu_SlotDataOn[5];
s32 menusaveloadSlotButtonanm[3];
char menusaveloadfilename[30];
char menusaveloadcommentname[68];
u8 menufarmanm[20];
u8 menumodeanm[20];
u8 menufarmanmold[20];
ButtonAnim menubackanm[20];
s16 menufarmmh[20];
s16 menufarmmn[20];
s16 menufarmst[20];
s16 menufarmsl[20];
s16 menufarmcl[20];
s16 menusetst[20];
s16 menusetsl[20];
s16 menusetmh[20];
ButtonAnim menusetbtanm[12];
s16 menusetcl[20];
s16 menusetmn[20];
static f32 lookCam[3];
u8 menufarmjfl[20];
f32 menu_zoideyecam[3];
static char flPictureF[200];
char menuyesnoshopmsg[80];
char menudemoselstr1[80];
char menudemoselstr2[80];
u8 Tea_demomsg[2200];
s16 menudemocha[6];
s16 Tea_demomenu[11];
static s32 voiceSn2[20];
u32 TXT_Menu[1178] = {
    0x00000000, 0x0000002E, 0x0000005C, 0x0000008A, 0x000000B8, 0x00000140, 0x000001C8, 0x000001F6, 
    0x00000224, 0x00000252, 0x00000280, 0x000002AE, 0x000002B2, 0x0000033A, 0x000003C2, 0x000003C6, 
    0x000003CA, 0x000003CE, 0x000003D2, 0x000003D6, 0x000003DA, 0x000003DE, 0x000003E2, 0x000003E6, 
    0x000003EA, 0x000003EE, 0x00000418, 0x00000442, 0x0000046C, 0x00000496, 0x000004C0, 0x000004EA, 
    0x000004EE, 0x00000518, 0x0000051C, 0x00000520, 0x00000524, 0x00000528, 0x0000052C, 0x00000530, 
    0x00000534, 0x00000538, 0x000005C0, 0x000005C4, 0x000005C8, 0x000005CC, 0x000005D0, 0x000005D4, 
    0x000005D8, 0x000005DC, 0x0000065A, 0x000006D8, 0x00000756, 0x000007D4, 0x00000852, 0x000008D0, 
    0x0000094E, 0x00000952, 0x000009D0, 0x00000A4E, 0x00000ACC, 0x00000B4A, 0x00000BC8, 0x00000C46, 
    0x00000CC4, 0x00000D42, 0x00000DC0, 0x00000E3E, 0x00000E42, 0x00000EC0, 0x00000F3E, 0x00000FBC, 
    0x00000FC0, 0x00001048, 0x000010D0, 0x00001158, 0x000011E0, 0x00001268, 0x000012F0, 0x00001378, 
    0x00001400, 0x00001488, 0x0000148C, 0x00001490, 0x00001494, 0x0000151C, 0x000015A4, 0x0000162C, 
    0x000016B4, 0x00001732, 0x00001736, 0x0000173A, 0x0000173E, 0x00001742, 0x00001746, 0x0000174A, 
    0x0000174E, 0x00001750, 0x00001754, 0x00001758, 0x0000175E, 0x000017E6, 0x0000186E, 0x000018F6, 
    0x0000197E, 0x00001A06, 0x00001A8E, 0x00001B16, 0x00001B9E, 0x00001C26, 0x00001CAE, 0x00001D2C, 
    0x00001DAA, 0x00001E28, 0x00001EB0, 0x00001EDE, 0x00001F0C, 0x00001F3E, 0x00001F43, 0x00001F48, 
    0x00001F4D, 0x00001F52, 0x00001F57, 0x00001F5C, 0x00001F61, 0x00001F66, 0x00001F6B, 0x00001F70, 
    0x00001F75, 0x00001F7A, 0x00001F7F, 0x00001F84, 0x00001F89, 0x00001F8E, 0x00001F93, 0x00001F98, 
    0x00001F9D, 0x00001FA2, 0x00001FA7, 0x00001FAC, 0x00001FB1, 0x00001FB6, 0x00001FBB, 0x00001FC0, 
    0x00001FC5, 0x00001FCA, 0x00001FCF, 0x00001FD4, 0x00001FD9, 0x00001FDE, 0x00001FE3, 0x00001FE8, 
    0x00001FED, 0x00001FF2, 0x00001FF7, 0x00001FFC, 0x00002001, 0x00002006, 0x0000200B, 0x00002010, 
    0x00002015, 0x0000201A, 0x0000201F, 0x00002024, 0x00002029, 0x0000202E, 0x00002033, 0x00002038, 
    0x0000203D, 0x00002042, 0x00002047, 0x0000204C, 0x00002051, 0x00002056, 0x0000205B, 0x00002060, 
    0x00002065, 0x0000206A, 0x0000206F, 0x00002074, 0x00002079, 0x0000207E, 0x00002083, 0x00002088, 
    0x0000208D, 0x00002092, 0x00002097, 0x0000209C, 0x000020A1, 0x000020A6, 0x000020AB, 0x000020B0, 
    0x000020B5, 0x000020BA, 0x000020BF, 0x000020C4, 0x000020C9, 0x000020CE, 0x000020D3, 0x000020D8, 
    0x000020DD, 0x000020E2, 0x000020E7, 0x000020EC, 0x000020F1, 0x000020F6, 0x000020FB, 0x00002100, 
    0x00002105, 0x0000210A, 0x0000210F, 0x00002114, 0x00002119, 0x0000211E, 0x00002123, 0x00002128, 
    0x0000212D, 0x00002132, 0x00002137, 0x0000213C, 0x00002141, 0x00002146, 0x0000214B, 0x00002150, 
    0x00002155, 0x0000215A, 0x0000215F, 0x00002164, 0x00002169, 0x0000216E, 0x00002173, 0x00002178, 
    0x0000217D, 0x00002182, 0x00002187, 0x0000218C, 0x00002191, 0x00002196, 0x0000219B, 0x000021A0, 
    0x000021A5, 0x000021AA, 0x000021AF, 0x000021B4, 0x000021B9, 0x000021BE, 0x000021C3, 0x000021C8, 
    0x000021CD, 0x000021D2, 0x000021D7, 0x000021DC, 0x000021E1, 0x000021E6, 0x000021EB, 0x000021F0, 
    0x000021F5, 0x000021FA, 0x000021FF, 0x00002204, 0x00002209, 0x0000220E, 0x00002213, 0x00002218, 
    0x0000221D, 0x00002222, 0x00002227, 0x0000222C, 0x00002231, 0x00002236, 0x0000223B, 0x00002240, 
    0x00002245, 0x0000224A, 0x0000224F, 0x00002254, 0x00002259, 0x0000225E, 0x00002263, 0x00002268, 
    0x0000226D, 0x00002272, 0x00002277, 0x0000227C, 0x00002281, 0x00002286, 0x0000228B, 0x00002290, 
    0x00002295, 0x0000229A, 0x0000229F, 0x000022A4, 0x000022A9, 0x000022AE, 0x000022B3, 0x000022B8, 
    0x000022BD, 0x000022C2, 0x000022C7, 0x000022CC, 0x000022D1, 0x000022D6, 0x000022DB, 0x000022E0, 
    0x000022E5, 0x000022EA, 0x000022EF, 0x000022F4, 0x000022F9, 0x000022FE, 0x00002303, 0x00002308, 
    0x0000230D, 0x00002312, 0x00002317, 0x0000231C, 0x00002321, 0x00002326, 0x0000232B, 0x00002330, 
    0x00002335, 0x0000233A, 0x0000233F, 0x00002344, 0x00002349, 0x0000234E, 0x00002353, 0x00002358, 
    0x0000235D, 0x00002362, 0x00002367, 0x0000236C, 0x00002371, 0x00002376, 0x0000237B, 0x00002380, 
    0x00002385, 0x0000238A, 0x0000238F, 0x00002394, 0x00002399, 0x0000239E, 0x000023A3, 0x000023A8, 
    0x000023AD, 0x000023B2, 0x000023B7, 0x000023BC, 0x000023C1, 0x000023C6, 0x000023CB, 0x000023D0, 
    0x000023D5, 0x000023DA, 0x000023DF, 0x000023E4, 0x000023E9, 0x000023EE, 0x000023F3, 0x000023F8, 
    0x000023FD, 0x00002402, 0x00002407, 0x0000240C, 0x00002411, 0x00002416, 0x0000241B, 0x00002420, 
    0x00002425, 0x0000242A, 0x0000242F, 0x00002434, 0x00002439, 0x0000243E, 0x00002443, 0x00002448, 
    0x0000244D, 0x00002452, 0x00002457, 0x0000245C, 0x00002461, 0x00002466, 0x0000246B, 0x00002470, 
    0x00002475, 0x0000247A, 0x0000247F, 0x00002484, 0x00002489, 0x0000248E, 0x00002493, 0x00002498, 
    0x0000249D, 0x000024A2, 0x000024A7, 0x000024AC, 0x000024B1, 0x000024B6, 0x000024BB, 0x000024C0, 
    0x000024C5, 0x000024CA, 0x000024CF, 0x000024D4, 0x000024D9, 0x000024DE, 0x000024E3, 0x000024E8, 
    0x000024ED, 0x000024F2, 0x000024F7, 0x000024FC, 0x00002501, 0x00002506, 0x0000250B, 0x00002510, 
    0x00002515, 0x0000251A, 0x0000251F, 0x00002524, 0x00002529, 0x0000252E, 0x00002533, 0x00002538, 
    0x0000253D, 0x00002542, 0x00002547, 0x0000254C, 0x00002551, 0x00002556, 0x0000255B, 0x00002560, 
    0x00002565, 0x0000256A, 0x0000256F, 0x00002574, 0x00002579, 0x0000257E, 0x00002583, 0x00002588, 
    0x0000258D, 0x00002592, 0x00002597, 0x0000259C, 0x000025A1, 0x000025A6, 0x000025AB, 0x000025B0, 
    0x000025B5, 0x000025BA, 0x000025BF, 0x000025C4, 0x000025C9, 0x000025CE, 0x000025D3, 0x000025D8, 
    0x000025DD, 0x000025E2, 0x000025E7, 0x000025EC, 0x000025F1, 0x000025F6, 0x000025FB, 0x00002600, 
    0x00002605, 0x0000260A, 0x0000260F, 0x00002614, 0x00002619, 0x0000261E, 0x00002623, 0x00002628, 
    0x0000262D, 0x00002632, 0x00002637, 0x0000263C, 0x00002641, 0x00002646, 0x0000264B, 0x00002650, 
    0x00002655, 0x0000265A, 0x0000265F, 0x00002664, 0x00002669, 0x0000266E, 0x00002673, 0x00002678, 
    0x0000267D, 0x00002682, 0x00002687, 0x0000268C, 0x00002691, 0x00002696, 0x0000269B, 0x000026A0, 
    0x000026A5, 0x000026AA, 0x000026AF, 0x000026B4, 0x000026B9, 0x000026BE, 0x000026C3, 0x000026C8, 
    0x000026CD, 0x000026D2, 0x000026D7, 0x000026DC, 0x000026E1, 0x000026E6, 0x000026EB, 0x000026F0, 
    0x000026F5, 0x000026FA, 0x000026FF, 0x00002704, 0x00002709, 0x0000270E, 0x00002713, 0x00002718, 
    0x0000271D, 0x00002722, 0x00002727, 0x0000272C, 0x00002731, 0x00002736, 0x0000273B, 0x00002740, 
    0x00002745, 0x0000274A, 0x0000274F, 0x00002754, 0x00002759, 0x0000275E, 0x00002763, 0x00002768, 
    0x0000276D, 0x00002772, 0x00002777, 0x0000277C, 0x00002781, 0x00002786, 0x0000278B, 0x00002790, 
    0x00002795, 0x0000279A, 0x0000279F, 0x000027A4, 0x000027A9, 0x000027AE, 0x000027B3, 0x000027B8, 
    0x000027BD, 0x000027C2, 0x000027C7, 0x000027CC, 0x000027D1, 0x000027D6, 0x000027DB, 0x000027E0, 
    0x000027E5, 0x000027EA, 0x000027EF, 0x000027F4, 0x000027F9, 0x000027FE, 0x00002803, 0x00002808, 
    0x0000280D, 0x00002812, 0x00002817, 0x0000281C, 0x00002821, 0x00002826, 0x0000282B, 0x00002830, 
    0x00002835, 0x0000283A, 0x0000283F, 0x00002844, 0x00002849, 0x0000284E, 0x00002853, 0x00002858, 
    0x0000285D, 0x00002862, 0x00002867, 0x0000286C, 0x00002871, 0x00002876, 0x0000287B, 0x00002880, 
    0x00002885, 0x0000288A, 0x0000288F, 0x00002894, 0x00002899, 0x0000289E, 0x000028A3, 0x000028A8, 
    0x000028AD, 0x000028B2, 0x000028B7, 0x000028BC, 0x000028C1, 0x000028C6, 0x000028CB, 0x000028D0, 
    0x000028D5, 0x000028DA, 0x000028DF, 0x000028E4, 0x000028E9, 0x000028EE, 0x000028F3, 0x000028F8, 
    0x000028FD, 0x00002902, 0x00002907, 0x0000290C, 0x00002911, 0x00002916, 0x0000291B, 0x00002920, 
    0x00002925, 0x0000292A, 0x0000292F, 0x00002934, 0x00002939, 0x0000293E, 0x00002943, 0x00002948, 
    0x0000294D, 0x00002952, 0x00002957, 0x0000295C, 0x00002961, 0x00002966, 0x0000296B, 0x00002970, 
    0x00002975, 0x0000297A, 0x0000297F, 0x00002984, 0x00002989, 0x0000298E, 0x00002993, 0x00002998, 
    0x0000299D, 0x000029A2, 0x000029A7, 0x000029AC, 0x000029B1, 0x000029B6, 0x000029BB, 0x000029C0, 
    0x000029C5, 0x000029CA, 0x000029CF, 0x000029D4, 0x000029D9, 0x000029DE, 0x000029E3, 0x000029E8, 
    0x000029ED, 0x000029F2, 0x000029F7, 0x000029FC, 0x00002A01, 0x00002A06, 0x00002A0B, 0x00002A10, 
    0x00002A15, 0x00002A1A, 0x00002A1F, 0x00002A24, 0x00002A29, 0x00002A2E, 0x00002A33, 0x00002A38, 
    0x00002A3D, 0x00002A42, 0x00002A47, 0x00002A4C, 0x00002A51, 0x00002A56, 0x00002A5B, 0x00002A60, 
    0x00002A65, 0x00002A6A, 0x00002A6F, 0x00002A74, 0x00002A79, 0x00002A7E, 0x00002A83, 0x00002A88, 
    0x00002A8D, 0x00002A92, 0x00002A97, 0x00002A9C, 0x00002AA1, 0x00002AA6, 0x00002AAB, 0x00002AB0, 
    0x00002AB5, 0x00002ABA, 0x00002ABF, 0x00002AC4, 0x00002AC9, 0x00002ACE, 0x00002AD3, 0x00002AD8, 
    0x00002ADD, 0x00002AE2, 0x00002AE7, 0x00002AEC, 0x00002AF1, 0x00002AF6, 0x00002AFB, 0x00002B00, 
    0x00002B05, 0x00002B0A, 0x00002B0F, 0x00002B14, 0x00002B19, 0x00002B1E, 0x00002B23, 0x00002B28, 
    0x00002B2D, 0x00002B32, 0x00002B37, 0x00002B3C, 0x00002B41, 0x00002B46, 0x00002B4B, 0x00002B50, 
    0x00002B55, 0x00002B5A, 0x00002B5F, 0x00002B64, 0x00002B69, 0x00002B6E, 0x00002B73, 0x00002B78, 
    0x00002B7D, 0x00002B82, 0x00002B87, 0x00002B8C, 0x00002B91, 0x00002B96, 0x00002B9B, 0x00002BA0, 
    0x00002BA5, 0x00002BAA, 0x00002BAF, 0x00002BB4, 0x00002BB9, 0x00002BBE, 0x00002BC3, 0x00002BC8, 
    0x00002BCD, 0x00002BD2, 0x00002BD7, 0x00002BDC, 0x00002BE1, 0x00002BE6, 0x00002BEB, 0x00002BF0, 
    0x00002BF5, 0x00002BFA, 0x00002BFF, 0x00002C04, 0x00002C09, 0x00002C0E, 0x00002C13, 0x00002C18, 
    0x00002C1D, 0x00002C22, 0x00002C27, 0x00002C2C, 0x00002C31, 0x00002C36, 0x00002C3B, 0x00002C40, 
    0x00002C45, 0x00002C4A, 0x00002C4F, 0x00002C54, 0x00002C59, 0x00002C5E, 0x00002C63, 0x00002C68, 
    0x00002C6D, 0x00002C72, 0x00002C77, 0x00002C7C, 0x00002C81, 0x00002C86, 0x00002C8B, 0x00002C90, 
    0x00002C95, 0x00002C9A, 0x00002C9F, 0x00002CA4, 0x00002CA9, 0x00002CAE, 0x00002CB3, 0x00002CB8, 
    0x00002CBD, 0x00002CC2, 0x00002CC7, 0x00002CCC, 0x00002CD1, 0x00002CD6, 0x00002CDB, 0x00002CE0, 
    0x00002CE5, 0x00002CEA, 0x00002CEF, 0x00002CF4, 0x00002CF9, 0x00002CFE, 0x00002D03, 0x00002D08, 
    0x00002D0D, 0x00002D12, 0x00002D17, 0x00002D1C, 0x00002D21, 0x00002D26, 0x00002D2B, 0x00002D30, 
    0x00002D35, 0x00002D3A, 0x00002D3F, 0x00002D44, 0x00002D49, 0x00002D4E, 0x00002D53, 0x00002D58, 
    0x00002D5D, 0x00002D62, 0x00002D67, 0x00002D6C, 0x00002D71, 0x00002D76, 0x00002D7B, 0x00002D80, 
    0x00002D85, 0x00002D8A, 0x00002D8F, 0x00002D94, 0x00002D99, 0x00002D9E, 0x00002DA3, 0x00002DA8, 
    0x00002DAD, 0x00002DB2, 0x00002DB7, 0x00002DBC, 0x00002DC1, 0x00002DC6, 0x00002DCB, 0x00002DD0, 
    0x00002DD5, 0x00002DDA, 0x00002DDF, 0x00002DE4, 0x00002DE9, 0x00002DEE, 0x00002DF3, 0x00002DF8, 
    0x00002DFD, 0x00002E02, 0x00002E07, 0x00002E0C, 0x00002E11, 0x00002E16, 0x00002E1B, 0x00002E20, 
    0x00002E25, 0x00002E2A, 0x00002E2F, 0x00002E34, 0x00002E39, 0x00002E3E, 0x00002E43, 0x00002E48, 
    0x00002E4D, 0x00002E52, 0x00002E57, 0x00002E5C, 0x00002E61, 0x00002E66, 0x00002E6B, 0x00002E70, 
    0x00002E75, 0x00002E7A, 0x00002E7F, 0x00002E84, 0x00002E89, 0x00002E8E, 0x00002E93, 0x00002E98, 
    0x00002E9D, 0x00002EA2, 0x00002EA7, 0x00002EAC, 0x00002EB1, 0x00002EB6, 0x00002EBB, 0x00002EC0, 
    0x00002EC5, 0x00002ECA, 0x00002ECF, 0x00002ED4, 0x00002ED9, 0x00002EDE, 0x00002EE3, 0x00002EE8, 
    0x00002EED, 0x00002EF2, 0x00002EF7, 0x00002EFC, 0x00002F01, 0x00002F06, 0x00002F0B, 0x00002F10, 
    0x00002F15, 0x00002F1A, 0x00002F1F, 0x00002F24, 0x00002F29, 0x00002F2E, 0x00002F33, 0x00002F38, 
    0x00002F3D, 0x00002F42, 0x00002F47, 0x00002F4C, 0x00002F51, 0x00002F56, 0x00002F5B, 0x00002F60, 
    0x00002F65, 0x00002F6A, 0x00002F6F, 0x00002F74, 0x00002F79, 0x00002F7E, 0x00002F83, 0x00002F88, 
    0x00002F8D, 0x00002F92, 0x00002F97, 0x00002F9C, 0x00002FA1, 0x00002FA6, 0x00002FAB, 0x00002FB0, 
    0x00002FB5, 0x00002FBA, 0x00002FBF, 0x00002FC4, 0x00002FC9, 0x00002FCE, 0x00002FD3, 0x00002FD8, 
    0x00002FDD, 0x00002FE2, 0x00002FE7, 0x00002FEC, 0x00002FF1, 0x00002FF6, 0x00002FFB, 0x00003000, 
    0x00003005, 0x0000300A, 0x0000300F, 0x00003014, 0x00003019, 0x0000301E, 0x00003023, 0x00003028, 
    0x0000302D, 0x00003032, 0x00003037, 0x0000303C, 0x00003041, 0x00003046, 0x0000304B, 0x00003050, 
    0x00003055, 0x0000305A, 0x0000305F, 0x00003064, 0x00003069, 0x0000306E, 0x00003073, 0x00003078, 
    0x0000307D, 0x00003083, 0x00003089, 0x0000308F, 0x00003095, 0x0000309B, 0x000030A1, 0x000030A7, 
    0x000030AD, 0x000030B3, 0x000030B9, 0x000030BF, 0x000030C5, 0x000030CB, 0x000030D1, 0x000030D7, 
    0x000030DD, 0x000030E3, 0x000030E9, 0x000030EF, 0x000030F5, 0x000030FB, 0x00003101, 0x00003107, 
    0x0000310D, 0x00003113, 0x00003119, 0x0000311F, 0x00003125, 0x0000312B, 0x00003131, 0x00003137, 
    0x0000313D, 0x00003143, 0x00003149, 0x0000314F, 0x00003155, 0x0000315B, 0x00003161, 0x00003167, 
    0x0000316D, 0x00003173, 0x00003179, 0x0000317F, 0x00003185, 0x0000318B, 0x00003191, 0x00003197, 
    0x0000319D, 0x000031A3, 0x000031A9, 0x000031AF, 0x000031B5, 0x000031BB, 0x000031C1, 0x000031C7, 
    0x000031CD, 0x000031D3, 0x000031D9, 0x000031DF, 0x000031E5, 0x000031EB, 0x000031F1, 0x000031F7, 
    0x000031FD, 0x00003203, 0x00003209, 0x0000320F, 0x00003215, 0x0000321B, 0x00003221, 0x00003227, 
    0x0000322D, 0x00003233, 0x00003239, 0x0000323F, 0x00003245, 0x0000324B, 0x00003251, 0x000032D9, 
    0x00003361, 0x00003367, 0x0000336D, 0x00003373, 0x00003379, 0x0000337F, 0x00003385, 0x0000338B, 
    0x00003391, 0x00003397, 0x0000339D, 0x000033A3, 0x000033A9, 0x000033AF, 0x000033B5, 0x000033BB, 
    0x000033C1, 0x000033C7, 0x000033CD, 0x000033D3, 0x000033D9, 0x000033DF, 0x000033E5, 0x000033EB, 
    0x000033F1, 0x000033F7, 0x000033FD, 0x00003403, 0x00003409, 0x0000340F, 0x00003415, 0x0000341B, 
    0x00003421, 0x00003427, 0x0000342D, 0x00003433, 0x00003439, 0x0000343F, 0x00003445, 0x0000344B, 
    0x00003451, 0x00003457, 0x0000345D, 0x00003463, 0x00003469, 0x0000346F, 0x00003475, 0x0000347B, 
    0x00003481, 0x00003487, 0x0000348D, 0x00003493, 0x00003499, 0x0000349F, 0x00003527, 0x000035AF, 
    0x00003637, 0x0000363D, 0x000036C5, 0x0000374D, 0x00003753, 0x00003759, 0x0000375F, 0x000037E7, 
    0x0000386F, 0x000038F7, 0x000038FD, 0x00003903, 0x0000398B, 0x00003A13, 0x00003A9B, 0x00003AA1, 
    0x00003B29, 0x00003BB1, 0x00003BB7, 0x00003BBD, 0x00003BC3, 0x00003BC9, 0x00003BCF, 0x00003BD5, 
    0x00003BDB, 0x00003BE1, 0x00003BE7, 0x00003BED, 0x00003BF3, 0x00003BF9, 0x00003BFF, 0x00003C05, 
    0x00003C0B, 0x00003C11, 0x00003C17, 0x00003C1D, 0x00003C23, 0x00003C29, 0x00003C2F, 0x00003C35, 
    0x00003C3B, 0x00003C3B, 
};
BatMapEntry menu_BatMapData[37] = {
    65, 1670, 1, 8, 22, 28, 11, 28, 
    82, 1670, 1, 8, 22, 28, 11, 28, 
    72, 1670, 1, 8, 22, 28, 11, 28, 
    83, 1667, 2, 1, 9, 14, 10, 14, 
    0, 1667, 2, 1, 9, 14, 10, 14, 
    75, 1667, 2, 1, 9, 14, 10, 14, 
    84, 1675, 3, 12, 1, 25, 7, 25, 
    85, 1675, 3, 12, 1, 25, 7, 25, 
    10, 1675, 3, 12, 1, 25, 7, 25, 
    2, 1683, 4, 2, 16, 8, 17, 1, 
    18, 1683, 4, 2, 16, 8, 17, 1, 
    6, 1683, 4, 2, 16, 8, 17, 1, 
    86, 1679, 5, 9, 21, 20, 19, 18, 
    87, 1679, 5, 9, 21, 20, 19, 18, 
    88, 1679, 5, 9, 21, 20, 19, 18, 
    89, 1665, 6, 10, 0, 10, 2, 9, 
    1, 1665, 6, 10, 0, 10, 2, 9, 
    76, 1665, 6, 10, 0, 10, 2, 9, 
    66, 1669, 7, 5, 40, 22, -1, -1, 
    17, 1669, 7, 5, 40, 22, -1, -1, 
    54, 1669, 7, 5, 40, 22, -1, -1, 
    78, 1666, 8, 12, 3, 31, 8, 24, 
    22, 1666, 8, 12, 3, 31, 8, 24, 
    11, 1666, 8, 12, 3, 31, 8, 24, 
    90, 1678, 9, 17, 27, 13, 28, 32, 
    91, 1678, 9, 17, 27, 13, 28, 32, 
    92, 1678, 9, 17, 27, 13, 28, 32, 
    53, 1674, 10, 3, 4, 5, 14, 19, 
    93, 1674, 10, 3, 4, 5, 14, 19, 
    23, 1674, 10, 3, 4, 5, 14, 19, 
    73, 1688, 11, 4, 31, 7, 33, 17, 
    94, 1688, 11, 4, 31, 7, 33, 17, 
    95, 1688, 11, 4, 31, 7, 33, 17, 
    96, 1684, 12, 11, 26, 21, -1, -1, 
    97, 1684, 12, 11, 26, 21, -1, -1, 
    98, 1684, 12, 11, 26, 21, -1, -1, 
    99, 1669, 12, 2, 40, 22, -1, -1, 
};
BatMapEntry menu_MisMapData[60] = {
    65, 1683, 1, 19, 16, 28, 17, 1, 
    83, 1670, 2, 11, 19, 28, 11, 28, 
    84, 1675, 3, 3, 24, 33, 25, 4, 
    2, 1665, 4, 6, 0, 10, 2, 1, 
    86, 1676, 5, 7, 22, 28, -1, -1, 
    89, 1666, 6, 7, 3, 0, 5, 14, 
    66, 1683, 7, 8, 16, 8, 23, 2, 
    78, 1692, 8, 8, 22, 33, 18, 15, 
    90, 1667, 9, 9, 5, 14, 17, 14, 
    53, 1691, 10, 9, 37, 32, 38, 15, 
    65, 1683, 11, 2, 16, 28, 17, 1, 
    83, 1670, 12, 17, 19, 28, 11, 28, 
    84, 1675, 13, 10, 24, 33, 25, 4, 
    2, 1665, 14, 14, 0, 10, 2, 1, 
    86, 1676, 15, 15, 22, 28, -1, -1, 
    89, 1666, 16, 8, 3, 0, 5, 14, 
    66, 1683, 17, 9, 16, 8, 23, 2, 
    78, 1692, 18, 9, 22, 33, 18, 15, 
    90, 1667, 19, 9, 5, 14, 17, 14, 
    53, 1691, 20, 14, 37, 32, 38, 15, 
    65, 1683, 21, 12, 16, 28, 17, 1, 
    83, 1670, 22, 2, 19, 28, 11, 28, 
    84, 1675, 23, 4, 24, 33, 25, 4, 
    2, 1665, 24, 9, 0, 10, 2, 1, 
    86, 1676, 25, 5, 22, 28, -1, -1, 
    89, 1666, 26, 14, 3, 0, 5, 14, 
    66, 1683, 27, 18, 16, 8, 23, 2, 
    78, 1692, 28, 13, 22, 33, 18, 15, 
    90, 1667, 29, 13, 5, 14, 17, 14, 
    53, 1691, 30, 5, 37, 32, 38, 15, 
    65, 1683, 31, 14, 16, 28, 17, 1, 
    83, 1670, 32, 14, 19, 28, 11, 28, 
    84, 1675, 33, 15, 24, 33, 25, 4, 
    2, 1665, 34, 1, 0, 10, 2, 1, 
    86, 1676, 35, 6, 22, 28, -1, -1, 
    89, 1666, 36, 9, 3, 0, 5, 14, 
    99, 1669, 37, 9, 40, 22, -1, -1, 
    89, 1666, 38, 0, 3, 0, 5, 14, 
    89, 1666, 39, 0, 3, 0, 5, 14, 
    89, 1666, 40, 9, 3, 0, 5, 14, 
    89, 1666, 41, 0, 3, 0, 5, 14, 
    89, 1666, 42, 0, 3, 0, 5, 14, 
    89, 1666, 43, 9, 3, 0, 5, 14, 
    89, 1666, 44, 7, 3, 0, 5, 14, 
    89, 1666, 45, 7, 3, 0, 5, 14, 
    89, 1666, 46, 7, 3, 0, 5, 14, 
    89, 1666, 47, 7, 3, 0, 5, 14, 
    89, 1666, 48, 8, 3, 0, 5, 14, 
    89, 1666, 49, 9, 3, 0, 5, 14, 
    89, 1666, 50, 0, 3, 0, 5, 14, 
    89, 1666, 51, 9, 3, 0, 5, 14, 
    89, 1666, 52, 0, 3, 0, 5, 14, 
    89, 1666, 53, 0, 3, 0, 5, 14, 
    89, 1666, 54, 0, 3, 0, 5, 14, 
    89, 1666, 55, 0, 3, 0, 5, 14, 
    89, 1666, 56, 0, 3, 0, 5, 14, 
    89, 1666, 57, 0, 3, 0, 5, 14, 
    89, 1666, 58, 0, 3, 0, 5, 14, 
    89, 1666, 59, 0, 3, 0, 5, 14, 
    99, 1669, 60, 0, 40, 22, -1, -1, 
};
static u32 infinite_light_pos_wobjdesc[5] = {
    0x00000000, 0x00000000, 0x447A0000, 0x447A0000, 0x00000000, 
};
static u32 infinite_light[7] = {
    0x00000000, 0x00000000, 0x000D0000, 0xFFFFFFFF, 0x80129E08, 0x00000000, 0x802EF284, 
};
static u32 menustrMain_763[28] = {
    0x8011C050, 0x00000002, 0x00000064, 0x00000000, 0x8011C05C, 0x00000002, 0x000000C8, 0x00000000, 
    0x8011C070, 0x00000002, 0x0000012C, 0x00000000, 0x8011C084, 0x00000002, 0x00000190, 0x00000000, 
    0x8011C098, 0x00000002, 0x000001F4, 0x00000000, 0x8011C0A4, 0x00000002, 0x00002328, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
static u32 menustrMain_794[12] = {
    0x8011C118, 0x00000002, 0x00000067, 0x00000000, 0x802F131C, 0x00000002, 0x00000068, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
u8 menufarmjno[12] = {
    0, 2, 2, 2, 2, 2, 2, 2, 
    8, 8, 0xFF, 0xFF, 
};
u8 menufarmano[12] = {
    0, 0, 1, 2, 3, 4, 5, 6, 
    0, 1, 0xFF, 0xFF, 
};
f32 menubackcampos[3] = {
    0.0f, 0.0f, 0.0f, 
};
f32 menubackcambak[3] = {
    0.0f, 0.0f, 0.0f, 
};
f32 menubackintpos[3] = {
    0.0f, 0.0f, 0.0f, 
};
f32 menubackintbak[3] = {
    0.0f, 0.0f, 0.0f, 
};
static u32 addStgFlagZ[555] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000019, 
    0x0000001A, 0x00000027, 0x0000003A, 0x0000003E, 0x00000043, 0x00000048, 0x0000004C, 0x00000055, 
    0x00000069, 0x0000006A, 0x0000006B, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000005, 0x00000007, 
    0x00000016, 0x00000029, 0x00000051, 0x0000006C, 0x00000071, 0x00000074, 0x00000075, 0x00000076, 
    0x0000007C, 0x0000007D, 0x0000007F, 0x00000080, 0xFFFFFFFF, 0x00000011, 0x00000026, 0x0000002A, 
    0x00000052, 0x00000059, 0x0000008C, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 
};
static u32 addStgFlagM[592] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000008, 0x00000009, 0x0000000A, 
    0x0000000B, 0x0000000C, 0x0000000D, 0x0000000E, 0x0000003E, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0x0000001B, 0x0000001C, 0x0000001D, 0x0000001E, 0x0000001F, 0x00000020, 0x00000021, 0x00000022, 
    0x00000023, 0x00000024, 0x00000025, 0x00000026, 0x00000027, 0x00000028, 0x00000042, 0x00000043, 
    0x00000012, 0x00000013, 0x00000014, 0x00000015, 0x00000016, 0x00000018, 0x00000019, 0x0000001A, 
    0x0000002F, 0x0000003B, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
static u32 addManVs33[14] = {
    0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000008, 0x00000009, 0x0000000A, 
    0x0000000B, 0x0000000C, 0x0000000D, 0x0000000E, 0x0000003E, 0xFFFFFFFF, 
};
static u32 addManVs34[17] = {
    0x0000001B, 0x0000001C, 0x0000001D, 0x0000001E, 0x0000001F, 0x00000020, 0x00000021, 0x00000022, 
    0x00000023, 0x00000024, 0x00000025, 0x00000026, 0x00000027, 0x00000028, 0x00000042, 0x00000043, 
    0xFFFFFFFF, 
};
static u32 addManVs35[11] = {
    0x00000012, 0x00000013, 0x00000014, 0x00000015, 0x00000016, 0x00000018, 0x00000019, 0x0000001A, 
    0x0000002F, 0x0000003B, 0xFFFFFFFF, 
};
static s16 custmVs[32] = {
    0, 2, 9, 12, 18, 23, 34, 44, 
    52, 59, 63, 65, 68, 74, 77, 86, 
    94, 102, 125, 128, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
};
static s16 humanVs[60] = {
    0, 1, 2, 16, 17, 23, 60, 61, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 
};
static s16 custmFist[32] = {
    0, 2, 9, 12, 18, 23, 34, 44, 
    52, 59, 63, 65, 68, 74, 77, 86, 
    94, 102, 125, 128, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
};
static s16 humanFist[60] = {
    0, 1, 2, 16, 17, 23, 60, 61, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 
};
u32 stagename_mis[73] = {
    0x802F139C, 0x802F13A0, 0x802F13A4, 0x802F13A8, 0x802F13AC, 0x802F13A8, 0x802F13B0, 0x802F13B4, 
    0x802F13B8, 0x802F13BC, 0x802F13C0, 0x802F13C4, 0x802F13C8, 0x802F13A8, 0x802F13CC, 0x802F13A8, 
    0x802F13A8, 0x802F13A8, 0x802F13A8, 0x802F139C, 0x802F13A0, 0x802F13A4, 0x802F13A8, 0x802F13D0, 
    0x802F13A8, 0x802F13A8, 0x802F13A8, 0x802F13A8, 0x802F13A8, 0x802F13A8, 0x802F13D4, 0x802F13D8, 
    0x802F13DC, 0x802F13E0, 0x802F13A8, 0x802F13E4, 0x802F13A8, 0x802F13A8, 0x802F13A8, 0x802F13A8, 
    0x802F13E8, 0x802F13EC, 0x802F13F0, 0x802F13F4, 0x802F13A8, 0x802F13B0, 0x802F13B4, 0x802F13B8, 
    0x802F13BC, 0x802F13C0, 0x802F13C4, 0x802F13C8, 0x802F13A8, 0x802F13A8, 0x802F13AC, 0x802F13A8, 
    0x802F13D4, 0x802F13D8, 0x802F13DC, 0x802F13E0, 0x802F13A8, 0x802F13E4, 0x802F13A8, 0x802F13A8, 
    0x802F13CC, 0x802F13A8, 0x802F13E8, 0x802F13EC, 0x802F13F0, 0x802F13F4, 0x802F13A8, 0x802F13D0, 
    0x802F1310, 
};
#pragma force_active off
void TeaMenuBGM(s16 setno)
{
    s16 readsize = 0;
    s16 bgm = 0;

    if (setno == -1) {
        bgm = -1;
    } else {
        readsize = sReadLenPtr("menu/bgm.dat", &bgm, setno * 2, 2);
        if (bgm < 0)
            bgm = -1;
        if (bgm > 100)
            bgm = -1;
    }
    printf("TEA Menu BGM PLAY %d setno=%d readsize=%d\n", bgm, setno, readsize);
    TeaBgm(bgm);
}
#undef lit_379
#define lit_379 "menu/bgm.dat"
#undef lit_380
#define lit_380 "TEA Menu BGM PLAY %d setno=%d readsize=%d\n"
void ttlMenuAllPurge(void)
{
    s16 i = 0;
    s16 j = 0;

    ZoidPurge((ZoidState *)&((ZoidStateV2 *)Zoid)[1]);
    ZoidPurge(&Zoid[0]);
    for (i = 0; i < 150; i++) {
        if (menu_zoidp[i] != NULL) {
            PurgeArc2(menu_zoidp[i]);
            menu_zoidp[i] = NULL;
        }
    }
    for (i = 0; i < 71; i++) {
        if (menu_personp[i] != NULL) {
            PurgeArc2(menu_personp[i]);
            menu_personp[i] = NULL;
        }
    }
    if (menu_detail1p != NULL) {
        PurgeArc2(menu_detail1p);
        menu_detail1p = NULL;
    }
    if (menu_back1p != NULL) {
        PurgeArc2(menu_back1p);
        menu_back1p = NULL;
    }
    if (menu1p != NULL) {
        PurgeArc2(menu1p);
        menu1p = NULL;
    }
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
}
void ttlMenuAllPurgeMission(void)
{
    s16 i = 0;
    s16 j = 0;

    ZoidPurge((ZoidState *)&((ZoidStateV2 *)Zoid)[1]);
    ZoidPurge(&Zoid[0]);
    for (i = 0; i < 150; i++) {
        if (menu_zoidp[i] != NULL) {
            PurgeArc2(menu_zoidp[i]);
            menu_zoidp[i] = NULL;
        }
    }
    for (i = 0; i < 71; i++) {
        if (menu_personp[i] != NULL) {
            PurgeArc2(menu_personp[i]);
            menu_personp[i] = NULL;
        }
    }
    if (menu_back1p != NULL) {
        PurgeArc2(menu_back1p);
        menu_back1p = NULL;
    }
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
}

void ttlMenuLoadYesNo(void) {}
void ttlMenuPurgeParts(void)
{
    if (menu1p != NULL) {
        PurgeArc2(menu1p);
        menu1p = NULL;
    }
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
}
void ttlMenuLoadParts(s32 kind)
{
    ttlMenuPurgeParts();
    switch (kind) {
    case 0:
        return;
    case 4:
        menu1p = LoadArc2("menu/battle_menu/farm/farm.dat");
        break;
    case 5:
        menu1p = LoadArc2("menu/battle_menu/battle_entry.dat");
        break;
    case 6:
        menu1p = LoadArc2("menu/title/title.dat");
        break;
    case 7:
        menu1p = LoadArc2("menu/vs_menu/vs_menu.dat");
        break;
    }
}
#undef lit_433
#define lit_433 "menu/battle_menu/farm/farm.dat"
#undef lit_434
#define lit_434 "menu/battle_menu/battle_entry.dat"
#undef lit_435
#define lit_435 "menu/title/title.dat"
#undef lit_436
#define lit_436 "menu/vs_menu/vs_menu.dat"
void ttlMenuPurgeBack(void)
{
    if (menu_back1p != NULL) {
        PurgeArc2(menu_back1p);
        menu_back1p = NULL;
    }
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
}
void ttlMenuLoadBack(s32 kind)
{
    ttlMenuPurgeBack();
    switch (kind) {
    case 0:
        return;
    case 3:
        menu_back1p = LoadArc2("menu/vs_menu/bglogo.dat");
        break;
    case 4:
        menu_back1p = LoadArc2("menu/garage/ngarage.dat");
        break;
    case 5:
        menu_back1p = LoadArc2("menu/garage00.dat");
        break;
    case 6:
        if (GameMode.field_2C == 0)
            menu_back1p = LoadArc2("menu/garage/ngarage.dat");
        if (GameMode.field_2C == 1)
            menu_back1p = LoadArc2("menu/garage/kgarage.dat");
        if (GameMode.field_2C == 2)
            menu_back1p = LoadArc2("menu/garage/tgarage.dat");
        break;
    }
}
#undef lit_456
#define lit_456 "menu/vs_menu/bglogo.dat"
#undef lit_457
#define lit_457 "menu/garage/ngarage.dat"
#undef lit_458
#define lit_458 "menu/garage00.dat"
#undef lit_459
#define lit_459 "menu/garage/kgarage.dat"
#undef lit_460
#define lit_460 "menu/garage/tgarage.dat"
void ttlMenuLoadDetail(void)
{
    ttlMenuPurgeDetail();
    menu_detail1p = LoadArc2("menu/battle_menu/detail.dat");
}
#undef lit_462
#define lit_462 "menu/battle_menu/detail.dat"
void ttlMenuPurgeDetail(void)
{
    if (menu_detail1p != NULL) {
        PurgeArc2(menu_detail1p);
        menu_detail1p = NULL;
    }
}
void ttlMenuLoadZoid(void)
{
    s16 i = 0;
    s16 j = 0;

    for (i = 1; i < 16; i++)
        GameZoid[i].type = -1;
    GameZoid[0].type = IveSave[0].field_15F4;
    GameZoid[1].type = IveSave[0].field_15FA;
    GameZoid[0].field_58 = IveSave[0].field_1600;
    GameZoid[1].field_58 = IveSave[0].field_1602;
    GameZoid[0].color = IveSave[0].zoid_color[IveSave[0].zoid_slot];
    if (IveSave[0].partner_slot != -1)
        GameZoid[1].color = IveSave[0].zoid_color[IveSave[0].partner_slot];
    ZoidInitC(&Zoid[0], GameZoid[0].type, GameZoid[0].color);
    if (GameMode.game_type == 2)
        ZoidInitC((ZoidState *)&((ZoidStateV2 *)Zoid)[1], GameZoid[1].type, GameZoid[1].color);
}
void ttlMenuLoadPersonBanner(void)
{
    s16 i = 0;
    s16 j = 0;
    char name[40];

    for (i = 0; i < 71; i++) {
        sprintf(name, "menu/picture/sp%02d.dat", i);
        if (menu_personp[i] == 0) {
            if (fexist(name) == 1)
                menu_personp[i] = LoadArc2(name);
            else
                menu_personp[i] = NULL;
        }
    }
}
#undef lit_488
#define lit_488 "menu/picture/sp%02d.dat"
void ttlMenuLoadZoidBanner(void)
{
    s16 i = 0;
    s16 j = 0;
    char name[40];

    for (i = 0; i < 150; i++) {
        sprintf(name, "menu/picture/sz%03d.dat", i);
        if (menu_zoidp[i] == 0) {
            if (fexist(name) == 1) {
                menu_zoidp[i] = LoadArc2(name);
            } else {
                menu_zoidp[i] = NULL;
                menu_zoidp[i] = LoadArc2("menu/picture/sz000.dat");
            }
        }
    }
}
#undef lit_500
#define lit_500 "menu/picture/sz%03d.dat"
#undef lit_501
#define lit_501 "menu/picture/sz000.dat"
#pragma force_active on
static const char lit_529[9] = "GX_CULL\n";
static const char lit_530[10] = "SetZmode\n";
static const char lit_533[11] = "CurCam On\n";
static const char lit_534[12] = "CurCam Off\n";
static const char lit_535[12] = "CurCam End\n";
static const char lit_536[11] = "EndRender\n";
static const char lit_537[27] = "ResetCam.................\n";
#pragma force_active off
void ttlMenuDispParts(HSD_JObj *joint, s32 a, s32 b, s32 c, s32 d)
{
    HSD_JObj *child;

    if (joint != NULL) {
        child = GetChild(joint, a, b, c, d, -1);
        if (child != NULL)
            HSD_JObjDisp(child, 0, 7, 0);
    }
}

void debugmenudraw(void)
{
    HSD_VICopyXFB(0);
    HSD_VISetXFBDrawDone();
    VIWaitForRetrace();
}
void ttlMain_MissionFlagChkSet(void)
{
    if (GameMode.win == 2) {
        if ((s32)IveSave[0].country == 1)
            IveSaveFlag.system_flags[1] = 1;
        if ((s32)IveSave[0].country == 2)
            IveSaveFlag.system_flags[2] = 1;
        if ((s32)IveSaveFlag.system_flags[1] == 1 && (s32)IveSaveFlag.system_flags[2] == 1)
            IveSaveFlag.system_flags[3] = 1;
        if ((s32)IveSaveFlag.system_flags[3] == 1 && (s32)IveSaveFlag.system_flags[0] == 1)
            IveSaveFlag.system_flags[8] = 1;
    }
}

void ttlMain_BattleFlagChkSet(void)
{
    if (GameMode.win == 2) {
        if (GameMode.map >= 11)
            IveSaveFlag.system_flags[0] = 1;
        if ((s32)IveSaveFlag.system_flags[3] == 1 && (s32)IveSaveFlag.system_flags[0] == 1)
            IveSaveFlag.system_flags[8] = 1;
        batFlag();
    }
}

void ttlMain(void)
{
    s32 unused = 0;
    char *movie[6] = { "movie/TomyLogo.THP", "movie/zoid_vs.thp", "", "", "thpdemo/rebirth.thp", "" };
    s8 first = 1;
    s32 i = 0;
    s32 played = 0;
    register s32 menu = 0;

    menudispyesnoflag = 0;
    PSMTXTrans(dspMtx, 0.0f, 0.0f, 0.0f);
    if (ttlLight == NULL)
        ttlLight = HSD_LObjLoadDesc(&infinite_light);
    HSD_VISetBlack(0);
    OSReport("GameStart01\n");
    played = 0;
    if (GameMode.game_type == 4) {
        OSReport("mission --");
        if (GameMode.win == 1) {
            OSReport("1 ");
            menu = 107;
            OSReport("GameStartMis 107\n");
        }
        if (GameMode.win == 2) {
            OSReport("Youre Win\n");
            memcopy(&IveSave[0], &IveSave[4], sizeof(GameSaveRecord));
            if (GameMode.map >= 100) {
                OSReport("after epilogue ---");
                GameMode.map = 0;
                ttlMain_MissionFlagChkSet();
                menuSetTitle(0);
                menuSetTitleButton(13);
                ttlMenuLoadSaveLoad();
                (void)(sPad1->pressed_a, 0);
                menudispyesnoflag = 1;
                (void)(sPad1->pressed_a, 0);
                menudispyesnopoint = menu = 1;
                (void)(sPad1->pressed_a, 0);
                do {
                    sPadAll();
                    menudispyesnopoint = menuYesNo(menudispyesnopoint, "", GetMenuMsgAdr(0x4d));
                    menu_sDispStartYesNo();
                } while (sPad1->pressed_a == 0);
                menu = menudispyesnopoint;
                menudispyesnoflag = 0;
                if (menu == 1) {
                    IveSave[0].field_1C87 = 0;
                    IveSave[0].stage = 0;
                    IveSave[0].field_1C8D = (u8)(menu = 0);
                    (void)(sPad1->pressed_a, 0); (void)(sPad1->pressed_a, 0); GameMode.map = menu = 0;
                    memcpy(&IveSaveFlagBak, &IveSaveFlag, sizeof(GameSaveRecord));
                    memcpy(&IveSave[3], &IveSaveFlag, sizeof(GameSaveRecord));
                    if (ttlMemCardChkStartNew() != 0) {
                        ttlMain_MissionFlagChkSet();
                        ttlMenuSystemMemCardSave();
                        ttlMemCardChkStartNew();
                    }
                }
                EndeFin();
                OSReport("save end \n");
                ttlMenuPurgeSaveLoad();
                menudispyesnoflag = 0;
                GameMode.win = 0;
                menu = 0;
            } else {
                GameMode.map++;
                IveSave[0].next_stage++;
                IveSave[0].stage++;
                menuZoidBattleModeSingleTeam = IveSave[0].single_team;
                menu = 101;
                OSReport("GameStartMis Win Next %d\n", IveSave[0].next_stage);
            }
        }
        if (GameMode.win == 3) {
            memcopy(&IveSave[0], &IveSave[4], sizeof(GameSaveRecord));
            GameMode.map = IveSave[0].stage;
            menu = 107;
            menuZoidBattleModeSingleTeam = IveSave[0].single_team;
            OSReport("GameStartMis loss Continue stage=%d\n", IveSave[0].next_stage);
        }
    }
    if (GameMode.game_type == 1) {
        if (GameMode.win == 1)
            menu = 230;
        if (GameMode.win == 2) {
            memcopy(&IveSave[0], &IveSave[4], sizeof(GameSaveRecord));
            IveSave[0].money = IveSave[0].money + GameMode.bp_earned;
            IveSave[4].money += GameMode.bp_earned;
            menu = 230;
            IveSave[0].play_time = IveSave[0].play_time + GameMode.gametime;
            IveSave[4].play_time += GameMode.gametime;
            ttlMain_BattleFlagChkSet();
            if (GameMode.map >= 11) {
                OSReport("Youre Win 12\x96\xca\n");
                OSReport("save load icon\n");
                OSReport("addHuman\n");
                ttlMenuLoadSaveLoad();
                menudispyesnoflag = 1;
                menudispyesnopoint = 1;
                menuSetTitle(2);
                menuSetTitleButton(12);
                do {
                    sPadAll();
                    menudispyesnopoint = menuYesNo(menudispyesnopoint, "", GetMenuMsgAdr(0x4d));
                    menu_sDispStartYesNo();
                } while (sPad1->pressed_a == 0);
                menudispyesnoflag = 0;
                if (menudispyesnopoint == 1) {
                    ttlMain_BattleFlagChkSet();
                    memcpy(&IveSaveFlagBak, &IveSaveFlag, sizeof(GameSaveRecord));
                    OSReport("END GAME save system end \n");
                    menuSetTitle(2);
                    menuSetTitleButton(11);
                    do {
                        if (ttlMemCardChkStartNew() == 0)
                            break;
                        ttlMain_BattleFlagChkSet();
                        ttlMenuSystemMemCardSave();
                        IveSave[0].field_1C87 = 0;
                        IveSave[0].stage = 0;
                        IveSave[0].field_1C89 = 0;
                        IveSave[0].map_no = menu_BatMapData[0].map;
                        IveSave[0].field_1C8D = 0;
                        IveSave[0].next_stage = 0;
                        GameMode.map = 0;
                        if (ttlMenuBatSaveLoad(1) == 1) {
                            flag_saveok;
                            break;
                        }
                    } while (flag_saveok != 1);
                }
                EndeFin();
                OSReport("save end \n");
                ttlMenuPurgeSaveLoad();
                menudispyesnoflag = 0;
                GameMode.win = 0;
                menu = 0;
            }
            GameMode.map++;
            IveSave[0].next_stage = IveSave[0].stage = GameMode.map;
            menuZoidBattleModeSingleTeam = IveSave[0].single_team;
            OSReport("GameStart Win Next %d time=%d bp=%d\n", IveSave[0].next_stage, IveSave[0].play_time, IveSave[0].money);
        }
        if (GameMode.win == 3) {
            memcopy(&IveSave[0], &IveSave[4], sizeof(GameSaveRecord));
            IveSave[0].play_time = IveSave[0].play_time + GameMode.gametime;
            menu = 230;
            menuZoidBattleModeSingleTeam = IveSave[0].single_team;
            OSReport("GameStart loss Continue stage=%d\n", IveSave[0].next_stage);
        }
    }
    if (GameMode.game_type == 2 || GameMode.game_type == 3) {
        if (GameMode.win != 0) {
            memcopy(&IveSave[0], &IveSave[4], sizeof(GameSaveRecord));
            memcopy(&IveSave[1], &IveSave[5], sizeof(GameSaveRecord));
            if (GameMode.field_08 == 0)
                menu = 306;
            else
                menu = 606;
            sCamNN2 = 2;
            sCamNN = 2;
            menuZoidBattleModeSingleTeam = IveSave[0].single_team;
        } else {
            menuZoidBattleModeSingleTeam = IveSave[0].single_team = 1;
        }
    }
    OSReport("GameStart02\n");
    for (;;) {
        if (IveSave[0].money > 999999999)
            IveSave[0].money = 999999999;
        debug11();
        OSReport("menu=%d type=%d win=%d ", menu, GameMode.game_type, GameMode.win);
        switch (menu) {
        case 0:
            menudispyesnoanm.anim = 1;
            menudispyesnoanm.state = 1;
            if (GameMode.field_38 == 0) {
                menusaveloadslot = 0;
                if (ttlMemCardChkStartNew() != 0) {
                    if (IveSaveFlag.system_flags[49] == 0) {
                        IveSaveFlag.system_flags[49] = (s8)TEA_mcFileOldFileExist(0);
                        if ((s32)IveSaveFlag.system_flags[49] == 1)
                            ttlMenuSystemMemCardSave();
                    }
                } else {
                    for (i = 0; i < 30; i++) {
                        HSD_AudioSFXKeyOffAll();
                        VIWaitForRetrace();
                    }
                }
                menu = 3;
                break;
            }
            if (GameMode.field_38 == 1) {
                menu = 8;
                break;
            }
            ttlMemCardChkStartNew();
            menu = 8;
            break;
        case 1:
            menu = 8;
            break;
        case 2:
            if (played == 0) {
                ttlMovie(movie[1], 1);
                played = 1;
                menu = 8;
            } else {
                GameMode.game_type = 9;
                menu = 9999;
                played = 0;
            }
            break;
        case 3:
            HSD_AudioSFXKeyOffAll();
            ttlMovie(movie[0], 0);
            for (i = 0; i < 10; i++) {
                HSD_AudioSFXKeyOffAll();
                VIWaitForRetrace();
            }
            HSD_AudioSFXKeyOffAll();
            menu = 2;
            break;
        case 5:
            HSD_AudioSFXKeyOffAll();
            ttlMovie(movie[4], 1);
            menu = 1;
            break;
        case 8:
            gRefresh();
            menu = menufncPushStart();
            break;
        case 10:
            (void)(GameMode.win, 0);
            menu = menufncMainMenu();
            break;
        case 100:
            menu = menufncStoryMode01();
            OSReport("Mission DEMO \x8fI\x97\xb9 \n");
            (void)(sPad1->pressed_a, 0);
            if ((s32)IveSave[0].stage == 100) {
                GameMode.map = 100;
                GameZoid[0].type = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                OSReport("Mission DEMO 100 %s\n", Tea_zoiddat[GameZoid[0].type].name);
                menu = 9999;
                break;
            }
            GameMode.map = IveSave[0].stage;
            break;
        case 101:
            menufncStoryMode01();
            OSReport("Mission DEMOb \x8fI\x97\xb9 \n");
            if ((s32)IveSave[0].stage == 100) {
                GameZoid[0].type = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                OSReport("Mission DEMO 100 %s\n", Tea_zoiddat[GameZoid[0].type].name);
                GameMode.map = 100;
                menu = 9999;
                break;
            }
            GameMode.map = IveSave[0].stage;
            (void)(menu * 3);
            menu = -menu;
            menu = (s32)&GameMode;
            menufncZoidBattle02Hokyu();
            ttlMenu_sDispMissionHokyu();
            ttlMenuLoadZoid();
        case 104:
            menu = menufncMission_Menu();
            break;
        case 105:
            (void)(sPad1->pressed_a, 0);
            (void)(sPad1->pressed_a, 0);
            (void)(sPad1->pressed_a, 0);
            menu = menufncZoidMission_MenuEntry();
            break;
        case 107:
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            OSReport("Mission DATA Load Start\n");
            menufncZoidBattle02();
            OSReport("Mission DATA Load End\n");
            OSReport("Mission DATA Load End\n");
            OSReport("Mission DATA Load End\n");
            OSReport("Mission DATA Load End\n");
            OSReport("Mission DATA Load End\n");
            OSReport("Mission DATA Load End\n");
            (void)(sPad1->pressed_a, 0);
            menu = menufncMission_Menu();
            break;
        case 200:
            menu = GameMode.win;
            menufncZoidBattle01Human();
            menu = 220;
            break;
        case 204:
            menu = menufncZoidBattle_BattleMenu();
            break;
        case 205:
            menu = menufncZoidBattle_BattleMenuEntry();
            break;
        case 220:
            menu = menufncZoidBattle_HumanBattleMenuEntry();
            break;
        case 221:
            TeaMenuBGM(8);
            IveSave[0].next_stage = IveSave[0].stage;
            menuZoidBattleSetStage_fromnowstage();
            ttlMenu_sDispBattleNextYokok();
        case 222:
            menufncZoidBattle02();
            menu = 204;
            break;
        case 230:
            menu = 221;
            break;
        case 210:
        case 310:
            menu = menufncZoidBattleEnd();
            break;
        case 300:
            menuZoidVS_NonSaveData_InitData(0);
            menuZoidVS_NonSaveData_InitData(1);
            menufncZoidVs01();
            menu = 304;
            break;
        case 304:
            menu = menufncZoidVsMenu();
            break;
        case 305:
            menu = menufncZoidVsMenuEntry();
            break;
        case 306:
            menufncZoidVs01();
            menu = 304;
            break;
        case 400:
            menu = 10;
            break;
        case 500:
            ttlMenuOption();
            menu = 10;
            break;
        case 600:
            menuZoidFist_NonSaveData_InitData(0);
            menuZoidFist_NonSaveData_InitData(1);
            menufncZoidVs01();
            menu = 604;
            break;
        case 604:
            menu = menufncZoidVsFistMenu();
            break;
        case 605:
            menu = menufncZoidVsMenuEntry();
            menu = 604;
            break;
        case 606:
            menufncZoidVs01();
            menu = 604;
            break;
        case 9000:
            testbitsetmenu_zoid();
            menu = 10;
            break;
        case 9999:
            TeaBgm(-1);
            if (ttlLight != NULL)
                HSD_LObjRemoveAll(ttlLight);
            ttlLight = 0;
            memcopy(&IveSave[4], &IveSave[0], sizeof(GameSaveRecord));
            ttlMenuAllPurge();
            dspLoadF = 1;
            return;
        case 106:
        default:
            menu = 1;
            break;
        }
    }
}
#undef lit_572
#define lit_572 "movie/TomyLogo.THP"
#undef lit_573
#define lit_573 "movie/zoid_vs.thp"
#undef lit_574
#define lit_574 ""
#undef lit_575
#define lit_575 "thpdemo/rebirth.thp"
#undef lit_722
#define lit_722 "GameStart01\n"
#undef lit_723
#define lit_723 "mission --"
#undef lit_724
#define lit_724 "1 "
#undef lit_725
#define lit_725 "GameStartMis 107\n"
#undef lit_726
#define lit_726 "Youre Win\n"
#undef lit_727
#define lit_727 "after epilogue ---"
#undef lit_728
#define lit_728 "save end \n"
#undef lit_729
#define lit_729 "GameStartMis Win Next %d\n"
#undef lit_730
#define lit_730 "GameStartMis loss Continue stage=%d\n"
#undef lit_731
#define lit_731 "Youre Win 12\x96\xca\n"
#undef lit_732
#define lit_732 "save load icon\n"
#undef lit_733
#define lit_733 "addHuman\n"
#undef lit_734
#define lit_734 "END GAME save system end \n"
#undef lit_735
#define lit_735 "GameStart Win Next %d time=%d bp=%d\n"
#undef lit_736
#define lit_736 "GameStart loss Continue stage=%d\n"
#undef lit_737
#define lit_737 "GameStart02\n"
#undef lit_738
#define lit_738 "menu=%d type=%d win=%d "
#undef lit_739
#define lit_739 "Mission DEMO \x8fI\x97\xb9 \n"
#undef lit_740
#define lit_740 "Mission DEMO 100 %s\n"
#undef lit_741
#define lit_741 "Mission DEMOb \x8fI\x97\xb9 \n"
#undef lit_742
#define lit_742 "Mission DATA Load Start\n"
#undef lit_743
#define lit_743 "Mission DATA Load End\n"

void menu_sDispStartYesNo(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    s32 e = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(0, 0);
    if (HSD_CObjSetCurrent(menu_yesnop->cobj) != 0) {
        HSD_LObjSetCurrentAll(menu_yesnop->lobj);
        HSD_LObjSetupInit(menu_yesnop->cobj);
        GXSetCullMode(0);
        menu_sDispYesNoMenu();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}

void ttlMovie(char *path, s32 skip)
{
    s32 i;

    if (ttlLight != NULL)
        HSD_LObjRemoveAll(ttlLight);
    ttlLight = 0;
    TEA_THP_PlaySkip(path, 0, skip);
    PSMTXTrans(dspMtx, 0.0f, 0.0f, 0.0f);
    menucam = &sCamera;
    sCameraSetNormal(menucam);
    ttlLight = HSD_LObjLoadDesc(&infinite_light);
    menucam->lobj = ttlLight;
    HSD_VISetBlack(0);
    for (i = 0; i < 10; i++) {
        sPadAll();
        VIWaitForRetrace();
        HSD_VICopyXFB(0);
        HSD_VISetXFBDrawDone();
    }
    sPadAll();
    sPadInit();
}

s32 TEA_defstatusload(void)
{
    s32 unused = 0;
    void *data;

    sReadPre();
    data = sReadFileU("etc/n_Zoid.dat", 0);
    memcpy(Tea_zoiddat, data, sizeof(Tea_zoiddat));
    free(data);
    sReadPre();
    data = sReadFileU("etc/n_Parts.dat", 0);
    memcpy(Tea_partsdat, data, sizeof(Tea_partsdat));
    free(data);
    sReadPre();
    data = sReadFileU("etc/n_Bullet.dat", 0);
    memcpy(Tea_bulletdat, data, sizeof(Tea_bulletdat));
    free(data);
    sReadPre();
    data = sReadFileU("etc/n_Person.dat", 0);
    memcpy(Tea_persondat, data, sizeof(Tea_persondat));
    free(data);
    sReadPre();
    data = sReadFileU("etc/n_Effect.dat", 0);
    memcpy(Tea_effectdat, data, sizeof(Tea_effectdat));
    free(data);
    sReadPre();
    data = sReadFileU("etc/TXT_Menu.BIN", 0);
    memcpy(TXT_Menu_dat, data, TXT_Menu_size);
    free(data);
    return 1;
}
#undef lit_756
#define lit_756 "etc/n_Zoid.dat"
#undef lit_757
#define lit_757 "etc/n_Parts.dat"
#undef lit_758
#define lit_758 "etc/n_Bullet.dat"
#undef lit_759
#define lit_759 "etc/n_Person.dat"
#undef lit_760
#define lit_760 "etc/n_Effect.dat"
#undef lit_761
#define lit_761 "etc/TXT_Menu.BIN"
#pragma force_active on
static const char lit_764[11] = "STORY MODE";
static const char lit_765[18] = "ZOIDS BATTLE MODE";
static const char lit_766[20] = "MISSION REPLAY MODE";
static const char lit_767[18] = "ZOIDS MUSEUM MODE";
static const char lit_768[12] = "OPTION MODE";
static const char lit_769[9] = "Zoid\x90\xdd\x92\xe8";
#pragma force_active off

s32 menufncMainMenu(void)
{
    s32 unused = 0;

    return 205;
}

s32 menufncStoryMode01(void)
{
    s16 i = 0;

    menudemo[0] = LoadArc2("menu/demo/demo.dat");
    menudemo[1] = LoadArc2("menu/demo/demosell00.dat");
    menudemo[2] = LoadArc2("menu/demo/demosell01.dat");
    menudemo[3] = LoadArc2("menu/demo/demosell02.dat");
    GameMode.map = IveSave[0].stage;
    menu_MissionDemo();
    for (i = 0; i < 4; i++) {
        if (menudemo[i] != NULL) {
            PurgeArc2(menudemo[i]);
            menudemo[i] = NULL;
        }
    }
    return 101;
}
#undef lit_790
#define lit_790 "menu/demo/demo.dat"
#undef lit_791
#define lit_791 "menu/demo/demosell00.dat"
#undef lit_792
#define lit_792 "menu/demo/demosell01.dat"
#undef lit_793
#define lit_793 "menu/demo/demosell02.dat"
#pragma force_active on
static const char lit_795[9] = "\x8b\xa4\x98" "a\x8d\x91\x8cR";
static const char lit_809[16] = "MenuStoryMode2\n";
static const char lit_810[17] = "TEST STORY MODE2";
#pragma force_active off

void ttlMenuOption(void) {}
#pragma force_active on
static const char lit_796[7] = "\x92\xe9\x8d\x91\x8cR";
#pragma force_active off

void testbitsetmenu_zoid(void)
{
    char buf[0x50];
    s32 d2 = 0;
    s32 d1 = 0;
    s32 base = 150;
    s32 y = base;
    s32 page = 0;
    s32 i = 0;
    s32 cursor = 0;
    s32 x = -240;
    s32 sel = 0;
    s32 d0 = 0;

    menuCursor = 0;
    menuParticles = 0;
    OSReport("MenuZoidBattle\n");
    while (1) {
        sPadAll();
        if (sPad3->field_5C != 0) {
            if (page < 5) {
                page++;
            }
        }
        if (sPad3->field_5B != 0) {
            if (page > 0) {
                page--;
            }
        }
        if (sPad3->field_4F != 0) {
            cursor--;
        }
        if (sPad3->field_50 != 0) {
            cursor++;
        }
        if (cursor < 0) {
            cursor = 9;
        }
        if (cursor > 9) {
            cursor = 0;
        }
        if (sPad3->field_5E != 0) {
            break;
        }
        if (sPad3->pressed_a != 0) {
            sel = testnumedit(x, -150, sel);
        }
        for (i = 0; i < 10; i++) {
            y = base - 24 * i;
            sPutString2(buf, x, y, -450, (GXColor){255, 255, 255, 255}, (GXColor){255, 255, 255, 0}, 0, 2);
        }
        {
            sPutString2("\x81\xa8", x - 40, base - 24 * cursor, -450, (GXColor){255, 255, 255, 255}, (GXColor){0, 255, 255, 55}, 0, 2);
        }
        if (page != 5) {
            sPutString2("R:\x8e\x9f\x82\xd6", 160, -150, -450, (GXColor){255, 255, 255, 255}, (GXColor){0, 0, 0, 0}, 0, 2);
        }
        if (page != 0) {
            sPutString2("L:\x91O\x82\xd6", -300, -150, -450, (GXColor){255, 255, 255, 255}, (GXColor){0, 0, 0, 0}, 0, 2);
        }
        debugmenudraw();
    }
}
#undef lit_844
#define lit_844 "\xff\xff\xff"
#undef lit_888
#define lit_888 "MenuZoidBattle\n"
#undef lit_889
#define lit_889 "\x81\xa8"
#undef lit_890
#define lit_890 "R:\x8e\x9f\x82\xd6"
#undef lit_891
#define lit_891 "L:\x91O\x82\xd6"

s32 Tea_atoi(char *text)
{
    s32 i = 0;
    register s8 digit = 0;
    s32 tmp = 0;
    s32 len = 6;
    s32 value = 0;
    s32 scale[6] = { 1, 10, 100, 1000, 10000, 100000 };

    for (i = 0; i < len; i++) {
        digit = text[len - 1 - i] - '0';
        if (digit == 10)
            digit = 0;
        tmp = digit * scale[i];
        value += tmp;
    }
    return value;
}
#pragma force_active on
static const char lit_1010[21] = "PARTS SET %d max=%d\n";
#pragma force_active off

s32 testnumedit(s32 x, s32 y, s32 value)
{
    char text[20];
    s32 unused = 0;
    s8 digit = 0;
    s32 cursor = 0;
    s32 len = 6;
    s32 number = value;

    while (1) {
        sprintf(text, "%06d", number);
        sPadAll();
        if (sPad3->field_51)
            cursor--;
        if (sPad3->field_52)
            cursor++;
        if (cursor > len - 1)
            cursor = 0;
        if (cursor < 0)
            cursor = len - 1;
        digit = text[cursor] - '0';
        if (sPad3->field_4F)
            digit++;
        if (sPad3->field_50)
            digit--;
        if (digit < 1)
            digit = 10;
        if (digit > 10)
            digit = 1;
        text[cursor] = digit + '0';
        number = Tea_atoi(text);
        if (sPad3->field_5E)
            return value;
        if (sPad3->pressed_a)
            return number;
        sprintf(text, "%06d", number);
        sPutString2(text, x, y, -450, (GXColor){ 255, 255, 255, 255 }, (GXColor){ 255, 255, 255, 0 }, 0, 2);
        sPutString2("\x81\xab", x + cursor * 13 - 4, y + 20, -450, (GXColor){ 255, 255, 255, 255 },
                    (GXColor){ 0, 255, 255, 0 }, 0, 2);
        debugmenudraw();
        digit = digit;
    }
}
#undef lit_927
#define lit_927 "\xff\xff\xff"
#undef lit_946
#define lit_946 "%06d"
#undef lit_947
#define lit_947 "\x81\xab"

void menuPartsWepSetListZoid(s16 category, s16 zoid, s16 point)
{
    s16 i = 0;
    s16 j = 0;
    s16 found = 0;

    for (i = 0; i < 450; i++)
        IveSave[0].list_parts[i] = IveSave[0].list_flag[i] = -1;
    IveSave[0].list_count = 0;
    for (i = 0; i < IveSave[0].owned_part_count; i++) {
        if (IveSave[0].owned_parts[i] > 0) {
            OSReport("PARTS List %d %d %s\n", i, IveSave[0].owned_parts[i], Tea_partsdat[IveSave[0].owned_parts[i]].name);
            found = 0;
            for (j = 0; j < 10; j++) {
                if (Tea_zoiddat[zoid].mount_parts[j] > 0 && IveSave[0].owned_parts[i] == Tea_zoiddat[zoid].mount_parts[j]
                    && point == Tea_zoiddat[zoid].weapon_points[j]) {
                    OSReport("PARTS WepP=%d pr=%d  %d:%s\n", Tea_zoiddat[zoid].weapon_points[j],
                             Tea_partsdat[IveSave[0].owned_parts[i]].category, Tea_zoiddat[zoid].mount_parts[j],
                             Tea_partsdat[Tea_zoiddat[zoid].mount_parts[j]].name);
                    found = 1;
                }
            }
            if (found) {
                IveSave[0].list_parts[IveSave[0].list_count] = IveSave[0].owned_parts[i];
                IveSave[0].list_flag[IveSave[0].list_count] = i;
                IveSave[0].list_count++;
            }
        }
    }
    OSReport("PARTS SET %d Wep%d max=%d\n", category, point, IveSave[0].list_count);
    if (IveSave[0].list_count == 0) {
        for (i = 0; i < 450; i++) {
        }
    }
}
#undef lit_1046
#define lit_1046 "PARTS List %d %d %s\n"
#undef lit_1047
#define lit_1047 "PARTS WepP=%d pr=%d  %d:%s\n"
#undef lit_1048
#define lit_1048 "PARTS SET %d Wep%d max=%d\n"
void menuPartsShopListZoid(s16 category, s16 zoid)
{
    s16 i = 0;
    s16 k = 0;
    s16 ok = 0;

    for (i = 0; i < 450; i++)
        IveSave[0].list_parts[i] = IveSave[0].list_flag[i] = -1;
    IveSave[0].list_count = 0;
    for (i = 0; i < 450; i++) {
        if (IveSave[0].shop_flag[i] && Tea_partsdat[i].category == category && Tea_partsdat[i].price != 0) {
            if (Tea_partsdat[i].zoids[0] == -1) {
                ok = 1;
            } else {
                for (k = 0; k < 12; k++) {
                    if (Tea_partsdat[i].zoids[k] == zoid) {
                        ok = 1;
                        break;
                    }
                }
            }
            if (ok) {
                ok = 0;
                OSReport("PARTSshop %d:n=%3d z=%d %s pr=%d\n", i, IveSave[0].list_count, zoid, Tea_partsdat[i].short_name,
                         category);
                IveSave[0].list_parts[IveSave[0].list_count] = i;
                IveSave[0].list_flag[IveSave[0].list_count] = i;
                IveSave[0].list_count++;
            }
        }
    }
}
#undef lit_1079
#define lit_1079 "PARTSshop %d:n=%3d z=%d %s pr=%d\n"
void menuPartsShopListAttackerCompareZoid(s16 category, s16 zoid)
{
    s16 i = 0;
    s16 k = 0;
    s16 mount = 0;

    for (i = 0; i < 450; i++)
        IveSave[0].list_parts[i] = IveSave[0].list_flag[i] = -1;
    IveSave[0].list_count = 0;
    for (i = 0; i < 450; i++) {
        if (IveSave[0].shop_flag[i] && Tea_partsdat[i].category == category && Tea_partsdat[i].price != 0) {
            for (k = 0; k < 10; k++) {
                mount = Tea_zoiddat[zoid].mount_parts[k];
                if (mount > 0 && i == mount) {
                    IveSave[0].list_parts[IveSave[0].list_count] = i;
                    IveSave[0].list_flag[IveSave[0].list_count] = i;
                    IveSave[0].list_count++;
                    break;
                }
            }
        }
    }
}
void menuPartsHaveListZoid(s16 category, s16 zoid)
{
    s16 i = 0;
    s16 k = 0;
    s16 part = 0;
    s16 ok = 0;

    for (i = 0; i < 450; i++)
        IveSave[0].list_parts[i] = IveSave[0].list_flag[i] = -1;
    IveSave[0].list_count = 0;
    for (i = 0; i < IveSave[0].owned_part_count; i++) {
        part = IveSave[0].owned_parts[i];
        if (part > 0 && Tea_partsdat[part].category == category) {
            if (zoid == -1)
                ok = 1;
            if (Tea_partsdat[part].zoids[0] == -1) {
                ok = 1;
            } else {
                for (k = 0; k < 12; k++) {
                    if (Tea_partsdat[part].zoids[k] == zoid) {
                        ok = 1;
                        break;
                    }
                }
            }
            if (ok) {
                ok = 0;
                OSReport("PARTSshop[%d] %d:n=%3d z=%d %s pr=%d\n", i, part, IveSave[0].list_count, zoid,
                         Tea_partsdat[i].short_name, category);
                IveSave[0].list_parts[IveSave[0].list_count] = part;
                IveSave[0].list_flag[IveSave[0].list_count] = i;
                IveSave[0].list_count++;
            }
        }
    }
}
#undef lit_1136
#define lit_1136 "PARTSshop[%d] %d:n=%3d z=%d %s pr=%d\n"

void menufnczoidmountpointdisp(void)
{
    s16 i = 0;
    s16 mount = 0;
    s16 count = 0;

    for (i = 0; i < 10; i++) {
        if ((s32)menuZoidMountPointDsip[i] == 1) {
            count++;
            mount = ttl_GetMountNo(IveSave[0].field_15F4, i);
            zdDispCursorSet(&Zoid[0], mount, 1);
        }
    }
    if (count == 0) {
        for (i = 0; i < 28; i++)
            zdDispCursorSet(&Zoid[0], i, 0);
    }
}

void menufnczoidmountpointdispset(s8 point)
{
    s16 i = 0;

    for (i = 0; i < 28; i++)
        menuZoidMountPointDsip[i] = 0;
    if (point >= 0) {
        menuZoidMountPointDsip[point] = 1;
        OSReport("MountPointDsip:%d\n", point);
    }
}
#undef lit_1160
#define lit_1160 "MountPointDsip:%d\n"
#pragma force_active on
static const char lit_1195[17] = "ZoidListDisp:Now";
#pragma force_active off

s32 menufncZoidBattle01Human(void)
{
    ttlMenuPurgeDetail();
    ttlMenuPurgeBack();
    ttlMenuPurgeParts();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    dspLoadF = 1;
    TeaMenuBGM(6);
    IveSave[0].field_1600 = 0;
    IveSave[0].field_1602 = 1;
    ttlMenuLoadParts(5);
    ttlMenuLoadBack(4);
    ttlMenuLoadDetail();
    ttlMenuLoadZoidBanner();
    ttlMenuLoadPersonBanner();
    ttlMenuLoadYesNo();
    return 220;
}

s32 menufncZoidBattle02(void)
{
    ttlMenuPurgeDetail();
    ttlMenuPurgeBack();
    ttlMenuPurgeParts();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    dspLoadF = 1;
    if (GameMode.field_2C == 0)
        TeaMenuBGM(3);
    else if (GameMode.field_2C == 1)
        TeaMenuBGM(1);
    else
        TeaMenuBGM(2);
    ttlMenuLoadParts(4);
    ttlMenuLoadBack(6);
    ttlMenuLoadDetail();
    ttlMenuLoadZoidBanner();
    ttlMenuLoadPersonBanner();
    ttlMenuLoadYesNo();
    ttlMenuLoadZoid();
    return 204;
}

s32 menufncZoidBattle02Hokyu(void)
{
    ttlMenuPurgeDetail();
    ttlMenuPurgeBack();
    ttlMenuPurgeParts();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    dspLoadF = 1;
    GameMode.field_2C = (s8)IveSave[0].country;
    GameMode.game_type = IveSave[0].field_1C8C;
    if (GameMode.field_2C == 1)
        TeaMenuBGM(1);
    else
        TeaMenuBGM(2);
    ttlMenuLoadParts(4);
    ttlMenuLoadBack(6);
    ttlMenuLoadDetail();
    ttlMenuLoadZoidBanner();
    ttlMenuLoadPersonBanner();
    ttlMenuLoadYesNo();
    return 204;
}

s32 menufncZoidBattleEnd(void)
{
    ttlMenuPurgeDetail();
    if (menu1p != NULL)
        PurgeArc2(menu1p);
    menu1p = NULL;
    if (menu_back1p != NULL)
        PurgeArc2(menu_back1p);
    menu_back1p = NULL;
    ttlMenuPurgeParts();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    return menuretno;
}

void menuZoidBattleGoPartsOptSet_GameZoid(s16 gz, s16 save, s16 slot, s32 flag)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 type = 0;
    s16 i = 0;
    s16 k = 0;
    s16 mount = 0;
    s16 part = 0;
    s16 color = 0;

    GameZoid[gz].type = type = IveSave[save].owned_zoids[slot];
    if (flag != 0)
        GameZoid[gz].field_58 = IveSave[save].field_1602;
    else
        GameZoid[gz].field_58 = IveSave[save].field_1600;
    GameZoid[gz].color = IveSave[save].zoid_color[slot];
    k = slot;
    for (i = 0; i < 28; i++)
        GameZoid[gz].mounts[i] = -1;
    for (i = 0; i < 10; i++) {
        if (IveSave[save].zoid_parts[k][i] == 0)
            IveSave[save].zoid_parts[k][i] = -1;
    }
    for (i = 0; i < 10; i++) {
        part = IveSave[save].zoid_parts[k][i];
        if (part != -1) {
            mount = ttl_GetMountNo(type, i);
            if (mount != -1) {
                GameZoid[gz].mounts[mount] = part;
                color = IveSave[save].part_colors[k][mount];
                GameZoid[gz].mount_colors[mount] = color;
                OSReport("\x95\x90\x8a\xed\x83J\x83\x89\x81[\x83Z\x83" "b\x83gGameZoid=%d WepP=%d mt=%d color=%d \n", gz, i, mount, color);
            }
        }
    }
    GameZoid[gz].field_5E = IveSave[save].zoid_field_119E[k];
    for (i = 0; i < 3; i++)
        GameZoid[gz].field_60[i] = IveSave[save].zoid_field_11DA[k][i];
}
#undef lit_1226
#define lit_1226 "\x95\x90\x8a\xed\x83J\x83\x89\x81[\x83Z\x83" "b\x83gGameZoid=%d WepP=%d mt=%d color=%d \n"
#pragma force_active on
static const char lit_1232[16] = "ZOIDS1_%01d.TEA";
static const char lit_1233[16] = "MemCardLoad:%s\n";
static const char lit_1239[16] = "MemCardSave:%s\n";
static const u32 lit_1240[16] = {
    0x00030000, 0x00010005, 0x00020004, 0x0017000F, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 
};
static const u16 lit_1241[15] = {
    0x0000, 0x0017, 0x0011, 0x0004, 0x0002, 0x0022, 0x0001, 0x0002, 
    0x000D, 0x0016, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
};
static const char lit_1341[16] = "MemCardTest:%s\n";
static const char lit_1342[12] = "zoid/%s.dat";
#pragma force_active off
void ttl_HaveSortHuman(s16 save)
{
    s16 i = 0;
    s16 j = 0;
    s16 tmp = 0;

    for (i = 0; i < IveSave[save].person_count; i++) {
        if (IveSave[save].persons[i] < 0 || IveSave[save].persons[i] > 71) {
            IveSave[save].persons[i] = IveSave[save].persons[IveSave[save].person_count - 1];
            IveSave[save].persons[IveSave[save].person_count - 1] = -1;
            IveSave[save].person_count--;
        }
    }
    for (j = 0; j < IveSave[save].person_count; j++) {
        for (i = 0; i < IveSave[save].person_count - 1; i++) {
            if (IveSave[save].persons[i] > IveSave[save].persons[i + 1]) {
                tmp = IveSave[save].persons[i];
                IveSave[save].persons[i] = IveSave[save].persons[i + 1];
                IveSave[save].persons[i + 1] = tmp;
            }
        }
    }
}
void ttl_HaveAddHuman(s16 save, s16 person)
{
    s16 i = 0;

    if (person >= 0 && person < 71 && IveSave[save].person_count < 71
        && IveSave[save].person_got[person] == 0) {
        IveSave[save].person_got[person] = 1;
        for (i = 0; i < IveSave[save].person_count; i++) {
            if (person == IveSave[save].persons[i])
                return;
        }
        IveSave[save].field_1688++;
        IveSave[save].field_1689++;
        IveSave[save].persons[IveSave[save].person_count] = person;
        IveSave[save].person_count++;
        OSReport("add man %d ---------------------------------\n", person);
    }
}
#undef lit_1380
#define lit_1380 "add man %d ---------------------------------\n"
void addZoids(s32 save, s32 zoid)
{
    s32 i;

    OSReport("req ADD-ZOIDS dno=%d , z=%d \n", save, zoid);
    if (zoid >= 150 || zoid < 0) {
        OSReport("error add-zoid %d\n", zoid);
    } else {
        for (i = 0; i < IveSave[save].zoid_list_count; i++) {
            if (IveSave[save].zoid_got[i] && zoid == IveSave[save].zoid_list[i])
                return;
        }
        if (i < 150) {
            IveSave[save].zoid_list[i] = (s16)zoid;
            IveSave[save].zoid_got[i] = 1;
            IveSave[save].zoid_list_count++;
            IveSave[save].field_1A0F++;
            OSReport("add-zoid %d ----------------------------\n", zoid);
        }
    }
}
#undef lit_1396
#define lit_1396 "req ADD-ZOIDS dno=%d , z=%d \n"
#undef lit_1397
#define lit_1397 "error add-zoid %d\n"
#undef lit_1398
#define lit_1398 "add-zoid %d ----------------------------\n"
void ttl_SortZoid(s16 kind)
{
    s16 save = kind;
    s16 i = 0;
    s16 j = 0;
    s16 tmp = 0;

    if (IveSave[save].zoid_list_count > 3) {
        printf("ZOID SORT  MAX=%d HAVE=%d \n", 150, IveSave[save].zoid_list_count);
        for (j = 0; j < IveSave[save].zoid_list_count; j++) {
            for (i = 0; i < IveSave[save].zoid_list_count - 1; i++) {
                if (IveSave[save].zoid_got[i] && IveSave[save].zoid_got[i + 1]
                    && (s32)IveSave[save].zoid_list[i] > IveSave[save].zoid_list[i + 1]) {
                    tmp = IveSave[save].zoid_list[i];
                    IveSave[save].zoid_list[i] = IveSave[save].zoid_list[i + 1];
                    IveSave[save].zoid_list[i + 1] = tmp;
                }
            }
        }
        printf("ZOID SORT  MAX=%d HAVE=%d \n", 150, IveSave[save].zoid_list_count);
    }
}
#undef lit_1418
#define lit_1418 "ZOID SORT  MAX=%d HAVE=%d \n"
void clearFlagThink(s32 save, s32 mode)
{
    s32 i;
    s32 v;
    s16 man = 0;
    s32 stage;
    s32 idx;
    s32 bit;

    for (stage = 0; stage < 36; stage++) {
        idx = stage / 8 + 10;
        bit = stage % 8;
        if (IveSaveFlag.system_flags[idx] & bitTBL[bit]) {
            if (mode == 1) {
                for (i = 0; i < 15; i++) {
                    v = ((s32 (*)[15])addStgFlagZ)[stage][i];
                    if (v != -1)
                        addZoids(save, v);
                }
            }
            if (mode == 1 || mode == 2 || mode == 3) {
                for (i = 0; i < 16; i++) {
                    v = ((s32 (*)[16])addStgFlagM)[stage][i];
                    man = v;
                    if (man != -1)
                        ttl_HaveAddHuman(save, man);
                }
            }
        }
    }
}

void iveClear(s32 save)
{
    GameSaveRecord *record = &IveSave[save];
    s32 i = 0;
    s32 j;

    for (; i < 30; i++) {
        record->zoid_color[i] = 0;
        for (j = 0; j < 28; j++)
            record->part_colors[i][j] = 0;
    }
}

void menuZoidBattle_NonSaveData_InitData(void)
{
    s16 zoids_init[32] = { 2, 6, 12, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    s16 no_zoid[51] = { 138, 139, 5, 7, 17, 22, 25, 26, 38, 39, 41, 42, 45, 49, 51, 58, 62, 67, 72, 76, 81, 82, 85, 89, 105, 106, 107, 108, 113, 116, 117, 118, 119, 121, 122, 123, 124, 125, 127, 128, 140, 129, 130, 131, 132, 133, 134, 135, 136, 137, -1 };
    s16 persons_init[60] = { 0, 1, 2, 61, 16, 17, 23, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    char text[80];
    s16 unused = 0;
    s32 size = sizeof(GameSaveRecord);
    s16 slot = 0;
    s16 save = 0;
    s16 i;
    s16 j;
    s16 count;
    s32 found;

    OSReport("menuZoidBattle_NonSaveData_InitData\n");
    sprintf(text, "ZOIDS1B%01d.TEA", slot);
    OSReport(lit_1341, text);
    iveClear(save);
    IveSave[save].money = 30000;
    IveSave[save].zoid_list_count = 0;
    IveSave[save].field_1A0F = 0;
    IveSave[save].field_0000 = 0;
    for (i = 0; i < 150; i++) {
        IveSave[save].zoid_list[i] = -1;
        IveSave[save].zoid_got[i] = 0;
    }
    count = 0;
    for (i = 0; i < 150; i++) {
        found = 0;
        for (j = 0; j < 50; j++) {
            if (no_zoid[j] == -1)
                break;
            found = 0;
            if (i == no_zoid[j]) {
                found = 1;
                break;
            }
        }
        if (found != 1 && Tea_zoiddat[i].field_50 != 0) {
            IveSave[save].zoid_list[count] = i;
            IveSave[save].zoid_got[count] = 1;
            IveSave[save].zoid_list_count++;
            count++;
            IveSave[save].field_1A0F++;
        }
    }
    for (i = 0; i < 50; i++)
        IveSave[save].system_flags[i] = 0;
    for (i = 0; i < 30; i++)
        IveSave[save].owned_zoids[i] = -1;
    for (i = 0; i < 30; i++) {
        IveSave[save].zoid_field_0AD2[i] = 0;
        IveSave[save].owned_zoids[i] = zoids_init[i];
    }
    for (i = 0; i < 30; i++) {
        IveSave[save].zoid_color[i] = 0;
        for (j = 0; j < 28; j++)
            IveSave[save].zoid_parts[i][j] = -1;
    }
    IveSave[save].person_count = 0;
    IveSave[save].field_1688 = IveSave[save].field_1689 = 0;
    for (i = 0; i < 71; i++) {
        IveSave[save].person_got[i] = 0;
        IveSave[save].persons[i] = -1;
        for (j = 0; j < 57; j++) {
            if (persons_init[j] != -1 && i == persons_init[j]) {
                IveSave[save].person_got[i] = 1;
                IveSave[save].field_1688++;
                IveSave[save].field_1689++;
                IveSave[save].persons[IveSave[save].person_count] = i;
                IveSave[save].person_count++;
            }
        }
    }
    menuZoidBattle_SaveData_AddData();
    if (IveSaveFlag.system_flags[0x31]) {
        if (IveSave[save].money <= 100000)
            IveSave[save].money = 100000;
        addZoids(save, 0x2D);
        OSReport("add-zoid-45\n");
    }
    IveSave[save].owned_zoid_count = 0;
    for (i = 0; i < 30; i++) {
        j = IveSave[save].owned_zoids[i];
        if (j != -1) {
            sprintf(text, lit_1342, Tea_zoiddat[j].file);
            if (fexist(text) == 1)
                IveSave[save].owned_zoid_count++;
            else
                IveSave[save].owned_zoids[i] = -1;
        }
    }
    for (i = 0; i < IveSave[save].owned_zoid_count; i++)
        ttl_IveSaveSetZoidDefParts(0, i, IveSave[save].owned_zoids[i]);
    clearFlagThink(save, 1);
    IveSave[save].field_15F4 = IveSave[save].owned_zoids[0];
    IveSave[save].zoid_slot = 0;
    IveSave[save].field_15FA = IveSave[save].owned_zoids[1];
    IveSave[save].partner_slot = 1;
    IveSave[save].field_1600 = 0;
    IveSave[save].field_1602 = 1;
    IveSave[save].owned_part_count = 0;
    for (i = 0; i < 450; i++)
        IveSave[save].owned_parts[i] = -1;
    IveSave[save].shop_count = 0;
    for (i = 0; i < 450; i++) {
        IveSave[save].shop_flag[i] = 1;
        IveSave[save].shop_count++;
    }
    OSReport("MemCard_NonSaveDataInit_06\n");
}
#undef lit_1553
#define lit_1553 "menuZoidBattle_NonSaveData_InitData\n"
#undef lit_1554
#define lit_1554 "ZOIDS1B%01d.TEA"
#undef lit_1555
#define lit_1555 "add-zoid-45\n"
#undef lit_1556
#define lit_1556 "MemCard_NonSaveDataInit_06\n"
void menuZoidBattle_SaveData_AddData(void)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 unused2 = 0;
    s16 unused3 = 0;
    s32 unused5 = 0;
    s32 size = sizeof(GameSaveRecord);
    s16 save = 0;
    s16 i = 0;

    for (i = 0; i < 4; i++) {
        if (IveSaveFlag.system_flags[i + 4]) {
            addZoids(save, i + 35);
            OSReport("add-zoid-4 diab =====================================================================================================================================================\n");
        }
    }
    if (IveSaveFlag.system_flags[0])
        OSReport("battle all clear\n");
    if (IveSaveFlag.system_flags[1]) {
        OSReport("mission-k all clear\n");
        addZoids(save, 49);
        OSReport("add-zoid-49\n");
    }
    if (IveSaveFlag.system_flags[2]) {
        OSReport("mission-t all clear\n");
        addZoids(save, 51);
        OSReport("add-zoid-51\n");
    }
    if (IveSaveFlag.system_flags[3]) {
        OSReport("all clear\n");
        addZoids(save, 119);
        addZoids(save, 121);
        addZoids(save, 122);
        addZoids(save, 123);
        OSReport("add-zoid-119 121 122 123\n");
    }
    if (IveSaveFlag.system_flags[5]) {
        addZoids(save, 138);
        OSReport("add-zoid-138\n");
    }
    if (IveSaveFlag.system_flags[6]) {
        addZoids(save, 139);
        OSReport("add-zoid-139\n");
    }
    clearFlagThink(save, 1);
    if (IveSaveFlag.system_flags[1]) {
        ttl_HaveAddHuman(0, 63);
        ttl_HaveAddHuman(0, 43);
        ttl_HaveAddHuman(0, 44);
        ttl_HaveAddHuman(0, 42);
        OSReport("add-chara-63 43 44 42\n");
    }
    if (IveSaveFlag.system_flags[2]) {
        ttl_HaveAddHuman(0, 64);
        ttl_HaveAddHuman(0, 45);
        ttl_HaveAddHuman(0, 46);
        ttl_HaveAddHuman(0, 41);
        OSReport("add-chara-64 45 46 41\n");
    }
    if (IveSaveFlag.system_flags[3]) {
        ttl_HaveAddHuman(0, 65);
        OSReport("add-chara-65\n");
    }
    clearFlagThink(save, 1);
}
#undef lit_1587
#define lit_1587 "add-zoid-4 diab =====================================================================================================================================================\n"
#undef lit_1588
#define lit_1588 "battle all clear\n"
#undef lit_1589
#define lit_1589 "mission-k all clear\n"
#undef lit_1590
#define lit_1590 "add-zoid-49\n"
#undef lit_1591
#define lit_1591 "mission-t all clear\n"
#undef lit_1592
#define lit_1592 "add-zoid-51\n"
#undef lit_1593
#define lit_1593 "all clear\n"
#undef lit_1594
#define lit_1594 "add-zoid-119 121 122 123\n"
#undef lit_1595
#define lit_1595 "add-zoid-138\n"
#undef lit_1596
#define lit_1596 "add-zoid-139\n"
#undef lit_1597
#define lit_1597 "add-chara-63 43 44 42\n"
#undef lit_1598
#define lit_1598 "add-chara-64 45 46 41\n"
#undef lit_1599
#define lit_1599 "add-chara-65\n"

void menuZoidMission_NonSaveData_InitData(void)
{
    s16 zoids_a[32] = { 12, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    s16 zoids_b[32] = { 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    s16 persons_init[15] = { 63, -1, -1, 64, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    char text[80];
    s16 unused = 0;
    s32 size = sizeof(GameSaveRecord);
    s16 slot = 0;
    s16 save = 0;
    s16 i = 0;
    s16 j = 0;
    s16 k = 0;

    sprintf(text, "ZOIDS1M%01d.TEA", slot);
    OSReport(lit_1341, text);
    iveClear(save);
    IveSave[save].money = 30000;
    IveSave[save].zoid_list_count = 0;
    IveSave[save].field_1A0F = 0;
    IveSave[save].field_0000 = 0;
    for (i = 0; i < 150; i++) {
        IveSave[save].zoid_list[i] = i;
        IveSave[save].zoid_got[i] = 1;
        IveSave[save].zoid_list_count++;
        IveSave[save].field_1A0F++;
        for (j = 0; j < 30; j++) {
        }
    }
    for (i = 0; i < 50; i++)
        IveSave[save].system_flags[i] = 0;
    for (i = 0; i < 30; i++)
        IveSave[save].owned_zoids[i] = -1;
    for (i = 0; i < 30; i++) {
        IveSave[save].zoid_field_0AD2[i] = 0;
        if (GameMode.field_2C == 1)
            IveSave[save].owned_zoids[i] = zoids_a[i];
        else
            IveSave[save].owned_zoids[i] = zoids_b[i];
    }
    for (i = 0; i < 30; i++) {
        IveSave[save].zoid_color[i] = 0;
        for (j = 0; j < 28; j++)
            IveSave[save].zoid_parts[i][j] = -1;
    }
    IveSave[save].owned_zoid_count = 0;
    for (i = 0; i < 30; i++) {
        j = IveSave[save].owned_zoids[i];
        if (j != -1) {
            sprintf(text, lit_1342, Tea_zoiddat[j].file);
            if (fexist(text) == 1)
                IveSave[save].owned_zoid_count++;
            else
                IveSave[save].owned_zoids[i] = -1;
        }
    }
    for (i = 0; i < IveSave[save].owned_zoid_count; i++)
        ttl_IveSaveSetZoidDefParts(save, i, IveSave[save].owned_zoids[i]);
    IveSave[save].person_count = 0;
    IveSave[save].field_1688 = IveSave[save].field_1689 = 0;
    for (i = 0; i < 71; i++) {
        IveSave[save].person_got[i] = 0;
        IveSave[save].persons[i] = -1;
        for (j = 0; j < 3; j++) {
            k = j + (GameMode.field_2C - 1) * 3;
            if (k != -1 && i == persons_init[k]) {
                IveSave[save].person_got[IveSave[save].field_1688] = 1;
                IveSave[save].field_1688++;
                IveSave[save].field_1689++;
                IveSave[save].persons[IveSave[save].person_count] = i;
                IveSave[save].person_count++;
            }
        }
    }
    IveSave[save].field_15F4 = IveSave[save].owned_zoids[0];
    IveSave[save].zoid_slot = 0;
    IveSave[save].field_15FA = IveSave[save].owned_zoids[1];
    IveSave[save].partner_slot = 1;
    IveSave[save].field_1600 = IveSave[save].persons[0];
    IveSave[save].field_1602 = IveSave[save].persons[1];
    IveSave[save].owned_part_count = 0;
    for (i = 0; i < 450; i++)
        IveSave[save].owned_parts[i] = -1;
    IveSave[save].shop_count = 0;
    for (i = 0; i < 450; i++) {
        IveSave[save].shop_flag[i] = 1;
        IveSave[save].shop_count++;
    }
    OSReport("MemCard_NonSaveDataInit_06 Mission\n");
}
#undef lit_1681
#define lit_1681 "ZOIDS1M%01d.TEA"
#undef lit_1682
#define lit_1682 "MemCard_NonSaveDataInit_06 Mission\n"

void menuZoidMission_EndSaveData_start(void)
{
    s16 persons_init[15] = { 31, 32, 33, 34, 35, 36, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    s16 unused0 = 0;
    s16 unused1 = 0;
    s32 size = sizeof(GameSaveRecord);
    s16 i = 0;
    s16 j = 0;
    s16 k = 0;
    s16 save = 0;

    OSReport("menuZoidMission_EndSaveData_start\n");
    IveSave[save].field_0000 = 0;
    IveSave[save].person_count = 0;
    IveSave[save].field_1688 = IveSave[save].field_1689 = 0;
    for (i = 0; i < 71; i++) {
        IveSave[save].person_got[i] = 0;
        IveSave[save].persons[i] = -1;
        for (j = 0; j < 3; j++) {
            k = j + (GameMode.field_2C - 1) * 3;
            if (i == persons_init[k]) {
                IveSave[save].person_got[IveSave[save].field_1688] = 1;
                IveSave[save].field_1688++;
                IveSave[save].field_1689++;
                IveSave[save].persons[IveSave[save].person_count] = i;
                IveSave[save].person_count++;
            }
        }
    }
    IveSave[save].field_1600 = IveSave[save].persons[0];
    IveSave[save].field_1602 = IveSave[save].persons[1];
    OSReport("MemCard_EndSaveDataInit_Start\n");
}
#undef lit_1697
#define lit_1697 "menuZoidMission_EndSaveData_start\n"
#undef lit_1698
#define lit_1698 "MemCard_EndSaveDataInit_Start\n"
s32 chkBattleClear(s32 n)
{
    s32 idx = n / 8 + 10;
    s32 bit = n % 8;

    return IveSaveFlag.system_flags[idx] & bitTBL[bit];
}
s32 chkBattleClear(s32 n);

void addManBattleClear(s32 save, s32 flag, s32 *list)
{
    if (chkBattleClear(flag) != 0 && list != 0) {
        while (*list != -1) {
            ttl_HaveAddHuman(save, *list);
            list++;
        }
    }
}
void addManBattleClear(s32 save, s32 flag, s32 *list);

void addVS_FIST(s32 save)
{
    addManBattleClear(save, 33, (s32 *)addManVs33);
    addManBattleClear(save, 34, (s32 *)addManVs34);
    addManBattleClear(save, 35, (s32 *)addManVs35);
}
void addZoidsCustm(s32 save, s32 zoid)
{
    s32 i;

    OSReport("add-zoidsC %d ", zoid);
    for (i = 0; i < 30; i++) {
        if (IveSave[save].owned_zoids[i] == -1)
            break;
    }
    if (i == 30) {
        OSReport("error\n");
    } else {
        IveSave[save].zoid_field_0AD2[i] = 0;
        IveSave[save].owned_zoids[i] = (s16)zoid;
        OSReport("OK\n");
    }
}
#undef lit_1721
#define lit_1721 "add-zoidsC %d "
#undef lit_1722
#define lit_1722 "error\n"
#undef lit_1723
#define lit_1723 "OK\n"

void menuZoidVS_NonSaveData_InitData(s16 side)
{
    char text[84];
    s16 unused = 0;
    s32 size = sizeof(GameSaveRecord);
    s16 slot = 0;
    s16 i = 0;
    s16 j = 0;

    sprintf(text, "ZOIDS1V%01d.TEA", slot);
    OSReport(lit_1341, text);
    IveSave[side].money = 30000;
    IveSave[side].zoid_list_count = 0;
    IveSave[side].field_1A0F = 0;
    IveSave[side].field_0000 = 0;
    for (i = 0; i < 150; i++) {
        IveSave[side].zoid_list[i] = i;
        IveSave[side].zoid_got[i] = 1;
        IveSave[side].zoid_list_count++;
        IveSave[side].field_1A0F++;
    }
    for (i = 0; i < 30; i++)
        IveSave[side].owned_zoids[i] = -1;
    for (i = 0; i < 30; i++) {
        if (custmVs[i] != -1) {
            IveSave[side].zoid_field_0AD2[i] = 0;
            IveSave[side].owned_zoids[i] = custmVs[i];
        }
    }
    if (IveSaveFlag.system_flags[0]) {
        addZoidsCustm(side, 0x81);
        addZoidsCustm(side, 0x84);
    }
    if (IveSaveFlag.system_flags[1]) {
        addZoidsCustm(side, 0x85);
        addZoidsCustm(side, 0x83);
    }
    if (IveSaveFlag.system_flags[2]) {
        addZoidsCustm(side, 0x86);
        addZoidsCustm(side, 0x88);
    }
    if (IveSaveFlag.system_flags[3])
        addZoidsCustm(side, 0x87);
    if (IveSaveFlag.system_flags[4])
        addZoidsCustm(side, 0x89);
    for (i = 0; i < 30; i++) {
        IveSave[side].zoid_color[i] = 0;
        for (j = 0; j < 28; j++)
            IveSave[side].zoid_parts[i][j] = -1;
    }
    IveSave[side].owned_zoid_count = 0;
    for (i = 0; i < 30; i++) {
        j = IveSave[side].owned_zoids[i];
        if (j != -1) {
            sprintf(text, lit_1342, Tea_zoiddat[j].file);
            if (fexist(text) == 1)
                IveSave[side].owned_zoid_count++;
            else
                IveSave[side].owned_zoids[i] = -1;
        }
    }
    for (i = 0; i < IveSave[side].owned_zoid_count; i++)
        ttl_IveSaveSetZoidDefParts(side, i, IveSave[side].owned_zoids[i]);
    IveSave[side].person_count = 0;
    IveSave[side].field_1688 = IveSave[side].field_1689 = 0;
    for (i = 0; i < 70; i++) {
        IveSave[side].person_got[i] = 0;
        IveSave[side].persons[i] = -1;
        for (j = 0; j < 57; j++) {
            if (humanVs[j] != -1 && i == humanVs[j]) {
                IveSave[side].person_got[i] = 1;
                IveSave[side].field_1688++;
                IveSave[side].field_1689++;
                IveSave[side].persons[IveSave[side].person_count] = i;
                IveSave[side].person_count++;
            }
        }
    }
    if (IveSaveFlag.system_flags[1]) {
        ttl_HaveAddHuman(side, 0x3F);
        ttl_HaveAddHuman(side, 0x2B);
        ttl_HaveAddHuman(side, 0x2C);
        ttl_HaveAddHuman(side, 0x2A);
        OSReport("add-c63.43.44.42\n");
    }
    if (IveSaveFlag.system_flags[2]) {
        ttl_HaveAddHuman(side, 0x40);
        ttl_HaveAddHuman(side, 0x2D);
        ttl_HaveAddHuman(side, 0x2E);
        ttl_HaveAddHuman(side, 0x29);
        OSReport("add-chara-64.45.46.41\n");
    }
    if (IveSaveFlag.system_flags[3]) {
        ttl_HaveAddHuman(side, 0x41);
        OSReport("add-chara-65 \n");
    }
    addVS_FIST(side);
    IveSave[side].field_15F4 = IveSave[side].owned_zoids[0];
    IveSave[side].zoid_slot = 0;
    IveSave[side].field_15FA = IveSave[side].owned_zoids[1];
    IveSave[side].partner_slot = 1;
    IveSave[side].field_1600 = 0;
    IveSave[side].field_1602 = 1;
    IveSave[side].owned_part_count = 0;
    for (i = 0; i < 450; i++)
        IveSave[side].owned_parts[i] = -1;
    IveSave[side].shop_count = 0;
    for (i = 0; i < 450; i++) {
        IveSave[side].shop_flag[i] = 1;
        IveSave[side].shop_count++;
    }
    OSReport("MemCard_NonSaveDataInit_VS\n");
}
#undef lit_1809
#define lit_1809 "ZOIDS1V%01d.TEA"
#undef lit_1810
#define lit_1810 "add-c63.43.44.42\n"
#undef lit_1811
#define lit_1811 "add-chara-64.45.46.41\n"
#undef lit_1812
#define lit_1812 "add-chara-65 \n"
#undef lit_1813
#define lit_1813 "MemCard_NonSaveDataInit_VS\n"

void menuZoidFist_NonSaveData_InitData(s16 side)
{
    char text[84];
    s16 unused = 0;
    s32 size = sizeof(GameSaveRecord);
    s16 slot = 0;
    s16 i = 0;
    s16 j = 0;

    sprintf(text, lit_1809, slot);
    OSReport(lit_1341, text);
    IveSave[side].money = 30000;
    IveSave[side].zoid_list_count = 0;
    IveSave[side].field_1A0F = 0;
    IveSave[side].field_0000 = 0;
    for (i = 0; i < 150; i++) {
        IveSave[side].zoid_list[i] = i;
        IveSave[side].zoid_got[i] = 1;
        IveSave[side].zoid_list_count++;
        IveSave[side].field_1A0F++;
    }
    for (i = 0; i < 30; i++)
        IveSave[side].owned_zoids[i] = -1;
    for (i = 0; i < 30; i++) {
        IveSave[side].zoid_field_0AD2[i] = 0;
        IveSave[side].owned_zoids[i] = custmFist[i];
    }
    for (i = 0; i < 30; i++) {
        IveSave[side].zoid_color[i] = 0;
        for (j = 0; j < 28; j++)
            IveSave[side].zoid_parts[i][j] = -1;
    }
    IveSave[side].owned_zoid_count = 0;
    for (i = 0; i < 30; i++) {
        j = IveSave[side].owned_zoids[i];
        if (j != -1) {
            sprintf(text, lit_1342, Tea_zoiddat[j].file);
            if (fexist(text) == 1)
                IveSave[side].owned_zoid_count++;
            else
                IveSave[side].owned_zoids[i] = -1;
        }
    }
    for (i = 0; i < IveSave[side].owned_zoid_count; i++)
        ttl_IveSaveSetZoidDefParts(side, i, IveSave[side].owned_zoids[i]);
    IveSave[side].person_count = 0;
    IveSave[side].field_1688 = IveSave[side].field_1689 = 0;
    for (i = 0; i < 70; i++) {
        IveSave[side].person_got[i] = 0;
        IveSave[side].persons[i] = -1;
        for (j = 0; j < 57; j++) {
            if (humanFist[j] != -1 && i == humanFist[j]) {
                IveSave[side].person_got[i] = 1;
                IveSave[side].field_1688++;
                IveSave[side].field_1689++;
                IveSave[side].persons[IveSave[side].person_count] = i;
                IveSave[side].person_count++;
            }
        }
    }
    if (IveSaveFlag.system_flags[1]) {
        ttl_HaveAddHuman(side, 0x3F);
        ttl_HaveAddHuman(side, 0x2B);
        ttl_HaveAddHuman(side, 0x2C);
        ttl_HaveAddHuman(side, 0x2A);
        OSReport("add-chara-63.43.44.42\n");
    }
    if (IveSaveFlag.system_flags[2]) {
        ttl_HaveAddHuman(side, 0x40);
        ttl_HaveAddHuman(side, 0x2D);
        ttl_HaveAddHuman(side, 0x2E);
        ttl_HaveAddHuman(side, 0x29);
        OSReport("add-chara-64.45.46.41\n");
    }
    if (IveSaveFlag.system_flags[3]) {
        ttl_HaveAddHuman(side, 0x41);
        OSReport(lit_1599);
    }
    addVS_FIST(side);
    IveSave[side].field_15F4 = IveSave[side].owned_zoids[0];
    IveSave[side].zoid_slot = 0;
    IveSave[side].field_15FA = IveSave[side].owned_zoids[1];
    IveSave[side].partner_slot = 1;
    IveSave[side].field_1600 = 0;
    IveSave[side].field_1602 = 1;
    IveSave[side].owned_part_count = 0;
    for (i = 0; i < 450; i++)
        IveSave[side].owned_parts[i] = -1;
    IveSave[side].shop_count = 0;
    for (i = 0; i < 450; i++) {
        IveSave[side].shop_flag[i] = 1;
        IveSave[side].shop_count++;
    }
    OSReport(lit_1813);
}
#undef lit_1887
#define lit_1887 "add-chara-63.43.44.42\n"

void ttl_HaveAddParts(s16 save, s16 part)
{
    s16 i = 0;

    if (part != -1) {
        OSReport("ADD PartsNo=%d p=%d max=%d-----------------------------------\n", part,
                 IveSave[save].owned_part_count, IveSave[save].owned_part_count);
        IveSave[save].owned_parts[IveSave[save].owned_part_count] = part;
        IveSave[save].owned_part_count++;
        for (i = 0; i < IveSave[save].owned_part_count; i++)
            OSReport("Parts p=%d:No=%3d %s \n", i, IveSave[save].owned_parts[i], Tea_partsdat[IveSave[save].owned_parts[i]].name);
    }
}
#undef lit_1897
#define lit_1897 "ADD PartsNo=%d p=%d max=%d-----------------------------------\n"
#undef lit_1898
#define lit_1898 "Parts p=%d:No=%3d %s \n"

void ttl_HaveAddZoid(s16 save, s16 zoid)
{
    if (zoid != -1) {
        IveSave[save].owned_zoids[IveSave[save].owned_zoid_count] = zoid;
        ttl_IveSaveSetZoidDefParts(save, IveSave[save].owned_zoid_count, zoid);
        IveSave[save].owned_zoid_count++;
    }
}

s32 ttl_ZoidChkWepPoint(s16 zoid, s16 point)
{
    s16 i = 0;

    for (i = 0; i < 10; i++) {
        if (point == Tea_zoiddat[zoid].weapon_points[i])
            return 1;
    }
    return 0;
}

s32 ttl_ZoidWepPointPermission(s16 zoid, s16 point)
{
    s32 ok = 0;

    ok = ttl_ZoidChkWepPoint(zoid, point);
    if (ok == 0)
        return 0;
    if (Tea_zoiddat[zoid].point_flags[point] == 0)
        return 0;
    return 1;
}

s16 ttl_ZoidRemoveContractWep(s16 save, s16 slot, s16 point)
{
    s16 zoid = 0;
    s16 part = 0;

    if (slot >= IveSave[save].owned_zoid_count)
        return -1;
    if (slot < 0)
        return -1;
    zoid = IveSave[save].owned_zoids[slot];
    if (zoid < 0)
        return -1;
    if (point < 0)
        return -1;
    if (point >= 8)
        return -1;
    if (Tea_zoiddat[zoid].point_flags[point] == 0)
        return -1;
    part = IveSave[save].zoid_parts[slot][point];
    if (part <= 0)
        return -1;
    IveSave[save].zoid_parts[slot][point] = -1;
    OSReport("\x8e\xe6\x8aO\x82\xb5Parts=%d WepPOint=%d \n", part, point);
    return part;
}
#undef lit_1930
#define lit_1930 "\x8e\xe6\x8aO\x82\xb5Parts=%d WepPOint=%d \n"

s16 ttl_ZoidRemoveContractOpt(s16 save, s16 slot)
{
    s16 option = 0;
    s16 zoid = 0;

    zoid = IveSave[save].owned_zoids[slot];
    option = IveSave[save].zoid_field_119E[slot];
    IveSave[save].zoid_field_119E[slot] = -1;
    if (option <= 0)
        return -1;
    return option;
}

s16 ttl_ZoidRemoveContractSub(s16 save, s16 slot, s16 sub)
{
    s16 zoid = 0;
    s16 value;

    zoid = IveSave[save].owned_zoids[slot];
    value = 0;
    if (sub < 0)
        return -1;
    if (sub >= 3)
        return -1;
    value = IveSave[save].zoid_field_11DA[slot][sub];
    IveSave[save].zoid_field_11DA[slot][sub] = -1;
    if (value <= 0)
        return -1;
    return value;
}

void ttl_IveSaveSetZoidDefParts(s16 save, s16 slot, s16 zoid)
{
    s16 i = 0;
    s16 part = 0;
    s16 mount = 0;

    if (save >= 0 && slot >= 0 && zoid >= 0) {
        IveSave[save].mount_8 = -1;
        IveSave[save].mount_9 = -1;
        for (i = 0; i < 28; i++)
            IveSave[save].zoid_parts[slot][i] = -1;
        for (i = 0; i < 28; i++)
            IveSave[save].part_colors[slot][i] = 0;
        for (i = 0; i < 28; i++)
            menuwepdispmount[i] = -1;
        IveSave[save].zoid_field_119E[slot] = 0;
        for (i = 0; i < 3; i++)
            IveSave[save].zoid_field_11DA[slot][i] = 0;
        menuwepdispnum = 0;
        IveSave[save].owned_zoids[slot] = zoid;
        IveSave[save].zoid_color[slot] = Tea_zoiddat[zoid].default_color;
        OSReport("MountSetIveSave zd=%s ", Tea_zoiddat[zoid].name);
        for (i = 0; i < 10; i++) {
            part = Tea_zoiddat[zoid].default_parts[i];
            if (part > 0) {
                mount = Tea_zoiddat[zoid].default_parts[i + 10];
                if (mount >= 0) {
                    if (i == 8 || i == 9) {
                        if (Tea_zoiddat[zoid].point_flags[i] == 1)
                            IveSave[save].zoid_field_119E[slot] = part;
                    } else {
                        IveSave[save].zoid_parts[slot][i] = part;
                    }
                    if (i == 8)
                        IveSave[save].mount_8 = mount;
                    if (i == 9)
                        IveSave[save].mount_9 = mount;
                    menuwepdispmount[menuwepdispnum] = mount;
                    menuwepdispnum++;
                    OSReport("WP=%02d MP=%02d %s \n ", i, mount, Tea_partsdat[part].name);
                }
            }
        }
    }
}
#undef lit_1987
#define lit_1987 "MountSetIveSave zd=%s "
#undef lit_1988
#define lit_1988 "WP=%02d MP=%02d %s \n "

void ttl_GameZoidSetZoidDefParts(s16 gz, s16 zoid)
{
    s16 unused = 0;
    s16 i = 0;
    s16 part = 0;
    s16 mount = 0;

    if (gz >= 0 && zoid >= 0) {
        GameZoid[gz].mount_8 = -1;
        mount = GameZoid[gz].mount_8;
        for (i = 0; i < 28; i++)
            GameZoid[gz].mounts[i] = -1;
        for (i = 0; i < 28; i++)
            GameZoid[gz].mount_colors[i] = 0;
        GameZoid[gz].field_5E = 0;
        for (i = 0; i < 3; i++)
            GameZoid[gz].field_60[i] = 0;
        GameZoid[gz].type = zoid;
        OSReport("MountSetGameZoid zd=%s ", Tea_zoiddat[zoid].name);
        for (i = 0; i < 10; i++) {
            part = Tea_zoiddat[zoid].default_parts[i];
            if (part > 0) {
                mount = Tea_zoiddat[zoid].default_parts[i + 10];
                if (mount >= 0) {
                    if (i == 8 || i == 9) {
                        if (Tea_zoiddat[zoid].point_flags[i] == 1)
                            GameZoid[gz].field_5E = part;
                    } else {
                        GameZoid[gz].mounts[mount] = part;
                    }
                    if (i == 8)
                        GameZoid[gz].mount_8 = mount;
                    if (i == 9)
                        GameZoid[gz].mount_9 = mount;
                    OSReport("i=%02d %s-%02d : ", i, Tea_partsdat[part].name, mount);
                }
            }
        }
        GameZoid[gz].color = Tea_zoiddat[zoid].default_color;
        OSReport("\n");
    }
}
#undef lit_2028
#define lit_2028 "MountSetGameZoid zd=%s "
#undef lit_2029
#define lit_2029 "i=%02d %s-%02d : "
#undef lit_2030
#define lit_2030 "\n"

void ttl_WeaponAllUnMount(s16 slot)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 i = 0;
    s16 c = 0;
    s16 d = 0;
    s16 unused4 = 0;

    for (i = 0; i < 10; i++) {
        if (IveSave[0].zoid_parts[slot][i] != -1)
            ttl_GetMountNo(IveSave[0].owned_zoids[slot], i);
        d = 1;
    }
    ZoidWeaponPurge(&Zoid[0]);
    for (i = 0; i < 28; i++) {
        menuwepdispmount[i] = -1;
    }
    menuwepdispnum = 0;
    c = 1;
    d = d;
    d = d;
    d = d;
    d = d;
    slot = 0;
}

void ttl_WeaponAllMount(s16 slot)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 i = 0;
    s16 zoid = 0;
    s16 mount = 0;
    s32 weapon;

    zoid = IveSave[0].owned_zoids[slot];
    for (i = 0; i < 10; i++) {
        if (IveSave[0].zoid_parts[slot][i] != -1) {
            mount = ttl_GetMountNo(zoid, i);
            sWeaponMount(IveSave[0].zoid_parts[slot][i], &Zoid[0], mount, i);
            OSReport("NowZoid=%s %d wepp=%d parts=%d %d\n", Tea_zoiddat[zoid].name, i, ttl_GetMountNo(zoid, i),
                     Tea_zoiddat[zoid].default_parts[i], IveSave[0].zoid_parts[slot][i]);
            weapon = *(s32 *)((u8 *)&Zoid[0] + mount * 12 + 0x185C);
            if (weapon != -1)
                sWeaponColor(weapon, IveSave[0].part_colors[slot][mount], 0);
        }
    }
}
#undef lit_2068
#define lit_2068 "NowZoid=%s %d wepp=%d parts=%d %d\n"

s16 ttl_GetMountNo(s16 zoid, s16 slot)
{
    return Tea_zoiddat[zoid].default_parts[slot + 10];
}

char *GetMenuMsgAdr(s16 msg)
{
    s32 next;
    s32 offset;

    offset = TXT_Menu[msg];
    next = TXT_Menu[msg + 1];
    if (msg == -1)
        return 0;
    return (char *)TXT_Menu_dat + offset;
}
#pragma force_active on
static s32 detZoidCstm = -1;
static s32 detZoidPerson = -2;
static s32 detPartsn = -1;
static s32 detPersonn = -1;
static f32 shininess = 16.0f;
CameraState *menucam = (CameraState *)&sCamera;
s32 menuCursor = 1;
s32 menuParticles = 1;
s8 menuZoidBattleStage = -1;
s32 menudispyesnopoint = 1;
s16 flag_VsFarmPlayer1Icon = -1;
s16 flag_VsFarmPlayer2Icon = -1;
#pragma force_active off
void SetMenuMsgAdr(s16 msg, char *dst)
{
    static s16 nn[1] = {-2};
    s32 offset = 0;
    s32 next = 0;

    if (msg != nn[0]) {
        if (msg == -1) {
            strcpy(dst, " ");
        } else {
            offset = TXT_Menu[msg];
            next = TXT_Menu[msg + 1];
            strcpy(dst, (char *)TXT_Menu_dat + offset);
        }
    }
}
#undef lit_2083
#define lit_2083 " "
s32 menuYesNo(s32 point, char *msg1, char *msg2)
{
    s16 ret = 0;

    strcpy(menudispyesnostr1, msg1);
    strcpy(menudispyesnostr2, msg2);
    if (sPad1->field_59 || sPad2->field_59) {
        SeSelectB();
        return 1;
    }
    if (sPad1->field_5A || sPad2->field_5A) {
        SeSelectB();
        return 0;
    }
    return point;
}

void menuYesNoSetStr(char *str1, char *str2)
{
    strcpy(menudispyesnostr1, str1);
    strcpy(menudispyesnostr2, str2);
}

void menu_sDispYesNoMenu(void)
{
    s32 unused0 = 0;
    s16 unused1 = 0;
    s16 unused2 = 0;
    s16 unused3 = 0;
    s32 unused4 = 0;
    GXColor white = { 255, 255, 255, 255 };
    GXColor clear = { 0, 0, 0, 0 };
    char text[180];
    s32 kind = 1;
    s32 child = 0;
    s32 done = 0;
    HSD_JObj *rect;

    if (menudispyesnoflag != 0) {
        if (menu_yesnop != NULL) {
            kind = 2;
            if (menudispyesnostr1[0] && !menudispyesnostr2[0])
                kind = 1;
            if (menudispyesnoanm.state == 1) {
                SetAnimArc2(menu_yesnop, kind, menudispyesnoanm.anim);
                menudispyesnoanm.state = 2;
            }
            if (menudispyesnoanm.state == 2 && (done = jobjAnim(menu_yesnop->joints[kind])) != 0) {
                menudispyesnoanm.state = 2;
                menudispyesnoanm.anim = 1;
                SetAnimArc2(menu_yesnop, kind, menudispyesnoanm.anim);
                jobjAnim(menu_yesnop->joints[kind]);
                sMakeMatrix(menu_yesnop->joints[kind]);
            }
            child = 0;
            jobjDisp(GetChild(menu_yesnop->joints[kind], child, -1, -1, -1, -1));
            if (!menudispyesnostr1[0] || !menudispyesnostr2[0]) {
                if (menudispyesnopoint == 1)
                    jobjDispAll(GetChild(menu_yesnop->joints[kind], 0, 0, -1, -1, -1));
                else
                    jobjDisp(GetChild(menu_yesnop->joints[kind], 0, 0, -1, -1, -1));
                if (menudispyesnopoint == 0)
                    jobjDispAll(GetChild(menu_yesnop->joints[kind], 0, 1, -1, -1, -1));
                else
                    jobjDisp(GetChild(menu_yesnop->joints[kind], 0, 1, -1, -1, -1));
            }
            if (menudispyesnostr1[0] && !menudispyesnostr2[0])
                sprintf(text, "%s ", menudispyesnostr1);
            if (!menudispyesnostr1[0] && menudispyesnostr2[0])
                sprintf(text, "%s ", menudispyesnostr2);
            if (menudispyesnostr1[0] && menudispyesnostr2[0])
                sprintf(text, "%s ", menudispyesnostr1);
            rect = GetChild(menu_yesnop->joints[kind], 0, 2, 0, -1, -1);
            sPutFontRect(text, GetChild(menu_yesnop->joints[kind], 0, 2, -1, -1, -1), rect, white, clear, 0, 2);
        } else {
            OSReport("YesNoMenu Not Loading\n");
        }
    }
}
#undef lit_2132
#define lit_2132 "%s "
#undef lit_2133
#define lit_2133 "YesNoMenu Not Loading\n"

void menu_sDispSaveLoadMenuSet(s16 mode)
{
    s16 i = 0;
    s16 other = 0;
    s16 current = 0;

    switch (mode) {
    case -1:
        other = 0;
        break;
    case -2:
        other = 6;
        menudispslwindow[0].anim = other;
        menudispslwindow[0].active = 1;
        menudispslwindow[1].anim = other;
        menudispslwindow[1].active = 1;
        menudispslwindow[2].anim = other;
        menudispslwindow[2].active = 1;
        return;
    case -3:
        other = 1;
        menudispslwindow[0].anim = other;
        menudispslwindow[0].active = 1;
        menudispslwindow[1].anim = other;
        menudispslwindow[1].active = 1;
        menudispslwindow[2].anim = other;
        menudispslwindow[2].active = 1;
        return;
    default:
        other = 2;
        current = 3;
        break;
    }
    for (i = 0; i < 3; i++) {
        if (mode % 3 == i && mode >= 0) {
            menudispslwindow[i].anim = current;
            menudispslwindow[i].active = 1;
        } else {
            menudispslwindow[i].anim = other;
            menudispslwindow[i].active = 1;
        }
    }
}

void menu_sDispSaveLoadMenu(void)
{
    s16 i = 0;
    s16 idx = 24;
    s32 done = 0;

    for (i = 0; i < 3; i++) {
        if (menudispslwindow[i].active == 1) {
            done = sSetAnim1(menu1p, idx, GetChild(((MenuArchive *)menu1p)->joints[idx], 0, i, -1, -1, -1), menudispslwindow[i].anim);
            menudispslwindow[i].active = 2;
        }
        if (menudispslwindow[i].active == 2) {
            if ((done = jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, i, -1, -1, -1))) != 0) {
                menudispslwindow[i].active = 3;
                if (menudispslwindow[i].anim == 1 || menudispslwindow[i].anim == 6) {
                    if (menudispslwindow[i].anim == 1) {
                        menudispslwindow[i].anim = 2;
                        menudispslwindow[i].active = 2;
                    }
                    if (menudispslwindow[i].anim == 6) {
                        menudispslwindow[i].anim = 0;
                        menudispslwindow[i].active = 2;
                    }
                    done = sSetAnim1(menu1p, idx, GetChild(((MenuArchive *)menu1p)->joints[idx], 0, i, -1, -1, -1), menudispslwindow[i].anim);
                    jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, i, -1, -1, -1));
                } else if (menudispslwindow[i].anim == 0) {
                    menudispslwindow[i].active = 2;
                } else if (menudispslwindow[i].anim == 2) {
                    menudispslwindow[i].anim = 2;
                    menudispslwindow[i].active = 2;
                } else if (menudispslwindow[i].anim == 3) {
                    menudispslwindow[i].anim = 3;
                    menudispslwindow[i].active = 2;
                } else if (menudispslwindow[i].anim == 4) {
                    menudispslwindow[i].anim = 4;
                    menudispslwindow[i].active = 2;
                } else if (menudispslwindow[i].anim == 5) {
                    menudispslwindow[i].anim = 5;
                    menudispslwindow[i].active = 2;
                }
            }
        }
        if (menudispslwindow[i].active != 0)
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[24], 0, i, -1, -1, -1));
    }
}

void menu_sDispAllHavePartsList(void)
{
    s16 tbl[7] = { 0, 1, 3, 4, 0, 0, 0 };
    s32 u7 = 7;
    s32 u1 = 1;
    s16 u0 = 0;
    s32 u2 = 0;
    s32 u3 = 0;
    s32 idx = 22;
    s32 lines = 0;
    s32 k = 0;
    s16 pages = 0;
    s16 rem = 0;
    s16 part = 0;
    s16 off = 0;
    s16 dead = 0;
    HSD_JObj *jobj = 0;
    s16 i = 0;
    s16 count = 0;
    s16 n = 0;
    s32 u4 = 0;
    s16 u5 = 0;
    s16 list[450];

    if (flag_AllHavePartsList != 0) {
        count = 0;
        for (i = 0; i < IveSave[0].owned_part_count; i++) {
            if (IveSave[0].owned_parts[i] > 0) {
                list[i] = IveSave[0].owned_parts[i];
                count++;
            }
        }
        n = count;
        pages = n / 13;
        rem = n % 13;
        if (rem > 13)
            rem = 13;
        if (flag_AllHavePartsList > pages + 1) {
            dead = 0;
        }
        if (flag_AllHavePartsList != 0 && flag_AllHavePartsListanm[1] == 0) {
            flag_AllHavePartsListanm[0] = 1;
            flag_AllHavePartsListanm[1] = 1;
        }
        if (flag_AllHavePartsList > pages + 1 && flag_AllHavePartsListanm[1] != 3) {
            flag_AllHavePartsListanm[0] = 1;
            flag_AllHavePartsListanm[1] = 3;
            flag_AllHavePartsList = 0;
        }
        off = 0;
        lines = lines;
        if (flag_AllHavePartsListanm[1] == 3)
            off = 1;
        if (pages + 1 == flag_AllHavePartsList - off)
            lines = rem;
        else
            lines = 13;
        if (n == 0) {
            flag_AllHavePartsList = 0;
            return;
        }
        dead = rem;
        dead = flag_AllHavePartsListanm[1];
        flag_AllHavePartsListanm[0] = 1;
        flag_AllHavePartsListanm[1] = 2;
        SetAnimArc2((MenuArchive *)menu1p, idx, flag_AllHavePartsListanm[1]);
        jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
        sMakeMatrix(((MenuArchive *)menu1p)->joints[idx]);
        if (flag_AllHavePartsList != 0) {
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, -1, -1, -1, -1));
            for (i = 0; i < 13; i++) {
                if (i < lines) {
                    k = i + (flag_AllHavePartsList - 1 - off) * 13;
                    part = list[k];
                } else {
                    part = 0;
                }
                jobj = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, i + 1, -1, -1, -1);
                PutParts(0, part, jobj);
            }
            (void)jobj;
            if (flag_AllHavePartsListanm[1] == 0 && flag_AllHavePartsListanm[0] == 0)
                flag_AllHavePartsList = 0;
        }
    }
}

void menuZoidPartsCopy(s16 src_save, s16 src_slot, s16 dst_save, s16 dst_slot)
{
    s16 unused = 0;
    s16 i = 0;

    IveSave[dst_save].zoid_color[dst_slot] = IveSave[src_save].zoid_color[src_slot];
    IveSave[dst_save].owned_zoids[dst_slot] = IveSave[src_save].owned_zoids[src_slot];
    for (i = 0; i < 28; i++)
        IveSave[dst_save].zoid_parts[dst_slot][i] = IveSave[src_save].zoid_parts[src_slot][i];
    for (i = 0; i < 28; i++)
        IveSave[dst_save].part_colors[dst_slot][i] = IveSave[src_save].part_colors[src_slot][i];
    for (i = 0; i < 3; i++)
        IveSave[dst_save].zoid_field_11DA[dst_slot][i] = IveSave[src_save].zoid_field_11DA[src_slot][i];
    IveSave[dst_save].zoid_field_119E[dst_slot] = IveSave[src_save].zoid_field_119E[src_slot];
}

void PutParts(s16 x, s16 part, HSD_JObj *pos)
{
    (void)dtMain;
    s16 row = 0;
    s32 kind = 11;
    s16 x_copy = x;
    s16 part_copy = part;
    s16 dead = 0;
    HSD_JObj *target = pos;
    s16 col = 0;
    HSD_JObj *child = 0;
    HSD_JObj *root = ((MenuArchive *)menu1p)->joints[kind];
    char *name;

    if (root != 0) {
        child = GetChild(root, 0, -1, -1, -1, -1);
        jobjCopyPos(target, child);
        sMakeMatrix(child);
        HSD_JObjDisp(child, 0, 7, 0);
        if (part <= 0) {
            col = 15;
            row = 1;
        } else {
            col = Tea_partsdat[part].rank;
            if (Tea_partsdat[part].category == 1) {
                row = 1;
                if (col > 15)
                    col = 16;
            }
            if (Tea_partsdat[part].category == 2) {
                row = 3;
                if (col > 4)
                    col = 4;
            }
            if (Tea_partsdat[part].category == 3) {
                row = 4;
                if (col > 8)
                    col = 8;
            }
        }
        if (part <= 0)
            name = "NONE";
        else
            name = Tea_partsdat[part].short_name;
        jobjDispAll(GetChild(root, 0, row, col, -1, -1));
        putAscJobj(GetChild(root, 0, 2, -1, -1, -1), 2, name);
        dead = 1;
    }
}
#undef lit_2268
#define lit_2268 "NONE"

s16 menuSeartchPartsHave(s16 part)
{
    s16 i = 0;
    s16 count = 0;

    for (i = 0; i < IveSave[0].owned_part_count; i++) {
        if (IveSave[0].owned_parts[i] == part)
            count++;
    }
    return count;
}

s16 menuSeartchPartsZoid(s16 part, s16 slot)
{
    s16 kind = 0;
    s16 i = 0;
    s16 unused = 0;
    s16 count = 0;

    kind = Tea_partsdat[part].category;
    if (kind == 1) {
        for (i = 0; i < 8; i++) {
            if (IveSave[0].zoid_parts[slot][i] == part)
                count++;
        }
    }
    if (kind == 2) {
        if (IveSave[0].zoid_field_119E[slot] == part)
            count++;
    }
    if (kind == 3) {
        for (i = 0; i < 3; i++) {
            if (IveSave[0].zoid_field_11DA[slot][i] == part)
                count++;
        }
    }
    return count;
}

s16 menuSeartchPartsAllZoid(s16 part)
{
    s16 unused = 0;
    s16 i = 0;
    s16 count = 0;
    s16 total = 0;

    for (i = 0; i < IveSave[0].owned_zoid_count; i++) {
        count = menuSeartchPartsZoid(part, i);
        total += count;
    }
    return total;
}

void menuDelPartsHave(s16 part)
{
    s16 i = 0;

    if (IveSave[0].owned_part_count > 0) {
        for (i = 0; i < IveSave[0].owned_part_count; i++) {
            if (IveSave[0].owned_parts[i] == part) {
                IveSave[0].owned_parts[i] = IveSave[0].owned_parts[IveSave[0].owned_part_count - 1];
                IveSave[0].owned_part_count--;
                printf("\x83p\x81[\x83" "c\x8d\xed\x8f\x9cOK pno=%d num=%d %s\n", part, IveSave[0].owned_part_count, Tea_partsdat[part].name);
                return;
            }
        }
        printf("\x83p\x81[\x83" "c\x8d\xed\x8f\x9c\x82\xc8\x82\xb5 pno=%d num=%d %s\n", part, IveSave[0].owned_part_count, Tea_partsdat[part].name);
    }
}
#undef lit_2316
#define lit_2316 "\x83p\x81[\x83" "c\x8d\xed\x8f\x9cOK pno=%d num=%d %s\n"
#undef lit_2317
#define lit_2317 "\x83p\x81[\x83" "c\x8d\xed\x8f\x9c\x82\xc8\x82\xb5 pno=%d num=%d %s\n"

void menuSortPartsHave(void)
{
    s16 i = 0;
    s16 j = 0;
    s16 tmp = 0;
    s16 a = 0;
    s16 dead0 = 0;
    s16 dead1 = 0;

    if (IveSave[0].owned_part_count > 1) {
        for (j = 0; j < IveSave[0].owned_part_count - 1; j++) {
            for (i = 0; i < IveSave[0].owned_part_count - 1; i++) {
                tmp = IveSave[0].owned_parts[i];
                if (IveSave[0].owned_parts[i] > IveSave[0].owned_parts[i + 1]) {
                    IveSave[0].owned_parts[i] = IveSave[0].owned_parts[i + 1];
                    IveSave[0].owned_parts[i + 1] = tmp;
                    (void)a;
                }
            }
        }
        for (j = 0; j < IveSave[0].owned_part_count - 1; j++) {
            for (i = 0; i < IveSave[0].owned_part_count - 1; i++) {
                tmp = IveSave[0].owned_parts[i];
                dead0 = Tea_partsdat[tmp].category;
                if (dead0 > (a = Tea_partsdat[(dead1 = IveSave[0].owned_parts[i + 1])].category)) {
                    IveSave[0].owned_parts[i] = IveSave[0].owned_parts[i + 1];
                    IveSave[0].owned_parts[i + 1] = tmp;
                    (void)dead1;
                    (void)a;
                }
            }
        }
    }
}

void ttljobjcopyposdispall(HSD_JObj *src, HSD_JObj *dst)
{
    if (src != NULL && dst != NULL) {
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
    }
}

void MessageBox(char *msg)
{
    s16 unused = 0;

    strcpy(menudispyesnostr1, msg);
    strcpy(menudispyesnostr2, "WAIT MSG");
    menudispyesnoflag = 1;
}
#undef lit_2351
#define lit_2351 "WAIT MSG"
#pragma force_active on
static const char lit_2378[45] = "[ttlMemCardChk] 1137 err\x82\xb1\x82\xcc\x82\xdc\x82\xdc\x83Q\x81[\x83\x80\x82\xc5\x81" "A\x81" "A";
static const char lit_2379[34] = "[ttlMemCardChk] SLOT CHK \x8aJ\x8en\x81" "A\x81" "A";
static const char lit_2380[32] = "[ttlMemCardChk] SLOT CHK OK\x81" "A\x81" "A";
static const char lit_2381[35] = "[ttlMemCardChk] SLOT SECTOR OK\x81" "A\x81" "A";
static const char lit_2382[29] = "[ttlMemCardChk] MOUNT OK\x81" "A\x81" "A";
static const char lit_2383[48] = "[ttlMemCardChk] TeaMemCardChk Zoid Chk \x8e\xb8\x94s\x81" "A\x81" "A";
static const char lit_2384[49] = "[ttlMemCardChk] TeaMemCardChk ZoidChk\x8e\xb8\x94s \x8d\xc4\x81" "A\x81" "A";
static const char lit_2385[42] = "[ttlMemCardChk] TeaMemCardChk ZOID OK\x81" "A\x81" "A";
static const char lit_2386[11] = "MOunt\x8b\x96\x89\xc2\n";
static const char lit_2387[35] = "[ttlMemCardChk] ZOID DATS CHK \x81" "A\x81" "A";
static const char lit_2388[33] = "[ttlMemCardChk] ZOID DATS OK\x81" "A\x81" "A";
static const char lit_2389[40] = "[ttlMemCardChk] ZOID DATA \x96\xb3\x82\xb5 1120\x81" "A\x81" "A";
#pragma force_active off

s32 ttlMemCardChkStartNew(void)
{
    s32 error = 0;
    s32 slot = menusaveloadslot;

    ttlMenuLoadYesNo();
    ttlMenuSaveDataSetFileName(-1, 0);
    error = 0;
    while (1) {
        if (error == 1) {
            clrYesNoMsg();
            puts("[SLOT CHK NEW] err 1137 \x82\xb1\x82\xcc\x82\xdc\x82\xdc\x83Q\x81[\x83\x80\x82\xc5\x81" "A\x81" "A");
            if (TEA_mcMsgYesNoNotChkSlot(slot, 0x471) != 0) {
                OSReport("\x82\xb1\x82\xcc\x82\xdc\x82\xdc\x83Q\x81[\x83\x80\n");
                clrYesNoMsg();
                TEA_mcUnMount(slot);
                return 0;
            }
            OSReport("SLOT \x8d\xc4\n");
            clrYesNoMsg();
            TEA_mcUnMount(slot);
            error = 0;
            continue;
        }
        OSReport("SLOT CHK\n");
        puts("[SLOT CHK NEW] SLOTCHK1\x81" "A\x81" "A");
        menu_SlotOn[slot] = TEA_mcChkSlot(slot);
        if (menu_SlotOn[slot] == 0) {
            TEA_mcMsg(slot, 0x47C);
            error = 1;
            continue;
        }
        if (MemCardStatus[slot] == -2) {
            TEA_mcMsgNoChkSlot(slot, 0x472);
            error = 1;
            continue;
        }
        OSReport("SLOT \x82\xa0\x82\xe8 \n");
        puts("[SLOT CHK NEW] SLOTCHK1 OK\x81" "A\x81" "A");
        puts("[SLOT CHK NEW] CHK SECTOR\x81" "A\x81" "A");
        if (TEA_mcChkSector(slot) == 0) {
            TEA_mcMsgNoChkSlot(slot, 0x47F);
            error = 1;
            continue;
        }
        puts("[SLOT CHK NEW] SECTOR OK\x81" "A\x81" "A");
        puts("[SLOT CHK NEW] MOUNT \x8aJ\x8en\x81" "A\x81" "A");
        if (TEA_mcMountNoChkError(slot) == 0) {
            if (MemCardStatus[slot] == -2) {
                TEA_mcMsgNoChkSlot(slot, 0x472);
                error = 1;
                continue;
            }
            if (MemCardStatus[slot] == -128) {
                TEA_mcMsgNoChkSlot(slot, 0x477);
                error = 1;
                continue;
            }
            if (MemCardStatus[slot] == -5) {
                TEA_mcMsgNoChkSlot(slot, 0x477);
                error = 1;
                continue;
            }
            if (MemCardStatus[slot] == -13) {
                if (TEA_mcBadEncFormatEnc(slot) == 1) {
                    error = 0;
                    continue;
                }
                if (TEA_mcChkSlot(slot) == 0)
                    TEA_mcMsgNoChkSlot(slot, 0x47C);
                error = 1;
                TEA_mcUnMount(slot);
                continue;
            }
        } else if (MemCardStatus[slot] == -6) {
            if (TEA_mcBadEncFormatEnc(slot) == 1) {
                error = 0;
                TEA_mcUnMount(slot);
                continue;
            }
            error = 1;
            TEA_mcUnMount(slot);
            continue;
        }
        puts("[SLOT CHK NEW] MOUNT OK\x81" "A\x81" "A");
        OSReport("SLOT IOERR CHK\n");
        if (TEA_mcChkIoErr(slot) == 0) {
            OSReport("SLOT IOERR \x8eg\x82\xa6\x82\xc8\x82\xa2\x83\x81\x83" "f\x83" "B\x83" "A\n");
            if (MemCardStatus[slot] == -2)
                TEA_mcMsgNoChkSlot(slot, 0x472);
            if (MemCardStatus[slot] == -5)
                TEA_mcMsgNoChkSlot(slot, 0x477);
            if (MemCardStatus[slot] == -128)
                TEA_mcMsgNoChkSlot(slot, 0x477);
            error = 1;
            continue;
        }
        puts("[SLOT CHK NEW] CHK BROKEN\x81" "A\x81" "A");
        if (TEA_mcChkBroken(slot) == 0 && MemCardStatus[slot] == -6) {
            clrYesNoMsg();
            if (TEA_mcBadEncFormatEnc(slot) == 1) {
                clrYesNoMsg();
                error = 0;
                continue;
            }
            clrYesNoMsg();
            if (TEA_mcChkSlot(slot) == 0) {
                clrYesNoMsg();
                TEA_mcMsgNoChkSlot(slot, 0x47C);
                clrYesNoMsg();
                error = 1;
                TEA_mcUnMount(slot);
                continue;
            }
            clrYesNoMsg();
            error = 1;
            TEA_mcUnMount(slot);
            continue;
        }
        puts("[SLOT CHK NEW] BROKEN CHK OK\x81" "A\x81" "A");
        clrYesNoMsg();
        OSReport("SLOT \x83Z\x83N\x83^\x83T\x83" "C\x83Y ok\n");
        puts("[SLOT CHK NEW] TeaMemCardChk \x8aJ\x8en\x81" "A\x81" "A");
        if (TeaMemCardChk() == 0) {
            puts("[SLOT CHK NEW] TeaMemCardChk \x8e\xb8\x94s\x81" "A\x81" "A");
            error = 1;
            TEA_mcUnMount(slot);
            continue;
        }
        clrYesNoMsg();
        puts("[SLOT CHK NEW] TeaMemCardChk OK\x81" "A\x81" "A");
        OSReport(lit_2386);
        ZoidFileBroken = 0;
        if (ttlMenuSaveLoadexistenceDataStartNew0() != 0) {
            puts("ZOID \x83" "f\x81[\x83^\x83`\x83" "F\x83" "b\x83NOK");
            TEA_mcUnMount(slot);
            return 1;
        }
        puts("ZOID \x83" "f\x81[\x83^\x83`\x83" "F\x83" "b\x83N \x83~\x83X");
        if (ZoidFileBroken == 1) {
            ZoidFileBroken = 0;
            if (TEA_mcChkSlot(slot) == 0) {
                TEA_mcMsgNoChkSlot(slot, 0x47C);
                error = 1;
                continue;
            }
            TEA_mcMsgNoChkSlot(slot, 4);
            TEA_mcMsgNoChkSlot(slot, 5);
            puts("ZOID \x83" "f\x81[\x83^\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xe9");
            error = 1;
            continue;
        }
        if (TEA_mcMsgYesNo(slot, 0x480) != 0) {
            TEA_mcMsgDisp(slot, 0x478);
            OSReport("Zoid SystemData \x8d\xec\x90\xac\x8aJ\x8en\n");
            if (ttlMenuSaveLoadexistenceData0(1) != 0) {
                menudispyesnoflag = 0;
                return 1;
            }
            TEA_mcUnMount(slot);
            if (ZoidFileBroken == 1)
                TEA_mcMsgNoChkSlot(slot, 0x477);
            ZoidFileBroken = 0;
            error = 1;
            continue;
        }
        TEA_mcUnMount(slot);
        error = 1;
    }
}
#undef lit_2462
#define lit_2462 "[SLOT CHK NEW] err 1137 \x82\xb1\x82\xcc\x82\xdc\x82\xdc\x83Q\x81[\x83\x80\x82\xc5\x81" "A\x81" "A"
#undef lit_2463
#define lit_2463 "\x82\xb1\x82\xcc\x82\xdc\x82\xdc\x83Q\x81[\x83\x80\n"
#undef lit_2464
#define lit_2464 "SLOT \x8d\xc4\n"
#undef lit_2465
#define lit_2465 "SLOT CHK\n"
#undef lit_2466
#define lit_2466 "[SLOT CHK NEW] SLOTCHK1\x81" "A\x81" "A"
#undef lit_2467
#define lit_2467 "SLOT \x82\xa0\x82\xe8 \n"
#undef lit_2468
#define lit_2468 "[SLOT CHK NEW] SLOTCHK1 OK\x81" "A\x81" "A"
#undef lit_2469
#define lit_2469 "[SLOT CHK NEW] CHK SECTOR\x81" "A\x81" "A"
#undef lit_2470
#define lit_2470 "[SLOT CHK NEW] SECTOR OK\x81" "A\x81" "A"
#undef lit_2471
#define lit_2471 "[SLOT CHK NEW] MOUNT \x8aJ\x8en\x81" "A\x81" "A"
#undef lit_2472
#define lit_2472 "[SLOT CHK NEW] MOUNT OK\x81" "A\x81" "A"
#undef lit_2473
#define lit_2473 "SLOT IOERR CHK\n"
#undef lit_2474
#define lit_2474 "SLOT IOERR \x8eg\x82\xa6\x82\xc8\x82\xa2\x83\x81\x83" "f\x83" "B\x83" "A\n"
#undef lit_2475
#define lit_2475 "[SLOT CHK NEW] CHK BROKEN\x81" "A\x81" "A"
#undef lit_2476
#define lit_2476 "[SLOT CHK NEW] BROKEN CHK OK\x81" "A\x81" "A"
#undef lit_2477
#define lit_2477 "SLOT \x83Z\x83N\x83^\x83T\x83" "C\x83Y ok\n"
#undef lit_2478
#define lit_2478 "[SLOT CHK NEW] TeaMemCardChk \x8aJ\x8en\x81" "A\x81" "A"
#undef lit_2479
#define lit_2479 "[SLOT CHK NEW] TeaMemCardChk \x8e\xb8\x94s\x81" "A\x81" "A"
#undef lit_2480
#define lit_2480 "[SLOT CHK NEW] TeaMemCardChk OK\x81" "A\x81" "A"
#undef lit_2481
#define lit_2481 "ZOID \x83" "f\x81[\x83^\x83`\x83" "F\x83" "b\x83NOK"
#undef lit_2482
#define lit_2482 "ZOID \x83" "f\x81[\x83^\x83`\x83" "F\x83" "b\x83N \x83~\x83X"
#undef lit_2483
#define lit_2483 "ZOID \x83" "f\x81[\x83^\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xe9"
#undef lit_2484
#define lit_2484 "Zoid SystemData \x8d\xec\x90\xac\x8aJ\x8en\n"

s32 ttlMenuSystemCardLoad(void)
{
    s32 error = 0;
    s32 slot = menusaveloadslot;

    ttlMenuLoadYesNo();
    ttlMenuSaveDataSetFileName(-1, 0);
    puts("ttlMenuSystemCardLoad");
    error = 0;
    while (1) {
        if (error == 1) {
            error = 1;
            TEA_mcUnMount(slot);
            puts("ttlMenuSystemCardLoad FALSE");
            return 0;
        }
        menu_SlotOn[slot] = TEA_mcChkSlotNoMsg(slot);
        if (menu_SlotOn[slot] == 0) {
            error = 1;
            continue;
        }
        if (MemCardStatus[slot] == -2) {
            error = 1;
            continue;
        }
        puts("[SYSTEM LOAD] CHK SECTOR\x81" "A\x81" "A");
        if (TEA_mcChkSector(slot) == 0) {
            error = 1;
            continue;
        }
        puts("[SYSTEMLOAD] SECTOR OK\x81" "A\x81" "A");
        if (TEA_mcMountNoChkError(slot) == 0) {
            if (MemCardStatus[slot] == -2) {
                error = 1;
                continue;
            }
            if (MemCardStatus[slot] == -5) {
                error = 1;
                continue;
            }
            if (MemCardStatus[slot] == -13) {
                error = 1;
                TEA_mcUnMount(slot);
                continue;
            }
        } else {
            if (MemCardStatus[slot] == -6) {
                error = 1;
                TEA_mcUnMount(slot);
                continue;
            }
        }
        puts("ttlMenuSystemCardLoad MountOk");
        if (TEA_mcChkSector(slot) == 0) {
            error = 1;
            continue;
        }
        puts("ttlMenuSystemCardLoad SectorOk");
        if (TEA_mcChkBroken(slot) == 0) {
            error = 1;
            continue;
        }
        puts("ttlMenuSystemCardLoad BrokenOk");
        if (TEA_mcChkIoErr(slot) == 0) {
            error = 1;
            continue;
        }
        puts("ttlMenuSystemCardLoad IoErrOk");
        clrYesNoMsg();
        if (TeaMemCardChk() == 0) {
            error = 1;
            TEA_mcUnMount(slot);
            continue;
        }
        clrYesNoMsg();
        ZoidFileBroken = 0;
        if (ttlMenuSaveLoadexistenceDataStartNew0() != 0) {
            puts(lit_2481);
            TEA_mcUnMount(slot);
            return 1;
        }
        puts(lit_2482);
        if (ZoidFileBroken == 1) {
            ZoidFileBroken = 0;
            error = 1;
            continue;
        }
        TEA_mcUnMount(slot);
        ZoidFileBroken = 0;
        error = 1;
    }
}
#undef lit_2523
#define lit_2523 "ttlMenuSystemCardLoad"
#undef lit_2524
#define lit_2524 "ttlMenuSystemCardLoad FALSE"
#undef lit_2525
#define lit_2525 "[SYSTEM LOAD] CHK SECTOR\x81" "A\x81" "A"
#undef lit_2526
#define lit_2526 "[SYSTEMLOAD] SECTOR OK\x81" "A\x81" "A"
#undef lit_2527
#define lit_2527 "ttlMenuSystemCardLoad MountOk"
#undef lit_2528
#define lit_2528 "ttlMenuSystemCardLoad SectorOk"
#undef lit_2529
#define lit_2529 "ttlMenuSystemCardLoad BrokenOk"
#undef lit_2530
#define lit_2530 "ttlMenuSystemCardLoad IoErrOk"
#pragma force_active on
static const char lit_2692[3] = "01";
static const char lit_2693[3] = "02";
static const char lit_2694[3] = "03";
static const char lit_2695[4] = "000";
static const char lit_2696[4] = "04A";
static const char lit_2697[3] = "05";
static const char lit_2698[3] = "06";
static const char lit_2699[3] = "07";
static const char lit_2700[3] = "08";
static const char lit_2701[3] = "09";
static const char lit_2702[3] = "10";
static const char lit_2703[3] = "11";
static const char lit_2704[4] = "04B";
static const char lit_2705[4] = "16B";
static const char lit_2706[4] = "12A";
static const char lit_2707[4] = "13A";
static const char lit_2708[4] = "14A";
static const char lit_2709[4] = "15A";
static const char lit_2710[4] = "16A";
static const char lit_2711[4] = "12B";
static const char lit_2712[4] = "13B";
static const char lit_2713[4] = "14B";
static const char lit_2714[4] = "15B";
#pragma force_active off

void menu_sDispSaveLoad(void)
{
    s32 unused0 = 0;
    s32 unused1 = 0;
    s32 unused2 = 0;
    s32 unused3 = 0;
    s32 dead = 0;
    GXColor color;

    if (menu_saveloadp == NULL) {
        OSReport("non menu_saveloadp\n");
    } else {
        HSD_FogSet(0);
        HSD_SetZSortMode(0, 0);
        color = (GXColor){ 0, 0, 0, 0 };
        HSD_VIData.bg_color = color;
        HSD_StartRender(0);
        if (HSD_CObjSetCurrent(((MenuArchive *)menu_saveloadp)->cobj) != 0) {
            HSD_LObjSetCurrentAll(((MenuArchive *)menu_saveloadp)->lobj);
            HSD_LObjSetupInit(((MenuArchive *)menu_saveloadp)->cobj);
            GXSetCullMode(0);
            if (menu_saveloadbgp != NULL)
                jobjDispAll(GetChild(((MenuArchive *)menu_saveloadbgp)->joints[0], -1, -1, -1, -1, -1));
            menu_sDispSaveLoadTitle();
            menu_sDispSaveLoadTitleSubButton();
            menu_sDispSaveLoadDataWindow();
            menu_sDispYesNoMenu();
            HSD_CObjEndCurrent();
        }
        HSD_EndRender();
        GXSetCullMode(2);
        debugmenudraw();
        vsyncN++;
        dead = 1;
    }
}
#undef lit_2728
#define lit_2728 "non menu_saveloadp\n"

void menu_sDispSaveLoadTitle(void)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 unused2 = 0;
    s32 unused3 = 0;
    s32 unused4 = 0;
    s32 kind = 0;
    s32 x;
    s32 y;

    kind = 0;
    x = 0;
    kind = 0;
    y = 0;
    x = 0;
    SetAnimArc2((MenuArchive *)menu_saveloadp, kind, 1);
    jobjAnim(((MenuArchive *)menu_saveloadp)->joints[kind]);
    ttlMenuDispParts(((MenuArchive *)menu_saveloadp)->joints[kind], 0, -1, -1, -1);
    ttlMenuDispParts(((MenuArchive *)menu_saveloadp)->joints[kind], 0, menusaveloadmode, -1, -1);
    x = 1;
    y = 1;
}

void menu_sDispSaveLoadTitleSubButton(void)
{
    s32 dead = 0;
    s32 kind = 0;
    s16 i = 0;
    s16 count = 0;
    s16 guide = 0;
    HSD_JObj *from = 0;
    HSD_JObj *to = 0;
    s32 all = 0;

    dead = 0;
    kind = 0;
    count = 0;
    for (i = 1; i < 7; i++) {
        if (menutitleguide[i] != -1)
            count++;
    }
    for (i = 1; i < 7; i++) {
        if (menutitleguide[i] != -1) {
            kind = 1;
            count = menutitleanm[i].state;
            if (menutitleanm[i].state != menutitleanm[i].prev) {
                menutitleanm[i].mode = 1;
                menutitleanm[i].prev = menutitleanm[i].state;
            }
            if (menutitleanm[i].mode == 3 && menutitleanm[i].state == 0)
                menutitleanm[i].state = 1;
            if (menutitleanm[i].mode == 3 && menutitleanm[i].state == 1)
                menutitleanm[i].mode = 1;
            if (menutitleanm[i].mode == 1) {
                menutitleanm[i].mode = 2;
                if (i == 4) {
                    SetAnimArc2((MenuArchive *)menu_saveloadp, kind, 0);
                    OSReport("menu_saveloadp 1-0 %d\n", i);
                }
                if (i == 1)
                    (void)0;
                if (i == 4)
                    count = 1;
            }
        }
    }
    for (i = 1; i < 7; i++) {
        guide = menutitleguide[i];
        if (guide != -1) {
            kind = 1;
            if (menutitleanm[i].mode == 2) {
                jobjAnim1(GetChild(((MenuArchive *)menu_saveloadp)->joints[kind], 0, 4, guide, -1, -1));
                if (all != 0) {
                    for (i = 1; i < 7; i++)
                        menutitleanm[i].mode = 3;
                }
                break;
            }
        }
    }
    for (i = 1; i < 7; i++) {
        if (menutitleguide[i] != -1)
            count = 1;
    }
    if (menutitleguide[1] != -1) {
        kind = 1;
        from = GetChild(((MenuArchive *)menu_saveloadp)->joints[kind], (count = menutitleguide[1], 0), 3, -1, -1, -1);
        if (menutitleguide[3] == -1)
            to = GetChild(((MenuArchive *)menu_saveloadp)->joints[kind], 0, 0, 0, -1, -1);
        else
            to = GetChild(((MenuArchive *)menu_saveloadp)->joints[kind], 0, 0, 1, -1, -1);
        jobjCopyPos(from, to);
        jobjDispAll(to);
    }
    if (menutitleguide[4] != -1) {
        kind = 1;
        from = GetChild(((MenuArchive *)menu_saveloadp)->joints[kind], (count = menutitleguide[4], 0), 2, -1, -1, -1);
        if (menutitleguide[5] != -1) {
            if (menutitleguide[6] == -1)
                to = GetChild(((MenuArchive *)menu_saveloadp)->joints[kind], 0, 1, 0, -1, -1);
            else
                to = GetChild(((MenuArchive *)menu_saveloadp)->joints[kind], 0, 1, 1, -1, -1);
            if (to != NULL && from != NULL) {
                jobjCopyPos(from, to);
                jobjDispAll(to);
            }
        }
    }
    for (i = 1; i < 7; i++) {
        guide = menutitleguide[i];
        if (guide != -1) {
            kind = 1;
            if (i <= 3)
                from = GetChild(((MenuArchive *)menu_saveloadp)->joints[kind], 0, 0, i + 1, -1, -1);
            else
                from = GetChild(((MenuArchive *)menu_saveloadp)->joints[kind], 0, 1, i - 2, -1, -1);
            to = GetChild(((MenuArchive *)menu_saveloadp)->joints[kind], 0, 4, guide, -1, -1);
            if (from != NULL && to != NULL) {
                jobjCopyPos(from, to);
                HSD_JObjDispAll(to, 0, 7, 0);
            }
        }
    }
}
#undef lit_2835
#define lit_2835 "menu_saveloadp 1-0 %d\n"

void menu_sDispSaveLoadDataWindow(void)
{
    s16 unused0 = 5;
    s32 unused1 = 0;
    s32 unused2 = 0;
    s32 unused3 = 0;
    s32 i = 0;
    s16 model = 5;
    s32 done = 0;

    if (menusaveloadDataSelect == 0)
        return;
    if (menusaveloadDatamainanm.state == 0)
        return;
    if (menusaveloadDatamainanm.state == 1) {
        SetAnimArc2((MenuArchive *)menu_saveloadp, model, menusaveloadDatamainanm.anim);
        menusaveloadDatamainanm.state = 2;
    }
    if (menusaveloadDatamainanm.state == 2 && (done = jobjAnim(((MenuArchive *)menu_saveloadp)->joints[model])) != 0) {
        menusaveloadDatamainanm.state = 3;
        switch (menusaveloadDatamainanm.anim) {
        case 0:
            menusaveloadDatamainanm.anim = 1;
            menusaveloadDatamainanm.state = 2;
            break;
        case 2:
            menusaveloadDatamainanm.state = 0;
            return;
        case 1:
        default:
            menusaveloadDatamainanm.state = 2;
            break;
        }
        SetAnimArc2((MenuArchive *)menu_saveloadp, model, menusaveloadDatamainanm.anim);
        jobjAnim(((MenuArchive *)menu_saveloadp)->joints[model]);
    }
    jobjDisp(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, -1, -1, -1, -1));
    jobjDisp(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 0, -1, -1, -1));
    jobjDisp(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 0, menusaveloadslot, -1, -1));
    jobjDispAll(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 1, -1, -1, -1));
    if (GameMode.game_type == 1)
        model = 7;
    else if (GameMode.game_type == 4)
        model = 6;
    if (menusaveloadDatamainanm.anim != 1)
        return;
    if (menusaveloadDataNoanm.state == 0)
        return;
    if (menusaveloadDataNoanm.state == 1) {
        SetAnimArc2((MenuArchive *)menu_saveloadp, model, menusaveloadDataNoanm.anim);
        menusaveloadDataNoanm.state = 2;
    }
    if (menusaveloadDataNoanm.state == 2 && (done = jobjAnim(((MenuArchive *)menu_saveloadp)->joints[model])) != 0) {
        menusaveloadDataNoanm.state = 3;
        switch (menusaveloadDataNoanm.anim) {
        case 0:
            menusaveloadDataNoanm.anim = 1;
            menusaveloadDataNoanm.state = 2;
            break;
        case 2:
            menusaveloadDataNoanm.state = 0;
            return;
        case 1:
        default:
            menusaveloadDataNoanm.state = 2;
            break;
        }
        SetAnimArc2((MenuArchive *)menu_saveloadp, model, menusaveloadDataNoanm.anim);
        jobjAnim(((MenuArchive *)menu_saveloadp)->joints[model]);
    }
    for (i = 0; i < 4; i++)
        menu_sDispSaveLoadDataNo(i + menuSLPos[2], i);
}

void menu_sDispSaveLoadDataNo(s16 no, s16 slot)
{
    s16 five = 5;
    s16 hour = 0;
    s16 model = GameMode.game_type + 6;
    s16 minute = 0;
    s16 second = 0;
    s32 money = 0;
    s32 year = 0;
    HSD_JObj *j1 = 0;
    HSD_JObj *j2 = 0;
    s32 u0 = 0;
    s32 u1 = 0;
    char buf[80];

    (void)model;

    if (GameMode.game_type == 4)
        model = 6;
    else if (GameMode.game_type == 1)
        model = 7;
    j1 = GetChild(((MenuArchive *)menu_saveloadp)->joints[five], 0, 1, slot, -1, -1);
    j2 = GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, -1, -1, -1, -1);
    jobjCopyPos(j1, j2);
    jobjDisp(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, -1, -1, -1, -1));
    jobjDisp(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 0, -1, -1, -1));
    jobjDisp(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 0, no, -1, -1));
    if (menu_SlotDataOn[no] == 1) {
        jobjDispAll(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 1, -1, -1, -1));
        jobjDispAll(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 2, -1, -1, -1));
        jobjDispAll(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 3, -1, -1, -1));
        if (GameMode.game_type == 4) {
            year = IveSave[no + 5].save_year % 100;
            sprintf(buf, "%02d/%02d/%02d", year, IveSave[no + 5].save_month + 1, IveSave[no + 5].save_day);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 2, 0, -1, -1), 4, buf, 0);
            hour = IveSave[no + 5].play_time / 3600;
            if (hour > 99)
                hour = 99;
            minute = IveSave[no + 5].play_time / 60 % 60;
            if (minute > 60)
                minute = 60;
            second = (u32)IveSave[no + 5].play_time % 60;
            if (second > 60)
                second = 60;
            sprintf(buf, "%02d", hour);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 3, 0, -1, -1), 1, buf, 0);
            sprintf(buf, "%02d", minute);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 3, 1, -1, -1), 1, buf, 0);
            sprintf(buf, "%02d", second);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 3, 2, -1, -1), 1, buf, 0);
            j1 = GetChild(((MenuArchive *)menu_saveloadp)->joints[6], 0, 4, -1, -1, -1);
            j2 = GetChild(((MenuArchive *)menu_saveloadp)->joints[8], 0, (s8)IveSave[no + 5].country - 1, -1, -1, -1);
            jobjCopyPos(j1, j2);
            jobjDispAll(j2);
            j1 = GetChild(((MenuArchive *)menu_saveloadp)->joints[6], 0, 5, -1, -1, -1);
            j2 = GetChild(((MenuArchive *)menu_saveloadp)->joints[9], 0, -1, -1, -1, -1);
            jobjCopyPos(j1, j2);
            sMakeMatrix(j2);
            sprintf(buf, "%s", (char *)stagename_mis[IveSave[no + 5].stage]);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[9], 0, 0, 0, -1, -1), 1, buf, 0);
            jobjDisp(GetChild(((MenuArchive *)menu_saveloadp)->joints[9], 0, 0, 1, 0, -1));
            jobjDisp(GetChild(((MenuArchive *)menu_saveloadp)->joints[9], 0, 0, -1, -1, -1));
        } else {
            year = IveSave[no + 5].save_year % 100;
            sprintf(buf, "%02d/%02d/%02d", year, IveSave[no + 5].save_month + 1, IveSave[no + 5].save_day);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 2, 0, -1, -1), 4, buf, 0);
            sprintf(buf, "BATTLE%02d", IveSave[no + 5].stage + 1);
            if (IveSave[no + 5].field_1C87 == 1)
                strcat(buf, "\x83\x74\x83\x40\x81\x5b\x83\x80");
            else
                strcat(buf, "\x83\x4e\x83\x8a\x83\x41");
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 4, -1, -1, -1), 1, buf, 0);
            hour = IveSave[no + 5].play_time / 3600;
            if (hour > 99)
                hour = 99;
            minute = IveSave[no + 5].play_time / 60 % 60;
            if (minute > 60)
                minute = 60;
            second = (u32)IveSave[no + 5].play_time % 60;
            if (second > 60)
                second = 60;
            sprintf(buf, "%02d", hour);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 3, 0, -1, -1), 1, buf, 0);
            sprintf(buf, "%02d", minute);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 3, 1, -1, -1), 1, buf, 0);
            sprintf(buf, "%02d", second);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 3, 2, -1, -1), 1, buf, 0);
            money = IveSave[no + 5].money;
            if (money > 9999999)
                money = 9999999;
            sprintf(buf, "%-7d ", money);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 6, -1, -1, -1), 1, buf, 0);
            sprintf(buf, "%-3d", IveSave[no + 5].owned_zoid_count);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 5, -1, -1, -1), 1, buf, 0);
            sprintf(buf, "%s", Tea_persondat[IveSave[no + 5].field_1600].name);
            putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 8, 0, -1, -1), 1, buf, 0);
            if ((s32)IveSave[no + 5].single_team == 1) {
                (void)(IveSave[0].money + 1);
                sprintf(buf, "%s", Tea_persondat[IveSave[no + 5].field_1602].name);
                putAscJobjLR(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 8, 1, -1, -1), 1, buf, 0);
            }
        }
    } else {
        if (GameMode.game_type == 4)
            putAscJobj(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 6, -1, -1, -1), 1, " NO DATA");
        else
            putAscJobj(GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, 7, -1, -1, -1), 1, " NO DATA");
    }
    if (menuSLPos[0] != slot) {
        j1 = GetChild(((MenuArchive *)menu_saveloadp)->joints[model], 0, -1, -1, -1, -1);
        j2 = GetChild(((MenuArchive *)menu_saveloadp)->joints[12], 0, -1, -1, -1, -1);
        jobjCopyPos(j1, j2);
        jobjDispAll(j2);
    }
}
#undef lit_2911
#define lit_2911 "%02d/%02d/%02d"
#undef lit_2912
#define lit_2912 "%02d"
#undef lit_2913
#define lit_2913 "%s"
#undef lit_2914
#define lit_2914 "BATTLE%02d"
#undef lit_2915
#define lit_2915 "\x83t\x83@\x81[\x83\x80"
#undef lit_2916
#define lit_2916 "\x83N\x83\x8a\x83" "A"
#undef lit_2917
#define lit_2917 "%-7d "
#undef lit_2918
#define lit_2918 "%-3d"
#undef lit_2919
#define lit_2919 " NO DATA"

void ttlMenuPurgeSaveLoad(void)
{
    if (menu_saveloadbgp != NULL) {
        PurgeArc2(menu_saveloadbgp);
        menu_saveloadbgp = NULL;
    }
    if (menu_saveloadp != NULL) {
        PurgeArc2(menu_saveloadp);
        menu_saveloadp = NULL;
    }
    HSD_VIWaitEFBFlush();
}

void ttlMenuLoadSaveLoad(void)
{
    ttlMenuPurgeSaveLoad();
    menu_saveloadp = LoadArc2("menu/saveload/saveload.dat");
    menu_saveloadbgp = LoadArc2("menu/saveload/bgtitle.dat");
}
#undef lit_2946
#define lit_2946 "menu/saveload/saveload.dat"
#undef lit_2947
#define lit_2947 "menu/saveload/bgtitle.dat"

void ttlMenuKeyClr1P2P(s32 pad1, s32 pad2)
{
    if (pad1 == 0) {
        sPad1->pressed_a = 0;
        sPad1->field_5E = 0;
        sPad1->field_5F = 0;
        sPad1->field_60 = 0;
        sPad1->field_61 = 0;
        sPad1->field_47 = 0;
        sPad1->field_63 = 0;
        sPad1->field_5B = 0;
        sPad1->field_5C = 0;
        sPad1->field_57 = 0;
        sPad1->field_58 = 0;
        sPad1->field_59 = 0;
        sPad1->field_5A = 0;
        sPad1->field_4F = 0;
        sPad1->field_50 = 0;
        sPad1->field_51 = 0;
        sPad1->field_52 = 0;
    }
    if (pad2 == 0) {
        sPad2->pressed_a = 0;
        sPad2->field_5E = 0;
        sPad2->field_5F = 0;
        sPad2->field_60 = 0;
        sPad2->field_61 = 0;
        sPad2->field_47 = 0;
        sPad2->field_63 = 0;
        sPad2->field_5B = 0;
        sPad2->field_5C = 0;
        sPad2->field_57 = 0;
        sPad2->field_58 = 0;
        sPad2->field_59 = 0;
        sPad2->field_5A = 0;
        sPad2->field_4F = 0;
        sPad2->field_50 = 0;
        sPad2->field_51 = 0;
        sPad2->field_52 = 0;
    }
}
extern s32 TEA_mcMsgYesNoEx(s32 slot, s16 message);
extern s32 TEA_menufnc12p(s16 *a, s16 *b, s16 c, s16 d);
extern s64 gametimenow;
extern u32 __OSBusClock : 0x800000F8;
s32 ttlMenuBatSaveLoad1P2P(s32 mode, s32 a, s32 b)
{
    s16 x21 = 0;
    s16 slot = 0;
    s16 x23 = 0;
    s32 x20 = 0;
    register s32 ret = 0;
    register s32 done = 0;
    s32 i;

    OSReport((char *)lit_732);
    MemCardKeepBlock = 10;
    if (a != 0) {
        slot = 0;
    } else {
        slot = 1;
    }
    menusaveloadslot = slot;
    ttlMenuLoadSaveLoad();
    menu_SlotOn[slot] = TEA_mcChkSlot(slot);
    if (GameMode.game_type == 4) {
        x23 = 0;
        if (GameMode.field_2C == 0)
            x23 = 2;
        x21 = x23;
        if (mode) {
            (void)0;
        }
        if (mode != 0) {
            x21 = 0;
        } else {
            x21 = 1;
        }
    } else {
        if (mode != 0) {
            x21 = 2;
        } else {
            x21 = 3;
        }
    }
    menusaveloadmode = x21;
    menuSetTitle(x21);
    menuSetTitleButton(0xb);
    if (mode == 1) {
        x20 = 0;
    } else {
        x20 = 1;
    }
    menu_SlotOn[slot] = TEA_mcChkSlot(slot);
    OSReport("SLOT CHECK\n");
    menuSLPos[0] = 0;
    menuSLPos[1] = 0;
    menuSLPos[2] = 0;
    menuSLPos[3] = 4;
    menuSLPos[4] = 4;
    menusaveloadDataSelect = 1;
    menusaveloadDatamainanm.anim = 0;
    menusaveloadDatamainanm.state = 1;
    menusaveloadDataNoanm.anim = 1;
    menusaveloadDataNoanm.state = 1;
    ttlMenuSaveDataChk();
    SetMenuMsgAdr(x20 + 0x5f, menudispwinstr);
    strcpy(menusaveloadfilename, MC_FILENAME_MSG);
    flag_saveok = 0;
    for (;;) {
        sPadAll();
        ttlMenuKeyClr1P2P(a, b);
        menu_SlotOn[slot] = TEA_mcChkSlot(slot);
        if (menu_SlotOn[slot] == 0) {
            ttlMenuPurgeSaveLoad();
            return 0;
        }
        done = 0;
        menuSLPos[0] = (u16)TEA_menufnc12p(&menuSLPos[1], &menuSLPos[2], menuSLPos[3], menuSLPos[4]);
        menu_sDispSaveLoad();
        if (sPad1->pressed_a || sPad2->pressed_a) {
            SeRet();
            if (GameMode.game_type == 4) {
                x23 = 0;
            } else {
                x23 = 1;
            }
            strcpy(menusaveloadfilename, MC_FILENAME_MSG);
            if (mode == 1) {
                OSReport("save -1 \n");
                clrYesNoMsg();
                menudispyesnoflag = 1;
                menudispyesnopoint = 1;
                for (;;) {
                    sPadAll();
                    ttlMenuKeyClr1P2P(a, b);
                    menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x4d), lit_574);
                    menu_sDispSaveLoad();
                    if (sPad1->pressed_a || sPad2->pressed_a)
                        break;
                    if (TEA_mcChkSlot(slot) == 0) {
                        menuYesNoSetStr(lit_574, lit_574);
                        menu_sDispStartYesNo();
                        menudispyesnoflag = 0;
                        ttlMenuPurgeSaveLoad();
                        TEA_mcUnMount(menusaveloadslot);
                        return 0;
                    }
                }
                if (done == 1)
                    continue;
                clrYesNoMsg();
                menudispyesnoflag = 0;
                if (menudispyesnopoint == 0)
                    continue;
                if (menu_SlotDataOn[menuSLPos[0]] == 1) {
                    clrYesNoMsg();
                    menudispyesnoflag = 1;
                    menudispyesnopoint = 1;
                    for (;;) {
                        sPadAll();
                        ttlMenuKeyClr1P2P(a, b);
                        menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x54), lit_574);
                        menu_sDispSaveLoad();
                        if (TEA_mcChkSlot(slot) == 0) {
                            menuYesNoSetStr(lit_574, lit_574);
                            menu_sDispStartYesNo();
                            menudispyesnoflag = 0;
                            ttlMenuPurgeSaveLoad();
                            return 0;
                        }
                        if (sPad1->pressed_a || sPad2->pressed_a)
                            break;
                    }
                    if (done == 1)
                        continue;
                    menudispyesnoflag = 0;
                    menu_sDispStartYesNo();
                    clrYesNoMsg();
                    if (menudispyesnopoint == 0)
                        continue;
                }
                if (TEA_mcLoadFile0(slot, 1, -1) == 0) {
                    TEA_mcMsg(0, 0x437);
                    return 0;
                }
                if (TEA_mcSaveFile0(slot, 1, -1) == 0) {
                    TEA_mcMsg(0, 0x437);
                    return 0;
                }
                IveSave[0].field_0000 = 1;
                clrYesNoMsg();
                menudispyesnoflag = 1;
                menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x478), GetMenuMsgAdr(0x478));
                menu_sDispStartYesNo();
                IveSave[0].field_0000 = 1;
                gametimenow = (OSGetTime() / (s64)(__OSBusClock / 4)) - gametimestart;
                IveSave[0].play_time = (s32)gametimenow;
                if (TEA_mcSaveFile0(slot, menuSLPos[0] + ((s16)x23 << 2) + 2, 0) != 0) {
                    memcpy(&IveSave[menuSLPos[0] + 5], &IveSave[0], 0x1a90);
                    ttlMenuSaveDataSetFileName(-1, 0);
                    if (TEA_mcSaveFile0(slot, 1, -1) == 0) {
                        TEA_mcMsg(0, 0x437);
                        return 0;
                    }
                    ttlMenuSaveDataChk();
                    menudispyesnopoint = 1;
                    clrYesNoMsg();
                    menudispyesnoflag = 1;
                    for (;;) {
                        sPadAll();
                        ttlMenuKeyClr1P2P(a, b);
                        menuYesNoSetStr(GetMenuMsgAdr(0x55), GetMenuMsgAdr(0x55));
                        menu_sDispStartYesNo();
                        if (TEA_mcChkSlot(slot) == 0) {
                            menuYesNoSetStr(lit_574, lit_574);
                            menu_sDispStartYesNo();
                            menudispyesnoflag = 0;
                            flag_saveok = 1;
                            return 0;
                        }
                        if (sPad1->pressed_a || sPad2->pressed_a)
                            break;
                    }
                    SeRet();
                    menudispyesnoflag = 0;
                    menuYesNoSetStr(lit_574, lit_574);
                    menu_sDispStartYesNo();
                } else {
                    puts("mc SaveError01 1079");
                    TEA_mcMsg(0, 0x437);
                    return 0;
                }
                menudispyesnoflag = 0;
                menu_sDispStartYesNo();
                clrYesNoMsg();
            } else {
                if (menu_SlotDataOn[menuSLPos[0]] == 0)
                    continue;
                clrYesNoMsg();
                menudispyesnoflag = 1;
                menudispyesnopoint = 1;
                for (;;) {
                    sPadAll();
                    ttlMenuKeyClr1P2P(a, b);
                    menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x4e), lit_574);
                    menu_sDispSaveLoad();
                    if (TEA_mcChkSlot(slot) == 0) {
                        menuYesNoSetStr(lit_574, lit_574);
                        menudispyesnoflag = 0;
                        menu_sDispStartYesNo();
                        ttlMenuPurgeSaveLoad();
                        return 0;
                    }
                    if (sPad1->pressed_a || sPad2->pressed_a)
                        break;
                }
                if (done == 1)
                    continue;
                SeRet();
                menudispyesnoflag = 0;
                clrYesNoMsg();
                if (menudispyesnopoint == 0)
                    continue;
                menudispyesnoflag = 1;
                menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x69), GetMenuMsgAdr(0x69));
                menu_sDispStartYesNo();
                if (TEA_mcLoadFile0(slot, menuSLPos[0] + ((s16)x23 << 2) + 2, 2) != 0) {
                    gametimestart = (OSGetTime() / (s64)(__OSBusClock / 4));
                    OSReport("MemLoad End Stage=%d Mapno=%d Single=%d NowZoid=%d NowCust=%d \x90l\x95\xa8=%d \n", (s8)IveSave[2].stage, IveSave[2].map_no, (s8)IveSave[2].single_team, IveSave[2].field_15F4, IveSave[2].zoid_slot);
                    GameMode.map = (s8)IveSave[2].stage;
                    menuZoidBattleStage = IveSave[2].stage;
                    GameMode.field_28 = IveSave[2].map_no;
                    menuZoidBattleModeSingleTeam = IveSave[2].single_team;
                    if ((u8)menuZoidBattleModeSingleTeam == 0) {
                        IveSave[2].partner_slot = 0;
                        if (IveSave[2].zoid_slot == 0)
                            IveSave[2].partner_slot = 1;
                        IveSave[2].field_15FA = IveSave[2].owned_zoids[IveSave[2].partner_slot];
                    }
                    GameMode.win = 1;
                    gametimestart = (OSGetTime() / (s64)(__OSBusClock / 4)) - (s64)(u32)IveSave[2].play_time;
                    gametimenow = (OSGetTime() / (s64)(__OSBusClock / 4)) - gametimestart;
                    IveSave[2].play_time = (s32)gametimenow;
                    menudispyesnopoint = 1;
                    clrYesNoMsg();
                    menudispyesnoflag = 1;
                    for (;;) {
                        sPadAll();
                        ttlMenuKeyClr1P2P(a, b);
                        menuYesNoSetStr(GetMenuMsgAdr(0x56), GetMenuMsgAdr(0x56));
                        menu_sDispStartYesNo();
                        if (sPad1->pressed_a || sPad2->pressed_a)
                            break;
                    }
                    SeRet();
                    menudispyesnoflag = 0;
                    clrYesNoMsg();
                    for (i = 0; i < 60; i++) {
                        (void)ret;
                        (void)done;

                        sPadAll();
                        HSD_StartRender(0);
                        HSD_EndRender();
                        debugmenudraw();
                        vsyncN++;
                    }
                    clearFlagThink(2, GameMode.game_type);
                    ret = 1;
                    goto finish;
                } else {
                    menudispyesnoflag = 0;
                    clrYesNoMsg();
                    menu_sDispStartYesNo();
                    return 0;
                }
            }
        }
        {
            if (sPad1->field_5E == 0 && sPad2->field_5E == 0)
                continue;
            menudispwinstr[0] = 0;
            SeCancel();
            if (mode == 1) {
                if (TEA_mcMsgYesNoEx(slot, 0xc) != 0) {
                    flag_saveok = 1;
                    goto finish;
                }
                if (TEA_mcChkSlot(slot) == 0)
                    goto finish;
                continue;
            } else {
                if (TEA_mcMsgYesNoEx(slot, 0xd) != 0)
                    goto finish;
                if (TEA_mcChkSlot(slot) != 0)
                    continue;
                goto finish;
            }
        }
    }
finish:
    menuYesNoSetStr(lit_574, lit_574);
    menusaveloadDataNoanm.anim = 2;
    menusaveloadDataNoanm.state = 1;
    menusaveloadDatamainanm.anim = 2;
    menusaveloadDatamainanm.state = 1;
    menusaveloadDataSelect = 0;
    TEA_mcUnMount(slot);
    menudispyesnoflag = 0;
    menudispyesnoflag = 0;
    if (mode != 0) {
        OSReport("MemCard SaveTrueEnd \x90l\x95\xa8=%d:%s Zd=%d:%s\n", IveSave[0].field_1600, Tea_persondat[IveSave[0].field_1600].name, IveSave[0].field_15F4, Tea_zoiddat[IveSave[0].field_15F4].name);
    } else {
        (void)menuSLPos[0];
        OSReport("MemCard LoadTrueEnd \x90l\x95\xa8=%d:%s Zd=%d:%s\n", IveSave[2].field_1600, Tea_persondat[IveSave[2].field_1600].name, IveSave[2].field_15F4, Tea_zoiddat[IveSave[2].field_15F4].name);
    }
    menudispyesnoflag = 0;
    ttlMenuPurgeSaveLoad();
    MemCardKeepBlock = 10;
    return ret;
}
#undef lit_3092
#define lit_3092 "SLOT CHECK\n"
#undef lit_3093
#define lit_3093 "save -1 \n"
#undef lit_3094
#define lit_3094 "mc SaveError01 1079"
#undef lit_3095
#define lit_3095 "MemLoad End Stage=%d Mapno=%d Single=%d NowZoid=%d NowCust=%d \x90l\x95\xa8=%d \n"
#undef lit_3096
#define lit_3096 "MemCard SaveTrueEnd \x90l\x95\xa8=%d:%s Zd=%d:%s\n"
#undef lit_3097
#define lit_3097 "MemCard LoadTrueEnd \x90l\x95\xa8=%d:%s Zd=%d:%s\n"

s32 ttlMenuBatSaveLoad(s32 mode)
{
    return ttlMenuBatSaveLoad1P2P(mode, 1, 1);
}

void ttlMenuSaveDataChk(void)
{
    s16 i = 0;
    s16 kind = 0;
    s16 slot = menusaveloadslot;

    if (GameMode.game_type == 4)
        kind = 0;
    else
        kind = 1;
    strcpy(menusaveloadfilename, MC_FILENAME_MSG);
    for (i = 0; i < 4; i++) {
        menu_SlotDataOn[i] = TEA_mcLoadFile0(slot, i + (kind << 2) + 2, i + 5);
        if (menu_SlotDataOn[i] == 1)
            menu_SlotDataOn[i] = IveSave[i + 5].field_0000;
        OSReport("menu_SlotData %d_%d:%d Stage=%d\n", i, menu_SlotDataOn[i], IveSave[i + 5].field_0000, IveSave[i + 5].stage);
    }
}
#undef lit_3200
#define lit_3200 "menu_SlotData %d_%d:%d Stage=%d\n"

void ttlMenuSaveDataSetFileName(s16 a, s16 b)
{
    s16 a_copy = a;
    s16 b_copy = b;
    s16 unused = 0;
    s16 slot = menusaveloadslot;

    strcpy(menusaveloadfilename, MC_FILENAME_MSG);
    strcpy(menusaveloadcommentname, "\x83v\x83\x8c\x83" "C\x83" "f\x81[\x83^\x81@\x81@                                 ");
}
#undef lit_3202
#define lit_3202 "\x83v\x83\x8c\x83" "C\x83" "f\x81[\x83^\x81@\x81@                                 "
#pragma force_active on
static const char lit_3222[37] = "MemSecurableSystemAreaCreate Ok %d \n";
static const char lit_3223[31] = "MemSecurableDataCreate Ok %d \n";
static const u32 lit_3224[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00010001, 0x00010001, 
};
static const u32 lit_3225[5] = {
    0x00000000, 0x00000001, 0x00020003, 0x00000001, 0x00020003, 
};
static const u32 lit_3226[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
static const char lit_3259[54] = "MemExistData USE:%d KeepBlk:%d %d\x83Z\x83N\x83^ \x8b\xf3\x82\xab[%d]DIR \n";
#pragma force_active off

s32 ttlMenuSaveLoadexistenceData0(s32 mode)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 file_a[10] = { -1, -2, 0, 0, 0, 0, 1, 1, 1, 1 };
    s16 file_b[10] = { 0, 0, 0, 1, 2, 3, 0, 1, 2, 3 };
    s16 file_c[10] = { -1, -2, 0, 0, 0, 0, 0, 0, 0, 0 };
    s32 mode_copy = mode;
    s16 index = 0;
    s16 used = 0;
    s32 ok = 0;
    s16 slot = menusaveloadslot;

    MemCardKeepBlock = 10;
    ZoidFileBroken = 0;
    ttlMenuSaveDataSetFileName(file_a[index], file_b[index]);
    strcpy(menusaveloadfilename, MC_FILENAME_MSG);
    if (TEA_mcFileMountExist(menusaveloadslot, menusaveloadfilename) == 1 && TEA_mcLoadFile0(menusaveloadslot, 1, -1) != 0) {
        ok = 1;
        configGet();
    }
    if (ok == 1)
        return 1;
    MemCardKeepBlock = 10;
    OSReport(lit_3259, used, MemCardKeepBlock, MemCardFreeSize, MemCardFreeDir);
    if (MemCardFreeSize < MemCardKeepBlock * 0x2000) {
        TEA_mcMsg(slot, 0x46E);
        TEA_mcMsg(slot, 0x46F);
        return 0;
    }
    CARDFreeBlocks(slot, 0, &MemCardFreeDir);
    if (MemCardFreeDir < 1) {
        TEA_mcMsg(slot, 0x46D);
        TEA_mcMsg(slot, 0x46E);
        TEA_mcMsg(slot, 0x46F);
        return 0;
    }
    ttlMenuSaveDataSetFileName(file_a[index], file_b[index]);
    strcpy(menusaveloadfilename, MC_FILENAME_MSG);
    if (TEA_mcSaveFile0(slot, 1, -1) == 0) {
        ZoidFileBroken = 1;
        return 0;
    }
    return 1;
}
#pragma force_active on
static const u32 lit_3276[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00010001, 0x00010001, 
};
static const u32 lit_3277[5] = {
    0x00000000, 0x00000001, 0x00020003, 0x00000001, 0x00020003, 
};
static const u32 lit_3278[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
static const char lit_3299[34] = "MemExistData USE:%d \x8b\xf3\x82\xab[%d]DIR \n";
static const u32 lit_3300[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00010001, 0x00010001, 
};
static const u32 lit_3301[5] = {
    0x00000000, 0x00000001, 0x00020003, 0x00000001, 0x00020003, 
};
static const u32 lit_3302[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
static const u32 lit_3330[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00010001, 0x00010001, 
};
static const u32 lit_3331[5] = {
    0x00000000, 0x00000001, 0x00020003, 0x00000001, 0x00020003, 
};
static const u32 lit_3332[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
#pragma force_active off

s32 ttlMenuSaveLoadexistenceDataStartNew0(void)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 file_a[10] = { -1, -2, 0, 0, 0, 0, 1, 1, 1, 1 };
    s16 file_b[10] = { 0, 0, 0, 1, 2, 3, 0, 1, 2, 3 };
    s16 file_c[10] = { -1, -2, 0, 0, 0, 0, 0, 0, 0, 0 };
    s16 index = 0;
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 ok = 0;
    s16 slot;

    a = 0;
    b = 0;
    slot = menusaveloadslot;
    c = 0;
    MemCardKeepBlock = 10;
    ttlMenuSaveDataSetFileName(file_a[index], file_b[index]);
    strcpy(menusaveloadfilename, MC_FILENAME_MSG);
    if (TEA_mcFileMountExist(slot, menusaveloadfilename) == 1) {
        if (TEA_mcLoadFile0(slot, 1, -1) != 0) {
            configGet();
            ok = 1;
        } else {
            ZoidFileBroken = 1;
            ok = 0;
        }
    } else {
        ZoidFileBroken = 0;
    }
    if (ok == 0)
        return 0;
    return 1;
}
#pragma force_active on
static const u32 lit_3372[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00010001, 0x00010001, 
};
static const u32 lit_3373[5] = {
    0x00000000, 0x00000001, 0x00020003, 0x00000001, 0x00020003, 
};
static const u32 lit_3374[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
#pragma force_active off
s32 ttlMenuSystemMemCardSave(void)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 file_a[10] = { -1, -2, 0, 0, 0, 0, 1, 1, 1, 1 };
    s16 file_b[10] = { 0, 0, 0, 1, 2, 3, 0, 1, 2, 3 };
    s16 file_c[10] = { -1, -2, 0, 0, 0, 0, 0, 0, 0, 0 };
    s16 index = 0;
    s16 dead = 0;
    s16 slot = menusaveloadslot;

    dead = 0;
    MemCardKeepBlock = 10;
    flag_saveok = 0;
    ttlMenuSaveDataSetFileName(file_a[index], file_b[index]);
    puts("1144 Save\x92\x86");
    TEA_mcMsgDisp(slot, 0x478);
    strcpy(menusaveloadfilename, MC_FILENAME_MSG);
    if (TEA_mcSaveFile0(slot, 1, -1) != 0) {
        puts(" 85 Save\x82\xaa\x8fI\x97\xb9\x82\xb5\x82\xdc\x82\xb5\x82\xbd");
        TEA_mcMsg(slot, 0x55);
        flag_saveok = 1;
        return 1;
    }
    puts("1079 Save\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd");
    TEA_mcMsg(slot, 0x437);
    flag_saveok = 0;
    return 0;
}
#undef lit_3456
#define lit_3456 "1144 Save\x92\x86"
#undef lit_3457
#define lit_3457 " 85 Save\x82\xaa\x8fI\x97\xb9\x82\xb5\x82\xdc\x82\xb5\x82\xbd"
#undef lit_3458
#define lit_3458 "1079 Save\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd"
#pragma force_active on
static const u32 lit_3459[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00010001, 0x00010001, 
};
static const u32 lit_3460[5] = {
    0x00000000, 0x00000001, 0x00020003, 0x00000001, 0x00020003, 
};
static const u32 lit_3461[5] = {
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
#pragma force_active off

s32 TeaMemCardChk(void)
{
    s32 result = 0;
    s32 slot = menusaveloadslot;

    ttlMenuLoadYesNo();
    ttlMenuSaveDataSetFileName(-1, 0);
retry:
    menu_SlotOn[slot] = TEA_mcChkSlot(slot);
    if (menu_SlotOn[slot] == 0) {
        TEA_mcMsgNoChkSlot(slot, 0x47C);
        return 0;
    }
    if (MemCardStatus[slot] == -2) {
        TEA_mcMsgNoChkSlot(slot, 0x472);
        return 0;
    }
    OSReport(lit_2467);
    if (TEA_mcChkSector(slot) == 0) {
        TEA_mcMsgNoChkSlot(slot, 0x47F);
        return 0;
    }
    OSReport(lit_2477);
    if (TEA_mcChkRest(slot) == 0) {
        clrYesNoMsg();
        if (TEA_mcMsgYesNo(slot, 0x476) != 0) {
            clrYesNoMsg();
            if (TEA_mcChkSlot(slot) == 0)
                goto retry;
            clrYesNoMsg();
            TEA_mcMsgDisp(slot, 0x478);
            result = TEA_mcFormat(slot);
            if (MemCardError == 1) {
                clrYesNoMsg();
                TEA_mcMsgNoChkSlot(slot, 0x474);
                clrYesNoMsg();
                return 0;
            }
        } else {
            clrYesNoMsg();
            return 0;
        }
    }
    clrYesNoMsg();
    if (TEA_mcChkSlot(slot) == 0)
        goto retry;
    if (TEA_mcChkEncoding(slot) == 0) {
        clrYesNoMsg();
        if (TEA_mcMsgYesNo(slot, 0x476) != 0) {
            clrYesNoMsg();
            if (TEA_mcChkSlot(slot) == 0)
                goto retry;
            clrYesNoMsg();
            TEA_mcMsgDisp(slot, 0x478);
            TEA_mcFormat(slot);
            clrYesNoMsg();
            if (MemCardError == 1) {
                TEA_mcMsgNoChkSlot(slot, 0x474);
                clrYesNoMsg();
                return 0;
            }
        } else {
            clrYesNoMsg();
            return 0;
        }
    }
    clrYesNoMsg();
    if (TEA_mcChkSlot(slot) == 0)
        goto retry;
    clrYesNoMsg();
    return 1;
}

void menu_sDispBattleEntry(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    s32 e = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(2);
        menu_sDispBattleFarmBack();
        menu_sDispBattleFarmZoid();
        menu_sDispBattleFarmWeapon();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        menu_sDispBattleFarmTitle();
        if (menumode != -1) {
            menu_sDispBattleFarmTitleSubButton();
            menu_sDispBattleEntryPersonPartner();
            menu_sDispBattleEntryPersonPartnerZoidPic();
            menu_sDispBattleEntryPersonZoidStatus();
            menu_sDispBattleEntryPilotZoidList();
            menu_sDispBattleEntryPersonPartnerZoidSelectFrame();
        }
        menu_sDispDetailAll();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}

void menu_sDispBattleEntryPersonPartnerZoidPic(void)
{
    s32 x0 = 0;
    s32 x1 = 0;
    s32 x2 = 0;
    s32 x3 = 0;
    s16 x4 = 0;
    s32 x5 = 5;
    s32 x6 = 2;
    s16 p = 0;
    s16 b = 0;

    b = 0;
    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[6], 0, b, 9, -1, -1));
    jobjDisp(GetChild(((MenuArchive *)menu1p)->joints[6], 0, b, -1, -1, -1));
    p = IveSave[0].field_1600;
    if (p != -1) {
        ttljobjcopyposdispall(GetChild(((MenuArchive *)menu1p)->joints[6], 0, b, -1, -1, -1), GetChild(menu_personp[p] != 0 ? ((MenuArchive *)menu_personp[p])->joints[0] : 0, 0, 0, -1, -1, -1));
    }
    ttljobjcopyposdispall(GetChild(((MenuArchive *)menu1p)->joints[6], 0, 0, 2, -1, -1), GetChild(((MenuArchive *)menu_zoidp[IveSave[0].field_15F4])->joints[0], 0, 0, -1, -1, -1));
    if ((u8)menuZoidBattleModeSingleTeam == 1) {
        b = 1;
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[6], 0, b, 9, -1, -1));
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[6], 0, b, -1, -1, -1));
        p = IveSave[0].field_1602;
        if (p != -1) {
            ttljobjcopyposdispall(GetChild(((MenuArchive *)menu1p)->joints[6], 0, b, -1, -1, -1), GetChild(menu_personp[p] != 0 ? ((MenuArchive *)menu_personp[p])->joints[0] : 0, 0, 0, -1, -1, -1));
        }
        ttljobjcopyposdispall(GetChild(((MenuArchive *)menu1p)->joints[6], 0, 1, 2, -1, -1), GetChild(((MenuArchive *)menu_zoidp[IveSave[0].field_15FA])->joints[0], 0, 0, -1, -1, -1));
    }
}

void menu_sDispBattleEntryPersonPartner(void)
{
    s32 dead1 = 0;
    s32 dead2 = 0;
    s16 i = 0;
    s32 anm = 0;
    s32 flag = 0;
    s32 joint = 5;
    s32 idx = 2;
    u8 tbl[12] = {0, 1, 4, 1, 2, 3, 3, 0, 0, 0, 0, 0};

    for (i = 0; i < 7; i++) {
        idx = i;
        if (menufarmanm[idx] == 7) {
            menumodeanm[idx] = 0;
            menufarmanmold[idx] = menufarmanm[idx];
        }
        if (menufarmanm[idx] != menufarmanmold[idx]) {
            menumodeanm[idx] = 1;
            menufarmanmold[idx] = menufarmanm[idx];
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 0) {
            menufarmanm[idx] = 1;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 1) {
            menumodeanm[idx] = 1;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 3) {
            menumodeanm[idx] = 3;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 4) {
            menumodeanm[idx] = 4;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 5) {
            menumodeanm[idx] = 5;
        }
        if (menumodeanm[idx] == 1) {
            menumodeanm[idx] = 2;
            sSetAnim1(menu1p, joint, GetChild(((MenuArchive *)menu1p)->joints[joint], 0, idx, -1, -1, -1), tbl[menufarmanm[idx]]);
        }
    }
    anm = 0;
    flag = 0;
    for (i = 0; i < 7; i++) {
        idx = i;
        if (menumodeanm[idx] == 2) {
            if (flag == 0) {
                anm = jobjAnim(((MenuArchive *)menu1p)->joints[joint]);
                flag = 1;
            }
            if (anm != 0) {
                menumodeanm[idx] = 3;
                menuanmend = 1;
            }
        }
    }
    if (menumodeanm[idx] == 3) {
        i = i;
    } else {
    }
    for (i = 0; i < 3; i++) {
        idx = i;
        if (menufarmanm[idx] != 7) {
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[joint], 0, idx, -1, -1, -1));
        }
    }
}
#pragma force_active on
static const char lit_3716[25] = "person=%d pos=%d BOOL%d\n";
#pragma force_active off

void menu_sDispBattleEntryPersonZoidStatus(void)
{
    s32 u1 = 0;
    s32 u2 = 0;
    s32 u3 = 2;
    s32 u4 = 0;
    char text[20];
    s32 kind = 6;
    s32 side = 0;

    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 0, 0, -1, -1));
    if (menuZoidBattleStage == -1 || (u8)menuZoidBattleModeSingleTeam == 1) {
        side = 1;
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 0, -1, -1));
    }
    side = 0;
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 0, 0, -1), 1, Tea_persondat[IveSave[0].field_1600].name);
    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 6, -1, -1));
    sprintf(text, "%04d", *(s16 *)((u8 *)&Tea_persondat[IveSave[0].field_1600] + 0x22));
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 6, 0, -1), 2, text);
    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 7, -1, -1));
    sprintf(text, "%04d", *(s16 *)((u8 *)&Tea_persondat[IveSave[0].field_1600] + 0x20));
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 7, 0, -1), 2, text);
    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 8, -1, -1));
    sprintf(text, "%04d", *(s16 *)((u8 *)&Tea_persondat[IveSave[0].field_1600] + 0x24));
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 8, 0, -1), 2, text);
    if ((u8)menuZoidBattleModeSingleTeam == 1) {
        side = 1;
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 0, 0, -1), 1, Tea_persondat[IveSave[0].field_1602].name);
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 6, -1, -1));
        sprintf(text, "%04d", *(s16 *)((u8 *)&Tea_persondat[IveSave[0].field_1602] + 0x22));
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 6, 0, -1), 2, text);
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 7, -1, -1));
        sprintf(text, "%04d", *(s16 *)((u8 *)&Tea_persondat[IveSave[0].field_1602] + 0x20));
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 7, 0, -1), 2, text);
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 8, -1, -1));
        sprintf(text, "%04d", *(s16 *)((u8 *)&Tea_persondat[IveSave[0].field_1602] + 0x24));
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 8, 0, -1), 2, text);
    }
    side = 0;
    if (IveSave[0].field_15F4 != -1) {
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 0, 1, -1), 1, Tea_zoiddat[IveSave[0].field_15F4].name);
    }
    if ((u8)menuZoidBattleModeSingleTeam == 1) {
        side = 1;
        if (IveSave[0].field_15FA != -1) {
            putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 0, 1, -1), 1, Tea_zoiddat[IveSave[0].field_15FA].name);
        }
    }
    if (IveSave[0].field_15F4 >= 0) {
        zdCalcFarm(Zoid, 0, IveSave[0].zoid_slot, -1);
        menu_sDispBattleEntryZoidStatusKind(0, 0, *(s16 *)((u8 *)&Tea_zoiddat[IveSave[0].field_15F4] + 0x52));
        menu_sDispBattleEntryZoidStatusKind(0, 1, ZF(&((ZoidStateV2 *)Zoid)[0], s32, 0x1DD8));
        menu_sDispBattleEntryZoidStatusKind(0, 2, ZF(&((ZoidStateV2 *)Zoid)[0], s32, 0x1DDC));
    }
    if ((u8)menuZoidBattleModeSingleTeam == 1 && IveSave[0].field_15FA != -1) {
        zdCalcFarm((ZoidState *)&((ZoidStateV2 *)Zoid)[1], 0, IveSave[0].partner_slot, -1);
        menu_sDispBattleEntryZoidStatusKind(1, 0, *(s16 *)((u8 *)&Tea_zoiddat[IveSave[0].field_15FA] + 0x52));
        menu_sDispBattleEntryZoidStatusKind(1, 1, ZF(&((ZoidStateV2 *)Zoid)[1], s32, 0x1DD8));
        menu_sDispBattleEntryZoidStatusKind(1, 2, ZF(&((ZoidStateV2 *)Zoid)[1], s32, 0x1DDC));
    }
}
#undef lit_3584
#define lit_3584 "%04d"

void menu_sDispBattleEntryZoidStatusKind(s32 group, s32 kind_index, s32 value)
{
    HSD_JObj *joint;

    f32 spacing = 0.0f;
    char text[52];
    s32 pad;
    s32 unused1 = 0;
    s32 kind = 6;

    {
        joint = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, group, kind_index + 3, -1, -1);
        jobjDispAll(joint);
    }
    sprintf(text, "%04d", value);
    putAscJobjLR(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, group, kind_index + 3, 0, -1), 2, text, 0);
}
void menu_sDispBattleEntryPilotZoidList(void)
{
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s32 idx = 6;
    s16 i = 0;
    s32 dead = 0;
    f32 fl = 0.0f;

    if (menumode >= 5) {
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 2, -1, -1, -1), 0, 7, 0);
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 2, 0, -1, -1), 0, 7, 0);
        fl = (f32)menufarmmh[menumode] + (f32)menufarmmn[menumode];
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 2, 1, -1, -1), 0, 7, 0);
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 2, 2, -1, -1), 0, 7, 0);
        if (menumode == 5 || menumode == 6) {
            menu_sDispBattleEntryPersonListDisp(IveSave[0].field_1600, 0);
            if ((u8)menuZoidBattleModeSingleTeam == 1) {
                menu_sDispBattleEntryPersonListDisp(IveSave[0].field_1602, 1);
            }
        }
        if (menumode == 7 || menumode == 8) {
            for (i = 0; i < menufarmmh[menumode]; i++) {
                menu_sDispBattleEntryZoidListDisp(IveSave[0].owned_zoids[i + menufarmst[menumode]], i);
            }
        }
        if ((s32)menucuranm[0] == 1) {
            SetAnimArc2((MenuArchive *)menu1p, 7, 0);
            menucuranm[0] = 2;
        }
        if ((s32)menucuranm[0] == 2) {
            if (jobjAnim(((MenuArchive *)menu1p)->joints[7]) != 0) {
                menucuranm[0] = 3;
            }
        } else if ((s32)menucuranm[0] == 3) {
            SetAnimArc2((MenuArchive *)menu1p, 7, 0);
            jobjAnim(((MenuArchive *)menu1p)->joints[7]);
            menucuranm[0] = 2;
        }
        src = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 4, menufarmsl[menumode], -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 1, -1, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDisp(dst, 0, 7, 0);
        dst = dst;
    }
}

void menu_sDispBattleEntryPersonListDisp(s16 idx, s16 y)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    char text[0x3C];
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s32 dead1 = 0;
    s32 dead2 = 0;

    dead1 = 0;
    dead2 = 0;
    if (menumode == 5 || menumode == 6) {
        if (idx == -1) {
            return;
        }
        src = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 4, y, -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 3, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDispAll(dst, 0, 7, 0);
        src = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 4, y, -1);
        dst = GetChild(menu_personp[idx] ? ((MenuArchive *)menu_personp[idx])->joints[0] : 0, 0, 1, -1, -1, -1);
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
        sprintf(text, " %s", Tea_persondat[idx].name);
        putAscJobj(src, 0, text);
    }
}
#undef lit_3644
#define lit_3644 " %s"

void menu_sDispBattleEntryZoidListDisp(s16 idx, s16 y)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    char text[0x3C];
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s32 dead1 = 0;
    s32 dead2 = 0;

    dead1 = 0;
    dead2 = 0;
    if (menumode == 7 || menumode == 8) {
        if (idx == -1) {
            return;
        }
        src = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 4, y, -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 3, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDispAll(dst, 0, 7, 0);
        src = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 4, y, -1);
        if (menu_zoidp[idx] == NULL) {
            return;
        }
        dst = GetChild(((MenuArchive *)menu_zoidp[idx])->joints[0], 0, 1, -1, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDispAll(dst, 0, 7, 0);
        sprintf(text, " %s", Tea_zoiddat[idx].name);
        putAscJobj(src, 0, text);
    }
}

void menu_sDispBattleEntryPersonPartnerZoidSelectFrame(void)
{
    s32 s18 = 0;
    s32 s14 = 0;
    u8 tbl[6] = {0, 1, 4, 1, 2, 3};
    s16 s8 = 0;
    HSD_JObj *j2 = 0;
    s16 flag30 = 0;
    s16 flag29 = 1;
    HSD_JObj *j1 = 0;

    if (menumode != 2 && menumode < 5) {
        if (menumode == 3) {
            flag30 = 0;
        }
        if (menumode == 4) {
            flag30 = 1;
        }
        if (menufarmcl[menumode] == 0) {
            flag29 = 1;
        }
        if (menufarmcl[menumode] == 1) {
            flag29 = 2;
        }
        if ((s32)menucuranm[0] == 1) {
            SetAnimArc2((MenuArchive *)menu1p, 7, 0);
            menucuranm[0] = 2;
        }
        if ((s32)menucuranm[0] == 2) {
            if (jobjAnim(((MenuArchive *)menu1p)->joints[7]) != 0) {
                menucuranm[0] = 3;
            }
        } else if ((s32)menucuranm[0] == 3) {
            SetAnimArc2((MenuArchive *)menu1p, 7, 0);
            jobjAnim(((MenuArchive *)menu1p)->joints[7]);
            menucuranm[0] = 2;
        }
        j1 = GetChild(((MenuArchive *)menu1p)->joints[6], 0, flag30, flag29, -1, -1);
        j2 = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 0, -1, -1, -1);
        jobjCopyPos(j1, j2);
        HSD_JObjDisp(j2, NULL, 7, 0);
    }
}

void menu_BattleSet_EntryPersonPartnerExit(void)
{
    s16 i = 0;

    if (menumode == 2) {
        i = 0;
        for (i = 0; i < 3; i++) {
            if (i == menufarmcl[2]) {
                menufarmanm[i] = 4;
            } else {
                menufarmanm[i] = 1;
            }
        }
        if (menuZoidBattleStage != -1 && (u8)menuZoidBattleModeSingleTeam == 0) {
            menufarmanm[1] = 7;
        }
    } else if (menumode == 3) {
        menufarmanm[0] = 5;
        menufarmanm[1] = 3;
        if (menuZoidBattleStage != -1 && (u8)menuZoidBattleModeSingleTeam == 0) {
            menufarmanm[1] = 7;
        }
    } else if (menumode == 4) {
        menufarmanm[1] = 5;
        menufarmanm[0] = 3;
    } else if (menumode == 0) {
        menufarmanm[1] = 7;
        menufarmanm[0] = 7;
        menufarmanm[2] = 7;
    } else {
        menufarmanm[2] = 7;
    }
}

void ttlbatentzoidchange_stagedownup(s16 unmount_slot, s16 zoid_slot)
{
    SeLiftStart();
    menu_StageUpDownFlag = 0;
    menuDetailMode = 0;
    while (menu_sDispBattleStageDown(1) == 0) {
        menu_sDispBattleEntry();
    }
    SeLiftStop();
    ttl_WeaponAllUnMount(unmount_slot);
    ZoidWeaponPurge(&Zoid[0]);
    ZoidPurge(&Zoid[0]);
    IveSave[0].zoid_slot = zoid_slot;
    IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
    ZoidInit(&Zoid[0], IveSave[0].field_15F4);
    ZoidSetAnim((ZoidState *)&Zoid, 0x67);
    ZoidColor(&Zoid[0], IveSave[0].zoid_color[IveSave[0].zoid_slot], 0);
    ttl_WeaponAllMount(IveSave[0].zoid_slot);
    SeLiftStart();
    menu_StageUpDownFlag = 0;
    while (menu_sDispBattleStageDown(0) == 0) {
        menu_sDispBattleEntry();
    }
    SeLiftStop();
}
#pragma force_active on
static const char lit_3729[5] = "%03d";
namespace { __declspec(section ".sdata2") extern const char lit_3730[7] = "jobj.h"; }
namespace { __declspec(section ".sdata2") extern const char lit_3731[5] = "jobj"; }
#pragma force_active off

s32 menufncZoidBattle_BattleMenuEntry(void)
{
    s32 u28;
    s32 u24;
    s32 u20;
    s32 u1c;
    s32 u18;
    s32 u14;
    s8 u10;
    s16 zd1;
    s16 zd2;
    s32 u08;
    s32 i = 0;
    s32 d1 = 0;
    s32 d2 = 0;
    s16 t = 0;
    s32 d3 = 0;
    s16 x = 0;
    s16 y = 0;

    u28 = 0;
    u24 = 0;
    u20 = -0xf0;
    u1c = 0;
    u18 = 0;
    u14 = 0;
    u10 = 0;

    menuCursor = 0;
    menuParticles = 0;
    menumode = 3;
    for (i = 0; i < 20; i++) {
        menumodeanm[i] = 0;
    }
    for (i = 0; i < 20; i++) {
        menufarmanm[i] = 0;
    }
    vsyncN = 0;
    menucuranm[0] = 1;
    menuSetTitle(1);
    TEA_setvsync();
    menufarmmode = 1;
    menufarmcl[2] = 0;
    menufarmsl[2] = 0;
    menufarmst[2] = 0;
    if (menuZoidBattleModeSingleTeam == 1) {
        menufarmmh[2] = 2;
        menufarmmn[2] = 2;
    } else if (menuZoidBattleStage != -1) {
        menufarmmh[2] = 1;
        menufarmmn[2] = 1;
    } else {
        menufarmmh[2] = 2;
        menufarmmn[2] = 2;
    }
    menufarmcl[3] = 0;
    menufarmsl[3] = 0;
    menufarmst[3] = 0;
    menufarmmh[3] = 2;
    menufarmmn[3] = 2;
    menufarmcl[4] = 0;
    menufarmsl[4] = 0;
    menufarmst[4] = 0;
    menufarmmh[4] = 2;
    menufarmmn[4] = 2;
    for (i = 5; i <= 8; i++) {
        menufarmcl[i] = 0;
        menufarmsl[i] = 0;
        menufarmst[i] = 0;
        menufarmmh[i] = 8;
        menufarmmn[i] = 2;
    }
    menufarmsl[5] = 0;
    menufarmmn[5] = IveSave[0].person_count;
    if (menufarmmn[5] < menufarmmh[5])
        menufarmmh[5] = menufarmmn[5];
    menufarmst[5] = -1;
    menufarmsl[6] = 0;
    menufarmmn[6] = IveSave[0].person_count;
    if (menufarmmn[6] < menufarmmh[6])
        menufarmmh[6] = menufarmmn[6];
    menufarmst[6] = -1;
    menufarmsl[7] = IveSave[0].zoid_slot;
    menufarmmn[7] = IveSave[0].owned_zoid_count;
    if (menufarmmn[7] < menufarmmh[7])
        menufarmmh[7] = menufarmmn[7];
    menufarmst[7] = -1;
    menufarmsl[8] = IveSave[0].partner_slot;
    menufarmmn[8] = IveSave[0].owned_zoid_count;
    if (menufarmmn[8] < menufarmmh[8])
        menufarmmh[8] = menufarmmn[8];
    menufarmst[8] = -1;
    OSReport("ZoidNum=%d\n", IveSave[0].owned_zoid_count);
    menufarmmh[5] = 8;
    menufarmmh[6] = 8;
    menufarmmn[5] = IveSave[0].person_count;
    menufarmmn[6] = IveSave[0].person_count;
    IveSave[4].field_15F4 = IveSave[0].field_15F4;
    IveSave[4].field_15FA = IveSave[0].field_15FA;
    x = IveSave[0].field_1600;
    y = IveSave[0].field_1602;
    OSReport("Entry:person=%d partner=%d zoid=%d z2=%d \n", IveSave[0].field_1600, IveSave[0].field_1602, IveSave[0].field_15F4, IveSave[0].field_15FA);
    for (i = 0; i < IveSave[0].person_count; i++) {
        if (IveSave[0].field_1600 == IveSave[0].persons[i]) {
            menufarmcl[5] = (s16)i;
            menufarmsl[5] = (s16)i;
            menufarmst[5] = -1;
            IveSave[0].field_1604 = (s16)i;
            break;
        }
    }
    for (i = 0; i < IveSave[0].person_count; i++) {
        if (IveSave[0].field_1602 == IveSave[0].persons[i]) {
            menufarmcl[6] = (s16)i;
            menufarmsl[6] = (s16)i;
            menufarmst[6] = -1;
            IveSave[0].field_1606 = (s16)i;
            break;
        }
    }
    i = 5;
    menufarmcl[i] = 0;
    menufarmsl[i] = 0;
    menufarmst[i] = 0;
    menufarmmh[i] = menuZoidBattleModeSingleTeam + 1;
    menufarmmn[i] = menuZoidBattleModeSingleTeam + 1;
    i = 6;
    menufarmcl[i] = 0;
    menufarmsl[i] = 0;
    menufarmst[i] = 0;
    menufarmmh[i] = menuZoidBattleModeSingleTeam + 1;
    menufarmmn[i] = menuZoidBattleModeSingleTeam + 1;
    menu_BattleSet_EntryPersonPartnerExit();
    ttl_WeaponAllMount(IveSave[0].zoid_slot);
    OSReport("Zoid bat ent main start\n", IveSave[0].owned_zoid_count);
    for (;;) {
        sPadAll();
        ttl_batent_detailsetparam();
        menu_sDispSetButton();
        if (menumode <= 2) {
            menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        }
        if (menumode >= 5) {
            menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        }
        menu_BattleSet_EntryPersonPartnerExit();
        menu_sDispBattleEntry();
        if (sPad1->field_5E) {
            SeCancel();
            if (menumode == 2) {
                d1 = menumode + menufarmcl[-menumode];
                continue;
            }
            if (menumode <= 4) {
                menumode = 2;
                menufarmcl[2] = menufarmsl[2] = 2;
            } else if (menumode == 5) {
                if (IveSave[0].field_1600 == IveSave[0].field_1602) {
                    if (IveSave[0].field_1600 == x)
                        IveSave[0].field_1600 = x;
                    else
                        IveSave[0].field_1600 = y;
                }
                menumode = 3;
            } else if (menumode == 6) {
                if (IveSave[0].field_1602 == IveSave[0].field_1600) {
                    if (IveSave[0].field_1602 == y)
                        IveSave[0].field_1602 = x;
                    else
                        IveSave[0].field_1602 = y;
                }
                menumode = 4;
            } else if (menumode == 7) {
                IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                menumode = 3;
            } else if (menumode == 8) {
                IveSave[0].field_15FA = IveSave[0].owned_zoids[IveSave[0].partner_slot];
                menumode = 4;
            }
        }
        if (sPad1->pressed_a) {
            switch (menumode) {
            case 2:
                if (menufarmcl[menumode] == 2) {
                    SeRet();
                    menumode = -1;
                    menufncZoidBattle_BattleMenuReturn();
                    if (menuZoidBattleStage == -1) {
                        menuZoidBattleStage = 0;
                        GameMode.win = 1;
                    }
                    IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                    OSReport("zd1=%d zd2=%d\n", IveSave[0].field_15F4, IveSave[0].field_15FA);
                    return 0xcc;
                } else if (menufarmcl[2] == 1) {
                    SeRet();
                    menumode = 4;
                } else if (menufarmcl[2] == 0) {
                    SeRet();
                    menumode = 3;
                }
                break;
            case 3:
                SeRet();
                if (menufarmcl[3] == 0)
                    menumode = 5;
                if (menufarmcl[3] == 1) {
                    menumode = 7;
                    IveSave[0].field_15F8 = IveSave[0].zoid_slot;
                    IveSave[0].field_15FE = IveSave[0].partner_slot;
                    menufarmcl[menumode] = 0;
                    menufarmsl[menumode] = 0;
                    menufarmst[menumode] = 0;
                    OSReport("dummy\n");
                }
                break;
            case 4:
                SeRet();
                if (menufarmcl[4] == 0)
                    menumode = 6;
                if (menufarmcl[4] == 1) {
                    IveSave[0].field_15F8 = IveSave[0].zoid_slot;
                    IveSave[0].field_15FE = IveSave[0].partner_slot;
                    menumode = 8;
                    menufarmcl[menumode] = 0;
                    menufarmsl[menumode] = 0;
                    menufarmst[menumode] = 0;
                }
                break;
            case 5:
                if (menufarmcl[menumode] == 1) {
                    SeRet();
                    t = IveSave[0].field_1600;
                    IveSave[0].field_1600 = IveSave[0].field_1602;
                    IveSave[0].field_1602 = t;
                    TEA_VoiceOn((u8)IveSave[0].field_1600, 1);
                } else {
                    SeCancel();
                }
                menumode = 3;
                break;
            case 6:
                if (menufarmcl[menumode] == 0) {
                    SeRet();
                    t = IveSave[0].field_1600;
                    IveSave[0].field_1600 = IveSave[0].field_1602;
                    IveSave[0].field_1602 = t;
                    TEA_VoiceOn((u8)IveSave[0].field_1602, 1);
                } else {
                    SeCancel();
                }
                menumode = 4;
                break;
            case 7:
                menumode = 3;
                if (menuZoidBattleModeSingleTeam == 1) {
                    if (IveSave[0].partner_slot == menufarmcl[7]) {
                        IveSave[0].partner_slot = IveSave[0].zoid_slot;
                        IveSave[0].field_15FA = IveSave[0].owned_zoids[IveSave[0].partner_slot];
                    }
                }
                SeRet();
                menumode = 3;
                if (IveSave[0].field_15F8 != menufarmcl[7]) {
                    ttlbatentzoidchange_stagedownup(IveSave[0].field_15F8, menufarmcl[7]);
                }
                dlgzoidno = IveSave[0].field_15F4;
                menumode = 3;
                break;
            case 8:
                menumode = 4;
                if (IveSave[0].zoid_slot == menufarmcl[8]) {
                    IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].partner_slot];
                    IveSave[0].field_15FA = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                    ttlbatentzoidchange_stagedownup(IveSave[0].zoid_slot, IveSave[0].partner_slot);
                    IveSave[0].zoid_slot = IveSave[0].partner_slot;
                    IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                }
                SeRet();
                IveSave[0].partner_slot = menufarmcl[8];
                IveSave[0].field_15FA = IveSave[0].owned_zoids[IveSave[0].partner_slot];
                menumode = 4;
                break;
            }
        }
        if (sPad1->field_4F || sPad1->field_53) {
            if (menuZoidBattleModeSingleTeam == 1) {
                if (menumode == 4) {
                    menumode = 3;
                    menufarmcl[2] = menufarmsl[2] = 0;
                }
            } else if (menumode == 2) {
                if (menufarmcl[2] == 1) {
                    menufarmcl[2] = menufarmsl[2] = 0;
                }
            }
            if (menumode == 3 || menumode == 4)
                SeSelectB();
        } else if (sPad1->field_50 || sPad1->field_54) {
            if (menumode == 2 && menufarmcl[2] >= 2) {
                menufarmcl[2] = menufarmsl[2] = 2;
            }
            if (menuZoidBattleModeSingleTeam == 1 && menumode == 3) {
                menumode = 4;
                menufarmcl[2] = menufarmsl[2] = 1;
            }
            if (menumode == 3 || menumode == 4) {
                d2 = IveSave[0].owned_zoids[IveSave[0].owned_zoids[2]];
                (void)(IveSave[0].owned_zoids[IveSave[0].partner_slot] + 1);
                d3 = IveSave[0].owned_zoids[2] + (IveSave[0].owned_zoids[IveSave[0].partner_slot] ^ 3);
                SeSelectB();
                (void)(menumode + 1);
            }
        } else if (sPad1->field_51 || sPad1->field_55) {
            if (menumode == 2 && menufarmcl[2] == 2) {
                menumode = 3;
            }
            if (menumode == 3 || menumode == 4) {
                SeSelectB();
                if (menufarmcl[menumode] != 0) {
                    menufarmcl[menumode] = menufarmsl[menumode] = 0;
                }
            }
        } else if (sPad1->field_52 || sPad1->field_56) {
            if (menumode == 2 && menufarmcl[2] != 2) {
                menufarmcl[2] = menufarmsl[2] = 2;
            }
            if (menumode == 3 || menumode == 4) {
                SeSelectB();
                if (menufarmcl[menumode] == 0) {
                    menufarmcl[menumode] = menufarmsl[menumode] = 1;
                } else if (menufarmcl[menumode] == 1) {
                    menumode = 2;
                    menufarmcl[2] = menufarmsl[2] = 2;
                }
            }
        } else {
            menumode;
        }
    }
}
#undef lit_3940
#define lit_3940 "ZoidNum=%d\n"
#undef lit_3941
#define lit_3941 "Entry:person=%d partner=%d zoid=%d z2=%d \n"
#undef lit_3942
#define lit_3942 "Zoid bat ent main start\n"
#undef lit_3943
#define lit_3943 "zd1=%d zd2=%d\n"
#undef lit_3944
#define lit_3944 "dummy\n"
#pragma force_active on
static f32 eyeCam[3] = {
    0.0f, 0.0f, 100.0f, 
};
#pragma force_active off

void menufncZoidBattle_BattleMenuReturn(void)
{
    s16 i = 0;
    GXColor color;

    menu_TITLE_dispEntry(0);
    for (i = 0; i < 1; i++) {
        color = (GXColor){ 0, 0, 0, 0 };
        HSD_VIData.bg_color = color;
        VIWaitForRetrace();
        HSD_VICopyXFB(0);
        HSD_VISetXFBDrawDone();
    }
    GameZoid[0].type = IveSave[0].field_15F4;
    GameZoid[1].type = IveSave[0].field_15FA;
}

void menu_TITLE_dispEntry(s32 state)
{
    s16 i = 0;

    menuanmend = 0;
    menutitleanm[0].mode = 1;
    for (i = 1; i < 7; i++)
        menutitleanm[i].mode = 1;
    if (state == 1) {
        menutitleanm[0].state = 0;
        for (i = 1; i < 7; i++)
            menutitleanm[i].state = 0;
    } else {
        menutitleanm[0].state = 2;
        for (i = 1; i < 7; i++)
            menutitleanm[i].state = 2;
    }
    while (menuanmend == 0) {
        menu_sDispSetButton();
        menu_sDispBattleEntry();
    }
    menuanmend = 0;
}

void ttl_batent_detailsetparam(void)
{
    if (menumode == 2) {
        if (menufarmcl[2] == 0)
            menumode = 3;
        if (menufarmcl[2] == 1)
            menumode = 4;
        menuDetailMode = 0;
        detCustm(-1);
        if (menufarmcl[2] == 2)
            detCustm(IveSave[0].zoid_slot);
    }
    if (menumode == 3) {
        if (menufarmcl[menumode] == 0)
            detPerson(IveSave[0].field_1600);
        if (menufarmcl[menumode] == 1)
            detCustm(IveSave[0].zoid_slot);
    }
    if (menumode == 4) {
        if (menufarmcl[menumode] == 0)
            detPerson(IveSave[0].field_1602);
        if (menufarmcl[menumode] == 1)
            detCustm(IveSave[0].partner_slot);
    }
    if (menumode == 5 || menumode == 6) {
        if (menufarmcl[menumode] == 0)
            detPerson(IveSave[0].field_1600);
        else
            detPerson(IveSave[0].field_1602);
    }
    if (menumode == 7)
        detCustm(menufarmcl[7]);
    if (menumode == 8)
        detCustm(menufarmcl[8]);
    printf("menumode=%d [5]=%d hmn=%d \n", menumode, menufarmcl[5], IveSave[0].persons[menufarmcl[5]]);
}
#undef lit_4012
#define lit_4012 "menumode=%d [5]=%d hmn=%d \n"

void menu_sDispBattleSet(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    s32 e = 0;

    if (menu_back1p == 0) {
        putErr("out of memory. menu back error");
    }
    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if (menu_back1p != 0 && HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(2);
        menu_sDispBattleFarmBack();
        menu_sDispBattleFarmZoid();
        menu_sDispBattleFarmWeapon();
        if (menumode == 4 || menumode == 7) {
            menufnczoidmountpointdisp();
        }
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if (menu1p != 0 && HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        selectcursolanm(7);
        menu_sDispBattleFarmTitle();
        menu_sDispBattleFarmTitleSubButton();
        menu_sDispBattleSetunfold();
        menu_sDispBattleSetLButton();
        menu_sDispBattleSetZoidStatus();
        menu_sDispBattleSetZoidHaveZoidList();
        menu_sDispBattleSetequipPartsList();
        menu_sDispBattleSetequipParts();
        menu_sDispBattleSetHavePartsList();
        menu_sDispMsgWin(3, 0);
        if (menumode != 4) {
            flPictureDispParts(detPartsn);
        }
        menu_sDispDetailAll();
        menu_sDispYesNoMenu();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
}
#undef lit_4027
#define lit_4027 "out of memory. menu back error"

void selectcursolanm(s16 model)
{
    if ((s32)menucuranm[0] == 1) {
        SetAnimArc2((MenuArchive *)menu1p, model, 0);
        menucuranm[0] = 2;
    }
    if ((s32)menucuranm[0] == 2) {
        if (jobjAnim(((MenuArchive *)menu1p)->joints[model]) != 0)
            menucuranm[0] = 3;
    } else if ((s32)menucuranm[0] == 3) {
        SetAnimArc2((MenuArchive *)menu1p, model, 0);
        jobjAnim(((MenuArchive *)menu1p)->joints[model]);
        menucuranm[0] = 2;
    }
}

void menu_sDispBattleSetHavePartsList(void)
{
    s32 u0 = 7;
    s32 u1 = 0;
    s32 idx = 10;
    s32 u2 = 0;
    s32 dead = 0;
    s32 frame = 1;
    s32 flag = 0;
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s16 i = 0;
    s16 part = 0;
    register s16 sprite = 0;

    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)sprite;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    (void)frame;
    if (menumode == 7) {
        flag = 1;
        frame = 1;
    }
    if (menumode == 5) {
        flag = 1;
        frame = 3;
    }
    if (menumode == 8) {
        flag = 1;
        frame = 4;
    }
    if (flag != 0 && menuzoidhavepartslist != 0) {
        if (menumode == 5)
            src = GetChild(((MenuArchive *)menu1p)->joints[14], 0, 8, -1, -1, -1);
        if (menumode == 7)
            src = GetChild(((MenuArchive *)menu1p)->joints[14], 0, 8, -1, -1, -1);
        if (menumode == 8)
            src = GetChild(((MenuArchive *)menu1p)->joints[14], 0, 8, -1, -1, -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, -1, -1, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDispAll(dst, 0, 7, 0);
        idx = 11;
        dead = 1;
        for (i = 0; i < menusetmh[menumode]; i++) {
            src = GetChild(((MenuArchive *)menu1p)->joints[10], 0, 3, i, -1, -1);
            dst = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, -1, -1, -1, -1);
            jobjCopyPos(src, dst);
            HSD_JObjDisp(dst, 0, 7, 0);
            if (1) {
                part = IveSave[0].list_parts[i + menusetst[menumode]];
                if (part <= 0) {
                    if (menumode == 7) { (void)0; }
                    if (menumode == 4) { (void)0; }
                    if (menumode == 8) { (void)0; }
                    sprite = 16;
                    frame = 1;
                    part = -1;
                } else {
                    if (Tea_partsdat[part].category == 1)
                        frame = 1;
                    if (Tea_partsdat[part].category == 2)
                        frame = 3;
                    if (Tea_partsdat[part].category == 3)
                        frame = 4;
                    sprite = Tea_partsdat[part].rank;
                    if (frame == 1 && sprite > 16)
                        sprite = 16;
                    if (frame == 3 && sprite > 4)
                        sprite = 4;
                    if (frame == 4 && sprite > 8)
                        sprite = 8;
                }
                HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, frame, sprite, -1, -1), 0, 7, 0);
                dst = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 2, -1, -1, -1);
                if (part == -1)
                    putAscJobj(dst, 2, lit_2268);
                else
                    putAscJobj(dst, 2, Tea_partsdat[part].short_name);
                if (i == menusetsl[menumode]) {
                    dst = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 2, -1, -1, -1);
                    jobjCopyPos(src, dst);
                    HSD_JObjDispAll(dst, 0, 7, 0);
                    sprintf(menudispwinstr, "%s                                                                      ", Tea_partsdat[part].name);
                    menudispwinstr[56] = 0;
                }
            }
        }
    }
}
#undef lit_4089
#define lit_4089 "%s                                                                      "

void menu_sDispBattleSetequipPartsList(void)
{
    s16 u = 0;
    s32 idx = 14;
    s32 btn = 8;
    s32 kind = 0;
    s32 anim = 0;
    s32 flag = 0;
    HSD_JObj *jobj = 0;
    HSD_JObj *j2 = 0;

    if (menumode == 5)
        flag = 1;
    if (menumode == 4)
        flag = 1;
    if (menumode == 7)
        flag = 1;
    if (menumode == 6)
        flag = 1;
    if (menumode == 8)
        flag = 1;
    if (flag != 0 && menuzoidequippartslist != 0) {
        if (menumode < 7)
            kind = menumode;
        else
            kind = menumode - 3;
        if (menumode == 8)
            kind = 6;
        jobj = GetChild(((MenuArchive *)menu1p)->joints[13], 0, kind - 4, -1, -1, -1);
        if ((u8)menusetbtanm[btn].cur != menusetbtanm[btn].old) {
            menusetbtanm[btn].state = 1;
            menusetbtanm[btn].old = menusetbtanm[btn].cur;
        }
        if (menusetbtanm[btn].state == 0)
            return;
        if (menusetbtanm[btn].state == 1) {
            menusetbtanm[btn].state = 2;
            SetAnimArc2((MenuArchive *)menu1p, idx, menusetbtanm[btn].cur);
        }
        if (menusetbtanm[btn].state == 2) {
            anim = jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
            sMakeMatrix(((MenuArchive *)menu1p)->joints[idx]);
            if (anim != 0) {
                menusetbtanm[btn].state = 3;
                menuanmend = 1;
                if (menusetbtanm[btn].cur == 0) {
                    j2 = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, -1, -1, -1, -1);
                    jobjCopyPos(jobj, j2);
                    HSD_JObjDispAll(j2, 0, 7, 0);
                    return;
                } else {
                    if (menusetbtanm[btn].cur == 2)
                        return;
                    if (menusetbtanm[btn].cur == 4)
                        menusetbtanm[btn].cur = 27;
                    if (menusetbtanm[btn].cur == 5)
                        return;
                    if (menusetbtanm[btn].cur == 7)
                        menusetbtanm[btn].cur = 27;
                    if (menusetbtanm[btn].cur == 8)
                        return;
                    if (menusetbtanm[btn].cur == 10)
                        menusetbtanm[btn].cur = 27;
                    if (menusetbtanm[btn].cur == 11)
                        return;
                    if (menusetbtanm[btn].cur == 13)
                        menusetbtanm[btn].cur = 27;
                    if (menusetbtanm[btn].cur == 14)
                        return;
                    if (menusetbtanm[btn].cur == 16)
                        menusetbtanm[btn].cur = 27;
                    if (menusetbtanm[btn].cur == 17)
                        return;
                    if (menusetbtanm[btn].cur == 19)
                        menusetbtanm[btn].cur = 27;
                    if (menusetbtanm[btn].cur == 20)
                        return;
                    if (menusetbtanm[btn].cur == 22)
                        menusetbtanm[btn].cur = 27;
                    if (menusetbtanm[btn].cur == 23)
                        return;
                    if (menusetbtanm[btn].cur == 25)
                        menusetbtanm[btn].cur = 27;
                    if (menusetbtanm[btn].cur == 26)
                        return;
                    menusetbtanm[btn].state = 2;
                    SetAnimArc2((MenuArchive *)menu1p, idx, menusetbtanm[btn].cur);
                    jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
                    sMakeMatrix(((MenuArchive *)menu1p)->joints[idx]);
                }
            }
        }
        j2 = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, -1, -1, -1, -1);
        jobjCopyPos(jobj, j2);
        HSD_JObjDispAll(j2, 0, 7, 0);
    }
}

void menu_sDispBattleSetequipParts(void)
{
    s32 u0 = 7;
    s32 u1 = 0;
    s32 idx = 11;
    s32 u2 = 0;
    s32 kind = 4;
    s32 count = 0;
    s32 frame = 0;
    s32 off = 0;
    s32 flag = 0;
    s32 sel = 0;
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s16 i = 0;
    s16 part = 0;
    s16 sprite = 0;

    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    (void)dst;
    if (menumode == 4) {
        flag = 1;
        kind = 4;
        frame = 1;
        count = 8;
        sel = 1;
    }
    if (menumode == 7) {
        flag = 1;
        kind = 4;
        frame = 1;
        count = 5;
    }
    if (menumode == 5) {
        flag = 1;
        kind = 5;
        frame = 3;
        count = 1;
    }
    if (menumode == 6) {
        flag = 1;
        kind = 6;
        frame = 4;
        count = 3;
        sel = 1;
    }
    if (menumode == 8) {
        flag = 1;
        kind = 6;
        frame = 4;
        count = 3;
    }
    if (flag != 0 && menuzoidequippartslist != 0) {
        for (i = 0; i < count; i++) {
            if (menumode == 4 && ttl_ZoidChkWepPoint(IveSave[0].field_15F4, i) == 0)
                continue;
            src = GetChild(((MenuArchive *)menu1p)->joints[14], 0, i, -1, -1, -1);
            dst = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, -1, -1, -1, -1);
            jobjCopyPos(src, dst);
            HSD_JObjDisp(dst, 0, 7, 0);
            if (kind == 4) {
                off = 0;
                if (Tea_zoiddat[IveSave[0].field_15F4].point_flags[i] == 0)
                    off = -1;
                HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, i + menusetst[kind], off, -1), 0, 7, 0);
            }
            if (kind == 4)
                part = IveSave[0].zoid_parts[IveSave[0].zoid_slot][i + menusetst[kind]];
            if (kind == 5)
                part = IveSave[0].zoid_field_119E[IveSave[0].zoid_slot];
            if (kind == 6)
                part = IveSave[0].zoid_field_11DA[IveSave[0].zoid_slot][i];
            if (part <= 0) {
                sprite = 15;
                frame = 1;
                part = -1;
            } else {
                if (Tea_partsdat[part].category == 1)
                    frame = 1;
                if (Tea_partsdat[part].category == 2)
                    frame = 3;
                if (Tea_partsdat[part].category == 3)
                    frame = 4;
                sprite = Tea_partsdat[part].rank;
                if (kind == 4 && sprite > 15)
                    sprite = 16;
                if (kind == 5 && sprite > 4)
                    sprite = 4;
                if (kind == 6 && sprite > 8)
                    sprite = 8;
            }
            HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, frame, sprite, -1, -1), 0, 7, 0);
            dst = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 2, -1, -1, -1);
            if (part == -1)
                putAscJobj(dst, 2, lit_2268);
            else
                putAscJobj(dst, 2, Tea_partsdat[part].short_name);
            if (i == menusetsl[kind] && sel == 1) {
                dst = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 2, -1, -1, -1);
                jobjCopyPos(src, dst);
                HSD_JObjDispAll(dst, 0, 7, 0);
            }
        }
    }
}

void menu_sDispBattleSetZoidHaveZoidList(void)
{
    HSD_JObj *a = 0;
    HSD_JObj *b = 0;
    HSD_JObj *c = 0;
    s16 i = 0;
    char buf[0x40];

    if (menumode == 3) {
        a = GetChild(((MenuArchive *)menu1p)->joints[13], 0, 3, -1, -1, -1);
        b = GetChild(((MenuArchive *)menu1p)->joints[12], 0, -1, -1, -1, -1);
        jobjCopyPos(a, b);
        HSD_JObjDispAll(b, NULL, 7, 0);
        i = 0;
        for (i = 0; i < menusetmh[3]; i++) {
            a = GetChild(((MenuArchive *)menu1p)->joints[12], 0, 3, i, -1, -1);
            c = GetChild(((MenuArchive *)menu_zoidp[IveSave[0].owned_zoids[menusetst[3] + i]])->joints[0], 0, 1, -1, -1, -1);
            jobjCopyPos(a, c);
            HSD_JObjDispAll(c, NULL, 7, 0);
            sprintf(buf, lit_3644, Tea_zoiddat[IveSave[0].owned_zoids[menusetst[3] + i]].name);
            putAscJobj(a, 0, buf);
            if (i == menusetsl[3]) {
                a = GetChild(((MenuArchive *)menu1p)->joints[12], 0, 3, i, -1, -1);
                b = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 1, -1, -1, -1);
                jobjCopyPos(a, b);
                HSD_JObjDispAll(b, NULL, 7, 0);
            }
        }
    }
}

void menu_sDispBattleSetunfold(void)
{
    s32 j = 13;
    s32 i = 7;
    s32 anim = 0;
    s32 dead = 0;

    if (menumode >= 3 && menumode <= 6) {
        if (menusetbtanm[i].cur != menusetbtanm[i].old) {
            menusetbtanm[i].state = 1;
            menusetbtanm[i].old = menusetbtanm[i].cur;
        }
        if (menusetbtanm[i].state == 1) {
            menusetbtanm[i].state = 2;
            SetAnimArc2((MenuArchive *)menu1p, j, menusetbtanm[i].cur);
        }
        if (menusetbtanm[i].state == 2) {
            anim = jobjAnim(((MenuArchive *)menu1p)->joints[j]);
            sMakeMatrix(((MenuArchive *)menu1p)->joints[j]);
            if (anim != 0) {
                menusetbtanm[i].state = 3;
                menuanmend = 1;
                if (menusetbtanm[i].cur == 0) {
                    menusetbtanm[i].cur = 1;
                }
                if (menusetbtanm[i].cur == 2) {
                    menusetbtanm[i].cur = 9;
                }
                if (menusetbtanm[i].cur == 3) {
                    menusetbtanm[i].cur = 4;
                }
                if (menusetbtanm[i].cur == 5) {
                    menusetbtanm[i].cur = 9;
                }
                if (menusetbtanm[i].cur == 6) {
                    menusetbtanm[i].cur = 7;
                }
                if (menusetbtanm[i].cur == 8) {
                    menusetbtanm[i].cur = 9;
                }
                menusetbtanm[i].state = 2;
                SetAnimArc2((MenuArchive *)menu1p, j, menusetbtanm[i].cur);
                jobjAnim(((MenuArchive *)menu1p)->joints[j]);
                sMakeMatrix(((MenuArchive *)menu1p)->joints[j]);
            }
        }
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[j], 0, -1, -1, -1, -1), 0, 7, 0);
    }
    dead = 0;
}

void menu_sDispBattleSetZoidStatus(void)
{
    s32 unused = 0;
    char text[0x3C];
    HSD_JObj *joint = (HSD_JObj *)NULL;
    s32 kind = 9;

    if (menumode != 0) {
        menu_sDispBattleSetLButtonEtc();
        joint = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 6, 0, -1, -1);
        sprintf(text, "%s", Tea_zoiddat[IveSave[0].field_15F4].name);
        putAscJobj(joint, 1, text);
        zdCalcFarm(Zoid, 0, IveSave[0].zoid_slot, -1);
        menu_sDispBattleSetZoidStatusKind(0, Tea_zoiddat[IveSave[0].field_15F4].field_66);
        menu_sDispBattleSetZoidStatusKind(1, ZF(&((ZoidStateV2 *)Zoid)[0], s32, 0x1dd8));
        menu_sDispBattleSetZoidStatusKind(2, ZF(&((ZoidStateV2 *)Zoid)[0], s32, 0x1ddc));
    }
}

void menu_sDispBattleSetZoidStatusKind(s32 index, s32 value)
{
    s32 unused = 0;
    char text[8];
    s32 dead = 0;
    s32 kind = 9;

    sprintf(text, lit_3584, value);
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 6, index + 1, 0, -1), 2, text);
    dead = 1;
}

void menu_sDispBattleSetLButton(void)
{
    if (menumode != 0) {
        menu_sDispBattleSetLButtonZoid();
        menu_sDispBattleSetLButtonWeapon();
        menu_sDispBattleSetLButtonOption();
        menu_sDispBattleSetLButtonSubParts();
        menu_sDispBattleSetLButtonReset();
        menu_sDispBattleSetLButtonExit();
        menu_sDispBattleSetZoidStatus();
    }
}

void menu_sDispBattleSetLButtonZoid(void)
{
    HSD_JObj *joint1 = 0;
    HSD_JObj *joint2 = 0;
    s32 idx = 9;

    joint1 = GetChild(((MenuArchive *)menu1p)->joints[13], 0, 4, -1, -1, -1);
    joint2 = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, -1, -1, -1);
    jobjCopyPos(joint1, joint2);
    if (menusetbtanm[0].cur != menusetbtanm[0].old) {
        menusetbtanm[0].state = 1;
        menusetbtanm[0].old = menusetbtanm[0].cur;
    }
    if (menusetbtanm[0].state == 1) {
        menusetbtanm[0].state = 2;
        sSetAnim1(menu1p, idx, GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, -1, -1, -1), menusetbtanm[0].cur);
    }
    if (menusetbtanm[0].state == 2) {
        if (jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, -1, -1, -1)) != 0) {
            menusetbtanm[0].state = 3;
            menuanmend = 1;
            if (menusetbtanm[0].cur == 5)
                return;
            if (menusetbtanm[0].cur == 0) {
                if (menusetbtanm[0].cur == 0)
                    menusetbtanm[0].cur = 1;
                sSetAnim1(menu1p, idx, GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, -1, -1, -1), menusetbtanm[0].cur);
                jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, -1, -1, -1));
            } else {
                menusetbtanm[0].state = 1;
            }
        }
    }
    HSD_JObjDispAll(joint2, 0, 7, 0);
}

void menu_sDispBattleSetLButtonWeapon(void)
{
    menu_sDispBattleSetLButtonAnm(1);
}

void menu_sDispBattleSetLButtonOption(void)
{
    menu_sDispBattleSetLButtonAnm(2);
}

void menu_sDispBattleSetLButtonSubParts(void)
{
    menu_sDispBattleSetLButtonAnm(3);
}

void menu_sDispBattleSetLButtonReset(void)
{
    s32 k = 4;
    s32 b = 9;
    HSD_JObj *j = 0;
    s32 dead = 0;

    j = GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1);
    if (menusetbtanm[k].cur != menusetbtanm[k].old) {
        menusetbtanm[k].state = 1;
        menusetbtanm[k].old = menusetbtanm[k].cur;
    }
    if (menusetbtanm[k].state == 1) {
        menusetbtanm[k].state = 2;
        sSetAnim1((MenuArchive *)menu1p, b, GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1), menusetbtanm[k].cur);
    }
    if (menusetbtanm[k].state == 2) {
        if (jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1)) != 0) {
            menusetbtanm[k].state = 3;
            menuanmend = 1;
            if (menusetbtanm[k].cur == 5) {
                return;
            }
            if (menusetbtanm[k].cur == 0) {
                if (menusetbtanm[k].cur == 0) {
                    menusetbtanm[k].cur = 1;
                }
                sSetAnim1((MenuArchive *)menu1p, b, GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1), menusetbtanm[k].cur);
                jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1));
            } else {
                menusetbtanm[k].state = 1;
            }
        }
    }
    HSD_JObjDispAll(j, 0, 7, 0);
}

void menu_sDispBattleSetLButtonExit(void)
{
    s32 k = 5;
    s32 b = 9;
    HSD_JObj *j = 0;
    s32 dead = 0;

    j = GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1);
    if (menusetbtanm[k].cur != menusetbtanm[k].old) {
        menusetbtanm[k].state = 1;
        menusetbtanm[k].old = menusetbtanm[k].cur;
    }
    if (menusetbtanm[k].state == 1) {
        menusetbtanm[k].state = 2;
        sSetAnim1((MenuArchive *)menu1p, b, GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1), menusetbtanm[k].cur);
    }
    if (menusetbtanm[k].state == 2) {
        if (jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1)) != 0) {
            menusetbtanm[k].state = 3;
            menuanmend = 1;
            if (menusetbtanm[k].cur == 5) {
                return;
            }
            if (menusetbtanm[k].cur == 0) {
                if (menusetbtanm[k].cur == 0) {
                    menusetbtanm[k].cur = 1;
                }
                sSetAnim1((MenuArchive *)menu1p, b, GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1), menusetbtanm[k].cur);
                jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1));
            } else {
                menusetbtanm[k].state = 1;
            }
        }
    }
    HSD_JObjDispAll(j, 0, 7, 0);
}

void menu_sDispBattleSetLButtonEtc(void)
{
    s32 unused0 = 6;
    s32 unused1 = 0;
    s32 i = 0;
    s32 dead = 0;
    s32 kind = 9;
    s32 row = 6;

    for (i = 0; i < 4; i++) {
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, row, i, -1, -1), 0, 7, 0);
        dead = 1;
    }
}

void menu_sDispBattleSetLButtonAnm(s32 k)
{
    s32 b = 9;
    HSD_JObj *a = 0;
    HSD_JObj *j = 0;

    a = GetChild(((MenuArchive *)menu1p)->joints[13], 0, k - 1, -1, -1, -1);
    j = GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1);
    if (menusetbtanm[k].cur != menusetbtanm[k].old) {
        menusetbtanm[k].state = 1;
        menusetbtanm[k].old = menusetbtanm[k].cur;
    }
    if (menusetbtanm[k].state == 1) {
        menusetbtanm[k].state = 2;
        sSetAnim1((MenuArchive *)menu1p, b, GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1), menusetbtanm[k].cur);
    }
    if (menusetbtanm[k].state == 2) {
        if (jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1)) != 0) {
            menusetbtanm[k].state = 3;
            menuanmend = 1;
            if (menusetbtanm[k].cur == 5) {
                return;
            }
            if (menusetbtanm[k].cur == 0) {
                if (menusetbtanm[k].cur == 0) {
                    menusetbtanm[k].cur = 1;
                }
                sSetAnim1((MenuArchive *)menu1p, b, GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1), menusetbtanm[k].cur);
                jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1));
            } else {
                menusetbtanm[k].state = 1;
            }
        }
    }
    jobjCopyPos(a, j);
    HSD_JObjDispAll(j, 0, 7, 0);
}

void menuZoidBattleSettingmenubuttonset(void)
{
    s32 dead = 0;
    s32 i = 0;

    for (i = 0; i < 6; i++)
        menusetbtanm[i].cur = 1;
    if (menumode == 1)
        menusetbtanm[menusetcl[menumode] + 4].cur = 2;
    else if (menumode == 2)
        menusetbtanm[menusetcl[menumode]].cur = 2;
    else if (menumode == 3)
        menusetbtanm[0].cur = 3;
    else if (menumode == 4)
        menusetbtanm[1].cur = 3;
}
extern void sWeaponUnMount(ZoidState *zoid, s32 slot);
extern void SeColorChg(void);
extern s32 zdWeaponColor(ZoidState *zoid, s32 slot, s32 color);
s32 menufncZoidBattle_BattleMenuSet(void)
{
    s32 dead4 = 0;
    s32 dead3 = 0;
    s32 dead2 = 0;
    s32 dead1 = 0;
    f32 deadf = 0.0f;
    s32 i = 0;
    s32 part = 0;
    s32 color = 1;
    s32 old = 0;
    s16 slot = 0;
    s16 idx = 0;
    s16 *flag = 0;
    s32 j = 0;
    s32 fail = 0;
    s32 added = 0;
    s32 mount;
    s32 slot2;
    s32 cur;

    menuCursor = 0;
    menuParticles = 0;
    menumode = 0;
    dlgpersonno = 0;
    dlgzoidno = 0;
    menudispwinstr[0] = 0;
    menudispyesnoflag = 0;
    TEA_SetTimerInit();
    IveSave[0].owned_zoid_count = 0;
    for (i = 0; i < 30; i++) {
        if (IveSave[0].owned_zoids[i] < 0)
            break;
    }
    IveSave[0].owned_zoid_count = i;
    memcpy(&IveSave[4], &IveSave[0], sizeof(GameSaveRecord));
    menubackcampos[0] = menubackcambak[0] - 2.0;
    for (i = 0; i < 12; i++) {
        menubackintpos[0] = menubackintbak[0] - i * 0.5;
        menu_sDispBattleSet();
    }
    menubackintpos[0] = menubackintbak[0] - 6.0;
    menu_sDispBattleSet();
    for (j = 0; j < 7; j++) {
        menusetbtanm[j].old = menusetbtanm[j].cur = 0;
        menusetbtanm[j].state = 1;
    }
    j = 7;
    menusetbtanm[j].old = menusetbtanm[j].cur = 1;
    menusetbtanm[j].state = 0;
    j = 8;
    menusetbtanm[j].old = menusetbtanm[j].cur = 1;
    menusetbtanm[j].state = 0;
    for (j = 9; j < 11; j++) {
        menusetbtanm[j].old = menusetbtanm[j].cur = 0;
        menusetbtanm[j].state = 0;
    }
    for (i = 0; i < 20; i++) {
        menusetcl[i] = 0;
        menusetsl[i] = 0;
        menusetst[i] = 0;
        menusetmh[i] = 0;
        menusetmn[i] = 0;
    }
    menusetcl[1] = 1;
    menusetsl[1] = 1;
    menusetmh[1] = 2;
    menusetmn[1] = 2;
    menusetcl[2] = 0;
    menusetsl[2] = 0;
    menusetmh[2] = 4;
    menusetmn[2] = 4;
    menusetcl[3] = 0;
    menusetsl[3] = 0;
    menusetmh[3] = 4;
    menusetmn[3] = IveSave[0].owned_zoid_count;
    if (menusetmn[3] < 4)
        menusetmh[3] = menusetmn[3];
    i = 4;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 8;
    menusetmn[i] = 8;
    i = 6;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 3;
    menusetmn[i] = 3;
    i = 5;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 5;
    i = 7;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 5;
    menusetmn[i] = 8;
    i = 8;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 5;
    menusetmn[i] = 10;
    flag_AllHavePartsList = 0;
    menuSortPartsHave();
    menumode = 1;
    menuZoidBattleSettingmenubuttonset();
    dlgzoidno = IveSave[0].field_15F4;
    menucuranm[0] = 1;
    SetMenuMsgAdr(0x1E, menudispwinstr);
    detCustm(IveSave[0].zoid_slot);
    flPicturePrologue();

    while (1) {
        sPadAll();
        menusetcl[menumode] = TEA_menufnc(&menusetsl[menumode], &menusetst[menumode], menusetmn[menumode], menusetmh[menumode]);
        if (menumode == 1) {
            if (menusetcl[menumode] == 0)
                SetMenuMsgAdr(0x1D, menudispwinstr);
            if (menusetcl[menumode] == 1)
                SetMenuMsgAdr(0x1E, menudispwinstr);
        } else if (menumode == 2) {
            SetMenuMsgAdr(menusetcl[menumode] + 0x19, menudispwinstr);
        } else if (menumode == 4) {
            SetMenuMsgAdr(0x20, menudispwinstr);
        } else if (menumode == 6) {
            SetMenuMsgAdr(0xA, menudispwinstr);
        }
        ttl_batset_detailsetparam();
        if (sPad1->field_4F || sPad1->field_53) {
            if (menumode == 4 || menumode == 7)
                menufnczoidmountpointdispset(menusetcl[4]);
            menuZoidBattleSettingmenubuttonset();
        }
        if (sPad1->field_50 || sPad1->field_54) {
            if (menumode == 4 || menumode == 7)
                menufnczoidmountpointdispset(menusetcl[4]);
            menuZoidBattleSettingmenubuttonset();
        }
        if (sPad1->field_51 || sPad1->field_55) {
            if (menumode == 1)
                menumode = 2;
            menuZoidBattleSettingmenubuttonset();
        }
        if (sPad1->field_52 || sPad1->field_56) {
            if (menumode == 2)
                menumode = 1;
            menuZoidBattleSettingmenubuttonset();
        }
        if (sPad1->field_5C)
            flag_AllHavePartsList++;
        if (sPad1->pressed_a) {
            if (menumode == 1) {
                if (menusetcl[menumode] == 0) {
                    SeConfirm();
                    if (IveSave[0].owned_part_count >= 450) {
                        menudispyesnopoint = 1;
                        menudispyesnoflag = 1;
                        do {
                            sPadAll();
                            menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x29), "NOT");
                            menu_sDispBattleSet();
                        } while (!sPad1->pressed_a);
                        SeRet();
                        menudispyesnoflag = 0;
                        continue;
                    }
                    menudispyesnopoint = 1;
                    menudispyesnoflag = 1;
                    do {
                        sPadAll();
                        menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x4A), lit_574);
                        menu_sDispBattleSet();
                    } while (!sPad1->pressed_a);
                    menudispyesnoflag = 0;
                    menu_sDispBattleSet();
                    if (menudispyesnopoint == 0) {
                        SeRet();
                        continue;
                    }
                    SeOn(0x7D);
                    ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                    fail = 0;
                    added = 0;
                    for (i = 0; i < 8; i++) {
                        if (IveSave[0].owned_part_count < 450) {
                            part = ttl_ZoidRemoveContractWep(0, IveSave[0].zoid_slot, i);
                            if (part != -1) {
                                ttl_HaveAddParts(0, part);
                                added = 1;
                            }
                        } else {
                            fail = 1;
                        }
                    }
                    if (IveSave[0].owned_part_count < 450) {
                        part = ttl_ZoidRemoveContractOpt(0, IveSave[0].zoid_slot);
                        if (part != -1) {
                            ttl_HaveAddParts(0, part);
                            added = 1;
                        }
                    } else {
                        fail = 1;
                    }
                    for (i = 0; i < 3; i++) {
                        if (IveSave[0].owned_part_count < 450) {
                            part = ttl_ZoidRemoveContractSub(0, IveSave[0].zoid_slot, i);
                            if (part != -1) {
                                ttl_HaveAddParts(0, part);
                                added = 1;
                            }
                        } else {
                            fail = 1;
                        }
                    }
                    if (fail == 1 && added == 1) {
                        menudispyesnopoint = 1;
                        menudispyesnoflag = 1;
                        do {
                            sPadAll();
                            menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x29), "NOT");
                            menu_sDispBattleSet();
                        } while (!sPad1->pressed_a);
                        SeRet();
                        menudispyesnoflag = 0;
                        menu_sDispBattleSet();
                    }
                    menudispyesnoflag = 0;
                    ttl_WeaponAllMount(IveSave[0].zoid_slot);
                    continue;
                }
                if (menusetcl[menumode] == 1) {
                    SeRet();
                    menudispwinstr[0] = 0;
                    menusetbtanm[5].cur = 4;
                    menusetbtanm[5].state = 1;
                    if (IveSave[4].zoid_slot == IveSave[0].zoid_slot)
                        break;
                    SeLiftStart();
                    menu_StageUpDownFlag = 0;
                    menuDetailMode = 0;
                    while (!menu_sDispBattleStageDown(1))
                        menu_sDispBattleSet();
                    SeLiftStop();
                    ZoidWeaponPurge(&Zoid[0]);
                    ttl_WeaponAllUnMount(menusetcl[3]);
                    ZoidPurge(&Zoid[0]);
                    i = IveSave[0].zoid_slot;
                    part = IveSave[0].zoid_slot;
                    fail = IveSave[0].zoid_slot;
                    IveSave[0].zoid_slot = IveSave[4].zoid_slot;
                    i = 0;
                    i = 0;
                    i = 0;
                    i = 0;
                    i = 0;
                    IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                    ZoidInitC(&Zoid[0], IveSave[0].field_15F4, IveSave[0].zoid_color[IveSave[0].zoid_slot]);
                    ttl_WeaponAllMount(IveSave[0].zoid_slot);
                    SeLiftStart();
                    menu_StageUpDownFlag = 0;
                    menuDetailMode = 0;
                    while (!menu_sDispBattleStageDown(0))
                        menu_sDispBattleSet();
                    SeLiftStop();
                    break;
                }
            }
            if (menumode == 2) {
                if (menusetcl[menumode] == 0) {
                    SeRet();
                    menumode = 3;
                    menuZoidBattleSettingmenubuttonset();
                    menu_sDispBattleSet();
                    menusetbtanm[7].cur = 0;
                    menusetbtanm[7].state = 1;
                    while (menusetbtanm[7].cur != 1)
                        menu_sDispBattleSet();
                    menuanmend = 0;
                    while (menuanmend == 0)
                        menu_sDispBattleSet();
                    continue;
                }
                if (menusetcl[menumode] == 1) {
                    menusetcl[4] = 0;
                    menusetsl[4] = 0;
                    menusetmh[4] = 0;
                    menusetmn[4] = 0;
                    for (i = 0; i < 8; i++) {
                        if (ttl_ZoidChkWepPoint(IveSave[0].field_15F4, i) == 1) {
                            menusetmh[4] = i + 1;
                            menusetmn[4] = i + 1;
                        }
                    }
                    if (menusetmh[4] == 0) {
                        SeCancel();
                        continue;
                    }
                    SeRet();
                    menuzoidequippartslist = 0;
                    menusetbtanm[8].cur = 0x1A;
                    menusetbtanm[8].old = 0x1A;
                    menusetbtanm[8].state = 0;
                    menumode = 4;
                    menuZoidBattleSettingmenubuttonset();
                    menu_sDispBattleSet();
                    menusetbtanm[7].cur = 3;
                    menuanmend = 0;
                    menusetbtanm[7].state = 1;
                    while (menusetbtanm[7].cur != 4)
                        menu_sDispBattleSet();
                    menu_sDispBattleSet();
                    menuzoidequippartslist = 1;
                    menusetbtanm[8].cur = 0;
                    menusetbtanm[8].state = 1;
                    while (menusetbtanm[8].state != 3)
                        menu_sDispBattleSet();
                    menusetbtanm[8].cur = 1;
                    menu_sDispBattleSet();
                    menufnczoidmountpointdispset(menusetcl[4]);
                    continue;
                }
                if (menusetcl[menumode] == 2) {
                    menuSortPartsHave();
                    menuPartsHaveListZoid(2, IveSave[0].field_15F4);
                    if (IveSave[0].list_count == 0) {
                        menusetcl[5] = 0;
                        menusetsl[5] = 0;
                        menusetmh[5] = 0;
                        menusetmn[5] = 0;
                        MessageBoxBatNoParts();
                    } else {
                        SeRet();
                    }
                    i = 5;
                    menusetmn[i] = IveSave[0].list_count;
                    menusetmh[i] = 5;
                    if (menusetmn[i] < 5)
                        menusetmh[i] = menusetmn[i];
                    if (menusetmn[i] < menusetsl[i] - 1) {
                        menusetcl[i] = 0;
                        menusetsl[i] = 0;
                        menusetst[i] = 0;
                    }
                    if (menusetmh[i] < menusetsl[i] - 1) {
                        menusetcl[i] = 0;
                        menusetsl[i] = 0;
                        menusetst[i] = 0;
                    }
                    menusetst[i] = 0;
                    menumode = 5;
                    menuZoidBattleSettingmenubuttonset();
                    menu_sDispBattleSet();
                    fail = IveSave[0].zoid_slot;
                    idx = IveSave[0].zoid_slot;
                    added = IveSave[0].zoid_slot;
                    menusetbtanm[7].cur = 6;
                    menuanmend = 0;
                    menusetbtanm[7].state = 1;
                    fail *= 3;
                    while (menusetbtanm[7].cur != 7)
                        menu_sDispBattleSet();
                    OSReport("menumode01=Start\n");
                    menusetbtanm[7].cur = 7;
                    menu_sDispBattleSet();
                    menuzoidhavepartslist = 1;
                    menusetbtanm[8].cur = 2;
                    menusetbtanm[8].state = 1;
                    menuzoidequippartslist = 1;
                    menu_sDispBattleSet();
                    menuzoidhavepartslist = 1;
                    while (menusetbtanm[8].state != 3)
                        menu_sDispBattleSet();
                    menusetbtanm[8].cur = 3;
                    menusetbtanm[8].state = 1;
                    menuzoidhavepartslist = 1;
                    menu_sDispBattleSet();
                    OSReport("menumode03=Start\n");
                    i = 5;
                    menusetcl[i] = 0;
                    menusetsl[i] = 0;
                    menusetst[i] = 0;
                    menusetcl[menumode] = menusetsl[menumode] = menusetst[menumode] = 0;
                    continue;
                }
                if (menusetcl[menumode] == 3) {
                    SeRet();
                    menuzoidequippartslist = 0;
                    menusetbtanm[8].cur = 0x1A;
                    menusetbtanm[8].old = 0x1A;
                    menusetbtanm[8].state = 0;
                    menumode = 6;
                    menuZoidBattleSettingmenubuttonset();
                    menu_sDispBattleSet();
                    menuzoidequippartslist = 1;
                    menusetbtanm[8].cur = 0;
                    menusetbtanm[8].state = 1;
                    while (menusetbtanm[8].state != 3)
                        menu_sDispBattleSet();
                    menusetbtanm[8].cur = 1;
                    menusetbtanm[8].state = 1;
                    menu_sDispBattleSet();
                    menufnczoidmountpointdispset(menusetcl[4]);
                    continue;
                }
            }
            if (menumode == 3) {
                if (IveSave[0].zoid_slot == menusetcl[3])
                    continue;
                SeRet();
                menuZoidBattleSettingmenubuttonset();
                menuanmend = 0;
                menusetbtanm[7].cur = 2;
                menusetbtanm[7].state = 1;
                while (menusetbtanm[7].cur != 9)
                    menu_sDispBattleSet();
                menu_sDispBattleSet();
                menumode = 2;
                SeLiftStart();
                menu_StageUpDownFlag = 0;
                menuDetailMode = 0;
                while (!menu_sDispBattleStageDown(1))
                    menu_sDispBattleSet();
                SeLiftStop();
                ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                ZoidPurge(&Zoid[0]);
                IveSave[0].zoid_slot = menusetcl[3];
                IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                ZoidInitC(&Zoid[0], IveSave[0].field_15F4, IveSave[0].zoid_color[IveSave[0].zoid_slot]);
                ttl_WeaponAllMount(IveSave[0].zoid_slot);
                SeLiftStart();
                menu_StageUpDownFlag = 0;
                menuDetailMode = 0;
                while (menu_sDispBattleStageDown(0) == 0)
                    menu_sDispBattleSet();
                SeLiftStop();
                dlgzoidno = IveSave[0].field_15F4;
                menuZoidBattleSettingmenubuttonset();
                for (i = 4; i <= 8; i++) {
                    menusetcl[i] = menusetsl[i] = menusetst[i] = 0;
                }
                menuDetailMode = 0;
                detCustm(IveSave[0].zoid_slot);
                menuDetailMode = 0;
            }
            if (menumode == 4) {
                menuPartsWepSetListZoid(1, IveSave[0].field_15F4, menusetsl[4]);
                if (IveSave[0].list_count == 0) {
                    MessageBoxBatNoParts();
                    continue;
                }
                if (ttl_ZoidWepPointPermission(IveSave[0].field_15F4, menusetsl[4]) == 0) {
                    MessageBoxBatNoParts();
                    continue;
                }
                SeRet();
                menuzoidhavepartslist = 1;
                menusetbtanm[8].cur = menusetsl[4] * 3 + 2;
                menusetbtanm[8].state = 1;
                while (menusetbtanm[8].state != 3)
                    menu_sDispBattleSet();
                menusetbtanm[8].cur = menusetsl[4] * 3 + 3;
                menusetbtanm[8].state = 1;
                menu_sDispBattleSet();
                menumode = 7;
                menuZoidBattleSettingmenubuttonset();
                menu_sDispBattleSet();
                i = 7;
                menusetmn[i] = IveSave[0].list_count;
                menusetmh[i] = 5;
                if (menusetmn[i] < 5)
                    menusetmh[i] = menusetmn[i];
                if (menusetmn[i] < menusetsl[i] - 1) {
                    menusetcl[i] = 0;
                    menusetsl[i] = 0;
                    menusetst[i] = 0;
                }
                if (menusetmh[i] < menusetsl[i] - 1) {
                    menusetcl[i] = 0;
                    menusetsl[i] = 0;
                    menusetst[i] = 0;
                }
                menusetcl[i] = menusetst[i] = menusetsl[i] = 0;
                menusetst[i] = 0;
                menusetcl[menumode] = menusetsl[menumode] = menusetst[menumode] = 0;
                continue;
            }
            if (menumode == 7) {
                SeRet();
                slot = IveSave[0].zoid_slot;
                old = IveSave[0].zoid_parts[slot][menusetcl[4]];
                OSReport("wep %d ", old);
                IveSave[0].zoid_parts[slot][menusetcl[4]] = IveSave[0].list_parts[menusetcl[7]];
                menuzoidequippartslist = 1;
                menusetbtanm[8].cur = 4;
                menusetbtanm[8].state = 1;
                while (menusetbtanm[8].cur != 0x1B)
                    menu_sDispBattleSet();
                menuzoidhavepartslist = 0;
                menusetbtanm[8].cur = 1;
                menusetbtanm[8].state = 1;
                menu_sDispBattleSet();
                menumode = 4;
                menuZoidBattleSettingmenubuttonset();
                menu_sDispBattleSet();
                ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                ttl_WeaponAllMount(IveSave[0].zoid_slot);
                menuzoidhavepartslist = 0;
                idx = IveSave[0].list_flag[menusetcl[7]];
                if (old != -1) {
                    IveSave[0].owned_parts[idx] = old;
                    continue;
                }
                IveSave[0].owned_part_count--;
                IveSave[0].owned_parts[idx] = *(s16 *)((u8 *)&IveSave[0].owned_parts[IveSave[0].owned_part_count] + (idx = IveSave[0].list_flag[menusetcl[7]], 0));
                IveSave[0].owned_parts[IveSave[0].owned_part_count] = -1;
                continue;
            }
            if (menumode == 6) {
                menuSortPartsHave();
                menuPartsHaveListZoid(3, IveSave[0].field_15F4);
                if (IveSave[0].list_count == 0) {
                    MessageBoxBatNoParts();
                    continue;
                }
                SeRet();
                i = IveSave[0].zoid_slot;
                menuzoidhavepartslist = 1;
                menusetbtanm[8].cur = (menusetsl[6] - 1) * 3 + 5;
                menusetbtanm[8].state = 1;
                while (menusetbtanm[8].state != 3)
                    menu_sDispBattleSet();
                menusetbtanm[8].cur = (menusetsl[6] - 1) * 3 + 6;
                menusetbtanm[8].state = 1;
                menu_sDispBattleSet();
                menumode = 8;
                menuZoidBattleSettingmenubuttonset();
                menu_sDispBattleSet();
                menuSortPartsHave();
                menuPartsHaveListZoid(3, IveSave[0].field_15F4);
                i = 8;
                menusetmn[i] = IveSave[0].list_count;
                menusetmh[i] = 5;
                if (menusetmn[i] < 5)
                    menusetmh[i] = menusetmn[i];
                if (menusetmn[i] < menusetsl[i] - 1) {
                    menusetcl[i] = 0;
                    menusetsl[i] = 0;
                    menusetst[i] = 0;
                }
                if (menusetmh[i] < menusetsl[i] - 1) {
                    menusetcl[i] = 0;
                    menusetsl[i] = 0;
                    menusetst[i] = 0;
                }
                menusetst[i] = 0;
                menusetcl[menumode] = menusetsl[menumode] = menusetst[menumode] = 0;
                continue;
            }
            if (menumode == 8) {
                SeRet();
                menuzoidequippartslist = 1;
                menusetbtanm[8].cur = menusetsl[6] * 3 + 7;
                menusetbtanm[8].state = 1;
                while (menusetbtanm[8].cur != 0x1B)
                    menu_sDispBattleSet();
                menuzoidhavepartslist = 0;
                menusetbtanm[8].cur = 1;
                menusetbtanm[8].state = 1;
                menu_sDispBattleSet();
                slot = IveSave[0].zoid_slot;
                i = menusetcl[6];
                old = IveSave[0].zoid_field_11DA[slot][i];
                part = IveSave[0].list_parts[menusetcl[8]];
                OSReport("Sub=%d %d %d custm=%d po=%d\n", i, old, part, slot, menusetcl[8]);
                IveSave[0].zoid_field_11DA[slot][i] = part;
                menumode = 6;
                menuZoidBattleSettingmenubuttonset();
                menu_sDispBattleSet();
                menuzoidhavepartslist = 0;
                if (old > 0) {
                    IveSave[0].owned_parts[IveSave[0].list_flag[menusetcl[8]]] = old;
                    OSReport("List1=%d po=%d wep=%d\n", menusetcl[8], IveSave[0].list_flag[menusetcl[8]], IveSave[0].owned_parts[IveSave[0].list_flag[menusetcl[8]]]);
                } else {
                    part = (s32)&menusetcl[8];
                    (flag = &IveSave[0].list_flag[*(s16 *)part], i = old, IveSave[0].owned_part_count) = IveSave[0].owned_part_count - 1;
                    old = IveSave[0].owned_parts[IveSave[0].owned_part_count];
                    IveSave[0].owned_parts[*flag] = old;
                    IveSave[0].owned_parts[IveSave[0].owned_part_count] = -1;
                    OSReport("List2=%d po=%d wep=%d\n", menusetcl[8], IveSave[0].list_flag[menusetcl[8]], old);
                }
            }
            if (menumode == 5) {
                if (IveSave[0].list_count == 0) {
                    SeCancel();
                    continue;
                }
                SeRet();
                menusetbtanm[7].cur = 8;
                menusetbtanm[7].state = 1;
                while (menusetbtanm[7].cur != 9)
                    menu_sDispBattleSet();
                OSReport("menumode5=End\n");
                menuzoidhavepartslist = 0;
                menu_sDispBattleSet();
                slot = IveSave[0].zoid_slot;
                old = IveSave[0].zoid_field_119E[slot];
                IveSave[0].zoid_field_119E[slot] = IveSave[0].list_parts[menusetcl[5]];
                idx = IveSave[0].list_flag[menusetcl[5]];
                if (old > 0) {
                    IveSave[0].owned_parts[idx] = old;
                } else {
                    IveSave[0].owned_part_count--;
                    IveSave[0].owned_parts[idx] = IveSave[0].owned_parts[IveSave[0].owned_part_count];
                    IveSave[0].owned_parts[IveSave[0].owned_part_count] = -1;
                }
                menuSortPartsHave();
                menuPartsHaveListZoid(2, IveSave[0].field_15F4);
                menumode = 2;
                menuZoidBattleSettingmenubuttonset();
                continue;
            }
        }
        if (sPad1->field_5E) {
            SeCancel();
            if (menumode == 3) {
                menusetbtanm[7].cur = 2;
                menusetbtanm[7].state = 1;
                menuanmend = 0;
                while (menusetbtanm[7].cur != 9)
                    menu_sDispBattleSet();
                menu_sDispBattleSet();
                menumode = 2;
                menuZoidBattleSettingmenubuttonset();
                menuanmend = 0;
                while (menuanmend == 0)
                    menu_sDispBattleSet();
                continue;
            }
            if (menumode == 4) {
                menufnczoidmountpointdispset(-1);
                menusetbtanm[8].cur = 0x1A;
                menusetbtanm[8].state = 1;
                menuzoidequippartslist = 0;
                menusetbtanm[7].cur = 5;
                menusetbtanm[7].state = 1;
                while (menusetbtanm[7].cur != 9)
                    menu_sDispBattleSet();
                menu_sDispBattleSet();
                menuanmend = 0;
                menumode = 2;
                menuZoidBattleSettingmenubuttonset();
                menuanmend = 0;
                continue;
            }
            if (menumode == 7) {
                menuzoidequippartslist = 1;
                menusetbtanm[8].cur = menusetsl[4] * 3 + 4;
                menusetbtanm[8].state = 1;
                while (menusetbtanm[8].cur != 0x1B)
                    menu_sDispBattleSet();
                menuzoidhavepartslist = 0;
                menusetbtanm[8].cur = 1;
                menusetbtanm[8].state = 1;
                menu_sDispBattleSet();
                menuzoidhavepartslist = 0;
                menumode = 4;
                menuZoidBattleSettingmenubuttonset();
                menuanmend = 0;
                while (menuanmend == 0)
                    menu_sDispBattleSet();
                continue;
            }
            if (menumode == 5) {
                menusetbtanm[7].cur = 8;
                menusetbtanm[7].state = 1;
                while (menusetbtanm[7].cur != 9)
                    menu_sDispBattleSet();
                OSReport("menumode5=End\n");
                menuzoidhavepartslist = 0;
                menu_sDispBattleSet();
                menumode = 2;
                menuZoidBattleSettingmenubuttonset();
                continue;
            }
            if (menumode == 6) {
                menusetbtanm[8].cur = 0x1A;
                menusetbtanm[8].state = 1;
                menuzoidequippartslist = 0;
                menuanmend = 0;
                menumode = 2;
                menuZoidBattleSettingmenubuttonset();
                continue;
            }
            if (menumode == 8) {
                menuzoidequippartslist = 1;
                menusetbtanm[8].cur = menusetsl[6] * 3 + 4;
                menusetbtanm[8].state = 1;
                while (menusetbtanm[8].cur != 0x1B)
                    menu_sDispBattleSet();
                menuzoidhavepartslist = 0;
                menusetbtanm[8].cur = 1;
                menusetbtanm[8].state = 1;
                menu_sDispBattleSet();
                menuzoidhavepartslist = 0;
                menumode = 6;
                menuZoidBattleSettingmenubuttonset();
                menuanmend = 0;
                while (menuanmend == 0)
                    menu_sDispBattleSet();
                continue;
            }
            menumode = 1;
            menusetcl[menumode] = 1;
            menusetst[menumode] = 0;
            menusetsl[menumode] = 1;
            menuZoidBattleSettingmenubuttonset();
            continue;
        }
        if (sPad1->field_5B) {
            if (menumode == 4 && IveSave[0].owned_part_count < 450) {
                part = ttl_ZoidRemoveContractWep(0, IveSave[0].zoid_slot, menusetcl[menumode]);
                if (part != -1) {
                    SeOn(0x7D);
                    sWeaponUnMount(&Zoid[0], ttl_GetMountNo(IveSave[0].owned_zoids[IveSave[0].zoid_slot], menusetcl[menumode]));
                    ttl_HaveAddParts(0, part);
                } else {
                    MessageBoxBat(GetMenuMsgAdr(0x65));
                }
            }
            if (menumode == 5) {
                if (IveSave[0].list_count == 0) {
                    menusetcl[5] = 0;
                    menusetsl[5] = 0;
                    menusetmh[5] = 1;
                    menusetmn[5] = 1;
                }
                if (IveSave[0].owned_part_count < 450) {
                    part = ttl_ZoidRemoveContractOpt(0, IveSave[0].zoid_slot);
                    if (part != -1) {
                        SeOn(0x7D);
                        ttl_HaveAddParts(0, part);
                    }
                    i = 5;
                    menuSortPartsHave();
                    menuPartsHaveListZoid(2, IveSave[0].field_15F4);
                    menusetmn[i] = IveSave[0].list_count;
                    menusetmh[i] = 5;
                    if (menusetmn[i] < 5)
                        menusetmh[i] = menusetmn[i];
                    if (menusetmn[i] < menusetsl[i] - 1) {
                        menusetcl[i] = 0;
                        menusetsl[i] = 0;
                        menusetst[i] = 0;
                    }
                    if (menusetmh[i] < menusetsl[i] - 1) {
                        menusetcl[i] = 0;
                        menusetsl[i] = 0;
                        menusetst[i] = 0;
                    }
                    menusetst[i] = 0;
                }
            }
            if (menumode == 6 && IveSave[0].owned_part_count < 450) {
                part = ttl_ZoidRemoveContractSub(0, IveSave[0].zoid_slot, menusetcl[menumode]);
                if (part != -1) {
                    SeOn(0x7D);
                    ttl_HaveAddParts(0, part);
                }
            }
        }
        if (sPad1->field_60) {
            slot2 = IveSave[0].zoid_slot;
            cur = menusetcl[menumode];
            i = cur;
            if (menumode <= 6) {
                if (menumode != 4) {
                    (void)(IveSave[0].owned_parts[cur] + 1);
                    color = IveSave[0].zoid_color[IveSave[0].zoid_slot] + 1;
                    i = 0;
                    if (ZoidColor(&Zoid[0], color, 0) == 0) {
                        color = 0;
                        ZoidColor(&Zoid[0], color, 0);
                    } else {
                        SeColorChg();
                    }
                    OSReport("ZoidColor=%d \n", color);
                    IveSave[0].zoid_color[IveSave[0].zoid_slot] = (s8)color;
                    GameZoid[0].color = (s8)color;
                } else {
                    mount = ttl_GetMountNo(*(s32 *)((u8 *)&Zoid[0] + 0x20), cur);
                    if (*(s32 *)((u8 *)&Zoid[0] + 0x20) != IveSave[0].owned_zoids[slot2])
                        OSReport("zoid number error zdN=%d,custm-zoid=%d\n", *(s32 *)((u8 *)&Zoid[0] + 0x20), IveSave[0].owned_zoids[slot2]);
                    color = IveSave[0].part_colors[slot2][mount];
                    color = color + 1;
                    if (zdWeaponColor(&Zoid[0], mount, color) == 0) {
                        color = 0;
                        zdWeaponColor(&Zoid[0], mount, 0);
                        OSReport("color=0 \n");
                    } else {
                        SeColorChg();
                    }
                    IveSave[0].part_colors[slot2][mount] = (s8)color;
                    OSReport("slt=%d mtn=%d color=%d \n", cur, mount, color);
                }
            }
        }
        menu_sDispBattleSet();
        (void)old;
        (void)old;
        (void)old;
        (void)idx;
        (void)idx;
        (void)cur;
        (void)cur;
        (void)slot2;
        (void)slot2;
        (void)mount;
    }
    flPictureEpilogue();
    IveSave[0].owned_zoid_count = 0;
    for (i = 0; i < 30; i++) {
        if (IveSave[0].owned_zoids[i] < 0)
            break;
    }
    IveSave[0].owned_zoid_count = i;
    for (j = 0; j < 11; j++) {
        menusetbtanm[j].cur = 5;
        menusetbtanm[j].state = 1;
    }
    menuanmend = 0;
    menumode = 0;
    menu_sDispBattleSet();
    menubackcampos[0] = menubackcambak[0];
    for (i = 0; i < 12; i++) {
        menubackintpos[0] = -6.0 - (menubackintbak[0] - i * 0.5);
        menu_sDispBattleSet();
    }
    menubackcampos[0] = menubackcambak[0];
    menubackintpos[0] = menubackintbak[0];
    menu_sDispBattleSet();
    return 0xCD;
}
#undef lit_4735
#define lit_4735 "NOT"
#undef lit_4736
#define lit_4736 "menumode01=Start\n"
#undef lit_4737
#define lit_4737 "menumode03=Start\n"
#undef lit_4738
#define lit_4738 "wep %d "
#undef lit_4739
#define lit_4739 "Sub=%d %d %d custm=%d po=%d\n"
#undef lit_4740
#define lit_4740 "List1=%d po=%d wep=%d\n"
#undef lit_4741
#define lit_4741 "List2=%d po=%d wep=%d\n"
#undef lit_4742
#define lit_4742 "menumode5=End\n"
#undef lit_4743
#define lit_4743 "ZoidColor=%d \n"
#undef lit_4744
#define lit_4744 "zoid number error zdN=%d,custm-zoid=%d\n"
#undef lit_4745
#define lit_4745 "color=0 \n"
#undef lit_4746
#define lit_4746 "slt=%d mtn=%d color=%d \n"

void MessageBoxBatNoParts(void)
{
    MessageBoxBat(GetMenuMsgAdr(0x66));
}

void MessageBoxBat(char *msg)
{
    MessageBox(msg);
    SeConfirm();
    do {
        sPadAll();
        menu_sDispBattleSet();
    } while (sPad1->pressed_a == 0 && sPad1->field_5E == 0);
    SeOn(0x6F);
    menudispyesnoflag = 0;
}

void ttl_batset_detailsetparam(void)
{
    s32 v = 0;

    if (menumode == 2) {
        detCustm(IveSave[0].zoid_slot);
    }
    if (menumode == 3) {
        detCustm(menusetcl[3]);
    }
    if (menumode == 4) {
        v = IveSave[0].zoid_parts[IveSave[0].zoid_slot][menusetcl[4]];
        if (v > 0) {
            detWeapon(v);
        } else {
            menuDetailMode = 0;
        }
    }
    if (menumode == 7) {
        detWeapon(IveSave[0].list_parts[menusetcl[7]]);
    }
    if (menumode == 5) {
        v = IveSave[0].list_parts[menusetcl[5]];
        if (v > 0 && IveSave[0].list_count > 0) {
            detOption(v);
        } else {
            menuDetailMode = 0;
        }
    }
    if (menumode == 6) {
        v = IveSave[0].zoid_field_11DA[IveSave[0].zoid_slot][menusetcl[6]];
        if (v > 0) {
            detSubparts(v);
        } else {
            menuDetailMode = 0;
        }
    }
    if (menumode == 8) {
        detSubparts(IveSave[0].list_parts[menusetcl[8]]);
    }
}

void menu_sDispBattleFarm(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    register s32 dead = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(2);
        menu_sDispBattleFarmBack();
        menu_sDispBattleFarmZoid();
        menu_sDispBattleFarmWeapon();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        if (menumodeanm[0] != 0) {
            menu_sDispBattleFarmTitle();
            menu_sDispBattleFarmTitleSubButton();
            menu_sDispBattleFarmModeButton(0, 0);
            menu_sDispBattleFarmPersonZoidPartner();
            menu_sDispSaveLoadMenu();
            menu_sDispMsgWin(3, 1);
            menu_sDispBattleFarmMsgWindowHelp();
            menu_sDispDetailAll();
            menu_sDispYesNoMenu();
        } else {
            ttljobjcopyposdispall(GetChild(((MenuArchive *)menu1p)->joints[1], 0, 2, -1, -1, -1), GetChild(((MenuArchive *)menu1p)->joints[1], 0, 1, 5, -1, -1));
            ttljobjcopyposdispall(GetChild(((MenuArchive *)menu1p)->joints[1], 0, 1, 2, -1, -1), GetChild(((MenuArchive *)menu1p)->joints[1], 0, 4, 7, -1, -1));
        }
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}

void menu_sDispMsgWin(s16 index, s16 value)
{
    s32 unused0 = 0;
    s32 unused1 = 0;
    s32 unused2 = 0;
    char text[200];
    HSD_JObj *joint1 = 0;
    HSD_JObj *joint0 = 0;

    if (menu1p != NULL) {
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[index], 0, value, -1, -1, -1), 0, 7, 0);
        if (menudispwinstr != NULL) {
            sprintf(text, "%s ", menudispwinstr);
            joint0 = GetChild(((MenuArchive *)menu1p)->joints[index], 0, value, 0, -1, -1);
            joint1 = GetChild(((MenuArchive *)menu1p)->joints[index], 0, value, 1, -1, -1);
            sPutFontRect(text, joint0, joint1, (GXColor){ 255, 255, 255, 255 }, (GXColor){ 0, 0, 0, 0 }, 0, 2);
        }
    }
}

void menu_sDispBattleFarmMsgWindowHelp(void)
{
    s16 x = 0;
    s16 y = 0;
    s16 z = 0;
    s32 u = 0;
    s32 v = 0;
    register s32 country;
    register s32 b;
    s32 c;

    country = 0;
    country = 0;
    b = 0;
    c = 0;
    c = 0;
    if (jobjAnim(((MenuArchive *)menu1p)->joints[4]) != 0) {
        SetAnimArc2((MenuArchive *)menu1p, 4, 1);
        jobjAnim(((MenuArchive *)menu1p)->joints[4]);
    }
    country = (s8)IveSave[0].country;
    HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[4], 0, country, -1, -1, -1), 0, 7, 0);
}

void menu_sDispBattleFarmPersonZoidPartner(void)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    char text[0x3C];
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s32 dead1 = 0;
    s32 dead2 = 0;

    dead1 = 0;
    dead2 = 0;
    src = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 0, 0, -1, -1);
    dst = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 3, -1, -1);
    jobjCopyPos(src, dst);
    jobjDispAll(dst);
    src = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 0, 0, -1, -1);
    dst = 0;
    if (menu_personp[IveSave[0].field_1600] != 0) {
        dst = GetChild(((MenuArchive *)menu_personp[IveSave[0].field_1600])->joints[0], 0, 1, -1, -1, -1);
    }
    jobjCopyPos(src, dst);
    jobjDispAll(dst);
    sprintf(text, lit_3644, Tea_persondat[IveSave[0].field_1600].name);
    putAscJobj(src, 0, text);
    src = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 0, 1, -1, -1);
    dst = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 3, -1, -1);
    jobjCopyPos(src, dst);
    jobjDispAll(dst);
    src = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 0, 1, -1, -1);
    dst = GetChild(((MenuArchive *)menu_zoidp[IveSave[0].field_15F4])->joints[0], 0, 1, -1, -1, -1);
    jobjCopyPos(src, dst);
    jobjDispAll(dst);
    sprintf(text, lit_3644, Tea_zoiddat[IveSave[0].field_15F4].name);
    putAscJobj(src, 0, text);
    if ((u8)menuZoidBattleModeSingleTeam == 1) {
        src = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 1, 0, -1, -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 3, -1, -1);
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
        src = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 1, 0, -1, -1);
        dst = 0;
        if (menu_personp[IveSave[0].field_1602] != 0) {
            dst = GetChild(((MenuArchive *)menu_personp[IveSave[0].field_1602])->joints[0], 0, 1, -1, -1, -1);
        }
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
        sprintf(text, lit_3644, Tea_persondat[IveSave[0].field_1602].name);
        putAscJobj(src, 0, text);
        src = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 1, 1, -1, -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 3, -1, -1);
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
        src = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 1, 1, -1, -1);
        dst = GetChild(((MenuArchive *)menu_zoidp[IveSave[0].field_15FA])->joints[0], 0, 1, -1, -1, -1);
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
        sprintf(text, lit_3644, Tea_zoiddat[IveSave[0].field_15FA].name);
        putAscJobj(src, 0, text);
    }
}

void menu_sDispSetButton(void)
{
    s16 i = 0;
    s16 j = 0;

    if (menumode != 0) {
        for (i = 2; i <= 6; i++)
            menufarmanm[i] = 3;
    }
    switch (menumode) {
    case -1:
        for (i = 0; i <= 9; i++) {
            menufarmanm[i] = 2;
            menumodeanm[i] = 1;
        }
        break;
    case 0:
        menufarmanm[1] = 3;
        menufarmanm[7] = 3;
        menufarmanm[8] = 3;
        menufarmanm[9] = 3;
        for (i = 2; i <= 6; i++)
            menufarmanm[i] = 3;
        i = menufarmcl[0] + 2;
        menufarmanm[i] = 4;
        break;
    case 1:
        if (menufarmcl[1] == 0) {
            i = 1;
            j = 7;
        } else {
            i = 7;
            j = 1;
        }
        menufarmanm[i] = 4;
        menufarmanm[j] = 3;
        break;
    case 2:
        i = 1;
        menufarmanm[i] = 5;
        if (menufarmcl[2] == 0) {
            i = 8;
            j = 9;
        } else {
            i = 9;
            j = 8;
        }
        menufarmanm[i] = 4;
        menufarmanm[j] = 3;
        break;
    }
}

void menu_sDispBattleFarmTitle(void)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    s32 e = 0;
    s32 idx = 0;
    s32 joint = 0;
    s32 dead = 0;

    idx = 0;
    dead = 0;
    idx = 0;
    if ((u8)menutitleanm[0].state != menutitleanm[0].prev) {
        menutitleanm[0].mode = 1;
        menutitleanm[0].prev = menutitleanm[0].state;
    }
    if (menutitleanm[0].mode == 1) {
        menutitleanm[0].mode = 2;
        SetAnimArc2((MenuArchive *)menu1p, idx, menutitleanm[0].state);
    }
    if (menutitleanm[0].mode == 2 && menutitleanm[0].mode == 2) {
        joint = jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
        if (joint != 0) {
            menutitleanm[0].mode = 3;
            menuanmend = 1;
            if (menutitleanm[0].state == 2) {
                menutitleanm[0].mode = 0;
                return;
            }
            if ((u8)menutitleanm[0].state <= 2) {
                if (menutitleanm[0].state == 0)
                    menutitleanm[0].state = 1;
                if (menutitleanm[0].state == 1)
                    menutitleanm[0].mode = 1;
                SetAnimArc2((MenuArchive *)menu1p, idx, menutitleanm[0].state);
                jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
            }
        }
    }
    if (menutitleanm[0].mode != 0) {
        ttlMenuDispParts(((MenuArchive *)menu1p)->joints[idx], 0, -1, -1, -1);
        ttlMenuDispParts(((MenuArchive *)menu1p)->joints[idx], 0, menutitleguide[0], -1, -1);
    }
}

void menu_sDispBattleFarmTitleSubButton(void)
{
    s32 dead = 0;
    s32 kind = 0;
    s16 i = 0;
    s32 count = 0;
    s16 g = 0;
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s32 flag = 0;

    dead = 0;
    kind = 0;
    count = 0;
    for (i = 1; i < 7; i++) {
        if (menutitleguide[i] != -1) {
            count++;
        }
    }
    for (i = 1; i < 7; i++) {
        if (menutitleguide[i] != -1) {
            kind = 1;
            count = menutitleanm[i].state;
            if (menutitleanm[i].state != menutitleanm[i].prev) {
                menutitleanm[i].mode = 1;
                menutitleanm[i].prev = menutitleanm[i].state;
            }
            if (menutitleanm[i].mode == 3 && menutitleanm[i].state == 0) {
                menutitleanm[i].state = 1;
            }
            if (menutitleanm[i].mode == 3 && menutitleanm[i].state == 1) {
                menutitleanm[i].mode = 1;
            }
            if (menutitleanm[i].mode == 1) {
                menutitleanm[i].mode = 2;
                if (i == 4) {
                    SetAnimArc2((MenuArchive *)menu1p, kind, 0);
                    OSReport("menu1p 1-0 %d\n", i);
                }
                if (i == 1) {
                    (void)0;
                }
                if (i == 4) {
                    (void)0;
                }
            }
        }
    }
    for (i = 1; i < 7; i++) {
        g = menutitleguide[i];
        if (g != -1) {
            kind = 1;
            if (menutitleanm[i].mode == 2) {
                jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 4, g, -1, -1));
                if (flag != 0) {
                    for (i = 1; i < 7; i++) {
                        menutitleanm[i].mode = 3;
                    }
                }
                break;
            }
        }
    }
    for (i = 1; i < 7; i++) {
        if (menutitleguide[i] != -1) {
            (void)0;
        }
    }
    if (menutitleguide[1] != -1) {
        kind = 1;
        src = GetChild(((MenuArchive *)menu1p)->joints[kind], (count = menutitleguide[1], 0), 3, -1, -1, -1);
        if (menutitleguide[2] == -1) {
            dst = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 0, 5, -1, -1);
        } else if (menutitleguide[3] == -1) {
            dst = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 0, 0, -1, -1);
        } else {
            dst = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 0, 1, -1, -1);
        }
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
    }
    if (menutitleguide[4] != -1) {
        kind = 1;
        src = GetChild(((MenuArchive *)menu1p)->joints[kind], (count = menutitleguide[4], 0), 2, -1, -1, -1);
        if (menutitleguide[5] == -1) {
            dst = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 1, 5, -1, -1);
        } else if (menutitleguide[6] == -1) {
            dst = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 1, 0, -1, -1);
        } else {
            dst = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 1, 1, -1, -1);
        }
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
    }
    for (i = 1; i < 7; i++) {
        g = menutitleguide[i];
        if (g != -1) {
            kind = 1;
            if (i <= 3) {
                src = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 0, i + 1, -1, -1);
            } else {
                src = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 1, i - 2, -1, -1);
            }
            dst = GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 4, g, -1, -1);
            jobjCopyPos(src, dst);
            HSD_JObjDispAll(dst, 0, 7, 0);
        }
    }
}
#undef lit_4960
#define lit_4960 "menu1p 1-0 %d\n"

void menu_sDispBattleFarmModeButton(s16 a, s16 b)
{
    s32 unused0 = 0;
    s32 unused1 = 0;
    u8 anm[6] = {0, 1, 4, 1, 2, 3};
    s16 lim = 0;
    s32 endf = 0;
    s16 j = 0;
    s32 jno = 2;
    s32 i = 1;
    (void)a;

    lim = 9;
    if ((u8)menuZoidBattleModeSingleTeam == 0) {
        lim = 9;
    }
    {
        j = 0;
        while (j < 20) {
            menufarmjfl[j] = 0;
            j++;
        }
    }
    for (i = 1; i <= lim; i++) {
        jno = menufarmjno[i];
        if (jno != -1) {
            menufarmjfl[jno] = 1;
            if (menufarmanm[i] != menufarmanmold[i]) {
                menumodeanm[i] = 1;
                menufarmanmold[i] = menufarmanm[i];
            }
            if (menumodeanm[i] == 3 && menufarmanm[i] == 0) {
                menufarmanm[i] = 1;
            }
            if (menumodeanm[i] == 3 && menufarmanm[i] == 1) {
                menumodeanm[i] = 1;
            }
            if (menumodeanm[i] == 3 && menufarmanm[i] == 3) {
                menumodeanm[i] = 3;
            }
            if (menumodeanm[i] == 3 && menufarmanm[i] == 4) {
                menumodeanm[i] = 4;
            }
            if (menumodeanm[i] == 3 && menufarmanm[i] == 5) {
                menumodeanm[i] = 5;
            }
            if (menumodeanm[i] == 1) {
                menumodeanm[i] = 2;
                b = menufarmanm[i];
                sSetAnim1((MenuArchive *)menu1p, jno, GetChild(((MenuArchive *)menu1p)->joints[jno], 0, menufarmano[i], -1, -1, -1), anm[menufarmanm[i]]);
            }
        }
    }
    for (j = 0; j < 20; j++) {
        if (menufarmjfl[j] == 1) {
            jobjAnim(((MenuArchive *)menu1p)->joints[j]);
        }
    }
    for (i = 1; i <= 9; i++) {
        jno = menufarmjno[i];
        if (jno != -1) {
            if (menumodeanm[i] == 2) {
                endf = getEndF1(GetChild(((MenuArchive *)menu1p)->joints[jno], 0, menufarmano[i], -1, -1, -1));
                if (endf != 0) {
                    menumodeanm[i] = 3;
                    menuanmend = 1;
                }
            }
            if (menumodeanm[i] == 3) {
                (void)0;
            }
        }
    }
    for (j = 0; j < 20; j++) {
        if (menufarmjfl[j] == 1) {
            jobjDispAll(((MenuArchive *)menu1p)->joints[j]);
        }
    }
    (void)a;
}
f32 rPadX(void)
{
    f32 ry = menuzoidry;
    f32 absf;
    void *p;
    f32 f31 = 0.0f;
    f32 f30;
    f32 f29;

    p = *(void **)((u8 *)Zoid + 0x594);
    if (p == 0) {
        p = *(void **)((u8 *)Zoid + 0x4f8);
    }
    if (p == 0) {
        p = *(void **)((u8 *)Zoid + 0x40);
    }
    if (p != 0) {
        f31 = *(f32 *)((u8 *)p + 0x60);
    }
    f30 = sPad1->field_70;
    if (f30 > 0.35) {
        lookCamY += 0.1;
    }
    if (f30 < -0.35) {
        lookCamY = lookCamY - 0.1;
    }
    absf = f30;
    *(u32 *)&absf &= 0x7fffffff;
    if (absf < 0.2) {
        if (lookCamY > f31) {
            lookCamY = lookCamY - 0.01;
            if (lookCamY < f31) {
                lookCamY = f31;
            }
        }
        if (lookCamY < f31) {
            lookCamY += 0.01;
            if (lookCamY > f31) {
                lookCamY = f31;
            }
        }
    }
    if (lookCamY < 0.0f) {
        lookCamY = 0.0f;
    }
    if (lookCamY > 20.0f) {
        lookCamY = 20.0f;
    }
    f29 = sPad1->field_6C;
    if (f29 > 0.35) {
        ry += 0.0122173043474961;
    }
    if (f29 < -0.35) {
        ry = ry - 0.012391837103270758;
    }
    angLimit(&ry);
    if (ry > 2.7925267555555555) {
        ry = 2.79252672f;
    }
    if (ry < -2.7925267555555555) {
        ry = -2.79252672f;
    }
    if (ry == 0.0f) {
        ry = 0.00174532924f;
    }
    menuzoidry = ry;
    return ry;
}
#pragma push
#pragma dont_inline off
namespace { extern const char lit_3730[7]; extern const char lit_3731[5]; }

static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_3730, 0x25d, lit_3731);
    return !(((u32 *)jobj)[5] & 0x800000) && (((u32 *)jobj)[5] & 0x40);
}

static inline void HSD_JObjSetMtxDirty(HSD_JObj *jobj)
{
    if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
        HSD_JObjSetMtxDirtySub(jobj);
    }
}

static inline void HSD_JObjSetRotationY(HSD_JObj *jobj, f32 y)
{
    jobj ? (void)0 : __assert(lit_3730, 0x2b8, lit_3731);
    !(((u32 *)jobj)[5] & 0x20000) ? (void)0 : __assert(lit_3730, 0x2b9, "!(jobj->flags & JOBJ_USE_QUATERNION)");
    ((f32 *)jobj)[8] = y;
    if (!(((u32 *)jobj)[5] & 0x2000000)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}


void menu_sDispBattleFarmBack(void)
{
    f32 fdead;
    s32 dead;
    s32 anim = 0;
    f32 pad;
    HSD_JObj *obj;

    pad = rPadX();
    fdead = 0.0f;
    dead = 0;
    if (menubackanm[0].cur != menubackanm[0].old) {
        menubackanm[0].state = 1;
        menubackanm[0].old = menubackanm[0].cur;
    }
    if (menubackanm[0].state == 1) {
        menubackanm[0].state = 2;
        SetAnimArc2((MenuArchive *)menu_back1p, 1, menubackanm[0].cur);
        menu_StageUpDownFlag = 0;
    }
    if (menubackanm[0].state == 2) {
        anim = jobjAnim(((MenuArchive *)menu_back1p)->joints[1]);
        if (anim != 0) {
            menubackanm[0].state = 3;
            menuanmend = 1;
            menu_StageUpDownFlag = 1;
            if (menubackanm[0].cur == 2 || menubackanm[0].cur == 3) {
                if (menubackanm[0].cur == 2) {
                    menubackanm[0].cur = 0;
                }
                if (menubackanm[0].cur == 3) {
                    menubackanm[0].cur = 1;
                }
                SetAnimArc2((MenuArchive *)menu_back1p, 1, menubackanm[0].cur);
                jobjAnim(((MenuArchive *)menu_back1p)->joints[1]);
            }
        }
    }
    obj = GetChild(((MenuArchive *)menu_back1p)->joints[1], 0, 2, -1, -1, -1);
    obj = GetChild(((MenuArchive *)menu_back1p)->joints[0], 0, -1, -1, -1, -1);
    HSD_JObjSetRotationY(obj, pad);
    sMakeMatrix(obj);
    obj = GetChild(((MenuArchive *)menu_back1p)->joints[1], 0, -1, -1, -1, -1);
    HSD_JObjSetRotationY(obj, pad);
    sMakeMatrix(obj);
    jobjDispAll(((MenuArchive *)menu_back1p)->joints[0]);
    jobjDispAll(((MenuArchive *)menu_back1p)->joints[1]);
}
#pragma pop
#undef lit_5103
#define lit_5103 "!(jobj->flags & JOBJ_USE_QUATERNION)"
void eyeCamPos(void)
{
    eyeCam[0] = 0.0f;
    eyeCam[1] = 3.0f;
    eyeCam[2] = 19.5f;
    HSD_CObjSetEyePosition(((MenuArchive *)menu_back1p)->cobj, eyeCam);
}
typedef struct FarmVec {
    f32 x;
    f32 y;
    f32 z;
} FarmVec;

void menu_sDispBattleFarmZoid(void)
{
    HSD_JObj *obj = 0;
    HSD_JObj *joint = 0;
    s16 z = IveSave[0].field_15F4;
    s32 dead = 0;
    f32 rx = rPadX();
    f32 ry = 0.2 * sPad1->field_70;
    FarmVec v0 = { 0.0f, 0.0f, 0.0f };
    FarmVec v1 = { 0.0f, 0.0f, 0.0f };
    f32 f;
    s32 k;
    s32 j;

    if (z != -1 && Zoid[0].joint != 0) {
        dead = IveSave[0].field_15F4;
        f = 0.0f;
        eyeCamPos();
        v1.x = menubackintpos[0];
        v1.y = menubackintpos[1] + f;
        v1.z = menubackintpos[2];
        lookCam[0] = v1.x;
        lookCam[1] = v1.y + lookCamY;
        lookCam[2] = v1.z;
        eyeCamPos();
        HSD_CObjSetInterest(((MenuArchive *)menu_back1p)->cobj, lookCam);
        obj = GetChild(((MenuArchive *)menu_back1p)->joints[1], 0, 2, -1, -1, -1);
        joint = Zoid[0].joint;
        jobjAnim(joint);
        jobjCopyPos(obj, joint);
        for (k = 0; k < 3; k++)
            for (j = 0; j < 4; j++)
                *(f32 *)((u8 *)joint + k * 16 + j * 4 + 0x44) = *(f32 *)((u8 *)obj + k * 16 + j * 4 + 0x44);
        HSD_JObjDispAll(joint, 0, 7, 0);
        zdDispCursor();
    }
}

void menu_sDispBattleFarmWeapon(void)
{
    sWeaponAnimAll();
    sWeaponMoveAll();
    sWeaponDispAll();
}

s32 menufncZoidBattle_BattleMenu(void)
{
    s32 u0 = 0;
    s32 u1 = 0;
    s32 u2 = -240;
    s16 u3 = 0;
    s16 u4 = 0;
    s32 u5 = 0;
    s32 u6 = 0;
    f32 pos[3];
    s32 dead;
    s32 dead2;
    s32 i = 0;
    s32 a = 0;
    s32 ret = 0;
    s32 b = 0;
    GXColor color;

    menuCursor = 0;
    menuParticles = 0;
    menumode = 0;
    for (i = 0; i < 20; i++) {
        menumodeanm[i] = 0;
    }
    for (i = 0; i < 20; i++) {
        menufarmanm[i] = 2;
        menufarmanmold[i] = 2;
    }
    OSReport(lit_888);
    vsyncN = 0;
    TeaMenuBGM(3);
    pos[0] = menu_zoideyecam[0];
    pos[1] = menu_zoideyecam[1] + lookCamY;
    pos[2] = menu_zoideyecam[2];
    menuSetTitle(0);
    TEA_setvsync();
    menufarmmode = 0;
    i = 5;
    menumodeanm[0] = 0;
    menufarmanm[0] = 0;
    menufarmcl[0] = 0;
    menufarmsl[0] = 0;
    menufarmst[0] = 0;
    menufarmmh[0] = 5;
    menufarmmn[0] = 5;
    menufarmcl[1] = 0;
    menufarmsl[1] = 0;
    menufarmst[1] = 0;
    menufarmmh[1] = 2;
    menufarmmn[1] = 2;
    menufarmcl[2] = 0;
    menufarmsl[2] = 0;
    menufarmst[2] = 0;
    menufarmmh[2] = 2;
    menufarmmn[2] = 2;
    menufarmcl[3] = 0;
    menufarmsl[3] = 0;
    menufarmst[3] = 0;
    menufarmmh[3] = 2;
    menufarmmn[3] = 2;
    menufarmcl[4] = 0;
    menufarmsl[4] = 0;
    menufarmst[4] = 0;
    menufarmmh[4] = 2;
    menufarmmn[4] = 2;
    menufarmcl[i] = 0;
    menufarmsl[i] = 0;
    menufarmst[i] = 0;
    menufarmmh[i] = 3;
    menufarmmn[i] = 3;
    menubackanm[0].state = 1;
    menubackanm[0].cur = 1;
    menubackanm[0].old = 0;
    menuDetailMode = 1;
    detCustm(IveSave[0].zoid_slot);
    detSaveNoSet(0);
    menu_sDispSaveLoadMenuSet(-1);
    ttl_WeaponAllMount(IveSave[0].zoid_slot);
    eyeCamPos();
    SetMenuMsgAdr(50, menudispwinstr);
    OSReport("Menu CamPos x=%f y=%f z=%f \n", menubackcampos[0], menubackcampos[1], menubackcampos[2]);
    menuSortPartsHave();
    for (i = 1; i <= 9; i++) {
        menufarmanm[i] = 0;
    }
    memcpy(&IveSave[4], &IveSave[0], 0x1a90);
    for (i = 0; i <= 7; i++) {
        menumodeanm[i] = 1;
        menufarmanm[i] = 0;
    }
    flag_AllHavePartsListanm[1] = 0;
    menu_sDispSetButton();
    menuSetTitle(0);
    menu_TITLE_disp(1);
    while (1) {
        sPadAll();
        if (menumodeanm[0] == 0) {
            sPad1->pressed_a = 0;
            sPad1->field_5E = 0;
            sPad1->field_63 = 0;
            sPad1->field_60 = 0;
            sPad1->field_62 = 0;
            sPad1->field_5B = 0;
            sPad1->field_5C = 0;
            sPad1->field_57 = 0;
            sPad1->field_58 = 0;
            sPad1->field_59 = 0;
            sPad1->field_5A = 0;
        }
        menu_sDispSetButton();
        menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        ttl_batfarm_msgsetparam();
        menu_sDispBattleFarm();
        if (sPad1->field_5C) {
            flag_AllHavePartsList++;
            continue;
        }
        if (sPad1->field_61) {
            if (menumodeanm[0] == 0)
                menumodeanm[0] = 1;
            else
                menumodeanm[0] = 0;
            continue;
        }
        if (sPad1->field_62) {
            menufarmanm[0] = 2;
            detCustm(IveSave[0].zoid_slot);
            continue;
        }
        if (sPad1->field_5E) {
            if (menumode > 0) {
                menumode--;
            } else {
                SeCancel();
                continue;
            }
            SeCancel();
            continue;
        }
        if (sPad1->pressed_a || sPad1->pressed_a) {
            switch (menumode) {
            case 0:
                if (menufarmcl[0] == 4) {
                    SeRet();
                    menudispyesnopoint = 1;
                    menudispyesnoflag = 1;
                    do {
                        sPadAll();
                        menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(72), lit_574);
                        menu_sDispBattleFarm();
                    } while (!sPad1->pressed_a);
                    SeRet();
                    menudispyesnoflag = 0;
                    if (menudispyesnopoint == 0)
                        continue;
                    sCamNN2 = 2;
                    if ((u8)menuZoidBattleModeSingleTeam == 1)
                        sCamNN2 = 2;
                    if ((u8)menuZoidBattleModeSingleTeam == 0) {
                        IveSave[0].field_1602 = IveSave[0].field_15FA = -1;
                    }
                    menuZoidBattleGo();
                    menu_TITLE_disp(0);
                    TeaBgm(-1);
                    HSD_VISetBlack(1);
                    for (i = 0; i < 1; i++) {
                        sPadAll();
                        VIWaitForRetrace();
                        HSD_VICopyXFB(0);
                        HSD_VISetXFBDrawDone();
                    }
                    ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                    ttlMenuAllPurge();
                    menuretno = 9999;
                    dspLoadF = 1;
                    return 210;
                } else if (menufarmcl[0] == 0) {
                    SeRet();
                    menu_TITLE_disp(0);
                    menuSetTitle(menufarmcl[0] + 2);
                    menufncZoidBattle_BattleMenuSet();
                    menuSetTitle(0);
                    menu_TITLE_disp(1);
                    detCustm(IveSave[0].zoid_slot);
                } else if (menufarmcl[0] == 1) {
                    SeRet();
                    menu_TITLE_disp(0);
                    menuSetTitle(menufarmcl[0] + 2);
                    menufncZoidBattle_BattleMenuShop();
                    menuSetTitle(0);
                    menu_TITLE_disp(1);
                    detCustm(IveSave[0].zoid_slot);
                } else if (menufarmcl[0] == 2) {
                    SeRet();
                    menu_TITLE_disp(0);
                    menuSetTitle(menufarmcl[0] + 2);
                    menuSetTitleButton(10);
                    menufncZoidBattle_MapInfo();
                    menuSetTitle(0);
                    menu_TITLE_disp(1);
                } else {
                    SeRet();
                    menu_TITLE_disp(0);
                    menuSetTitle(menufarmcl[0] + 2);
                    menu_TITLE_disp(1);
                    menufarmanm[5] = 5;
                    menumodeanm[5] = 1;
                    menu_sDispSaveLoadMenuSet(-3);
                    while (menudispslwindow[0].anim != 2)
                        menu_sDispBattleFarm();
                    menumode = 5;
                    menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                    menuSetTitleButton(11);
                    menu_sDispBattleFarm();
                    while (1) {
                        menuDetailMode = 0;
                        sPadAll();
                        menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
                        ttl_batfarm_msgsetparam();
                        menu_sDispBattleFarm();
                        if (sPad1->field_5E) {
                            SeCancel();
                            break;
                        }
                        if (sPad1->pressed_a) {
                            SeRet();
                            if (menufarmcl[menumode] == 0) {
                                menu_SlotOn[0] = TEA_mcChkSlotNoMsg(0);
                                if (menu_SlotOn[0] == 0) {
                                    TEA_mcMsg(0, 0x47c);
                                    continue;
                                }
                                memcpy(&IveSaveFlagBak, &IveSaveFlag, 0x1a90);
                                if (ttlMemCardChkStartNew() == 0) {
                                    memcpy(&IveSaveFlag, &IveSaveFlagBak, 0x1a90);
                                    continue;
                                }
                                IveSave[0].field_1C87 = 1;
                                ret = ttlMenuBatSaveLoad(1);
                                memcpy(&IveSaveFlag, &IveSaveFlagBak, 0x1a90);
                                menuSetTitle(menufarmcl[0] + 2);
                                menu_sDispSetButton();
                                menu_sDispSaveLoadMenuSet(-1);
                                menu_sDispBattleFarm();
                                menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                                menu_sDispBattleFarm();
                            } else if (menufarmcl[menumode] == 1) {
                                memcpy(&IveSaveFlagBak, &IveSaveFlag, 0x1a90);
                                menu_SlotOn[0] = TEA_mcChkSlotNoMsg(0);
                                if (menu_SlotOn[0] == 0) {
                                    TEA_mcMsg(0, 0x47c);
                                    continue;
                                }
                                if (ttlMemCardChkStartNew() == 0) {
                                    memcpy(&IveSaveFlag, &IveSaveFlagBak, 0x1a90);
                                    continue;
                                }
                                IveSave[0].field_1C87 = 1;
                                ret = ttlMenuBatSaveLoad(0);
                                if (ret == 1) {
                                    ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                                    if ((u8)menuZoidBattleModeSingleTeam == 1)
                                        ttl_WeaponAllUnMount(IveSave[0].partner_slot);
                                    ZoidPurge(&Zoid[0]);
                                    memcpy(&IveSave[0], &IveSave[2], 0x1a90);
                                    menuZoidBattle_SaveData_AddData();
                                    if (IveSave[0].field_1C87 == 0) {
                                        TeaBgm(-1);
                                        ttlMenuAllPurge();
                                        return 200;
                                    }
                                    dspLoadF = 1;
                                    TeaMenuBGM(3);
                                    ZoidInitC(&Zoid[0], IveSave[0].field_15F4, IveSave[0].zoid_color[IveSave[0].zoid_slot]);
                                    ZoidSetAnim(&Zoid[0], 103);
                                    jobjAnim(Zoid[0].joint);
                                    ttl_WeaponAllMount(IveSave[0].zoid_slot);
                                    menuZoidBattleModeSingleTeam = IveSave[0].single_team;
                                    if ((u8)menuZoidBattleModeSingleTeam == 0) {
                                        IveSave[0].partner_slot = -1;
                                        IveSave[0].field_15FA = -1;
                                    } else {
                                        IveSave[0].field_15FA = IveSave[0].owned_zoids[IveSave[0].partner_slot];
                                    }
                                    menuSortPartsHave();
                                    GameMode.map = IveSave[0].stage;
                                    menuZoidBattleStage = IveSave[0].stage;
                                    GameMode.field_28 = IveSave[0].map_no;
                                    GameMode.game_type = IveSave[0].field_1C8C = 1;
                                    menuZoidBattleModeSingleTeam = IveSave[0].single_team;
                                    GameMode.field_24 = IveSave[0].field_1C89 % 3;
                                    rdMapInfo(0, GameMode.field_24 + GameMode.map * 3);
                                    detCustm(IveSave[0].zoid_slot);
                                } else {
                                    memcpy(&IveSaveFlag, &IveSaveFlagBak, 0x1a90);
                                }
                                menuSetTitle(menufarmcl[0] + 2);
                                menu_sDispSetButton();
                                menu_sDispSaveLoadMenuSet(-1);
                                ttl_batfarm_msgsetparam();
                                menu_sDispBattleFarm();
                                menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                                menu_sDispBattleFarm();
                            } else if (menufarmcl[menumode] == 2) {
                                menudispyesnopoint = 1;
                                menudispyesnoflag = 1;
                                do {
                                    sPadAll();
                                    menudispyesnopoint = menuYesNo(menudispyesnopoint, lit_574, GetMenuMsgAdr(100));
                                    menu_sDispBattleFarm();
                                } while (!sPad1->pressed_a);
                                SeRet();
                                menudispyesnoflag = 0;
                                if (menudispyesnopoint != 0) {
                                    for (i = 0; i < 1; i++) {
                                        GXColor zero = { 0, 0, 0, 0 };
                                        s32 dead3;
                                        s32 dead4;

                                        color = zero;
                                        HSD_VIData.bg_color = color;
                                        VIWaitForRetrace();
                                        HSD_VICopyXFB(0);
                                        HSD_VISetXFBDrawDone();
                                    }
                                    ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                                    ttlMenuAllPurge();
                                    OSReport("MenuEnd \n");
                                    strcpy(menudispwinstr, lit_574);
                                    menudispyesnoflag = 0;
                                    strcpy(menudispyesnostr1, lit_574);
                                    strcpy(menudispyesnostr2, lit_574);
                                    GameMode.field_38 = 1;
                                    dspLoadF = 1;
                                    resetSkipOp();
                                } else {
                                    menuSetTitle(menufarmcl[0] + 2);
                                    menu_sDispSetButton();
                                    menu_sDispSaveLoadMenuSet(-1);
                                    menu_sDispBattleFarm();
                                    menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                                    menu_sDispBattleFarm();
                                }
                            }
                            IveSave[0].field_1C87 = 0;
                        }
                        if (sPad1->field_4F || sPad1->field_53)
                            menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                        if (sPad1->field_50 || sPad1->field_54)
                            menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                    }
                    menumode = 0;
                    menu_sDispSaveLoadMenuSet(-2);
                    menu_TITLE_disp(0);
                    menuSetTitle(0);
                    menu_TITLE_disp(1);
                }
                break;
            case 1:
                if (menufarmcl[1] == 0) {
                    SeRet();
                    menuanmend = 0;
                    OSReport("MenuZoidBattle Entry On\n");
                    menumode = -1;
                    menuHiddenTitle();
                    menu_TITLE_disp(0);
                    OSReport("MenuZoidBattle Entry Ok\n");
                    menufarmmode = 1;
                    ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                    return 205;
                }
                if ((u8)menuZoidBattleModeSingleTeam == 1) {
                    SeLiftStart();
                    menu_sDispBattleFarmStageDown(1);
                    SeLiftStop();
                    ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                    ZoidWeaponPurge(&Zoid[0]);
                    ZoidWeaponPurge(&Zoid[0]);
                    ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                    ZoidPurge(&Zoid[0]);
                    memcpy(&IveSave[2], &IveSave[0], 0x1a90);
                    IveSave[0].field_1600 = IveSave[0].field_1602;
                    IveSave[0].field_1602 = IveSave[2].field_1600;
                    IveSave[0].field_15F4 = IveSave[0].field_15FA;
                    IveSave[0].field_15FA = IveSave[2].field_15F4;
                    IveSave[0].zoid_slot = IveSave[0].partner_slot;
                    IveSave[0].partner_slot = IveSave[2].zoid_slot;
                    detCustm(IveSave[0].zoid_slot);
                    ZoidInitC(&Zoid[0], IveSave[0].field_15F4, IveSave[0].zoid_color[IveSave[0].zoid_slot]);
                    ZoidSetAnim(&Zoid[0], 103);
                    ttl_WeaponAllMount(IveSave[0].zoid_slot);
                    SeLiftStart();
                    menu_sDispBattleFarmStageDown(0);
                    SeLiftStop();
                } else {
                    SeRet();
                }
                break;
            case 2:
                if (menufarmcl[1] == 0)
                    menumode = 3;
                else
                    menumode = 4;
                TEA_HotReset(0);
                break;
            }
        }
        if (sPad1->field_4F) {
            (void)0;
        }
        if (sPad1->field_4F) {}
        if (sPad1->field_50 || sPad1->field_54) {
            (void)0;
        }
        if ((sPad1->field_51 || sPad1->field_55) && menumode == 0) {
            OSReport("MenuZoidBattle Entry\n");
            menumode = 1;
            menufarmcl[menumode] = 0;
            menufarmsl[menumode] = 0;
            menufarmst[menumode] = 0;
        }
        if ((sPad1->field_52 || sPad1->field_56) && menumode != 0) {
            OSReport("MenuZoidBattle Farm\n");
            if (menumode > 1) {
                menuerasewait(0);
                menuSetTitle(0);
            }
            menumode = 0;
            menufarmmode = 0;
        }
        if (sPad1->field_63) {}
        if (sPad1->field_63) {}
        if (sPad1->field_63)
            puts("PUSH START");
        a = a;
        b = b;
    }
}
#undef lit_5330
#define lit_5330 "Menu CamPos x=%f y=%f z=%f \n"
#undef lit_5331
#define lit_5331 "MenuEnd \n"
#undef lit_5332
#define lit_5332 "MenuZoidBattle Entry On\n"
#undef lit_5333
#define lit_5333 "MenuZoidBattle Entry Ok\n"
#undef lit_5334
#define lit_5334 "MenuZoidBattle Entry\n"
#undef lit_5335
#define lit_5335 "MenuZoidBattle Farm\n"
#undef lit_5336
#define lit_5336 "PUSH START"

void menu_sDispBattleFarmStageDown(s32 dir)
{
    menuanmend = 0;
    if (dir == 1)
        menubackanm[0].cur = 2;
    else
        menubackanm[0].cur = 3;
    while (1) {
        menu_sDispBattleFarm();
        if (dir == 1) {
            if (menubackanm[0].cur == 0)
                break;
        } else if (menubackanm[0].cur == 1) {
            break;
        }
    }
    if (dir == 1)
        menubackanm[0].cur = 0;
    else
        menubackanm[0].cur = 1;
    menuanmend = 0;
}

s32 menu_sDispBattleStageDown(s32 mode)
{
    if (mode == 1)
        menubackanm[0].cur = 2;
    else
        menubackanm[0].cur = 3;
    return menu_StageUpDownFlag;
}

void menuZoidBattleSetStage_fromnowstage(void)
{
    menuZoidBattleSetStage_PersonSelect(0);
}

void menuZoidBattleSetStage_PersonSelect(s32 mode)
{
    s16 person = 0;
    s16 group = 0;
    s16 map = 0;

    if ((s8)IveSave[0].next_stage >= 12)
        IveSave[0].next_stage = 0;
    group = (s8)IveSave[0].next_stage * 3;
    if (mode != 0) {
        person = *(s16 *)((u8 *)&Tea_persondat[IveSave[0].field_1600] + 0x2E);
    } else {
        person = (s8)IveSave[0].field_1C89 % 3;
    }
    if (person < 0)
        person = 0;
    if (person > 2)
        person = 0;
    map = person + group;
    rdMapInfo(0, map);
    IveSave[0].field_1C89 = group + person;
    GameZoid[5].field_58 = enemyChara[0];
    GameZoid[6].field_58 = enemyChara[1];
    GameMode.field_28 = menu_BatMapData[group + person].map;
    IveSave[0].map_no = menu_BatMapData[group + person].map;
    GameMode.map = (s8)IveSave[0].next_stage;
    GameMode.field_24 = person;
}

void menuZoidBattleGo(void)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 i = 0;
    s16 zoid = 0;
    s16 k;
    s32 dead1 = 0;
    s32 dead2 = 0;

    for (i = 0; i < 10; i++) {
        GameZoid[i].type = -1;
    }
    GameZoid[0].type = IveSave[0].field_15F4;
    GameZoid[0].field_58 = IveSave[0].field_1600;
    zoid = IveSave[0].zoid_slot;
    menuZoidBattleGoPartsOptSet_GameZoid(0, 0, zoid, 0);
    if ((u8)menuZoidBattleModeSingleTeam == 1) {
        menuZoidBattleGoPartsOptSet_GameZoid(1, 0, IveSave[0].partner_slot, 1);
    } else {
        GameZoid[1].type = -1;
        GameZoid[1].field_58 = -1;
        GameZoid[1].color = IveSave[0].zoid_color[zoid];
        for (i = 0; i < 10; i++) {
            GameZoid[1].mounts[i] = -1;
            GameZoid[1].mount_colors[i] = 0;
        }
        GameZoid[1].field_5E = -1;
        for (i = 0; i < 3; i++) {
            GameZoid[1].field_60[i] = -1;
        }
    }
    dead1 = 0;
    dead2 = 0;
    for (k = 0; k < 100; k++) {
        i = (OSGetTime() / ((__OSBusClock / 4) / 1000)) % IveSave[0].person_count;
        zoid = IveSave[0].persons[i];
        if (IveSave[0].field_1600 != zoid) {
            if (IveSave[0].field_1602 != zoid) {
                break;
            }
        }
    }
    menuZoidBattleSetStage_fromnowstage();
}

s16 TEA_menufnc(s16 *a, s16 *b, s16 c, s16 d)
{
    s16 tmp = 0;

    if (*b == -1) {
        tmp = *a;
        *b = *a;
        *a = 0;
        if (*a + *b > c - d) {
            *a = *b;
            *b = c - d;
        } else {
            *a = tmp;
            *b = 0;
        }
        if (*a + 1 > d) {
            if (*a + d >= c) {
                *b = c - d;
                *a = tmp - *b;
            } else {
                *b = tmp;
                *a = 0;
            }
        } else {
            *b = 0;
        }
        OSReport("menupSet=%d maxh=%d menust=%d maxn=%d\n", *a, d, *b, c);
    }
    if (sPad1->field_57 != 0) {
        SeSelectB();
        if (*a == 0) {
            if (*b > 0) {
                *b = *b - 1;
            }
            OSReport("Menu UP menup=%d max=%d menust=%d maxn=%d\n", *a, d, *b, c);
            return *b;
        }
        *a = *a - 1;
        OSReport("Menu UP menup=%d max=%d menust=%d maxn=%d\n", *a, d, *b, c);
        return *b + *a;
    }
    if (sPad1->field_58 != 0) {
        SeSelectB();
        if (*a == d - 1) {
            if (*a + *b >= c - 1) {
                return *b + *a;
            }
            *b = *b + 1;
            OSReport("Menu Down Lst menup=%d max=%d menust=%d maxn=%d\n", *a, d, *b, c);
            return *b + *a;
        }
        *a = *a + 1;
        OSReport("Menu Down menup=%d max=%d menust=%d maxn=%d\n", *a, d, *b, c);
        return *b + *a;
    }
    if (sPad1->field_5A != 0) {
        SeSelectB();
    }
    if (sPad1->field_59 != 0) {
        SeSelectB();
    }
    return *b + *a;
}
#undef lit_5461
#define lit_5461 "menupSet=%d maxh=%d menust=%d maxn=%d\n"
#undef lit_5462
#define lit_5462 "Menu UP menup=%d max=%d menust=%d maxn=%d\n"
#undef lit_5463
#define lit_5463 "Menu Down Lst menup=%d max=%d menust=%d maxn=%d\n"
#undef lit_5464
#define lit_5464 "Menu Down menup=%d max=%d menust=%d maxn=%d\n"
s16 TEA_menufnc2p(s32 a, s32 b, s16 c, s16 d)
{
    s16 sv = 0;

    if ((*(s16 *)b) == -1) {
        sv = (*(s16 *)a);
        (*(s16 *)b) = (*(s16 *)a);
        (*(s16 *)a) = 0;
        if ((*(s16 *)a) + (*(s16 *)b) > c - d) {
            (*(s16 *)a) = (*(s16 *)b);
            (*(s16 *)b) = c - d;
        } else {
            (*(s16 *)a) = sv;
            (*(s16 *)b) = 0;
        }
        if ((*(s16 *)a) + 1 > d) {
            if ((*(s16 *)a) + d >= c) {
                (*(s16 *)b) = c - d;
                (*(s16 *)a) = sv - (*(s16 *)b);
            } else {
                (*(s16 *)b) = sv;
                (*(s16 *)a) = 0;
            }
        } else {
            (*(s16 *)b) = 0;
        }
        OSReport("menup2Set=%d maxh=%d menust=%d maxn=%d\n", (*(s16 *)a), d, (*(s16 *)b), c);
    }
    if (sPad2->field_57) {
        SeSelectB();
        if ((*(s16 *)a) == 0) {
            if ((*(s16 *)b) > 0) {
                (*(s16 *)b) = (*(s16 *)b) - 1;
            }
            OSReport("Menu2 UP menup=%d max=%d menust=%d maxn=%d\n", (*(s16 *)a), d, (*(s16 *)b), c);
            return (*(s16 *)b);
        }
        (*(s16 *)a) = (*(s16 *)a) - 1;
        OSReport("Menu2 UP menup=%d max=%d menust=%d maxn=%d\n", (*(s16 *)a), d, (*(s16 *)b), c);
        return (*(s16 *)b) + (*(s16 *)a);
    } else if (sPad2->field_58) {
        SeSelectB();
        if ((*(s16 *)a) == d - 1) {
            if ((*(s16 *)a) + (*(s16 *)b) >= c - 1) {
                return (*(s16 *)b) + (*(s16 *)a);
            }
            (*(s16 *)b) = (*(s16 *)b) + 1;
            OSReport("Menu Down Last menup=%d max=%d\n", (*(s16 *)a), d);
            return (*(s16 *)b) + (*(s16 *)a);
        }
        (*(s16 *)a) = (*(s16 *)a) + 1;
        OSReport("Menu Down ok menup=%d max=%d\n", (*(s16 *)a), d);
        return (*(s16 *)b) + (*(s16 *)a);
    } else if (sPad2->field_5A) {
        SeSelectB();
    }
    if (sPad2->field_59) {
        SeSelectB();
    }
    return (*(s16 *)b) + (*(s16 *)a);
}
#undef lit_5499
#define lit_5499 "menup2Set=%d maxh=%d menust=%d maxn=%d\n"
#undef lit_5500
#define lit_5500 "Menu2 UP menup=%d max=%d menust=%d maxn=%d\n"
#undef lit_5501
#define lit_5501 "Menu Down Last menup=%d max=%d\n"
#undef lit_5502
#define lit_5502 "Menu Down ok menup=%d max=%d\n"

s32 TEA_menufnc12p(s16 *a, s16 *b, s16 c, s16 d)
{
    s32 dead;

    dead = 0;
    (void)a;
    (void)a;
    if (*b == -1) {
        *b = (dead = *b, *a);
        *a = 0;
        if (*a + *b > c - d) {
            *b = c - d;
            *a = d - 1;
        } else {
            *a = 0;
        }
    }
    if (sPad1->field_57 != 0 || sPad2->field_57 != 0) {
        SeSelectB();
        if (*a == 0) {
            if (*b > 0) {
                *b = *b - 1;
                OSReport("menus=%d:menup=%d menu=%d \n", *b, *a, *b);
                return *b;
            }
            *b = c - d;
            *a = d - 1;
            OSReport("menus=%d:menup=%d menu=%d \n", *b, *a, (s16)(*b + *a));
            return *b + *a;
        }
        *a = *a - 1;
        OSReport("menus=%d:menup=%d menu=%d \n", *b, *a, (s16)(*b + *a));
        return *b + *a;
    }
    if (sPad1->field_58 != 0 || sPad2->field_58 != 0) {
        SeSelectB();
        if (*a == d - 1) {
            if (*a + *b >= c - 1) {
                *a = 0;
                *b = 0;
                OSReport("menus=%d:menup=%d menu=%d \n", *b, *a, (s16)(*b + *a));
                return *b + *a;
            }
            *b = *b + 1;
            OSReport("menus=%d:menup=%d menu=%d \n", *b, *a, (s16)(*b + *a));
            return *b + *a;
        }
        *a = *a + 1;
        OSReport("menus=%d:menup=%d menu=%d \n", *b, *a, (s16)(*b + *a));
        return *b + *a;
    }
    if (sPad1->pressed_a) {
        (void)0;
    }
    if (sPad1->field_5E) {
        (void)0;
    }
    if (sPad2->field_5A != 0) {
        SeSelectB();
    }
    if (sPad2->field_59 != 0) {
        SeSelectB();
    }
    return *b + *a;
}
#undef lit_5537
#define lit_5537 "menus=%d:menup=%d menu=%d \n"

void menuerasewait(s16 index)
{
    menuanmend = 0;
    menufarmanm[index] = 2;
    while (menuanmend == 0)
        menu_sDispBattleFarm();
    menuanmend = 0;
    index = 0;
}

void menu_TITLE_disp(s32 mode)
{
    s16 i = 0;

    i = 1;
    menuanmend = 0;
    menutitleanm[0].mode = 1;
    for (; i < 7; i++) {
        menutitleanm[i].mode = 1;
    }
    if (mode == 1) {
        menutitleanm[0].state = 0;
        i = 1;
        for (; i < 7; i++) {
            menutitleanm[i].state = 0;
        }
    } else {
        menutitleanm[0].state = 2;
        i = 1;
        for (; i < 7; i++) {
            menutitleanm[i].state = 2;
        }
    }
    while (menuanmend == 0) {
        menu_sDispSetButton();
        menu_sDispBattleFarm();
        i++;
    }
    menuanmend = 0;
}
namespace { extern const char lit_5592[7]; }

void menuSetTitle(s16 title)
{
    s16 i = 0;
    register s32 dead;
    dead = 0;

    menutitleanm[0].mode = 1;
    menutitleanm[0].state = 0;
    menutitleanm[0].prev = 2;
    menutitleanm[0].reserved = 0;
    for (i = 1; i < 7; i++) {
        menutitleanm[i].mode = 3;
        menutitleanm[i].state = 2;
        menutitleanm[i].prev = 2;
        menutitleanm[i].reserved = 1;
    }
    menutitleguide[0] = title;
    if (title == 0) {
        menutitleguide[1] = 0;
        menutitleguide[2] = 1;
        menutitleguide[3] = -1;
        menutitleguide[4] = 2;
        menutitleguide[5] = 5;
        menutitleguide[6] = 6;
    }
    if (title == 1) {
        menutitleguide[1] = -1;
        menutitleguide[2] = -1;
        menutitleguide[3] = -1;
        menutitleguide[4] = 0;
        menutitleguide[5] = 1;
        menutitleguide[6] = 2;
    }
    if (title == 2) {
        menutitleguide[1] = 0;
        menutitleguide[2] = 1;
        menutitleguide[3] = 2;
        menutitleguide[4] = 3;
        menutitleguide[5] = 5;
        menutitleguide[6] = 9;
    }
    if (title == 3) {
        menutitleguide[1] = 0;
        menutitleguide[2] = 1;
        menutitleguide[3] = -1;
        menutitleguide[4] = 2;
        menutitleguide[5] = 5;
        menutitleguide[6] = -1;
    }
    if (title == 101) {
        menutitleguide[1] = -1;
        menutitleguide[2] = -1;
        menutitleguide[3] = -1;
        menutitleguide[4] = 0;
        menutitleguide[5] = 1;
        menutitleguide[6] = 2;
    }
    if (title == 101) {
        title = 0;
        menutitleguide[0] = title;
    }
    for (i = 1; i < 7; i++) {
        if (menutitleguide[i] == -1) {
            menutitleanm[i].mode = 0;
            menutitleanm[i].state = 2;
            menutitleanm[i].prev = 2;
        } else {
            menutitleanm[i].mode = 1;
            menutitleanm[i].state = 0;
            menutitleanm[i].prev = 2;
        }
        OSReport(lit_5592, menutitleanm[i].mode, menutitleanm[i].state);
    }
    OSReport(lit_2030);
}
namespace { extern const char lit_5592[7]; }

void menuSetTitleButton(s16 button)
{
    s16 i = 0;
    s16 j = 0;

    for (i = 1; i < 7; i++) {
        menutitleanm[i].mode = 3;
        menutitleanm[i].state = 2;
        menutitleanm[i].prev = 2;
        menutitleanm[i].reserved = 1;
        j = j;
    }
    if (button == 0) {
        menutitleguide[1] = 0;
        menutitleguide[2] = 1;
        menutitleguide[3] = -1;
        menutitleguide[4] = 2;
        menutitleguide[5] = 5;
        menutitleguide[4] = -1;
        menutitleguide[5] = -1;
        menutitleguide[6] = -1;
    }
    if (button == 1) {
        menutitleguide[1] = 2;
        menutitleguide[2] = 5;
        menutitleguide[3] = -1;
        menutitleguide[4] = 0;
        menutitleguide[5] = 1;
        menutitleguide[6] = -1;
    }
    if (button == 2) {
        menutitleguide[1] = 0;
        menutitleguide[2] = 1;
        menutitleguide[3] = 2;
        menutitleguide[4] = 3;
        menutitleguide[5] = 4;
        menutitleguide[6] = 5;
    }
    if (button == 3) {
        menutitleguide[1] = 0;
        menutitleguide[2] = 1;
        menutitleguide[3] = -1;
        menutitleguide[4] = 4;
        menutitleguide[5] = 5;
        menutitleguide[6] = -1;
    }
    if (button == 10) {
        menutitleguide[1] = -1;
        menutitleguide[2] = -1;
        menutitleguide[3] = -1;
        menutitleguide[4] = 1;
        menutitleguide[5] = -1;
        menutitleguide[6] = -1;
    }
    if (button == 11) {
        menutitleguide[1] = -1;
        menutitleguide[2] = -1;
        menutitleguide[3] = -1;
        menutitleguide[4] = 0;
        menutitleguide[5] = 1;
        menutitleguide[6] = -1;
    }
    if (button == 12) {
        menutitleguide[1] = -1;
        menutitleguide[2] = -1;
        menutitleguide[3] = -1;
        menutitleguide[4] = 0;
        menutitleguide[5] = -1;
        menutitleguide[6] = -1;
    }
    if (button == 13) {
        menutitleguide[1] = -1;
        menutitleguide[2] = -1;
        menutitleguide[3] = -1;
        menutitleguide[4] = -1;
        menutitleguide[5] = -1;
        menutitleguide[6] = -1;
    }
    if (button == 101) {
        menutitleguide[1] = -1;
        menutitleguide[2] = -1;
        menutitleguide[3] = -1;
        menutitleguide[4] = 0;
        menutitleguide[5] = 1;
        menutitleguide[6] = 2;
    }
    for (i = 1; i < 7; i++) {
        if (menutitleguide[i] == -1) {
            menutitleanm[i].mode = 0;
            menutitleanm[i].state = 2;
            menutitleanm[i].prev = 2;
        } else {
            menutitleanm[i].mode = 1;
            menutitleanm[i].state = 0;
            menutitleanm[i].prev = 2;
        }
        OSReport(lit_5592, menutitleanm[i].mode, menutitleanm[i].state);
    }
    OSReport(lit_2030);
}

void menuHiddenTitle(void)
{
    s16 i = 0;
    s16 dead = 0;

    i = 1;
    menutitleanm[0].state = 2;
    for (; i < 7; i++) {
        if (menutitleguide[i] != -1)
            menutitleanm[i].state = 2;
        dead = 1;
    }
}

void ttl_batfarm_msgsetparam(void)
{
    if (menumode == 1) {
        if (menufarmcl[1] == 0)
            SetMenuMsgAdr(0x31, menudispwinstr);
        else
            SetMenuMsgAdr(0x36, menudispwinstr);
    }
    if (menumode == 0) {
        if (menufarmcl[0] == 4)
            SetMenuMsgAdr(0x37, menudispwinstr);
        else
            SetMenuMsgAdr(menufarmcl[0] + 0x32, menudispwinstr);
    }
    if (menumode == 5)
        SetMenuMsgAdr(menufarmcl[5] + 0x6E, menudispwinstr);
}

void flPicturePrologue(void)
{
    if (flPicture != 0)
        PurgeArc2(flPicture);
    flPicture = LoadArc2("weapon/picture/w_pic.dat");
}
#undef lit_5650
#define lit_5650 "weapon/picture/w_pic.dat"

void flPictureEpilogue(void)
{
    if (flPicture != 0)
        PurgeArc2(flPicture);
    flPicture = 0;
    flPictureT = 0;
    flPictureF[0] = 0;
}

s32 flPictureDisp(char *name)
{
    HSD_JObj *joint;

    if (flPicture == 0)
        return 0;
    joint = ((MenuArchive *)flPicture)->joints[0];
    if (joint == 0)
        return 0;
    if (name[0] == 0)
        return 0;
    if (flPictureT != 0) {
        if (strcmp(name, flPictureF) == 0) {
            jobjDispAll(joint);
            return 1;
        }
    } else if (strcmp(name, flPictureF) == 0) {
        return 0;
    }
    strcpy(flPictureF, name);
    flPictureT = jobjColor(joint, name, 0);
    if (flPictureT == 0)
        return 0;
    jobjDispAll(joint);
    return 1;
}

void flPictureDispParts(s32 part)
{
    char text[260];

    if ((sPad1->field_47 == 0 || menudispyesnoflag != 0) && part >= 0 && menuDetailMode >= 3 && menuDetailMode <= 5) {
        sprintf(text, "weapon/picture/%03d.tpl", part);
        flPictureDisp(text);
    }
}
#undef lit_5682
#define lit_5682 "weapon/picture/%03d.tpl"

void menu_sDispBattleShop(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    s32 e = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if (menu_back1p == 0)
        putErr("out of memory. menu back shop");
    if (menu1p == 0)
        putErr("out of memory. menu 1 shop");
    if (menu_back1p != 0 && HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(0);
        menu_sDispBattleFarmBack();
        menu_sDispBattleFarmZoid();
        menu_sDispBattleFarmWeapon();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if (menu1p != 0 && HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        menu_sDispBattleFarmTitle();
        menu_sDispBattleFarmTitleSubButton();
        menu_sDispBattleShopHaveListDsip();
        menu_sDispBattleShopLButtonAnimZoidWepOptSub();
        menu_sDispBattleShopunfold();
        menu_sDispBattleShopLButton();
        menu_sDispBattleShopLButtonZoidWepOptSub();
        menu_sDispBattleShopHaveBp();
        menu_sDispBattleShopZoidHaveZoidList();
        menu_sDispBattleShopHavePartsList();
        menu_sDispMsgWin(3, 1);
        menu_sDispBattleMsgArea();
        flPictureDispParts(detPartsn);
        menu_sDispDetailAll();
        menu_sDispYesNoMenu();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
}
#undef lit_5694
#define lit_5694 "out of memory. menu back shop"
#undef lit_5695
#define lit_5695 "out of memory. menu 1 shop"

void menu_sDispBattleShopHaveListDsip(void)
{
    s32 button = 8;
    s32 k = 0x15;
    s32 anim = 0;
    s32 dead = 0;

    if (menusetbtanm[button].cur != menusetbtanm[button].old) {
        menusetbtanm[button].state = 1;
        menusetbtanm[button].old = menusetbtanm[button].cur;
    }
    if (menusetbtanm[button].state == 1) {
        menusetbtanm[button].state = 2;
        SetAnimArc2((MenuArchive *)menu1p, k, menusetbtanm[button].cur);
    }
    if (menusetbtanm[button].state == 2) {
        anim = jobjAnim(((MenuArchive *)menu1p)->joints[k]);
        sMakeMatrix(((MenuArchive *)menu1p)->joints[k]);
        if (anim != 0) {
            menusetbtanm[button].state = 3;
            menuanmend = 1;
            switch (menusetbtanm[button].cur) {
            case 0:
                menusetbtanm[button].cur = 0;
                menusetbtanm[button].state = 2;
                break;
            case 1:
                menusetbtanm[button].cur = 2;
                menusetbtanm[button].state = 2;
                break;
            case 2:
                menusetbtanm[button].cur = 2;
                menusetbtanm[button].state = 2;
                break;
            case 3:
                menusetbtanm[button].cur = 0;
                menusetbtanm[button].state = 2;
                break;
            case 4:
                menusetbtanm[button].cur = 5;
                menusetbtanm[button].state = 2;
                break;
            case 5:
                menusetbtanm[button].cur = 5;
                menusetbtanm[button].state = 2;
                break;
            case 6:
                menusetbtanm[button].cur = 0;
                menusetbtanm[button].state = 2;
                break;
            case 7:
                menusetbtanm[button].cur = 8;
                menusetbtanm[button].state = 2;
                break;
            case 8:
                menusetbtanm[button].cur = 8;
                menusetbtanm[button].state = 2;
                break;
            case 9:
                menusetbtanm[button].cur = 0;
                menusetbtanm[button].state = 2;
                break;
            default:
                menusetbtanm[button].state = 2;
                break;
            }
            SetAnimArc2((MenuArchive *)menu1p, k, menusetbtanm[button].cur);
            jobjAnim(((MenuArchive *)menu1p)->joints[k]);
            sMakeMatrix(((MenuArchive *)menu1p)->joints[k]);
        }
    }
    HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[k], 0, -1, -1, -1, -1), 0, 7, 0);
    dead = 0;
}
#pragma force_active on
namespace { __declspec(section ".sdata2") extern const char lit_5592[7] = "%d:%d "; }
#pragma force_active off

void menu_sDispBattleShopHavePartsList(void)
{
    s32 unused0 = 4;
    s32 unused1 = 1;
    s32 unused2 = 0;
    s32 unused3 = 0;
    s32 dB = 10;
    s32 dA = 7;
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    HSD_JObj *j24 = 0;
    s16 i = 0;
    s16 part = 0;
    s16 have = 0;
    s16 dlg = 0;
    s16 allz = 0;
    char text[0x3C];

    (void)dB;
    (void)dA;

    if (menuzoidhavepartslist != 0) {
        src = GetChild(((MenuArchive *)menu1p)->joints[21], 0, 2, -1, -1, -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[10], 0, -1, -1, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDispAll(dst, 0, 7, 0);
        if (menumodeShop != 0) {
            src = GetChild(((MenuArchive *)menu1p)->joints[10], 0, 4, -1, -1, -1);
            dst = GetChild(((MenuArchive *)menu1p)->joints[18], 0, 1, -1, -1, -1);
            jobjCopyPos(src, dst);
            HSD_JObjDispAll(dst, 0, 7, 0);
            if ((s32)menumodeShop == 1) {
                dlg = dlgzoidno;
                src = GetChild(((MenuArchive *)menu1p)->joints[21], 0, 1, -1, -1, -1);
                j24 = GetChild(((MenuArchive *)menu_zoidp[dlg])->joints[0], 0, 1, -1, -1, -1);
                jobjCopyPos(src, j24);
                HSD_JObjDispAll(j24, 0, 7, 0);
                sprintf(text, lit_3644, Tea_zoiddat[dlg].name);
                putAscJobj(src, 0, text);
            }
        }
        for (i = 0; i < menusetmh[3]; i++) {
            src = GetChild(((MenuArchive *)menu1p)->joints[10], 0, 3, i, -1, -1);
            part = IveSave[0].list_parts[menusetst[3] + i];
            if ((s32)menumodeShop == 1) {
                part = IveSave[0].list_parts[menusetst[3] + i];
            }
            if ((s32)menumodeShop == 2) {
                part = IveSave[0].list_parts[menusetst[3] + i];
            }
            PutParts(0, part, src);
            if ((s32)menumodeShop == 1) {
                dst = GetChild(((MenuArchive *)menu1p)->joints[11], 0, 2, 0, -1, -1);
                have = menuSeartchPartsHave(part);
                allz = menuSeartchPartsAllZoid(part);
                sprintf(text, "F%03d/E%02d", have, allz);
                putAscJobj(dst, 2, text);
            }
            if (i == menusetsl[3]) {
                if (menumodeShop != 0) {
                    dst = GetChild(((MenuArchive *)menu1p)->joints[18], 0, 1, 1, -1, -1);
                    if ((s32)menumodeShop == 1) {
                        sprintf(text, "%7d", Tea_partsdat[part].price);
                    }
                    if ((s32)menumodeShop == 2) {
                        sprintf(text, "%7d", Tea_partsdat[part].field_54);
                    }
                    putAscJobjLR(dst, 4, text, 1);
                }
                if ((s32)menucuranm[0] == 1) {
                    SetAnimArc2((MenuArchive *)menu1p, 7, 0);
                    menucuranm[0] = 2;
                }
                if ((s32)menucuranm[0] == 2) {
                    if (jobjAnim(((MenuArchive *)menu1p)->joints[7]) != 0) {
                        menucuranm[0] = 3;
                    }
                } else if ((s32)menucuranm[0] == 3) {
                    SetAnimArc2((MenuArchive *)menu1p, 7, 0);
                    jobjAnim(((MenuArchive *)menu1p)->joints[7]);
                    menucuranm[0] = 2;
                }
                dst = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 2, -1, -1, -1);
                jobjCopyPos(src, dst);
                HSD_JObjDispAll(dst, 0, 7, 0);
                sprintf(menudispwinstr, lit_4089, Tea_partsdat[part].name, GetMenuMsgAdr(0x25));
                menudispwinstr[36] = 0;
            }
        }
    }
}
#undef lit_5753
#define lit_5753 "F%03d/E%02d"
#undef lit_5754
#define lit_5754 "%7d"

void menu_sDispBattleMsgArea(void)
{
    s32 x = 0;
    s16 y = 0;
    s16 z = 18;
    register s32 dead = 0;
    s8 country = 0;

    if (jobjAnim(((MenuArchive *)menu1p)->joints[4]) != 0) {
        SetAnimArc2((MenuArchive *)menu1p, 4, 1);
        jobjAnim(((MenuArchive *)menu1p)->joints[4]);
    }
    country = IveSave[0].country;
    HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[4], 0, country, -1, -1, -1), 0, 7, 0);
    dead = 1;
}

void menu_sDispBattleShopHaveBp(void)
{
    char text[8];
    s16 unused = 0;
    register s32 dead1 = 0;
    register s32 dead2 = 0;
    s16 kind = 18;

    HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 0, -1, -1, -1), 0, 7, 0);
    if (IveSave[0].money > 9999999) {
        sprintf(text, "9999999");
    } else {
        sprintf(text, lit_5754, IveSave[0].money);
    }
    putAscJobjLR(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 0, 1, -1, -1), 4, text, 1);
}
#undef lit_5762
#define lit_5762 "9999999"

void menu_sDispBattleShopZoidHaveZoidList(void)
{
    HSD_JObj *a = 0;
    HSD_JObj *b = 0;
    HSD_JObj *c = 0;
    s16 i = 0;
    s16 zi = 0;
    char text[0x38];

    if (menumode != 0 && menudispshophavezoidlist != 0) {
        a = GetChild(((MenuArchive *)menu1p)->joints[21], 0, 0, -1, -1, -1);
        b = GetChild(((MenuArchive *)menu1p)->joints[12], 0, -1, -1, -1, -1);
        jobjCopyPos(a, b);
        HSD_JObjDispAll(b, 0, 7, 0);
        for (i = 0; i < menusetmh[3]; i++) {
            a = GetChild(((MenuArchive *)menu1p)->joints[12], 0, 3, i, -1, -1);
            if (menudispshophavezoidlist == 1)
                zi = IveSave[0].zoid_list[menusetst[3] + i];
            if (menudispshophavezoidlist == 2)
                zi = IveSave[0].owned_zoids[menusetst[3] + i];
            if (zi == -1 || menu_zoidp[zi] == NULL || ((MenuArchive *)menu_zoidp[zi])->joints[0] == NULL) {
                OSReport("who=err\n", menusetst[3] + i);
            } else {
                c = GetChild(((MenuArchive *)menu_zoidp[zi])->joints[0], 0, 1, -1, -1, -1);
                jobjCopyPos(a, c);
                HSD_JObjDispAll(c, 0, 7, 0);
                sprintf(text, " %s", Tea_zoiddat[zi].name);
                putAscJobj(a, 0, text);
                if (i == menusetsl[3]) {
                    if ((s32)menumodeShop == 1 && menudispshophavezoidlist == 1) {
                        a = GetChild(((MenuArchive *)menu1p)->joints[12], 0, 4, -1, -1, -1);
                        c = GetChild(((MenuArchive *)menu1p)->joints[18], 0, 1, -1, -1, -1);
                        jobjCopyPos(a, c);
                        HSD_JObjDispAll(c, 0, 7, 0);
                        a = GetChild(((MenuArchive *)menu1p)->joints[18], 0, 1, 1, -1, -1);
                        sprintf(text, "%7d", Tea_zoiddat[IveSave[0].zoid_list[menusetst[3] + i]].field_50);
                        putAscJobjLR(a, 4, text, 1);
                    }
                    if ((s32)menumodeShop == 2 && menudispshophavezoidlist == 2) {
                        a = GetChild(((MenuArchive *)menu1p)->joints[12], 0, 4, -1, -1, -1);
                        c = GetChild(((MenuArchive *)menu1p)->joints[18], 0, 1, -1, -1, -1);
                        jobjCopyPos(a, c);
                        HSD_JObjDispAll(c, 0, 7, 0);
                        a = GetChild(((MenuArchive *)menu1p)->joints[18], 0, 1, 1, -1, -1);
                        sprintf(text, "%7d", *(s32 *)Tea_zoiddat[IveSave[0].owned_zoids[menusetst[3] + i]].field_54);
                        putAscJobjLR(a, 4, text, 1);
                    }
                    menuyesnoshopmsg[32] = 0;
                    if ((s32)menucuranm[0] == 1) {
                        SetAnimArc2((MenuArchive *)menu1p, 7, 0);
                        menucuranm[0] = 2;
                    }
                    if ((s32)menucuranm[0] == 2) {
                        if (jobjAnim(((MenuArchive *)menu1p)->joints[7]) != 0)
                            menucuranm[0] = 3;
                    } else if ((s32)menucuranm[0] == 3) {
                        SetAnimArc2((MenuArchive *)menu1p, 7, 0);
                        jobjAnim(((MenuArchive *)menu1p)->joints[7]);
                        menucuranm[0] = 2;
                    }
                    a = GetChild(((MenuArchive *)menu1p)->joints[12], 0, 3, i, -1, -1);
                    b = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 1, -1, -1, -1);
                    jobjCopyPos(a, b);
                    HSD_JObjDispAll(b, 0, 7, 0);
                }
            }
        }
    }
}
#undef lit_5798
#define lit_5798 "who=err\n"

void menu_sDispBattleShopLButtonAnimZoidWepOptSub(void)
{
    s32 idx = 7;
    s32 anim;
    s32 done;
    s32 dead;

    anim = 17;
    done = 0;
    dead = 0;
    (void)dead;
    if (menusetbtanm[idx].cur != menusetbtanm[idx].old) {
        menusetbtanm[idx].state = 1;
        menusetbtanm[idx].old = menusetbtanm[idx].cur;
    }
    if (menusetbtanm[idx].state == 1) {
        menusetbtanm[idx].state = 2;
        SetAnimArc2((MenuArchive *)menu1p, anim, menusetbtanm[idx].cur);
    }
    if (menusetbtanm[idx].state == 2) {
        done = jobjAnim(((MenuArchive *)menu1p)->joints[anim]);
        sMakeMatrix(((MenuArchive *)menu1p)->joints[anim]);
        if (done != 0) {
            menusetbtanm[idx].state = 3;
            menuanmend = 1;
            switch (menusetbtanm[idx].cur) {
            case 0:
                menusetbtanm[idx].cur = 1;
                menusetbtanm[idx].state = 2;
                break;
            case 1:
                menusetbtanm[idx].cur = 1;
                menusetbtanm[idx].old = 2;
                break;
            case 2:
                menusetbtanm[idx].cur = 3;
                menusetbtanm[idx].old = 2;
                break;
            case 3:
                menusetbtanm[idx].cur = 3;
                menusetbtanm[idx].old = 2;
                break;
            case 4:
                menusetbtanm[idx].cur = 1;
                menusetbtanm[idx].old = 2;
                break;
            case 5:
                menusetbtanm[idx].cur = 6;
                menusetbtanm[idx].old = 2;
                break;
            case 6:
                menusetbtanm[idx].cur = 6;
                menusetbtanm[idx].old = 2;
                break;
            case 7:
                menusetbtanm[idx].cur = 1;
                menusetbtanm[idx].old = 2;
                break;
            case 8:
                menusetbtanm[idx].cur = 9;
                menusetbtanm[idx].old = 2;
                break;
            case 9:
                menusetbtanm[idx].cur = 9;
                menusetbtanm[idx].old = 2;
                break;
            case 10:
                menusetbtanm[idx].cur = 1;
                menusetbtanm[idx].old = 2;
                break;
            case 11:
                menusetbtanm[idx].cur = 12;
                menusetbtanm[idx].old = 2;
                break;
            case 12:
                menusetbtanm[idx].cur = 12;
                menusetbtanm[idx].old = 2;
                break;
            case 13:
                menusetbtanm[idx].cur = 1;
                menusetbtanm[idx].old = 2;
                break;
            case 14:
                menusetbtanm[idx].cur = 14;
                menusetbtanm[idx].old = 14;
                menusetbtanm[idx].state = 3;
                return;
            default:
                menusetbtanm[idx].state = 2;
                break;
            }
            SetAnimArc2((MenuArchive *)menu1p, anim, menusetbtanm[idx].cur);
            jobjAnim(((MenuArchive *)menu1p)->joints[anim]);
            sMakeMatrix(((MenuArchive *)menu1p)->joints[anim]);
        }
    }
    HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[anim], 0, -1, -1, -1, -1), 0, 7, 0);
}

void menu_sDispBattleShopLButtonZoid(void)
{
    s32 k = 9;
    HSD_JObj *child = 0;
    HSD_JObj *joint = 0;

    joint = GetChild(((MenuArchive *)menu1p)->joints[17], 0, 0, -1, -1, -1);
    child = GetChild(((MenuArchive *)menu1p)->joints[k], 0, 0, -1, -1, -1);
    jobjCopyPos(joint, child);
    if ((u8)menusetbtanm[0].cur != menusetbtanm[0].old) {
        menusetbtanm[0].state = 1;
        menusetbtanm[0].old = menusetbtanm[0].cur;
    }
    if (menusetbtanm[0].state == 1) {
        menusetbtanm[0].state = 2;
        sSetAnim1(menu1p, k, GetChild(((MenuArchive *)menu1p)->joints[k], 0, 0, -1, -1, -1), menusetbtanm[0].cur);
    }
    if (menusetbtanm[0].state == 2) {
        if (jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[k], 0, 0, -1, -1, -1)) != 0) {
            menusetbtanm[0].state = 3;
            menuanmend = 1;
            if (menusetbtanm[0].cur == 5)
                return;
            if (menusetbtanm[0].cur == 0) {
                if (menusetbtanm[0].cur == 0)
                    menusetbtanm[0].cur = 1;
                sSetAnim1(menu1p, k, GetChild(((MenuArchive *)menu1p)->joints[k], 0, 0, -1, -1, -1), menusetbtanm[0].cur);
                jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[k], 0, 0, -1, -1, -1));
            } else {
                menusetbtanm[0].state = 1;
            }
        }
    }
    HSD_JObjDispAll(child, 0, 7, 0);
}

void menu_sDispBattleShopunfold(void)
{
    s32 k = 10;
    s32 b = 16;
    HSD_JObj *j = 0;
    s32 dead = 0;

    if (menusetbtanm[k].cur != menusetbtanm[k].old) {
        menusetbtanm[k].state = 1;
        menusetbtanm[k].old = menusetbtanm[k].cur;
    }
    if (menusetbtanm[k].state == 1) {
        menusetbtanm[k].state = 2;
        SetAnimArc2((MenuArchive *)menu1p, b, menusetbtanm[k].cur);
    }
    if (menusetbtanm[k].state == 2) {
        j = (HSD_JObj *)jobjAnim(((MenuArchive *)menu1p)->joints[b]);
        if (j != 0) {
            sMakeMatrix(((MenuArchive *)menu1p)->joints[b]);
            menusetbtanm[k].state = 3;
            menuanmend = 1;
            switch (menusetbtanm[k].cur) {
            case 1:
                menusetbtanm[k].cur = 2;
                menusetbtanm[k].state = 2;
                break;
            case 3:
                menusetbtanm[k].cur = 0;
                menusetbtanm[k].state = 2;
                break;
            case 4:
                menusetbtanm[k].cur = 5;
                menusetbtanm[k].state = 2;
                break;
            case 6:
                menusetbtanm[k].cur = 0;
                menusetbtanm[k].state = 2;
                break;
            default:
                menusetbtanm[k].state = 2;
                break;
            }
            SetAnimArc2((MenuArchive *)menu1p, b, menusetbtanm[k].cur);
            jobjAnim(((MenuArchive *)menu1p)->joints[b]);
        }
    }
    HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[b], 0, -1, -1, -1, -1), 0, 7, 0);
    dead = 0;
}

void menuZoidBattleShopmenubuttonset(void)
{
    s32 dead = 0;
    s32 i = 0;

    for (i = 0; i < 7; i++)
        menusetbtanm[i].cur = 1;
    if (menumode == 1)
        menusetbtanm[menusetcl[menumode] + 4].cur = 2;
    else if (menumode == 10)
        menusetbtanm[menusetcl[menumode] + 5].cur = 2;
    else if (menumode == 2) {
        if (menumodeShop == 1) {
            menusetbtanm[5].cur = 3;
            menusetbtanm[5].state = 1;
        }
        if (menumodeShop == 2) {
            menusetbtanm[6].cur = 3;
            menusetbtanm[6].state = 1;
        }
        menusetbtanm[menusetcl[menumode]].cur = 2;
    } else if (menumode == 3) {
        if (menumodeShop == 1) {
            menusetbtanm[5].cur = 3;
            menusetbtanm[5].state = 1;
        }
        if (menumodeShop == 2) {
            menusetbtanm[6].cur = 3;
            menusetbtanm[6].state = 1;
        }
        menusetbtanm[0].cur = 3;
        menusetbtanm[0].state = 1;
    } else if (menumode == 4) {
        return;
    }
}

void menu_sDispBattleShopLButton(void)
{
    if (menumode != 0) {
        menu_sDispBattleShopLButtonAnm(0);
        menu_sDispBattleShopLButtonAnm(1);
        menu_sDispBattleShopLButtonAnm(2);
    }
}

void menu_sDispBattleShopLButtonZoidWepOptSub(void)
{
    if (menumode != 0 && (menumode == 2 || menumode == 3)) {
        menu_sDispBattleShopLButtonZoid();
        menu_sDispBattleShopLButtonWeapon();
        menu_sDispBattleShopLButtonOption();
        menu_sDispBattleShopLButtonSubParts();
    }
}

void menu_sDispBattleShopLButtonWeapon(void)
{
    menu_sDispBattleShopLButtonMainAnm(1);
}

void menu_sDispBattleShopLButtonOption(void)
{
    menu_sDispBattleShopLButtonMainAnm(2);
}

void menu_sDispBattleShopLButtonSubParts(void)
{
    menu_sDispBattleShopLButtonMainAnm(3);
}

void menu_sDispBattleShopLButtonAnm(s32 k)
{
    s32 b = 15;
    s32 m = 0;
    HSD_JObj *a = 0;
    HSD_JObj *j = 0;

    a = GetChild(((MenuArchive *)menu1p)->joints[16], 0, k, -1, -1, -1);
    j = GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1);
    m = k + 5;
    if (m == 7) {
        m = 4;
    }
    if (menusetbtanm[m].cur != menusetbtanm[m].old) {
        menusetbtanm[m].state = 1;
        menusetbtanm[m].old = menusetbtanm[m].cur;
    }
    if (menusetbtanm[m].state == 1) {
        menusetbtanm[m].state = 2;
        sSetAnim1((MenuArchive *)menu1p, b, GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1), menusetbtanm[m].cur);
    }
    if (menusetbtanm[m].state == 2) {
        if (jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1)) != 0) {
            menusetbtanm[m].state = 3;
            menuanmend = 1;
            if (menusetbtanm[m].cur == 5) {
                return;
            }
            if (menusetbtanm[m].cur == 0) {
                if (menusetbtanm[m].cur == 0) {
                    menusetbtanm[m].cur = 1;
                }
                sSetAnim1((MenuArchive *)menu1p, b, GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1), menusetbtanm[m].cur);
                jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[b], 0, k, -1, -1, -1));
            } else {
                menusetbtanm[m].state = 1;
            }
        }
    }
    jobjCopyPos(a, j);
    HSD_JObjDispAll(j, 0, 7, 0);
}

void menu_sDispBattleShopLButtonMainAnm(s32 button)
{
    s32 b = 9;
    HSD_JObj *a = 0;
    HSD_JObj *j = 0;

    a = GetChild(((MenuArchive *)menu1p)->joints[17], 0, button, -1, -1, -1);
    j = GetChild(((MenuArchive *)menu1p)->joints[b], 0, button, -1, -1, -1);
    if (menusetbtanm[button].cur != menusetbtanm[button].old) {
        menusetbtanm[button].state = 1;
        menusetbtanm[button].old = menusetbtanm[button].cur;
    }
    if (menusetbtanm[button].state == 1) {
        menusetbtanm[button].state = 2;
        sSetAnim1((MenuArchive *)menu1p, b, GetChild(((MenuArchive *)menu1p)->joints[b], 0, button, -1, -1, -1), menusetbtanm[button].cur);
    }
    if (menusetbtanm[button].state == 2) {
        if (jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[b], 0, button, -1, -1, -1)) != 0) {
            menusetbtanm[button].state = 3;
            menuanmend = 1;
            menuanmend = 1;
            switch (menusetbtanm[button].cur) {
            case 0:
                menusetbtanm[button].cur = 1;
                menusetbtanm[button].state = 2;
                break;
            case 1:
                menusetbtanm[button].cur = 1;
                menusetbtanm[button].state = 2;
                break;
            case 2:
                menusetbtanm[button].cur = 2;
                menusetbtanm[button].state = 2;
                break;
            case 3:
                menusetbtanm[button].cur = 3;
                menusetbtanm[button].state = 2;
                break;
            case 4:
                menusetbtanm[button].cur = 4;
                menusetbtanm[button].state = 2;
                break;
            case 5:
                menusetbtanm[button].state = 0;
                return;
            default:
                menusetbtanm[button].state = 1;
                break;
            }
            sSetAnim1((MenuArchive *)menu1p, b, GetChild(((MenuArchive *)menu1p)->joints[b], 0, button, -1, -1, -1), menusetbtanm[button].cur);
            jobjAnim1(GetChild(((MenuArchive *)menu1p)->joints[b], 0, button, -1, -1, -1));
        }
    }
    jobjCopyPos(a, j);
    HSD_JObjDispAll(j, 0, 7, 0);
}

void menufncShopyesno(s32 buy)
{
    char text[204];

    menudispyesnoflag = 1;
    do {
        sPadAll();
        if (buy != 0)
            sprintf(text, "%s\n%s", menuyesnoshopmsg, GetMenuMsgAdr(0x4B));
        else
            sprintf(text, "%s\n%s", menuyesnoshopmsg, GetMenuMsgAdr(0x4C));
        menudispyesnopoint = menuYesNo(menudispyesnopoint, text, "");
        menu_sDispBattleShop();
    } while (sPad1->pressed_a == 0);
    SeRet();
    menudispyesnoflag = 0;
}
#undef lit_5938
#define lit_5938 "%s\n%s"

void shopDispParts(s32 slot)
{
    GameSaveRecord *save = &IveSave[slot];
    s32 i;
    s32 count;

    OSReport("parts --- ");
    for (i = 0; i < 450; i++) {
        if ((count = save->owned_parts[i]) > 0) {
            if (i == save->owned_part_count)
                OSReport(" : ");
            OSReport("%3d ", save->owned_parts[i]);
        }
    }
    OSReport(" --- \n");
}
#undef lit_5971
#define lit_5971 "parts --- "
#undef lit_5972
#define lit_5972 " : "
#undef lit_5973
#define lit_5973 "%3d "
#undef lit_5974
#define lit_5974 " --- \n"
s32 menufncZoidBattle_BattleMenuShop(void)
{
    s32 unk0 = 0;
    s32 unk1 = 0;
    s32 unk2 = 0;
    s16 zn[3];
    s32 unk3 = 0;
    s32 unk4 = 0;
    f32 unkf = 0.0f;
    s32 i = 0;
    s32 k = 0;
    s32 mh = 0;
    s32 mn = 0;
    s32 color = 1;
    s32 j = 0;
    s32 change = 0;
    s32 count;
    s32 sel;

    menumode = 0;
    dlgpersonno = 0;
    dlgzoidno = 0;
    menumodeShop = 0;
    TEA_SetTimerInit();
    memcpy(&IveSave[4], &IveSave[0], 0x1a90);
    ttl_SortZoid(0);
    menudispyesnoflag = 0;
    menubackcampos[0] = menubackcambak[0] - 2.0;
    for (i = 0; i < 12; i++) {
        menubackintpos[0] = menubackintbak[0] - i * 0.5;
        menu_sDispBattleShop();
    }
    menubackintpos[0] = menubackintbak[0] - 6.0;
    menu_sDispBattleShop();
    for (j = 0; j < 7; j++) {
        menusetbtanm[j].old = menusetbtanm[j].cur = 0;
        menusetbtanm[j].state = 1;
    }
    j = 7;
    menusetbtanm[j].old = menusetbtanm[j].cur = 1;
    menusetbtanm[j].state = 0;
    j = 8;
    menusetbtanm[j].old = menusetbtanm[j].cur = 0;
    menusetbtanm[j].state = 0;
    for (j = 9; j < 11; j++) {
        menusetbtanm[j].old = menusetbtanm[j].cur = 0;
        menusetbtanm[j].state = 0;
    }
    for (i = 0; i < 20; i++) {
        menusetcl[i] = 0;
        menusetsl[i] = 0;
        menusetst[i] = 0;
        menusetmh[i] = 0;
        menusetmn[i] = 0;
    }
    menusetcl[1] = 0;
    menusetsl[1] = 0;
    menusetmh[1] = 1;
    menusetmn[1] = 1;
    menusetcl[2] = 0;
    menusetsl[2] = 0;
    menusetmh[2] = 4;
    menusetmn[2] = 4;
    menusetcl[3] = 0;
    menusetsl[3] = 0;
    menusetmh[3] = 4;
    menusetmn[3] = IveSave[0].owned_zoid_count;
    if (menusetmn[3] < 4)
        menusetmh[3] = menusetmn[3];
    i = 4;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 8;
    menusetmn[i] = 10;
    i = 6;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 3;
    menusetmn[i] = 3;
    i = 5;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 5;
    i = 7;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 5;
    menusetmn[i] = 10;
    i = 8;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 5;
    menusetmn[i] = 10;
    i = 10;
    menusetcl[i] = 0;
    menusetsl[i] = 0;
    menusetmh[i] = 2;
    menusetmn[i] = 2;
    menusetbtanm[7].cur = 0;
    menumode = 1;
    menuZoidBattleShopmenubuttonset();
    dlgzoidno = IveSave[0].owned_zoids[menusetst[3]];
    menudispshophavezoidlist = 0;
    menuzoidhavepartslist = 0;
    menusetbtanm[10].cur = 0;
    menusetbtanm[10].state = 1;
    menu_sDispBattleShop();
    menucuranm[0] = 1;
    SetMenuMsgAdr(0x4f, menudispwinstr);
    zn[0] = IveSave[0].zoid_slot;
    if ((u8)menuZoidBattleModeSingleTeam == 1)
        zn[1] = IveSave[0].partner_slot;
    else
        zn[1] = -1;
    zn[2] = IveSave[0].zoid_slot;
    flag_AllHavePartsList = 0;
    menuSortPartsHave();
    menuDetailMode = 1;
    detCustm(IveSave[0].zoid_slot);
    flPicturePrologue();
    for (;;) {
        sPadAll();
        menusetcl[menumode] = TEA_menufnc(&menusetsl[menumode], &menusetst[menumode], menusetmn[menumode], menusetmh[menumode]);
        if (menumode == 1)
            SetMenuMsgAdr(0x58, menudispwinstr);
        if (menumode == 10)
            SetMenuMsgAdr(menusetcl[menumode] + 0x39, menudispwinstr);
        if (menumode == 2)
{
            i = menusetcl[menumode] + (menumodeShop - 1) * 4;
            SetMenuMsgAdr(i + 0x3b, menudispwinstr);
        }
        if (sPad1->field_4F || sPad1->field_53) {
            if (menumode == 4 || menumode == 7)
                menufnczoidmountpointdispset(menusetcl[4]);
            menuZoidBattleShopmenubuttonset();
        }
        if (sPad1->field_50 || sPad1->field_54) {
            if (menumode == 4 || menumode == 7)
                menufnczoidmountpointdispset(menusetcl[4]);
            menuZoidBattleShopmenubuttonset();
        }
        if (sPad1->field_51 || sPad1->field_55) {
            if (menumode == 1) {
                menumode = 10;
                menuZoidBattleShopmenubuttonset();
            }
        }
        if (sPad1->field_52 || sPad1->field_56) {
            if (menumode == 10) {
                menumode = 1;
            }
            menuZoidBattleShopmenubuttonset();
        }
        if (sPad1->field_5C)
            flag_AllHavePartsList++;
        ttl_batshop_detailsetparam();
        if (sPad1->pressed_a) {
            if (menumode == 1) {
                SeRet();
                menudispwinstr[0] = 0;
                break;
            }
            if (menumode == 10) {
                SeRet();
                if (menusetcl[menumode] == 0) {
                    menumodeShop = 1;
                    menusetbtanm[10].cur = 1;
                    menusetbtanm[10].state = 1;
                    while (menusetbtanm[10].cur != 2)
                        menu_sDispBattleShop();
                    menusetbtanm[7].cur = 0;
                    menusetbtanm[7].state = 1;
                    while (menusetbtanm[7].cur != 1)
                        menu_sDispBattleShop();
                    menusetcl[3] = 0;
                    menusetsl[3] = 0;
                    menusetst[3] = 0;
                    menusetmh[3] = 4;
                    menusetmn[3] = IveSave[0].zoid_list_count;
                    if (menusetmn[3] < 4)
                        menusetmh[3] = menusetmn[3];
                    menusetbtanm[5].cur = 3;
                }
                if (menusetcl[menumode] == 1) {
                    menumodeShop = 2;
                    menusetbtanm[10].cur = 4;
                    menusetbtanm[10].state = 1;
                    while (menusetbtanm[10].cur != 5)
                        menu_sDispBattleShop();
                    menusetbtanm[7].cur = 0;
                    menusetbtanm[7].state = 1;
                    while (menusetbtanm[7].cur != 1)
                        menu_sDispBattleShop();
                    menusetcl[3] = 0;
                    menusetsl[3] = 0;
                    menusetst[3] = 0;
                    menusetmh[3] = 4;
                    menusetmn[3] = IveSave[0].owned_zoid_count;
                    if (menusetmn[3] < 4)
                        menusetmh[3] = menusetmn[3];
                    menusetbtanm[6].cur = 3;
                    menuSortPartsHave();
                }
                menumode = 2;
                menuZoidBattleShopmenubuttonset();
                continue;
            }
            if (menumode == 2) {
                if (menusetcl[menumode] == 0) {
                    if (IveSave[0].owned_zoid_count == 0 && menumodeShop == 2) {
                        change = 0;
                        SeCancel();
                        continue;
                    }
                    SeRet();
                    menusetcl[3] = 0;
                    menusetsl[3] = 0;
                    menusetst[3] = 0;
                    menusetmh[3] = 4;
                    if (menumodeShop == 1)
                        menusetmn[3] = IveSave[0].zoid_list_count;
                    if (menumodeShop == 2)
                        menusetmn[3] = IveSave[0].owned_zoid_count;
                    if (menusetmn[3] < 4)
                        menusetmh[3] = menusetmn[3];
                    menumode = 3;
                    menuZoidBattleShopmenubuttonset();
                    menu_sDispBattleShop();
                    menusetbtanm[7].cur = 2;
                    menusetbtanm[7].state = 1;
                    while (menusetbtanm[7].cur != 3)
                        menu_sDispBattleShop();
                    menudispshophavezoidlist = menumodeShop;
                    menusetbtanm[8].cur = 1;
                    menu_sDispBattleShop();
                    IveSave[0].field_15F8 = IveSave[0].zoid_slot;
                    if (menumodeShop == 1) {
                        menuDetailMode = 2;
                        detZoid(-1);
                        for (;;) {
                            sPadAll();
                            menu_sDispBattleShop();
                            menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                            detZoid(IveSave[0].zoid_list[menusetcl[3]]);
                            if (sPad1->field_5C)
                                flag_AllHavePartsList++;
                            if (sPad1->field_5E) {
                                SeCancel();
                                menuDetailMode = 1;
                                detZoid(-1);
                                break;
                            }
                            if (!sPad1->pressed_a)
                                continue;
                            menudispshophavezoidlist = 0;
                            if (IveSave[0].owned_zoid_count >= 30) {
                                SeCancel();
                                menudispyesnoflag = 1;
                                menudispyesnopoint = 1;
                                do {
                                    sPadAll();
                                    menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x4f), "BUY");
                                    menu_sDispBattleShop();
                                } while (!sPad1->pressed_a);
                                menudispyesnoflag = 0;
                                menu_sDispBattleShop();
                                menudispshophavezoidlist = menumodeShop;
                                continue;
                            }
                            dlgzoidno = IveSave[0].zoid_list[menusetcl[3]];
                            if (IveSave[0].money >= Tea_zoiddat[dlgzoidno].field_50) {
                                SeRet();
                                SeLiftStart();
                                menu_StageUpDownFlag = 0;
                                menuDetailMode = 0;
                                while (menu_sDispBattleStageDown(1) == 0)
                                    menu_sDispBattleShop();
                                SeLiftStop();
                                ttl_WeaponAllUnMount(IveSave[0].field_15F8);
                                ZoidPurge(&Zoid[0]);
                                ttl_GameZoidSetZoidDefParts(0, dlgzoidno);
                                if (dlgzoidno != -1) {
                                    ZoidInit(&Zoid[0], dlgzoidno);
                                }
                                color = Zoid[0].field_1DDC;
                                color = GameZoid[0].color;
                                if (!ZoidColor(&Zoid[0], color, 0))
                                    ZoidColor(&Zoid[0], 0, 0);
                                ttl_IveSaveSetZoidDefParts(0, IveSave[0].owned_zoid_count, dlgzoidno);
                                ttl_WeaponAllMount(IveSave[0].owned_zoid_count);
                                SeLiftStart();
                                menu_StageUpDownFlag = 0;
                                menuDetailMode = 0;
                                while (menu_sDispBattleStageDown(0) == 0)
                                    menu_sDispBattleShop();
                                SeLiftStop();
                                menudispyesnopoint = 1;
                                menufncShopyesno(1);
                                if (menudispyesnopoint != 0) {
                                    IveSave[0].money -= Tea_zoiddat[dlgzoidno].field_50;
                                    IveSave[0].zoid_slot = IveSave[0].owned_zoid_count;
                                    IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                                    zn[2] = IveSave[0].zoid_slot;
                                    IveSave[0].owned_zoid_count++;
                                    menudispshophavezoidlist = menumodeShop;
                                    menuZoidBattleShopmenubuttonset();
                                    continue;
                                }
                                SeLiftStart();
                                menu_StageUpDownFlag = 0;
                                menuDetailMode = 0;
                                while (menu_sDispBattleStageDown(1) == 0)
                                    menu_sDispBattleShop();
                                SeLiftStop();
                                ZoidWeaponPurge(Zoid);
                                ZoidPurge(&Zoid[0]);
                                IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                                if (IveSave[0].field_15F4 != -1) {
                                    ZoidInit(Zoid, IveSave[0].field_15F4);
                                    ZoidSetAnim(&Zoid[0], 0x67);
                                }
                                ttl_WeaponAllMount(IveSave[0].zoid_slot);
                                color = IveSave[0].zoid_color[IveSave[0].zoid_slot];
                                if (ZoidColor(Zoid, color, 0) == 0)
                                    ZoidColor(Zoid, 0, 0);
                                SeLiftStart();
                                menu_StageUpDownFlag = 0;
                                menuDetailMode = 0;
                                while (menu_sDispBattleStageDown(0) == 0)
                                    menu_sDispBattleShop();
                                SeLiftStop();
                                menudispshophavezoidlist = menumodeShop;
                                IveSave[0].owned_zoids[IveSave[0].owned_zoid_count] = -1;
                            } else {
                                MessageBoxBatShopNoBp();
                                menudispshophavezoidlist = menumodeShop;
                            }
                        }
                    } else if (menumodeShop == 2) {
                        menusetmn[3] = IveSave[0].owned_zoid_count;
                        detCustm(-1);
                        for (;;) {
                            sPadAll();
                            menu_sDispBattleShop();
                            menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                            if (sPad1->field_5C)
                                flag_AllHavePartsList++;
                            if (sPad1->field_5E) {
                                SeCancel();
                                detCustm(-1);
                                break;
                            }
                            detCustm(menusetcl[3]);
                            if (!sPad1->pressed_a)
                                continue;
                            OSReport("menu set cl =%2d \n", menusetcl[3]);
                            if (zn[0] == menusetcl[3]) {
                                SeBuzzerC();
                                continue;
                            }
                            if (zn[1] == menusetcl[3] && (u8)menuZoidBattleModeSingleTeam == 1) {
                                SeBuzzerC();
                                continue;
                            }
                            if (IveSave[0].owned_zoid_count <= 2) {
                                SeBuzzerC();
                                continue;
                            }
                            detCustm(-1);
                            SeRet();
                            menudispyesnopoint = 1;
                            menufncShopyesno(0);
                            if (menudispyesnopoint == 0) {
                                menudispshophavezoidlist = menumodeShop;
                                continue;
                            }
                            ttl_WeaponAllUnMount(menusetcl[3]);
                            for (i = 0; i < 8; i++) {
                                if ((k = ttl_ZoidRemoveContractWep(0, menusetcl[3], i)) != -1) {
                                    if (IveSave[0].owned_part_count < 450)
                                        ttl_HaveAddParts(0, k);
                                    else
                                        IveSave[0].money += Tea_partsdat[k].field_54;
                                }
                            }
                            if ((k = ttl_ZoidRemoveContractOpt(0, menusetcl[3])) != -1) {
                                if (IveSave[0].owned_part_count < 450)
                                    ttl_HaveAddParts(0, k);
                                else
                                    IveSave[0].money += Tea_partsdat[k].field_54;
                            }
                            for (i = 0; i < 3; i++) {
                                if ((k = ttl_ZoidRemoveContractSub(0, menusetcl[3], i)) != -1) {
                                    if (IveSave[0].owned_part_count < 450)
                                        ttl_HaveAddParts(0, k);
                                    else
                                        IveSave[0].money += Tea_partsdat[k].field_54;
                                }
                            }
                            menudispshophavezoidlist = 0;
                            change = 0;
                            dlgzoidno = IveSave[0].owned_zoids[menusetcl[3]];
                            IveSave[0].money += *(s32 *)Tea_zoiddat[dlgzoidno].field_54;
                            i = menusetcl[3];
                            zn[2] = i;
                            menuZoidPartsCopy(0, IveSave[0].owned_zoid_count - 1, 0, i);
                            menusetmn[3]--;
                            if ((s16)menusetmn[3] < menusetmh[3])
                                menusetmh[3] = menusetmn[3];
                            if (menusetmn[3] - 1 < menusetcl[3] && menusetst[3] > 0)
                                menusetst[3]--;
                            if (menusetmh[3] - 1 < menusetsl[3])
                                menusetsl[3] = menusetmh[3] - 1;
                            IveSave[0].owned_zoid_count--;
                            IveSave[4].owned_zoid_count--;
                            if (IveSave[0].owned_zoid_count == zn[0]) {
                                zn[0] = i;
                                IveSave[0].field_15F4 = i;
                                OSReport("mine change col=%d \n", i);
                            }
                            if ((u8)menuZoidBattleModeSingleTeam == 1 && IveSave[0].owned_zoid_count == zn[1]) {
                                zn[1] = i;
                                IveSave[0].field_15FA = i;
                                OSReport("partnar change col=%d \n", i);
                            }
                            menusetcl[3] = 0;
                            menusetst[3] = 0;
                            IveSave[0].owned_zoids[i] = IveSave[0].owned_zoids[IveSave[0].owned_zoid_count];
                            IveSave[0].owned_zoids[IveSave[0].owned_zoid_count] = -1;
                            IveSave[0].zoid_slot = zn[0];
                            IveSave[0].partner_slot = zn[1];
                            IveSave[0].field_15F4 = IveSave[0].owned_zoids[zn[0]];
                            IveSave[0].field_15FA = IveSave[0].owned_zoids[zn[1]];
                            OSReport("Err 2:etczn=%d cl[3]=%d znum=%d \n", zn[2], menusetcl[3], IveSave[0].owned_zoid_count);
                            OSReport("Err 2:%d\n", menusetcl[3]);
                            SeLiftStart();
                            menu_StageUpDownFlag = 0;
                            menuDetailMode = 0;
                            while (!menu_sDispBattleStageDown(1))
                                menu_sDispBattleShop();
                            SeLiftStop();
                            ZoidWeaponPurge(&Zoid[0]);
                            ZoidPurge(&Zoid[0]);
                            zn[2] = menusetcl[3];
                            ZoidInit(Zoid, IveSave[0].field_15F4);
                            ZoidSetAnim(&Zoid[0], 0x67);
                            ttl_WeaponAllMount(IveSave[0].zoid_slot);
                            color = IveSave[0].zoid_color[IveSave[0].zoid_slot];
                            if (ZoidColor(&Zoid[0], color, 0) == 0)
                                ZoidColor(&Zoid[0], 0, 0);
                            SeLiftStart();
                            menu_StageUpDownFlag = 0;
                            menuDetailMode = 0;
                            while (!menu_sDispBattleStageDown(0))
                                menu_sDispBattleShop();
                            SeLiftStop();
                            i = IveSave[0].owned_zoid_count;
                            IveSave[0].zoid_color[i] = 0;
                            IveSave[4].zoid_color[i] = 0;
                            IveSave[0].owned_zoids[i] = -1;
                            IveSave[4].owned_zoids[i] = -1;
                            for (k = 0; k < 28; k++)
                                IveSave[0].zoid_parts[i][k] = -1;
                            for (k = 0; k < 28; k++)
                                IveSave[4].zoid_parts[i][k] = -1;
                            for (k = 0; k < 3; k++)
                                IveSave[0].zoid_field_11DA[i][k] = -1;
                            for (k = 0; k < 3; k++)
                                IveSave[4].zoid_field_11DA[i][k] = -1;
                            IveSave[0].zoid_field_119E[i] = -1;
                            menudispshophavezoidlist = menumodeShop;
                        }
                    }
                    menusetbtanm[8].cur = 3;
                    while (menusetbtanm[8].cur != 0)
                        menu_sDispBattleShop();
                    menusetbtanm[8].cur = 0;
                    menu_sDispBattleShop();
                    menudispshophavezoidlist = 0;
                    menuanmend = 0;
                    menusetbtanm[7].cur = 4;
                    menusetbtanm[7].state = 1;
                    menu_sDispBattleShop();
                    menumode = 2;
                    menudispshophavezoidlist = 0;
                    menumode = 2;
                    menusetcl[2] = 0;
                    menusetsl[2] = 0;
                    menusetst[2] = 0;
                    menuZoidBattleShopmenubuttonset();
                    menusetbtanm[menusetcl[menumode]].cur = 2;
                    continue;
                } else if (menusetcl[menumode] == 1) {
                    if (menumodeShop == 1) {
                        if (IveSave[0].owned_zoid_count == 0) {
                            SeCancel();
                            continue;
                        }
                        SeRet();
                        SetMenuMsgAdr(0x44, menudispwinstr);
                        menusetbtanm[7].cur = 5;
                        menusetbtanm[7].state = 1;
                        while (menusetbtanm[7].cur != 6)
                            menu_sDispBattleShop();
                        menusetbtanm[5].cur = 3;
                        menusetbtanm[1].cur = 3;
                        menusetcl[3] = 0;
                        menusetsl[3] = 0;
                        menusetst[3] = 0;
                        menusetmh[3] = 4;
                        menusetmn[3] = IveSave[0].owned_zoid_count;
                        if (menusetmn[3] < 4)
                            menusetmh[3] = menusetmn[3];
                        menudispshophavezoidlist = 2;
                        menusetbtanm[8].cur = 1;
                        menu_sDispBattleShop();
                        for (;;) {
                            sPadAll();
                            menu_sDispBattleShop();
                            menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                            if (sPad1->field_5C)
                                flag_AllHavePartsList++;
                            if (sPad1->field_5E) {
                                SeCancel();
                                break;
                            }
                            detCustm(menusetcl[3]);
                            if (!sPad1->pressed_a)
                                continue;
                            dlgzoidno = IveSave[0].owned_zoids[menusetcl[3]];
                            i = menusetsl[3];
                            k = menusetst[3];
                            mh = menusetmh[3];
                            mn = menusetmn[3];
                            menuPartsShopListAttackerCompareZoid(1, dlgzoidno);
                            if (IveSave[0].list_count == 0) {
                                SeCancel();
                                break;
                            }
                            SeRet();
                            menusetbtanm[8].cur = 3;
                            while (menusetbtanm[8].cur != 0)
                                menu_sDispBattleShop();
                            menudispshophavezoidlist = 0;
                            menusetcl[3] = menusetsl[3] = 0;
                            menusetst[3] = 0;
                            menusetmh[3] = 5;
                            menusetmn[3] = IveSave[0].list_count;
                            if (menusetmn[3] < 5)
                                menusetmh[3] = menusetmn[3];
                            menuzoidhavepartslist = 1;
                            menusetbtanm[8].cur = 4;
                            while (menusetbtanm[8].cur != 5)
                                menu_sDispBattleShop();
                            for (;;) {
                                sPadAll();
                                menu_sDispBattleShop();
                                menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                                if (sPad1->field_5C)
                                    flag_AllHavePartsList++;
                                if (sPad1->field_5E) {
                                    SeCancel();
                                    SetMenuMsgAdr(0x44, menudispwinstr);
                                    break;
                                }
                                detWeapon(IveSave[0].list_parts[menusetcl[3]]);
                                if (!sPad1->pressed_a)
                                    continue;
                                if (IveSave[0].owned_part_count >= 450) {
                                    SeCancel();
                                    menudispyesnoflag = 1;
                                    menudispyesnopoint = 1;
                                    do {
                                        sPadAll();
                                        menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x50), "BUY");
                                        menu_sDispBattleShop();
                                    } while (!sPad1->pressed_a);
                                    menudispyesnoflag = 0;
                                    menu_sDispBattleShop();
                                    continue;
                                }
                                dlgpartsno = IveSave[0].list_parts[menusetcl[3]];
                                if (IveSave[0].money >= Tea_partsdat[dlgpartsno].price) {
                                    SeRet();
                                    menudispyesnopoint = 1;
                                    menufncShopyesno(1);
                                    if (menudispyesnopoint != 1)
                                        continue;
                                    IveSave[0].owned_parts[IveSave[0].owned_part_count] = dlgpartsno;
                                    IveSave[0].owned_part_count++;
                                    IveSave[0].money -= Tea_partsdat[dlgpartsno].price;
                                } else {
                                    MessageBoxBatShopNoBp();
                                }
                            }
                            menusetsl[3] = i;
                            menusetst[3] = k;
                            menusetmh[3] = mh;
                            menusetmn[3] = mn;
                            menusetbtanm[8].cur = 6;
                            while (menusetbtanm[8].cur != 0)
                                menu_sDispBattleShop();
                            menuzoidhavepartslist = 0;
                            menudispshophavezoidlist = 2;
                            menusetbtanm[8].cur = 1;
                            SetMenuMsgAdr(0x44, menudispwinstr);
                            detCustm(-2);
                        }
                        menusetbtanm[8].cur = 3;
                        while (menusetbtanm[8].cur != 0)
                            menu_sDispBattleShop();
                        menudispshophavezoidlist = 0;
                        menusetbtanm[7].cur = 7;
                        menusetbtanm[7].state = 1;
                        menu_sDispBattleShop();
                        menumode = 2;
                        menuZoidBattleShopmenubuttonset();
                        detCustm(-1);
                    } else if (menumodeShop == 2) {
                        menuPartsHaveListZoid(1, -1);
                        if (IveSave[0].list_count == 0) {
                            MessageBoxBatShopNoParts();
                            continue;
                        }
                        SeRet();
                        menusetbtanm[7].cur = 5;
                        menusetbtanm[7].state = 1;
                        while (menusetbtanm[7].cur != 6)
                            menu_sDispBattleShop();
                        menusetbtanm[6].cur = 3;
                        menusetbtanm[1].cur = 3;
                        menusetcl[3] = menusetsl[3] = 0;
                        menusetst[3] = 0;
                        menusetmh[3] = 5;
                        menusetmn[3] = IveSave[0].list_count;
                        if (menusetmn[3] < 5)
                            menusetmh[3] = menusetmn[3];
                        menuzoidhavepartslist = 1;
                        menusetbtanm[8].cur = 7;
                        while (menusetbtanm[8].cur != 8)
                            menu_sDispBattleShop();
                        shopDispParts(0);
                        for (;;) {
                            sPadAll();
                            menu_sDispBattleShop();
                            menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                            if (sPad1->field_5C)
                                flag_AllHavePartsList++;
                            if (sPad1->field_5E) {
                                SeCancel();
                                break;
                            }
                            detWeapon(IveSave[0].list_parts[menusetcl[3]]);
                            if (!sPad1->pressed_a)
                                continue;
                            count = IveSave[0].list_count;
                            sel = menusetcl[3];
                            if (IveSave[0].list_parts[sel] <= 0 || count <= 0) {
                                SeCancel();
                                continue;
                            }
                            SeRet();
                            menudispyesnopoint = 1;
                            menufncShopyesno(0);
                            if (menudispyesnopoint == 0)
                                continue;
                            OSReport("po=%d parts=%d col=%d max=%d \n", IveSave[0].list_flag[sel], (dlgpartsno = IveSave[0].list_parts[sel], dlgpartsno), menusetcl[3], count);
                            IveSave[0].money += Tea_partsdat[dlgpartsno].field_54;
                            menuDelPartsHave(dlgpartsno);
                            menuSortPartsHave();
                            menuPartsHaveListZoid(1, -1);
                            menusetmn[3] = IveSave[0].list_count;
                            if (menusetmn[3] < menusetmh[3])
                                menusetmh[3] = menusetmn[3];
                            if (menusetmn[3] - 1 < menusetcl[3])
                                menusetsl[3] = menusetmn[3] - 1;
                            if (menusetmn[3] - 1 < menusetcl[3] && menusetst[3] > 0)
                                menusetst[3]--;
                            if (menusetmh[3] - 1 < menusetsl[3])
                                menusetsl[3] = menusetmh[3] - 1;
                            if (IveSave[0].list_count == 0)
                                break;
                        }
                        menusetbtanm[8].cur = 9;
                        while (menusetbtanm[8].cur != 0)
                            menu_sDispBattleShop();
                        menuzoidhavepartslist = 0;
                        menusetbtanm[7].cur = 7;
                        menusetbtanm[7].state = 1;
                        menu_sDispBattleShop();
                        shopDispParts(0);
                        shopDispParts(0);
                        menumode = 2;
                        menuZoidBattleShopmenubuttonset();
                        shopDispParts(0);
                        detCustm(-1);
                    }
                    continue;
                } else if (menusetcl[menumode] == 2) {
                    if (menumodeShop == 1) {
                        if (IveSave[0].owned_zoid_count == 0) {
                            SeCancel();
                            continue;
                        }
                        SeRet();
                        SetMenuMsgAdr(0x45, menudispwinstr);
                        menusetbtanm[7].cur = 8;
                        menusetbtanm[7].state = 1;
                        while (menusetbtanm[7].cur != 9)
                            menu_sDispBattleShop();
                        menusetbtanm[5].cur = 3;
                        menusetbtanm[2].cur = 3;
                        menusetcl[3] = 0;
                        menusetsl[3] = 0;
                        menusetst[3] = 0;
                        menusetmh[3] = 4;
                        menusetmn[3] = IveSave[0].owned_zoid_count;
                        if (menusetmn[3] < 4)
                            menusetmh[3] = menusetmn[3];
                        menudispshophavezoidlist = 2;
                        menusetbtanm[8].cur = 1;
                        menu_sDispBattleShop();
                        for (;;) {
                            sPadAll();
                            menu_sDispBattleShop();
                            menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                            if (sPad1->field_5C)
                                flag_AllHavePartsList++;
                            if (sPad1->field_5E) {
                                SeCancel();
                                break;
                            }
                            detCustm(menusetcl[3]);
                            if (!sPad1->pressed_a)
                                continue;
                            SeRet();
                            dlgzoidno = IveSave[0].owned_zoids[menusetcl[3]];
                            i = menusetsl[3];
                            k = menusetst[3];
                            mh = menusetmh[3];
                            mn = menusetmn[3];
                            menuPartsShopListZoid(2, IveSave[0].owned_zoids[menusetcl[3]]);
                            menusetbtanm[8].cur = 3;
                            while (menusetbtanm[8].cur != 0)
                                menu_sDispBattleShop();
                            menudispshophavezoidlist = 0;
                            menusetcl[3] = menusetsl[3] = 0;
                            menusetst[3] = 0;
                            menusetmh[3] = 5;
                            menusetmn[3] = IveSave[0].list_count;
                            if (menusetmn[3] < 5)
                                menusetmh[3] = menusetmn[3];
                            menuzoidhavepartslist = 2;
                            menusetbtanm[8].cur = 4;
                            while (menusetbtanm[8].cur != 5)
                                menu_sDispBattleShop();
                            for (;;) {
                                sPadAll();
                                menu_sDispBattleShop();
                                menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                                if (sPad1->field_5C)
                                    flag_AllHavePartsList++;
                                if (sPad1->field_5E) {
                                    SeCancel();
                                    break;
                                }
                                detOption(IveSave[0].list_parts[menusetcl[3]]);
                                if (!sPad1->pressed_a)
                                    continue;
                                if (IveSave[0].owned_part_count >= 450) {
                                    SeCancel();
                                    menudispyesnoflag = 1;
                                    menudispyesnopoint = 1;
                                    do {
                                        sPadAll();
                                        menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x50), "BUY");
                                        menu_sDispBattleShop();
                                    } while (!sPad1->pressed_a);
                                    menudispyesnoflag = 0;
                                    menu_sDispBattleShop();
                                    continue;
                                }
                                dlgpartsno = IveSave[0].list_parts[menusetcl[3]];
                                if (IveSave[0].money >= Tea_partsdat[dlgpartsno].price) {
                                    SeRet();
                                    menudispyesnopoint = 1;
                                    menufncShopyesno(1);
                                    if (menudispyesnopoint == 0)
                                        continue;
                                    IveSave[0].owned_parts[IveSave[0].owned_part_count] = dlgpartsno;
                                    IveSave[0].owned_part_count++;
                                    IveSave[0].money -= Tea_partsdat[dlgpartsno].price;
                                } else {
                                    MessageBoxBatShopNoBp();
                                }
                            }
                            menusetsl[3] = i;
                            menusetst[3] = k;
                            menusetmh[3] = mh;
                            menusetmn[3] = mn;
                            menusetbtanm[8].cur = 6;
                            while (menusetbtanm[8].cur != 0)
                                menu_sDispBattleShop();
                            menuzoidhavepartslist = 0;
                            menudispshophavezoidlist = 2;
                            menusetbtanm[8].cur = 1;
                            SetMenuMsgAdr(0x45, menudispwinstr);
                            detCustm(-2);
                        }
                        menusetbtanm[8].cur = 3;
                        while (menusetbtanm[8].cur != 0)
                            menu_sDispBattleShop();
                        menudispshophavezoidlist = 0;
                        menusetbtanm[7].cur = 10;
                        menusetbtanm[7].state = 1;
                        menu_sDispBattleShop();
                        menumode = 2;
                        menuZoidBattleShopmenubuttonset();
                        detCustm(-1);
                    } else if (menumodeShop == 2) {
                        menuPartsHaveListZoid(2, -1);
                        if (IveSave[0].list_count == 0) {
                            MessageBoxBatShopNoParts();
                            continue;
                        }
                        SeRet();
                        menusetbtanm[7].cur = 8;
                        menusetbtanm[7].state = 1;
                        while (menusetbtanm[7].cur != 9)
                            menu_sDispBattleShop();
                        menusetbtanm[6].cur = 3;
                        menusetbtanm[2].cur = 3;
                        menusetcl[3] = menusetsl[3] = 0;
                        menusetst[3] = 0;
                        menusetmh[3] = 5;
                        menusetmn[3] = IveSave[0].list_count;
                        if (menusetmn[3] < 5)
                            menusetmh[3] = menusetmn[3];
                        menuzoidhavepartslist = 2;
                        menusetbtanm[8].cur = 7;
                        while (menusetbtanm[8].cur != 8)
                            menu_sDispBattleShop();
                        for (;;) {
                            s32 count2;
                            s16 slot2;
                            sPadAll();
                            menu_sDispBattleShop();
                            menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                            if (sPad1->field_5C)
                                flag_AllHavePartsList++;
                            if (sPad1->field_5E) {
                                SeCancel();
                                break;
                            }
                            detOption(IveSave[0].list_parts[menusetcl[3]]);
                            if (!sPad1->pressed_a)
                                continue;
                            if (IveSave[0].list_parts[menusetcl[3]] <= 0 || IveSave[0].list_count <= 0) {
                                SeCancel();
                                continue;
                            }
                            SeRet();
                            menudispyesnopoint = 1;
                            menufncShopyesno(0);
                            if (menudispyesnopoint == 0)
                                continue;
                            count2 = menusetcl[3];
                            sel = menumode;
                            slot2 = IveSave[0].list_flag[menusetcl[3]];
                            dlgpartsno = IveSave[0].list_parts[menusetcl[3]];
                            IveSave[0].money += Tea_partsdat[dlgpartsno].field_54;
                            menuDelPartsHave(dlgpartsno);
                            menuSortPartsHave();
                            menuPartsHaveListZoid(2, -1);
                            menusetmn[3] = IveSave[0].list_count;
                            if (menusetmn[3] < menusetmh[3])
                                menusetmh[3] = menusetmn[3];
                            if (menusetmn[3] - 1 < menusetcl[3])
                                menusetsl[3] = menusetmn[3] - 1;
                            if (menusetmn[3] - 1 < menusetcl[3] && menusetst[3] > 0)
                                menusetst[3]--;
                            if (menusetmh[3] - 1 < menusetsl[3])
                                menusetsl[3] = menusetmh[3] - 1;
                            if (IveSave[0].list_count == 0)
                                break;
                        }
                        menusetbtanm[8].cur = 9;
                        while (menusetbtanm[8].cur != 0)
                            menu_sDispBattleShop();
                        menuzoidhavepartslist = 0;
                        shopDispParts(0);
                        menusetbtanm[7].cur = 10;
                        menusetbtanm[7].state = 1;
                        menu_sDispBattleShop();
                        menumode = 2;
                        menuZoidBattleShopmenubuttonset();
                        detCustm(-1);
                    }
                    continue;
                } else if (menusetcl[menumode] == 3) {
                    if (menumodeShop == 1) {
                        if (IveSave[0].owned_zoid_count == 0) {
                            SeCancel();
                            continue;
                        }
                        SeRet();
                        SetMenuMsgAdr(0x46, menudispwinstr);
                        menusetbtanm[7].cur = 11;
                        menusetbtanm[7].state = 1;
                        while (menusetbtanm[7].cur != 12)
                            menu_sDispBattleShop();
                        menusetbtanm[5].cur = 3;
                        menusetbtanm[3].cur = 3;
                        menusetcl[3] = 0;
                        menusetsl[3] = 0;
                        menusetst[3] = 0;
                        menusetmh[3] = 4;
                        menusetmn[3] = IveSave[0].owned_zoid_count;
                        if (menusetmn[3] < 4)
                            menusetmh[3] = menusetmn[3];
                        menudispshophavezoidlist = 2;
                        menusetbtanm[8].cur = 1;
                        menu_sDispBattleShop();
                        for (;;) {
                            sPadAll();
                            menu_sDispBattleShop();
                            menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                            if (sPad1->field_5C)
                                flag_AllHavePartsList++;
                            if (sPad1->field_5E) {
                                SeCancel();
                                break;
                            }
                            detCustm(menusetcl[3]);
                            if (!sPad1->pressed_a)
                                continue;
                            SeRet();
                            dlgzoidno = IveSave[0].owned_zoids[menusetcl[3]];
                            i = menusetsl[3];
                            k = menusetst[3];
                            mh = menusetmh[3];
                            mn = menusetmn[3];
                            menuPartsShopListZoid(3, IveSave[0].owned_zoids[menusetcl[3]]);
                            menusetbtanm[8].cur = 3;
                            while (menusetbtanm[8].cur != 0)
                                menu_sDispBattleShop();
                            menudispshophavezoidlist = 0;
                            menusetcl[3] = menusetsl[3] = 0;
                            menusetst[3] = 0;
                            menusetmh[3] = 5;
                            menusetmn[3] = IveSave[0].list_count;
                            if (menusetmn[3] < 5)
                                menusetmh[3] = menusetmn[3];
                            menuzoidhavepartslist = 3;
                            menusetbtanm[8].cur = 4;
                            while (menusetbtanm[8].cur != 5)
                                menu_sDispBattleShop();
                            for (;;) {
                                sPadAll();
                                menu_sDispBattleShop();
                                menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                                if (sPad1->field_5C)
                                    flag_AllHavePartsList++;
                                if (sPad1->field_5E) {
                                    SeCancel();
                                    break;
                                }
                                detSubparts(IveSave[0].list_parts[menusetcl[3]]);
                                if (!sPad1->pressed_a)
                                    continue;
                                if (IveSave[0].owned_part_count >= 450) {
                                    SeCancel();
                                    menudispyesnoflag = 1;
                                    menudispyesnopoint = 1;
                                    do {
                                        sPadAll();
                                        menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x50), "BUY");
                                        menu_sDispBattleShop();
                                    } while (!sPad1->pressed_a);
                                    menudispyesnoflag = 0;
                                    menu_sDispBattleShop();
                                    continue;
                                }
                                dlgpartsno = IveSave[0].list_parts[menusetcl[3]];
                                if (IveSave[0].money >= Tea_partsdat[dlgpartsno].price) {
                                    SeRet();
                                    menudispyesnopoint = 1;
                                    menufncShopyesno(1);
                                    if (menudispyesnopoint == 0)
                                        continue;
                                    IveSave[0].owned_parts[IveSave[0].owned_part_count] = dlgpartsno;
                                    IveSave[0].owned_part_count++;
                                    IveSave[0].money -= Tea_partsdat[dlgpartsno].price;
                                } else {
                                    MessageBoxBatShopNoBp();
                                }
                            }
                            menusetsl[3] = i;
                            menusetst[3] = k;
                            menusetmh[3] = mh;
                            menusetmn[3] = mn;
                            SetMenuMsgAdr(0x46, menudispwinstr);
                            menusetbtanm[8].cur = 6;
                            while (menusetbtanm[8].cur != 0)
                                menu_sDispBattleShop();
                            menuzoidhavepartslist = 0;
                            menudispshophavezoidlist = 2;
                            menusetbtanm[8].cur = 1;
                            SetMenuMsgAdr(0x46, menudispwinstr);
                            detCustm(-2);
                        }
                        menusetbtanm[8].cur = 3;
                        while (menusetbtanm[8].cur != 0)
                            menu_sDispBattleShop();
                        menudispshophavezoidlist = 0;
                        menusetbtanm[7].cur = 10;
                        menusetbtanm[7].state = 1;
                        menu_sDispBattleShop();
                        menumode = 2;
                        menuZoidBattleShopmenubuttonset();
                        detCustm(-1);
                    } else if (menumodeShop == 2) {
                        menuPartsHaveListZoid(3, -1);
                        if (IveSave[0].list_count == 0) {
                            MessageBoxBatShopNoParts();
                            continue;
                        }
                        SeRet();
                        menusetbtanm[7].cur = 11;
                        menusetbtanm[7].state = 1;
                        while (menusetbtanm[7].cur != 12)
                            menu_sDispBattleShop();
                        menusetbtanm[6].cur = 3;
                        menusetbtanm[3].cur = 3;
                        menusetcl[3] = menusetsl[3] = 0;
                        menusetst[3] = 0;
                        menusetmh[3] = 5;
                        menusetmn[3] = IveSave[0].list_count;
                        if (menusetmn[3] < 5)
                            menusetmh[3] = menusetmn[3];
                        menuzoidhavepartslist = 3;
                        menusetbtanm[8].cur = 7;
                        while (menusetbtanm[8].cur != 8)
                            menu_sDispBattleShop();
                        for (;;) {
                            s32 count2;
                            s16 slot2;
                            sPadAll();
                            menu_sDispBattleShop();
                            menusetcl[3] = TEA_menufnc(&menusetsl[3], &menusetst[3], menusetmn[3], menusetmh[3]);
                            if (sPad1->field_5C)
                                flag_AllHavePartsList++;
                            if (sPad1->field_5E) {
                                SeCancel();
                                break;
                            }
                            detSubparts(IveSave[0].list_parts[menusetcl[3]]);
                            if (!sPad1->pressed_a)
                                continue;
                            if (IveSave[0].list_parts[menusetcl[3]] <= 0 || IveSave[0].list_count <= 0) {
                                SeCancel();
                                continue;
                            }
                            SeRet();
                            menudispyesnopoint = 1;
                            menufncShopyesno(0);
                            if (menudispyesnopoint == 0)
                                continue;
                            count2 = menusetcl[3];
                            sel = menumode;
                            slot2 = IveSave[0].list_flag[menusetcl[3]];

                            dlgpartsno = IveSave[0].list_parts[menusetcl[3]];
                            IveSave[0].money += Tea_partsdat[dlgpartsno].field_54;
                            menuDelPartsHave(dlgpartsno);
                            menuSortPartsHave();
                            menuPartsHaveListZoid(3, -1);
                            menusetmn[3] = IveSave[0].list_count;
                            if (menusetmn[3] < menusetmh[3])
                                menusetmh[3] = menusetmn[3];
                            if (menusetmn[3] - 1 < menusetcl[3])
                                menusetsl[3] = menusetmn[3] - 1;
                            if (menusetmn[3] - 1 < menusetcl[3] && menusetst[3] > 0)
                                menusetst[3]--;
                            if (menusetmh[3] - 1 < menusetsl[3])
                                menusetsl[3] = menusetmh[3] - 1;
                            if (IveSave[0].list_count == 0)
                                break;
                        }
                        menusetbtanm[8].cur = 9;
                        while (menusetbtanm[8].cur != 0)
                            menu_sDispBattleShop();
                        menuzoidhavepartslist = 0;
                        menusetbtanm[7].cur = 10;
                        menusetbtanm[7].state = 1;
                        menu_sDispBattleShop();
                        menumode = 2;
                        menuZoidBattleShopmenubuttonset();
                        detCustm(-1);
                    }
                    continue;
                }
            }
            if (menumode == 3) {
                menusetbtanm[8].cur = 3;
                while (menusetbtanm[8].cur != 0)
                    menu_sDispBattleShop();
                menusetbtanm[8].cur = 0;
                menu_sDispBattleShop();
                menudispshophavezoidlist = 0;
                if (menumodeShop == 1) {
                    dlgzoidno = IveSave[0].zoid_list[menusetcl[3]];
                    if (IveSave[0].money >= Tea_zoiddat[dlgzoidno].field_50) {
                        SeRet();
                        ttl_IveSaveSetZoidDefParts(0, IveSave[0].owned_zoid_count, dlgzoidno);
                        IveSave[0].owned_zoid_count++;
                        change = 1;
                        IveSave[0].money -= Tea_zoiddat[dlgzoidno].field_50;
                        menuanmend = 0;
                        menusetbtanm[7].cur = 4;
                        menusetbtanm[7].state = 1;
                        menu_sDispBattleShop();
                        menumode = 2;
                        SeLiftStart();
                        menu_StageUpDownFlag = 0;
                        menuDetailMode = 0;
                        while (menu_sDispBattleStageDown(1) == 0)
                            menu_sDispBattleShop();
                        SeLiftStop();
                        ttl_WeaponAllUnMount(IveSave[0].field_15F8);
                        ZoidPurge(&Zoid[0]);
                        IveSave[0].zoid_color[IveSave[0].zoid_slot] = 0;
                    } else {
                        MessageBoxBatShopNoBp();
                    }
                }
                if (menumodeShop == 2) {
                    menuanmend = 0;
                    menusetbtanm[7].cur = 4;
                    menusetbtanm[7].state = 1;
                    menu_sDispBattleShop();
                    menumode = 2;
                    if (IveSave[0].owned_zoid_count <= 1) {
                        change = 0;
                        SeCancel();
                    } else {
                        SeRet();
                        i = menusetcl[3];
                        dlgzoidno = IveSave[0].owned_zoids[menusetcl[3]];
                        if (dlgzoidno == IveSave[0].field_15F4)
                            change = 1;
                        else
                            change = 0;
                        IveSave[0].money += *(s32 *)Tea_zoiddat[dlgzoidno].field_54;
                        if (IveSave[0].owned_zoid_count != 0) {
                            if (change == 1) {
                                SeLiftStart();
                                menu_StageUpDownFlag = 0;
                                menuDetailMode = 0;
                                while (!menu_sDispBattleStageDown(1))
                                    menu_sDispBattleShop();
                                SeLiftStop();
                                ZoidWeaponPurge(&Zoid[0]);
                                ttl_WeaponAllUnMount(menusetcl[3]);
                                ZoidPurge(&Zoid[0]);
                            }
                            IveSave[0].zoid_color[IveSave[0].zoid_slot] = 0;
                            IveSave[0].owned_zoid_count--;
                            IveSave[0].owned_zoids[i] = IveSave[0].owned_zoids[IveSave[0].owned_zoid_count];
                            for (k = 0; k < 28; k++)
                                IveSave[0].zoid_parts[i][k] = IveSave[0].zoid_parts[IveSave[0].owned_zoid_count][k];
                            IveSave[0].owned_zoids[IveSave[0].owned_zoid_count] = -1;
                            for (k = 0; k < 10; k++)
                                IveSave[0].zoid_parts[IveSave[0].owned_zoid_count][k] = -1;
                            IveSave[0].field_15F4 = IveSave[0].owned_zoids[0];
                        } else {
                            IveSave[0].owned_zoid_count = 0;
                            IveSave[0].owned_zoids[0] = -1;
                            IveSave[0].field_15F4 = -1;
                        }
                        IveSave[0].zoid_slot = 0;
                    }
                }
                if (change == 1) {
                    if (IveSave[0].field_15F4 != -1) {
                        ZoidSetAnim(&Zoid[0], 0x67);
                        ZoidInit(Zoid, IveSave[0].field_15F4);
                    }
                    ttl_WeaponAllMount(IveSave[0].zoid_slot);
                    SeLiftStart();
                    menu_StageUpDownFlag = 0;
                    menuDetailMode = 0;
                    while (menu_sDispBattleStageDown(0) == 0)
                        menu_sDispBattleShop();
                    SeLiftStop();
                    menuZoidBattleShopmenubuttonset();
                }
            }
            if (menumode == 4) {
                menumode = 2;
                continue;
            }
            if (menumode == 7) {
                menumode = 2;
                continue;
            }
            if (menumode == 6) {
                menumode = 2;
                continue;
            }
            if (menumode == 8) {
                menumode = 2;
                continue;
            }
            if (menumode == 5) {
                menumode = 2;
                continue;
            }
        }
        if (sPad1->field_5E) {
            SeCancel();
            if (menumode == 3) {
                menuanmend = 0;
                menusetbtanm[7].cur = 4;
                menusetbtanm[7].state = 1;
                menu_sDispBattleShop();
                menudispshophavezoidlist = 0;
                menumode = 2;
                menusetcl[2] = 0;
                menusetsl[2] = 0;
                menusetst[2] = 0;
                menuZoidBattleShopmenubuttonset();
                menudispshophavezoidlist = 0;
                menuZoidBattleShopmenubuttonset();
            } else if (menumode == 2) {
                menusetbtanm[10].cur = 0;
                menusetbtanm[10].state = 1;
                menu_sDispBattleShop();
                menuanmend = 0;
                menusetbtanm[7].cur = 14;
                menusetbtanm[7].state = 1;
                menu_sDispBattleShop();
                menumode = 10;
                menuZoidBattleShopmenubuttonset();
            } else if (menumode == 10) {
                menusetbtanm[10].cur = 0;
                menusetbtanm[10].state = 1;
                menu_sDispBattleShop();
                menumode = 1;
                menuZoidBattleShopmenubuttonset();
                puts("EXIT MENU1");
            } else {
                printf("MenuMode=%d \n", menumode);
                menumode = 1;
            }
            continue;
        }
        if (sPad1->field_60) {
            (void)(IveSave[0].owned_zoids[menusetcl[menumode]] + 1);
        }
        menu_sDispBattleShop();
    }
    flPictureEpilogue();
    menusetbtanm[4].cur = 4;
    menusetbtanm[4].state = 1;
    menuHiddenTitle();
    menubackcampos[0] = menubackcambak[0];
    for (i = 0; i < 12; i++) {
        menubackintpos[0] = -6.0 - (menubackintbak[0] - i * 0.5);
        menu_sDispBattleShop();
    }
    menumode = 0;
    menu_sDispBattleShop();
    menubackcampos[0] = menubackcambak[0];
    menubackintpos[0] = menubackintbak[0];
    menu_sDispBattleShop();
    return 205;
}
#undef lit_6733
#define lit_6733 "BUY"
#undef lit_6734
#define lit_6734 "menu set cl =%2d \n"
#undef lit_6735
#define lit_6735 "mine change col=%d \n"
#undef lit_6736
#define lit_6736 "partnar change col=%d \n"
#undef lit_6737
#define lit_6737 "Err 2:etczn=%d cl[3]=%d znum=%d \n"
#undef lit_6738
#define lit_6738 "Err 2:%d\n"
#undef lit_6739
#define lit_6739 "po=%d parts=%d col=%d max=%d \n"
#undef lit_6740
#define lit_6740 "EXIT MENU1"
#undef lit_6741
#define lit_6741 "MenuMode=%d \n"

void MessageBoxBatShopNoParts(void)
{
    MessageBoxBatShop(GetMenuMsgAdr(0x68));
}

void MessageBoxBatShopNoBp(void)
{
    MessageBoxBatShop(GetMenuMsgAdr(0x67));
}

void MessageBoxBatShop(char *msg)
{
    MessageBox(msg);
    SeConfirm();
    do {
        sPadAll();
        menu_sDispBattleShop();
    } while (sPad1->pressed_a == 0 && sPad1->field_5E == 0);
    SeOn(0x6F);
    menudispyesnoflag = 0;
}

void ttl_batshop_detailsetparam(void)
{
    if (menumode == 1)
        detCustm(IveSave[0].zoid_slot);
    if (menumode == 2)
        detCustm(IveSave[0].zoid_slot);
    if (menumode == 10)
        detCustm(IveSave[0].zoid_slot);
}

void menu_sDispBattleHumanEntry(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    s32 e = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj)) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(2);
        menu_sDispBattleFarmBack();
        menu_sDispBattleFarmZoid();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj)) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        menu_sDispBattleFarmTitle();
        if (menumode != -1) {
            menu_sDispBattleFarmTitleSubButton();
            menu_sDispBattleHumanEntryPersonPartner();
            menu_sDispBattleHumanEntryPersonPartnerZoidPic();
            menu_sDispBattleHumanEntryPersonZoidStatus();
            menu_sDispBattleHumanEntryPilotZoidList();
            menu_sDispHumanEntryPersonStatus();
            menu_sDispBattleHumanEntryPersonPartnerZoidSelectFrame();
        }
        menu_sDispDetailAll();
        menu_sDispYesNoMenu();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}

void menu_sDispHumanEntryPersonStatus(void)
{
    s32 s0;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 mode = 0;
    s32 idx = 4;
    char text[0x14];

    s0 = 0;
    s1 = 0;
    s2 = 2;
    s3 = 0;
    mode = 0;
    if (((MenuArchive *)menu1p)->joints[idx] != NULL) {
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 0, -1, -1));
        if ((u8)menuZoidBattleModeSingleTeam == 1) {
            mode = 1;
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 0, -1, -1));
        }
        mode = 0;
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 0, 0, -1), 1, Tea_persondat[IveSave[0].field_1600].name);
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 6, -1, -1));
        sprintf(text, lit_3584, *(s16 *)&Tea_persondat[IveSave[0].field_1600].name[0x22]);
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 6, 0, -1), 2, text);
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 7, -1, -1));
        sprintf(text, lit_3584, *(s16 *)&Tea_persondat[IveSave[0].field_1600].name[0x20]);
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 7, 0, -1), 2, text);
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 8, -1, -1));
        sprintf(text, lit_3584, *(s16 *)&Tea_persondat[IveSave[0].field_1600].name[0x24]);
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 8, 0, -1), 2, text);
        if ((u8)menuZoidBattleModeSingleTeam == 1) {
            mode = 1;
            putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 0, 0, -1), 1, Tea_persondat[IveSave[0].field_1602].name);
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 6, -1, -1));
            sprintf(text, lit_3584, *(s16 *)&Tea_persondat[IveSave[0].field_1602].name[0x22]);
            putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 6, 0, -1), 2, text);
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 7, -1, -1));
            sprintf(text, lit_3584, *(s16 *)&Tea_persondat[IveSave[0].field_1602].name[0x20]);
            putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 7, 0, -1), 2, text);
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 8, -1, -1));
            sprintf(text, lit_3584, *(s16 *)&Tea_persondat[IveSave[0].field_1602].name[0x24]);
            putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, mode, 8, 0, -1), 2, text);
        }
    }
}

void menu_sDispBattleHumanEntryPersonPartnerZoidPic(void)
{
    s16 idx = 0;
    HSD_JObj *j1 = 0;
    s32 u18 = 0;
    s32 u14 = 0;
    s16 u10 = 0;
    HSD_JObj *j2 = 0;
    s16 kind = 0;
    s32 u0c = 5;
    s32 u08 = 2;

    idx = IveSave[0].field_1600;
    if (idx != -1) {
        kind = 0;
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[4], 0, kind, 9, -1, -1));
        j1 = GetChild(((MenuArchive *)menu1p)->joints[4], 0, 0, 1, -1, -1);
        j2 = GetChild(((u32)menu_personp[idx] != 0) ? *(HSD_JObj **)((u8 *)menu_personp[idx] + 0x3c) : 0, 0, 0, -1, -1, -1);
        jobjCopyPos(j1, j2);
        jobjDispAll(j2);
    }
    if ((u8)menuZoidBattleModeSingleTeam == 1) {
        idx = IveSave[0].field_1602;
        if (idx != -1) {
            kind = 1;
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[4], 0, kind, 9, -1, -1));
            j1 = GetChild(((MenuArchive *)menu1p)->joints[4], 0, 1, 1, -1, -1);
            j2 = GetChild(((u32)menu_personp[idx] != 0) ? *(HSD_JObj **)((u8 *)menu_personp[idx] + 0x3c) : 0, 0, 0, -1, -1, -1);
            jobjCopyPos(j1, j2);
            jobjDispAll(j2);
        }
    }
}

void menu_sDispBattleHumanEntryPersonPartner(void)
{
    s32 dead1 = 0;
    s32 dead2 = 0;
    s16 i = 0;
    s32 anm = 0;
    s32 flag = 0;
    s32 joint = 3;
    s32 idx = 2;
    u8 tbl[12] = {0, 1, 4, 1, 2, 3, 3, 0, 0, 0, 0, 0};

    for (i = 0; i < 7; i++) {
        idx = i;
        if (menufarmanm[idx] == 7) {
            menumodeanm[idx] = 0;
            menufarmanmold[idx] = menufarmanm[idx];
        }
        if (menufarmanm[idx] != menufarmanmold[idx]) {
            menumodeanm[idx] = 1;
            menufarmanmold[idx] = menufarmanm[idx];
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 0) {
            menufarmanm[idx] = 1;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 1) {
            menumodeanm[idx] = 1;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 3) {
            menumodeanm[idx] = 3;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 4) {
            menumodeanm[idx] = 4;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 5) {
            menumodeanm[idx] = 5;
        }
        if (menumodeanm[idx] == 1) {
            menumodeanm[idx] = 2;
            sSetAnim1(menu1p, joint, GetChild(((MenuArchive *)menu1p)->joints[joint], 0, idx, -1, -1, -1), menufarmanm[idx]);
        }
    }
    anm = 0;
    flag = 0;
    for (i = 0; i < 7; i++) {
        idx = i;
        if (menumodeanm[idx] == 2) {
            if (flag == 0) {
                anm = jobjAnim(((MenuArchive *)menu1p)->joints[joint]);
                flag = 1;
            }
            if (anm != 0) {
                menumodeanm[idx] = 3;
                menuanmend = 1;
            }
        }
    }
    if (menumodeanm[idx] == 3) {
        i = i;
    } else {
    }
    for (i = 0; i < 3; i++) {
        idx = i;
        if (menufarmanm[idx] != 7) {
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[joint], 0, idx, -1, -1, -1));
        }
    }
}

void menu_sDispBattleHumanEntryPersonZoidStatus(void)
{
    s32 unused0 = 0;
    s32 unused1 = 0;
    s32 unused2 = 2;
    s32 unused3 = 0;
    s32 side = 0;
    s32 kind = 4;

    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, 0, 0, -1, -1));
    side = 1;
    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 0, -1, -1));
    side = 0;
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 0, 0, -1), 1, Tea_persondat[IveSave[0].field_1600].name);
    if ((u8)menuZoidBattleModeSingleTeam == 1) {
        side = 1;
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 0, 0, -1), 1, Tea_persondat[IveSave[0].field_1602].name);
    } else {
        side = 1;
        putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[kind], 0, side, 0, 0, -1), 1, "NO ENTRY");
    }
}
#undef lit_6840
#define lit_6840 "NO ENTRY"

void menu_sDispBattleHumanEntryPilotZoidList(void)
{
    HSD_JObj *child1 = 0;
    s32 dead = 0;
    HSD_JObj *child2 = 0;
    s32 k = 4;
    s16 i = 0;

    if (menumode >= 5) {
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[k], 0, 2, -1, -1, -1), 0, 7, 0);
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[k], 0, 2, 0, -1, -1), 0, 7, 0);
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[k], 0, 2, 1, -1, -1), 0, 7, 0);
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[k], 0, 2, 2, -1, -1), 0, 7, 0);
        if (menumode == 5 || menumode == 6) {
            i = 0;
            while (i < menufarmmh[menumode]) {
                menu_sDispBattleHumanEntryPersonListDisp(IveSave[0].persons[i + menufarmst[menumode]], i);
                i++;
            }
        }
        if ((s32)menucuranm[0] == 1) {
            SetAnimArc2((MenuArchive *)menu1p, 5, 0);
            menucuranm[0] = 2;
        }
        if ((s32)menucuranm[0] == 2) {
            if (jobjAnim(((MenuArchive *)menu1p)->joints[5]) != 0)
                menucuranm[0] = 3;
        } else if ((s32)menucuranm[0] == 3) {
            SetAnimArc2((MenuArchive *)menu1p, 5, 0);
            jobjAnim(((MenuArchive *)menu1p)->joints[5]);
            menucuranm[0] = 2;
        }
        child1 = GetChild(((MenuArchive *)menu1p)->joints[4], 0, 2, 4, menufarmsl[menumode], -1);
        child2 = GetChild(((MenuArchive *)menu1p)->joints[5], 0, 1, -1, -1, -1);
        jobjCopyPos(child1, child2);
        HSD_JObjDisp(child2, 0, 7, 0);
        child2 = child2;
    }
}

void menu_sDispBattleHumanEntryPersonListDisp(s16 idx, s16 y)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    char text[0x3C];
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s32 dead1 = 0;
    s32 dead2 = 0;

    dead1 = 0;
    dead2 = 0;
    if (menumode == 5 || menumode == 6) {
        if (idx == -1) {
            return;
        }
        src = GetChild(((MenuArchive *)menu1p)->joints[4], 0, 2, 4, y, -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[4], 0, 2, 3, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDispAll(dst, 0, 7, 0);
        src = GetChild(((MenuArchive *)menu1p)->joints[4], 0, 2, 4, y, -1);
        dst = GetChild(menu_personp[idx] ? ((MenuArchive *)menu_personp[idx])->joints[0] : 0, 0, 1, -1, -1, -1);
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
        sprintf(text, lit_3644, Tea_persondat[idx].name);
        putAscJobj(src, 0, text);
    }
}

void menu_sDispBattleHumanEntryPersonPartnerZoidSelectFrame(void)
{
    s32 a = 0;
    s32 b = 0;
    HSD_JObj *dst = 0;
    HSD_JObj *src = 0;
    s16 x = 0;
    s16 c = 1;
    s16 arr[3] = {1, 0x401, 0x203};
    s16 e = 0;

    if (menumode == 2) {
        if (menufarmcl[menumode] == 2) {
            return;
        }
    }
    if (menumode != 2) {
        return;
    }
    x = menufarmcl[2];
    if ((s32)menucuranm[0] == 1) {
        SetAnimArc2((MenuArchive *)menu1p, 5, 0);
        menucuranm[0] = 2;
    }
    if ((s32)menucuranm[0] == 2) {
        if (jobjAnim(((MenuArchive *)menu1p)->joints[5]) != 0) {
            menucuranm[0] = 3;
        }
    } else if ((s32)menucuranm[0] == 3) {
        SetAnimArc2((MenuArchive *)menu1p, 5, 0);
        jobjAnim(((MenuArchive *)menu1p)->joints[5]);
        menucuranm[0] = 2;
    }
    src = GetChild(((MenuArchive *)menu1p)->joints[4], 0, x, c, -1, -1);
    dst = GetChild(((MenuArchive *)menu1p)->joints[5], 0, 0, -1, -1, -1);
    jobjCopyPos(src, dst);
    HSD_JObjDisp(dst, 0, 7, 0);
}

void menu_BattleSet_HumanEntryPersonPartnerExit(void)
{
    s16 i = 0;

    if (menumode == 2) {
        i = 0;
        for (i = 0; i < 3; i++) {
            menufarmanm[i] = 1;
        }
        if (menufarmcl[2] == 2) {
            menufarmanm[2] = 2;
        }
    } else if (menumode == 3) {
        menufarmanm[0] = 5;
        menufarmanm[1] = 3;
        menufarmanm[2] = 7;
    } else if (menumode == 4) {
        menufarmanm[1] = 5;
        menufarmanm[0] = 3;
        menufarmanm[2] = 7;
    } else if (menumode == 0) {
        menufarmanm[1] = 7;
        menufarmanm[0] = 7;
        menufarmanm[2] = 7;
    } else {
        menufarmanm[2] = 7;
    }
}

s32 menufncZoidBattle_HumanBattleMenuEntry(void)
{
    s32 u0 = 0;
    s32 u1 = 0;
    s32 u2 = -240;
    s32 u3 = 0;
    s32 u4 = 0;
    s32 u5 = 0;
    u8 u6 = 0;
    s32 i = 0;
    s32 d8 = 8;
    s32 d0 = 0;
    s32 d1 = 0;
    s16 x = 0;
    s16 y = 0;

    menucuranm[0] = 1;
    menuCursor = 0;
    menuParticles = 0;
    menumode = 2;
    menuZoidBattleModeSingleTeam = 1;
    menuzoidry = -0.7f;
    IveSave[0].single_team = 1;
    for (i = 0; i < 20; i++) {
        menumodeanm[i] = 0;
    }
    for (i = 0; i < 20; i++) {
        menufarmanm[i] = 0;
    }
    vsyncN = 0;
    TeaMenuBGM(6);
    menuSetTitle(0x65);
    TEA_setvsync();
    menufarmmode = 1;
    menufarmcl[2] = 0;
    menufarmsl[2] = 0;
    menufarmst[2] = 0;
    if ((u8)menuZoidBattleModeSingleTeam == 1) {
        menufarmmh[2] = 2;
        menufarmmn[2] = 2;
    } else if (menuZoidBattleStage != -1) {
        menufarmmh[2] = 1;
        menufarmmn[2] = 1;
    } else {
        menufarmmh[2] = 2;
        menufarmmn[2] = 2;
    }
    menufarmcl[3] = 0;
    menufarmsl[3] = 0;
    menufarmst[3] = 0;
    menufarmmh[3] = 2;
    menufarmmn[3] = 2;
    menufarmcl[4] = 0;
    menufarmsl[4] = 0;
    menufarmst[4] = 0;
    menufarmmh[4] = 2;
    menufarmmn[4] = 2;
    for (i = 5; i <= 8; i++) {
        menufarmcl[i] = 0;
        menufarmsl[i] = 0;
        menufarmst[i] = 0;
        menufarmmh[i] = 8;
        menufarmmn[i] = 2;
    }
    menudispyesnoflag = 0;
    menufarmsl[5] = 0;
    menufarmmn[5] = IveSave[0].person_count;
    if (menufarmmn[5] < menufarmmh[5])
        menufarmmh[5] = menufarmmn[5];
    menufarmst[5] = -1;
    menufarmsl[6] = 0;
    menufarmmn[6] = IveSave[0].person_count;
    if (menufarmmn[6] < menufarmmh[6])
        menufarmmh[6] = menufarmmn[6];
    menufarmst[6] = -1;
    menufarmmh[5] = 8;
    menufarmmh[6] = 8;
    menufarmmn[5] = IveSave[0].person_count;
    menufarmmn[6] = IveSave[0].person_count;
    if (IveSave[0].person_count < 8) {
        menufarmmh[5] = menufarmmh[6] = IveSave[0].person_count;
    }
    IveSave[4].field_15F4 = IveSave[0].field_15F4;
    IveSave[4].field_15FA = IveSave[0].field_15FA;
    x = IveSave[0].field_1600;
    y = IveSave[0].field_1602;
    ttl_HaveSortHuman(0);
    for (i = 0; i < IveSave[0].person_count; i++) {
        if (IveSave[0].field_1600 == IveSave[0].persons[i]) {
            menufarmcl[5] = i;
            menufarmsl[5] = i;
            menufarmst[5] = -1;
            IveSave[0].field_1604 = i;
            break;
        }
    }
    for (i = 0; i < IveSave[0].person_count; i++) {
        if (IveSave[0].field_1602 == IveSave[0].persons[i]) {
            menufarmcl[6] = i;
            menufarmsl[6] = i;
            menufarmst[6] = -1;
            IveSave[0].field_1606 = i;
            break;
        }
    }
    IveSave[0].field_15F4 = IveSave[0].owned_zoids[0];
    IveSave[0].zoid_slot = 0;
    IveSave[0].field_1600 = 0;
    IveSave[0].field_1602 = 1;
    IveSave[0].partner_slot = 1;
    IveSave[0].field_15FA = IveSave[0].owned_zoids[1];
    if (IveSave[0].zoid_slot == 1) {
        IveSave[0].partner_slot = 0;
        IveSave[0].field_15FA = IveSave[0].owned_zoids[0];
    }
    menu_sDispSetButton();
    menu_BattleSet_HumanEntryPersonPartnerExit();
    detSaveNoSet(0);
    for (;;) {
        d8 = 1;
        d0 = 1;
        d1 = 1;
        sPadAll();
        menu_sDispSetButton();
        if (menumode <= 2) {
            menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        }
        if (menumode >= 5) {
            menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        }
        if (menumode == 2 && menufarmcl[2] >= 2) {
            menufarmcl[2] = menufarmsl[2] = 2;
        }
        menu_BattleSet_HumanEntryPersonPartnerExit();
        ttl_batenthuman_detailsetparam();
        menu_sDispBattleHumanEntry();
        if (sPad1->field_5E) {
            SeCancel();
            if (menumode == 2) {
                if (menufarmcl[2] == 1) {
                    SeRet();
                    y = IveSave[0].field_1602;
                    menuZoidBattleModeSingleTeam = 0;
                    IveSave[0].field_1602 = 0x39;
                }
                menufarmcl[menumode] = 2;
                continue;
            } else if (menumode <= 4) {
                menumode = 2;
            } else if (menumode == 5) {
                IveSave[0].field_1600 = x;
                menumode = 3;
                menumode = 2;
            } else if (menumode == 6) {
                IveSave[0].field_1602 = y;
                if ((u8)menuZoidBattleModeSingleTeam == 0)
                    IveSave[0].field_1602 = 0x39;
                menumode = 4;
                menumode = 2;
            }
        }
        if (sPad1->pressed_a) {
            switch (menumode) {
            case 2:
                if (menufarmcl[menumode] == 2) {
                    SeRet();
                    menudispyesnoflag = 1;
                    menudispyesnopoint = 1;
                    do {
                        sPadAll();
                        menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x49), lit_574);
                        menu_sDispBattleHumanEntry();
                    } while (!sPad1->pressed_a);
                    SeRet();
                    menudispyesnoflag = 0;
                    if (menudispyesnopoint == 0)
                        continue;
                    menumode = -1;
                    menufncZoidBattle_BattleMenuReturn();
                    IveSave[0].single_team = menuZoidBattleModeSingleTeam;
                    menuZoidBattleStage = 0;
                    GameMode.win = 1;
                    ttlMenuLoadParts(4);
                    ttlMenuLoadZoid();
                    menuZoidBattleSetStage_PersonSelect(1);
                    return 0xdd;
                } else if (menufarmcl[2] == 1) {
                    SeRet();
                    menumode = 4;
                    menufarmcl[4] = 0;
                    menumode = 6;
                } else if (menufarmcl[2] == 0) {
                    SeRet();
                    menumode = 3;
                    menufarmcl[3] = 0;
                    menumode = 5;
                }
                break;
            case 5:
                SeRet();
                if (IveSave[0].persons[menufarmcl[5]] != IveSave[0].field_1602) {
                    IveSave[0].field_1600 = IveSave[0].persons[menufarmcl[5]];
                    x = IveSave[0].field_1600;
                    TEA_VoiceOn((u8)IveSave[0].field_1600, 1);
                }
                menumode = 2;
                break;
            case 6:
                SeRet();
                if (IveSave[0].persons[menufarmcl[6]] != IveSave[0].field_1600) {
                    IveSave[0].field_1602 = IveSave[0].persons[menufarmcl[6]];
                    y = IveSave[0].field_1602;
                    TEA_VoiceOn((u8)IveSave[0].field_1602, 1);
                } else {
                    menumode = 2;
                    continue;
                }
                menuZoidBattleModeSingleTeam = 1;
                menumode = 2;
                break;
            }
        }
        if (sPad1->field_4F || sPad1->field_53) {
            if ((u8)menuZoidBattleModeSingleTeam == 1) {
                if (menumode == 4) {
                    menumode = 3;
                    menufarmcl[2] = menufarmsl[2] = 0;
                }
            } else if (menumode == 2) {
                if (menufarmcl[2] == 1) {
                    menufarmcl[2] = menufarmsl[2] = 0;
                }
            }
            if (menumode == 3 || menumode == 4)
                SeSelectB();
            if (menumode == 7)
                IveSave[0].field_15F4 = IveSave[0].owned_zoids[menufarmcl[menumode]];
            if (menumode == 8)
                IveSave[0].field_15FA = IveSave[0].owned_zoids[menufarmcl[menumode]];
            continue;
        }
        if (sPad1->field_50 || sPad1->field_54) {
            if (menumode == 2 && menufarmcl[2] >= 2) {
                menufarmcl[2] = menufarmsl[2] = 2;
            }
            if ((u8)menuZoidBattleModeSingleTeam == 1 && menumode == 3) {
                menumode = 4;
                menufarmcl[2] = menufarmsl[2] = 1;
            }
            if (menumode == 3 || menumode == 4)
                SeSelectB();
            if (menumode == 7)
                IveSave[0].field_15F4 = IveSave[0].owned_zoids[menufarmcl[menumode]];
            if (menumode == 8)
                IveSave[0].field_15FA = IveSave[0].owned_zoids[menufarmcl[menumode]];
            continue;
        }
        if (sPad1->field_51 || sPad1->field_55) {
            if (menumode == 2 && menufarmcl[2] == 2) {
                menufarmcl[2] = menufarmsl[2] = 0;
            }
            if (menumode == 3 || menumode == 4) {
                SeSelectB();
                if (menufarmcl[menumode] != 0) {
                    menufarmcl[menumode] = menufarmsl[menumode] = 0;
                }
            }
        }
        if (sPad1->field_52 || sPad1->field_56) {
            if (menumode == 2 && menufarmcl[2] != 2) {
                menufarmcl[2] = menufarmsl[2] = 2;
            }
            if (menumode == 3 || menumode == 4) {
                SeSelectB();
                if (menufarmcl[menumode] == 0) {
                    menufarmcl[menumode] = menufarmsl[menumode] = 1;
                }
            }
        }
    }
}

void ttl_batenthuman_detailsetparam(void)
{
    if (menumode == 2) {
        if (menufarmcl[2] == 0) {
            menuDetailMode = 6;
            detPerson(IveSave[0].field_1600);
        }
        if (menufarmcl[2] == 1) {
            if (IveSave[0].field_1602 != 0x39) {
                menuDetailMode = 6;
                detPerson(IveSave[0].field_1602);
            } else {
                menuDetailMode = 0;
            }
            if ((u8)menuZoidBattleModeSingleTeam == 0)
                menuDetailMode = 0;
        }
        if (menufarmcl[2] == 2)
            menuDetailMode = 0;
    }
    if (menumode == 3) {
        menuDetailMode = 6;
        detPerson(IveSave[0].field_1600);
    }
    if (menumode == 4) {
        menuDetailMode = 6;
        detPerson(IveSave[0].field_1602);
    }
    if (menumode == 5) {
        menuDetailMode = 6;
        detPerson(IveSave[0].persons[menufarmcl[5]]);
    }
    if (menumode == 6) {
        menuDetailMode = 6;
        detPerson(IveSave[0].persons[menufarmcl[6]]);
    }
}
#pragma push
#pragma dont_inline off
extern s32 radarLen[];

static inline void HSD_JObjAddTranslationX(HSD_JObj *jobj, f32 x)
{
    jobj ? (void)0 : __assert(lit_3730, 0x455, lit_3731);
    ((f32 *)jobj)[14] += x;
    if (!(((u32 *)jobj)[5] & 0x2000000)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}

static inline void HSD_JObjAddTranslationY(HSD_JObj *jobj, f32 y)
{
    jobj ? (void)0 : __assert(lit_3730, 0x460, lit_3731);
    ((f32 *)jobj)[15] += y;
    if (!(((u32 *)jobj)[5] & 0x2000000)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}

void menu_sDispBattleMapInfo(u8 *info, u8 *flags, s32 sel, s32 anim, MenuArchive *obj, MenuArchive **arr)
{
    s32 dead = 0;
    GXColor white = { 255, 255, 255, 255 };
    GXColor black = { 0, 0, 0, 0 };
    GXColor green = { 0, 255, 0, 255 };
    GXColor blue = { 0, 0, 0, 255 };
    HSD_JObj *k;
    s32 i;
    HSD_JObj *x;
    s32 kind;
    s32 scale;
    s32 n;
    HSD_JObj *j22;
    HSD_JObj *y;
    HSD_JObj *j19;
    HSD_JObj *base;
    s32 type;
    f32 fx;
    f32 fy;

    j22 = ((MenuArchive *)menu1p)->joints[19];
    j19 = ((MenuArchive *)menu1p)->joints[20];
    x = x;
    y = y;
    HSD_StartRender(0);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(0);
        menu_sDispBattleFarmBack();
        menu_sDispBattleFarmWeapon();
        menu_sDispBattleFarmZoid();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    if (HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        menu_sDispBattleFarmTitle();
        menu_sDispBattleFarmTitleSubButton();
        if (anim == -1) {
            SetAnimArc2((MenuArchive *)menu1p, 0x13, 0);
        }
        if (jobjAnim(j22) != 0) {
            SetAnimArc2((MenuArchive *)menu1p, 0x13, 1);
            jobjAnim(j22);
        }
        sMakeMatrix(j22);
        jobjDispAll(GetChild(j22, 0, 1, -1, -1, -1));
        putAscJobj(GetChild(j22, 0, 1, 0, -1, -1), 4, (char *)info + 0x2a84);
        x = GetChild(j22, 0, 1, 1, -1, -1);
        y = GetChild(j22, 0, 1, 1, 0, -1);
        sPutFontRect((char *)info + 0x28, x, y, white, black, 0, 2);
        jobjDispAll(GetChild(j22, 0, 3, -1, -1, -1));
        x = GetChild(j22, 0, 3, 0, -1, -1);
        y = GetChild(j22, 0, 3, 0, 0, -1);
        sPutFontRect("\x8f\x9f\x97\x98\x8f\xf0\x8c\x8f", x, y, green, blue, 0, 2);
        x = GetChild(j22, 0, 3, 2, -1, -1);
        y = GetChild(j22, 0, 3, 2, 0, -1);
        sPutFontRect((char *)info + 0x8c, x, y, white, black, 0, 2);
        x = GetChild(j22, 0, 3, 1, -1, -1);
        y = GetChild(j22, 0, 3, 1, 0, -1);
        sPutFontRect("\x94s\x96k\x8f\xf0\x8c\x8f", x, y, green, blue, 0, 2);
        x = GetChild(j22, 0, 3, 3, -1, -1);
        y = GetChild(j22, 0, 3, 3, 0, -1);
        sPutFontRect((char *)info + 0xdc, x, y, white, black, 0, 2);
        base = 0;
        if (obj != 0) {
            base = GetChild(j22, 0, 2, -1, -1, -1);
            jobjCopyPosS(base, obj->joints[0]);
            jobjDispAll(obj->joints[0]);
        }
        if (jobjAnim(((MenuArchive *)menu1p)->joints[5]) != 0) {
            SetAnimArc2((MenuArchive *)menu1p, 5, sel == 0 ? 2 : 1);
            jobjAnim(((MenuArchive *)menu1p)->joints[5]);
        }
        sMakeMatrix(((MenuArchive *)menu1p)->joints[5]);
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[5], 0, 2, -1, -1, -1));
        jobjAnim(j19);
        sMakeMatrix(j19);
        for (i = 1; i < 17; i++) {
            k = GetChild(j19, 0, i - 1, -1, -1, -1);
            HiddenParts(k, (s8)0 == (s8)(info + 0x12c)[i * 0x24c]);
            switch ((s8)flags[i]) {
            case 0:
                sSetAnim1(menu1p, 0x14, k, 1);
                flags[i] = 1;
                break;
            case 1:
                if (sel == i) {
                    sSetAnim1(menu1p, 0x14, k, 2);
                    flags[i] = 2;
                }
                break;
            case 2:
                if (sel != i) {
                    sSetAnim1(menu1p, 0x14, k, 1);
                    flags[i] = 1;
                }
                break;
            }
        }
        jobjDispAll(j19);
        if (obj != 0) {
            jobjAnim(obj->joints[1]);
            jobjCopyPosS(base, obj->joints[1]);
            sMakeMatrix(obj->joints[1]);
            for (n = 1; n < 18; n++) {
                char buf[2] = "A";
                i = n;
                if (n == 17)
                    i = sel;
                if (i <= 0)
                    break;
                if ((s8)(info + 0x12c)[i * 0x24c] != 0) {
                    kind = *(s32 *)((info + 0x14c) + i * 0x24c);
                    if (kind > 0) {
                        fx = *(s32 *)((info + 0x1dc) + i * 0x24c);
                        fy = *(s32 *)((info + 0x1e4) + i * 0x24c);
                        if (kind == 1 && (s8)(info + 0x23d)[i * 0x24c] != 0)
                            kind = 4;
                        k = GetChild(obj->joints[1], 0, kind - 1, -1, -1, -1);
                        if (k != 0) {
                            jobjCopyPosS(base, k);
                            scale = radarLen[*(s32 *)(info + 0x2ad4)];
                            if (scale != 0) {
                                HSD_JObjAddTranslationX(k, 2.0 * (12.8 * (fx / (f32)scale)));
                                HSD_JObjAddTranslationY(k, 2.0 * (12.8 * (-fy / (f32)scale)));
                            }
                            HiddenParts(GetChild(k, 0, 0, -1, -1, -1), sel != i);
                            jobjDispAll(k);
                            sMakeMatrix(k);
                            buf[0] = i + 0x40;
                            x = GetChild(k, 0, 0, 0, -1, -1);
                            sMakeMatrix(x);
                            putAscJobj(x, 4, buf);
                        }
                    }
                }
            }
        }
        jobjDisp(GetChild(j22, 0, 4, -1, -1, -1));
        jobjDisp(GetChild(j22, 0, 4, 0, -1, -1));
        x = GetChild(j22, 0, 4, 2, -1, -1);
        y = GetChild(j22, 0, 4, 2, 0, -1);
        sPutFontRect((char *)&(info + 0x12c)[sel * 0x24c], x, y, white, black, 0, 2);
        if (sel != 0) {
            f32 tmp[3];
            k = GetChild(j19, 0, sel - 1, -1, -1, -1);
            for (i = 0; i < 3; i++) {
                tmp[i] = *(f32 *)((u8 *)k + i * 16 + 0x50);
            }
            x = GetChild(j22, 0, 4, 1, -1, -1);
            jobjCopyPos(x, k);
            sGlobal2Local(k);
            sMakeMatrix(k);
            jobjDispAll(k);
            for (i = 0; i < 3; i++) {
                *(f32 *)((u8 *)k + i * 16 + 0x50) = tmp[i];
            }
            sGlobal2Local(k);
            sMakeMatrix(k);
        }
        switch (*(s32 *)((info + 0x14c) + sel * 0x24c)) {
        case 0:
            type = 3;
            break;
        case 1:
        case 3:
        case 4:
            type = 5;
            break;
        case 2:
            type = 3;
            break;
        default:
            type = -1;
            break;
        }
        k = 0;
        if (type != -1) {
            k = GetChild(j22, 0, 4, type, -1, -1);
        }
        if (arr[sel] != 0 && k != 0) {
            jobjCopyPosS(k, arr[sel]->joints[0]);
            if (type == 5) {
                jobjDisp(GetChild(arr[sel]->joints[0], 0, 0, -1, -1, -1));
            } else {
                jobjDispAll(arr[sel]->joints[0]);
            }
        }
        x = GetChild(j22, 0, 4, 4, -1, -1);
        y = GetChild(j22, 0, 4, 4, 0, -1);
        sPutFontRect((char *)&(info + 0x178)[sel * 0x24c], x, y, white, black, 0, 2);
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}
#pragma pop
#undef lit_7146
#define lit_7146 "A"
#undef lit_7221
#define lit_7221 "\x8f\x9f\x97\x98\x8f\xf0\x8c\x8f"
#undef lit_7222
#define lit_7222 "\x94s\x96k\x8f\xf0\x8c\x8f"

void menufnc_MapInfo(s32 mode, s32 map)
{
    char text[0x100];
    u8 flags[0x20];
    MenuArchive *ptrs[30];
    s32 sel = 0;
    s32 prev = -1;
    MenuArchive *arc = 0;
    s32 i;
    s32 dir;
    s32 zoid;
    s8 *info;

    for (i = 0; i < 0x1E; i++) {
        flags[i] = 0;
        ptrs[i] = 0;
    }
    info = (s8 *)rdMapInfo(mode, map);
    if (info == NULL) {
        putErr("map info error");
        OSReport("map info error\n");
        info = (s8 *)rdMapInfo(mode, map = 0x63);
    }
    sprintf(text, "menu/battle_menu/farm/%s.dat", info);
    arc = (MenuArchive *)LoadArc2(text);
    for (i = 0; i < 0x11; i++) {
        if (*(info + 0x150 + i * 0x24C) != 0) {
            sprintf(text, "menu/picture/%s.dat", info + i * 0x24C + 0x150);
            ptrs[i] = (MenuArchive *)LoadArc2(text);
        } else {
            zoid = -1;
            if (*(s32 *)(info + i * 0x24C + 0x14C) == 4) {
                if (IveSave[0].field_1602 < 0) {
                    *(info + 0x12C + i * 0x24C) = 0;
                } else {
                    zoid = IveSave[0].field_15FA;
                }
                if (IveSave[0].field_1602 == 0x39) {
                    *(info + 0x12C + i * 0x24C) = 0;
                }
            }
            if (*(s32 *)(info + i * 0x24C + 0x14C) == 3) {
                zoid = IveSave[0].field_15F4;
            }
            if (zoid != -1) {
                sprintf(text, lit_500, zoid);
                OSReport("zoid =%s %d \n", text, zoid);
                ptrs[i] = (MenuArchive *)LoadArc2(text);
            }
        }
    }
    OSReport("part = %d \n", IveSave[0].field_1602);
    for (;;) {
        dir = 0;
        if (*(info + 0x12C + sel * 0x24C) == 0) {
            dir = 1;
        }
        if (sPad1->field_58 != 0) {
            dir = 1;
        }
        if (sPad1->field_57 != 0) {
            dir = -1;
        }
        if (sPad1->field_5A != 0) {
            dir = 0;
            sel = 0;
        }
        if (sPad1->field_59 != 0) {
            dir = 1;
            sel = 0;
        }
        for (;;) {
            sel = sel + dir;
            if (sel < 0) {
                sel = 0x10;
            }
            if (sel > 0x10) {
                sel = 0;
            }
            if (*(info + 0x12C + sel * 0x24C) != 0) {
                break;
            }
            if (dir == 0) {
                dir = 1;
            }
        }
        menu_sDispBattleMapInfo((u8 *)info, flags, sel, prev, arc, ptrs);
        if (prev != sel && prev != -1) {
            SeSelectB();
        }
        prev = sel;
        sPadAll();
        if (sPad1->field_5E != 0) {
            sel = 0;
        }
        if (sPad1->pressed_a == 0) {
            continue;
        }
        if (sel != 0) {
            continue;
        }
        SeRet();
        break;
    }
    for (i = 0; i < 0x1E; i++) {
        if (ptrs[i] != 0) {
            PurgeArc2(ptrs[i]);
        }
        ptrs[i] = 0;
    }
    if (arc != 0) {
        PurgeArc2(arc);
    }
}
#undef lit_7293
#define lit_7293 "map info error"
#undef lit_7294
#define lit_7294 "map info error\n"
#undef lit_7295
#define lit_7295 "menu/battle_menu/farm/%s.dat"
#undef lit_7296
#define lit_7296 "menu/picture/%s.dat"
#undef lit_7297
#define lit_7297 "zoid =%s %d \n"
#undef lit_7298
#define lit_7298 "part = %d \n"

void menufncZoidBattle_MapInfo(void)
{
    OSReport("=================================\n stage=%d , stage2=%d \n=======================================\n", GameMode.map, GameMode.field_24);
    menufnc_MapInfo(0, GameMode.field_24 + GameMode.map * 3);
}
#undef lit_7300
#define lit_7300 "=================================\n stage=%d , stage2=%d \n=======================================\n"

void menufncZoidMission_MapInfo(void)
{
    menufnc_MapInfo(1, GameMode.map);
}

void loadMapInfo(void)
{
    s32 over;
    s32 mode;
    s32 map;

    if (GameMode.game_type == 1) {
        mode = 0;
        over = GameMode.field_24;
        if (over < 0)
            over = 0;
        if (over > 2)
            over = 2;
        if (GameMode.field_24 != over)
            OSReport("mg  stage*3 + stage2  stage2 over(0,1,2)  is %d \n", GameMode.field_24);
        GameMode.field_24 = over;
        map = GameMode.field_24 + GameMode.map * 3;
    } else {
        mode = 1;
        map = GameMode.map;
    }
    if (GameMode.map != 100)
        rdMapInfo(mode, map);
}
#undef lit_7314
#define lit_7314 "mg  stage*3 + stage2  stage2 over(0,1,2)  is %d \n"
#pragma force_active on
static const char lit_7379[27] = "menu/yokoku/yokoku%02d.dat";
static const f32 lit_7336[1] = { 1.0f };
#pragma force_active off

void menu_sDispBattleNextYokok2(void *data, MenuArchive *archive)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    GXColor white = {255, 255, 255, 255};
    GXColor black = {0, 0, 0, 0};

    if (data != 0 && archive != 0) {
        GXInvalidateVtxCache();
        GXInvalidateTexAll();
        HSD_SetZSortMode(1, 1);
        HSD_SetZSortMode(1, 1);
        HSD_SetZSortMode(0, 0);
        if (HSD_CObjSetCurrent(archive->cobj) != 0) {
            HSD_LObjSetCurrentAll(archive->lobj);
            HSD_LObjSetupInit(archive->cobj);
            jobjDispAll(archive->joints[0]);
            HSD_CObjEndCurrent();
            GXSetCullMode(2);
        }
        HSD_EndRender();
        debugmenudraw();
        vsyncN++;
    }
}

void yokoku2(s32 table)
{
    char buf[0xC8];
    MenuArchive *archive;
    u8 *ent;
    void *mapinfo;
    s32 res;
    s32 i;
    s32 phase;
    s32 timer;
    s32 state;

    state = 0;
    mapinfo = rdMapInfo(0, table);
    sPadAll();
    if (mapinfo != NULL) {
        i = 0;
        phase = 0;
        for (; i < 0x11; i++) {
            if (phase == 2) {
                break;
            }
            ent = (u8 *)mapinfo + 0x12c + i * 0x24c;
            if (*(s32 *)(ent + 0x20) != 1) {
                continue;
            }
            if (*(s8 *)(ent + 0xbc) == 0) {
                continue;
            }
            if ((s32)ent[0x111] == 1) {
                continue;
            }
            enemyChara[phase] = *(s8 *)(ent + 0x110);
            enemyZoid[phase] = *(s32 *)(ent + 0xc0);
            *(s32 *)&((ZoidState *)&((ZoidStateV2 *)Zoid)[phase + 6])->field_0000[0x24] = enemyChara[phase];
            *(s32 *)&((ZoidState *)&((ZoidStateV2 *)Zoid)[phase + 6])->field_0000[0x20] = enemyZoid[phase];
            ((ZoidState *)&((ZoidStateV2 *)Zoid)[phase + 6])->joint = (struct HSD_JObj *)1;
            zdVoiceScript((ZoidState *)&((ZoidStateV2 *)Zoid)[phase + 6]);
            ((ZoidState *)&((ZoidStateV2 *)Zoid)[phase + 6])->joint = 0;
            phase++;
        }
        sprintf(buf, (char *)tst999, table);
        archive = (MenuArchive *)LoadArc2(buf);
        if (archive == 0) {
            sprintf(buf, (char *)tst999, 0);
            archive = (MenuArchive *)LoadArc2(buf);
        }
        if (archive != 0) {
            SetAnimArc2(archive, 0, state);
            jobjAnim(archive->joints[0]);
        }
        GXInvalidateVtxCache();
        GXInvalidateTexAll();
        HSD_SetZSortMode(0, 0);
        HSD_StateInvalidate(-1);
        HSD_SetZSortMode(0, 0);
        gRefresh();
        timer = 0;
        while (1) {
            menu_sDispBattleNextYokok2(mapinfo, archive);
            sPadAll();
            if (archive == 0) {
                continue;
            }
            if (state == 4) {
                timer++;
                if (sPad1->field_5E != 0) {
                    break;
                }
                if (sPad1->pressed_a != 0) {
                    break;
                }
                if (timer > 0xb4) {
                    break;
                }
            }
            if (jobjAnim(archive->joints[0]) == 0) {
                continue;
            }
            if (state == 4) {
                continue;
            }
            state++;
            timer = 0;
            SetAnimArc2(archive, 0, state);
            switch (state) {
            case 1:
            case 2:
                break;
            case 3: {
                if (yokoSeTBL != 0) {
                    SeOn(((u16 *)yokoSeTBL)[table]);
                } else {
                    SeOn(7);
                }
                void *snd = ((ZoidState *)&((ZoidStateV2 *)Zoid)[6])->field_0028;
                if (snd != 0) {
                    VoiceEntry *voice = (VoiceEntry *)binB(snd, 0x63);
                    if (voice != 0) {
                        res = zdVoiceScriptCalc(voice, (ZoidState *)&((ZoidStateV2 *)Zoid)[6], 7, 0);
                        if (res != -1) {
                            camVoiceNow(*(s32 *)&((ZoidState *)&((ZoidStateV2 *)Zoid)[6])->field_0000[0x24], res);
                        }
                    }
                }
                break;
            }
            default:
                break;
            }
            jobjAnim(archive->joints[0]);
        }
        if (yokoSeTBL != 0) {
            SeOn(((u16 *)yokoSeTBL)[36]);
        } else {
            SeOn(0x1d);
        }
        if (archive != 0) {
            PurgeArc2(archive);
        }
    }
}
void ttlMenu_sDispBattleNextYokok(void)
{
    gRefresh();
    gClear();
    sPadAll();
    sPad1->pressed_a = 0;
    enemyChara[0] = -1;
    enemyChara[1] = -1;
    enemyZoid[0] = -1;
    enemyZoid[1] = -1;
    ((ZoidState *)&((ZoidStateV2 *)Zoid)[6])->field_0028 = NULL;
    ((ZoidState *)&((ZoidStateV2 *)Zoid)[7])->field_0028 = NULL;
    yokoSeTBL = sReadFileU("menu/yokoku/se_yokoku.tbl", 0);
    yokoku2(GameMode.field_24 + GameMode.map * 3);
    if (yokoSeTBL != 0)
        free(yokoSeTBL);
    free(((ZoidState *)&((ZoidStateV2 *)Zoid)[6])->field_0028);
    free(((ZoidState *)&((ZoidStateV2 *)Zoid)[7])->field_0028);
    gRefresh();
    gClear();
    OSReport("end . battle yokok \n");
}
#undef lit_7440
#define lit_7440 "menu/yokoku/se_yokoku.tbl"
#undef lit_7441
#define lit_7441 "end . battle yokok \n"

void menu_sDispMisFarm(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    s32 e = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if ((u32)menu_back1p != 0 && HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(2);
        menu_sDispBattleFarmBack();
        menu_sDispBattleFarmZoid();
        menu_sDispBattleFarmWeapon();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if ((u32)menu1p != 0 && HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        if (menumodeanm[0] != 0) {
            menu_sDispBattleFarmTitle();
            menu_sDispBattleFarmTitleSubButton();
            menu_sDispMisFarmModeButton(0, 0);
            menu_sDispBattleFarmPersonZoidPartner();
            menu_sDispSaveLoadMenu();
            menu_sDispMsgWin(3, 1);
            menu_sDispBattleFarmMsgWindowHelp();
            menu_sDispDetailAll();
            menu_sDispYesNoMenu();
        } else {
            ttljobjcopyposdispall(GetChild(((MenuArchive *)menu1p)->joints[1], 0, 2, -1, -1, -1), GetChild(((MenuArchive *)menu1p)->joints[1], 0, 1, 5, -1, -1));
            ttljobjcopyposdispall(GetChild(((MenuArchive *)menu1p)->joints[1], 0, 1, 2, -1, -1), GetChild(((MenuArchive *)menu1p)->joints[1], 0, 4, 7, -1, -1));
        }
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}

void menu_sDispMisFarmModeButton(s16 p0, s16 p1)
{
    s16 limit = 0;
    s32 finished;
    HSD_JObj *source = 0;
    HSD_JObj *target = 0;
    finished = 0;
    s16 i = 0;
    s32 joint_index = 2;
    s32 menu_index = 1;
    u8 animations[6] = {0, 1, 4, 1, 2, 3};

    limit = 9;
    if ((u8)menuZoidBattleModeSingleTeam == 0) {
        limit = 9;
    }
    for (i = 0; i < 20; i++) {
        menufarmjfl[i] = 0;
    }
    for (menu_index = 1; menu_index <= limit; menu_index++) {
        joint_index = menufarmjno[menu_index];
        if (joint_index == -1) {
            continue;
        }
        menufarmjfl[joint_index] = 1;
        if (menufarmanm[menu_index] != menufarmanmold[menu_index]) {
            menumodeanm[menu_index] = 1;
            menufarmanmold[menu_index] = menufarmanm[menu_index];
        }
        if (menumodeanm[menu_index] == 3 && menufarmanm[menu_index] == 0) {
            menufarmanm[menu_index] = 1;
        }
        if (menumodeanm[menu_index] == 3 && menufarmanm[menu_index] == 1) {
            menumodeanm[menu_index] = 1;
        }
        if (menumodeanm[menu_index] == 3 && menufarmanm[menu_index] == 3) {
            menumodeanm[menu_index] = 3;
        }
        if (menumodeanm[menu_index] == 3 && menufarmanm[menu_index] == 4) {
            menumodeanm[menu_index] = 4;
        }
        if (menumodeanm[menu_index] == 3 && menufarmanm[menu_index] == 5) {
            menumodeanm[menu_index] = 5;
        }
        if (menumodeanm[menu_index] == 1) {
            menumodeanm[menu_index] = 2;
            if (menufarmano[menu_index] == 2) {
                sSetAnim1((MenuArchive *)menu1p, joint_index, GetChild(((MenuArchive *)menu1p)->joints[joint_index], 0, 1, -1, -1, -1), animations[menufarmanm[menu_index]]);
            }
            if (menufarmano[menu_index] != 1) {
                sSetAnim1((MenuArchive *)menu1p, joint_index, GetChild(((MenuArchive *)menu1p)->joints[joint_index], 0, menufarmano[menu_index], -1, -1, -1), animations[menufarmanm[menu_index]]);
            }
        }
    }
    for (i = 0; i < 20; i++) {
        if (menufarmjfl[i] == 1) {
            jobjAnim(((MenuArchive *)menu1p)->joints[i]);
        }
    }
    for (menu_index = 1; menu_index <= 9; menu_index++) {
        if (menufarmano[menu_index] == 1) {
            continue;
        }
        joint_index = menufarmjno[menu_index];
        if (joint_index == -1) {
            continue;
        }
        if (menumodeanm[menu_index] == 2) {
            finished = getEndF1(GetChild(((MenuArchive *)menu1p)->joints[joint_index], 0, menufarmano[menu_index], -1, -1, -1));
            if (finished != 0) {
                menumodeanm[menu_index] = 3;
                menuanmend = 1;
            }
        }
        if (menumodeanm[menu_index] == 3) {
            p0 = 0;
            p1 = 0;
        }
        if (menufarmano[menu_index] == 2) {
            source = GetChild(((MenuArchive *)menu1p)->joints[joint_index], 0, 2, -1, -1, -1);
            target = GetChild(((MenuArchive *)menu1p)->joints[joint_index], 0, 1, -1, -1, -1);
            jobjCopyPos(source, target);
            jobjDispAll(target);
        } else if (menufarmano[menu_index] != 6) {
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[joint_index], 0, menufarmano[menu_index], -1, -1, -1));
        }
    }
}

s32 menufncMission_Menu(void)
{
    s32 u0 = 0;
    s32 u1 = 0;
    s32 u2 = -240;
    s16 u3 = 0;
    s32 u4 = 0;
    s32 u5 = 0;
    u8 u6 = 0;
    s32 e0;
    s32 e1;
    s32 i = 0;
    s32 ret = 0;
    s16 bgm = GameMode.field_2C + 22;
    s32 x0;
    s32 x1;
    GXColor color;

    OSGetTime() / (*(u32 *)0x800000F8 / 4);
    x0 = menufarmanm[i];
    menuCursor = 0;
    menuParticles = 0;
    menumode = 0;
    for (i = 0; i < 20; i++) {
        menumodeanm[i] = 0;
    }
    for (i = 0; i < 20; i++) {
        menufarmanm[i] = 2;
        menufarmanmold[i] = 2;
    }
    OSReport("MenuMission\n");
    ZoidPurge(Zoid);
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    if (GameMode.field_2C == 1) {
        bgm = 1;
    } else {
        bgm = 2;
    }
    TeaMenuBGM(bgm);
    debug11();
    vsyncN = 0;
    GameMode.field_28 = menu_MisMapData[GameMode.map].map;
    GameMode.map = IveSave[0].stage;
    IveSave[0].next_stage = IveSave[0].stage;
    GameMode.field_28 = menu_MisMapData[IveSave[0].stage].map;
    menuZoidBattleModeSingleTeam = 0;
    IveSave[0].owned_zoid_count = 0;
    for (i = 0; i < 30; i++) {
        if (IveSave[0].owned_zoids[i] < 0)
            break;
    }
    IveSave[0].owned_zoid_count = i;
    menuSetTitle(0);
    TEA_setvsync();
    menufarmmode = 0;
    menumodeanm[0] = 0;
    menufarmanm[0] = 0;
    menufarmcl[0] = 1;
    menufarmsl[0] = 1;
    menufarmst[0] = 0;
    menufarmmh[0] = 5;
    menufarmmn[0] = 5;
    x0 = x1;
    menufarmcl[1] = 0;
    menufarmsl[1] = 0;
    menufarmst[1] = 0;
    menufarmmh[1] = 1;
    menufarmmn[1] = 1;
    menufarmcl[2] = 0;
    menufarmsl[2] = 0;
    menufarmst[2] = 0;
    menufarmmh[2] = 2;
    menufarmmn[2] = 2;
    menufarmcl[3] = 0;
    menufarmsl[3] = 0;
    menufarmst[3] = 0;
    menufarmmh[3] = 2;
    menufarmmn[3] = 2;
    menufarmcl[4] = 0;
    menufarmsl[4] = 0;
    menufarmst[4] = 0;
    menufarmmh[4] = 2;
    menufarmmn[4] = 2;
    menubackanm[0].state = 1;
    menubackanm[0].cur = 1;
    menubackanm[0].old = 0;
    ZoidPurge(Zoid);
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    HSD_VIWaitXFBFlush();
    IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
    ZoidInitC(Zoid, IveSave[0].field_15F4, IveSave[0].zoid_color[IveSave[0].zoid_slot]);
    ZoidSetAnim(Zoid, 0x67);
    ttl_WeaponAllMount(IveSave[0].zoid_slot);
    OSReport("Now= %d %d %d \n", IveSave[0].field_15F4, IveSave[0].owned_zoids[IveSave[0].zoid_slot], IveSave[0].zoid_slot);
    OSReport(lit_5330, menubackcampos[0], menubackcampos[1], menubackcampos[2]);
    for (i = 1; i <= 9; i++) {
        menufarmanm[i] = 0;
    }
    memcopy(&IveSave[4], &IveSave[0], 0x1a90);
    for (i = 0; i <= 7; i++) {
        menumodeanm[i] = 1;
        menufarmanm[i] = 0;
    }
    ttl_WeaponAllMount(IveSave[0].zoid_slot);
    menuSortPartsHave();
    OSReport("mis-start-disp-00---------------\n");
    menu_sDispSetButton();
    OSReport("mis-start-disp-01---------------\n");
    menuSetTitle(0);
    OSReport("mis-start-disp-02---------------\n");
    menu_TITLE_dispMis(1);
    OSReport("mis-start-disp-03---------------\n");
    menu_sDispMisFarm();
    OSReport("mis-start-disp-04---------------\n");
    detCustm(IveSave[0].zoid_slot);
    detSaveNoSet(0);
    for (;;) {
        bgm = bgm;
        bgm = bgm;
        bgm = bgm;
        x0 = x0;
        x1 = x1;
        sPadAll();
        if (menumodeanm[0] == 0) {
            sPad1->pressed_a = 0;
            sPad1->field_5E = 0;
            sPad1->field_63 = 0;
            sPad1->field_60 = 0;
            sPad1->field_62 = 0;
            sPad1->field_5B = 0;
            sPad1->field_5C = 0;
            sPad1->field_57 = 0;
            sPad1->field_58 = 0;
            sPad1->field_59 = 0;
            sPad1->field_5A = 0;
        }
        menu_sDispSetButton();
        menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        if (sPad1->field_5C) {
            flag_AllHavePartsList++;
            continue;
        }
        if (sPad1->field_61) {
            if (menumodeanm[0] == 0)
                menumodeanm[0] = 1;
            else
                menumodeanm[0] = 0;
            continue;
        }
        if (sPad1->field_57) {
            if (menumode == 0 && menufarmcl[0] == 0) {
                menufarmcl[0] = menufarmsl[0] = 1;
            }
            continue;
        }
        ttl_misfarm_msgsetparam();
        menu_sDispMisFarm();
        if (sPad1->field_62)
            detCustm(IveSave[0].zoid_slot);
        if (sPad1->field_5E) {
            if (menumode > 0) {
                menumode--;
            } else {
                SeCancel();
                continue;
            }
            SeCancel();
            continue;
        }
        if (sPad1->pressed_a) {
            switch (menumode) {
            case 0:
                if (menufarmcl[0] == 4) {
                    SeRet();
                    menudispyesnopoint = 1;
                    menudispyesnoflag = 1;
                    do {
                        sPadAll();
                        menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(0x48), lit_574);
                        menu_sDispMisFarm();
                    } while (!sPad1->pressed_a);
                    puts("SE RET ");
                    SeRet();
                    menudispyesnoflag = 0;
                    if (menudispyesnopoint == 0)
                        continue;
                    if ((u8)menuZoidBattleModeSingleTeam == 1)
                        sCamNN2 = 2;
                    else
                        sCamNN2 = 1;
                    GameMode.map = IveSave[0].stage;
                    IveSave[0].next_stage = IveSave[0].stage;
                    GameMode.field_28 = menu_MisMapData[IveSave[0].stage].map;
                    puts("ZOID BATTLE GO");
                    menuZoidBattleGo();
                    GameMode.field_28 = menu_MisMapData[GameMode.map].map;
                    puts("BGM\x92\xe2\x8e~");
                    TeaBgm(-1);
                    puts("\x83^\x83" "C\x83g\x83\x8b\x8f\xc1\x82\xb7");
                    menu_TITLE_dispMis(0);
                    HSD_VISetBlack(1);
                    for (i = 0; i < 1; i++) {
                        sPadAll();
                        VIWaitForRetrace();
                        HSD_VICopyXFB(0);
                        HSD_VISetXFBDrawDone();
                    }
                    memcopy(&IveSave[4], &IveSave[0], 0x1a90);
                    ttlMenuAllPurge();
                    HSD_VIWaitXFBFlush();
                    HSD_VIWaitXFBFlush();
                    HSD_VIWaitXFBFlush();
                    menuretno = 9999;
                    IveSave[4].stage = IveSave[0].stage;
                    IveSave[4].next_stage = IveSave[0].stage;
                    IveSave[0].map_no = menu_MisMapData[IveSave[0].stage].map;
                    IveSave[4].map_no = menu_MisMapData[IveSave[0].stage].map;
                    GameMode.map = IveSave[0].stage;
                    x1 = OSGetTime() / (*(u32 *)0x800000F8 / 4); x0 = x1; i = 0; x0 = ~-i;
                    rdMapInfo(1, GameMode.map);
                    HSD_VIWaitEFBFlush();
                    HSD_VIWaitEFBFlush();
                    dspLoadF = 1;
                    return 9999;
                } else if (menufarmcl[0] == 1) {
                    SeRet();
                    menu_TITLE_dispMis(0);
                    menuSetTitle(menufarmcl[0] + 1);
                    menufncZoidBattle_BattleMenuSet();
                    menuSetTitle(0);
                    menu_TITLE_dispMis(1);
                } else if (menufarmcl[0] == 2) {
                    SeRet();
                    menu_TITLE_dispMis(0);
                    menuSetTitle(menufarmcl[0] + 2);
                    menuSetTitleButton(10);
                    menufncZoidMission_MapInfo();
                    menuSetTitle(0);
                    menu_TITLE_dispMis(1);
                } else {
                    SeRet();
                    menu_TITLE_dispMis(0);
                    menuSetTitle(menufarmcl[0] + 2);
                    menu_TITLE_dispMis(1);
                    menufarmanm[5] = 5;
                    menumodeanm[5] = 1;
                    menu_sDispSaveLoadMenuSet(-3);
                    while (menudispslwindow[0].anim != 2)
                        menu_sDispMisFarm();
                    menumode = 5;
                    menufarmcl[menumode] = menufarmsl[menumode] = menufarmst[menumode] = 0;
                    menufarmmh[menumode] = 3;
                    menufarmmn[menumode] = 3;
                    menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                    menuSetTitleButton(11);
                    menu_sDispMisFarm();
                    for (;;) {
                        menuDetailMode = 0;
                        sPadAll();
                        menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
                        ttl_misfarm_msgsetparam();
                        menu_sDispMisFarm();
                        if (sPad1->field_5E) {
                            SeCancel();
                            break;
                        }
                        if (sPad1->pressed_a) {
                            SeRet();
                            if (menufarmcl[menumode] != 2) {
                                menu_SlotOn[0] = TEA_mcChkSlotNoMsg(0);
                                if (menu_SlotOn[0] == 0) {
                                    TEA_mcMsg(0, 0x47c);
                                    continue;
                                }
                                memcpy(&IveSaveFlagBak, &IveSaveFlag, 0x1a90);
                                if (!ttlMemCardChkStartNew()) {
                                    memcpy(&IveSaveFlag, &IveSaveFlagBak, 0x1a90);
                                    continue;
                                }
                            }
                            IveSave[0].field_1C87 = 1;
                            if (menufarmcl[menumode] == 0) {
                                ret = ttlMenuBatSaveLoad(1);
                                MissionLoadInitData();
                                if (ret != 0)
                                    OSReport("man=%s zoid=%s stage=%d:%d\n", Tea_persondat[IveSave[0].field_1600].name, Tea_zoiddat[IveSave[0].field_15F4].name, GameMode.map, IveSave[0].stage);
                                memcpy(&IveSaveFlag, &IveSaveFlagBak, 0x1a90);
                                menuSetTitle(menufarmcl[0] + 2);
                                menu_sDispSetButton();
                                menu_sDispSaveLoadMenuSet(-1);
                                menu_sDispMisFarm();
                                menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                                ttl_misfarm_msgsetparam();
                                menu_sDispMisFarm();
                            }
                            if (menufarmcl[menumode] == 1) {
                                ret = ttlMenuBatSaveLoad(0);
                                MissionLoadInitData();
                                if (ret == 1) {
                                    ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                                    ZoidPurge(Zoid);
                                    HSD_VIWaitXFBFlush();
                                    memcopy(&IveSave[0], &IveSave[2], 0x1a90);
                                    GameMode.map = IveSave[0].stage;
                                    IveSave[0].next_stage = GameMode.map;
                                    GameMode.field_28 = menu_MisMapData[IveSave[0].stage].map;
                                    IveSave[4].stage = GameMode.map;
                                    IveSave[4].next_stage = GameMode.map;
                                    IveSave[0].field_1C8D = 0;
                                    GameMode.field_2C = (s8)IveSave[0].country;
                                    ttlMenuAllPurgeMission();
                                    if (GameMode.field_2C == 1) {
                                        bgm = 1;
                                    } else {
                                        bgm = 2;
                                    }
                                    dspLoadF = 1;
                                    TeaMenuBGM(bgm);
                                    ret = IveSave[0].stage;
                                    menubackanm[0].cur = 0;
                                    menubackanm[0].state = 1;
                                    HSD_VIWaitEFBFlush();
                                    dspLoadF = 1;
                                    GameZoid[1].type = IveSave[0].field_15FA;
                                    IveSave[0].field_15FA = -1;
                                    IveSave[0].field_1602 = -1;
                                    ttlMenuLoadBack(6);
                                    ttlMenuLoadZoidBanner();
                                    ttlMenuLoadPersonBanner();
                                    ttlMenuLoadZoid();
                                    OSReport("man=%s zoid=%s stage=%d:%d\n", Tea_persondat[IveSave[0].field_1600].name, Tea_zoiddat[IveSave[0].field_15F4].name, GameMode.map, IveSave[0].stage);
                                    ZoidInitC(Zoid, IveSave[0].field_15F4, IveSave[0].zoid_color[IveSave[0].zoid_slot]);
                                    ZoidSetAnim(Zoid, 0x67);
                                    ttl_WeaponAllMount(IveSave[0].zoid_slot);
                                    rdMapInfo(1, GameMode.map);
                                    menuSetTitle(menufarmcl[0] + 2);
                                    menu_sDispSetButton();
                                    menu_sDispSaveLoadMenuSet(-1);
                                    ttl_misfarm_msgsetparam();
                                    menu_sDispMisFarm();
                                    menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                                    menu_sDispMisFarm();
                                    ttl_misfarm_msgsetparam();
                                    menu_StageUpDownFlag = 0;
                                    while (menu_sDispBattleStageDown(0) == 0)
                                        menu_sDispMisFarm();
                                    menuSortPartsHave();
                                } else {
                                    memcpy(&IveSaveFlag, &IveSaveFlagBak, 0x1a90);
                                }
                            }
                            IveSave[0].field_1C87 = 0;
                            if (menufarmcl[menumode] == 2) {
                                menu_sDispSaveLoadMenuSet(-2);
                                menu_sDispMisFarm();
                                menudispyesnopoint = 1;
                                menudispyesnoflag = 1;
                                do {
                                    sPadAll();
                                    menudispyesnopoint = menuYesNo(menudispyesnopoint, lit_574, GetMenuMsgAdr(100));
                                    menu_sDispMisFarm();
                                } while (!sPad1->pressed_a);
                                SeRet();
                                menudispyesnoflag = 0;
                                if (menudispyesnopoint != 0) {
                                    for (i = 0; i < 1; i++) {
                                        color = (GXColor){ 0, 0, 0, 0 };
                                        HSD_VIData.bg_color = color;
                                        VIWaitForRetrace();
                                        HSD_VICopyXFB(0);
                                        HSD_VISetXFBDrawDone();
                                    }
                                    ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                                    ttlMenuAllPurge();
                                    OSReport(lit_5331);
                                    strcpy(menudispwinstr, lit_574);
                                    menudispyesnoflag = 0;
                                    strcpy(menudispyesnostr1, lit_574);
                                    strcpy(menudispyesnostr2, lit_574);
                                    GameMode.field_38 = 1;
                                    dspLoadF = 1;
                                    resetSkipOp();
                                } else {
                                    menuSetTitle(menufarmcl[0] + 2);
                                    menu_sDispSetButton();
                                    menu_sDispSaveLoadMenuSet(-1);
                                    menu_sDispMisFarm();
                                    menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                                    menu_sDispMisFarm();
                                }
                            }
                        }
                        if (sPad1->field_57)
                            menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                        if (sPad1->field_58)
                            menu_sDispSaveLoadMenuSet(menufarmcl[menumode]);
                    }
                    menumode = 0;
                    menu_sDispSaveLoadMenuSet(-2);
                    menu_TITLE_dispMis(0);
                    menuSetTitle(0);
                    menu_TITLE_dispMis(1);
                }
                break;
            case 1:
                if (menufarmcl[1] != 0)
                    continue;
                SeRet();
                menuanmend = 0;
                OSReport(lit_5332);
                menumode = -1;
                menuHiddenTitle();
                menu_TITLE_dispMis(0);
                OSReport(lit_5333);
                menufarmmode = 1;
                ttl_WeaponAllUnMount(IveSave[0].zoid_slot);
                HSD_VIWaitEFBFlush();
                HSD_VIWaitEFBFlush();
                return 0x69;
            case 2:
                if (menufarmcl[1] == 0)
                    menumode = 3;
                else
                    menumode = 4;
                x0 = menufarmmode;
                TEA_HotReset(0);
                break;
            }
        }
        if (sPad1->field_57 && menumode == 0 && menufarmcl[0] == 0) {
            menufarmcl[0] = menufarmsl[0] = 1;
        }
        x0 = menufarmcl[0];
        if (sPad1->field_59 && menumode == 0) {
            OSReport(lit_5334);
            menumode = 1;
            menufarmcl[menumode] = 0;
            menufarmsl[menumode] = 0;
            menufarmst[menumode] = 0;
        }
        if (sPad1->field_5A && menumode != 0) {
            OSReport(lit_5335);
            if (menumode > 1) {
                menuerasewait(0);
                menuSetTitle(0);
            }
            menumode = 0;
            menufarmmode = 0;
        }
    }
}
#undef lit_7709
#define lit_7709 "MenuMission\n"
#undef lit_7710
#define lit_7710 "Now= %d %d %d \n"
#undef lit_7711
#define lit_7711 "mis-start-disp-00---------------\n"
#undef lit_7712
#define lit_7712 "mis-start-disp-01---------------\n"
#undef lit_7713
#define lit_7713 "mis-start-disp-02---------------\n"
#undef lit_7714
#define lit_7714 "mis-start-disp-03---------------\n"
#undef lit_7715
#define lit_7715 "mis-start-disp-04---------------\n"
#undef lit_7716
#define lit_7716 "SE RET "
#undef lit_7717
#define lit_7717 "ZOID BATTLE GO"
#undef lit_7718
#define lit_7718 "BGM\x92\xe2\x8e~"
#undef lit_7719
#define lit_7719 "\x83^\x83" "C\x83g\x83\x8b\x8f\xc1\x82\xb7"
#undef lit_7720
#define lit_7720 "man=%s zoid=%s stage=%d:%d\n"

void menuMissionGo(void)
{
    s16 i = 0;
    s16 p = 0;

    for (i = 0; i < 10; i++)
        GameZoid[i].type = -1;
    p = IveSave[0].zoid_slot;
    menuZoidBattleGoPartsOptSet_GameZoid(0, 0, p, 0);
    if ((u8)menuZoidBattleModeSingleTeam == 1) {
        menuZoidBattleGoPartsOptSet_GameZoid(1, 0, IveSave[0].partner_slot, 1);
    } else {
        GameZoid[1].type = -1;
        GameZoid[1].field_58 = -1;
        GameZoid[1].color = IveSave[0].zoid_color[p];
        for (i = 0; i < 28; i++) {
            GameZoid[1].mounts[i] = -1;
            GameZoid[1].mount_colors[i] = 0;
        }
    }
    do {
        i = OSGetTime() / (((*(u32 *)0x800000F8) / 4) / 1000) % (s8)IveSave[0].person_count;
        p = IveSave[0].persons[i];
    } while (IveSave[0].field_1600 == p || IveSave[0].field_1602 == p);
    IveSave[2].field_1600 = p;
    switch (IveSave[2].field_1600) {
    case 0:
        IveSave[2].field_15F4 = 3;
        break;
    case 1:
        IveSave[2].field_15F4 = 2;
        break;
    case 2:
        IveSave[2].field_15F4 = 1;
        break;
    case 4:
        IveSave[2].field_15F4 = 5;
        break;
    case 13:
        IveSave[2].field_15F4 = 0;
        break;
    case 17:
        IveSave[2].field_15F4 = 15;
        break;
    case 22:
        IveSave[2].field_15F4 = 23;
        break;
    case 23:
        IveSave[2].field_15F4 = 4;
        break;
    default:
        IveSave[2].field_15F4 = 3;
        break;
    }
    do {
        i = VIGetRetraceCount() % (s8)IveSave[0].person_count;
        p = IveSave[0].persons[i];
        if (IveSave[0].field_1600 == p || IveSave[0].field_1602 == p) {
            i = OSGetTime() / (((*(u32 *)0x800000F8) / 4) / 1000) % (s8)IveSave[0].person_count;
            p = IveSave[0].persons[i];
        } else {
            break;
        }
    } while (IveSave[0].field_1600 == p || IveSave[0].field_1602 == p);
    IveSave[3].field_1600 = p;
    switch (IveSave[3].field_1600) {
    case 0:
        IveSave[3].field_15F4 = 3;
        break;
    case 1:
        IveSave[3].field_15F4 = 2;
        break;
    case 2:
        IveSave[3].field_15F4 = 1;
        break;
    case 4:
        IveSave[3].field_15F4 = 5;
        break;
    case 13:
        IveSave[3].field_15F4 = 0;
        break;
    case 17:
        IveSave[3].field_15F4 = 15;
        break;
    case 22:
        IveSave[3].field_15F4 = 23;
        break;
    case 23:
        IveSave[3].field_15F4 = 4;
        break;
    default:
        IveSave[3].field_15F4 = 3;
        break;
    }
    GameZoid[5].field_58 = IveSave[2].field_1600;
    GameZoid[6].field_58 = IveSave[3].field_1600;
    ttl_GameZoidSetZoidDefParts(5, IveSave[2].field_15F4);
    ttl_GameZoidSetZoidDefParts(6, IveSave[3].field_15F4);
    OSReport("z0=%d z1=%d z5=%d z6=%d \n", GameZoid[0].type, GameZoid[1].type, GameZoid[5].type, GameZoid[6].type);
    OSReport("p0=%d p1=%d p2=%d p3=%d \n", GameZoid[0].field_58, GameZoid[1].field_58, GameZoid[5].field_58, GameZoid[6].field_58);
}
#undef lit_7772
#define lit_7772 "z0=%d z1=%d z5=%d z6=%d \n"
#undef lit_7773
#define lit_7773 "p0=%d p1=%d p2=%d p3=%d \n"
#pragma force_active on
static u32 demoPicMan[395] = {
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x21, 0x21, 0x21, 0x21, 0x21, 
    0x21, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x23, 
    0x23, 0x23, 0x23, 0x23, 0x23, 0x24, 0x24, 0x24, 
    0x24, 0x2B, 0x26, 0x26, 0x27, 0x27, 0x2D, 0x2D, 
    0x2B, 0x2B, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
    0x3F, 0x3F, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
    0x40, 0x40, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 
    0x41, 0x41, 0x2A, 0x2D, 0x29, 0x29, 0x1F, 0x23, 
    0x3F, 0x40, 0x41, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
};
#pragma force_active off

void menu_TITLE_dispMis(s32 mode)
{
    s16 i = 0;

    menuanmend = 0;
    menutitleanm[0].mode = 1;
    for (i = 1; i < 7; i++)
        menutitleanm[i].mode = 1;
    if (mode == 1) {
        menutitleanm[0].state = 0;
        for (i = 1; i < 7; i++)
            menutitleanm[i].state = 0;
    } else {
        menutitleanm[0].state = 2;
        for (i = 1; i < 7; i++)
            menutitleanm[i].state = 2;
    }
    while (menuanmend == 0) {
        menu_sDispSetButton();
        menu_sDispMisFarm();
        i++;
    }
    menuanmend = 0;
}

void MissionLoadInitData(void)
{
    menuZoidBattleModeSingleTeam = 0;
    IveSave[0].single_team = 0;
    IveSave[0].field_15FA = -1;
    IveSave[0].partner_slot = -1;
    IveSave[0].field_1602 = 0x39;
}

void ttl_misfarm_msgsetparam(void)
{
    if (menumode == 0) {
        if (menufarmcl[0] == 1)
            SetMenuMsgAdr(0x32, menudispwinstr);
        if (menufarmcl[0] == 2)
            SetMenuMsgAdr(0x34, menudispwinstr);
        if (menufarmcl[0] == 3)
            SetMenuMsgAdr(0x35, menudispwinstr);
        if (menufarmcl[0] == 4)
            SetMenuMsgAdr(0x37, menudispwinstr);
    }
    if (menumode == 1) {
        if (menufarmcl[1] == 0)
            SetMenuMsgAdr(0x31, menudispwinstr);
    }
    if (menumode == 5)
        SetMenuMsgAdr(menufarmcl[5] + 0x6E, menudispwinstr);
}

void menu_sDispMissionEntry(void)
{
    s32 unused0 = 0;
    s32 unused1 = 0;
    s32 unused2 = 0;
    s32 unused3 = 0;
    s32 unused4 = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(2);
        menu_sDispBattleFarmBack();
        menu_sDispBattleFarmZoid();
        menu_sDispBattleFarmWeapon();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        menu_sDispBattleFarmTitle();
        if (menumode != -1) {
            menu_sDispBattleFarmTitleSubButton();
            menu_sDispMisEntryPersonPartner();
            menu_sDispMisEntryPersonPartnerZoidPic();
            menu_sDispBattleEntryPersonZoidStatus();
            menu_sDispMissionEntryPilotZoidList();
            menu_sDispBattleEntryPersonPartnerZoidSelectFrame();
            menu_sDispDetailAll();
        }
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}

void menu_sDispMissionEntryPilotZoidList(void)
{
    s32 unused0 = 0;
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s32 idx = 6;
    s16 i = 0;

    if (menumode >= 5) {
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 2, -1, -1, -1), 0, 7, 0);
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 2, 0, -1, -1), 0, 7, 0);
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 2, 1, -1, -1), 0, 7, 0);
        HSD_JObjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 2, 2, -1, -1), 0, 7, 0);
        if (menumode == 5 || menumode == 6) {
            for (i = 0; i < menufarmmh[menumode]; i++) {
                menu_sDispBattleEntryPersonListDisp(IveSave[0].persons[i + menufarmst[menumode]], i);
            }
        }
        if (menumode == 7 || menumode == 8) {
            for (i = 0; i < menufarmmh[menumode]; i++) {
                menu_sDispBattleEntryZoidListDisp(IveSave[0].owned_zoids[i + menufarmst[menumode]], i);
            }
        }
        if ((s32)menucuranm[0] == 1) {
            SetAnimArc2((MenuArchive *)menu1p, 7, 0);
            menucuranm[0] = 2;
        }
        if ((s32)menucuranm[0] == 2) {
            if (jobjAnim(((MenuArchive *)menu1p)->joints[7]) != 0) {
                menucuranm[0] = 3;
            }
        } else if ((s32)menucuranm[0] == 3) {
            SetAnimArc2((MenuArchive *)menu1p, 7, 0);
            jobjAnim(((MenuArchive *)menu1p)->joints[7]);
            menucuranm[0] = 2;
        }
        src = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 2, 4, menufarmsl[menumode], -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 1, -1, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDisp(dst, 0, 7, 0);
        dst = dst;
    }
}

void menu_sDispMisEntryPersonPartnerZoidPic(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    s16 e = 0;
    s32 f = 5;
    s32 g = 2;
    s16 pilot = 0;

    pilot = IveSave[0].field_1600;
    if (pilot != -1) {
        ttljobjcopyposdispall(GetChild(((MenuArchive *)menu1p)->joints[6], 0, 0, -1, -1, -1), GetChild(menu_personp[pilot] != 0 ? ((MenuArchive *)menu_personp[pilot])->joints[0] : 0, 0, 0, -1, -1, -1));
    }
    ttljobjcopyposdispall(GetChild(((MenuArchive *)menu1p)->joints[6], 0, 0, 2, -1, -1), GetChild(((MenuArchive *)menu_zoidp[IveSave[0].field_15F4])->joints[0], 0, 0, -1, -1, -1));
}

void menu_sDispMisEntryPersonPartner(void)
{
    s32 x = 0;
    s32 anim = 0;
    s32 flag = 0;
    s32 y = 0;
    s32 k = 5;
    s32 idx = 2;
    s16 i = 0;
    u8 anm[12] = {0, 1, 4, 1, 2, 3, 3, 0, 0, 0, 0, 0};

    for (i = 0; i < 3; i++) {
        idx = i;
        if (menufarmanm[idx] == 7) {
            menumodeanm[idx] = 0;
            menufarmanmold[idx] = menufarmanm[idx];
        }
        if (menufarmanm[idx] != menufarmanmold[idx]) {
            menumodeanm[idx] = 1;
            menufarmanmold[idx] = menufarmanm[idx];
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 0) {
            menufarmanm[idx] = 1;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 1) {
            menumodeanm[idx] = 1;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 3) {
            menumodeanm[idx] = 3;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 4) {
            menumodeanm[idx] = 4;
        }
        if (menumodeanm[idx] == 3 && menufarmanm[idx] == 5) {
            menumodeanm[idx] = 5;
        }
        if (menumodeanm[idx] == 1) {
            menumodeanm[idx] = 2;
            sSetAnim1(menu1p, k, GetChild(((MenuArchive *)menu1p)->joints[k], 0, idx, -1, -1, -1), anm[menufarmanm[idx]]);
        }
    }
    anim = 0;
    flag = 0;
    for (i = 0; i < 3; i++) {
        idx = i;
        if (menumodeanm[idx] == 2) {
            if (flag == 0) {
                anim = jobjAnim(((MenuArchive *)menu1p)->joints[k]);
                flag = 1;
            }
            if (anim != 0) {
                menumodeanm[idx] = 3;
                menuanmend = 1;
            }
        }
    }
    if (menumodeanm[idx] == 3) {
        idx = idx;
    }
    for (i = 0; i < 3; i++) {
        idx = i;
        if (menufarmanm[idx] != 7) {
            jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[k], 0, idx, -1, -1, -1));
        }
    }
}

void menu_MissionSet_EntryPersonPartnerExit(void)
{
    s16 i = 0;

    if (menumode == 2) {
        for (i = 0; i < 3; i++) {
            if (i == menufarmcl[2]) {
                menufarmanm[i] = 4;
            } else {
                menufarmanm[i] = 1;
            }
        }
        if ((u8)menuZoidBattleModeSingleTeam == 0) {
            menufarmanm[1] = 7;
        }
    } else if (menumode == 3) {
        menufarmanm[0] = 5;
        menufarmanm[1] = 3;
        menufarmanm[2] = 3;
        if ((u8)menuZoidBattleModeSingleTeam == 0) {
            menufarmanm[1] = 7;
        }
    } else if (menumode == 4) {
        menufarmanm[1] = 5;
        menufarmanm[0] = 3;
        menufarmanm[2] = 7;
    } else if (menumode == 0) {
        menufarmanm[1] = 7;
        menufarmanm[0] = 7;
        menufarmanm[2] = 7;
    } else {
        menufarmanm[2] = 7;
    }
}

s32 menufncZoidMission_MenuEntry(void)
{
    s32 u20;
    s32 u1c;
    s32 u18;
    s32 u14;
    s32 u10;
    s32 u0c;
    s8 u08;
    s32 i = 0;
    s16 zd1 = 0;
    s32 d1 = 0;
    s16 x = 0;
    s32 d2 = 0;

    u20 = 0;
    u1c = 0;
    u18 = -0xf0;
    u14 = 0;
    u10 = 0;
    u0c = 0;
    u08 = 0;

    menuCursor = 0;
    menuParticles = 0;
    menumode = 3;
    for (i = 0; i < 20; i++) {
        menumodeanm[i] = 0;
    }
    for (i = 0; i < 20; i++) {
        menufarmanm[i] = 0;
    }
    menucuranm[0] = 1;
    vsyncN = 0;
    menuSetTitle(1);
    TEA_setvsync();
    menufarmmode = 1;
    menufarmcl[2] = 0;
    menufarmsl[2] = 0;
    menufarmst[2] = 0;
    if (menuZoidBattleModeSingleTeam == 1) {
        menufarmmh[2] = 2;
        menufarmmn[2] = 2;
    } else {
        menufarmmh[2] = 1;
        menufarmmn[2] = 1;
    }
    menufarmcl[3] = 0;
    menufarmsl[3] = 0;
    menufarmst[3] = 0;
    menufarmmh[3] = 2;
    menufarmmn[3] = 2;
    menufarmcl[4] = 0;
    menufarmsl[4] = 0;
    menufarmst[4] = 0;
    menufarmmh[4] = 2;
    menufarmmn[4] = 2;
    for (i = 5; i <= 8; i++) {
        menufarmcl[i] = 0;
        menufarmsl[i] = 0;
        menufarmst[i] = 0;
        menufarmmh[i] = 6;
        menufarmmn[i] = 2;
    }
    if (GameMode.field_2C == 1 && GameMode.map == 0) {
        for (i = 0; i < 71; i++) {
                d2 = -menufarmcl[3] + -menumode;
        }
    }
    IveSave[0].owned_zoid_count = 0;
                -menumode;
    for (i = 0; i < 30; i++) {
        if (IveSave[0].owned_zoids[i] < 0)
            break;
    }
    IveSave[0].owned_zoid_count = i;
    menufarmsl[5] = 0;
    menufarmmn[5] = IveSave[0].person_count;
    if (menufarmmn[5] < menufarmmh[5])
        menufarmmh[5] = menufarmmn[5];
    menufarmst[5] = -1;
    menufarmsl[6] = 0;
    menufarmmn[6] = IveSave[0].person_count;
    if (menufarmmn[6] < menufarmmh[6])
        menufarmmh[6] = menufarmmn[6];
    menufarmmn[6] = 3;
    menufarmmh[6] = 3;
    menufarmst[6] = -1;
    menufarmsl[7] = IveSave[0].zoid_slot;
    menufarmmn[7] = IveSave[0].owned_zoid_count;
    menufarmmh[7] = 8;
    if (menufarmmn[7] < menufarmmh[7])
        menufarmmh[7] = menufarmmn[7];
    menufarmst[7] = -1;
    menufarmsl[8] = IveSave[0].partner_slot;
    menufarmmn[8] = IveSave[0].owned_zoid_count;
    menufarmmh[8] = 8;
    if (menufarmmn[8] < menufarmmh[8])
        menufarmmh[8] = menufarmmn[8];
    menufarmst[8] = -1;
    OSReport(lit_3940, IveSave[0].owned_zoid_count);
    menufarmmh[6] = 3;
    menufarmmn[5] = IveSave[0].person_count;
    menufarmmn[6] = IveSave[0].person_count;
    IveSave[4].field_15F4 = IveSave[0].field_15F4;
    IveSave[4].field_15FA = IveSave[0].field_15FA;
    zd1 = IveSave[0].field_15F4;
    x = IveSave[0].field_1600;
    menuZoidBattleModeSingleTeam = 0;
    menuZoidBattleStage = -2;
    for (i = 0; i < IveSave[0].person_count; i++) {
        if (IveSave[0].field_1600 == IveSave[0].persons[i]) {
            menufarmcl[5] = (s16)i;
            menufarmsl[5] = (s16)i;
            menufarmst[5] = -1;
            IveSave[0].field_1604 = (s16)i;
            break;
        }
    }
    for (i = 0; i < IveSave[0].person_count; i++) {
        if (IveSave[0].field_1602 == IveSave[0].persons[i]) {
            menufarmcl[6] = (s16)i;
            menufarmsl[6] = (s16)i;
            menufarmst[6] = -1;
            IveSave[0].field_1606 = (s16)i;
            break;
        }
    }
    menu_MissionSet_EntryPersonPartnerExit();
    ttl_WeaponAllMount(IveSave[0].zoid_slot);
    for (;;) {
        sPadAll();
        menu_sDispSetButton();
        if (menumode <= 2) {
            menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        }
        if (menumode >= 5) {
            menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        }
        menu_MissionSet_EntryPersonPartnerExit();
        menu_sDispMissionEntry();
        ttl_misent_detailsetparam();
        if (sPad1->field_5E) {
            SeCancel();
            if (menumode == 2) {
                d1 = -(-(-(-(-(-(-menumode))))));
                continue;
            }
            if (menumode == 3) {
                menumode = 2;
                menufarmcl[2] = menufarmsl[2] = 2;
            } else if (menumode == 5) {
                IveSave[0].field_1600 = x;
                menumode = 3;
            } else if (menumode == 7) {
                IveSave[0].field_15F4 = zd1;
                menumode = 3;
            }
            continue;
        }
        if (sPad1->pressed_a) {
            switch (menumode) {
            case 2:
                if (menufarmcl[menumode] == 2) {
                    SeRet();
                    menumode = -1;
                    menufncZoidBattle_BattleMenuReturn();
                    if (menuZoidBattleStage == -1) {
                        menuZoidBattleStage = 0;
                        GameMode.win = 1;
                    }
                    return 0x68;
                } else if (menufarmcl[2] == 0) {
                    SeRet();
                    menumode = 3;
                    if (menuZoidBattleStage == -1) {
                        menufarmcl[3] = 0;
                        menumode = 5;
                    } else {
                        menufarmcl[3] = 1;
                        IveSave[0].field_15F8 = IveSave[0].zoid_slot;
                        menumode = 7;
                    }
                }
                break;
            case 3:
                SeRet();
                if (menufarmcl[3] == 0) {
                    menumode = 5;
                    IveSave[0].field_1600 = IveSave[0].persons[menufarmcl[menumode]];
                }
                if (menufarmcl[3] == 1) {
                    menumode = 7;
                }
                break;
            case 5:
                SeRet();
                x = IveSave[0].field_1600;
                menumode = 3;
                TEA_VoiceOn((u8)IveSave[0].field_1600, 1);
                menumode = 3;
                break;
            case 7:
                SeRet();
                if (zd1 == IveSave[0].owned_zoids[menufarmcl[menumode]]) {
                    d2 = 0;
                    menumode = 2;
                    continue;
                }
                i = menumode;
                d1 = menufarmcl[3];
                (void)(menufarmcl[3] + 1);
                -(-menufarmcl[3]);
                IveSave[0].field_15F4 = IveSave[0].owned_zoids[menufarmcl[menumode]];
                menumode = 3;
                SeLiftStart();
                menu_StageUpDownFlag = 0;
                menuDetailMode = 0;
                while (menu_sDispBattleStageDown(1) == 0) {
                    menu_sDispMissionEntry();
                }
                SeLiftStop();
                ttl_WeaponAllUnMount(IveSave[0].field_15F8);
                ZoidWeaponPurge(&Zoid[0]);
                ZoidPurge(&Zoid[0]);
                IveSave[0].zoid_slot = menufarmcl[7];
                IveSave[0].field_15F4 = IveSave[0].owned_zoids[IveSave[0].zoid_slot];
                ZoidInitC(&Zoid[0], IveSave[0].field_15F4, IveSave[0].zoid_color[IveSave[0].zoid_slot]);
                ZoidSetAnim((ZoidState *)&Zoid, 0x67);
                ttl_WeaponAllMount(IveSave[0].zoid_slot);
                SeLiftStart();
                menu_StageUpDownFlag = 0;
                menuDetailMode = 0;
                while (menu_sDispBattleStageDown(0) == 0) {
                    menu_sDispMissionEntry();
                }
                SeLiftStop();
                zd1 = IveSave[0].field_15F4;
                dlgzoidno = IveSave[0].field_15F4;
                menumode = 3;
                break;
            }
        }
        if (sPad1->field_4F || sPad1->field_53) {
            if (menuZoidBattleModeSingleTeam == 1) {
                if (menumode == 4) {
                    menumode = 3;
                    menufarmcl[2] = menufarmsl[2] = 0;
                }
            } else if (menumode == 2) {
                if (menufarmcl[2] == 1) {
                    menufarmcl[2] = menufarmsl[2] = 0;
                }
            }
            if (menumode == 3 || menumode == 4)
                SeSelectB();
            if (menumode == 5) {
                IveSave[0].field_1600 = IveSave[0].persons[menufarmcl[menumode]];
            }
        } else if (sPad1->field_50 || sPad1->field_54) {
            if (menumode == 2 && menufarmcl[2] >= 2) {
                menufarmcl[2] = menufarmsl[2] = 2;
            }
            if (menuZoidBattleModeSingleTeam == 1 && menumode == 3) {
                menumode = 4;
                menufarmcl[2] = menufarmsl[2] = 1;
            }
            if (menumode == 3 || menumode == 4)
                SeSelectB();
            if (menumode == 5) {
                IveSave[0].field_1600 = IveSave[0].persons[menufarmcl[menumode]];
                OSReport("person=%d pos=%d \n", IveSave[0].field_1600, menufarmcl[menumode]);
            }
        } else {
            if (sPad1->field_51 || sPad1->field_55) {
                if (menumode == 2 && menufarmcl[2] == 2) {
                    menumode = 3;
                }
                if (menumode == 3 || menumode == 4) {
                    SeSelectB();
                    if (menufarmcl[menumode] != 0) {
                        menufarmcl[menumode] = menufarmsl[menumode] = 0;
                    }
                }
            }
            if (sPad1->field_52 || sPad1->field_56) {
                if (menumode == 2 && menufarmcl[2] != 2) {
                    menufarmcl[2] = menufarmsl[2] = 2;
                }
                if (menumode == 3 || menumode == 4) {
                    SeSelectB();
                    if (menufarmcl[menumode] == 0) {
                        menufarmcl[menumode] = menufarmsl[menumode] = 1;
                    } else if (menufarmcl[menumode] == 1) {
                        menumode = 2;
                        menufarmcl[2] = menufarmsl[2] = 2;
                    }
                }
            }
        }
        i = -(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(menumode)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
        i = -i + -(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(i)))))))))))))))))))))))))))))));
    }
}
#undef lit_8130
#define lit_8130 "person=%d pos=%d \n"

void ttl_misent_detailsetparam(void)
{
    if (menumode == 2) {
        if (menufarmcl[2] == 0)
            menumode = 3;
        menuDetailMode = 0;
        detCustm(-1);
        if (menufarmcl[2] == 2)
            detCustm(IveSave[0].zoid_slot);
    }
    if (menumode == 3) {
        if (menufarmcl[menumode] == 0)
            detPerson(IveSave[0].field_1600);
        if (menufarmcl[menumode] == 1)
            detCustm(IveSave[0].zoid_slot);
    }
    if (menumode == 5)
        detPerson(IveSave[0].field_1600);
    if (menumode == 7)
        detCustm(menufarmcl[7]);
}

void menu_sDispMissionDemo(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    s32 e = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(0, 0);
    if (HSD_CObjSetCurrent(((MenuArchive *)menudemo[0])->cobj)) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menudemo[0])->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menudemo[0])->cobj);
        GXSetCullMode(0);
        if (menudemoend == 1) {
            menu_sDispMissionDemoBack();
            menu_sDispMissionDemoPerson(1);
            menu_sDispMissionDemoPerson(0);
            if (menudemoanmend[0] == 1 && menudemoanmend[1] == 1) {
                menudemocharain = 1;
            }
            menu_sDispMissionDemoMsgArea();
            menu_sDispMissionDemoMsgCursor();
            menu_sDispMissionDemoSelectMenu();
        }
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}

void ttl_demomenumsgset(s16 no)
{
    s8 buf[0x128];
    s32 start = 0;
    s32 end = 0;
    s16 size = 0;
    s16 i = 0;
    s16 n = 0;
    s16 k = 0;

    if (no == 0) {
        start = 0x1f40;
    } else {
        sReadLenPtr("etc/demo/text.bin", &start, (no - 1) * 4, 4);
    }
    sReadLenPtr("etc/demo/text.bin", &end, no * 4, 4);
    size = sReadLenPtr("etc/demo/text.bin", buf, start, end - start);
    for (i = 0; i < size; i++) {
        menudemoselstr1[i] = buf[i];
        if (buf[i] == 0xd) {
            menudemoselstr1[i] = 0;
            n = i;
            break;
        }
    }
    k = n + 2;
    for (i = k; i < size; i++) {
        menudemoselstr2[i - k] = buf[i];
        if (buf[i] == 0xd || i >= size) {
            menudemoselstr2[i - k] = 0;
            break;
        }
    }
    printf("MissionDemoMsg no=%d size=%d [%s]-[%s]\n", no, size, menudemoselstr1, menudemoselstr2);
}
#undef lit_8175
#define lit_8175 "etc/demo/text.bin"
#undef lit_8176
#define lit_8176 "MissionDemoMsg no=%d size=%d [%s]-[%s]\n"
void ttl_demoflagset(s16 fl, s16 dat)
{
    if (fl < 0 || fl >= 50) {
        OSReport("FLAG ERR fl=%d dat=%d\n", fl, dat);
    }
    IveSave[0].system_flags[fl] = dat;
    IveSave[4].system_flags[fl] = dat;
    if (fl == 0 && dat == 100) {
        IveSave[0].stage = dat;
        GameMode.map = dat;
    }
    OSReport("FLAG SET FL=%d D=%d\n", fl, dat);
}
#undef lit_8185
#define lit_8185 "FLAG ERR fl=%d dat=%d\n"
#undef lit_8186
#define lit_8186 "FLAG SET FL=%d D=%d\n"

void menu_sDispMissionDemoSelectMenu(void)
{
    s8 on = -1;
    s8 off = -0x7f;
    s8 alpha = 0;
    s16 i = 0;
    HSD_JObj *c1 = 0;
    HSD_JObj *c2 = 0;

    if (menudemoselect != -1) {
        if (menudemoselstr1[0] != 0 || menudemoselstr2[0] != 0) {
            jobjDisp(GetChild(((MenuArchive *)menudemo[0])->joints[2], 0, 0, -1, -1, -1));
            jobjDisp(GetChild(((MenuArchive *)menudemo[0])->joints[2], 0, 1, -1, -1, -1));
            if (menudemoselect == 1) {
                jobjDisp(GetChild(((MenuArchive *)menudemo[0])->joints[2], 0, 0, 1, -1, -1));
            }
            if (menudemoselect == 0) {
                jobjDisp(GetChild(((MenuArchive *)menudemo[0])->joints[2], 0, 1, 1, -1, -1));
            }
            i = 0;
            if (menudemoselect == i) {
                alpha = on;
            } else {
                alpha = off;
            }
            c1 = GetChild(((MenuArchive *)menudemo[0])->joints[2], 0, i, 0, -1, -1);
            c2 = GetChild(((MenuArchive *)menudemo[0])->joints[2], 0, i, 0, 0, -1);
            sPutFontRect(menudemoselstr1, c1, c2, (GXColor){alpha, alpha, alpha, alpha}, (GXColor){0, 0, 0, 0}, 0, 2);
            i = 1;
            if (menudemoselect == i) {
                alpha = on;
            } else {
                alpha = off;
            }
            c1 = GetChild(((MenuArchive *)menudemo[0])->joints[2], 0, i, 0, -1, -1);
            c2 = GetChild(((MenuArchive *)menudemo[0])->joints[2], 0, i, 0, 0, -1);
            sPutFontRect(menudemoselstr2, c1, c2, (GXColor){alpha, alpha, alpha, alpha}, (GXColor){0, 0, 0, 0}, 0, 2);
        }
    }
}

void menu_sDispMissionDemoMsgCursor(void)
{
    s32 a = 0;
    HSD_JObj *child = 0;

    if (menudemocur != 0 && menudemokeywait != 0 && menudemocharain != 0) {
        child = GetChild(((MenuArchive *)menudemo[0])->joints[1], 0, -1, -1, -1, -1);
        if (menudemocur == 1) {
            SetAnimArc2((MenuArchive *)menudemo[0], 1, 0);
            menudemocur = 2;
        }
        if (menudemocur == 2)
            jobjAnim(((MenuArchive *)menudemo[0])->joints[1]);
        jobjDispAll(child);
    }
}

void menu_sDispMissionDemoMsgArea(void)
{
    register HSD_JObj *end = 0;
    HSD_JObj *start = 0;

    jobjDispAll(GetChild(((MenuArchive *)menudemo[0])->joints[0], 0, 1, (s8)IveSave[0].country - 1, -1, -1));
    jobjDisp(GetChild(((MenuArchive *)menudemo[0])->joints[0], 0, -1, -1, -1, -1));
    jobjDispAll(GetChild(((MenuArchive *)menudemo[0])->joints[0], 0, 0, -1, -1, -1));
    if (menudemocharain == 1 && menudispwinstr[0] != 0) {
        start = GetChild(((MenuArchive *)menudemo[0])->joints[0], 0, 0, 0, -1, -1);
        end = GetChild(((MenuArchive *)menudemo[0])->joints[0], 0, 0, 2, 0, -1);
        sPutFontRect(menudispwinstr, start, end, (GXColor){255, 255, 255, 255}, (GXColor){0, 0, 0, 0}, 0, 2);
    }
}

void menu_sDispMissionDemoBack(void)
{
    register s32 dead = 0;

    if (menudemobg != -1)
        jobjDispAll(GetChild(((MenuArchive *)menudemo[3])->joints[0], 0, menudemobg, -1, -1, -1));
}
#pragma push
#pragma dont_inline off

static inline void HSD_JObjSetScaleX(HSD_JObj *jobj, f32 x)
{
    jobj ? (void)0 : __assert(lit_3730, 0x325, lit_3731);
    ((f32 *)jobj)[11] = x;
    if (!(((u32 *)jobj)[5] & 0x2000000)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}

static inline void HSD_JObjSetScaleXP(HSD_JObj *jobj, const f32 *x)
{
    jobj ? (void)0 : __assert(lit_3730, 0x325, lit_3731);
    ((f32 *)jobj)[11] = *x;
    if (!(((u32 *)jobj)[5] & 0x2000000)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}

void menu_sDispMissionDemoPerson(s16 no)
{
    s32 unused = 0;
    s32 cull;
    s16 dead = 0;
    HSD_JObj *name = 0;
    HSD_JObj *obj = 0;
    HSD_JObj *root = 0;
    HSD_JObj *jobj = 0;
    s32 idx = 0;
    s32 done = 0;
    s32 anim;
    s32 y;
    s32 x;
    s32 pic;
    s32 end;

    if (no == 0) {
        idx = *(s16 *)(Tea_demomsg + menudemoC * 0x16 + 2);
    } else {
        idx = *(s16 *)(Tea_demomsg + menudemoC * 0x16 + 6);
    }
    if (idx == -1) {
        menudemoanmend[no] = 1;
        return;
    }
    root = GetChild(((MenuArchive *)menudemo[2])->joints[no], 0, -1, -1, -1, -1);
    jobj = ((MenuArchive *)menudemo[1])->joints[0];
    obj = GetChild(jobj, 0, idx, -1, -1, -1);
    if (((s16 (*)[3])menudemocha)[no][1] == ((s16 (*)[3])menudemocha)[no][2]) {
        (void)0;
    }
    if (((s16 (*)[3])menudemocha)[no][0] == 1) {
        s32 tbl[6] = {3, 2, 0, 1, 1, 2};

        y = ((s16 (*)[3])menudemocha)[no][1];
        x = ((s16 (*)[3])menudemocha)[no][1];
        switch (((s16 (*)[3])menudemocha)[no][1]) {
        case 0:
            break;
        case 1:
            x = 2;
            break;
        case 2:
            break;
        case 3:
            x = 2;
            break;
        case 4:
            y = 2;
            break;
        case 5:
            y = 2;
            break;
        }
        SetAnimArc2((MenuArchive *)menudemo[2], no, y);
        SetAnimArc2((MenuArchive *)menudemo[1], 0, tbl[x]);
        ((s16 (*)[3])menudemocha)[no][0] = 2;
    }
    if (((s16 (*)[3])menudemocha)[no][0] == 2) {
        anim = jobjAnim(((MenuArchive *)menudemo[2])->joints[no]);
        end = jobjAnim(jobj);
        switch (((s16 (*)[3])menudemocha)[no][1]) {
        case 0:
            done = anim;
            break;
        case 1:
            done = anim;
            break;
        case 2:
            done = anim;
            break;
        case 3:
            done = anim;
            break;
        case 4:
            done = end;
            break;
        case 5:
            done = end;
            break;
        }
        if (done != 0) {
            menudemoanmend[no] = 1;
            ((s16 (*)[3])menudemocha)[no][0] = 3;
            switch (((s16 (*)[3])menudemocha)[no][1]) {
            case 0:
                return;
            case 1:
                ((s16 (*)[3])menudemocha)[no][1] = 2;
                ((s16 (*)[3])menudemocha)[no][0] = 2;
                break;
            case 3:
                ((s16 (*)[3])menudemocha)[no][1] = 0;
                ((s16 (*)[3])menudemocha)[no][0] = 2;
                break;
            case 4:
                ((s16 (*)[3])menudemocha)[no][1] = 0;
                ((s16 (*)[3])menudemocha)[no][0] = 2;
                break;
            case 5:
                ((s16 (*)[3])menudemocha)[no][1] = 2;
                ((s16 (*)[3])menudemocha)[no][0] = 2;
                break;
            default:
                ((s16 (*)[3])menudemocha)[no][0] = 2;
                break;
            }
            if (((s16 (*)[3])menudemocha)[no][1] == 0) {
                SetAnimArc2((MenuArchive *)menudemo[2], no, 0);
            } else {
                SetAnimArc2((MenuArchive *)menudemo[2], no, 2);
                SetAnimArc2((MenuArchive *)menudemo[1], 0, 0);
            }
            jobjAnim(jobj);
            jobjAnim(((MenuArchive *)menudemo[2])->joints[no]);
        }
    }
    sMakeMatrix(root);
    jobjDisp(root);
    jobjCopyPos(root, jobj);
    sMakeMatrix(jobj);
    if (obj != 0) {
        if (no == 1) {
            HSD_JObjSetScaleX(obj, -1.0f);
            GXGetCullMode(&cull);
            GXSetCullMode(0);
        }
        jobjDispAll(obj);
        if (no == 1) {
            HSD_JObjSetScaleXP(obj, lit_7336);
            GXSetCullMode(cull);
        }
    }
    jobjDisp(GetChild(root, 0, 0, -1, -1, -1));
    pic = demoPicMan[idx];
    name = GetChild(root, 0, 0, 0, -1, -1);
    if (pic >= 0) {
        putAscJobj(name, 0, (char *)&Tea_persondat[pic]);
    }
    (void)name;
    (void)name;
}
#pragma pop

s16 menuDemoSelectMenu(s16 cur)
{
    sPadAll();
    if (sPad1->field_4F || sPad1->field_53) {
        SeSelectB();
        return 0;
    }
    if (sPad1->field_50 || sPad1->field_54) {
        SeSelectB();
        return 1;
    }
    return cur;
}

void menu_MissionDemo(void)
{
    s32 ret = 0;

    menudemop = 0;
    menudemoC = 0;
    menudemocha[1] = -1;
    menudemocha[4] = -1;
    menudemofase[0] = -1;
    menudemofase[1] = -1;
    menudemomsg = -1;
    menudemobg = -1;
    menudemobgm = -1;
    menudemovno = -1;
    menudemokeywait = 0;
    menudemojmp = -1;
    menudemoselmsg[0] = -1;
    menudemoselmsg[1] = -1;
    menudemoseljmp[0] = -1;
    menudemoseljmp[1] = -1;
    menudemocur = 0;
    menudemoanmend[0] = 0;
    menudemoanmend[1] = 0;
    menudemocharain = 0;
    menudemoselect = -1;
    menudemoend = 0;
    menudispwinstr[0] = 0;
    strcpy(menudemoselstr1, "");
    strcpy(menudemoselstr2, "");
    menudemoselstr1[0] = 0;
    menudemoselstr2[0] = 0;
    TeaBgm(0x1E);
    TEA_AudioSeqVol(0, 0);
    menu_MissionDemoLoadScrFile(GameMode.map);
    menudemoselect = -1;
    OSReport("DemoLoadEnd\n");
    OSReport("DemoLoadEnd\n");
    OSReport("DemoLoadEnd\n");
    OSReport("DemoLoadEnd\n");
    OSReport("DemoLoadEnd\n");
    OSReport("DemoLoadEnd\n");
    OSReport("DemoLoadEnd\n");
    menudemoend = 1;
    menudemop = 0;
    menudemoC = 0;
    MenuDemoSkipFlag = 0;
    menu_MissionDemoScrPlay1();

    for (;;) {
        sPadAll();
        menu_sDispMissionDemo();
        if (sPad1->field_63 != 0) {
            if (MenuDemoSkipFlag == 1) {
                MenuDemoSkipFlag = 0;
            } else {
                MenuDemoSkipFlag = 1;
            }
        }
        if (MenuDemoSkipFlag == 1) {
            menudemokeywait = 0;
        }
        if (menudemokeywait == 1) {
            if (sPad1->pressed_a != 0) {
                SeRetDemo();
                if (menudemokeywait == 1 && menudemoanmend[0] == 1 && menudemoanmend[1] == 1) {
                    menudemop = menudemop + 1;
                    ret = menu_MissionDemoScrPlay1();
                    menudemoselect = -1;
                    if (ret == 0) {
                        break;
                    }
                }
            }
        } else {
            if (menudemoanmend[0] == 1 && menudemoanmend[1] == 1) {
                menudemop = menudemop + 1;
                ret = menu_MissionDemoScrPlay1();
                menudemoselect = -1;
                if (ret == 0) {
                    break;
                }
                printf("MissionDemo Loop----------------------------%d %d \n", MenuDemoSkipFlag, menudemokeywait);
            }
        }
    }
    TeaBgm(-1);
    menudemoend = 0;
    menu_sDispMissionDemo();
    HSD_VIWaitEFBFlush();
    OSReport("demo End\n");
}
#undef lit_8348
#define lit_8348 "DemoLoadEnd\n"
#undef lit_8349
#define lit_8349 "MissionDemo Loop----------------------------%d %d \n"
#undef lit_8350
#define lit_8350 "demo End\n"

s32 menu_MissionDemoScrPlay1(void)
{
    s32 start = 0;
    s32 end = 0;
    s16 mode = 0;
    s16 size = 0;
    s16 p = menudemop;

    mode = ((s16 (*)[11])Tea_demomsg)[menudemop][0];
    menudemokeywait = 0;
    menudemocur = 0;
    menudemoanmend[0] = 0;
    menudemoanmend[1] = 0;
    menudemoselect = -1;
    OSReport("demo no=%d mode=%d \n", menudemop, ((s16 (*)[11])Tea_demomsg)[menudemop][0]);
    switch (mode) {
    case -1:
        MenuDemoSkipFlag = 0;
        return 0;
    case -2:
        menudemoend = 1;
        MenuDemoSkipFlag = 0;
        memcpy(Tea_demomenu, ((s16 (*)[11])Tea_demomsg)[menudemop], 22);
        menudemoselmsg[0] = Tea_demomenu[1];
        menudemoseljmp[0] = Tea_demomenu[2];
        ttl_demomenumsgset(menudemoselmsg[0]);
        ((s16 *)&menudemoselflg)[0] = Tea_demomenu[3];
        ((s16 *)&menudemoseldat)[0] = Tea_demomenu[4];
        menudemoselmsg[1] = Tea_demomenu[6];
        menudemoseljmp[1] = Tea_demomenu[7];
        ((s16 *)&menudemoselflg)[1] = Tea_demomenu[8];
        ((s16 *)&menudemoseldat)[1] = Tea_demomenu[9];
        menudemoselect = -2;
        menudemoselect = 0;
        for (;;) {
            menudemoselect = menuDemoSelectMenu(menudemoselect);
            (void)p;
            menu_sDispMissionDemo();
            if (sPad1->pressed_a != 0) {
                if (menudemoselect == -2) {
                    SeBuzzerC();
                } else {
                    SeRet();
                    break;
                }
            }
        }
        if (((s16 *)&menudemoselflg)[menudemoselect] != -1) {
            ttl_demoflagset(((s16 *)&menudemoselflg)[menudemoselect], ((s16 *)&menudemoseldat)[menudemoselect]);
        }
        if (menudemoseljmp[menudemoselect] != -1) {
            menu_MissionDemoLoadScrFile(menudemoseljmp[menudemoselect]);
            printf("JMP Menu Set %d no=%d ------------\n", menudemoselect, menudemoseljmp[menudemoselect]);
            menudemoselect = -1;
            menudemop = -1;
            menudemokeywait = 0;
            menudemocur = 0;
            menudemoanmend[0] = 0;
            menudemoanmend[1] = 0;
            menu_MissionDemoScr_MenuAfter_SetMsg1();
            menudemoselect = -1;
            return 1;
        }
        menudemop = p;
        menudemoselect = -1;
        menudemokeywait = 0;
        return 1;
    case -3:
        MenuDemoSkipFlag = 0;
        memcpy(Tea_demomenu, ((s16 (*)[11])Tea_demomsg)[menudemop], 22);
        OSReport("FLAG JMP FL=%d JMP=%d D=%d\n", Tea_demomenu[3], Tea_demomenu[2], Tea_demomenu[4]);
        menudemoseljmp[0] = Tea_demomenu[2];
        ((s16 *)&menudemoselflg)[0] = Tea_demomenu[3];
        ((s16 *)&menudemoseldat)[0] = Tea_demomenu[4];
        if (((s16 *)&menudemoselflg)[0] != -1) {
            if (IveSave[0].system_flags[((s16 *)&menudemoselflg)[0]] == ((s16 *)&menudemoseldat)[0]) {
                OSReport("FLAG JMP --OK\n");
                if (menudemoseljmp[0] != -1) {
                    menu_MissionDemoLoadScrFile(menudemoseljmp[0]);
                    menudemoselect = -1;
                    menudemop = -1;
                    menudemokeywait = 0;
                    menudemocur = 0;
                    menudemoanmend[0] = 0;
                    menudemoanmend[1] = 0;
                    menu_MissionDemoScr_MenuAfter_SetMsg1();
                    return 1;
                }
            }
        }
        menudemop = p;
        menudemokeywait = 0;
        menudemoanmend[0] = 1;
        menudemoanmend[1] = 1;
        return 1;
    case -4:
        memcpy(Tea_demomenu, ((s16 (*)[11])Tea_demomsg)[menudemop], 22);
        OSReport(lit_8186, Tea_demomenu[3], Tea_demomenu[4]);
        ((s16 *)&menudemoselflg)[0] = Tea_demomenu[3];
        ((s16 *)&menudemoseldat)[0] = Tea_demomenu[4];
        if (((s16 *)&menudemoselflg)[0] != -1) {
            ttl_demoflagset(((s16 *)&menudemoselflg)[0], ((s16 *)&menudemoseldat)[0]);
        }
        menudemop = p;
        menudemokeywait = 0;
        menudemoanmend[0] = 1;
        menudemoanmend[1] = 1;
        return 1;
    default:
        menudemoend = 1;
        menudemoC = menudemop;
        OSReport("\x90l\x95\xa8\x83`\x83" "F\x83\x93\x83W \x8d\xb6:%d \x89" "E:%d \n", ((s16 (*)[11])Tea_demomsg)[menudemop][2], ((s16 (*)[11])Tea_demomsg)[menudemop][4]);
        if (((s16 (*)[11])Tea_demomsg)[menudemop][2] == -1) {
            ((s16 (*)[11])Tea_demomsg)[menudemop][2] = 2;
        }
        if (menudemofase[0] != ((s16 (*)[11])Tea_demomsg)[menudemop][2]) {
            menudemocha[1] = ((s16 (*)[11])Tea_demomsg)[menudemop][2];
            if (menudemocha[1] == -1) {
                menudemocha[1] = 2;
            }
            menudemocha[0] = 1;
            menudemocharain = 0;
        }
        if (((s16 (*)[11])Tea_demomsg)[menudemop][4] == -1) {
            ((s16 (*)[11])Tea_demomsg)[menudemop][4] = 2;
        }
        if (menudemofase[1] != ((s16 (*)[11])Tea_demomsg)[menudemop][4]) {
            menudemocha[4] = ((s16 (*)[11])Tea_demomsg)[menudemop][4];
            if (menudemocha[4] == -1) {
                menudemocha[4] = 2;
            }
            menudemocha[3] = 1;
            menudemocharain = 0;
        }
        menudemobg = ((s16 (*)[11])Tea_demomsg)[menudemop][5];
        if (((s16 (*)[11])Tea_demomsg)[menudemop][6] == -2 || menudemomsg == ((s16 (*)[11])Tea_demomsg)[menudemop][6]) {
            menudemokeywait = 0;
            menudemocharain = 1;
            menudemocur = 0;
        } else {
            menudemomsg = ((s16 (*)[11])Tea_demomsg)[menudemop][6];
            if (((s16 (*)[11])Tea_demomsg)[menudemop][6] != -1) {
                menudemokeywait = 1;
                menudemocur = 1;
                if (menudemomsg == 0) {
                    start = 8000;
                } else {
                    sReadLenPtr((char *)lit_8175, &start, (menudemomsg - 1) * 4, 4);
                }
                sReadLenPtr((char *)lit_8175, &end, menudemomsg * 4, 4);
                size = sReadLenPtr((char *)lit_8175, menudispwinstr, start, end - start);
                printf("no=%d [%d] fadr=%d nextadr=%d size=%d rdsize=%d %s\n", menudemomsg, ((s16 (*)[11])Tea_demomsg)[menudemop][6], start, end, end - start, size, menudispwinstr);
            } else {
                menudemokeywait = 0;
                menudemocur = 0;
            }
        }
        if (((s16 (*)[11])Tea_demomsg)[menudemop][7] == -2) {
            if (((s16 (*)[11])Tea_demomsg)[menudemop][7] < 1000) {
                menudemobgm = ((s16 (*)[11])Tea_demomsg)[menudemop][7];
            } else if (((s16 (*)[11])Tea_demomsg)[menudemop][7] - 1000 != -1) {
                TEA_SeOn(((s16 (*)[11])Tea_demomsg)[menudemop][7] - 1000, 0.0f, 0.0f);
            }
        } else {
            if (((s16 (*)[11])Tea_demomsg)[menudemop][7] < 1000) {
                menudemobgm = ((s16 (*)[11])Tea_demomsg)[menudemop][7];
                TeaBgm(menudemobgm);
                TEA_AudioSeqVol(TEA_BGMVOL, 1);
            } else if (((s16 (*)[11])Tea_demomsg)[menudemop][7] - 1000 != -1) {
                TEA_SeOn(((s16 (*)[11])Tea_demomsg)[menudemop][7] - 1000, 0.0f, 0.0f);
            }
        }
        menudemovno = ((s16 (*)[11])Tea_demomsg)[menudemop][8];
        (void)(menudemovno + 1);
        (void)(menudemovno + 1);
        (void)(menudemop + 1);
        (void)(menudemovno + 1);
        (void)(menudemovno + 1);
        return 1;
    }
}
#undef lit_8416
#define lit_8416 "demo no=%d mode=%d \n"
#undef lit_8417
#define lit_8417 "JMP Menu Set %d no=%d ------------\n"
#undef lit_8418
#define lit_8418 "FLAG JMP FL=%d JMP=%d D=%d\n"
#undef lit_8419
#define lit_8419 "FLAG JMP --OK\n"
#undef lit_8420
#define lit_8420 "\x90l\x95\xa8\x83`\x83" "F\x83\x93\x83W \x8d\xb6:%d \x89" "E:%d \n"
#undef lit_8421
#define lit_8421 "no=%d [%d] fadr=%d nextadr=%d size=%d rdsize=%d %s\n"

void menu_MissionDemoLoadScrFile(s16 no)
{
    s16 dead = 0;
    char path[80];
    void *data;

    sprintf(path, "etc/demo/demo_%03d.bin", no);
    data = sReadFileU(path, 0);
    memcpy(Tea_demomsg, data, 0x898);
    free(data);
    OSReport("=========================================================\n%s", path);
    menudemop = 0;
    menudemocha[1] = -1;
    menudemocha[4] = -1;
    GameMode.map = no;
    IveSave[0].stage = no;
    if (no != 100)
        rdMapInfo(1, no);
    printf("_MissionDemoMsg_Read_End__----------------------_____\n");
}
#undef lit_8425
#define lit_8425 "etc/demo/demo_%03d.bin"
#undef lit_8426
#define lit_8426 "=========================================================\n%s"
#undef lit_8427
#define lit_8427 "_MissionDemoMsg_Read_End__----------------------_____\n"

s32 menu_MissionDemoScr_MenuAfter_SetMsg1(void)
{
    s32 start = 0;
    s32 end = 0;
    s16 mode = 0;
    s16 size = 0;
    s16 p = menudemop;

    menudemop = 0;
    mode = ((s16 (*)[11])Tea_demomsg)[menudemop][0];
    menudemokeywait = 0;
    menudemocur = 0;
    menudemoanmend[0] = 0;
    menudemoanmend[1] = 0;
    menudemoselect = 0;
    OSReport("demoMegReSet no=%d mode=%d \n", menudemop, ((s16 (*)[11])Tea_demomsg)[menudemop][0]);
    switch (mode) {
    case -1:
        MenuDemoSkipFlag = 0;
        return 0;
    case -2:
    case -3:
        menudemoanmend[0] = 1;
        menudemoanmend[1] = 1;
        break;
    case -4:
        memcpy(Tea_demomenu, ((s16 (*)[11])Tea_demomsg)[menudemop], 22);
        OSReport(lit_8186, Tea_demomenu[3], Tea_demomenu[4]);
        ((s16 *)&menudemoselflg)[0] = Tea_demomenu[3];
        ((s16 *)&menudemoseldat)[0] = Tea_demomenu[4];
        if (((s16 *)&menudemoselflg)[0] != -1) {
            ttl_demoflagset(((s16 *)&menudemoselflg)[0], ((s16 *)&menudemoseldat)[0]);
        }
        menudemop = p;
        menudemokeywait = 0;
        menudemoanmend[0] = 1;
        menudemoanmend[1] = 1;
        return 1;
    default:
        menudemoend = 1;
        menudemoC = menudemop;
        OSReport("\x90l\x95\xa8\x83`\x83" "F\x83\x93\x83W \x8d\xb6:%d \x89" "E:%d \n", ((s16 (*)[11])Tea_demomsg)[menudemop][2], ((s16 (*)[11])Tea_demomsg)[menudemop][4]);
        if (((s16 (*)[11])Tea_demomsg)[menudemop][2] == -1) {
            ((s16 (*)[11])Tea_demomsg)[menudemop][2] = 2;
        }
        if (menudemofase[0] != ((s16 (*)[11])Tea_demomsg)[menudemop][2]) {
            menudemocha[1] = ((s16 (*)[11])Tea_demomsg)[menudemop][2];
            if (menudemocha[1] == -1) {
                menudemocha[1] = 2;
            }
            menudemocha[0] = 1;
            menudemocharain = 0;
        }
        if (((s16 (*)[11])Tea_demomsg)[menudemop][4] == -1) {
            ((s16 (*)[11])Tea_demomsg)[menudemop][4] = 2;
        }
        if (menudemofase[1] != ((s16 (*)[11])Tea_demomsg)[menudemop][4]) {
            menudemocha[4] = ((s16 (*)[11])Tea_demomsg)[menudemop][4];
            if (menudemocha[4] == -1) {
                menudemocha[4] = 2;
            }
            menudemocha[3] = 1;
            menudemocharain = 0;
        }
        menudemobg = ((s16 (*)[11])Tea_demomsg)[menudemop][5];
        if (((s16 (*)[11])Tea_demomsg)[menudemop][6] == -2 || menudemomsg == ((s16 (*)[11])Tea_demomsg)[menudemop][6]) {
            menudemokeywait = 0;
            menudemocharain = 1;
            menudemocur = 0;
        } else {
            menudemomsg = ((s16 (*)[11])Tea_demomsg)[menudemop][6];
            if (((s16 (*)[11])Tea_demomsg)[menudemop][6] != -1) {
                menudemokeywait = 1;
                menudemocur = 1;
                if (menudemomsg == 0) {
                    start = 8000;
                } else {
                    sReadLenPtr(lit_8175, &start, (menudemomsg - 1) * 4, 4);
                }
                sReadLenPtr(lit_8175, &end, menudemomsg * 4, 4);
                size = sReadLenPtr(lit_8175, menudispwinstr, start, end - start);
                printf("no=%d [%d] fadr=%d nextadr=%d size=%d rdsize=%d %s\n", menudemomsg, ((s16 (*)[11])Tea_demomsg)[menudemop][6], start, end, end - start, size, menudispwinstr);
            } else {
                menudemokeywait = 0;
                menudemocur = 0;
            }
        }
        if (((s16 (*)[11])Tea_demomsg)[menudemop][7] == -2) {
            if (((s16 (*)[11])Tea_demomsg)[menudemop][7] < 1000) {
                menudemobgm = ((s16 (*)[11])Tea_demomsg)[menudemop][7];
            } else if (((s16 (*)[11])Tea_demomsg)[menudemop][7] - 1000 != -1) {
                TEA_SeOn(((s16 (*)[11])Tea_demomsg)[menudemop][7] - 1000, 0.0f, 0.0f);
            }
        } else {
            if (((s16 (*)[11])Tea_demomsg)[menudemop][7] < 1000) {
                menudemobgm = ((s16 (*)[11])Tea_demomsg)[menudemop][7];
                TeaBgm(menudemobgm);
                TEA_AudioSeqVol(TEA_BGMVOL, 1);
            } else if (((s16 (*)[11])Tea_demomsg)[menudemop][7] - 1000 != -1) {
                TEA_SeOn(((s16 (*)[11])Tea_demomsg)[menudemop][7] - 1000, 0.0f, 0.0f);
            }
        }
        mode = (menudemovno = ((s16 (*)[11])Tea_demomsg)[menudemop][8]) + 1;
        (void)(start + 1);
        (void)(menudemop + 1);
        break;
    }
    return 1;
}
#undef lit_8474
#define lit_8474 "demoMegReSet no=%d mode=%d \n"
namespace { extern const u32 lit_8475[1]; }
namespace { extern const f32 lit_8476[1]; }

s32 menu_sDispMissionHokyu(u8 *info, MenuArchive *arc, s32 done)
{
    s32 unused = 0;
    GXColor white = *(GXColor *)lit_8475;
    GXColor clear = *(GXColor *)lit_8476;
    HSD_JObj *j21;
    HSD_JObj *source;
    s32 cat;
    s32 cnt;
    s32 i;
    HSD_JObj *j28;
    s32 kind;
    s32 rank;
    register HSD_JObj *j30;
    register s32 idx;

    if (info == 0) {
        return 1;
    }
    if (menu1p == 0) {
        return 1;
    }
    j21 = arc->joints[5];
    HSD_SetZSortMode(1, 1);
    HSD_SetZSortMode(1, 1);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(2);
        menu_sDispBattleFarmBack();
        menu_sDispBattleFarmZoid();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if (HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        if (arc != 0) {
            if (jobjAnim(arc->joints[5]) != 0) {
                if (done != 0) {
                    if (anmN == 3) {
                        SetAnimArc2(arc, 5, 0);
                        anmN = 0;
                    } else {
                        SetAnimArc2(arc, 5, 3);
                        anmN = 3;
                    }
                } else {
                    SetAnimArc2(arc, 5, 2);
                    anmN = 2;
                }
            }
            if (anmN != 0) {
                jobjDispAll(arc->joints[5]);
                sMakeMatrix(j21);
                j28 = GetChild(j21, 0, 0, -1, -1, -1);
                putAscJobj(j28, 4, "SUPPLY LIST");
                j28 = GetChild(j21, 0, 1, -1, -1, -1);
                putAscJobj(j28, 4, "NEW PARTS");
                j28 = GetChild(j21, 0, 2, -1, -1, -1);
                putAscJobj(j28, 4, "NEW ZOIDS");
            }
            for (i = 0, cnt = 0; i < 2; i++) {
                idx = *(s32 *)(info + 0x2aac + i * 4);
                if (idx == -1) {
                    continue;
                }
                j30 = GetChild(((MenuArchive *)menu_zoidp[idx])->joints[0], 0, 1, -1, -1, -1);
                source = GetChild(j21, 0, 2, cnt, -1, -1);
                jobjCopyPos(source, j30);
                jobjDispAll(j30);
                *(f32 *)((u8 *)j30 + 0x50) += lit_7336[0];
                putAscJobj(j30, 0, Tea_zoiddat[idx].name);
                cnt++;
            }
            for (i = 0, cnt = 0; i < 8; i++) {
                if (cnt >= 8) {
                    break;
                }
                idx = *(s32 *)(info + 0x2ab4 + i * 4);
                if (idx > 0) {
                    j28 = ((MenuArchive *)menu1p)->joints[11];
                    source = GetChild(j21, 0, 1, cnt, -1, -1);
                    jobjCopyPos(source, j28);
                    (void)j28;
                    (void)j28;
                    sMakeMatrix(j28);
                    cat = Tea_partsdat[idx].category;
                    rank = Tea_partsdat[idx].rank;
                    kind = 1;
                    if (cat == 2) {
                        kind = 3;
                    }
                    if (cat == 3) {
                        kind = 4;
                    }
                    if (kind == 1 && rank > 16) {
                        rank = 16;
                    }
                    if (kind == 3 && rank > 4) {
                        rank = 4;
                    }
                    if (kind == 4 && rank > 8) {
                        rank = 8;
                    }
                    jobjDispAll(GetChild(j28, 0, kind, rank, -1, -1));
                    putAscJobj(GetChild(j28, 0, 2, -1, -1, -1), 2, Tea_partsdat[idx].short_name);
                    cnt++;
                }
            }
        }
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
    if (anmN == 0 && done != 0) {
        return 1;
    }
    return 0;
}
#undef lit_8533
#define lit_8533 "SUPPLY LIST"
#undef lit_8534
#define lit_8534 "NEW PARTS"
#undef lit_8535
#define lit_8535 "NEW ZOIDS"

void hokyuu(s32 map)
{
    u8 *info;
    void *arc;
    s32 done;
    s32 ret;
    s32 i;
    s32 v;

    info = (u8 *)rdMapInfo(1, map);
    if (info == NULL)
        return;
    arc = LoadArc2((char *)lit_462);
    if (arc) {
        SetAnimArc2((MenuArchive *)arc, 5, 1);
        anmN = 1;
    }
    done = 0;
    SeOn(0x1d);
    do {
        ret = menu_sDispMissionHokyu(info, (MenuArchive *)arc, done);
        sPadAll();
        if (sPad1->field_5E)
            done = 1;
        if (sPad1->pressed_a)
            done = 1;
    } while (ret == 0);
    SeRet();
    if (arc != 0)
        PurgeArc2(arc);
    for (i = 0; i < 2; i++) {
        v = *(s32 *)(info + 0x2aac + i * 4);
        if (v != -1) {
            ttl_HaveAddZoid(0, (s16)v);
            ttl_HaveAddZoid(4, (s16)v);
        }
    }
    for (i = 0; i < 8; i++) {
        v = *(s32 *)(info + 0x2ab4 + i * 4);
        if (v > 0) {
            ttl_HaveAddParts(0, (s16)v);
            ttl_HaveAddParts(4, (s16)v);
        }
    }
    if (*(s16 *)IveSave[0].field_1CA0 == 0) {
        v = (s8)info[0x2ad9];
    } else {
        v = (s8)info[0x2ada];
        if (v > 0) {
            ttl_HaveAddHuman(0, (s16)v);
            ttl_HaveAddHuman(4, (s16)v);
        }
        v = (s8)info[0x2adb];
    }
    if (v > 0) {
        ttl_HaveAddHuman(0, (s16)v);
        ttl_HaveAddHuman(4, (s16)v);
    }
}
void ttlMenu_sDispMissionHokyu(void)
{
    debug11();
    OSReport("hokyu start   --------\n");
    hokyuu(GameMode.map);
    OSReport("map no= %d\n", GameMode.map);
    gRefresh();
    gClear();
    debug11();
}
#undef lit_8575
#define lit_8575 "hokyu start   --------\n"
#undef lit_8576
#define lit_8576 "map no= %d\n"
void menu_sDispStart(void)
{
    s32 a0 = 0;
    s32 a1 = 0;
    s32 a2 = 0;
    s32 a3 = 0;
    HSD_JObj *j = 0;

    if (menu1p != NULL) {
        HSD_StartRender(0);
        if (HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
            HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
            HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
            GXSetCullMode(0);
            if (menumode != 0) {
                menu_sDispStartPushStart();
                menu_sDispZoidEmpireCountry();
                menu_sDispZoidBattleNewGameLoadGame();
                menu_sDispZoidBattleSingleTeamGame();
                menu_StartScrollBigCoreBlock();
                menu_sDispStartBG_Base();
                jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[0], 0, -1, -1, -1, -1));
                menu_sDispStartMainMenu();
                menu_sDispZoidVSGame();
                menu_sDispStartPushLogo();
                if (menumode != 0 && menumode > 1) {
                    menu_sDispStartMsgWin();
                }
            }
            menu_sDispStartWhiteInWhiteOut();
            HSD_CObjEndCurrent();
            GXSetCullMode(2);
        }
        HSD_EndRender();
        debugmenudraw();
        vsyncN++;
    }
}

void menu_sDispStartBG_Base(void)
{
    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[10], 0, -1, -1, -1, -1));
}

void menu_StartScrollBigCoreBlock(void)
{
    HSD_JObj *a;
    HSD_JObj *b;
    HSD_JObj *c;
    HSD_JObj *d;

    if (menu1p != 0) {
        a = ((MenuArchive *)menu1p)->joints[11];
        b = ((MenuArchive *)menu1p)->joints[12];
        c = ((MenuArchive *)menu1p)->joints[13];
        d = ((MenuArchive *)menu1p)->joints[14];
        jobjDispAll(GetChild(a, 0, -1, -1, -1, -1));
        jobjDispAll(GetChild(b, 0, -1, -1, -1, -1));
        jobjDispAll(GetChild(c, 0, -1, -1, -1, -1));
        jobjDispAll(GetChild(d, 0, -1, -1, -1, -1));
        jobjAnim(a);
        jobjAnim(b);
        jobjAnim(c);
        jobjAnim(d);
        sMakeMatrix(a);
    }
}

void menu_sDispStartPushStart(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 k = 1;
    s32 dead = 0;
    s32 joint = 0;
    s32 c = 0;

    if (menu1p != NULL && menumode != 0 && menumode <= 1) {
        if ((u8)menutitleanm[0].state != menutitleanm[0].prev) {
            menutitleanm[0].mode = 1;
            menutitleanm[0].prev = menutitleanm[0].state;
        }
        if (menutitleanm[0].mode == 1) {
            menutitleanm[0].mode = 2;
            SetAnimArc2((MenuArchive *)menu1p, k, menutitleanm[0].state);
        }
        if (menutitleanm[0].mode == 2) {
            joint = jobjAnim(((MenuArchive *)menu1p)->joints[1]);
            if (joint != 0) {
                menutitleanm[0].mode = 3;
                menuanmend = 1;
                if (menutitleanm[0].state == 2) {
                    menutitleanm[0].prev = 2;
                    menutitleanm[0].mode = 0;
                    return;
                }
                if (menutitleanm[0].state == 0) {
                    menutitleanm[0].state = 1;
                    menutitleanm[0].mode = 2;
                } else if (menutitleanm[0].state == 1) {
                    menutitleanm[0].state = 3;
                    menutitleanm[0].mode = 2;
                } else if (menutitleanm[0].state == 3) {
                    menutitleanm[0].state = 3;
                    menutitleanm[0].mode = 1;
                }
                SetAnimArc2((MenuArchive *)menu1p, k, menutitleanm[0].state);
                jobjAnim(((MenuArchive *)menu1p)->joints[1]);
            }
            sMakeMatrix(((MenuArchive *)menu1p)->joints[1]);
        }
        if (menutitleanm[0].mode != 0) {
            HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[k], 0, -1, -1, -1, -1), 0, 7, 0);
        }
    }
    dead = 0;
    dead = dead;
}

void menu_sDispStartPushLogo(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 j = 9;
    s32 i = 9;
    s32 dead = 0;
    s32 anim = 0;

    if (menu1p != NULL) {
        if (menutitleanm[i].state != menutitleanm[i].prev) {
            menutitleanm[i].mode = 1;
            menutitleanm[i].prev = menutitleanm[i].state;
        }
        if (menutitleanm[i].mode == 1) {
            menutitleanm[i].mode = 2;
            SetAnimArc2((MenuArchive *)menu1p, j, menutitleanm[i].state);
        }
        if (menutitleanm[i].mode == 2) {
            anim = jobjAnim(((MenuArchive *)menu1p)->joints[j]);
            if (anim != 0) {
                menutitleanm[i].mode = 3;
                menuanmend = 1;
                if (menutitleanm[i].state == 1 || menutitleanm[i].state == 3) {
                    if (menutitleanm[i].state == 1) {
                        menutitleanm[i].state = 2;
                    }
                    if (menutitleanm[i].state == 3) {
                        menutitleanm[i].state = 0;
                    }
                    SetAnimArc2((MenuArchive *)menu1p, j, menutitleanm[i].state);
                    jobjAnim(((MenuArchive *)menu1p)->joints[j]);
                } else if (menutitleanm[i].state == 0 || menutitleanm[i].state == 2) {
                    if (menutitleanm[i].state == 0) {
                        menutitleanm[i].mode = 2;
                    }
                    if (menutitleanm[i].state == 3) {
                        menutitleanm[i].mode = 2;
                    }
                    SetAnimArc2((MenuArchive *)menu1p, j, menutitleanm[i].state);
                    jobjAnim(((MenuArchive *)menu1p)->joints[j]);
                }
            }
            sMakeMatrix(((MenuArchive *)menu1p)->joints[j]);
        }
        if (menutitleanm[i].mode != 0) {
            HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[j], 0, -1, -1, -1, -1), 0, 7, 0);
        }
    }
    dead = 0;
    dead = dead;
}

void menu_sDispStartMainMenu(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 i = 0;
    s32 dead = 0;
    s32 joint = 0;
    s16 k = 2;

    if (menu1p != NULL && menumode == 2) {
        i = 2;
        if ((u8)menutitleanm[i].state != menutitleanm[i].prev) {
            menutitleanm[i].mode = 1;
            menutitleanm[i].prev = menutitleanm[i].state;
        }
        if (menutitleanm[i].mode == 1) {
            menutitleanm[i].mode = 2;
            SetAnimArc2((MenuArchive *)menu1p, k, menutitleanm[i].state);
        }
        if (menutitleanm[i].mode == 2) {
            joint = jobjAnim(((MenuArchive *)menu1p)->joints[k]);
            if (joint != 0) {
                menutitleanm[i].mode = 3;
                menuanmend = 1;
                if (menutitleanm[i].state == 2)
                    return;
                if (menutitleanm[i].state == 0) {
                    menutitleanm[i].state = 1;
                    menutitleanm[i].mode = 2;
                }
                SetAnimArc2((MenuArchive *)menu1p, k, menutitleanm[i].state);
                jobjAnim(((MenuArchive *)menu1p)->joints[k]);
            }
            sMakeMatrix(((MenuArchive *)menu1p)->joints[k]);
        }
        dead = 0;
        (void)dead;
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[k], 0, -1, -1, -1, -1), 0, 7, 0);
    }
}

void menu_sDispZoidBattleNewGameLoadGame(void)
{
    s32 d0 = 0;
    s32 idx = 0;
    s32 dead = 0;
    s32 d1 = 0;
    s32 j = 0;
    s16 sel = 3;

    if (menu1p != NULL && menumode == 3) {
        idx = 3;
        if (menutitleanm[idx].state != menutitleanm[idx].prev) {
            menutitleanm[idx].mode = 1;
            menutitleanm[idx].prev = menutitleanm[idx].state;
        }
        if (menutitleanm[idx].mode == 1) {
            menutitleanm[idx].mode = 2;
            SetAnimArc2((MenuArchive *)menu1p, sel, menutitleanm[idx].state);
        }
        if (menutitleanm[idx].mode == 2) {
            j = jobjAnim(((MenuArchive *)menu1p)->joints[sel]);
            if (j != 0) {
                menutitleanm[idx].mode = 3;
                menuanmend = 1;
                if (menutitleanm[idx].state == 2) {
                    return;
                }
                if (menutitleanm[idx].state == 0) {
                    menutitleanm[idx].state = 1;
                    menutitleanm[idx].mode = 2;
                }
                SetAnimArc2((MenuArchive *)menu1p, sel, menutitleanm[idx].state);
                jobjAnim(((MenuArchive *)menu1p)->joints[sel]);
            }
            sMakeMatrix(((MenuArchive *)menu1p)->joints[sel]);
        }
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[sel], 0, -1, -1, -1, -1), NULL, 7, 0);
    }
    dead = 0;
    dead = dead;
}

void menu_sDispZoidBattleSingleTeamGame(void)
{
    s32 d0 = 0;
    s32 d1 = 0;
    s32 idx = 0;
    s32 dead = 0;
    s32 j = 0;
    s16 sel = 4;

    if (menu1p != NULL && menumode == 4) {
        dead = 0;
        (void)dead;
        idx = sel;
        if (menutitleanm[idx].state != menutitleanm[idx].prev) {
            menutitleanm[idx].mode = 1;
            menutitleanm[idx].prev = menutitleanm[idx].state;
        }
        if (menutitleanm[idx].mode == 1) {
            menutitleanm[idx].mode = 2;
            SetAnimArc2((MenuArchive *)menu1p, sel, menutitleanm[idx].state);
        }
        if (menutitleanm[idx].mode == 2) {
            j = jobjAnim(((MenuArchive *)menu1p)->joints[sel]);
            if (j != 0) {
                menutitleanm[idx].mode = 3;
                menuanmend = 1;
                if (menutitleanm[idx].state == 2) {
                    return;
                }
                if (menutitleanm[idx].state == 0) {
                    menutitleanm[idx].state = 1;
                    menutitleanm[idx].mode = 2;
                }
                SetAnimArc2((MenuArchive *)menu1p, sel, menutitleanm[idx].state);
                jobjAnim(((MenuArchive *)menu1p)->joints[sel]);
            }
            sMakeMatrix(((MenuArchive *)menu1p)->joints[sel]);
        }
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[sel], 0, -1, -1, -1, -1), NULL, 7, 0);
    }
}

void menu_sDispZoidVSGame(void)
{
    s32 d0 = 0;
    s32 d1 = 0;
    s32 idx = 0;
    s32 dead = 0;
    s32 j = 0;
    s16 sel = 5;

    if (menu1p != NULL && menumode == 5) {
        dead = 0;
        (void)dead;
        idx = sel;
        if (menutitleanm[idx].state != menutitleanm[idx].prev) {
            menutitleanm[idx].mode = 1;
            menutitleanm[idx].prev = menutitleanm[idx].state;
        }
        if (menutitleanm[idx].mode == 1) {
            menutitleanm[idx].mode = 2;
            SetAnimArc2((MenuArchive *)menu1p, sel, menutitleanm[idx].state);
        }
        if (menutitleanm[idx].mode == 2) {
            j = jobjAnim(((MenuArchive *)menu1p)->joints[sel]);
            if (j != 0) {
                menutitleanm[idx].mode = 3;
                menuanmend = 1;
                if (menutitleanm[idx].state == 2) {
                    return;
                }
                if (menutitleanm[idx].state == 0) {
                    menutitleanm[idx].state = 1;
                    menutitleanm[idx].mode = 2;
                }
                SetAnimArc2((MenuArchive *)menu1p, sel, menutitleanm[idx].state);
                jobjAnim(((MenuArchive *)menu1p)->joints[sel]);
            }
            sMakeMatrix(((MenuArchive *)menu1p)->joints[sel]);
        }
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[sel], 0, -1, -1, -1, -1), NULL, 7, 0);
    }
}

void menu_sDispZoidEmpireCountry(void)
{
    s32 a = 0;
    s32 i = 0;
    s32 dead = 0;
    s32 joint = 0;
    s32 b = 0;
    s16 k = 6;

    if (menu1p != NULL && menumode == 6) {
        i = k;
        if ((u8)menutitleanm[i].state != menutitleanm[i].prev) {
            menutitleanm[i].mode = 1;
            menutitleanm[i].prev = menutitleanm[i].state;
        }
        if (menutitleanm[i].mode == 1) {
            menutitleanm[i].mode = 2;
            SetAnimArc2((MenuArchive *)menu1p, k, menutitleanm[i].state);
        }
        if (menutitleanm[i].mode == 2) {
            joint = jobjAnim(((MenuArchive *)menu1p)->joints[k]);
            if (joint != 0) {
                menutitleanm[i].mode = 3;
                menuanmend = 1;
                if (menutitleanm[i].state == 2)
                    return;
                if (menutitleanm[i].state == 0) {
                    menutitleanm[i].state = 1;
                    menutitleanm[i].mode = 2;
                }
                SetAnimArc2((MenuArchive *)menu1p, k, menutitleanm[i].state);
                jobjAnim(((MenuArchive *)menu1p)->joints[k]);
            }
            sMakeMatrix(((MenuArchive *)menu1p)->joints[k]);
        }
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[k], 0, -1, -1, -1, -1), 0, 7, 0);
    }
    dead = 0;
    dead = dead;
}
#pragma force_active on
namespace { __declspec(section ".sdata2") extern const u32 lit_8475[1] = {
    0xFFFFFFFF, 
}; }
namespace { __declspec(section ".sdata2") extern const f32 lit_8476[1] = { 0.0f }; }
#pragma force_active off
void menu_sDispStartMsgWin(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    char text[200];
    HSD_JObj *child = 0;
    s32 dead = 0;

    if (menu1p != NULL) {
        child = GetChild(((MenuArchive *)menu1p)->joints[7], 0, -1, -1, -1, -1);
        SetAnimArc2((MenuArchive *)menu1p, 7, 1);
        jobjAnim(((MenuArchive *)menu1p)->joints[7]);
        HSD_JObjDispAll(child, 0, 7, 0);
        if (menudispwinstr != NULL && menudispwinstr[0] != 0) {
            sprintf(text, "%s ", menudispwinstr);
            sPutFontRect(text, GetChild(((MenuArchive *)menu1p)->joints[7], 0, 0, -1, -1, -1),
                         GetChild(((MenuArchive *)menu1p)->joints[7], 0, 0, 0, -1, -1),
                         (GXColor){ 255, 255, 255, 255 }, (GXColor){ 0, 0, 0, 0 }, 0, 2);
            dead = 1;
        }
    }
}

void menu_sDispStartWhiteInWhiteOut(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 dead = 0;
    s32 jobj = 0;

    if (menu1p == NULL) {
        return;
    }
    if (menutitleanm[1].state != menutitleanm[1].prev) {
        menutitleanm[1].mode = 1;
        menutitleanm[1].prev = menutitleanm[1].state;
    }
    if (menutitleanm[1].mode == 3) {
        return;
    }
    if (menutitleanm[1].mode == 1) {
        menutitleanm[1].mode = 2;
        SetAnimArc2((MenuArchive *)menu1p, 8, menutitleanm[1].state);
    }
    if (menutitleanm[1].mode == 2) {
        jobj = jobjAnim(((MenuArchive *)menu1p)->joints[8]);
        if (jobj != 0) {
            if (menutitleanm[1].state == 0) {
                menutitleanm[1].state = 2;
                menutitleanm[1].mode = 2;
            } else if (menutitleanm[1].state == 1) {
                menutitleanm[1].state = 3;
                menutitleanm[1].mode = 2;
            }
            SetAnimArc2((MenuArchive *)menu1p, 8, menutitleanm[1].state);
            jobjAnim(((MenuArchive *)menu1p)->joints[8]);
            menuanmend = 1;
            return;
        }
        sMakeMatrix(((MenuArchive *)menu1p)->joints[8]);
    }
    HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[8], 0, -1, -1, -1, -1), 0, 7, 0);
    dead = dead;
}

void menu_BlackInWhite(void)
{
    menutitleanm[1].mode = 1;
    menutitleanm[1].state = 0;
    menutitleanm[1].prev = 1;
    do {
        menu_sDispStart();
    } while (menutitleanm[1].state != 2);
}

void menu_BlackInWhiteSet(void)
{
    menutitleanm[1].mode = 1;
    menutitleanm[1].state = 0;
    menutitleanm[1].prev = 1;
}

void menu_WhiteOutBlack(void)
{
    menutitleanm[1].mode = 1;
    menutitleanm[1].state = 1;
    menutitleanm[1].prev = 0;
    do {
        menu_sDispStart();
    } while (menutitleanm[1].state != 3);
}

s16 menufncPushStart(void)
{
    s16 d0 = 0;
    s16 d1 = 0;
    s16 d2 = 0;
    s16 r = 0;
    s16 r2 = 0;
    s32 ret = 0;
    s32 i;
    u32 start;

    if (ttlLight != NULL)
        HSD_LObjRemoveAll(ttlLight);
    ttlLight = 0;
    menuCursor = 0;
    menuParticles = 0;
    TEA_SoundMasterVol(127, 0);
    configGet();
    TeaMenuBGM(0);
    ttlMenuLoadParts(6);
    if (menu1p != 0) {
        SetAnimArc2((MenuArchive *)menu1p, 11, 1);
        SetAnimArc2((MenuArchive *)menu1p, 12, 1);
        SetAnimArc2((MenuArchive *)menu1p, 13, 1);
        SetAnimArc2((MenuArchive *)menu1p, 14, 1);
    }
    TEA_SetTimerInit();
    menumodeShop = 0;
    menumode = 0;
    menudispyesnoanm.state = 1;
    menudispyesnoanm.anim = 1;
    menutitleanm[0].mode = 0;
    menutitleanm[0].state = 2;
    menutitleanm[0].prev = 2;
    menutitleanm[9].mode = 0;
    menutitleanm[9].state = 0;
    menutitleanm[9].prev = 0;
    menutitleanm[1].mode = 0;
    menutitleanm[1].state = 2;
    menutitleanm[1].prev = 2;
    menudispwinstr[0] = 0;
    menutitleanm[1].mode = 1;
    menutitleanm[1].state = 3;
    menuanmend = 0;
    menuanmend = 0;
    menumode = 0;
    menutitleanm[0].state = 0;
    menutitleanm[9].mode = 1;
    menu_BlackInWhiteSet();
    while (menutitleanm[1].state != 2)
        menu_sDispStart();
    menumode = 1;
    menu_WhiteOutBlack();
    u8 voice[8] = { 0, 1, 2, 4, 13, 17, 22, 23 };
    TEA_VoiceOn(voice[(u8)VIGetRetraceCount() % 8], 17);
    start = OSGetTime() / (*(u32 *)0x800000F8 / 4);
    while (1) {
        sPadAll();
        HSD_Rand();
        if (sPad1->field_63 != 0 || sPad2->field_63 != 0)
            break;
        menu_sDispStart();
        if (OSGetTime() / (*(u32 *)0x800000F8 / 4) - start > 30) {
            TeaBgm(-1);
            if (menu1p != NULL)
                PurgeArc2(menu1p);
            menu1p = 0;
            return 2;
        }
    }
    SeRet();
    menuanmend = 0;
    menutitleanm[0].mode = 1;
    menutitleanm[0].state = 2;
    while (1) {
        if (menutitleanm[0].mode == 0)
            break;
        menu_sDispStart();
    }
    menutitleanm[0].mode = 0;
    menuanmend = 0;
    menutitleanm[9].mode = 1;
    menutitleanm[9].state = 0;
    while (1) {
        menu_sDispStart();
        if (menutitleanm[9].state == 2)
            break;
        if (menutitleanm[9].state == 0)
            menutitleanm[9].state = 1;
    }
    menudispwinstr[0] = 0;
    menuanmend = 0;
    menumode = 2;
    while (1) {
        if (menumode == 2) {
            r = menufncPushStartMain();
            if (r == 0) {
                while (1) {
                    r2 = menufncZoidBattleNewGameLoadGame();
                    GameMode.game_type = 4;
                    if (r2 == -1) {
                        menumode = 2;
                        break;
                    } else if (r2 == 0) {
                        GameMode.game_type = 4;
                        gametimestart = OSGetTime() / (*(u32 *)0x800000F8 / 4);
                        IveSave[0].play_time = 0;
                        menumode = 6;
                        GameMode.map = 0;
                        IveSave[0].stage = 0;
                        IveSave[0].field_1C8C = 4;
                        r = menufncZoidMissionEmpireCountry();
                        if (r == -1)
                            continue;
                        ttlMenuSystemCardLoad();
                        menuZoidMission_NonSaveData_InitData();
                        GameMode.game_type = 4;
                        GameMode.win = 0;
                        GameMode.field_28 = 0;
                        GameMode.win = 0;
                        menumode = 9999;
                        ret = 101;
                        OSReport("GameStart 101\n");
                        break;
                    } else if (r2 == 1) {
                        GameMode.game_type = 4;
                        GameMode.win = 0;
                        GameMode.map = 0;
                        GameMode.field_28 = 12;
                        memcpy(&IveSaveFlagBak, &IveSaveFlag, sizeof(GameSaveRecord));
                        if (ttlMemCardChkStartNew() == 0) {
                            GameMode.game_type = 4;
                            menumode = 2;
                            memcpy(&IveSaveFlag, &IveSaveFlagBak, sizeof(GameSaveRecord));
                            continue;
                        }
                        if (ttlMenuBatSaveLoad(0) == 1) {
                            for (i = 0; i < 60; i++) {
                                HSD_StartRender(0);
                                HSD_EndRender();
                                debugmenudraw();
                                vsyncN++;
                            }
                            menudispyesnoflag = 0;
                            memcpy(&IveSave[0], &IveSave[2], sizeof(GameSaveRecord));
                            GameMode.map = IveSave[0].stage;
                            rdMapInfo(1, GameMode.map);
                            GameMode.field_28 = IveSave[0].map_no;
                            GameMode.game_type = IveSave[0].field_1C8C = 4;
                            GameMode.win = IveSave[0].field_1C8D = 1;
                            GameMode.field_2C = (s8)IveSave[0].country;
                            menuZoidBattleModeSingleTeam = IveSave[0].single_team = 0;
                            IveSave[0].field_15FA = -1;
                            IveSave[0].field_1602 = 57;
                            if (IveSave[0].field_1C87 == 0) {
                                do {
                                    r = menufncZoidMissionEmpireCountry();
                                } while (r == -1);
                                if ((s32)IveSave[0].country == 1)
                                    GameMode.map = IveSave[0].stage = 0;
                                else
                                    GameMode.map = IveSave[0].stage = 19;
                                rdMapInfo(1, GameMode.map);
                                menuZoidMission_EndSaveData_start();
                                GameMode.game_type = 4;
                                GameMode.win = 0;
                            }
                            OSReport("GameStart Mission stage=%d type=%d win=%d single=%d zoid=%d man=%d \n", IveSave[0].stage, IveSave[0].field_1C8C, IveSave[0].field_1C8D, (s8)IveSave[0].single_team, IveSave[0].field_15F4, IveSave[0].field_1600);
                            if (menu1p != NULL)
                                PurgeArc2(menu1p);
                            menu1p = 0;
                            GameMode.game_type = 4;
                            ret = 107;
                            return ret;
                        }
                        memcpy(&IveSaveFlag, &IveSaveFlagBak, sizeof(GameSaveRecord));
                        GameMode.game_type = 4;
                        menudispyesnoflag = 0;
                    }
                }
            } else if (r == 1) {
                menumode = 3;
            } else if (r == 2) {
                GameMode.field_08 = 0;
                menuZoidBattleModeSingleTeam = 1;
                ret = 300;
                menumode = 5;
            } else if (r == 3) {
                GameMode.field_08 = 1;
                menuZoidBattleModeSingleTeam = 1;
                ret = 600;
                menumode = 5;
            } else if (r == 4) {
                menufncZoid_ConfigMenu();
                memcpy(&IveSaveFlagBak, &IveSaveFlag, sizeof(GameSaveRecord));
                memcpy(&IveSave[3], &IveSaveFlag, sizeof(GameSaveRecord));
                memcpy(&IveSaveFlagBak, &IveSaveFlag, sizeof(GameSaveRecord));
                if (ttlMemCardChkStartNew() == 1) {
                    if (TEA_mcMsgYesNo(0, 0x57) != 0) {
                        if (ttlMenuSaveLoadexistenceDataStartNew0() == 1) {
                            memcpy(&IveSaveFlag, &IveSave[3], sizeof(GameSaveRecord));
                            ttlMenuSystemMemCardSave();
                        }
                    }
                }
                memcpy(&IveSaveFlag, &IveSaveFlagBak, sizeof(GameSaveRecord));
                configGet();
                TeaMenuBGM(0);
            }
        }
        if (menumode == 3) {
            r = menufncZoidBattleNewGameLoadGame();
            if (r == -1)
                menumode = 2;
            menuZoidBattleModeSingleTeam = 1;
            menuZoidBattleStage = -1;
            ret = 200;
            if (r == 0) {
                gametimestart = OSGetTime() / (*(u32 *)0x800000F8 / 4);
                ttlMenuSystemCardLoad();
                menuZoidBattle_NonSaveData_InitData();
                GameMode.game_type = 1;
                GameMode.win = 0;
                IveSave[0].next_stage = 0;
                GameMode.map = 0;
                IveSave[0].field_15F4 = IveSave[0].owned_zoids[0];
                IveSave[0].zoid_slot = 0;
                IveSave[0].field_15FA = IveSave[0].owned_zoids[1];
                IveSave[0].partner_slot = 1;
                IveSave[0].field_1600 = 0;
                IveSave[0].field_1602 = 1;
                GameMode.field_2C = (s8)(IveSave[0].country = 0);
                GameMode.game_type = 1;
                GameMode.win = 0;
                GameMode.field_28 = 12;
                GameMode.map = 0;
                IveSave[0].stage = GameMode.map;
                IveSave[0].map_no = GameMode.field_28;
                IveSave[0].field_1C8C = GameMode.game_type;
                IveSave[0].field_1C8D = GameMode.win;
                IveSave[0].single_team = menuZoidBattleModeSingleTeam;
                IveSave[0].play_time = 0;
                break;
            } else if (r == 1) {
                memcpy(&IveSaveFlagBak, &IveSaveFlag, sizeof(GameSaveRecord));
                if (ttlMemCardChkStartNew() == 0) {
                    memcpy(&IveSaveFlag, &IveSaveFlagBak, sizeof(GameSaveRecord));
                    continue;
                }
                GameMode.game_type = 1;
                GameMode.win = 0;
                GameMode.map = 0;
                GameMode.field_28 = 12;
                if (ttlMenuBatSaveLoad(0) == 1) {
                    memcpy(&IveSave[0], &IveSave[2], sizeof(GameSaveRecord));
                    GameMode.map = IveSave[0].stage;
                    menuZoidBattleStage = IveSave[0].stage;
                    GameMode.field_28 = IveSave[0].map_no;
                    GameMode.game_type = IveSave[0].field_1C8C = 1;
                    GameMode.win = IveSave[0].field_1C8D = 1;
                    menuZoidBattleModeSingleTeam = IveSave[0].single_team;
                    GameMode.win = 1;
                    GameMode.field_24 = IveSave[0].field_1C89 % 3;
                    rdMapInfo(0, GameMode.field_24 + GameMode.map * 3);
                    if ((s32)(u8)IveSave[0].field_1C87 == 1)
                        ret = 222;
                    GameMode.field_2C = (menumode = menumode, 0);
                    menuZoidBattle_SaveData_AddData();
                    break;
                }
                memcpy(&IveSaveFlag, &IveSaveFlagBak, sizeof(GameSaveRecord));
                continue;
            }
        }
        if (menumode == 4) {
            r = menufncZoidBattleSingleTeamGame();
            if (r == -1)
                menumode = 3;
            ret = 200;
            if (r == 0)
                break;
            if (r == 1)
                break;
        }
        if (menumode == 5) {
            menuZoidBattleModeSingleTeam = 1;
            r = menufncZoidVSGame();
            if (r == -1)
                menumode = 2;
            if (r == 0) {
                GameMode.game_type = 2;
                sCamNN2 = 2;
                sCamNN = 2;
                menuZoidBattleModeSingleTeam = 1;
                break;
            } else if (r == 1) {
                GameMode.game_type = 2;
                sCamNN2 = 2;
                sCamNN = 2;
                menuZoidBattleModeSingleTeam = 0;
                break;
            }
        }
        if (menumode == 9999)
            break;
    }
    menu_BlackInWhite();
    menumode = 0;
    menu_WhiteOutBlack();
    menuanmend = 0;
    GXColor black;
    black = (GXColor){ 0, 0, 0, 0 };
    HSD_VIData.bg_color = black;
    TeaBgm(-1);
    if (menu1p != NULL)
        PurgeArc2(menu1p);
    menu1p = 0;
    dspLoadF = 1;
    return ret;
}
#undef lit_8937
#define lit_8937 "GameStart 101\n"
#undef lit_8938
#define lit_8938 "GameStart Mission stage=%d type=%d win=%d single=%d zoid=%d man=%d \n"

s16 menufncPushStartMain(void)
{
    s32 unused2;
    s16 k = 2;
    s16 i = 0;
    unused2 = 0;
    register s16 n;
    n = 0;
    register s32 z = 0;
    s32 ret = 0;
    void *obj = 0;
    f32 f31 = 0.0f;
    f32 f28 = 0.0625f;
    f32 f30 = 0.003f;
    f32 f29 = 0.0f;
    s16 unused0 = 0;
    s32 y = 0;
    s32 unused1 = 0;
    s16 msg[6] = {0, 1, 2, 0x74, 3, 0};

    (void)unused2;
    menu1p == 0;
    y = 1;
    obj = searchTObjN(GetChild(((MenuArchive *)menu1p)->joints[k], 0, -1, -1, -1, -1), &y);
    n = 2;
    menumode = 2;
    menutitleanm[n].mode = 0;
    menutitleanm[n].state = 0;
    menutitleanm[n].prev = 2;
    menufarmcl[n] = 1;
    menufarmsl[n] = 1;
    menufarmst[n] = 0;
    menufarmmh[n] = 5;
    menufarmmn[n] = 5;
    f31 = f28 * (f32)menufarmcl[n];
    texScroll(obj, 0.0f, f31, 0.0f);
    menu_sDispStart();
    texScroll(obj, 0.0f, f31, 0.0f);
    menu_sDispStart();
    texScroll(obj, 0.0f, f31, 0.0f);
    texScroll(obj, 0.0f, f31, 0.0f);
    menu_sDispStart();
    menu_sDispStart();
    menu_sDispStart();
    for (;;) {
        sPadAll();
        menufarmcl[menumode] = (u16)TEA_menufnc12p(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        strcpy(menudispwinstr, GetMenuMsgAdr(msg[menufarmcl[menumode]]));
        if (sPad1->field_5E || sPad2->field_5E)
            SeCancel();
        if (sPad1->pressed_a || sPad2->pressed_a || sPad1->field_63 || sPad2->field_63) {
            if (sPad2->pressed_a || sPad2->field_63) {
                if (menufarmcl[menumode] != 2 && menufarmcl[menumode] != 3)
                    continue;
            }
            if (menufarmcl[menumode] == 0)
                break;
            if (menufarmcl[menumode] == 1)
                break;
            if (menufarmcl[menumode] == 2)
                break;
            if (menufarmcl[menumode] == 3)
                break;
            if (menufarmcl[menumode] == 4)
                break;
        }
        if (sPad1->field_4F || sPad1->field_53 || sPad2->field_4F || sPad2->field_53) {
            SeSelectB();
            f31 = f28 * (f32)menufarmcl[menumode];
            i = 0;
            do {
                f29 = f31 + (19.0f * f30 - (f32)i * f30);
                texScroll(obj, 0.0f, f29, 0.0f);
                menu_sDispStart();
                i++;
            } while (i < 19);
            texScroll(obj, 0.0f, f31, 0.0f);
            menu_sDispStart();
        }
        if (sPad1->field_50 || sPad1->field_54 || sPad2->field_50 || sPad2->field_54) {
            SeSelectB();
            i = 0;
            do {
                texScroll(obj, 0.0f, f31 + (f32)i * f30, 0.0f);
                menu_sDispStart();
                i++;
            } while (i < 19);
            f31 = f28 * (f32)menufarmcl[menumode];
            texScroll(obj, 0.0f, f31, 0.0f);
            menu_sDispStart();
        }
        menu_sDispStart();
    }
    SeRet();
    if (menufarmcl[menumode] == 1 || menufarmcl[menumode] == 2)
        menutitleanm[menumode].state = 2;
    else
        menutitleanm[menumode].state = 3;
    menutitleanm[menumode].state = 2;
    menutitleanm[menumode].mode = 1;
    menuanmend = 0;
    while (menutitleanm[menumode].mode != 3)
        menu_sDispStart();
    ret = menufarmcl[menumode];
    OSReport("menumain end\n");
    return ret;
}
#undef lit_9005
#define lit_9005 "menumain end\n"

s16 menufncZoidBattleNewGameLoadGame(void)
{
    s16 p0 = 0;
    s16 p1 = 0;
    s16 p2 = 0;
    s32 z4 = 0;
    s32 dead = 0;
    f32 delay = 0.003f;
    f32 spd = 0.0f;
    f32 kf = 0.0625f;
    s16 jn = 3;
    s32 dead1;
    s16 sel;
    void *obj;
    s32 dead2;

    (menu1p == NULL);
    dead2 = 0;
    sel = 0;
    dead1 = 0;
    obj = 0;
    z4 = 0;
    dead2 = -1;
    dead1 = 0;
    obj = searchTObjN(GetChild(((MenuArchive *)menu1p)->joints[jn], dead1, -1, -1, -1, dead2), &z4);
    spd = 0.0f;
    texScroll(obj, 0.0f, spd, 0.0f);
    menumode = 3;
    menutitleanm[menumode].mode = 1;
    menutitleanm[menumode].state = 0;
    menutitleanm[menumode].prev = 2;
    menufarmcl[menumode] = 0;
    menufarmsl[menumode] = 0;
    menufarmst[menumode] = 0;
    menufarmmh[menumode] = 2;
    menufarmmn[menumode] = 2;
    menuanmend = 0;
    while (1) {
        sPadAll();
        menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        strcpy(menudispwinstr, GetMenuMsgAdr(menufarmcl[menumode] + 6));
        if (menuanmend == 1) {
            if (menutitleanm[menumode].state == 0) {
                menutitleanm[menumode].state = 1;
            }
            menuanmend = 0;
        }
        if (sPad1->field_5E || sPad2->field_5E) {
            SeCancel();
            spd = 0.0f;
            texScroll(obj, 0.0f, spd, 0.0f);
            return -1;
        }
        if (sPad1->pressed_a || sPad2->pressed_a) {
            break;
        }
        if (sPad1->field_4F || sPad1->field_53 || sPad2->field_4F || sPad2->field_53) {
            SeSelectB();
            if (menufarmcl[menumode] == 0) {
                spd = 0.0f;
                texScroll(obj, 0.0f, spd, 0.0f);
            }
        }
        if (sPad1->field_50 || sPad1->field_54 || sPad2->field_50 || sPad2->field_54) {
            SeSelectB();
            if (menufarmcl[menumode] == 1) {
                spd = kf;
                texScroll(obj, 0.0f, spd, 0.0f);
            }
        }
        menu_sDispStart();
    }
    SeRet();
    sel = menufarmcl[menumode];
    return sel;
}

s16 menufncZoidMissionEmpireCountry(void)
{
    s16 p0 = 0;
    s16 p1 = 0;
    s16 p2 = 0;
    s32 z4 = 0;
    s32 dead = 0;
    f32 delay = 0.003f;
    f32 spd = 0.0f;
    f32 kf = 0.0625f;
    s16 jn = 6;
    s32 dead1;
    s32 sel;
    void *obj;
    s32 dead2;

    (menu1p == NULL);
    dead2 = 0;
    sel = 0;
    dead1 = 0;
    obj = 0;
    z4 = 0;
    dead2 = -1;
    dead1 = 0;
    obj = searchTObjN(GetChild(((MenuArchive *)menu1p)->joints[jn], dead1, -1, -1, -1, dead2), &z4);
    spd = 0.0f;
    texScroll(obj, 0.0f, spd, 0.0f);
    menumode = 6;
    menutitleanm[menumode].mode = 1;
    menutitleanm[menumode].state = 0;
    menutitleanm[menumode].prev = 2;
    menufarmcl[menumode] = 0;
    menufarmsl[menumode] = 0;
    menufarmst[menumode] = 0;
    menufarmmh[menumode] = 2;
    menufarmmn[menumode] = 2;
    menuanmend = 0;
    while (1) {
        sPadAll();
        menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        strcpy(menudispwinstr, GetMenuMsgAdr(menufarmcl[menumode] + 8));
        if (menuanmend == 1) {
            if (menutitleanm[menumode].state == 0) {
                menutitleanm[menumode].state = 1;
            }
            menuanmend = 0;
        }
        if (sPad1->field_5E || sPad2->field_5E) {
            SeCancel();
            spd = 0.0f;
            texScroll(obj, 0.0f, spd, 0.0f);
            return -1;
        }
        if (sPad1->pressed_a || sPad2->pressed_a) {
            break;
        }
        if (sPad1->field_4F || sPad1->field_53 || sPad2->field_4F || sPad2->field_53) {
            SeSelectB();
            if (menufarmcl[menumode] == 0) {
                spd = 0.0f;
                texScroll(obj, 0.0f, spd, 0.0f);
            }
        }
        if (sPad1->field_50 || sPad1->field_54 || sPad2->field_50 || sPad2->field_54) {
            SeSelectB();
            if (menufarmcl[menumode] == 1) {
                spd = kf;
                texScroll(obj, 0.0f, spd, 0.0f);
            }
        }
        menu_sDispStart();
    }
    SeRet();
    sel = menufarmcl[menumode];
    if (sel == 0) {
        GameMode.field_2C = (s8)(IveSave[0].country = 1);
        GameMode.map = (s8)(IveSave[0].stage = 0);
    }
    if (sel == 1) {
        GameMode.field_2C = (s8)(IveSave[0].country = 2);
        GameMode.map = (s8)(IveSave[0].stage = 19);
    }
    return sel;
}

s16 menufncZoidBattleSingleTeamGame(void)
{
    s16 p0 = 0;
    s16 p1 = 0;
    s16 p2 = 0;
    s32 z4 = 0;
    s32 dead = 0;
    f32 delay = 0.003f;
    f32 spd = 0.0f;
    f32 kf = 0.0625f;
    s16 jn = 4;
    s32 dead1;
    s32 sel;
    void *obj;
    s32 dead2;

    (menu1p == NULL);
    dead2 = 0;
    sel = 0;
    dead1 = 0;
    obj = 0;
    z4 = 0;
    dead2 = -1;
    dead1 = 0;
    obj = searchTObjN(GetChild(((MenuArchive *)menu1p)->joints[jn], dead1, -1, -1, -1, dead2), &z4);
    spd = 0.0f;
    menumode = 4;
    texScroll(obj, 0.0f, spd, 0.0f);
    menutitleanm[menumode].mode = 1;
    menutitleanm[menumode].state = 0;
    menutitleanm[menumode].prev = 2;
    menufarmcl[menumode] = 0;
    menufarmsl[menumode] = 0;
    menufarmst[menumode] = 0;
    menufarmmh[menumode] = 2;
    menufarmmn[menumode] = 2;
    menuanmend = 0;
    while (1) {
        sPadAll();
        menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        if (menuanmend == 1) {
            if (menutitleanm[menumode].state == 0) {
                menutitleanm[menumode].state = 1;
            }
            menuanmend = 0;
        }
        if (sPad1->field_5E || sPad2->field_5E) {
            SeCancel();
            spd = 0.0f;
            texScroll(obj, 0.0f, spd, 0.0f);
            return -1;
        }
        if (sPad1->pressed_a || sPad2->pressed_a) {
            (void)sel;
            break;
        }
        if (sPad1->field_4F || sPad1->field_53 || sPad2->field_4F || sPad2->field_53) {
            SeSelectB();
            if (menufarmcl[menumode] == 0) {
                spd = 0.0f;
                texScroll(obj, 0.0f, spd, 0.0f);
            }
        }
        if (sPad1->field_50 || sPad1->field_54 || sPad2->field_50 || sPad2->field_54) {
            SeSelectB();
            if (menufarmcl[menumode] == 1) {
                spd = kf;
                texScroll(obj, 0.0f, spd, 0.0f);
            }
        }
        menu_sDispStart();
    }
    SeRet();
    sel = menufarmcl[menumode];
    menuZoidBattleModeSingleTeam = sel;
    return sel;
}

s16 menufncZoidVSGame(void)
{
    s16 p0 = 0;
    s16 p1 = 0;
    s16 p2 = 0;
    s32 z4 = 0;
    s32 dead = 0;
    f32 delay = 0.003f;
    s16 arr[6] = {12, 13, 14, 0, 0, 0};
    f32 spd = 0.0f;
    f32 kf = 0.0625f;
    s16 jn;
    s32 dead1;
    s32 sel;
    void *obj;
    s32 dead2;

    dead2 = 0;
    jn = 5;
    dead1 = 0;
    sel = 0;
    obj = 0;
    if (menu1p == NULL) {
        OSReport("menu1p\n");
    }
    OSReport("menu VS\n");
    z4 = 0;
    dead2 = -1;
    dead1 = 0;
    obj = searchTObjN(GetChild(((MenuArchive *)menu1p)->joints[jn], 0, -1, -1, -1, -1), &z4);
    spd = 0.0f;
    menumode = jn;
    texScroll(obj, 0.0f, spd, 0.0f);
    menutitleanm[menumode].mode = 1;
    menutitleanm[menumode].state = 0;
    menutitleanm[menumode].prev = 2;
    menufarmcl[menumode] = 0;
    menufarmsl[menumode] = 0;
    menufarmst[menumode] = 0;
    menufarmmh[menumode] = 2;
    menufarmmn[menumode] = 2;
    menuanmend = 0;
    menu_sDispStart();
    while (1) {
        sPadAll();
        menufarmcl[menumode] = TEA_menufnc(&menufarmsl[menumode], &menufarmst[menumode], menufarmmn[menumode], menufarmmh[menumode]);
        strcpy(menudispwinstr, GetMenuMsgAdr(menufarmcl[menumode] + 0x72));
        if (menuanmend == 1) {
            if (menutitleanm[menumode].state == 0) {
                menutitleanm[menumode].state = 1;
            }
            menuanmend = 0;
        }
        if (sPad1->field_5E || sPad2->field_5E) {
            SeCancel();
            texScroll(obj, 0.0f, spd = 0.0f, 0.0f);
            return -1;
        }
        if (sPad1->pressed_a || sPad2->pressed_a) {
            (void)jn;
            break;
        }
        if (sPad1->field_4F || sPad1->field_53 || sPad2->field_4F || sPad2->field_53) {
            SeSelectB();
            if (menufarmcl[menumode] == 0) {
                texScroll(obj, 0.0f, spd = 0.0f, 0.0f);
            }
        }
        if (sPad1->field_50 || sPad1->field_54 || sPad2->field_50 || sPad2->field_54) {
            SeSelectB();
            if (menufarmcl[menumode] == 1) {
                spd = kf;
                texScroll(obj, 0.0f, spd, 0.0f);
            }
        }
        menu_sDispStart();
    }
    SeRet();
    sel = menufarmcl[menumode];
    GameMode.win = 0;
    if (sel == 0) {
        GameMode.game_type = 2;
        sCamNN2 = 2;
        sCamNN = 2;
        menuZoidBattleModeSingleTeam = 1;
    } else {
        GameMode.game_type = 3;
        sCamNN2 = 2;
        sCamNN = 1;
        menuZoidBattleModeSingleTeam = 0;
    }
    ttlMenuSystemCardLoad();
    return sel;
}
#undef lit_9149
#define lit_9149 "menu1p\n"
#undef lit_9150
#define lit_9150 "menu VS\n"

s32 menufncZoidVs01(void)
{
    ttlMenuLoadPersonBanner();
    ttlMenuLoadZoidBanner();
    ttlMenuLoadBack(3);
    ttlMenuLoadParts(7);
    ttlMenuLoadDetail();
    return 0x130;
}

void menu_sDispVsEntry(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    s32 e = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if (menu_back1p != 0 && HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(2);
        jobjDispAll(((MenuArchive *)menu_back1p)->joints[0]);
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if (menu1p != 0 && HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        selectcursolanm(7);
        menu_sDispBattleFarmTitle();
        menu_sDispBattleFarmTitleSubButton();
        menu_sDispVsEntryPersonPartnerZoidPic();
        menu_sDispVsEntryPersonZoidStatus();
        menu_sDispVsFarmExitButton();
        vsman = 0;
        menu_sDispVsEntryLoadButton();
        vsman = 1;
        menu_sDispVsEntryLoadButton();
        jobjAnim(((MenuArchive *)menu1p)->joints[5]);
        menu_sDispVsFarmPlayer1IconEnt();
        menu_sDispVsFarmPlayer2IconEnt();
        SetAnimArc2((MenuArchive *)menu1p, 9, 1);
        jobjAnim(((MenuArchive *)menu1p)->joints[9]);
        vsman = 0;
        menu_sDispVsEntryPilotZoidList();
        vsman = 1;
        menu_sDispVsEntryPilotZoidList();
        vsman = 0;
        menu_sDispVsEntryPersonPartnerZoidSelectFrame();
        vsman = 1;
        menu_sDispVsEntryPersonPartnerZoidSelectFrame();
        vsman = 0;
        menu_sDispDetailAll();
        menu_sDispYesNoMenu();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}

void menu_sDispVsEntryLoadButton(void)
{
    s32 v = vsman;
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    HSD_JObj *joint = 0;
    s32 model = 4;
    s32 ret = 0;

    joint = GetChild(((MenuArchive *)menu1p)->joints[model], 0, v, -1, -1, -1);
    if (flag_VsEntryLoadButton[vsman].a != 0) {
        if (flag_VsEntryLoadButton[vsman].a == 1) {
            flag_VsEntryLoadButton[vsman].a = 2;
            sSetAnim1(menu1p, model, joint, flag_VsEntryLoadButton[vsman].b);
        }
        if (flag_VsEntryLoadButton[vsman].a == 2) {
            ret = jobjAnim1(joint);
            if (ret != 0) {
                flag_VsEntryLoadButton[vsman].a = 3;
                switch (flag_VsEntryLoadButton[vsman].b) {
                case 0:
                    flag_VsEntryLoadButton[vsman].b = 0;
                    flag_VsEntryLoadButton[vsman].a = 2;
                    break;
                case 1:
                    flag_VsEntryLoadButton[vsman].b = 2;
                    flag_VsEntryLoadButton[vsman].a = 2;
                    break;
                case 2:
                    flag_VsEntryLoadButton[vsman].b = 2;
                    flag_VsEntryLoadButton[vsman].a = 2;
                    break;
                case 3:
                    flag_VsEntryLoadButton[vsman].b = 3;
                    flag_VsEntryLoadButton[vsman].a = 2;
                    break;
                case 5:
                    flag_VsEntryLoadButton[vsman].a = 0;
                    break;
                default:
                    flag_VsEntryLoadButton[vsman].a = 2;
                    break;
                }
                sSetAnim1(menu1p, model, joint, flag_VsEntryLoadButton[vsman].b);
                jobjAnim1(joint);
            }
        }
        HSD_JObjDispAll(joint, 0, 7, 0);
    }
}
void menu_sDispVsFarmPlayer1IconEnt(void)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    register HSD_JObj *dst = 0;
    HSD_JObj *src = 0;
    s32 e = 0;
    s32 f = 0;

    e = 0;
    f = 0;
    flag_VsFarmPlayer1Icon = menufarmcl[0];
    if (flag_VsFarmPlayer1Icon != -1) {
        if ((u8)menuZoidBattleModeSingleTeam != 0 || ttl_vs_modeLR == 0) {
            if (flag_VsFarmPlayer1Icon == 3) {
                src = GetChild(((MenuArchive *)menu1p)->joints[3], 0, 3, 1, -1, -1);
            } else {
                return;
            }
            dst = GetChild(((MenuArchive *)menu1p)->joints[5], 0, 1, -1, -1, -1);
            jobjCopyPos(src, dst);
            HSD_JObjDispAll(dst, 0, 7, 0);
        }
    }
}
void menu_sDispVsFarmPlayer2IconEnt(void)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    HSD_JObj *target = 0;
    HSD_JObj *source = 0;
    s32 x = 0;
    s32 y = 0;

    x = 0;
    y = 0;
    flag_VsFarmPlayer2Icon = menufarmcl[1];
    if (flag_VsFarmPlayer2Icon != -1) {
        if ((u8)menuZoidBattleModeSingleTeam != 0 || ttl_vs_modeLR == 1) {
            if (flag_VsFarmPlayer2Icon == 3) {
                source = GetChild(((MenuArchive *)menu1p)->joints[3], 0, 3, 2, -1, -1);
            } else {
                return;
            }
            target = GetChild(((MenuArchive *)menu1p)->joints[5], 0, 0, -1, -1, -1);
            jobjCopyPos(source, target);
            HSD_JObjDispAll(target, 0, 7, 0);
        }
    }
}

void menu_sDispVsEntryPersonPartnerZoidPic(void)
{
    s32 a = 0;
    s32 b = 0;
    s16 c = 0;
    s32 d = 5;
    s32 e = 2;
    HSD_JObj *p = 0;
    HSD_JObj *q = 0;

    q = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 0, 1, -1, -1);
    p = 0;
    if (menu_personp[IveSave[0].field_1600]) {
        p = GetChild(((MenuArchive *)menu_personp[IveSave[0].field_1600])->joints[0], 0, 0, -1, -1, -1);
    }
    jobjCopyPos(q, p);
    jobjDispAll(p);
    q = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 0, 2, -1, -1);
    p = GetChild(((MenuArchive *)menu_zoidp[IveSave[0].field_15F4])->joints[0], 0, 0, -1, -1, -1);
    if (p != NULL) {
        jobjCopyPos(q, p);
        jobjDispAll(p);
    }
    q = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 1, 1, -1, -1);
    p = 0;
    if (menu_personp[IveSave[1].field_1600]) {
        p = GetChild(((MenuArchive *)menu_personp[IveSave[1].field_1600])->joints[0], 0, 0, -1, -1, -1);
    }
    jobjCopyPos(q, p);
    jobjDispAll(p);
    q = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 1, 2, -1, -1);
    p = GetChild(((MenuArchive *)menu_zoidp[IveSave[1].field_15F4])->joints[0], 0, 0, -1, -1, -1);
    if (p != NULL) {
        jobjCopyPos(q, p);
        jobjDispAll(p);
    }
}

void menu_sDispVsEntryPersonZoidStatus(void)
{
    s32 u0 = 0;
    s32 u1 = 2;
    s32 u2 = 0;
    s32 idx = 6;
    s32 p = 0;
    ZoidState *z = (ZoidState *)&((ZoidStateV2 *)Zoid)[6];
    s32 dead = 0;
    char text[40];

    p = 0;
    (void)dead;
    (void)dead;
    (void)dead;
    (void)dead;
    (void)dead;
    (void)dead;
    (void)dead;
    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, -1, -1, -1));
    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, 7, -1, -1));
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, 0, 0, -1), 1, Tea_persondat[IveSave[0].field_1600].name);
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, 0, 1, -1), 1, Tea_zoiddat[IveSave[0].field_15F4].name);
    zdCalcFarm((ZoidState *)&((ZoidStateV2 *)Zoid)[0], 0, IveSave[0].zoid_slot, -1);
    menu_sDispVsEntryZoidStatusKind(0, 0, Tea_zoiddat[IveSave[0].field_15F4].field_66);
    menu_sDispVsEntryZoidStatusKind(0, 1, ZF(&((ZoidStateV2 *)Zoid)[0], s32, 0x1DD8));
    menu_sDispVsEntryZoidStatusKind(0, 2, ZF(&((ZoidStateV2 *)Zoid)[0], s32, 0x1DDC));
    zdCalcFarm((ZoidState *)&((ZoidStateV2 *)Zoid)[p], 0, IveSave[p].zoid_slot, IveSave[p].field_1600);
    z = (ZoidState *)&((ZoidStateV2 *)Zoid)[p];
    sprintf(text, "%7d", *(s32 *)((u8 *)z + 0x1f88));
    putAscJobjLR(GetChild(((MenuArchive *)menu1p)->joints[6], 0, p, 6, 0, -1), 2, text, 1);
    p = 1;
    jobjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, 7, -1, -1));
    if ((u8)menuZoidBattleModeSingleTeam == 0)
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, 7, 1, -1));
    else
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, 7, 0, -1));
    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, 0, -1, -1));
    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, 6, -1, -1));
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, 0, 0, -1), 1, Tea_persondat[IveSave[1].field_1600].name);
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, p, 0, 1, -1), 1, Tea_zoiddat[IveSave[1].field_15F4].name);
    zdCalcFarm((ZoidState *)&((ZoidStateV2 *)Zoid)[1], 1, IveSave[p].zoid_slot, -1);
    menu_sDispVsEntryZoidStatusKind(1, 0, Tea_zoiddat[IveSave[1].field_15F4].field_66);
    menu_sDispVsEntryZoidStatusKind(1, 1, ZF(&((ZoidStateV2 *)Zoid)[1], s32, 0x1DD8));
    menu_sDispVsEntryZoidStatusKind(1, 2, ZF(&((ZoidStateV2 *)Zoid)[1], s32, 0x1DDC));
    zdCalcFarm((ZoidState *)&((ZoidStateV2 *)Zoid)[p], 1, IveSave[p].zoid_slot, IveSave[p].field_1600);
    z = (ZoidState *)&((ZoidStateV2 *)Zoid)[p];
    sprintf(text, "%7d", *(s32 *)((u8 *)z + 0x1f88));
    putAscJobjLR(GetChild(((MenuArchive *)menu1p)->joints[6], 0, p, 6, 0, -1), 2, text, 1);
}

void menu_sDispVsEntryZoidStatusKind(s32 kind, s32 row, s32 value)
{
    s32 a = 0;
    f32 f = 0.0f;
    char text[8];
    s32 b = 0;
    s32 idx = 6;
    register s32 dead = 0;

    jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, kind, row + 3, -1, -1));
    sprintf(text, lit_3584, value);
    putAscJobj(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, kind, row + 3, 0, -1), 2, text);
}

void menu_sDispVsEntryPilotZoidList(void)
{
    s32 dead1 = 6;
    s32 dead2 = 0;
    s16 i = 0;
    HSD_JObj *dst = 0;
    HSD_JObj *src = 0;

    if (flag_menuvsentzoid[vsman] == 1 || flag_menuvsenthuman[vsman] == 1) {
        jobjDispAll(GetChild(((MenuArchive *)menu1p)->joints[9], 0, vsman, -1, -1, -1));
    }
    if (menuvsmode[vsman] == vsman + 4 && flag_menuvsenthuman[vsman] != 0) {
        i = 0;
        for (i = 0; i < IveSave[vsman].person_count; i++) {
            if (menufarmst[vsman + 4] <= i && i - menufarmst[vsman + 4] <= menufarmmh[vsman + 4] - 1) {
                menu_sDispVsEntryPersonListDisp(IveSave[vsman].persons[i], i - menufarmst[vsman + 4]);
            }
        }
        src = GetChild(((MenuArchive *)menu1p)->joints[9], 0, vsman, 3, menufarmsl[vsman + 4], -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 1, -1, -1, -1);
        jobjCopyPos(src, dst);
        jobjDisp(dst);
    }
    if (menuvsmode[vsman] == vsman + 6 && flag_menuvsentzoid[vsman] != 0) {
        i = 0;
        for (i = 0; i < IveSave[vsman].owned_zoid_count; i++) {
            if (menufarmst[vsman + 6] <= i && i - menufarmst[vsman + 6] <= menufarmmh[vsman + 6] - 1) {
                menu_sDispVsEntryZoidListDisp(IveSave[vsman].owned_zoids[i], i - menufarmst[vsman + 6]);
            }
        }
        src = GetChild(((MenuArchive *)menu1p)->joints[9], 0, vsman, 3, menufarmsl[vsman + 6], -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 1, -1, -1, -1);
        jobjCopyPos(src, dst);
        jobjDisp(dst);
    }
}

void menu_sDispVsEntryPersonListDisp(s16 idx, s16 y)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    char text[0x50];
    s32 d = 0;
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s32 dead1 = 0;
    s32 dead2 = 0;

    dead1 = 0;
    dead2 = 0;
    if (flag_menuvsenthuman[vsman] != 0) {
        if (idx == -1) {
            return;
        }
        src = GetChild(((MenuArchive *)menu1p)->joints[9], 0, vsman, 3, y, -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 0, -1, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDispAll(dst, 0, 7, 0);
        src = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 0, -1, -1, -1);
        dst = 0;
        if (menu_personp[idx] != 0) {
            dst = GetChild(((MenuArchive *)menu_personp[idx])->joints[0], 0, 1, -1, -1, -1);
        }
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
        sprintf(text, lit_3644, Tea_persondat[idx].name);
        putAscJobj(src, 0, text);
    }
}

void menu_sDispVsEntryZoidListDisp(s16 idx, s16 row)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    char text[0x50];
    HSD_JObj *src = 0;
    HSD_JObj *dst = 0;
    s32 dead1 = 0;
    s32 dead2 = 0;

    dead1 = 0;
    dead2 = 0;
    if (flag_menuvsentzoid[vsman] != 0) {
        src = GetChild(((MenuArchive *)menu1p)->joints[9], 0, vsman, 3, row, -1);
        dst = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 0, -1, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDispAll(dst, 0, 7, 0);
        src = GetChild(((MenuArchive *)menu1p)->joints[8], 0, 0, -1, -1, -1);
        dst = GetChild(((MenuArchive *)menu_zoidp[idx])->joints[0], 0, 1, -1, -1, -1);
        jobjCopyPos(src, dst);
        HSD_JObjDispAll(dst, 0, 7, 0);
        sprintf(text, lit_3644, Tea_zoiddat[idx].name);
        putAscJobj(src, 0, text);
    }
}

void menu_sDispVsEntryPersonPartnerZoidSelectFrame(void)
{
    s32 z1 = 0;
    s32 z2 = 0;
    s16 z3 = 0;
    s16 z4 = 1;
    HSD_JObj *joint2 = (HSD_JObj *)NULL;
    s16 idx = 0;
    HSD_JObj *joint1 = (HSD_JObj *)NULL;

    if (menuvsmode[vsman] == vsman) {
        if ((u8)menuZoidBattleModeSingleTeam == 0) {
            if (ttl_vs_modeLR != vsman)
                goto end;
        }
        if (menufarmcl[menuvsmode[vsman]] == 0) {
            idx = 1;
        } else {
            if (menufarmcl[menuvsmode[vsman]] == 2) {
                idx = 2;
            } else {
                return;
            }
        }
        joint1 = GetChild(((MenuArchive *)menu1p)->joints[6], 0, vsman, idx, -1, -1);
        joint2 = GetChild(((MenuArchive *)menu1p)->joints[7], 0, 0, -1, -1, -1);
        jobjCopyPos(joint1, joint2);
        HSD_JObjDisp(joint2, 0, 7, 0);
    }
end:
    ;
}

void menu_TITLE_dispVsEntry(s32 mode)
{
    s16 i = 0;

    i = 1;
    menuanmend = 0;
    menutitleanm[0].mode = 1;
    for (; i < 7; i++) {
        menutitleanm[i].mode = 1;
    }
    if (mode == 1) {
        menutitleanm[0].state = 0;
        i = 1;
        for (; i < 7; i++) {
            menutitleanm[i].state = 0;
        }
    } else {
        menutitleanm[0].state = 2;
        i = 1;
        for (; i < 7; i++) {
            menutitleanm[i].state = 2;
        }
    }
    while (menuanmend == 0) {
        menu_sDispVsEntry();
    }
    menuanmend = 0;
}
s32 menufncZoidVsMenuEntry(void)
{
    s32 u0 = 0;
    s32 u1 = 0;
    s32 u2 = -240;
    s32 u3 = 0;
    s32 u4 = 0;
    u8 u5 = 0;
    s32 i = 0;
    s32 j = 0;
    s32 a = 1;
    s32 b = 1;
    u8 map = 0;
    u8 f28 = 0;
    s16 d0 = 0;
    s32 d1 = 0;
    s16 gt = 0;
    s32 d2 = 0;
    s32 d3 = 0;
    s16 pl = 0;
    s16 side = 0;
    s16 single = 0;

    menuCursor = 0;
    menuParticles = 0;
    menuvsmode[0] = 0;
    menuvsmode[1] = 1;
    ttl_HaveSortHuman(0);
    ttl_HaveSortHuman(1);
    for (i = 0; i < 20; i++) {
        menumodeanm[i] = 0;
    }
    for (i = 0; i < 20; i++) {
        j = vsman;
        menufarmanm[i] = 0;
    }
    OSReport("VS ENTRY\n");
    vsyncN = 0;
    menuSetTitle(1);
    menuSetTitleButton(0x65);
    TEA_setvsync();
    menufarmmode = 1;
    menufarmcl[0] = 0;
    menufarmsl[0] = 0;
    menufarmst[0] = 0;
    menufarmmh[0] = 4;
    menufarmmn[0] = 4;
    menufarmcl[1] = 0;
    menufarmsl[1] = 0;
    menufarmst[1] = 0;
    menufarmmh[1] = 4;
    menufarmmn[1] = 4;
    menufarmcl[2] = 0;
    menufarmsl[2] = 0;
    menufarmst[2] = 0;
    menufarmmh[2] = 4;
    menufarmmn[2] = 4;
    menufarmcl[3] = 0;
    menufarmsl[3] = 0;
    menufarmst[3] = 0;
    menufarmmh[3] = 4;
    menufarmmn[3] = 4;
    menufarmsl[4] = 0;
    menufarmmn[4] = IveSave[0].person_count;
    menufarmst[4] = -1;
    menufarmmh[4] = 4;
    menufarmsl[5] = 0;
    menufarmmn[5] = IveSave[1].person_count;
    menufarmst[5] = -1;
    menufarmmh[5] = 4;
    menufarmsl[6] = IveSave[0].zoid_slot;
    menufarmmn[6] = IveSave[0].owned_zoid_count;
    menufarmst[6] = -1;
    menufarmmh[6] = 4;
    menufarmsl[7] = IveSave[1].zoid_slot;
    menufarmmn[7] = IveSave[1].owned_zoid_count;
    menufarmst[7] = -1;
    menufarmmh[7] = 4;
    menufarmmh[4] = IveSave[0].person_count;
    if (menufarmmh[4] >= 4)
        menufarmmh[4] = 4;
    menufarmmn[4] = IveSave[0].person_count;
    menufarmmh[5] = IveSave[0].person_count;
    if (menufarmmh[5] >= 4)
        menufarmmh[5] = 4;
    menufarmmn[5] = IveSave[1].person_count;
    pl = 0;
    if (IveSave[pl].owned_zoid_count > menufarmmn[pl + 6])
        menufarmmn[pl + 6] = IveSave[pl].owned_zoid_count;
    if (menufarmmn[pl + 6] < menufarmmh[pl + 6])
        menufarmmh[pl + 6] = menufarmmn[pl + 6];
    pl = 1;
    if (IveSave[pl].owned_zoid_count < menufarmmh[pl + 6])
        menufarmmh[pl + 6] = IveSave[pl].owned_zoid_count;
    if (menufarmmn[pl + 6] < menufarmmh[pl + 6])
        menufarmmh[pl + 6] = menufarmmn[pl + 6];
    flag_menuvsenthuman[0] = 0;
    flag_menuvsenthuman[1] = 0;
    flag_menuvsentzoid[0] = 0;
    flag_menuvsentzoid[1] = 0;
    vsman = 0;
    flag_VsEntryLoadButton[vsman].b = 2;
    flag_VsEntryLoadButton[vsman].a = 1;
    vsman = 1;
    flag_VsEntryLoadButton[vsman].b = 2;
    flag_VsEntryLoadButton[vsman].a = 1;
    menucuranm[0] = 1;
    menuSetTitle(1);
    menuSetTitleButton(0x65);
    OSReport("VS ENTRY ST\n");
    for (;;) {
        sPadAll();
        if ((u8)menuZoidBattleModeSingleTeam == 0)
            sPad2->field_47 = 0;
        for (vsman = 0; vsman < 2; vsman++) {
            if ((u8)menuZoidBattleModeSingleTeam == 0) {
                if (vsman >= 1)
                    continue;
                if (menuvsmode[vsman] <= 1) {
                    if (sPad1->field_5A)
                        ttl_vs_modeLR = 1;
                    if (sPad1->field_59 && menuvsmode[1] != 5 && menuvsmode[1] != 7)
                        ttl_vs_modeLR = 0;
                }
            } else {
                ttl_vs_modeLR = vsman;
            }
            if (vsman == 0) {
                menufarmcl[menuvsmode[ttl_vs_modeLR]] = TEA_menufnc(&menufarmsl[menuvsmode[ttl_vs_modeLR]], &menufarmst[menuvsmode[ttl_vs_modeLR]], menufarmmn[menuvsmode[ttl_vs_modeLR]], menufarmmh[menuvsmode[ttl_vs_modeLR]]);
                if (menuvsmode[ttl_vs_modeLR] == ttl_vs_modeLR) {
                    if (ttl_vs_modeLR == 0) {
                        if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 1)
                            flag_VsFarmPlayer1Icon = 5;
                        else if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 3)
                            flag_VsFarmPlayer1Icon = 4;
                        else
                            flag_VsFarmPlayer1Icon = -1;
                    } else {
                        if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 1)
                            flag_VsFarmPlayer2Icon = 5;
                        else if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 3)
                            flag_VsFarmPlayer2Icon = 4;
                        else
                            flag_VsFarmPlayer2Icon = -1;
                    }
                } else if (ttl_vs_modeLR == 0) {
                    flag_VsFarmPlayer1Icon = -1;
                } else {
                    flag_VsFarmPlayer2Icon = -1;
                }
                if (sPad1->field_47) {
                    if ((u8)menuZoidBattleModeSingleTeam == 0) {
                        detSaveNoSet(ttl_vs_modeLR);
                        ttl_vsent_detailsetparam(ttl_vs_modeLR);
                    } else if (ttl_vs_modeLR == 0) {
                        detSaveNoSet(0);
                        ttl_vsent_detailsetparam(0);
                    }
                }
            }
            if (vsman == 1) {
                if ((u8)menuZoidBattleModeSingleTeam == 0)
                    menufarmcl[menuvsmode[vsman]] = TEA_menufnc(&menufarmsl[menuvsmode[vsman]], &menufarmst[menuvsmode[vsman]], menufarmmn[menuvsmode[vsman]], menufarmmh[menuvsmode[vsman]]);
                if ((u8)menuZoidBattleModeSingleTeam == 1)
                    menufarmcl[menuvsmode[vsman]] = TEA_menufnc2p((s32)&menufarmsl[menuvsmode[vsman]], (s32)&menufarmst[menuvsmode[vsman]], menufarmmn[menuvsmode[vsman]], menufarmmh[menuvsmode[vsman]]);
                if (menuvsmode[vsman] == 1) {
                    if (menufarmcl[menuvsmode[vsman]] == 1)
                        flag_VsFarmPlayer2Icon = 5;
                    else if (menufarmcl[menuvsmode[vsman]] == 3)
                        flag_VsFarmPlayer2Icon = 4;
                    else
                        flag_VsFarmPlayer2Icon = -1;
                } else {
                    flag_VsFarmPlayer2Icon = -1;
                }
                if (sPad2->field_47) {
                    if ((u8)menuZoidBattleModeSingleTeam == 0) {
                        sPad2->field_47 = 0;
                    } else {
                        detSaveNoSet(1);
                        ttl_vsent_detailsetparam(1);
                    }
                }
            }
            if ((sPad1->field_57 && vsman == 0) || (sPad1->field_58 && vsman == 0))
                ttl_vsent_setparamdispobjct();
            if ((sPad2->field_57 && vsman == 1) || (sPad2->field_58 && vsman == 1))
                ttl_vsent_setparamdispobjct();
            if (sPad1->field_5A && (u8)menuZoidBattleModeSingleTeam == 0)
                ttl_vsent_setparamdispobjct();
            if (sPad1->field_59 && (u8)menuZoidBattleModeSingleTeam == 0)
                ttl_vsent_setparamdispobjct();
            if ((sPad1->field_5E && vsman == 0) || (sPad2->field_5E && vsman == 1)) {
                side = vsman;
                if ((u8)menuZoidBattleModeSingleTeam == 0)
                    side = ttl_vs_modeLR;
                if (menuvsmode[side] == side + 4) {
                    SeCancel();
                    menuvsmode[side] = (s32)side;
                    flag_menuvsenthuman[side] = 0;
                } else if (menuvsmode[side] == side + 6) {
                    SeCancel();
                    menuvsmode[side] = (s32)side;
                    flag_menuvsentzoid[side] = 0;
                } else if (menuvsmode[ttl_vs_modeLR] == ttl_vs_modeLR) {
                    SeCancel();
                    menufarmcl[menuvsmode[ttl_vs_modeLR]] = 3;
                    menufarmsl[menuvsmode[ttl_vs_modeLR]] = 3;
                    menufarmst[menuvsmode[ttl_vs_modeLR]] = 0;
                    ttl_vsent_setparamdispobjct();
                }
            } else if ((sPad1->pressed_a && vsman == 0) || (sPad2->pressed_a && vsman == 1)) {
                switch (menuvsmode[ttl_vs_modeLR]) {
                case 0:
                case 1:
                    SeRet();
                    pl = ttl_vs_modeLR;
                    if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 0) {
                        if ((u8)menuZoidBattleModeSingleTeam == 0) {
                            menuvsmode[menuvsmode[ttl_vs_modeLR]] = ttl_vs_modeLR + 4;
                            flag_menuvsenthuman[ttl_vs_modeLR] = 1;
                        } else {
                            menuvsmode[menuvsmode[ttl_vs_modeLR]] = ttl_vs_modeLR + 4;
                            flag_menuvsenthuman[ttl_vs_modeLR] = 1;
                        }
                    } else if (menufarmcl[ttl_vs_modeLR] == 2) {
                        menuvsmode[ttl_vs_modeLR] = ttl_vs_modeLR + 6;
                        flag_menuvsentzoid[ttl_vs_modeLR] = 1;
                    } else if (menufarmcl[ttl_vs_modeLR] == 1) {
                        menuvsmode[0] = 0;
                        flag_menuvsentzoid[0] = 0;
                        menuvsmode[1] = 1;
                        flag_menuvsentzoid[1] = 0;
                        flag_menuvsenthuman[0] = 0;
                        flag_menuvsenthuman[1] = 0;
                        menu_sDispVsEntry();
                        GameMode.field_2C = 0;
                        gt = GameMode.game_type = 2;
                        GameMode.game_type = 1;
                        map = GameMode.map;
                        f28 = GameMode.field_28;
                        if ((u8)menuZoidBattleModeSingleTeam == 0) {
                            a = 1;
                            b = 0;
                        } else if (ttl_vs_modeLR == 0) {
                            a = 1;
                            b = 0;
                        } else {
                            a = 0;
                            b = 1;
                        }
                        if (a)
                            menusaveloadslot = 0;
                        else
                            menusaveloadslot = 1;
                        if (!TEA_mcChkSlotZoid(menusaveloadslot)) {
                            OSReport("SaveLoad\x96\xb3\x82\xb5\n");
                            menuvsmode[0] = 0;
                            flag_menuvsentzoid[0] = 0;
                            menuvsmode[1] = 1;
                            flag_menuvsentzoid[1] = 0;
                            flag_menuvsenthuman[0] = 0;
                            flag_menuvsenthuman[1] = 0;
                            vsman = 2;
                            GameMode.game_type = 2;
                            break;
                        }
                        if (MemCardStatus[menusaveloadslot] != 0) {
                            OSReport("SaveLoad\x96\xb3\x82\xb5" "2\n");
                            TEA_mcMsg(menusaveloadslot, menusaveloadslot + 0x47c);
                            menuvsmode[0] = 0;
                            flag_menuvsentzoid[0] = 0;
                            menuvsmode[1] = 1;
                            i = vsman;
                            i = 0;
                            flag_menuvsentzoid[1] = 0;
                            flag_menuvsenthuman[0] = 0;
                            flag_menuvsenthuman[1] = 0;
                            vsman = 2;
                            GameMode.game_type = 2;
                            break;
                        }
                        menu_TITLE_dispVsEntry(0);
                        single = (u8)menuZoidBattleModeSingleTeam;
                        if (ttlMenuBatSaveLoad1P2P(0, a, b)) {
                            GameMode.game_type = 2;
                            OSReport("SaveLoadEnd\n");
                            IveSave[pl].owned_zoid_count = IveSave[2].owned_zoid_count;
                            for (j = 0; j < 30; j++) {
                                IveSave[pl].owned_zoids[j] = IveSave[2].owned_zoids[j];
                            }
                            for (i = 0; i < 30; i++) {
                                for (j = 0; j < 28; j++) {
                                    IveSave[pl].zoid_parts[i][j] = IveSave[2].zoid_parts[i][j];
                                    IveSave[pl].part_colors[i][j] = IveSave[2].part_colors[i][j];
                                }
                                for (j = 0; j < 3; j++) {
                                    IveSave[pl].zoid_field_11DA[i][j] = IveSave[2].zoid_field_11DA[i][j];
                                }
                                IveSave[pl].zoid_field_119E[i] = IveSave[2].zoid_field_119E[i];
                                IveSave[pl].zoid_color[i] = IveSave[2].zoid_color[i];
                            }
                            IveSave[pl].field_15F4 = IveSave[2].field_15F4;
                            IveSave[pl].zoid_slot = IveSave[2].zoid_slot;
                            IveSave[pl].field_15F8 = IveSave[2].field_15F8;
                            IveSave[pl].field_15FA = IveSave[2].field_15FA;
                            IveSave[pl].partner_slot = IveSave[2].partner_slot;
                            IveSave[pl].field_15FE = IveSave[2].field_15FE;
                            IveSave[pl].zoid_list_count = IveSave[2].zoid_list_count;
                            IveSave[pl].zoid_list_count = IveSave[2].zoid_list_count;
                            IveSave[pl].zoid_list_count = IveSave[2].zoid_list_count;
                            for (i = 0; i < 150; i++) {
                                IveSave[pl].zoid_list[i] = IveSave[2].zoid_list[i];
                            }
                            for (i = 0; i < 150; i++) {
                                IveSave[pl].zoid_got[i] = IveSave[2].zoid_got[i];
                            }
                            menufarmmh[pl + 6] = 4;
                            menufarmmn[pl + 6] = IveSave[pl].owned_zoid_count;
                            if (IveSave[pl].owned_zoid_count > menufarmmn[pl + 6])
                                menufarmmn[pl + 6] = IveSave[pl].owned_zoid_count;
                            if (IveSave[pl].owned_zoid_count < menufarmmh[pl + 6])
                                menufarmmh[pl + 6] = IveSave[pl].owned_zoid_count;
                            for (j = 0; j < IveSave[2].person_count; j++) {
                                IveSave[pl].persons[j] = IveSave[2].persons[j];
                            }
                            IveSave[pl].person_count = IveSave[2].person_count;
                            menufarmmh[pl + 4] = IveSave[2].person_count;
                            if (menufarmmh[pl + 4] >= 4)
                                menufarmmh[pl + 4] = 4;
                            menufarmmn[pl + 4] = IveSave[2].person_count;
                            menufarmsl[4] = 0;
                            menufarmcl[4] = 0;
                            menufarmst[4] = 0;
                            IveSave[pl].field_1600 = IveSave[pl].persons[0];
                            menufarmsl[5] = 0;
                            menufarmcl[5] = 0;
                            menufarmst[5] = 0;
                            IveSave[pl].field_1600 = IveSave[pl].persons[0];
                            GameMode.map = map;
                            GameMode.field_28 = f28;
                            GameMode.game_type = gt;
                            GameMode.win = 0;
                            sCamNN2 = 2;
                            sCamNN = 2;
                            menuSetTitle(1);
                            menuSetTitleButton(0x65);
                            GameMode.game_type = 2;
                            menufarmcl[pl + 6] = 0;
                            menufarmst[pl + 6] = 0;
                            menufarmsl[pl + 6] = 0;
                            IveSave[pl].zoid_slot = menufarmcl[pl + 6];
                            IveSave[pl].field_15F4 = IveSave[pl].owned_zoids[IveSave[pl].zoid_slot];
                            flag_menuvsentzoid[0] = 0;
                            flag_menuvsentzoid[1] = 0;
                            menuvsmode[0] = 0;
                            menuvsmode[1] = 1;
                            *(u8 *)&menuZoidBattleModeSingleTeam = single;
                            *(s8 *)&IveSave[pl].single_team = single;
                        } else {
                            GameMode.game_type = 2;
                            menuSetTitle(1);
                            menuSetTitleButton(0x65);
                            menuvsmode[0] = 0;
                            menuvsmode[1] = 1;
                            sPad1->pressed_a = 0;
                            sPad2->pressed_a = 0;
                            GameMode.map = map;
                            GameMode.field_28 = f28;
                            GameMode.game_type = gt;
                            *(u8 *)&menuZoidBattleModeSingleTeam = single;
                            *(s8 *)&IveSave[pl].single_team = single;
                            if (single == 0)
                                GameMode.game_type = 3;
                            if (single == 1)
                                GameMode.game_type = 2;
                            break;
                        }
                        GameMode.game_type = gt;
                        if (single == 0)
                            GameMode.game_type = 3;
                        if (single == 1)
                            GameMode.game_type = 2;
                        menuSetTitle(1);
                        menuSetTitleButton(11);
                    } else if (menufarmcl[pl] == 3) {
                        menu_TITLE_dispVsEntry(0);
                        detSaveNoSet(0);
                        return 8;
                    }
                    break;
                case 2:
                    SeRet();
                    break;
                case 4:
                    SeRet();
                    IveSave[ttl_vs_modeLR].field_1600 = IveSave[ttl_vs_modeLR].persons[menufarmcl[4]];
                    flag_menuvsenthuman[ttl_vs_modeLR] = 0;
                    menuvsmode[ttl_vs_modeLR] = 0;
                    TEA_VoiceOn((u8)IveSave[ttl_vs_modeLR].field_1600, 1);
                    break;
                case 5:
                    SeRet();
                    IveSave[ttl_vs_modeLR].field_1600 = IveSave[ttl_vs_modeLR].persons[menufarmcl[5]];
                    flag_menuvsenthuman[ttl_vs_modeLR] = 0;
                    menuvsmode[ttl_vs_modeLR] = 1;
                    TEA_VoiceOn((u8)IveSave[ttl_vs_modeLR].field_1600, 1);
                    break;
                case 6:
                    SeRet();
                    IveSave[ttl_vs_modeLR].zoid_slot = menufarmcl[6];
                    IveSave[ttl_vs_modeLR].field_15F4 = IveSave[ttl_vs_modeLR].owned_zoids[IveSave[ttl_vs_modeLR].zoid_slot];
                    flag_menuvsentzoid[ttl_vs_modeLR] = 0;
                    menuvsmode[ttl_vs_modeLR] = 0;
                    break;
                case 7:
                    SeRet();
                    IveSave[ttl_vs_modeLR].zoid_slot = menufarmcl[7];
                    IveSave[ttl_vs_modeLR].field_15F4 = IveSave[ttl_vs_modeLR].owned_zoids[IveSave[ttl_vs_modeLR].zoid_slot];
                    flag_menuvsentzoid[ttl_vs_modeLR] = 0;
                    menuvsmode[ttl_vs_modeLR] = 1;
                    break;
                }
            }
        }
        menu_sDispVsEntry();
        d0 = 0;
        d1 = 0;
        d2 = 0;
        d3 = 0;
    }
}
#undef lit_9566
#define lit_9566 "VS ENTRY\n"
#undef lit_9567
#define lit_9567 "VS ENTRY ST\n"
#undef lit_9568
#define lit_9568 "SaveLoad\x96\xb3\x82\xb5\n"
#undef lit_9569
#define lit_9569 "SaveLoad\x96\xb3\x82\xb5" "2\n"
#undef lit_9570
#define lit_9570 "SaveLoadEnd\n"
#pragma force_active on
static s32 VolSeTBL[11] = {
    0, 12, 24, 36, 48, 60, 72, 84, 
    96, 108, 120, 
};
static s32 VolBgmTBL[11] = {
    0, 12, 24, 36, 48, 60, 72, 84, 
    96, 108, 120, 
};
static s32 VolVoiceTBL[11] = {
    0, 12, 24, 36, 48, 60, 72, 84, 
    96, 108, 120, 
};
SampleVoice samplevoiceno[100] = {
    0, 0, 1, 0, 0, 20, 1, 0, 
    1, 1, 0, 34, 2, 0, 1, 2, 
    0, 9, 3, 0, 2, 3, 0, 11, 
    4, 0, 10, 4, 0, 25, 5, 0, 
    25, 5, 0, 10, 6, 0, 10, 6, 
    0, 27, 7, 0, 1, 7, 0, 49, 
    8, 0, 2, 8, 0, 6, 9, 0, 
    1, 9, 0, 8, 0xA, 0, 1, 0xA, 
    0, 7, 0xB, 0, 1, 0xB, 0, 54, 
    0xC, 0, 1, 0xC, 0, 7, 0xD, 0, 
    2, 0xD, 0, 7, 0xE, 0, 1, 0xE, 
    0, 7, 0x39, 0, 1, 0x39, 0, 10, 
    0x10, 0, 1, 0x10, 0, 10, 0x11, 0, 
    1, 0x11, 0, 7, 0x12, 0, 1, 0x12, 
    0, 7, 0x13, 0, 9, 0x13, 0, 14, 
    0x14, 0, 1, 0x14, 0, 10, 0x15, 0, 
    1, 0x15, 0, 7, 0x16, 0, 1, 0x16, 
    0, 7, 0x17, 0, 9, 0x17, 0, 25, 
    0x18, 0, 1, 0x18, 0, 7, 0x19, 0, 
    1, 0x19, 0, 7, 0x1A, 0, 1, 0x1A, 
    0, 49, 0x1B, 0, 2, 0x1B, 0, 9, 
    0x1C, 0, 1, 0x1C, 0, 10, 0x1D, 0, 
    1, 0x1D, 0, 9, 0x1E, 0, 1, 0x1E, 
    0, 9, 0x1F, 0, 1, 0x1F, 0, 27, 
    0x20, 0, 2, 0x20, 0, 56, 0x21, 0, 
    1, 0x21, 0, 28, 0x22, 0, 1, 0x22, 
    0, 58, 0x23, 0, 6, 0x23, 0, 10, 
    0x24, 0, 9, 0x24, 0, 6, 0x25, 0, 
    1, 0x25, 0, 9, 0x26, 0, 1, 0x26, 
    0, 26, 0x27, 0, 8, 0x27, 0, 9, 
    0x28, 0, 1, 0x28, 0, 28, 0x3B, 0, 
    1, 0x3B, 0, 8, 0x3C, 0, 1, 0x3C, 
    0, 7, 0x3D, 0, 1, 0x3D, 0, 6, 
    0x3E, 0, 89, 0x3E, 0, 23, 0x3F, 0, 
    10, 0x3F, 0, 89, 0x40, 0, 8, 0x40, 
    0, 90, 0x41, 0, 8, 0x41, 0, 89, 
    0x42, 0, 1, 0x42, 0, 92, 0x43, 0, 
    2, 0x43, 0, 10, 
};
u32 VsTimeTBL[5] = {
    1, 3, 5, 0xA, 0xFFFFFFFF, 
};
static s32 menuMax[7] = {
    2, 2, 1, 1, 1, 1, 0, 
};
s32 voiceSn[5][20] = {
    4, 54, 41, 77, 7, 0, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 5, 23, 6, 18, 
    1, 21, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    6, 34, 66, 19, 44, 4, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 
};
#pragma force_active off

void ttl_vsent_detailsetparam(s16 side)
{
    s16 a = side;

    if (ttl_vs_modeLR == 0 && sPad2->field_47 == 0) {
        if (menuvsmode[ttl_vs_modeLR] == 0) {
            if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 0) {
                detPerson(IveSave[ttl_vs_modeLR].field_1600);
            }
            if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 1) {
                menuDetailMode = 0;
            }
            if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 2) {
                detCustm(IveSave[ttl_vs_modeLR].zoid_slot);
            }
            if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 3) {
                menuDetailMode = 0;
            }
        }
        if (menuvsmode[ttl_vs_modeLR] == 4) {
            detPerson(IveSave[ttl_vs_modeLR].persons[menufarmcl[4]]);
        }
        if (menuvsmode[ttl_vs_modeLR] == 6) {
            detCustm(menufarmcl[6]);
        }
    }
    if (ttl_vs_modeLR == 1) {
        if (menuvsmode[ttl_vs_modeLR] == 1) {
            if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 0) {
                detPerson(IveSave[ttl_vs_modeLR].field_1600);
            }
            if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 1) {
                menuDetailMode = 0;
            }
            if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 2) {
                detCustm(IveSave[1].zoid_slot);
            }
            if (menufarmcl[menuvsmode[ttl_vs_modeLR]] == 3) {
                menuDetailMode = 0;
            }
        }
        if (menuvsmode[ttl_vs_modeLR] == 5) {
            detPerson(IveSave[ttl_vs_modeLR].persons[menufarmcl[5]]);
        }
        if (menuvsmode[ttl_vs_modeLR] == 7) {
            detCustm(menufarmcl[7]);
        }
    }
}

void ttl_vsent_setparamdispobjct(void)
{
    if ((u8)menuZoidBattleModeSingleTeam == 0) {
        if (menuvsmode[ttl_vs_modeLR] == 0) {
            if (menufarmcl[ttl_vs_modeLR] == 1) {
                flag_VsEntryLoadButton[ttl_vs_modeLR].b = 3;
                flag_VsEntryLoadButton[ttl_vs_modeLR].a = 1;
            } else {
                flag_VsEntryLoadButton[ttl_vs_modeLR].b = 2;
                flag_VsEntryLoadButton[ttl_vs_modeLR].a = 1;
            }
            flag_VsEntryLoadButton[1].b = 2;
            flag_VsEntryLoadButton[1].a = 1;
        }
        if (menuvsmode[ttl_vs_modeLR] == 1) {
            if (menufarmcl[ttl_vs_modeLR] == 1) {
                flag_VsEntryLoadButton[ttl_vs_modeLR].b = 3;
                flag_VsEntryLoadButton[ttl_vs_modeLR].a = 1;
            } else {
                flag_VsEntryLoadButton[ttl_vs_modeLR].b = 2;
                flag_VsEntryLoadButton[ttl_vs_modeLR].a = 1;
            }
            flag_VsEntryLoadButton[0].b = 2;
            flag_VsEntryLoadButton[0].a = 1;
        }
        printf("SingleMode=%d LR=%d vsmode=%d cl=%d \n", (u8)menuZoidBattleModeSingleTeam, ttl_vs_modeLR, menuvsmode[ttl_vs_modeLR], menufarmcl[ttl_vs_modeLR]);
    } else {
        if (menuvsmode[ttl_vs_modeLR] == ttl_vs_modeLR) {
            if (menufarmcl[ttl_vs_modeLR] == 1) {
                flag_VsEntryLoadButton[ttl_vs_modeLR].b = 3;
                flag_VsEntryLoadButton[ttl_vs_modeLR].a = 1;
            } else {
                flag_VsEntryLoadButton[ttl_vs_modeLR].b = 2;
                flag_VsEntryLoadButton[ttl_vs_modeLR].a = 1;
            }
        }
    }
}
#undef lit_9631
#define lit_9631 "SingleMode=%d LR=%d vsmode=%d cl=%d \n"

void menu_sDispVsFarm(void)
{
    s32 a = 0;
    s32 b = 0;
    s32 c = 0;
    s32 d = 0;
    s32 e = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if (menu_back1p != 0 && HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
        GXSetCullMode(2);
        jobjDispAll(((MenuArchive *)menu_back1p)->joints[0]);
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if (menu1p != 0 && HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
        HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
        HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
        GXSetCullMode(0);
        menu_sDispBattleFarmTitle();
        menu_sDispBattleFarmTitleSubButton();
        menu_sDispVsEntryPersonPartnerZoidPic();
        menu_sDispVsEntryPersonZoidStatus();
        menu_sDispVsFarmEntryButton();
        menu_sDispVsFarmStageButton();
        menu_sDispVsFarmBattleButton();
        menu_sDispVsFarmMapInfo();
        jobjAnim(((MenuArchive *)menu1p)->joints[5]);
        menu_sDispVsFarmPlayer1Icon();
        menu_sDispVsFarmPlayer2Icon();
        menu_sDispYesNoMenu();
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}

void menu_VsFarmMapLoad(s16 map)
{
    char path[80];

    if (menu1p_map != NULL) {
        PurgeArc2(menu1p_map);
        menu1p_map = 0;
    }
    sprintf(path, "menu/picture/m%02d.dat", mgP->map);
    menu1p_map = LoadArc2(path);
    map = map;
}
#undef lit_9671
#define lit_9671 "menu/picture/m%02d.dat"

void menu_sDispVsFarmMapInfo(void)
{
    s32 idx = 10;
    HSD_JObj *j2 = 0;
    HSD_JObj *j1 = 0;
    s32 a = 0;
    s16 b = 0;
    s16 c = 0;
    s16 d = 0;
    s32 e = 0;

    jobjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, -1, -1, -1));
    j1 = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, 0, -1, -1);
    j2 = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, 0, 0, -1);
    if ((u32)mgP != 0) {
        sPutFontRect((char *)mgP + 0x28, j1, j2, gxWhite, gxBack, 0, 2);
    }
    j1 = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, 2, -1, -1);
    j2 = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, 2, 0, -1);
    if ((u32)mgP != 0) {
        sPutFontRect((char *)mgP + 0x178, j1, j2, gxWhite, gxBack, 0, 2);
    }
    jobjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, 0, -1, -1));
    jobjDisp(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, 2, -1, -1));
    if (menu1p_map != NULL) {
        j1 = GetChild(((MenuArchive *)menu1p)->joints[idx], 0, 0, 1, -1, -1);
        j2 = GetChild(((MenuArchive *)menu1p_map)->joints[0], 0, -1, -1, -1, -1);
        jobjCopyPos(j1, j2);
        jobjDispAll(j2);
    }
}

void menu_sDispVsFarmEntryButton(void)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    s32 idx = 3;
    s32 p = 0;
    s32 dead = 0;
    s32 z = 0;

    if (flag_VsFarmEntryButton[0] != 0) {
        if (flag_VsFarmEntryButton[0] == 1) {
            flag_VsFarmEntryButton[0] = 2;
            SetAnimArc2((MenuArchive *)menu1p, idx, flag_VsFarmEntryButton[1]);
        }
        if (flag_VsFarmEntryButton[0] == 2) {
            p = jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
            sMakeMatrix(((MenuArchive *)menu1p)->joints[idx]);
            if (p != 0) {
                flag_VsFarmEntryButton[0] = 3;
                switch (flag_VsFarmEntryButton[1]) {
                case 0:
                    flag_VsFarmEntryButton[1] = 1;
                    flag_VsFarmEntryButton[0] = 2;
                    break;
                case 5:
                    flag_VsFarmEntryButton[0] = 0;
                    break;
                default:
                    flag_VsFarmEntryButton[0] = 2;
                    break;
                }
                SetAnimArc2((MenuArchive *)menu1p, idx, flag_VsFarmEntryButton[1]);
                jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
            }
        }
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, z, -1, -1, -1), 0, 7, 0);
    }
    dead = 0;
}

void menu_sDispVsFarmStageButton(void)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    s32 idx = 3;
    s32 jobj = 0;
    s32 dead = 0;
    s32 e = 1;

    if (flag_VsFarmEntryButton[0] == 0) {
        return;
    }
    if (flag_VsFarmEntryButton[0] == 1) {
        flag_VsFarmEntryButton[0] = 2;
        SetAnimArc2((MenuArchive *)menu1p, idx, flag_VsFarmEntryButton[1]);
    }
    if (flag_VsFarmEntryButton[0] == 2) {
        jobj = jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
        sMakeMatrix(((MenuArchive *)menu1p)->joints[idx]);
        if (jobj != 0) {
            flag_VsFarmEntryButton[0] = 3;
            switch (flag_VsFarmEntryButton[1]) {
            case 0:
                flag_VsFarmEntryButton[1] = 1;
                flag_VsFarmEntryButton[0] = 2;
                break;
            case 5:
                flag_VsFarmEntryButton[0] = 0;
                break;
            default:
                flag_VsFarmEntryButton[0] = 2;
                break;
            }
            SetAnimArc2((MenuArchive *)menu1p, idx, flag_VsFarmEntryButton[1]);
            jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
        }
    }
    HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, e, -1, -1, -1), 0, 7, 0);
    dead = 0;
}

void menu_sDispVsFarmBattleButton(void)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    register s32 idx = 3;
    register s32 ok = 0;
    register s32 dead = 0;
    s32 two = 2;
    s32 d = 0;

    if (flag_VsFarmEntryButton[0] != 0) {
        if (flag_VsFarmEntryButton[0] == 1) {
            flag_VsFarmEntryButton[0] = 2;
            SetAnimArc2((MenuArchive *)menu1p, idx, flag_VsFarmEntryButton[1]);
        }
        if (flag_VsFarmEntryButton[0] == 2) {
            ok = jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
            sMakeMatrix(((MenuArchive *)menu1p)->joints[idx]);
            if (ok != 0) {
                flag_VsFarmEntryButton[0] = 3;
                switch (flag_VsFarmEntryButton[1]) {
                case 0:
                    flag_VsFarmEntryButton[1] = 1;
                    flag_VsFarmEntryButton[0] = 2;
                    break;
                case 5:
                    flag_VsFarmEntryButton[0] = 0;
                    break;
                default:
                    flag_VsFarmEntryButton[0] = 2;
                    break;
                }
                SetAnimArc2((MenuArchive *)menu1p, idx, flag_VsFarmEntryButton[1]);
                jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
            }
        }
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, two, -1, -1, -1), 0, 7, 0);
    }
}

void menu_sDispVsFarmExitButton(void)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    s32 idx = 3;
    s32 jobj = 0;
    s32 dead = 0;
    s32 e = 3;

    if (flag_VsFarmEntryButton[0] == 0) {
        return;
    }
    if (flag_VsFarmEntryButton[0] == 1) {
        flag_VsFarmEntryButton[0] = 2;
        SetAnimArc2((MenuArchive *)menu1p, idx, flag_VsFarmEntryButton[1]);
    }
    if (flag_VsFarmEntryButton[0] == 2) {
        jobj = jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
        sMakeMatrix(((MenuArchive *)menu1p)->joints[idx]);
        if (jobj != 0) {
            flag_VsFarmEntryButton[0] = 3;
            switch (flag_VsFarmEntryButton[1]) {
            case 0:
                flag_VsFarmEntryButton[1] = 1;
                flag_VsFarmEntryButton[0] = 2;
                break;
            case 5:
                flag_VsFarmEntryButton[0] = 0;
                break;
            default:
                flag_VsFarmEntryButton[0] = 2;
                break;
            }
            SetAnimArc2((MenuArchive *)menu1p, idx, flag_VsFarmEntryButton[1]);
            jobjAnim(((MenuArchive *)menu1p)->joints[idx]);
        }
    }
    HSD_JObjDispAll(GetChild(((MenuArchive *)menu1p)->joints[idx], 0, e, -1, -1, -1), 0, 7, 0);
    dead = 0;
}

void menu_sDispVsFarmPlayer1Icon(void)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    HSD_JObj *target = 0;
    HSD_JObj *source = 0;
    s32 x = 0;
    s32 y = 0;

    x = 0;
    y = 0;
    flag_VsFarmPlayer1Icon = menufarmcl[0];
    if (flag_VsFarmPlayer1Icon == -1)
        return;
    if (flag_VsFarmPlayer1Icon <= 2) {
        source = GetChild(((MenuArchive *)menu1p)->joints[3], 0, flag_VsFarmPlayer1Icon, 1, -1, -1);
        target = GetChild(((MenuArchive *)menu1p)->joints[5], 0, 1, -1, -1, -1);
    } else {
        source = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 0, -1, -1, -1);
        target = GetChild(((MenuArchive *)menu1p)->joints[5], 0, 2, -1, -1, -1);
    }
    jobjCopyPos(source, target);
    jobjDispAll(target);
}

void menu_sDispVsFarmPlayer2Icon(void)
{
    s16 a = 0;
    s16 b = 0;
    s16 c = 0;
    s32 d = 0;
    register HSD_JObj *dst = 0;
    HSD_JObj *src = 0;
    s32 e = 0;
    s32 f = 0;

    e = 0;
    f = 0;
    flag_VsFarmPlayer2Icon = menufarmcl[1];
    if (flag_VsFarmPlayer2Icon != -1 && (u8)menuZoidBattleModeSingleTeam == 1) {
        if (flag_VsFarmPlayer2Icon <= 2) {
            src = GetChild(((MenuArchive *)menu1p)->joints[3], 0, flag_VsFarmPlayer2Icon, 2, -1, -1);
            dst = GetChild(((MenuArchive *)menu1p)->joints[5], 0, 0, -1, -1, -1);
        } else {
            src = GetChild(((MenuArchive *)menu1p)->joints[6], 0, 1, -1, -1, -1);
            dst = GetChild(((MenuArchive *)menu1p)->joints[5], 0, 2, -1, -1, -1);
        }
        jobjCopyPos(src, dst);
        jobjDispAll(dst);
    }
}

s32 vsZoidBig1(s32 player)
{
    s32 zoid;
    s32 key;

    zoid = IveSave[player].field_15F4;
    key = Tea_zoiddat[zoid].sort_key;
    return key == 999;
}

s32 vsZoidBig(void)
{
    if (vsZoidBig1(0) != 0)
        return 1;
    if (vsZoidBig1(1) != 0)
        return 1;
    return 0;
}
s32 vsZoidStage(void)
{
    if (mgP == 0)
        return 1;
    if (mgP->field_321B && !IveSaveFlag.system_flags[3])
        return 1;
    if (mgP->field_3219 == 0)
        return 0;
    return vsZoidBig();
}

s32 nextStage(void)
{
    void *info;

    do {
        stage00++;
        info = rdMapInfo(3, stage00);
        if (info == 0) {
            if (vsMapMax > stage00)
                vsMapMax = stage00 - 1;
            stage00 = 0;
            info = rdMapInfo(3, stage00);
        }
        if (info != 0 && vsZoidStage() != 0)
            info = 0;
    } while (info == 0);
    return 1;
}

s32 backStage(void)
{
    void *info;

    do {
        stage00--;
        if (stage00 < 0)
            stage00 = vsMapMax;
        info = rdMapInfo(3, stage00);
        if (info == 0 && vsMapMax > stage00)
            vsMapMax = stage00 - 1;
        if (info != 0 && vsZoidStage() != 0)
            info = 0;
    } while (info == 0);
    return 1;
}

void vsMapMaxClear(void)
{
    vsMapMax = 20;
}

s32 menufncZoidVsMenu(void)
{
    s32 u0 = 0;
    s32 u1 = 0;
    s32 u2 = -240;
    s32 u3 = 0;
    s32 u4 = 0;
    u8 u5 = 0;
    s16 bgm = 3;
    s32 i = 0;
    s32 dead = 0;
    s16 j = 0;

    configBak();
    menuCursor = 0;
    menuParticles = 0;
    vsMapMax = 99;
    menumode = 0;
    for (i = 0; i < 20; i++) {
        menumodeanm[i] = 0;
    }
    for (i = 0; i < 20; i++) {
        menufarmanm[i] = 2;
        menufarmanmold[i] = 2;
    }
    OSReport("MenuZoidVs\n");
    vsyncN = 0;
    flag_VsFarmPlayer1Icon = 0;
    flag_VsFarmPlayer2Icon = 2;
    TeaMenuBGM(4);
    menuSetTitle(0);
    OSReport("title aaaaa");
    menuSetTitleButton(11);
    TEA_setvsync();
    menufarmmode = 0;
    menumodeanm[0] = 0;
    menufarmanm[0] = 0;
    menufarmcl[0] = 0;
    menufarmsl[0] = 0;
    menufarmst[0] = 0;
    menufarmmh[0] = 3;
    menufarmmn[0] = 3;
    menufarmcl[1] = 0;
    menufarmsl[1] = 0;
    menufarmst[1] = 0;
    menufarmmh[1] = 3;
    menufarmmn[1] = 3;
    menufarmcl[2] = 0;
    menufarmsl[2] = 0;
    menufarmst[2] = 0;
    menufarmmh[2] = 2;
    menufarmmn[2] = 2;
    menufarmcl[3] = 0;
    menufarmsl[3] = 0;
    menufarmst[3] = 0;
    menufarmmh[3] = 2;
    menufarmmn[3] = 2;
    menufarmcl[4] = 0;
    menufarmsl[4] = 0;
    menufarmst[4] = 0;
    menufarmmh[4] = 2;
    menufarmmn[4] = 2;
    menubackanm[0].state = 1;
    menubackanm[0].cur = 1;
    menubackanm[0].old = 0;
    stage00 = -1;
    nextStage();
    menu_VsFarmMapLoad(stage00);
    flag_VsFarmEntryButton[1] = 0;
    flag_VsFarmEntryButton[0] = 1;
    flag_VsFarmStageButton[1] = 0;
    flag_VsFarmStageButton[0] = 1;
    flag_VsFarmBattleButton[1] = 0;
    flag_VsFarmBattleButton[0] = 1;
    flag_VsFarmExitButton[1] = 0;
    flag_VsFarmExitButton[0] = 1;
    flag_VsFarmEntryStageBattleExit = 1;
    for (i = 1; i <= 9; i++) {
        menufarmanm[i] = 0;
    }
    memcpy(&IveSave[4], &IveSave[0], 0x1a90);
    menumodeanm[0] = 1;
    menufarmanm[0] = 0;
    for (i = 1; i <= 7; i++) {
        menumodeanm[i] = 1;
        menufarmanm[i] = 0;
    }
    menucuranm[0] = 1;
    OSReport("VsFarm 01 \n");
    for (;;) {
        dead = 1;
        dead = 0;
        dead = 1;
        bgm = 0;
        if (vsZoidStage()) {
            nextStage();
            menu_VsFarmMapLoad(stage00);
        }
        sPadAll();
        for (j = 0; j < 2; j++) {
            if (j == 0 && menufarmcl[j] != 3) {
                menufarmcl[j] = TEA_menufnc(&menufarmsl[j], &menufarmst[j], menufarmmn[j], menufarmmh[j]);
            }
            if (j == 1 && menufarmcl[j] != 3) {
                menufarmcl[j] = TEA_menufnc2p((s32)&menufarmsl[j], (s32)&menufarmst[j], menufarmmn[j], menufarmmh[j]);
            }
        }
        menu_sDispVsFarm();
        if ((u8)menuZoidBattleModeSingleTeam == 0) {
            if (sPad2->field_5E || sPad2->pressed_a || sPad2->field_5A || sPad2->field_59)
                continue;
        }
        if (sPad1->field_5E || sPad2->field_5E) {
            if (menufarmcl[0] == 3 && sPad1->field_5E) {
                menufarmcl[0] = 2;
            } else if (menufarmcl[1] == 3 && sPad2->field_5E) {
                menufarmcl[1] = 2;
            } else {
                menudispyesnoflag = 1;
                menudispyesnopoint = 1;
                do {
                    sPadAll();
                    menudispyesnopoint = menuYesNo(menudispyesnopoint, lit_574, GetMenuMsgAdr(100));
                    menu_sDispVsFarm();
                } while (!sPad1->pressed_a && !sPad2->pressed_a);
                menudispyesnoflag = 0;
                SeRet();
                if (menudispyesnopoint == 0)
                    continue;
                if (menu1p_map != (void *)0) {
                    PurgeArc2(menu1p_map);
                    menu1p_map = 0;
                }
                TeaBgm(-1);
                ttlMenuAllPurge();
                menuretno = 8;
                GameMode.field_38 = 1;
                resetSkipOp();
            }
        }
        if (menufarmcl[0] == 2 && sPad1->pressed_a)
            menufarmcl[0] = 3;
        if (menufarmcl[1] == 2 && sPad2->pressed_a)
            menufarmcl[1] = 3;
        if ((u8)menuZoidBattleModeSingleTeam == 0 && menufarmcl[0] == 3)
            menufarmcl[1] = 3;
        if (menufarmcl[0] == 3 && menufarmcl[1] == 3) {
            SeRet();
            IveSave[0].single_team = menuZoidBattleModeSingleTeam;
            if ((u8)menuZoidBattleModeSingleTeam == 0)
                GameMode.game_type = 3;
            if ((u8)menuZoidBattleModeSingleTeam == 1)
                GameMode.game_type = 2;
            for (i = 0; i < 10; i++) {
                GameZoid[i].type = -1;
            }
            menuZoidBattleGoPartsOptSet_GameZoid(0, 0, IveSave[0].zoid_slot, 0);
            menuZoidBattleGoPartsOptSet_GameZoid(5, 1, IveSave[1].zoid_slot, 0);
            menu_TITLE_dispVs(0);
            HSD_VISetBlack(1);
            menu_sDispVsFarm();
            if (menu1p_map != (void *)0) {
                PurgeArc2(menu1p_map);
                menu1p_map = 0;
            }
            GameMode.field_28 = stage00;
            GameMode.map = stage00;
            memcpy(&IveSave[4], &IveSave[0], 0x1a90);
            memcpy(&IveSave[5], &IveSave[1], 0x1a90);
            GameMode.win = 1;
            TeaBgm(-1);
            ttlMenuAllPurge();
            configBakRet();
            menuretno = 0x270f;
            return 210;
        }
        if ((menufarmcl[0] == 0 && sPad1->pressed_a) || (menufarmcl[1] == 0 && sPad2->pressed_a)) {
            SeRet();
            menuanmend = 0;
            OSReport((char *)lit_5845);
            menu_TITLE_dispVs(0);
            menufncZoidVsMenuEntry();
            menuSetTitle(0);
            menuSetTitleButton(11);
            menu_sDispVsFarm();
            menufarmcl[0] = 0;
            menufarmsl[0] = 0;
            menufarmst[0] = 0;
            menufarmmh[0] = 3;
            menufarmmn[0] = 3;
            menufarmcl[1] = 0;
            menufarmsl[1] = 0;
            menufarmst[1] = 0;
            menufarmmh[1] = 3;
            menufarmmn[1] = 3;
            continue;
        }
        if ((menufarmcl[0] == 1 && sPad1->field_5A) || (menufarmcl[1] == 1 && sPad2->field_5A)) {
            nextStage();
            GameMode.field_28 = stage00;
            menu_VsFarmMapLoad(GameMode.map);
            continue;
        }
        if ((menufarmcl[0] == 1 && sPad1->field_59) || (menufarmcl[1] == 1 && sPad2->field_59)) {
            backStage();
            GameMode.field_28 = stage00;
            menu_VsFarmMapLoad(GameMode.map);
            continue;
        }
    }
}
#undef lit_9964
#define lit_9964 "MenuZoidVs\n"
#undef lit_9965
#define lit_9965 "title aaaaa"
#undef lit_9966
#define lit_9966 "VsFarm 01 \n"

void menu_TITLE_dispVs(s32 state)
{
    s16 i = 0;

    menuanmend = 0;
    menutitleanm[0].mode = 1;
    for (i = 1; i < 7; i++)
        menutitleanm[i].mode = 1;
    if (state == 1) {
        menutitleanm[0].state = 0;
        for (i = 1; i < 7; i++)
            menutitleanm[i].state = 0;
    } else {
        menutitleanm[0].state = 2;
        for (i = 1; i < 7; i++)
            menutitleanm[i].state = 2;
    }
    while (menuanmend == 0) {
        menu_sDispVsFarm();
    }
    menuanmend = 0;
}

void menu_sDispVsFistFarm(void)
{
    s32 unused0 = 0;
    s32 unused1 = 0;
    s32 unused2 = 0;
    s32 unused3 = 0;
    s32 unused4 = 0;

    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if ((u32)menu_back1p != 0) {
        if (HSD_CObjSetCurrent(((MenuArchive *)menu_back1p)->cobj) != 0) {
            HSD_LObjSetCurrentAll(((MenuArchive *)menu_back1p)->lobj);
            HSD_LObjSetupInit(((MenuArchive *)menu_back1p)->cobj);
            GXSetCullMode(2);
            jobjDispAll(((MenuArchive *)menu_back1p)->joints[0]);
            HSD_CObjEndCurrent();
            GXSetCullMode(2);
        }
    }
    HSD_SetZSortMode(0, 0);
    if ((u32)menu1p != 0) {
        if (HSD_CObjSetCurrent(((MenuArchive *)menu1p)->cobj) != 0) {
            HSD_LObjSetCurrentAll(((MenuArchive *)menu1p)->lobj);
            HSD_LObjSetupInit(((MenuArchive *)menu1p)->cobj);
            GXSetCullMode(0);
            menu_sDispBattleFarmTitle();
            menu_sDispBattleFarmTitleSubButton();
            menu_sDispVsEntryPersonPartnerZoidPic();
            menu_sDispVsEntryPersonZoidStatus();
            menu_sDispVsFarmEntryButton();
            menu_sDispVsFarmStageButton();
            menu_sDispVsFarmBattleButton();
            menu_sDispVsFarmMapInfo();
            jobjAnim(((MenuArchive *)menu1p)->joints[5]);
            menu_sDispVsFarmPlayer1Icon();
            menu_sDispVsFarmPlayer2Icon();
            menu_sDispYesNoMenu();
            HSD_CObjEndCurrent();
            GXSetCullMode(2);
        }
    }
    HSD_EndRender();
    debugmenudraw();
    vsyncN++;
}

s32 menufncZoidVsFistMenu(void)
{
    s32 u0 = 0;
    s32 u1 = 0;
    s32 u2 = -240;
    s32 u3 = 0;
    s32 u4 = 0;
    u8 u5 = 0;
    s32 i = 0;
    s16 bgm = 3;
    s32 dead = 0;
    s16 j = 0;

    menuCursor = 0;
    menuParticles = 0;
    menumode = 0;
    for (i = 0; i < 20; i++) {
        menumodeanm[i] = 0;
    }
    for (i = 0; i < 20; i++) {
        menufarmanm[i] = 2;
        menufarmanmold[i] = 2;
    }
    OSReport("MenuZoidVs\n");
    vsyncN = 0;
    flag_VsFarmPlayer1Icon = 0;
    flag_VsFarmPlayer2Icon = 2;
    bgm = 5;
    vsMapMaxClear();
    TeaMenuBGM(bgm);
    menuSetTitle(0);
    OSReport("title aaaaa");
    menuSetTitleButton(11);
    TEA_setvsync();
    menufarmmode = 0;
    menumodeanm[0] = 0;
    menufarmanm[0] = 0;
    menufarmcl[0] = 0;
    menufarmsl[0] = 0;
    menufarmst[0] = 0;
    menufarmmh[0] = 3;
    menufarmmn[0] = 3;
    menufarmcl[1] = 0;
    menufarmsl[1] = 0;
    menufarmst[1] = 0;
    menufarmmh[1] = 3;
    menufarmmn[1] = 3;
    menufarmcl[2] = 0;
    menufarmsl[2] = 0;
    menufarmst[2] = 0;
    menufarmmh[2] = 2;
    menufarmmn[2] = 2;
    menufarmcl[3] = 0;
    menufarmsl[3] = 0;
    menufarmst[3] = 0;
    menufarmmh[3] = 2;
    menufarmmn[3] = 2;
    menufarmcl[4] = 0;
    menufarmsl[4] = 0;
    menufarmst[4] = 0;
    menufarmmh[4] = 2;
    menufarmmn[4] = 2;
    menubackanm[0].state = 1;
    menubackanm[0].cur = 1;
    menubackanm[0].old = 0;
    stage00 = -1;
    nextStage();
    menu_VsFarmMapLoad(stage00);
    flag_VsFarmEntryButton[1] = 0;
    flag_VsFarmEntryButton[0] = 1;
    flag_VsFarmStageButton[1] = 0;
    flag_VsFarmStageButton[0] = 1;
    flag_VsFarmBattleButton[1] = 0;
    flag_VsFarmBattleButton[0] = 1;
    flag_VsFarmExitButton[1] = 0;
    flag_VsFarmExitButton[0] = 1;
    flag_VsFarmEntryStageBattleExit = 1;
    for (i = 1; i <= 9; i++) {
        menufarmanm[i] = 0;
    }
    memcpy(&IveSave[4], &IveSave[0], 0x1a90);
    menumodeanm[0] = 1;
    menufarmanm[0] = 0;
    for (i = 1; i <= 7; i++) {
        menumodeanm[i] = 1;
        menufarmanm[i] = 0;
    }
    menucuranm[0] = 1;
    OSReport("VsFarm 01 \n");
    for (;;) {
        dead = 1;
        dead = 0;
        dead = 1;
        if (vsZoidStage()) {
            nextStage();
            menu_VsFarmMapLoad(stage00);
        }
        sPadAll();
        for (j = 0; j < 2; j++) {
            if (j == 0 && menufarmcl[j] != 3) {
                menufarmcl[j] = TEA_menufnc(&menufarmsl[j], &menufarmst[j], menufarmmn[j], menufarmmh[j]);
            }
            if (j == 1 && menufarmcl[j] != 3) {
                menufarmcl[j] = TEA_menufnc2p((s32)&menufarmsl[j], (s32)&menufarmst[j], menufarmmn[j], menufarmmh[j]);
            }
        }
        menu_sDispVsFistFarm();
        if ((u8)menuZoidBattleModeSingleTeam == 0) {
            if (sPad2->field_5E || sPad2->pressed_a || sPad2->field_5A || sPad2->field_59)
                continue;
        }
        if (sPad1->field_5E || sPad2->field_5E) {
            if (menufarmcl[0] == 3 && sPad1->field_5E) {
                menufarmcl[0] = 2;
            } else if (menufarmcl[1] == 3 && sPad2->field_5E) {
                menufarmcl[1] = 2;
            } else {
                menudispyesnoflag = 1;
                menudispyesnopoint = 1;
                do {
                    sPadAll();
                    menudispyesnopoint = menuYesNo(menudispyesnopoint, lit_574, GetMenuMsgAdr(100));
                    menu_sDispVsFistFarm();
                } while (!sPad1->pressed_a && !sPad2->pressed_a);
                menudispyesnoflag = 0;
                SeRet();
                if (menudispyesnopoint == 0)
                    continue;
                if (menu1p_map != (void *)0) {
                    PurgeArc2(menu1p_map);
                    menu1p_map = 0;
                }
                TeaBgm(-1);
                ttlMenuAllPurge();
                menuretno = 8;
                GameMode.field_38 = 1;
                resetSkipOp();
            }
        }
        if (menufarmcl[0] == 2 && sPad1->pressed_a)
            menufarmcl[0] = 3;
        if (menufarmcl[1] == 2 && sPad2->pressed_a)
            menufarmcl[1] = 3;
        if ((u8)menuZoidBattleModeSingleTeam == 0 && menufarmcl[0] == 3)
            menufarmcl[1] = 3;
        if (menufarmcl[0] == 3 && menufarmcl[1] == 3) {
            SeRet();
            IveSave[0].single_team = menuZoidBattleModeSingleTeam;
            if ((u8)menuZoidBattleModeSingleTeam == 0)
                GameMode.game_type = 3;
            if ((u8)menuZoidBattleModeSingleTeam == 1)
                GameMode.game_type = 2;
            for (i = 0; i < 10; i++) {
                GameZoid[i].type = -1;
            }
            menuZoidBattleGoPartsOptSet_GameZoid(0, 0, IveSave[0].zoid_slot, 0);
            menuZoidBattleGoPartsOptSet_GameZoid(5, 1, IveSave[1].zoid_slot, 0);
            menu_TITLE_dispVsFist(0);
            HSD_VISetBlack(1);
            menu_sDispVsFistFarm();
            if (menu1p_map != (void *)0) {
                PurgeArc2(menu1p_map);
                menu1p_map = 0;
            }
            GameMode.field_28 = stage00;
            GameMode.map = stage00;
            memcpy(&IveSave[4], &IveSave[0], 0x1a90);
            memcpy(&IveSave[5], &IveSave[1], 0x1a90);
            GameMode.win = 1;
            TeaBgm(-1);
            ttlMenuAllPurge();
            menuretno = 0x270f;
            return 210;
        }
        if ((menufarmcl[0] == 0 && sPad1->pressed_a) || (menufarmcl[1] == 0 && sPad2->pressed_a)) {
            SeRet();
            menuanmend = 0;
            OSReport((char *)lit_5845);
            menu_TITLE_dispVsFist(0);
            menufncZoidVsMenuEntry();
            menuSetTitle(0);
            menuSetTitleButton(11);
            menu_sDispVsFistFarm();
            menufarmcl[0] = 0;
            menufarmsl[0] = 0;
            menufarmst[0] = 0;
            menufarmmh[0] = 3;
            menufarmmn[0] = 3;
            menufarmcl[1] = 0;
            menufarmsl[1] = 0;
            menufarmst[1] = 0;
            menufarmmh[1] = 3;
            menufarmmn[1] = 3;
            continue;
        }
        if ((menufarmcl[0] == 1 && sPad1->field_5A) || (menufarmcl[1] == 1 && sPad2->field_5A)) {
            nextStage();
            GameMode.field_28 = stage00;
            menu_VsFarmMapLoad(GameMode.map);
            continue;
        }
        if ((menufarmcl[0] == 1 && sPad1->field_59) || (menufarmcl[1] == 1 && sPad2->field_59)) {
            backStage();
            GameMode.field_28 = stage00;
            menu_VsFarmMapLoad(GameMode.map);
            continue;
        }
    }
}

void menu_TITLE_dispVsFist(s32 mode)
{
    s16 i = 0;

    menuanmend = 0;
    menutitleanm[0].mode = 1;
    for (i = 1; i < 7; i++)
        menutitleanm[i].mode = 1;
    if (mode == 1) {
        menutitleanm[0].state = 0;
        for (i = 1; i < 7; i++)
            menutitleanm[i].state = 0;
    } else {
        menutitleanm[0].state = 2;
        for (i = 1; i < 7; i++)
            menutitleanm[i].state = 2;
    }
    while (menuanmend == 0)
        menu_sDispVsFistFarm();
    menuanmend = 0;
}

void configSet(void)
{
    TEA_VOICEVOL = (s16)VolVoiceTBL[VolVoice];
    TEA_BGMVOL = (s16)VolBgmTBL[VolBgm];
    TEA_SEVOL = (s16)VolSeTBL[VolSe];
    OSReport("config voice  volume %d  bgm%d\n", VolVoice, VolBgm);
}
#undef lit_10139
#define lit_10139 "config voice  volume %d  bgm%d\n"

void configGet(void)
{
    VolSe = IveSaveFlag.cfg_se;
    VolBgm = IveSaveFlag.cfg_bgm;
    VolVoice = IveSaveFlag.cfg_voice;
    rumble = IveSaveFlag.cfg_rumble;
    VsTime = IveSaveFlag.cfg_vstime;
    VsMode = IveSaveFlag.cfg_vsmode;
    configSet();
}

void configPut(void)
{
    IveSaveFlag.cfg_se = VolSe;
    IveSaveFlag.cfg_bgm = VolBgm;
    IveSaveFlag.cfg_voice = VolVoice;
    IveSaveFlag.cfg_rumble = rumble;
    IveSaveFlag.cfg_vstime = VsTime;
    IveSaveFlag.cfg_vsmode = VsMode;
}

void configClear(void)
{
    IveSaveFlag.cfg_se = 8;
    IveSaveFlag.cfg_bgm = 8;
    IveSaveFlag.cfg_voice = 8;
    IveSaveFlag.cfg_rumble = 1;
    IveSaveFlag.cfg_vstime = 1;
    IveSaveFlag.cfg_vsmode = 1;
    configGet();
}

void configBak(void)
{
    VolSe3 = VolSe;
    VolBgm3 = VolBgm;
    VolVoice3 = VolVoice;
    rumble93 = rumble;
    VsTime3 = VsTime;
    VsMode3 = VsMode;
}

void configBakRet(void)
{
    VolSe = VolSe3;
    VolBgm = VolBgm3;
    VolVoice = VolVoice3;
    rumble = rumble93;
    VsTime = VsTime3;
    VsMode = VsMode3;
    configSet();
}

void voiceSn2i(void)
{
    s32 i;

    for (i = 0; i < 20; i++)
        voiceSn2[i] = -1;
}

void voiceSn2a(s32 voice)
{
    s32 i;
    s32 j;

    for (i = 19; i > 0; i--) {
        if ((j = i - 1) < 0)
            return;
        voiceSn2[i] = voiceSn2[j];
    }
    voiceSn2[0] = voice;
}

s32 voiceSn2c1(s32 kind)
{
    s32 i;
    s32 voice;

    for (i = 0; i < 20; i++) {
        voice = voiceSn[kind][i + 1];
        if (voice == -1)
            break;
        if (voice != voiceSn2[i])
            return -1;
    }
    return voiceSn[kind][0];
}

s32 voiceSn2c(void)
{
    s32 i;
    s32 result;

    for (i = 0; i < 3; i++) {
        result = voiceSn2c1(i);
        if (result != -1)
            return result;
    }
    return -1;
}

void getSubMenuT(void)
{
    s32 base[7] = { 0, 2, 4, 5, 6, 7, -1 };

    if (crN == -1) {
        sbT = -1;
        return;
    }
    if (sbN < 0)
        sbN = 0;
    if (sbN >= menuMax[crN])
        sbN = menuMax[crN];
    sbT = sbN + base[crN];
}

void sub0(void)
{
    if (sPad1->field_59)
        VolBgm--;
    if (sPad1->field_5A)
        VolBgm++;
    if (VolBgm < 0)
        VolBgm = 0;
    if (VolBgm >= 10)
        VolBgm = 10;
    TEA_BGMVOL = (s16)VolBgmTBL[VolBgm];
    if (sPad1->pressed_a) {
        TeaBgm(-1);
        TeaBgm(TestBgm);
    }
    TEA_AudioSeqVolume(TEA_BGMVOL, 10, 0);
}
void sub1(void)
{
    if (sPad1->field_59)
        TestBgm--;
    if (sPad1->field_5A)
        TestBgm++;
    if (TestBgm <= 0)
        TestBgm = 1;
    if (TestBgm >= TEA_BGMMAXNUM - 1)
        TestBgm = TEA_BGMMAXNUM - 1;
    if (sPad1->pressed_a) {
        TeaBgm(-1);
        TeaBgm(TestBgm);
    }
}

void sub2(void)
{
    if (sPad1->field_59)
        VolVoice--;
    if (sPad1->field_5A)
        VolVoice++;
    if (VolVoice < 0)
        VolVoice = 0;
    if (VolVoice >= 10)
        VolVoice = 10;
    TEA_VOICEVOL = (s16)VolVoiceTBL[VolVoice];
    if (sPad1->pressed_a)
        TEA_VoiceOn(samplevoiceno[TestVoice].character, samplevoiceno[TestVoice].voice);
}

void sub3(void)
{
    s32 flag;

    if (sPad1->field_59)
        TestVoice--;
    if (sPad1->field_5A)
        TestVoice++;
    if (TestVoice < 0)
        TestVoice = 0;
    if ((u32)TestVoice >= 99)
        TestVoice = 99;
    if (sPad1->pressed_a) {
        TEA_VoiceOn(samplevoiceno[TestVoice].character, samplevoiceno[TestVoice].voice);
        voiceSn2a(TestVoice);
        flag = voiceSn2c();
        if (flag != -1) {
            OSReport("add sys-flag[%d] \n", flag);
            IveSaveFlag.system_flags[flag] = 1;
        }
    }
}
#undef lit_10232
#define lit_10232 "add sys-flag[%d] \n"

void sub4(void)
{
    if (sPad1->field_59)
        VolSe--;
    if (sPad1->field_5A)
        VolSe++;
    if (VolSe < 0)
        VolSe = 0;
    if (VolSe >= 10)
        VolSe = 10;
    TEA_SEVOL = (s16)VolSeTBL[VolSe];
    if (sPad1->pressed_a)
        SeOn(0x1D);
}

void sub5(void)
{
    s32 old = rumble;

    if (sPad1->field_59)
        rumble = 1;
    if (sPad1->field_5A)
        rumble = 0;
    if (old == 0 && rumble != 0)
        rumbleTm = 30;
}

void sub6(void)
{
    if (sPad1->field_59)
        VsTime--;
    if (sPad1->field_5A)
        VsTime++;
    if (VsTime < 0)
        VsTime = 0;
    if (VsTime > 4)
        VsTime = 4;
}
void sub7(void)
{
    if (sPad1->field_59)
        VsMode = 0;
    if (sPad1->field_5A)
        VsMode = 1;
}

void subMenu(void)
{
    if (sbT == -1) {
        voiceSn2i();
        return;
    }
    switch (sbT) {
    case 0:
        sub0();
        break;
    case 1:
        sub1();
        break;
    case 2:
        sub2();
        break;
    case 3:
        sub3();
        break;
    case 4:
        sub4();
        break;
    case 5:
        sub5();
        break;
    case 6:
        sub6();
        break;
    case 7:
        sub7();
        break;
    }
}

void dispBGM(MenuArchive *archive)
{
    s32 i;
    char buf[12];

    if (crN == 0) {
        jobjDispAll(archive->joints[6]);
        jobjDispAll(archive->joints[7]);
        for (i = 0; i < 10; i++) {
            HiddenParts(GetChild(archive->joints[10], 0, i, 0, -1, -1), i >= VolBgm);
        }
        jobjDispAll(archive->joints[10]);
        jobjDispAll(archive->joints[11]);
        sprintf(buf, "%02d", TestBgm);
        putAscJobj(GetChild(archive->joints[11], 0, 1, -1, -1, -1), 3, buf);
    }
}

void dispVoice(MenuArchive *archive)
{
    s32 i;
    char buf[12];

    if (crN == 1) {
        jobjDispAll(archive->joints[8]);
        jobjDispAll(archive->joints[9]);
        for (i = 0; i < 10; i++) {
            HiddenParts(GetChild(archive->joints[12], 0, i, 0, -1, -1), i >= VolVoice);
        }
        jobjDispAll(archive->joints[12]);
        jobjDispAll(archive->joints[13]);
        sprintf(buf, lit_3729, TestVoice);
        putAscJobj(GetChild(archive->joints[13], 0, 1, -1, -1, -1), 3, buf);
    }
}

void dispSe(MenuArchive *archive)
{
    s32 i;

    if (crN == 2) {
        jobjDispAll(archive->joints[19]);
        for (i = 0; i < 10; i++) {
            HiddenParts(GetChild(archive->joints[20], 0, i, 0, -1, -1), i >= VolSe);
        }
        jobjDispAll(archive->joints[20]);
    }
}

void dispRumble(MenuArchive *archive)
{
    s32 i;

    if (crN == 3) {
        for (i = 0; i < 2; i++) {
            HiddenParts(GetChild(archive->joints[14], 0, i, 0, -1, -1), i == rumble);
        }
        jobjDispAll(archive->joints[14]);
    }
}

void dispVsTime(MenuArchive *archive)
{
    s32 i;

    if (crN == 4) {
        for (i = 0; i < 5; i++) {
            HiddenParts(GetChild(archive->joints[21], 0, i, 0, -1, -1), i != VsTime);
        }
        jobjDispAll(archive->joints[21]);
    }
}
#pragma force_active on
static char *tst999 = (char *)lit_7379;
s16 menudemomsg = -1;
s16 menudemobg = -1;
s16 menudemobgm = -1;
s16 menudemovno = -1;
s16 menudemojmp = -1;
s16 menudemoselect = -1;
static s32 anmN = 1;
s32 VolSe = 8;
s32 VolBgm = 8;
s32 VolVoice = 8;
s32 rumble = 1;
s32 VsTime = 1;
s32 VsMode = 1;
static s32 TestBgm = 1;
#pragma force_active off

void dispVsMode(MenuArchive *archive)
{
    static s32 jk[1] = {-1};
    s32 i;

    if (crN != 5) {
        jk[0] = -1;
        return;
    }
    for (i = 0; i < 2; i++) {
        HiddenParts(GetChild(archive->joints[15], 0, i, 0, -1, -1), i != VsMode);
    }
    jobjDispAll(archive->joints[15]);
    if (jk[0] != VsMode) {
        SetAnimArc2(archive, 0x10, 0);
        jk[0] = VsMode;
    }
    if (jobjAnim(archive->joints[16]) != 0) {
        SetAnimArc2(archive, 0x10, 1);
    }
    for (i = 0; i < 2; i++)
        ;
    jobjDispAll(GetChild(archive->joints[16], 0, VsMode, -1, -1, -1));
}

s32 menu_sDispConfig(MenuArchive *archive, s32 flags)
{
    s32 ret = 0;
    HSD_JObj *jobj;
    HSD_JObj *j28;
    HSD_JObj *j27;
    s32 help;
    s32 end;
    s32 i;

    HSD_StartRender(0);
    HSD_SetZSortMode(1, 1);
    if (HSD_CObjSetCurrent(archive->cobj) != 0) {
        HSD_LObjSetCurrentAll(archive->lobj);
        HSD_LObjSetupInit(archive->cobj);
        GXSetCullMode(2);
        jobjDispAll(archive->joints[2]);
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(0, 0);
    if (HSD_CObjSetCurrent(archive->cobj) != 0) {
        s32 a1[7] = { 0, 1, 4, 7, 10, 13, 0 };
        s32 a2[7] = { 0, 2, 5, 8, 11, 14, 0 };
        s32 a3[7] = { 0, 3, 6, 9, 12, 15, 0 };
        HSD_LObjSetCurrentAll(archive->lobj);
        HSD_LObjSetupInit(archive->cobj);
        GXSetCullMode(0);
        menu_sDispBattleFarmTitleSubButton();
        jobj = archive->joints[0];
        if ((flags & 3) == 1)
            SetAnimArc2(archive, 0, 2);
        if (jobjAnim(jobj) != 0) {
            if ((flags & 3) == 3)
                ret = 1;
            else
                SetAnimArc2(archive, 0, 1);
        }
        jobjDispAll(jobj);
        if (sbTO != sbT) {
            if (sbTO == -1) {
                SetAnimArc2(archive, 4, 0);
                jobjAnim(archive->joints[4]);
                sbTA = 0;
                sbTO = sbT;
            }
            jobjAnim(archive->joints[4]);
            if (sbTA != 2) {
                SetAnimArc2(archive, 4, 2);
                sbTA = 2;
                jobjAnim(archive->joints[4]);
            } else {
                sbTO = sbT;
                if (sbT != -1) {
                    SetAnimArc2(archive, 4, 0);
                    sbTA = 0;
                    jobjAnim(archive->joints[4]);
                }
            }
        } else if (sbT != -1) {
            if (jobjAnim(archive->joints[4]) != 0) {
                SetAnimArc2(archive, 4, 1);
                sbTA = 1;
            }
            jobjAnim(archive->joints[4]);
        }
        if (sbTO != -1) {
            jobjCopyPos(GetChild(archive->joints[18], 0, sbTO, -1, -1, -1), archive->joints[4]);
            jobjDispAll(archive->joints[4]);
        }
        dispBGM(archive);
        dispVoice(archive);
        dispSe(archive);
        dispRumble(archive);
        dispVsTime(archive);
        dispVsMode(archive);
        j28 = archive->joints[17];
        j27 = archive->joints[5];
        jobjAnim(j28);
        end = getEndF(j28);
        if (crO != crN) {
            csA = a3[crO + 1];
            if (csAO == csA && end != 0) {
                crO = crN;
                csA = a2[crO + 1];
                csAO = -1;
            }
        } else if (csA == a1[crN + 1]) {
            if (end != 0)
                csA = a2[crN + 1];
        } else {
            csA = a2[crN + 1];
        }
        if (csAO != csA) {
            csAO = csA;
            SetAnimArc2(archive, 0x11, csA);
            jobjAnim(j28);
        }
        sMakeMatrix(j28);
        for (i = 0; i < 7; i++) {
            jobj = GetChild(j27, 0, i, -1, -1, -1);
            jobjCopyPos(GetChild(j28, 0, i, -1, -1, -1), jobj);
            if (jobjAnim1(jobj) != 0)
                sSetAnim1(archive, 5, jobj, i == csN ? 2 : 1);
            jobjDispAll(jobj);
        }
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_EndRender();
    debugmenudraw();
    return ret;
}

void menufncZoid_ConfigMenu(void)
{
    s32 ret;
    u32 cur;
    MenuArchive *arc;
    void *old;
    s32 saveCs;
    s32 saveSb;

    TeaBgm(1);
    arc = (MenuArchive *)LoadArc2("menu/config.dat");
    old = menu1p;
    menu1p = arc;
    OSReport("config    \n");
    if (arc != NULL) {
        configGet();
        configSet();
        menuSetTitleButton(11);
        csO = 1;
        csN = 6;
        csA = 0;
        csAO = 0;
        cur = 0;
        crO = -1;
        crN = -1;
        sbN = -1;
        sbT = -1;
        sbTA = -1;
        TEA_VoiceOff();
        OSReport("vsTime%d , vsMode%d \n", VsTime, VsMode);
        debug11();
        do {
            getSubMenuT();
            subMenu();
            ret = menu_sDispConfig(arc, cur);
            sPadAll();
            if (sPad1->pressed_a != 0 || sPad1->field_5E != 0) {
                sPad1->field_58 = 0;
                sPad1->field_57 = 0;
                sPad1->field_59 = 0;
                sPad1->field_5A = 0;
            }
            if (sPad1->pressed_a != 0)
                sPad1->field_5E = 0;
            if (sPad1->field_58 != 0)
                sPad1->field_57 = 0;
            if (sPad1->field_5A != 0) {
                sPad1->field_59 = 0;
                sPad1->field_58 = 0;
            }
            cur = (cur & 1) + (cur << 1);
            if (sPad1->field_5E != 0) {
                if (crN == -1) {
                    if (csN != 6)
                        SeCancel();
                    csN = 6;
                } else {
                    SeCancel();
                    crN = -1;
                    sbN = -1;
                    TeaBgm(1);
                }
                continue;
            }
            if (sPad1->pressed_a != 0 && csN == 6) {
                cur |= 1;
                SeRet();
                continue;
            }
            if (rumbleTm != 0) {
                rumbleTm--;
                sPad1->rumble = 1;
            } else {
                sPad1->rumble = 0;
            }
            if (cur & 1)
                sPad1->rumble = 0;
            if (crN == -1) {
                saveCs = csN;
                if (csN == 6 && sPad1->field_59 != 0)
                    csN = 0;
                if (csN != 6 && sPad1->field_5A != 0)
                    csN = 6;
                if (sPad1->field_57 != 0)
                    csN--;
                if (sPad1->field_58 != 0)
                    csN++;
                if (csN != saveCs) {
                    SeSelectB();
                    continue;
                }
            } else {
                if (csN != 6) {
                    saveSb = sbN;
                    if (sPad1->field_57 != 0)
                        sbN--;
                    if (sPad1->field_58 != 0)
                        sbN++;
                    if (sbN < 0)
                        sbN = menuMax[csN] - 1;
                    if (sbN >= menuMax[csN])
                        sbN = 0;
                    if (saveSb != sbN)
                        SeSelectB();
                    getSubMenuT();
                }
            }
            if (csN < 0)
                csN = 6;
            if (csN > 6)
                csN = 0;
            if (sPad1->pressed_a != 0 && crN == -1) {
                crN = csN;
                sbN = 0;
                SeRet();
                switch (csN) {
                case 0:
                case 1:
                case 2:
                    TeaBgm(-1);
                    break;
                }
            }
        } while (ret == 0);
        sPad1->rumble = 0;
        PurgeArc2(arc);
        menu1p = old;
        configPut();
    }
}
#undef lit_10459
#define lit_10459 "menu/config.dat"
#undef lit_10460
#define lit_10460 "config    \n"
#undef lit_10461
#define lit_10461 "vsTime%d , vsMode%d \n"

void detSaveNoSet(s16 no)
{
    detSaveNo = no;
}

void detCustm(s32 no)
{
    detZoidCstm = no;
    menuDetailMode = 1;
}

void detZoid(s32 no)
{
    detZoidn = no;
    menuDetailMode = 2;
}
void menu_sDispDetailSetting(f32 a, f32 b)
{
    s32 font = 2;
    s16 idx = 0;
    HSD_JObj *j1 = 0;
    HSD_JObj *j2 = 0;
    HSD_JObj *root;
    s32 partno;
    s16 i;
    s32 dead1 = 0;
    s32 dead2 = 0;
    ZoidState *zoid = (ZoidState *)((u8 *)Zoid + (detSaveNo + 6) * 0x1F8C);
    s32 calc;
    s16 zid;
    s32 dead3 = 0;
    char text[0x3C];
    GXColor white = {255, 255, 255, 255};
    GXColor black = {0, 0, 0, 0};

    if (menu_detail1p != (void *)0) {
        root = ((MenuArchive *)menu_detail1p)->joints[0];
        if (root != 0) {
            calc = zdCalcFarm((ZoidState *)((u8 *)Zoid + (detSaveNo + 6) * 0x1F8C), detSaveNo, detZoidCstm, detZoidPerson);
            zid = *(s32 *)((u8 *)Zoid + (detSaveNo + 6) * 0x1F8C + 0x20);
            idx = IveSave[detSaveNo].owned_zoids[detZoidCstm];
            SetAnimArc2((MenuArchive *)menu_detail1p, 0, 2);
            jobjAnim(((MenuArchive *)menu_detail1p)->joints[0]);
            HSD_JObjDispAll(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, -1, -1, -1, -1), 0, 7, 0);
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 0, -1, -1, -1), 4, "SETTING DETAIL");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 1, -1, -1, -1), font, "NAME");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, -1, -1, -1), font, "PARAMETER");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 3, -1, -1, -1), font, "WEAPON");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 4, -1, -1, -1), font, "OPTION");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 5, -1, -1, -1), font, "SUBPARTS");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 6, -1, -1, -1), font, "EX");
            j1 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 1, 0, -1, -1);
            j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 1, 0, 0, -1);
            sPutFontRect((char *)&Tea_zoiddat[idx], j1, j2, (GXColor){255, 255, 255, 255}, (GXColor){0, 0, 0, 0}, 0, 2);
            j1 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 1, 2, -1, -1);
            j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 1, 2, 0, -1);
            sPutFontRect((char *)&Tea_zoiddat[idx] + 0x1f, j1, j2, (GXColor){255, 255, 255, 255}, (GXColor){0, 0, 0, 0}, 0, 2);
            j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 0, -1, -1);
            j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 0, 3, -1);
            j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 1, -1, -1);
            j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 1, 3, -1);
            j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 2, -1, -1);
            j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 2, 3, -1);
            for (i = 0; i < 8; i++) {
                if (ttl_ZoidChkWepPoint(idx, i)) {
                    PutParts(-1, IveSave[detSaveNo].zoid_parts[detZoidCstm][i], GetChild(root, 0, 3, i, -1, -1));
                }
            }
            PutParts(-1, IveSave[detSaveNo].zoid_field_119E[detZoidCstm], GetChild(root, 0, 4, 0, -1, -1));
            for (i = 0; i < 3; i++) {
                PutParts(-1, IveSave[detSaveNo].zoid_field_11DA[detZoidCstm][i], GetChild(root, 0, 5, i, -1, -1));
            }
            partno = *(s16 *)((u8 *)&Tea_zoiddat[idx] + 0x8a);
            if (partno <= 0) {
                sprintf(text, "\x91\x53\x94\xad\x8e\xcb");
            } else {
                sprintf(text, (char *)&Tea_partsdat[partno]);
            }
            j1 = GetChild(root, 0, 6, 0, -1, -1);
            j2 = GetChild(root, 0, 6, 0, 0, -1);
            sPutFontRect(text, j1, j2, white, black, 0, 2);
            sprintf(text, "%4d", Tea_zoiddat[*(s32 *)((u8 *)zoid + 0x20)].field_66);
            putAscJobj(GetChild(root, 0, 2, 0, 0, -1), 4, text);
            sprintf(text, "%4d", *(s32 *)((u8 *)zoid + 0x1ddc));
            putAscJobj(GetChild(root, 0, 2, 1, 0, -1), 4, text);
            sprintf(text, "%4d", *(s32 *)((u8 *)zoid + 0x1dd8));
            putAscJobj(GetChild(root, 0, 2, 2, 0, -1), 4, text);
            sprintf(text, "%4d", *(s32 *)((u8 *)zoid + 0x1f74));
            putAscJobj(GetChild(root, 0, 2, 3, 0, -1), 4, text);
            sprintf(text, "%4d", *(s32 *)((u8 *)zoid + 0x1f70));
            putAscJobj(GetChild(root, 0, 2, 4, 0, -1), 4, text);
            sprintf(text, "%4d", *(s32 *)((u8 *)zoid + 0x1f7c));
            putAscJobj(GetChild(root, 0, 2, 5, 0, -1), 4, text);
            sprintf(text, "%4d", (s32)*(f32 *)((u8 *)zoid + 0x1dd4));
            putAscJobj(GetChild(root, 0, 2, 6, 0, -1), 4, text);
            sprintf(text, "%4d", (s32)*(f32 *)((u8 *)zoid + 0x1dd0));
            putAscJobj(GetChild(root, 0, 2, 7, 0, -1), 4, text);
            putAscJobj(GetChild(root, 0, 1, 1, -1, -1), 2, "TOTAL BP");
            sprintf(text, (char *)lit_5754, *(s32 *)((u8 *)zoid + 0x1f88));
            putAscJobj(GetChild(root, 0, 1, 1, 0, -1), 4, text);
            sprintf(text, (char *)lit_5754, *(s32 *)((u8 *)zoid + 0x1f50));
            putAscJobj(GetChild(root, 0, 2, 8, 0, -1), 4, text);
        }
    }
    (void)j1;
    (void)j1;
    a = a;
    b = b;
}
#undef lit_10511
#define lit_10511 "SETTING DETAIL"
#undef lit_10512
#define lit_10512 "NAME"
#undef lit_10513
#define lit_10513 "PARAMETER"
#undef lit_10514
#define lit_10514 "WEAPON"
#undef lit_10515
#define lit_10515 "OPTION"
#undef lit_10516
#define lit_10516 "SUBPARTS"
#undef lit_10517
#define lit_10517 "EX"
#undef lit_10518
#define lit_10518 "\x91S\x94\xad\x8e\xcb"
#undef lit_10519
#define lit_10519 "%4d"
#undef lit_10520
#define lit_10520 "TOTAL BP"

void menu_sDispDetailZoidn(f32 a, f32 b)
{
    s32 dead1 = 0;
    s32 dead2 = 0;
    s32 dead3 = 0;
    char text[0x3C];
    s32 font = 2;
    HSD_JObj *j1 = 0;
    HSD_JObj *j2 = 0;
    ZoidState *zoid = (ZoidState *)&((ZoidStateV2 *)Zoid)[detSaveNo + 6];
    s16 zoidn = detZoidn;
    ZoidDataEntry *zd = &Tea_zoiddat[zoidn];
    HSD_JObj *root;
    s32 partno;
    s32 part;
    HSD_JObj *dummy;
    s16 i;
    GXColor white = {255, 255, 255, 255};
    GXColor black = {0, 0, 0, 0};

    if (sPad1->field_47) {
        if (menu_detail1p != (void *)0) {
            root = ((MenuArchive *)menu_detail1p)->joints[0];
            if (root != 0 && detZoidn >= 0) {
                zdCalcZoid((ZoidState *)&((ZoidStateV2 *)Zoid)[detSaveNo + 6], detZoidn, -1);
                SetAnimArc2((MenuArchive *)menu_detail1p, 0, 2);
                jobjAnim(((MenuArchive *)menu_detail1p)->joints[0]);
                HSD_JObjDispAll(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, -1, -1, -1, -1), 0, 7, 0);
                putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 0, -1, -1, -1), 4, lit_10511);
                putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 1, -1, -1, -1), font, lit_10512);
                putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, -1, -1, -1), font, lit_10513);
                putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 3, -1, -1, -1), font, lit_10514);
                putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 4, -1, -1, -1), font, lit_10515);
                putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 5, -1, -1, -1), font, lit_10516);
                putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 6, -1, -1, -1), font, lit_10517);
                j1 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 1, 0, -1, -1);
                j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 1, 0, 0, -1);
                sPutFontRect((char *)&Tea_zoiddat[zoidn], j1, j2, (GXColor){255, 255, 255, 255}, (GXColor){0, 0, 0, 0}, 0, 2);
                j1 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 1, 2, -1, -1);
                j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 1, 2, 0, -1);
                sPutFontRect((char *)&Tea_zoiddat[zoidn] + 0x1f, j1, j2, (GXColor){255, 255, 255, 255}, (GXColor){0, 0, 0, 0}, 0, 2);
                dummy = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 0, -1, -1);
                dummy = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 0, 3, -1);
                dummy = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 1, -1, -1);
                dummy = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 1, 3, -1);
                dummy = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 2, -1, -1);
                dummy = GetChild(((MenuArchive *)menu_detail1p)->joints[0], 0, 2, 2, 3, -1);
                for (i = 0; i < 8; i++) {
                    if (ttl_ZoidChkWepPoint(detZoidn, i)) {
                        PutParts(-1, zd->default_parts[i], GetChild(root, 0, 3, i, -1, -1));
                    }
                }
                part = -1;
                if (zd->point_flags[8] != 0)
                    part = zd->default_parts[8];
                if (zd->point_flags[9] != 0)
                    part = zd->default_parts[9];
                if (zd->point_flags[10] != 0)
                    part = 0x115;
                if (zd->point_flags[11] != 0)
                    part = 0x18e;
                if (part == -1) {
                    (void)0;
                }
                PutParts(-1, part, GetChild(root, 0, 4, 0, -1, -1));
                for (i = 0; i < 3; i++) {
                }
                partno = *(s16 *)((u8 *)zd + 0x8a);
                if (partno <= 0) {
                    sprintf(text, lit_10518);
                } else {
                    sprintf(text, (char *)&Tea_partsdat[partno]);
                }
                j1 = GetChild(root, 0, 6, 0, -1, -1);
                j2 = GetChild(root, 0, 6, 0, 0, -1);
                sPutFontRect(text, j1, j2, white, black, 0, 2);
                sprintf(text, lit_10519, Tea_zoiddat[ZF(zoid, s32, 0x20)].field_66);
                putAscJobj(GetChild(root, 0, 2, 0, 0, -1), 4, text);
                sprintf(text, lit_10519, ZF(zoid, s32, 0x1ddc));
                putAscJobj(GetChild(root, 0, 2, 1, 0, -1), 4, text);
                sprintf(text, lit_10519, ZF(zoid, s32, 0x1dd8));
                putAscJobj(GetChild(root, 0, 2, 2, 0, -1), 4, text);
                sprintf(text, lit_10519, ZF(zoid, s32, 0x1f74));
                putAscJobj(GetChild(root, 0, 2, 3, 0, -1), 4, text);
                sprintf(text, lit_10519, ZF(zoid, s32, 0x1f70));
                putAscJobj(GetChild(root, 0, 2, 4, 0, -1), 4, text);
                sprintf(text, lit_10519, ZF(zoid, s32, 0x1f7c));
                putAscJobj(GetChild(root, 0, 2, 5, 0, -1), 4, text);
                sprintf(text, lit_10519, (s32)ZF(zoid, f32, 0x1dd4));
                putAscJobj(GetChild(root, 0, 2, 6, 0, -1), 4, text);
                sprintf(text, lit_10519, (s32)ZF(zoid, f32, 0x1dd0));
                putAscJobj(GetChild(root, 0, 2, 7, 0, -1), 4, text);
                putAscJobj(GetChild(root, 0, 1, 1, -1, -1), 2, lit_10520);
                sprintf(text, lit_5754, ZF(zoid, s32, 0x1f88));
                putAscJobj(GetChild(root, 0, 1, 1, 0, -1), 4, text);
                sprintf(text, lit_5754, ZF(zoid, s32, 0x1f50));
                putAscJobj(GetChild(root, 0, 2, 8, 0, -1), 4, text);
            }
        }
    }
    (void)j1;
    (void)j1;
    (void)j2;
    (void)j2;
    (void)j2;
    (void)j2;
    a = a;
    b = b;
}

void detWeapon(s32 no)
{
    if (no != -2)
        detPartsn = no;
    menuDetailMode = 3;
}

void menu_sDispDetailWeapon(f32 a, f32 b)
{
    s32 kind = 0;
    s32 font = 2;
    s16 value = 0;
    HSD_JObj *root;
    HSD_JObj *j1 = 0;
    HSD_JObj *j2 = 0;
    char *data;
    s32 dead1 = 0;
    s16 no = detPartsn;
    s32 dead2 = 0;
    char text[0x3C];
    GXColor white = {255, 255, 255, 255};
    GXColor black = {0, 0, 0, 0};
    char path[0x50];

    kind = 2;
    if (menu_detail1p != (void *)0) {
        root = ((MenuArchive *)menu_detail1p)->joints[kind];
        if (root != 0 && detPartsn > 0) {
            sprintf(path, "weapon/ID/wp%03d.txt", no);
            data = (char *)det_Load(path);
            SetAnimArc2((MenuArchive *)menu_detail1p, kind, 2);
            jobjAnim(((MenuArchive *)menu_detail1p)->joints[kind]);
            HSD_JObjDispAll(GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, -1, -1, -1, -1), 0, 7, 0);
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 0, -1, -1, -1), 4, "WEAPON DETAIL");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 1, -1, -1, -1), font, "NAME");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 2, -1, -1, -1), font, "PARAMETER");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 3, -1, -1, -1), font, "COMMENT");
            j1 = GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 1, 1, -1, -1);
            PutParts(0, no, j1);
            j1 = GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 1, 0, -1, -1);
            j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 1, 0, 0, -1);
            sPutFontRect((char *)&Tea_partsdat[no], j1, j2, (GXColor){255, 255, 255, 255}, (GXColor){0, 0, 0, 0}, 0, 2);
            GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 2, 0, -1, -1);
            GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 2, 0, 3, -1);
            j1 = GetChild(root, 0, 3, 0, -1, -1);
            j2 = GetChild(root, 0, 3, 0, 0, -1);
            if (*data != 0) {
                sPutFontRect(data, j1, j2, white, black, 0, 2);
            }
            value = *(s16 *)((u8 *)&Tea_partsdat[no] + 0x6C);
            (void)(*(s16 *)((u8 *)&Tea_partsdat[no] + 0x78));
            value += *(s16 *)((u8 *)&Tea_partsdat[no] + 0x78);
            value += 0x32;
            if (value >= 0) {
                sprintf(text, "%d", value);
            } else {
                sprintf(text, "-");
            }
            putAscJobjLR(GetChild(root, 0, 2, 0, 0, -1), 4, text, 1);
            value = *(s16 *)((u8 *)&Tea_partsdat[no] + 0xE6);
            if (value >= 0) {
                sprintf(text, "+%d", value);
            } else {
                sprintf(text, "%-d", value);
            }
            putAscJobjLR(GetChild(root, 0, 2, 1, 0, -1), 4, text, 1);
            value = *(s16 *)((u8 *)&Tea_partsdat[no] + 0x6E);
            if (value >= 0) {
                sprintf(text, "%d", value);
            } else {
                sprintf(text, "-");
            }
            putAscJobjLR(GetChild(root, 0, 2, 2, 0, -1), 4, text, 1);
            value = *(s16 *)((u8 *)&Tea_partsdat[no] + 0x70);
            if (*(s16 *)((u8 *)&Tea_partsdat[no] + 0xFC) > 1) {
                value = value * *(s16 *)((u8 *)&Tea_partsdat[no] + 0xFC);
            }
            if (value >= 0) {
                sprintf(text, "%d", value);
            } else {
                sprintf(text, "-");
            }
            putAscJobjLR(GetChild(root, 0, 2, 3, 0, -1), 4, text, 1);
            value = *(s16 *)((u8 *)&Tea_partsdat[no] + 0x74);
            if (value >= 0) {
                sprintf(text, "%d", value);
            } else {
                sprintf(text, "-");
            }
            putAscJobjLR(GetChild(root, 0, 2, 4, 0, -1), 4, text, 1);
            value = *(s32 *)((u8 *)&Tea_partsdat[no] + 0x58);
            sprintf(text, "%d", value);
            putAscJobjLR(GetChild(root, 0, 2, 5, 0, -1), 4, text, 1);
        }
    }
    a = a;
    b = b;
}
#undef lit_10631
#define lit_10631 "weapon/ID/wp%03d.txt"
#undef lit_10632
#define lit_10632 "WEAPON DETAIL"
#undef lit_10633
#define lit_10633 "COMMENT"
#undef lit_10634
#define lit_10634 "%d"
#undef lit_10635
#define lit_10635 "-"
#undef lit_10636
#define lit_10636 "+%d"
#undef lit_10637
#define lit_10637 "%-d"

void detOption(s32 no)
{
    if (no != -2)
        detPartsn = no;
    menuDetailMode = 4;
}

void menu_sDispDetailOption(f32 a_, f32 b_)
{
    s32 u0 = 0;
    s16 part = detPartsn;
    s32 u1 = 0;
    char text[0x3C];
    GXColor white = { 255, 255, 255, 255 };
    GXColor black = { 0, 0, 0, 0 };
    char path[100];
    s32 idx = 0;
    s32 font = 4;
    s16 v = 0;
    HSD_JObj *a = 0;
    HSD_JObj *b = 0;
    HSD_JObj *jobj;
    s8 *det;
    (void)b;
    (void)b;
    (void)jobj;

    idx = 3;
    if (menu_detail1p != NULL && (jobj = ((MenuArchive *)menu_detail1p)->joints[idx]) != 0) {
        sprintf(path, "weapon/ID/wp%03d.txt", part);
        det = det_Load(path);
        SetAnimArc2((MenuArchive *)menu_detail1p, idx, 2);
        jobjAnim(((MenuArchive *)menu_detail1p)->joints[idx]);
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, -1, -1, -1, -1), 0, 7, 0);
        putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 0, -1, -1, -1), 4, "OPTION DETAIL");
        putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 1, -1, -1, -1), 2, "NAME");
        putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 2, -1, -1, -1), 2, "PARAMETER");
        putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 3, -1, -1, -1), 2, "COMMENT");
        a = GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 1, 1, -1, -1);
        PutParts(0, part, a);
        a = GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 1, 0, -1, -1);
        b = GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 1, 0, 0, -1);
        sPutFontRect(Tea_partsdat[part].name, a, b, (GXColor){ 255, 255, 255, 255 }, (GXColor){ 0, 0, 0, 0 }, 0, 2);
        GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 2, 0, -1, -1);
        GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 2, 0, 3, -1);
        a = GetChild(jobj, 0, 3, 0, -1, -1);
        b = GetChild(jobj, 0, 3, 0, 0, -1);
        if (*det != 0)
            sPutFontRect((char *)det, a, b, white, black, 0, 2);
        v = *(s16 *)((u8 *)&Tea_partsdat[part] + 0x6c);
        font = 4;
        if (v >= 0)
            sprintf(text, "+%d", v);
        else
            sprintf(text, "%d", v);
        putAscJobjLR(GetChild(jobj, 0, 2, 0, 0, -1), font, text, 1);
        v = *(s16 *)((u8 *)&Tea_partsdat[part] + 0xe6);
        if (v >= 0)
            sprintf(text, "+%d", v);
        else
            sprintf(text, "%d", v);
        putAscJobjLR(GetChild(jobj, 0, 2, 1, 0, -1), font, text, 1);
        v = *(s16 *)((u8 *)&Tea_partsdat[part] + 0x6a);
        if (v >= 0)
            sprintf(text, "+%d", v);
        else
            sprintf(text, "%d", v);
        putAscJobjLR(GetChild(jobj, 0, 2, 2, 0, -1), font, text, 1);
        v = *(s32 *)((u8 *)&Tea_partsdat[part] + 0x58);
        if (v >= 0)
            sprintf(text, "+%d", v);
        else
            sprintf(text, "%d", v);
        putAscJobjLR(GetChild(jobj, 0, 2, 3, 0, -1), font, text, 1);
    }
    a_ = a_;
    b_ = b_;
}
#undef lit_10676
#define lit_10676 "OPTION DETAIL"

void detSubparts(s32 no)
{
    if (no != -2)
        detPartsn = no;
    menuDetailMode = 5;
}
#define PW(n, off) (*(s16 *)((u8 *)&Tea_partsdat[n] + (off)))

void menu_sDispDetailSubparts(f32 a, f32 b)
{
    s32 kind = 0;
    s32 font = 2;
    s16 value = 0;
    HSD_JObj *root;
    HSD_JObj *j1 = 0;
    HSD_JObj *j2 = 0;
    PartDataEntry *p;
    char *data;
    s32 dead1 = 0;
    s16 no = detPartsn;
    s32 dead2 = 0;
    char text[0x3C];
    GXColor white = {255, 255, 255, 255};
    GXColor black = {0, 0, 0, 0};
    char path[0x64];

    kind = 4;
    if (menu_detail1p != (void *)0) {
        root = ((MenuArchive *)menu_detail1p)->joints[kind];
        if (root != 0 && detPartsn > 0) {
            p = &Tea_partsdat[no];
            sprintf(path, lit_10631, no);
            data = (char *)det_Load(path);
            SetAnimArc2((MenuArchive *)menu_detail1p, kind, 2);
            jobjAnim(((MenuArchive *)menu_detail1p)->joints[kind]);
            jobjDispAll(GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, -1, -1, -1, -1));
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 0, -1, -1, -1), 4, "SUBPARTS DETAIL");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 1, -1, -1, -1), font, "NAME");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 2, -1, -1, -1), font, "PARAMETER");
            putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 3, -1, -1, -1), font, lit_10633);
            j1 = GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 1, 1, -1, -1);
            PutParts(0, no, j1);
            j1 = GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 1, 0, -1, -1);
            j2 = GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 1, 0, 0, -1);
            sPutFontRect((char *)&Tea_partsdat[no], j1, j2, (GXColor){255, 255, 255, 255}, (GXColor){0, 0, 0, 0}, 0, 2);
            GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 2, 0, -1, -1);
            GetChild(((MenuArchive *)menu_detail1p)->joints[kind], 0, 2, 0, 3, -1);
            j1 = GetChild(root, 0, 3, 0, -1, -1);
            j2 = GetChild(root, 0, 3, 0, 0, -1);
            if (*data != 0) {
                sPutFontRect(data, j1, j2, white, black, 0, 2);
            }
            value = PW(no, 0x6C);
            if (value >= 0) {
                sprintf(text, lit_10636, value);
            } else {
                sprintf(text, lit_10634, value);
            }
            putAscJobjLR(GetChild(root, 0, 2, 0, 0, -1), 4, text, 1);
            value = PW(no, 0xE6);
            if (value >= 0) {
                sprintf(text, lit_10636, value);
            } else {
                sprintf(text, lit_10634, value);
            }
            putAscJobjLR(GetChild(root, 0, 2, 1, 0, -1), 4, text, 1);
            value = PW(no, 0x6A);
            if (value >= 0) {
                sprintf(text, lit_10636, value);
            } else {
                sprintf(text, lit_10634, value);
            }
            putAscJobjLR(GetChild(root, 0, 2, 2, 0, -1), 4, text, 1);
            value = PW(no, 0x6C);
            if (value >= 0) {
                sprintf(text, lit_10636, value);
            } else {
                sprintf(text, lit_10634, value);
            }
            putAscJobjLR(GetChild(root, 0, 2, 3, 0, -1), 4, text, 1);
            value = *(s16 *)((u8 *)p + 0xEA);
            if (value >= 0) {
                sprintf(text, lit_10636, value);
            } else {
                sprintf(text, lit_10634, value);
            }
            putAscJobjLR(GetChild(root, 0, 2, 4, 0, -1), 4, text, 1);
            value = *(s16 *)((u8 *)p + 0xE8);
            if (value >= 0) {
                sprintf(text, lit_10636, value);
            } else {
                sprintf(text, lit_10634, value);
            }
            putAscJobjLR(GetChild(root, 0, 2, 5, 0, -1), 4, text, 1);
            value = *(s16 *)((u8 *)p + 0x72);
            if (value >= 0) {
                sprintf(text, lit_10636, value);
            } else {
                sprintf(text, lit_10634, value);
            }
            putAscJobjLR(GetChild(root, 0, 2, 6, 0, -1), 4, text, 1);
            value = *(s32 *)((u8 *)p + 0x58);
            if (value < 0) {
                value = 0;
            }
            sprintf(text, lit_10636, value);
            putAscJobjLR(GetChild(root, 0, 2, 7, 0, -1), 4, text, 1);
        }
    }
    (void)j2;
    (void)j2;
    (void)font;
    (void)font;
    a = a;
    b = b;
}
#undef lit_10728
#define lit_10728 "SUBPARTS DETAIL"

void detPerson(s32 no)
{
    detPersonn = no;
    menuDetailMode = 6;
}

void menu_sDispDetailPerson(f32 a_, f32 b_)
{
    s32 u0 = 0;
    s16 person = detPersonn;
    s32 u1 = 0;
    char text[0x3C];
    GXColor white = { 255, 255, 255, 255 };
    GXColor black = { 0, 0, 0, 0 };
    char path[0x100];
    s32 idx = 0;
    s32 font = 2;
    s16 v = 0;
    HSD_JObj *a = 0;
    HSD_JObj *b = 0;
    HSD_JObj *jobj;
    s8 *det;
    (void)b;
    (void)b;
    (void)jobj;

    idx = 6;
    if (menu_detail1p != NULL && (jobj = ((MenuArchive *)menu_detail1p)->joints[idx]) != 0) {
        sprintf(path, "chara/man%03d.txt", person);
        det = det_Load(path);
        SetAnimArc2((MenuArchive *)menu_detail1p, idx, 2);
        jobjAnim(((MenuArchive *)menu_detail1p)->joints[idx]);
        HSD_JObjDispAll(GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, -1, -1, -1, -1), 0, 7, 0);
        putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 0, -1, -1, -1), 4, "PILOT DATA");
        putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 1, -1, -1, -1), font, "NAME");
        putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 2, -1, -1, -1), font, "PARAMETER");
        putAscJobj(GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 3, -1, -1, -1), font, "COMMENT");
        a = GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 1, 0, -1, -1);
        b = GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 1, 0, 0, -1);
        sPutFontRect(Tea_persondat[person].name, a, b, (GXColor){ 255, 255, 255, 255 }, (GXColor){ 0, 0, 0, 0 }, 0, 2);
        GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 2, 0, -1, -1);
        GetChild(((MenuArchive *)menu_detail1p)->joints[idx], 0, 2, 0, 3, -1);
        a = GetChild(jobj, 0, 3, 0, -1, -1);
        b = GetChild(jobj, 0, 3, 0, 0, -1);
        if (*det != 0)
            sPutFontRect((char *)det, a, b, white, black, 0, 2);
        v = *(s16 *)((u8 *)&Tea_persondat[person] + 0x22);
        font = 4;
        if (v >= 0)
            sprintf(text, "+%d", v);
        else
            sprintf(text, "%d", v);
        putAscJobjLR(GetChild(jobj, 0, 2, 1, 0, -1), font, text, 1);
        v = *(s16 *)((u8 *)&Tea_persondat[person] + 0x20);
        if (v >= 0)
            sprintf(text, "+%d", v);
        else
            sprintf(text, "%d", v);
        putAscJobjLR(GetChild(jobj, 0, 2, 2, 0, -1), font, text, 1);
        v = *(s16 *)((u8 *)&Tea_persondat[person] + 0x24);
        if (v >= 0)
            sprintf(text, "+%d", v);
        else
            sprintf(text, "%d", v);
        putAscJobjLR(GetChild(jobj, 0, 2, 0, 0, -1), font, text, 1);
    }
    a_ = a_;
    b_ = b_;
}
#undef lit_10762
#define lit_10762 "chara/man%03d.txt"
#undef lit_10763
#define lit_10763 "PILOT DATA"

void menu_sDispDetailAll(void)
{
    f32 f31 = 0;
    f32 f30 = 0;

    if (sPad1->field_47 == 0) {
        if (sPad2->field_47 == 0) {
            menu_sDispAllHavePartsList();
        }
    }
    if (dispDetailFlag == 0) {
        if (flag_AllHavePartsList != flag_AllHavePartsListold) {
            SeWinon();
            flag_AllHavePartsListold = flag_AllHavePartsList;
        }
    } else {
        flag_AllHavePartsList = 0;
    }
    if (sPad1->field_5B != 0 || sPad1->field_57 != 0 || sPad1->field_58 != 0 || sPad1->field_59 != 0 || sPad1->field_5A != 0 || sPad1->pressed_a != 0 || sPad1->field_5E != 0 || sPad1->field_5F != 0 || sPad1->field_60 != 0 || sPad1->field_61 != 0 || sPad1->field_63 != 0) {
        if (flag_AllHavePartsList != 0) {
            SeWinon();
        }
        flag_AllHavePartsList = 0;
    }
    if (sPad1->field_47 == 0 && sPad2->field_47 == 0) {
        if (dispDetailFlag == 1 && menuDetailMode != 0) {
            SeOn(0x1d);
        }
        dispDetailFlag = 0;
    } else {
        if (menuDetailMode == 0) {
            return;
        }
        if (menudispyesnoflag != 0) {
            return;
        }
        if (dispDetailFlag == 0) {
            SeOn(0x1d);
        }
        dispDetailFlag = 1;
        if (menu_detail1p == NULL) {
            return;
        }
        if (menuDetailMode != 0) {
            flag_AllHavePartsList = 0;
        }
        if (menuDetailMode == 1) {
            menu_sDispDetailSetting(f31, f30);
        }
        if (menuDetailMode == 2) {
            menu_sDispDetailZoidn(f31, f30);
        }
        if (menuDetailMode == 3) {
            menu_sDispDetailWeapon(f31, f30);
        }
        if (menuDetailMode == 4) {
            menu_sDispDetailOption(f31, f30);
        }
        if (menuDetailMode == 5) {
            menu_sDispDetailSubparts(f31, f30);
        }
        if (menuDetailMode == 6) {
            menu_sDispDetailPerson(f31, f30);
        }
    }
}
#pragma force_active on
static s32 detSaveNo;
static s32 detZoidn;
void *menu_detail1p;
void *menu_back1p;
void *menu1p;
MenuArchive *menu_yesnop;
static HSD_LObj *ttlLight;
u8 menuZoidBattleModeSingleTeam;
s16 menuDetailMode;
void *menu_saveloadp;
void *menu_saveloadbgp;
s32 menu_saveloadiconp;
s16 menusaveloadslot;
s32 menusaveloadDataSelect;
s32 menudispyesnoflag;
s32 flag_VsFarmEntryStageBattleExit;
s16 flag_AllHavePartsList;
s16 flag_AllHavePartsListold;
s32 flag_saveok;
s16 menumode;
s32 menuanmend;
s8 menufarmmode;
s32 menuretno;
s8 vsman;
f32 menuzoidry;
s32 menu_StageUpDownFlag;
s32 menuzoidequippartslist;
s8 menuzoidhavepartslist;
s8 menumodeShop;
s16 menuwepdispnum;
s16 menusaveloadmode;
static f32 lookCamY;
static void *flPicture;
static s32 flPictureT;
s16 menudispshophavezoidlist;
static void *yokoSeTBL;
s16 menudemop;
s16 menudemoC;
s16 menudemokeywait;
s16 menudemocur;
s32 menudemocharain;
s32 menudemoend;
s32 MenuDemoSkipFlag;
s16 ttl_vs_modeLR;
void *menu1p_map;
static s32 stage00;
static s32 vsMapMax;
s32 rumbleTm;
static s32 VolSe3;
static s32 VolBgm3;
static s32 VolVoice3;
static s32 rumble93;
static s32 VsTime3;
static s32 VsMode3;
static s32 TestVoice;
static s32 sbN;
static s32 sbT;
static s32 sbTO;
static s32 sbTA;
static s32 crN;
static s32 crO;
static s32 csO;
static s32 csN;
static s32 csA;
static s32 csAO;
#pragma force_active off
}
