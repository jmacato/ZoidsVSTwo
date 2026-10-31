extern "C" {
#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"
#define ZoidState ZoidStateV3
#define ZoidStateSizeCheck ZoidStateSizeCheckV3
#define FootJoint FootJointV3
#define FootJointSizeCheck FootJointSizeCheckV3
#define Zoid ZoidV3
#define CoObj CoObjV3
#define CoObjSizeCheck CoObjSizeCheckV3
#include "zoid3.h"
#undef ZoidState
#undef ZoidStateSizeCheck
#undef FootJoint
#undef FootJointSizeCheck
#undef Zoid
#undef CoObj
#undef CoObjSizeCheck
#define NULL 0
#define JOBJ_MTX_INDEP_SRT 0x02000000
#define JOBJ_USER_DEF_MTX 0x800000
#define JOBJ_MTX_DIRTY 0x40

typedef struct ZoidState {
    u8 pad_0000[0x20];
    s32 type;
    s32 person;
    u8 pad_0028[0x18];
    HSD_JObj *joint;
    u8 pad_0044[0x108];
    s32 health;
    s32 max_health;
    s16 option_type;
    u8 option_use_state;
    u8 option_use_previous;
    s16 option_energy_max;
    s16 option_energy;
    u8 pad_015C[0x10];
    CoDamage damages[6];
    u8 pad_01B4[0x344];
    HSD_JObj *head_joint;
    u8 pad_04FC[0x98];
    HSD_JObj *neck_joint;
    u8 pad_0598[0x98];
    HSD_JObj *bust_joint;
    u8 pad_0634[0x98];
    HSD_JObj *hip_joint;
    u8 pad_06D0[0xBC4];
    s32 super_attack_enabled;
    s32 super_attack_cooldown;
    u8 pad_129C[0x54];
    s32 field_12F0;
    u8 pad_12F4[0x8];
    s32 weapon_mount;
    s32 weapon_mount_count;
    s32 selected_weapon_mount;
    s32 field_1374;
    WeaponMount weapon_mounts[20];
    u8 pad_176C[0xF0];
    WeaponSlot weapon_slots[30];
    HSD_JObj *camera_joint;
    u8 pad_19C8[0x68];
    HSD_JObj *buckle_joint;
    u8 pad_1A34[0x134];
    f32 position[3];
    u8 pad_1B74[0x4C];
    f32 heading;
    f32 high_angle;
    u8 pad_1BC8[0x64];
    u8 us_pad_1C2C[0xC];
    f32 knockback[3];
    u8 pad_1C38[0x78];
    s32 requested_action;
    u8 pad_1CB4[0x90];
    u16 ally_smoke_timer;
    u16 enemy_smoke_timer;
    s32 field_1E50;
    u8 pad_1D4C[0x4];
    s32 health_bar_timer;
    u8 pad_1D54[0x10];
    s32 death_started;
    u8 pad_1D68[0x10];
    s32 rumble_timer;
    u8 index;
    u8 slot;
    u8 target_pending;
    u8 target_slot;
    u8 pad_1D80[0x1A];
    u16 ground_polygon;
    u8 pad_1D9C[0xC];
    s32 option_active;
    s32 stel_state;
    u8 pad_1DB0[0x20];
    f32 field_1EF8;
    f32 search_range;
    u8 pad_1DD8[0x8];
    f32 distances[16];
    u8 pad_1E20[0x10];
    u8 attack_in_range[16];
    u8 pad_1E40[0x14C];
} ZoidState;

typedef char ZoidStateSizeCheck[sizeof(ZoidState) == 0x1F98 ? 1 : -1];

typedef struct ZoidName2 {
    u8 bytes[0x7FC];
} ZoidName2;

typedef struct CameraState2 CameraState2;

typedef struct WeaponCamera {
    s32 mode;
    s32 half;
    u8 reserved_08[0x580];
    s32 target_marker_count;
} WeaponCamera;

typedef struct PartEntry3 {
    u8 reserved_00[0x33];
    char english_name[0xC7];
    s16 icon;
    u8 reserved_FC[0x124 - 0xFC];
} PartEntry3;

extern PartEntry3 Tea_partsdat[];
extern s32 sCamNN;
extern s32 sCamNS;
extern s32 vsyncN;
extern void HSD_JObjDisp(HSD_JObj *joint, s32 a, s32 b, s32 c);
extern s32 sWeaponGetNumber(ZoidState *zoid, s32 selected);
extern s32 sWeaponGetMountBullet(ZoidState *zoid, s32 index);
extern void putAscJobjLR_sp(HSD_JObj *joint, s32 a, char *text, s32 b, f32 c);

extern ZoidState Zoid[16];
extern ZoidName2 Tea_zoiddat[];
extern ModelArchive *dsp1p;
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *joint);
extern void __assert(const char *file, s32 line, const char *expr);
extern HSD_JObj *GetChild(HSD_JObj *joint, s32 first, s32 second, s32 third, s32 fourth, s32 fifth);
extern s32 sCameraPt2d(CameraState2 *camera, Vec3 *position, s32 *screen_point);
extern void jobjDisp(HSD_JObj *joint);
extern void jobjCopyPos(HSD_JObj *source, HSD_JObj *target);
extern void jobjDispAll(HSD_JObj *joint);
extern void sMakeMatrix(HSD_JObj *joint);
extern void putAscJobj(HSD_JObj *joint, s32 font, char *text);
extern s32 sprintf(char *buffer, const char *format, ...);
extern ModelArchive *dspDemoPress;
extern ModelArchive *dspFilter;
extern ModelArchive *dspPos00;
extern s32 dspPartnerMsg;
extern s32 dspPartnerMsgM;
extern s32 dspPartnerMsgTm;
extern s32 dspPartnerMsgV;
extern s32 gameEndF;
extern s32 gameTime;
extern s32 jobjAnim(HSD_JObj *joint);
extern s32 onTime;
extern void PartnerFaceV(s32 message, s32 duration, s32 mode, s32 value);
extern void sDispOrderAnim(void *arg);
extern void sDispPartnerAnim(void *arg);
extern void sDispPurgePartsBP(void);
extern void sDispPurgePartsContinue(void);
extern void sDispPurgePartsGame(void);
extern void sDispPurgePartsMessage(void);
extern void sDispPurgePartsPause(void);

typedef f32 Mtx[3][4];

typedef struct GameModeS {
    s32 reserved_00;
    s32 game_type;
    s32 reserved_08;
    s32 win;
    u8 reserved_10[0x38];
} GameModeS;

extern GameModeS GameMode;
extern ModelArchive *dspRader;
extern s32 nowStage;
extern s32 radarLen[];
extern void jobjCopyPosS(HSD_JObj *source, HSD_JObj *target);
extern struct HSD_TObj *searchTObjN(HSD_JObj *joint, s32 *index);
extern void texScrollMtx(struct HSD_TObj *tobj, Mtx matrix);
extern void PSMTXTrans(Mtx matrix, f32 x, f32 y, f32 z);
extern void s1pRaderMapZoid(ZoidState *zoid, s32 kind, HSD_JObj *joint);

typedef struct RaderCamera {
    u8 pad_000[0x1C4];
    ZoidState *zoid;
    u8 pad_1C8[0x260 - 0x1C8];
    f32 yaw;
} RaderCamera;

extern ModelArchive *dspPoint;
extern void sCameraRela(RaderCamera *camera, Vec3 *point, Vec3 *out);
extern f64 __frsqrte(f64);

#include "gx_tev.h"

typedef struct PadState PadState;

typedef struct PauseCamera {
    u8 pad_000[0x1E8];
    PadState *pad;
} PauseCamera;

typedef struct MapInfoS {
    char farm[0x20];
    u8 reserved_0020[8];
    char title[0x64];
    char victory_text[0x50];
} MapInfoS;

extern ModelArchive *dspPause;
extern ModelArchive *dspBP;
extern ModelArchive *dspMessage;
extern ModelArchive *dspOrder;
extern ModelArchive *dspWarn;
extern ModelArchive *dspContinue;
extern s32 pause;
extern s32 menuRet;
extern PadState *sPad1;
extern PadState *sPad2;
extern MapInfoS mgI;
extern void sDispGameFilter(s32 index, s32 mode);
extern void HiddenParts(HSD_JObj *joint, s32 hidden);
extern void sSetAnim1(ModelArchive *archive, s32 model, HSD_JObj *joint, s32 animation);
extern void sPutFontRect(char *text, HSD_JObj *top_left, HSD_JObj *bottom_right, GXColor color, GXColor shadow, s32 alpha, s32 flags);

typedef struct ParabolaWeapon {
    s32 active;
    u8 pad_04[8];
    s32 field_0C;
    ModelArchive *archive;
    s32 field_14;
    u8 pad_18[0x20];
    ZoidState *owner;
} ParabolaWeapon;

extern s32 sWeaponGetBarrelParabola(ParabolaWeapon *weapon, Vec3 *position, f32 distance);

typedef struct ScopeCamera {
    u8 pad_000[0x1C4];
    ZoidState *zoid;
    u8 pad_1C8[0x2FA - 0x1C8];
    s8 scope_on;
    u8 pad_2FB;
    f32 fov;
} ScopeCamera;

extern ModelArchive *dspScope;
extern ModelArchive *dspPartner;
extern s32 dspPartnerMsgA;
extern s32 dspPartnerMsgB;
extern s32 dspPartnerMsgF;
extern void zdVoiceScG(ZoidState *zoid, s32 voice);
extern Mtx dspMtx;
extern s32 anmFilter[3];
extern HSD_JObj *jobjFilter[3];
extern ModelArchive *dspSearch;
extern HSD_JObj *dspJobjEx3[2];
extern s32 dspAnmEx3[2];
extern s32 dspAnmEx3n[2];
extern s32 dspPartnerMsgTm2;
extern s32 dspOrderAI;

typedef struct DispCamera {
    s32 mode;
    s32 half;
    struct HSD_LObj *lights;
    u8 pad_00C[0x1B8];
    ZoidState *zoid;
    u8 pad_1C8[0x20];
    PadState *pad;
    u8 pad_1EC[0x58];
    struct HSD_CObj *cobj;
} DispCamera;
extern s32 pBak;
void sDispPause(PauseCamera *camera);
#pragma force_active on
static Mtx dspMtx;
static s32 anmFilter[3];
static u32 str_1478[64];
HSD_JObj *jobjFilter[3];
u64 dspFileA[8];
u64 dspFileGrid[8];
#pragma force_active off
void sDispLoadPartsOrder(void)
{
    dspOrderAI = 0;
    if (dspOrder == 0)
        dspOrder = LoadArc2("disp/order.dat");
}
#undef lit_374
#define lit_374 "disp/order.dat"

void sDispPurgePartsOrder(void)
{
    if (dspOrder != 0) {
        PurgeArc2(dspOrder);
        dspOrder = 0;
    }
}
void sDispLoadPartsMessage(void)
{
    if (dspMessage == 0)
        dspMessage = LoadArc2("disp/div/message.dat");
}
#undef lit_381
#define lit_381 "disp/div/message.dat"
extern void SetAnimArc2(ModelArchive *archive, s32 model, s32 selected);

void sDispLoadPartsContinue(void)
{
    if (dspContinue == 0) {
        dspContinue = LoadArc2("disp/div/continue.dat");
        if (dspContinue != 0)
            SetAnimArc2(dspContinue, 0, 0);
    }
}
#undef lit_387
#define lit_387 "disp/div/continue.dat"
void sDispLoadPartsWarn(void)
{
    if (dspWarn == 0) {
        dspWarn = LoadArc2("disp/div/warning.dat");
        if (dspWarn != 0)
            SetAnimArc2(dspWarn, 6, 1);
    }
}
#undef lit_393
#define lit_393 "disp/div/warning.dat"
void sDispLoadPartsBP(void)
{
    if (dspBP == 0) {
        dspBP = LoadArc2("disp/div/bp.dat");
    }
}
#undef lit_397
#define lit_397 "disp/div/bp.dat"
void sDispLoadPartsPause(void)
{
    if (dspPause == 0) {
        dspPause = LoadArc2("disp/div/pause.dat");
    }
}
#undef lit_401
#define lit_401 "disp/div/pause.dat"

void sDispPurgePartsMessage(void)
{
    if (dspMessage != 0) {
        PurgeArc2(dspMessage);
        dspMessage = 0;
    }
}
void sDispPurgePartsContinue(void)
{
    if (dspContinue != 0) {
        PurgeArc2(dspContinue);
        dspContinue = 0;
    }
}

void sDispPurgePartsPause(void)
{
    if (dspPause != 0) {
        PurgeArc2(dspPause);
        dspPause = 0;
    }
}

void sDispPurgePartsWarn(void)
{
    if (dspWarn != 0) {
        PurgeArc2(dspWarn);
        dspWarn = 0;
    }
}

void sDispPurgePartsBP(void)
{
    if (dspBP != 0) {
        PurgeArc2(dspBP);
        dspBP = 0;
    }
}
extern s32 usbForce;
extern ModelArchive *LoadArc2_(char *name, s32 flag);
extern HSD_JObj *CopyJoint2(ModelArchive *archive, s32 model);
extern void sDispLoadPartsOrder(void);
extern void sDispLoadPartsWarn(void);
extern void sDispLoadPartsPause(void);
extern void SetAnimArc2(ModelArchive *archive, s32 model, s32 selected);

void sDispLoadParts(s32 mode, s32 flag)
{
    char radar[104];
    char partner[256];

    dspRader = 0;
    dsp1p = 0;
    onTime = 0;
    dspPos00 = 0;
    dspJobjEx3[0] = 0;
    dspJobjEx3[1] = 0;
    anmFilter[0] = -1;
    anmFilter[1] = -1;
    anmFilter[2] = -1;
    dspPartnerMsg = -1;
    dspPartnerMsgV = -1;
    dspPartnerMsgM = -1;
    dspPartnerMsgB = -1;
    dspPartnerMsgA = 0;
    dspPartnerMsgF = 0;
    dspPartnerMsgTm2 = 0;
    if (usbForce != 0)
        flag = 1;
    if (mode == 1 && Zoid[2].joint != 0 && Zoid[0].slot == Zoid[2].slot) {
        sprintf(partner, "chara/bp/bp%02d.dat", Zoid[2].person);
        dspPartner = LoadArc2_(partner, flag);
        SetAnimArc2(dspPartner, 0, 0);
        sDispLoadPartsOrder();
    }
    dspPos00 = LoadArc2_("disp/position00.dat", flag);
    if (sCamNN == 1)
        dspScope = LoadArc2_("disp/scope.dat", flag);
    if (sCamNN == 2) {
        if (sCamNS == 0)
            dspScope = LoadArc2_("disp/scope_01.dat", flag);
        else
            dspScope = LoadArc2_("disp/scope_02.dat", flag);
    }
    dspPoint = LoadArc2_("disp/point.dat", flag);
    if (dspPos00 != 0) {
        dsp1p = LoadArc2_("disp/interface.dat", flag);
        onTime = 0;
    }
    sprintf(radar, "disp/radar%02d.dat", nowStage);
    dspRader = LoadArc2_(radar, flag);
    dspSearch = LoadArc2_("disp/search.dat", flag);
    dspJobjEx3[0] = CopyJoint2(dsp1p, 3);
    dspAnmEx3[0] = 3;
    dspAnmEx3n[0] = -1;
    dspAnmEx3n[1] = -1;
    if (sCamNN == 2 && sCamNS == 1)
        dspAnmEx3[1] = 4;
    else
        dspAnmEx3[1] = 3;
    dspJobjEx3[1] = CopyJoint2(dsp1p, dspAnmEx3[1]);
    menuRet = 0;
    PSMTXTrans(dspMtx, 0.0f, 0.0f, 0.0f);
    sDispLoadPartsWarn();
    sDispLoadPartsPause();
    if (GameMode.game_type == 9 && dspDemoPress == 0)
        dspDemoPress = LoadArc2("disp/demo_press.dat");
}
#undef lit_442
#define lit_442 "chara/bp/bp%02d.dat"
#undef lit_443
#define lit_443 "disp/position00.dat"
#undef lit_444
#define lit_444 "disp/scope.dat"
#undef lit_445
#define lit_445 "disp/scope_01.dat"
#undef lit_446
#define lit_446 "disp/scope_02.dat"
#undef lit_447
#define lit_447 "disp/point.dat"
#undef lit_448
#define lit_448 "disp/interface.dat"
#undef lit_449
#define lit_449 "disp/radar%02d.dat"
#undef lit_450
#define lit_450 "disp/search.dat"
#undef lit_452
#define lit_452 "disp/demo_press.dat"
extern void jobjUnrefAll(HSD_JObj *joint);

void sDispPurgePartsGame(void)
{
    if (dspJobjEx3[0] != 0)
        jobjUnrefAll(dspJobjEx3[0]);
    if (dspJobjEx3[1] != 0)
        jobjUnrefAll(dspJobjEx3[1]);
    dspJobjEx3[0] = 0;
    dspJobjEx3[1] = 0;
    if (dspSearch != 0)
        PurgeArc2(dspSearch);
    dspSearch = 0;
    if (dspRader != 0)
        PurgeArc2(dspRader);
    dspRader = 0;
    if (dsp1p != 0)
        PurgeArc2(dsp1p);
    dsp1p = 0;
    if (dspPartner != 0)
        PurgeArc2(dspPartner);
    dspPartner = 0;
    if (dspPoint != 0)
        PurgeArc2(dspPoint);
    dspPoint = 0;
    if (dspScope != 0)
        PurgeArc2(dspScope);
    dspScope = 0;
    sDispPurgePartsWarn();
    sDispPurgePartsPause();
    sDispPurgePartsOrder();
}

void sDispPurgeParts(void)
{
    sDispPurgePartsGame();
    if (dspPos00 != 0)
        PurgeArc2(dspPos00);
    dspPos00 = 0;
    if (dspDemoPress != 0)
        PurgeArc2(dspDemoPress);
    dspDemoPress = 0;
    sDispPurgePartsBP();
    sDispPurgePartsContinue();
    sDispPurgePartsPause();
    sDispPurgePartsMessage();
    sDispPurgePartsPause();
}
extern HSD_JObj *GetChild(HSD_JObj *joint, s32 first, s32 second, s32 third, s32 fourth, s32 fifth);
extern void HSD_JObjDisp(HSD_JObj *joint, s32 a, s32 b, s32 c);

void d1ZoidDmg0(HSD_JObj *joint, s32 index, CoDamage *damage)
{
    f32 ratio;
    s32 level;
    HSD_JObj *child;
    s32 health;

    if (joint != (void *)0 && damage != (void *)0) {
        health = damage->health;
        if (damage->maximum_health != 0) {
            ratio = 100.0 * health / damage->maximum_health;
        } else {
            return;
        }
        level = 0;
        if (ratio < 50.0f) {
            level = 1;
        }
        if (ratio < 1.0f) {
            level = 2;
        }
        if (level == 0) {
            child = GetChild(joint, 0, index, -1, -1, -1);
        }
        if (level == 1) {
            child = GetChild(joint, 0, index, 0, -1, -1);
        }
        if (level == 2) {
            child = GetChild(joint, 0, index, 0, 0, -1);
        }
        if (child != (void *)0) {
            HSD_JObjDisp(child, 0, 7, 0);
        }
    }
}
void s1pZdDmg1(ZoidState *zoid, ModelArchive *archive, HSD_JObj *joint)
{
    HSD_JObj *child;

    if (archive->models[0] != (void *)0 && joint != (void *)0) {
        child = GetChild(joint, 0, 0, -1, -1, -1);
        if (child != (void *)0) {
            jobjCopyPos(child, archive->models[0]);
            HSD_JObjDisp(archive->models[0], 0, 7, 0);
            d1ZoidDmg0(archive->models[0], 0, &zoid->damages[0]);
            d1ZoidDmg0(archive->models[0], 1, &zoid->damages[1]);
            d1ZoidDmg0(archive->models[0], 2, &zoid->damages[3]);
            d1ZoidDmg0(archive->models[0], 3, &zoid->damages[2]);
            d1ZoidDmg0(archive->models[0], 4, &zoid->damages[5]);
            d1ZoidDmg0(archive->models[0], 5, &zoid->damages[4]);
        }
    }
}
#pragma dont_inline off
#pragma auto_inline off
#define JOBJ_USE_QUATERNION 0x20000
static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 605, "jobj");
    return !(jobj->flags & JOBJ_USER_DEF_MTX) && (jobj->flags & JOBJ_MTX_DIRTY);
}

static inline void HSD_JObjSetMtxDirty(HSD_JObj *jobj)
{
    if (jobj != NULL && !HSD_JObjMtxIsDirty(jobj))
        HSD_JObjSetMtxDirtySub(jobj);
}

static inline void HSD_JObjSetScaleX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 805, "jobj");
    jobj->scale.x = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

void s1pZdHP(DispCamera *camera, ZoidState *zoid, ModelArchive *display, HSD_JObj *parent)
{
    HSD_JObj *child;
    HSD_JObj *bar;
    HSD_JObj *target;
    f32 ratio;
    s32 danger;

    if (parent != 0) {
        child = GetChild(parent, 0, 1, -1, -1, -1);
        if (child != 0) {
            target = display->models[1];
            if (sCamNN == 2 && sCamNS == 1 && camera->half == 1) {
                target = display->models[2];
            }
            if (target != 0) {
                jobjCopyPosS(child, target);
                if (zoid->max_health <= 0) {
                    zoid->max_health = 100;
                }
                ratio = (f32)zoid->health / (f32)zoid->max_health;
                if (ratio < 0.3) {
                    danger = 1;
                } else {
                    danger = 0;
                }
                HSD_JObjDisp(GetChild(target, 0, danger, -1, -1, -1), 0, 7, 0);
                bar = GetChild(target, 0, danger, 0, -1, -1);
                HSD_JObjSetScaleX(bar, ratio);
                HSD_JObjDisp(bar, 0, 7, 0);
                HSD_JObjDisp(GetChild(target, 0, danger, 1, -1, -1), 0, 7, 0);
            }
        }
    }
}
#pragma dont_inline on
#undef lit_544
#define lit_544 "jobj.h"
#undef lit_545
#define lit_545 "jobj"
#pragma dont_inline off
extern void LoadArcAnimModel2(ModelArchive *archive, HSD_JObj *joint, s32 animation, s32 state);

void s1pEX(DispCamera *camera, ZoidState *zoid, HSD_JObj *joint)
{
    HSD_JObj *child;
    HSD_JObj *gauge;
    HSD_JObj *bar;
    s32 state;
    f32 ratio;

    if (joint != 0 && zoid != (void *)0 && zoid->joint != (void *)0 && zoid->super_attack_enabled != 0) {
        child = GetChild(joint, 0, 2, -1, -1, -1);
        if (child != 0) {
            gauge = dspJobjEx3[camera->half];
            if (gauge != (void *)0) {
                jobjCopyPos(child, gauge);
                ratio = 0.5f;
                state = 0;
                ratio = (600.0f - (f32)zoid->super_attack_cooldown) / 600.0f;
                bar = GetChild(gauge, 0, 1, -1, -1, -1);
                HSD_JObjSetScaleX(bar, ratio);
                if (zoid->super_attack_cooldown != 0)
                    state = 1;
                if (state != dspAnmEx3n[camera->half]) {
                    LoadArcAnimModel2(dsp1p, gauge, dspAnmEx3[camera->half], state);
                    dspAnmEx3n[camera->half] = state;
                }
                if (pause == 0)
                    jobjAnim(gauge);
                jobjDispAll(gauge);
            }
        }
    }
}
#pragma dont_inline on
#pragma dont_inline off
void s1pOption(WeaponCamera *camera, ZoidState *zoid, HSD_JObj *parent)
{
    HSD_JObj *gauge;
    HSD_JObj *child;
    HSD_JObj *icon;
    HSD_JObj *bar;
    HSD_JObj *fill;
    s32 ready;
    f32 ratio;

    if (parent != 0 && zoid != (void *)0 && zoid->joint != (void *)0 && zoid->option_type != 0 && zoid->option_energy_max != 0) {
        child = GetChild(parent, 0, 4, -1, -1, -1);
        if (child != 0) {
            gauge = dsp1p->models[5];
            icon = dsp1p->models[7];
            if (sCamNN == 2 && sCamNS == 1 && camera->half == 0) {
                gauge = dsp1p->models[6];
            }
            if (gauge != (void *)0) {
                jobjCopyPos(child, gauge);
                jobjCopyPos(child, icon);
                (void)child;
                ready = 0;
                if (zoid->option_energy_max == zoid->option_energy || (s8)zoid->option_use_state != 0)
                    ready = 1;
                bar = GetChild(gauge, 0, ready ? 0 : 1, -1, -1, -1);
                ratio = (f32)zoid->option_energy / (f32)zoid->option_energy_max;
                fill = GetChild(bar, 0, 0, -1, -1, -1);
                HSD_JObjSetScaleX(fill, ratio);
                jobjDispAll(bar);
                bar = GetChild(icon, 0, zoid->option_type - 1, -1, -1, -1);
                HiddenParts(bar, !ready);
                HiddenParts(GetChild(bar, 0, 0, -1, -1, -1), ready);
                jobjDispAll(bar);
            }
        }
    }
}
#pragma dont_inline on
#pragma dont_inline off
static inline f32 HSD_JObjGetScaleX(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_544, 899, lit_545);
    return jobj->scale.x;
}

static inline void HSD_JObjAddTranslationX(HSD_JObj *jobj, f32 x)
{
    jobj ? (void)0 : __assert(lit_544, 1109, lit_545);
    jobj->translation.x += x;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjAddTranslationY(HSD_JObj *jobj, f32 y)
{
    jobj ? (void)0 : __assert(lit_544, 1120, lit_545);
    jobj->translation.y += y;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjSetRotationZ(HSD_JObj *jobj, f32 z)
{
    jobj ? (void)0 : __assert(lit_544, 716, lit_545);
    !(jobj->flags & JOBJ_USE_QUATERNION) ? (void)0 : __assert(lit_544, 717, "!(jobj->flags & JOBJ_USE_QUATERNION)");
    jobj->rotation[2] = z;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

void s1pRaderMapZoid(ZoidState *zoid, s32 kind, HSD_JObj *joint)
{
    HSD_JObj *point;
    f32 scale;
    f32 x;
    f32 z;
    HSD_JObj *child;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    if (zoid->death_started != 0)
        return;
    if (zoid->option_active == 2)
        return;
    if (dspRader == NULL)
        return;
    child = GetChild(dspRader->models[0], 0, 1, -1, -1, -1);
    scale = HSD_JObjGetScaleX(joint);
    point = GetChild(dspPoint->models[0], 0, kind, -1, -1, -1);
    jobjCopyPos(child, point);
    x = zoid->position[0];
    z = zoid->position[2];
    if (point == NULL)
        return;
    HSD_JObjAddTranslationX(point, scale * (12.8 * (x / radarLen[nowStage])));
    HSD_JObjAddTranslationY(point, scale * (12.8 * (-z / radarLen[nowStage])));
    HSD_JObjSetRotationZ(point, 3.1415926 + zoid->heading);
    HSD_JObjDisp(point, 0, 7, 0);
}
#pragma dont_inline on
#undef lit_668
#define lit_668 "!(jobj->flags & JOBJ_USE_QUATERNION)"
#pragma dont_inline off
static inline void raderAddTranslationX(HSD_JObj *jobj, f32 scale, f32 v, f32 range)
{
    jobj ? (void)0 : __assert(lit_544, 1109, lit_545);
    jobj->translation.x += (f32)(scale * (12.8 * (v / range / 2.0f)));
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void raderAddTranslationY(HSD_JObj *jobj, f32 scale, f32 v, f32 range)
{
    jobj ? (void)0 : __assert(lit_544, 1120, lit_545);
    jobj->translation.y += (f32)(scale * (12.8 * (v / range / 2.0f)));
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline f32 sqrtf(f32 x, volatile f32 *y)
{
    if (x > 0.0f) {
        f64 guess = __frsqrte((f64)x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        *y = (f32)(x * guess);
        return *y;
    }
    return x;
}

void s1pRaderBattleZoid(RaderCamera *camera, ZoidState *zoid, s32 kind, HSD_JObj *base)
{
    ZoidState *target;
    HSD_JObj *child;
    HSD_JObj *point;
    HSD_JObj *joint;
    Vec3 rel;
    Vec3 pos;
    f32 scale;
    f32 len;
    f32 range;
    f32 x;
    volatile f32 root;
    f32 z;
    f32 angle;
    s32 i;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    if (zoid->death_started != 0)
        return;
    if (kind != 0 && zoid->option_active == 2)
        return;
    target = camera->zoid;
    if (target == (void *)0)
        return;
    if (target->joint == (void *)0)
        return;
    if (dspPoint == 0)
        return;
    if (dspRader == 0)
        return;
    child = GetChild(dspRader->models[0], 0, 1, -1, -1, -1);
    point = GetChild(dspPoint->models[0], 0, kind, -1, -1, -1);
    scale = HSD_JObjGetScaleX(base);
    jobjCopyPos(child, point);
    sMakeMatrix(point);
    joint = zoid->head_joint;
    if (joint == NULL)
        joint = zoid->bust_joint;
    if (joint == NULL)
        joint = zoid->neck_joint;
    if (joint == NULL)
        joint = zoid->hip_joint;
    if (joint == NULL)
        joint = zoid->buckle_joint;
    if (joint == NULL)
        joint = zoid->joint;
    for (i = 0; i < 3; i++) {
        (&pos.x)[i] = joint->matrix[i][3];
    }
    sCameraRela(camera, &pos, &rel);
    z = rel.z;
    x = rel.x;
    len = sqrtf(x * x + z * z, &root);
    range = target->search_range;
    if (range < len)
        return;
    if (point == NULL)
        return;
    raderAddTranslationX(point, scale, x, range);
    raderAddTranslationY(point, scale, z, range);
    angle = zoid->heading - camera->yaw;
    HSD_JObjSetRotationZ(point, angle);
    HSD_JObjDisp(point, 0, 7, 0);
}
#pragma dont_inline on
#pragma dont_inline off
static inline void HSD_JObjSetScaleY(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_544, 819, lit_545);
    jobj->scale.y = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjSetTranslateX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_544, 952, lit_545);
    jobj->translation.x = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjSetTranslateY(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_544, 966, lit_545);
    jobj->translation.y = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

void s1pRaderMap(ZoidState *zoid, HSD_JObj *joint, HSD_JObj *base)
{
    s32 kind;
    s32 i;
    ZoidState *other;
    HSD_JObj *child;
    struct HSD_TObj *tobj;
    f32 range;
    f32 stg;
    f32 unit;
    f32 factor;
    f32 base_scale;
    f32 x;
    f32 y;
    s32 index;
    Mtx matrix;

    if (dspRader == 0)
        return;
    range = zoid->field_1EF8;
    jobjCopyPosS(joint, dspRader->models[0]);
    child = GetChild(dspRader->models[0], 0, 1, -1, -1, -1);
    index = 0;
    tobj = searchTObjN(child, &index);
    PSMTXTrans(matrix, 0.0f, 0.0f, 0.0f);
    texScrollMtx(tobj, matrix);
    jobjDisp(child);
    child = GetChild(dspRader->models[0], 0, 0, 1, 0, 0);
    stg = radarLen[nowStage];
    if (child != 0 && stg) {
        unit = 2.0 * (range / stg);
        base_scale = HSD_JObjGetScaleX(base);
        factor = 12.8 * base_scale / stg;
        if (GameMode.game_type == 2)
            factor = 2.0 * (12.8 * base_scale / stg);
        HSD_JObjSetScaleX(child, 1.0f);
        HSD_JObjSetScaleY(child, 1.0f);
        x = zoid->position[0] * factor;
        HSD_JObjSetTranslateX(child, x);
        y = -zoid->position[2] * factor;
        HSD_JObjSetTranslateY(child, y);
        HSD_JObjSetScaleX(child, unit);
        HSD_JObjSetScaleY(child, unit);
        sMakeMatrix(child);
        jobjDisp(child);
    }
    s1pRaderMapZoid(zoid, 0, base);
    for (i = 0; i < 16; i++) {
        other = &Zoid[i];
        if (zoid == other)
            continue;
        if (other->joint == (void *)0)
            continue;
        kind = zoid->slot == other->slot ? 3 : 1;
        if (zoid->distances[i] > range)
            continue;
        s1pRaderMapZoid(other, kind, base);
    }
}
#pragma dont_inline on
#pragma dont_inline off
void s1pRaderBattle(RaderCamera *camera, ZoidState *zoid, HSD_JObj *joint, HSD_JObj *base)
{
    HSD_JObj *jobjCopyPosS_target;

    ZoidState *other;
    HSD_JObj *child;
    f32 value;
    s32 i;
    s32 kind;

    if (dspRader == 0)
        return;
    {
        jobjCopyPosS_target = dspRader->models[0];
        jobjCopyPosS(joint, jobjCopyPosS_target);
    }
    sMakeMatrix(dspRader->models[0]);
    child = GetChild(dspRader->models[0], 0, 0, -1, -1, -1);
    HSD_JObjDisp(child, 0, 7, 0);
    child = GetChild(dspRader->models[0], 0, 0, 0, -1, -1);
    value = 3.1415926 + -camera->yaw;
    HSD_JObjSetRotationZ(child, value);
    HSD_JObjDisp(child, 0, 7, 0);
    s1pRaderBattleZoid(camera, zoid, 0, base);
    for (i = 0; i < 16; i++) {
        other = &Zoid[i];
        if (zoid != other && other->joint != (void *)0) {
            if (zoid->slot == other->slot)
                kind = 3;
            else
                kind = 1;
            s1pRaderBattleZoid(camera, other, kind, base);
        }
    }
}
#pragma dont_inline on
#include "ground_collision.h"

typedef struct SearchCamera {
    u8 pad_000[0x214];
    Vec3 eye;
    u8 pad_220[0x40];
    f32 yaw;
} SearchCamera;

typedef struct MgState2 {
    u8 pad_0000[0x2AA4];
    s32 kind;
} MgState2;

extern MgState2 *mgP;
extern f32 calcAng(Vec3 *from, Vec3 *to);

#pragma dont_inline off
static inline void HSD_JObjSetRotationY(HSD_JObj *jobj, f32 y)
{
    jobj ? (void)0 : __assert(lit_544, 696, lit_545);
    !(jobj->flags & JOBJ_USE_QUATERNION) ? (void)0 : __assert(lit_544, 697, "!(jobj->flags & JOBJ_USE_QUATERNION)");
    jobj->rotation[1] = y;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

void s1pSearch(SearchCamera *camera, ZoidState *zoid, HSD_JObj *joint)
{
    u16 material;
    s32 kind;
    HSD_JObj *search;
    s32 mode;
    HSD_JObj *child;
    f32 angle;
    s32 state;
    u16 polygon;
    Vec3 pos;
    GroundMask *mask;

    state = 0;
    if (dspSearch == 0)
        return;
    search = dspSearch->models[0];
    if (search == 0)
        return;
    if (zoid == 0)
        return;
    if (zoid->joint == 0)
        return;
    mode = GameMode.game_type;
    if (mode == 9)
        return;
    if (mode == 1 || mode == 4) {
        kind = mgP->kind;
        if (kind == 3 || kind == 7) {
            pos.x = (f32) * (s16 *)((u8 *)mgP + 0x3082);
            pos.z = (f32) * (s16 *)((u8 *)mgP + 0x3084);
            state = 1;
            if (*(s8 *)((u8 *)mgP + 0x321A) != 0)
                state = 0;
        }
    }
    polygon = zoid->ground_polygon;
    if (polygon != GROUND_POLYGON_NONE) {
        material = grdcPolygon[polygon].material;
        mask = &grdcMat[material];
        if ((mask->bits[0] & 6) != 0) {
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
            state = 2;
        }
    }
    if (state == 0)
        return;
    {
        HSD_JObj *GetChild_joint = joint;
        child = GetChild(GetChild_joint, 0, state == 1 ? 9 : 13, -1, -1, -1);
    }
    if (child == 0)
        return;
    jobjCopyPosS(child, search);
    sMakeMatrix(search);
    angle = calcAng(&pos, &camera->eye);
    if (100.0f == angle)
        return;
    angle -= camera->yaw;
    child = GetChild(search, 0, 0, -1, -1, -1);
    if (child != 0)
        HSD_JObjSetRotationY(child, angle);
    jobjDispAll(search);
}
#pragma dont_inline on
void s1pRader(DispCamera *camera, ZoidState *zoid, HSD_JObj *joint)
{
    ZoidState *other;
    s32 found;
    s32 i;
    HSD_JObj *base;

    found = 0;
    base = GetChild(joint, 0, 5, -1, -1, -1);
    s1pSearch((SearchCamera *)camera, zoid, joint);
    if (dspRader == (void *)0)
        return;
    for (i = 0; i < 16; i++) {
        other = &Zoid[i];
        if (other == zoid)
            continue;
        if (other->joint == (void *)0)
            continue;
        if (other->death_started != 0)
            continue;
        if (zoid->slot == other->slot)
            continue;
        if (zoid->distances[i] <= zoid->search_range) {
            found = 1;
            break;
        }
    }
    if (camera->half + 1 == pause && camera->pad != 0 && ((s8 *)camera->pad)[78] == 0)
        found = 0;
    if (found != 0)
        s1pRaderBattle((RaderCamera *)camera, zoid, base, base);
    else
        s1pRaderMap(zoid, base, base);
}
void s1pWeapon(WeaponCamera *camera, ZoidState *zoid, HSD_JObj *joint)
{
    s32 idx;
    s32 num;
    s32 num2;
    s32 sel;
    HSD_JObj *frame;
    char *name;
    HSD_JObj *dst;
    s32 i;
    s32 digits[3];
    s16 icon;
    HSD_JObj *head;
    HSD_JObj *pos;
    s32 count;
    s32 k;
    s32 bullet;
    s32 icon2;

    if (dsp1p == 0)
        return;
    if (zoid->weapon_mount_count <= 0)
        return;
    sel = zoid->selected_weapon_mount;
    if (sel != -1 && zoid->weapon_mounts[sel].active == 0)
        return;
    if (sel == 19)
        sel = -1;
    if (sel == -1) {
        for (i = 0, sel = 0; i < 10; i++, sel++) {
            if (zoid->weapon_mounts[sel].active != 0)
                break;
        }
        if (i == 10)
            return;
    }
    if (joint == 0)
        return;
    head = GetChild(joint, 0, 3, -1, -1, -1);
    if (head == 0)
        return;
    frame = dsp1p->models[9];
    if (sCamNN == 2 && sCamNS == 1 && camera->half == 0)
        frame = dsp1p->models[10];
    if (frame == 0)
        return;
    jobjCopyPos(head, frame);
    sMakeMatrix(frame);
    HSD_JObjDisp(GetChild(frame, 0, -1, -1, -1, -1), 0, 7, 0);
    if (sel != -1) {
        num = sWeaponGetNumber(zoid, sel);
        count = camera->target_marker_count;
        pos = GetChild(frame, 0, 0, -1, -1, -1);
        HSD_JObjDisp(pos, 0, 7, 0);
        jobjDispAll(pos);
        pos = GetChild(frame, 0, 0, 0, -1, -1);
        dst = GetChild(dsp1p->models[12], 0, 0, count / 10 % 10, -1, -1);
        jobjCopyPos(pos, dst);
        HSD_JObjDisp(dst, 0, 7, 0);
        pos = GetChild(frame, 0, 0, 1, -1, -1);
        dst = GetChild(dsp1p->models[12], 0, 0, count % 10, -1, -1);
        jobjCopyPos(pos, dst);
        HSD_JObjDisp(dst, 0, 7, 0);
        pos = GetChild(frame, 0, 2, -1, -1, -1);
        icon = -1;
        if (num != -1)
            icon = Tea_partsdat[num].icon;
        if (icon >= 15 || icon < 0)
            icon = 0;
        if (icon >= 0) {
            dst = GetChild(dsp1p->models[11], 0, icon, -1, -1, -1);
            jobjCopyPos(pos, dst);
            if (zoid->weapon_mount != -1 || (vsyncN & 8))
                HSD_JObjDisp(dst, 0, 7, 0);
        }
        bullet = sWeaponGetMountBullet(zoid, sel);
        if (bullet >= 999)
            bullet = 999;
        digits[0] = bullet / 100 % 10;
        digits[1] = bullet / 10 % 10;
        digits[2] = bullet % 10;
        for (i = 0; i < 3; i++) {
            pos = GetChild(frame, 0, 4, i, -1, -1);
            dst = GetChild(dsp1p->models[12], 0, 1, digits[i], -1, -1);
            jobjCopyPos(pos, dst);
            HSD_JObjDisp(dst, 0, 7, 0);
        }
        pos = GetChild(frame, 0, 3, -1, -1, -1);
        if (pos != 0) {
            name = 0;
            if (num != -1)
                name = Tea_partsdat[num].english_name;
            if (name != 0) {
                if (sCamNN == 2 && sCamNS == 1 && camera->half == 0)
                    putAscJobjLR_sp(pos, 2, name, 0, 0.0f);
                else
                    putAscJobjLR_sp(pos, 2, name, 1, 0.0f);
            }
        }
    }
    if (zoid->weapon_mount_count > 1) {
        idx = sel + 1;
        k = 0;
        for (i = 0; i < 10; i++, idx++) {
            if (idx >= 10)
                idx -= 10;
            if (idx == sel)
                return;
            if (zoid->weapon_mounts[idx].active == 0)
                continue;
            num2 = sWeaponGetNumber(zoid, idx);
            if (num2 == -1)
                continue;
            icon2 = Tea_partsdat[num2].icon;
            if (icon2 >= 15 || icon2 < 0)
                icon2 = 0;
            pos = GetChild(frame, 0, 2, k, -1, -1);
            dst = GetChild(dsp1p->models[11], 0, icon2, 0, -1, -1);
            jobjCopyPos(pos, dst);
            HSD_JObjDisp(dst, 0, 7, 0);
            k++;
        }
    }
}
#pragma dont_inline off
void s1pTarget(CameraState2 *camera, ZoidState *zoid)
{
    s32 j;
    HSD_JObj *digit;
    HSD_JObj *bar;
    HSD_JObj *lock0;
    HSD_JObj *lock1;
    HSD_JObj *lock2;
    s32 flags;
    ZoidState *other;
    HSD_JObj *joint;
    s32 i;
    s32 health;
    s32 visible;
    s32 slot;
    s32 pt[2];
    f32 pos[3];
    char buf[12];
    f32 x;
    f32 y;
    f32 ratio;

    if (dsp1p == NULL)
        return;
    slot = zoid->slot;
    lock0 = GetChild(dsp1p->models[14], 0, 0, -1, -1, -1);
    lock1 = GetChild(dsp1p->models[14], 0, 1, -1, -1, -1);
    lock2 = GetChild(dsp1p->models[14], 0, 2, -1, -1, -1);
    bar = dsp1p->models[16];
    for (i = 0; i < 16; i++) {
        other = &Zoid[i];
        if (other->joint == (void *)0)
            continue;
        if (other == zoid)
            continue;
        if (slot == other->slot)
            continue;
        joint = other->camera_joint;
        flags = 0;
        for (j = 0; j < 3; j++)
            pos[j] = joint->matrix[j][3];
        visible = sCameraPt2d(camera, (Vec3 *)pos, pt);
        if (visible == 0)
            continue;
        if (zoid->attack_in_range[i] == 1)
            flags |= 4;
        if (other->field_1E50 != 0)
            flags = 0;
        x = (pt[0] - 320) / 10.0;
        y = (240 - pt[1]) / 10.0;
        if (other->stel_state == 2)
            flags = 0;
        if (flags != 0) {
            HSD_JObjSetTranslateX(lock0, x);
            HSD_JObjSetTranslateY(lock0, y);
            HSD_JObjSetTranslateX(lock1, x);
            HSD_JObjSetTranslateY(lock1, y);
            HSD_JObjSetTranslateX(lock2, x);
            HSD_JObjSetTranslateY(lock2, y);
            if (flags & 1)
                jobjDisp(lock0);
            if (flags & 2)
                jobjDisp(lock1);
            if (flags & 4)
                jobjDisp(lock2);
        }
        health = other->health;
        if (other->health_bar_timer != 0) {
            HSD_JObj *child;
            HSD_JObjSetTranslateX(bar, 2.0f + x);
            HSD_JObjSetTranslateY(bar, y - 2.0f);
            ratio = (f32)health / (f32)other->max_health;
            child = GetChild(bar, 0, 1, -1, -1, -1);
            HSD_JObjSetScaleX(child, ratio);
            sMakeMatrix(bar);
            jobjDispAll(bar);
            if (health > 9999)
                health = 9999;
            sprintf(buf, "%04d", health);
            for (j = 0; j < 4; j++) {
                digit = GetChild(dsp1p->models[12], 0, 0, buf[j] - '0', -1, -1);
                jobjCopyPos(GetChild(bar, 0, 0, j, -1, -1), digit);
                jobjDispAll(digit);
            }
            putAscJobj(GetChild(bar, 0, 0, 4, -1, -1), 0, (digit = bar, digit = dsp1p->models[12], (char *)&Tea_zoiddat[other->type]));
        }
    }
}
#pragma dont_inline on
#undef lit_1054
#define lit_1054 "%04d"
typedef struct StraightWeapon {
    s32 active;
    u8 pad_04[8];
    s32 field_0C;
    ModelArchive *archive;
    s32 field_14;
    u8 pad_18[0x20];
    ZoidState *owner;
    u8 pad_3C[0x3B0];
    f32 range;
} StraightWeapon;

extern s32 sWeaponGetBarrel(StraightWeapon *weapon, Vec3 *position, f32 distance);

#pragma dont_inline off
static inline f32 fabsf(f32 *value)
{
    return (*(u32 *)value &= 0x7FFFFFFF, *value);
}

void s1pWp1TgtStraight(WeaponCamera *camera, StraightWeapon *weapon, f32 distance)
{
    s32 i;
    HSD_JObj *jobj;
    HSD_JObj *target_joint;
    HSD_JObj *normal;
    HSD_JObj *lock;
    ZoidState *target;
    ZoidState *owner;
    f32 x;
    f32 y;
    s32 b;
    s32 a;
    Vec3 position;
    s32 pt[2];
    s32 slot;
    f32 diff;

    if (dsp1p == 0)
        return;
    if (weapon == (void *)0)
        return;
    if (weapon->active == 0)
        return;
    owner = weapon->owner;
    target_joint = 0;
    target = 0;
    b = weapon->field_14;
    a = weapon->field_0C;
    if (owner->target_pending != 0)
        target = &Zoid[owner->target_slot];
    if (target != 0)
        target_joint = target->camera_joint;
    slot = *(s32 *)((u8 *)weapon->archive + (weapon->field_14 << 2) + 0x3c);
    if (sWeaponGetBarrel(weapon, &position, distance) == 0)
        return;
    if (sCameraPt2d((CameraState2 *)camera, &position, pt) == 0)
        return;
    normal = GetChild(dsp1p->models[14], 0, 3, -1, -1, -1);
    lock = GetChild(dsp1p->models[14], 0, 1, -1, -1, -1);
    x = (pt[0] - 320) / 10.0;
    y = (240 - pt[1]) / 10.0;
    jobj = normal;
    if (target_joint != 0) {
        for (i = 0; i < 3; i++) {
            diff = target_joint->matrix[i][3] - (&position.x)[i];
            if (fabsf(&diff) > 5.0f)
                break;
        }
        if (i == 3)
            jobj = lock;
    }
    if (weapon->range < distance)
        jobj = normal;
    if (jobj == lock)
        camera->target_marker_count++;
    HSD_JObjSetTranslateX(jobj, x);
    HSD_JObjSetTranslateY(jobj, y);
    jobjDisp(jobj);
}
#pragma dont_inline on
#pragma dont_inline off
void s1pWp1TgtParabola(WeaponCamera *camera, ParabolaWeapon *weapon, f32 distance)
{
    register s32 i;
    HSD_JObj *jobj;
    HSD_JObj *target_joint;
    ZoidState *owner;
    HSD_JObj *normal;
    HSD_JObj *lock;
    ZoidState *target;
    f32 x;
    f32 y;
    s32 b;
    s32 a;
    Vec3 position;
    f32 tpos[3];
    s32 pt[2];
    s32 slot;
    volatile f32 root;
    f32 diff;

    if (dsp1p == 0)
        return;
    if (weapon == (void *)0)
        return;
    if (weapon->active == 0)
        return;
    owner = weapon->owner;
    target_joint = 0;
    target = 0;
    b = weapon->field_14;
    a = weapon->field_0C;
    if (owner != 0) {
        if (owner->target_pending != 0)
            target = &Zoid[owner->target_slot];
    }
    if (target != 0)
        target_joint = target->camera_joint;
    if (target_joint != 0) {
        for (i = 0; i < 3; i++)
            tpos[i] = target_joint->matrix[i][3];
        x = tpos[0] - weapon->archive->models[0]->matrix[0][3];
        y = tpos[2] - weapon->archive->models[0]->matrix[2][3];
        distance = sqrtf(x * x + y * y, &root);
    }
    slot = *(s32 *)((u8 *)weapon->archive + (weapon->field_14 << 2) + 0x3c);
    if (sWeaponGetBarrelParabola(weapon, &position, distance) == 0)
        return;
    if (sCameraPt2d((CameraState2 *)camera, &position, pt) == 0)
        return;
    normal = GetChild(dsp1p->models[14], 0, 3, -1, -1, -1);
    lock = GetChild(dsp1p->models[14], 0, 1, -1, -1, -1);
    x = (pt[0] - 320) / 10.0;
    y = (240 - pt[1]) / 10.0;
    jobj = normal;
    if (target_joint != 0) {
        for (i = 0; i < 3; i++) {
            diff = target_joint->matrix[i][3] - (&position.x)[i];
            if (fabsf(&diff) > 5.0f)
                break;
        }
        if (i == 3)
            jobj = lock;
    }
    if (jobj == lock)
        camera->target_marker_count++;
    HSD_JObjSetTranslateX(jobj, x);
    HSD_JObjSetTranslateY(jobj, y);
    jobjDisp(jobj);
}
#pragma dont_inline on
typedef struct HomeMarker {
    f32 position[3];
    s32 locked;
    u8 pad_10[4];
    ZoidState *target;
    void *source;
    HSD_JObj *joint;
    s32 field_20;
} HomeMarker;

typedef struct HomeWeapon {
    s32 active;
    u8 pad_04[8];
    s32 field_0C;
    u8 pad_10[4];
    s32 field_14;
    u8 pad_18[0x20];
    ZoidState *owner;
    u8 pad_3C[0x3B0];
    f32 range;
    u8 pad_3F0[0xC];
    s32 marker_count;
    u8 pad_400[8];
    HomeMarker markers[1];
} HomeWeapon;

#pragma dont_inline off
void s1pWp1TgtHome(WeaponCamera *camera, HomeWeapon *weapon)
{
    HSD_JObj *normal;
    HSD_JObj *lock;
    HSD_JObj *jobj;
    HomeMarker *marker;
    ZoidState *target;
    void *source;
    s32 i;
    s32 b;
    s32 a;
    s32 pt[2];
    f32 x;
    f32 y;

    if (dsp1p == 0)
        return;
    if (weapon == (void *)0)
        return;
    if (weapon->active == 0)
        return;
    b = weapon->field_14;
    a = weapon->field_0C;
    normal = GetChild(dsp1p->models[14], 0, 0, -1, -1, -1);
    lock = GetChild(dsp1p->models[14], 0, 1, -1, -1, -1);
    for (i = 0; i < weapon->marker_count; i++) {
        marker = &weapon->markers[i];
        if (marker->joint == (void *)0)
            continue;
        target = marker->target;
        source = marker->source;
        if (target == 0 && source == 0 && marker->field_20 == -1)
            continue;
        if (target != 0 && target->joint != 0 && target->stel_state == 2) {
            marker->joint = 0;
            continue;
        }
        if (target != 0 && target->joint != 0 && weapon->owner != 0 && weapon->owner->distances[target->index] > weapon->range) {
            marker->joint = 0;
            continue;
        }
        if (sCameraPt2d((CameraState2 *)camera, (Vec3 *)marker, pt) == 0) {
            if (marker->locked != 0)
                marker->joint = 0;
            continue;
        }
        jobj = normal;
        if (marker->locked != 0) {
            camera->target_marker_count++;
            jobj = lock;
        }
        x = (pt[0] - 320) / 10.0;
        y = (240 - pt[1]) / 10.0;
        HSD_JObjSetTranslateX(jobj, x);
        HSD_JObjSetTranslateY(jobj, y);
        jobjDisp(jobj);
    }
}
#pragma dont_inline on
typedef struct TgtWeapon {
    u8 pad_000[0x3F8];
    s32 kind;
    u8 pad_3FC[0x6D8 - 0x3FC];
} TgtWeapon;

extern TgtWeapon sWeapon[];

void s1pWpTgt_(WeaponCamera *disp, ZoidState *zoid, s32 mount)
{
    u8 target_slot;
    s32 i;
    TgtWeapon *weapon;
    f32 distance;
    s32 slot;
    s32 index;

    WeaponMount *wm = &zoid->weapon_mounts[mount];
    if (wm->active == 0)
        return;
    distance = 0.0f;
    target_slot = zoid->target_slot;
    target_slot = target_slot;
    if (zoid->target_slot == -1)
        zoid->target_pending = 0;
    if (zoid->target_pending != 0)
        distance = zoid->distances[zoid->target_slot];
    for (i = 0; i < 4; i++) {
        if (wm->entries[i].active == 0)
            continue;
        slot = wm->entries[i].slot;
        if (slot == -1)
            continue;
        index = zoid->weapon_slots[slot].weapon;
        if (index == -1)
            continue;
        weapon = &sWeapon[index];
        if (weapon->kind == 0)
            s1pWp1TgtStraight(disp, (StraightWeapon *)weapon, distance);
        if (weapon->kind == 1)
            s1pWp1TgtHome(disp, (HomeWeapon *)weapon);
        if (weapon->kind == 2)
            s1pWp1TgtParabola(disp, (ParabolaWeapon *)weapon, distance);
    }
}
void s1pWpTgt(DispCamera *camera, ZoidState *zoid)
{
    s32 i;

    ((WeaponCamera *)camera)->target_marker_count = 0;
    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    if (zoid->death_started != 0)
        return;
    if (zoid->weapon_mount == -1)
        return;
    if (zoid->selected_weapon_mount == -1)
        return;
    if (zoid->selected_weapon_mount != zoid->weapon_mount)
        return;
    if (zoid->selected_weapon_mount == WEAPON_MOUNT_SUPER_ATTACK && zoid->field_12F0 == 0) {
        for (i = 0; i < 10; i++)
            s1pWpTgt_((WeaponCamera *)camera, zoid, i);
    } else {
        s1pWpTgt_((WeaponCamera *)camera, zoid, zoid->selected_weapon_mount);
    }
}

void s1pTimer(HSD_JObj *joint)
{
    HSD_JObj *child;
    s32 time;
    HSD_JObj *from;
    HSD_JObj *timer;
    HSD_JObj *to;
    s32 i;
    s32 digit[4];

    if (gameTime <= 0)
        return;
    if (dsp1p == 0)
        return;
    if (joint == 0)
        return;
    child = GetChild(joint, 0, 8, -1, -1, -1);
    if (child == 0)
        return;
    timer = dsp1p->models[13];
    if (timer == 0)
        return;
    jobjCopyPos(child, timer);
    time = gameTime / 60;
    digit[3] = time % 10;
    time = time / 10;
    digit[2] = time % 6;
    time = time / 6;
    digit[1] = time % 10;
    time = time / 10;
    digit[0] = time % 6;
    time = time / 6;
    if (gameTime == 1800) {
        onTime = 1;
        if (gameTime != 0)
            SetAnimArc2(dsp1p, 13, 1);
    }
    if (gameTime <= 1800 && onTime == 0) {
        onTime = 1;
        if (gameTime != 0)
            SetAnimArc2(dsp1p, 13, 1);
    }
    jobjAnim(timer);
    HSD_JObjDisp(GetChild(timer, 0, -1, -1, -1, -1), 0, 7, 0);
    for (i = 0; i < 4; i++) {
        from = GetChild(timer, 0, 13 - i, -1, -1, -1);
        to = GetChild(timer, 0, digit[i], -1, -1, -1);
        jobjCopyPos(from, to);
        HSD_JObjDisp(to, 0, 7, 0);
    }
}
#pragma dont_inline off
extern void HiddenPartsAll(HSD_JObj *joint, s32 hidden);
extern void SetAnimArc2(ModelArchive *archive, s32 model, s32 selected);

static inline f32 HSD_JObjGetScaleY(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_544, 911, lit_545);
    return jobj->scale.y;
}

void sDispArea(DispCamera *camera, HSD_JObj *joint)
{
    register s32 GetChild_first;

    GroundMask *mask;

    u16 polygon;
    s32 material;
    register HSD_JObj *area;
    HSD_JObj *GetChild_joint;
    ZoidState *zoid;
    HSD_JObj *source;

    if (camera == (void *)0)
        return;
    zoid = camera->zoid;
    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    {
        GetChild_joint = joint;
        {
            GetChild_first = 0;
            source = GetChild(GetChild_joint, GetChild_first, 11, -1, -1, -1);
        }
    }
    if (dspWarn == (void *)0)
        return;
    area = dspWarn->models[6];
    jobjCopyPos(source, area);
    sMakeMatrix(area);
    HiddenPartsAll(GetChild(area, 0, 1, -1, -1, -1), 1);
    HiddenPartsAll(GetChild(area, 0, 2, -1, -1, -1), 1);
    HiddenPartsAll(GetChild(area, 0, 0, -1, -1, -1), 0);
    polygon = zoid->ground_polygon;
    if (polygon == GROUND_POLYGON_NONE)
        return;
    material = grdcPolygon[polygon].material;
    mask = &grdcMat[material];
    if ((mask->bits[0] & 6) == 0)
        return;
    if (pause == 0 && jobjAnim(area) != 0)
        SetAnimArc2(dspWarn, 6, 1);
    sMakeMatrix(area);
    if (0.0f != HSD_JObjGetScaleY(area))
        jobjDispAll(area);
}
#pragma dont_inline on
#pragma dont_inline off
void sDispScope(ScopeCamera *camera, HSD_JObj *joint)
{
    HSD_JObj *GetChild_joint;
    HSD_JObj *root;
    HSD_JObj *child;
    ZoidState *zoid;
    f32 ang;

    if (dspScope == 0)
        return;
    if (gameEndF > 3)
        return;
    if (camera == (void *)0)
        return;
    if (camera->scope_on == 0)
        return;
    zoid = camera->zoid;
    root = dspScope->models[0];
    if (sCamNN == 2) {
        {
            GetChild_joint = joint;
            child = GetChild(GetChild_joint, 0, -1, -1, -1, -1);
        }
        jobjCopyPos(child, root);
        sMakeMatrix(root);
    }
    if (root == 0)
        return;
    child = GetChild(root, 0, 1, -1, -1, -1);
    HSD_JObjSetRotationZ(child, camera->fov / 6.0);
    sMakeMatrix(child);
    jobjDisp(child);
    if (zoid != 0) {
        ang = 2.0 * (32.0f * zoid->heading / 3.1415926);
        if (ang > 32.0)
            ang -= 32.0;
        if (ang < -32.0)
            ang += 32.0;
        if (ang > 32.0)
            ang -= 32.0;
        if (ang < -32.0)
            ang += 32.0;
        child = GetChild(root, 0, 3, -1, -1, -1);
        HSD_JObjSetTranslateX(child, ang);
        sMakeMatrix(child);
        jobjDisp(child);
        ang = 2.0 * (32.0f * zoid->high_angle / 3.1415926);
        if (ang > 32.0)
            ang -= 32.0;
        if (ang < -32.0)
            ang += 32.0;
        if (ang > 32.0)
            ang -= 32.0;
        if (ang < -32.0)
            ang += 32.0;
        child = GetChild(root, 0, 2, -1, -1, -1);
        HSD_JObjSetTranslateY(child, ang);
        sMakeMatrix(child);
        jobjDisp(child);
    }
    child = GetChild(root, 0, 0, -1, -1, -1);
    sMakeMatrix(child);
    jobjDisp(child);
    child = GetChild(root, 0, 4, -1, -1, -1);
    sMakeMatrix(child);
    jobjDisp(child);
}
#pragma dont_inline on
typedef struct OrderData {
    u8 pad_00[0x8C];
    s32 field_8C;
} OrderData;

void sDispOrderAnim(void *arg)
{
    s32 order[5] = {-1, 0, 3, 1, 2};
    ZoidState *zoid;
    OrderData *data;
    s32 ai;
    s32 current;
    HSD_JObj *root;
    HSD_JObj *child;
    s32 i;

    if (sCamNN == 1 && dspOrder != 0 && gameEndF <= 3 && dspOrderAI >= 0
        && *(s8 *)((u8 *)&mgI + 0x3218) == 0 && arg != (void *)0
        && (zoid = ((DispCamera *)arg)->zoid, zoid != 0)
        && (data = *(OrderData **)((u8 *)&Zoid[2] + 0x1F3C), data != 0)) {
        if (dspPartnerMsgTm == 0 && data->field_8C != 0) {
            dspOrderAI = data->field_8C;
        }
        ai = dspOrderAI;
        current = order[ai];
        root = dspOrder->models[0];
        if (root != 0) {
            for (i = 0; i < 4; i++) {
                child = GetChild(root, 0, i, -1, -1, -1);
                HiddenParts(child, current != i);
                child = GetChild(root, 0, i, 0, -1, -1);
                HiddenParts(child, current == i);
            }
        }
    }
}
void sDispOrder(DispCamera *camera)
{
    s32 order[5] = {-1, 0, 3, 1, 2};
    HSD_JObj *root;

    if (sCamNN == 1 && *(s32 *)((u8 *)&GameMode + 0x3C) == 0 && dspOrder != 0
        && *(s8 *)((u8 *)&mgI + 0x3218) == 0 && camera->zoid != 0 && camera->zoid == &Zoid[0]
        && Zoid[2].joint != 0 && Zoid[0].slot == Zoid[2].slot && gameEndF <= 3
        && dspOrderAI >= 0 && camera != (void *)0 && Zoid[2].death_started == 0) {
        root = dspOrder->models[0];
        if (root != 0) {
            jobjDispAll(root);
        }
    }
}
#pragma dont_inline off
void sDispPartnerAnim(void *arg)
{
    s32 GetChild_first;

    register s32 i;
    HSD_JObj *bar;
    HSD_JObj *root;
    ZoidState *partner;
    s32 anim;
    f32 max;
    f32 ratio;

    if (*(s32 *)((u8 *)&GameMode + 0x3C) != 0)
        return;
    if (sCamNN != 1)
        return;
    if (dspPartner == 0)
        return;
    if (gameEndF > 3)
        return;
    partner = &Zoid[2];
    if (partner->joint == (void *)0)
        return;
    if (arg == (void *)0)
        return;
    root = dspPartner->models[0];
    if (root == 0)
        return;
    if (dspPartnerMsgF != 0)
        dspPartnerMsgF--;
    if (dspPartnerMsgTm != 0)
        dspPartnerMsgTm--;
    else
        dspPartnerMsg = -1;
    if (dspPartnerMsg >= 8)
        dspPartnerMsg = -1;
    for (i = 0; i < 8; i++) {
        {
            GetChild_first = 0;
            HiddenParts(GetChild(root, GetChild_first, 0, i < 4 ? 0 : 1, i & 3, -1), dspPartnerMsgB != i);
        }
    }
    GetChild(root, 0, -1, 0, 0, -1);
    switch (dspPartnerMsgA) {
    case 0:
        if (dspPartnerMsg != dspPartnerMsgB) {
            if (dspPartnerMsg != -1) {
                SetAnimArc2(dspPartner, 0, dspPartnerMsg < 4 ? 1 : 4);
                dspPartnerMsgB = dspPartnerMsg;
                dspPartnerMsgA = 1;
            } else {
                dspPartnerMsgB = -1;
            }
        }
        break;
    case 2:
        if (dspPartnerMsg != dspPartnerMsgB) {
            SetAnimArc2(dspPartner, 0, dspPartnerMsgB < 4 ? 3 : 6);
            dspPartnerMsgA = 3;
        }
        break;
    }
    anim = -1;
    if (jobjAnim(root) != 0) {
        switch (dspPartnerMsgA) {
        case 1:
            if (dspPartnerMsgV != -1 && dspPartnerMsgM != -1) {
                zdVoiceScG(&Zoid[2], dspPartnerMsgV);
            }
            if (dspPartnerMsgB < 4)
                anim = 2;
            else
                anim = 5;
            dspPartnerMsgA = 2;
            break;
        case 3:
            dspPartnerMsgA = 0;
            anim = 0;
            break;
        }
        if (anim != -1) {
            SetAnimArc2(dspPartner, 0, anim);
            jobjAnim(root);
        }
    }
    if ((bar = GetChild(root, 0, 1, 0, -1, -1)) != 0) {
        if (partner->max_health <= 0)
            partner->max_health = 100;
        max = (f32)partner->max_health;
        ratio = (f32)partner->health / max;
        HSD_JObjSetScaleX(bar, ratio);
    }
}
#pragma dont_inline on
#include "game_person_data.h"

void sDispPartner(DispCamera *enable)
{
    HSD_JObj *root;
    s32 u1;
    s32 u2;
    s32 u3;
    s32 u4;
    s32 u5;
    HSD_JObj *child;
    ZoidState *partner;
    char *name;

    if (*(s32 *)((u8 *)&GameMode + 0x3C) != 0)
        return;
    if (sCamNN != 1)
        return;
    if (dspPartner == 0)
        return;
    if (gameEndF > 3)
        return;
    partner = &Zoid[2];
    if (partner->joint == (void *)0)
        return;
    if (enable == (void *)0)
        return;
    root = dspPartner->models[0];
    if (root == 0)
        return;
    jobjDispAll(root);
    sMakeMatrix(root);
    child = GetChild(root, 0, 2, 0, -1, -1);
    name = Tea_persondat[Zoid[2].person].name;
    putAscJobj(child, 0, name);
}
extern void sDispOrder(DispCamera *camera);
extern void sDispPartner(DispCamera *camera);
extern void s1pZdHP(DispCamera *camera, ZoidState *zoid, ModelArchive *archive, HSD_JObj *joint);
extern void s1pEX(DispCamera *camera, ZoidState *zoid, HSD_JObj *joint);
extern void s1pZdDmg1(ZoidState *zoid, ModelArchive *archive, HSD_JObj *joint);
extern void s1pRader(DispCamera *camera, ZoidState *zoid, HSD_JObj *joint);
extern void s1pWpTgt(DispCamera *camera, ZoidState *zoid);
extern void sDispArea(DispCamera *camera, HSD_JObj *joint);

void sDisp1pA(DispCamera *camera)
{
    ZoidState *zoid;
    HSD_JObj *joint;
    HSD_JObj *target;

    zoid = camera->zoid;
    if (zoid != (void *)0 && zoid->joint != (void *)0 && dspPos00 != 0 && dsp1p != 0) {
        joint = GetChild(dspPos00->models[1], 0, 0, 8, -1, -1);
        target = GetChild(dspPos00->models[1], 0, 1, 8, -1, -1);
        jobjCopyPos(joint, target);
        joint = GetChild(dspPos00->models[2], 0, 0, 8, -1, -1);
        target = GetChild(dspPos00->models[2], 0, 1, 8, -1, -1);
        jobjCopyPos(joint, target);
        joint = 0;
        if (dspPos00 != 0) {
            if (sCamNN == 1) {
                joint = GetChild(dspPos00->models[0], 0, -1, -1, -1, -1);
            } else {
                joint = GetChild(dspPos00->models[1 + sCamNS], 0, camera->half, -1, -1, -1);
            }
        }
        if (joint != (void *)0) {
            sDispScope((ScopeCamera *)camera, joint);
            sDispOrder(camera);
            sDispPartner(camera);
            s1pZdHP(camera, zoid, dsp1p, joint);
            s1pEX(camera, zoid, joint);
            s1pOption((WeaponCamera *)camera, zoid, joint);
            s1pZdDmg1(zoid, dsp1p, joint);
            s1pWeapon((WeaponCamera *)camera, zoid, joint);
            s1pRader(camera, zoid, joint);
            s1pTimer(joint);
            s1pTarget((CameraState2 *)camera, zoid);
            s1pWpTgt(camera, zoid);
            sDispArea(camera, joint);
        }
    }
}

void sDispGameFilter(s32 index, s32 mode)
{
    s32 next;

    if (index < 0)
        return;
    if (index > 2)
        return;
    if (jobjFilter[index] == (void *)0)
        return;
    if (dspFilter == 0)
        return;
    if (anmFilter[index] == -1) {
        if (mode == 0)
            return;
        if (mode == 1)
            next = 0;
        else
            next = 3;
        LoadArcAnimModel2(dspFilter, jobjFilter[index], 0, next);
        anmFilter[index] = next;
        return;
    }
    if (jobjAnim(jobjFilter[index]) != 0) {
        next = anmFilter[index];
        switch (next) {
        case 0:
            next = 1;
            break;
        case 1:
            if (mode != 1)
                next = 2;
            break;
        case 2:
            next = -1;
            break;
        case 3:
            next = 4;
            break;
        case 4:
            if (mode != 2)
                next = 5;
            break;
        case 5:
            next = -1;
            break;
        default:
            return;
        }
        if (next != anmFilter[index]) {
            if (next != -1) {
                LoadArcAnimModel2(dspFilter, jobjFilter[index], 0, next);
                jobjAnim(jobjFilter[index]);
            }
            anmFilter[index] = next;
        }
        if (next == -1)
            return;
    }
    jobjDispAll(jobjFilter[index]);
}
void sDispFilter(DispCamera *camera, ZoidState *zoid)
{
    s32 index;
    s32 hit;
    s32 mode;

    if (zoid == (void *)0)
        return;
    index = camera->half;
    if (sCamNN == 1)
        index = 0;
    hit = 0;
    mode = 0;
    if (zoid->ally_smoke_timer != 0 || zoid->enemy_smoke_timer != 0)
        hit = 1;
    if (hit != 0)
        mode = 1;
    sDispGameFilter(index, mode);
}

void sDispFilterG(void)
{
    if (dspFilter == (void *)0)
        return;
    if (dspFilter->models[0] == (void *)0)
        return;
    switch (gameEndF) {
    case 0:
    case 1:
        return;
    case 2:
        SetAnimArc2(dspFilter, 0, 0);
        gameEndF = 3;
        break;
    case 3:
        if (jobjAnim(dspFilter->models[0]) != 0) {
            gameEndF = 4;
            SetAnimArc2(dspFilter, 0, 1);
            jobjAnim(dspFilter->models[0]);
        }
        break;
    case 4:
    case 5:
    case 6:
        jobjAnim(dspFilter->models[0]);
        break;
    }
    jobjDispAll(dspFilter->models[0]);
}

void sDispFilterB(void)
{
    if (dspFilter == (void *)0)
        return;
    if (dspFilter->models[0] == (void *)0)
        return;
    if (gameEndF == 7)
        gameEndF = 8;
    if (gameEndF == 6)
        gameEndF = 7;
}
#pragma force_active on
static s32 onTime;
static ModelArchive *dspSearch;
static ModelArchive *dspScope;
static ModelArchive *dspOrder;
static ModelArchive *dspMessage;
static ModelArchive *dspWarn;
static ModelArchive *dspBP;
static ModelArchive *dspPause;
static ModelArchive *dspContinue;
static ModelArchive *dspDemoPress;
static HSD_JObj *dspJobjEx3[2];
static s32 dspAnmEx3[2];
static s32 dspAnmEx3n[2];
static ModelArchive *dspRader;
static ModelArchive *dspPartner;
static s32 dspPartnerMsgV;
static s32 dspPartnerMsgM;
static s32 dspPartnerMsgB;
static s32 dspPartnerMsgA;
static ModelArchive *dspPoint;
static ModelArchive *dspPos00;
static s32 menuRet;
#pragma force_active off
typedef struct MessageCamera {
    s32 mode;
    s32 half;
    u8 pad_008[0x1C4 - 8];
    ZoidState *zoid;
} MessageCamera;

extern MessageCamera sCamera[];
extern s32 winF;
extern void sDispLoadPartsMessage(void);
extern s32 getEndF(HSD_JObj *joint);
extern s32 HSD_Randi(s32 maximum);

void sDispMessageMe(MessageCamera *camera)
{
    s32 tblA[4] = { 2, 0, 2, 1 };
    s32 tblB[4] = { 8, 7, 8, 8 };
    HSD_JObj *joint;
    HSD_JObj *target;
    HSD_JObj *child;
    s32 pick;
    s32 value;
    static s8 bJa;

    if (gameEndF >= 4) {
        if (gameEndF == 4)
            sDispLoadPartsMessage();
        if (dspMessage != 0) {
            joint = dspMessage->models[5];
            if (joint != (void *)0) {
                if (gameEndF <= 11) {
                    target = 0;
                    if (dspPos00 != 0) {
                        if (sCamNN == 2)
                            target = GetChild(dspPos00->models[sCamNS + 1], 0, camera->half, 10, -1, -1);
                    }
                    if (target != 0)
                        jobjCopyPos(target, joint);
                    if (camera == (MessageCamera *)sCamera)
                        jobjAnim(joint);
                    if (gameEndF == 4) {
                        bJa = 0;
                        return;
                    }
                    if (winF != 0) {
                        switch (bJa) {
                        case 0:
                            Zoid[0].rumble_timer = 0;
                            Zoid[1].rumble_timer = 0;
                            if (GameMode.game_type == 1 && camera->zoid == 0 && winF == 1) {
                                value = HSD_Randi(4) + 7;
                                if (value < 7)
                                    value = 7;
                                if (value > 10) {
                                    (void)value;
                                    (void)value;
                                }
                            }
                            SetAnimArc2(dspMessage, 5, 0);
                            bJa = 1;
                            return;
                        case 1:
                            if (getEndF(joint) != 0) {
                                SetAnimArc2(dspMessage, 5, 1);
                                bJa = 2;
                            }
                            break;
                        case 2:
                            if (gameEndF == 6)
                                bJa = 3;
                            else
                                getEndF(joint);
                            break;
                        case 3:
                            SetAnimArc2(dspMessage, 5, 2);
                            bJa = 4;
                            break;
                        case 4:
                            if (getEndF(joint) != 0) {
                                SetAnimArc2(dspMessage, 5, 3);
                                bJa = 5;
                                return;
                            }
                            break;
                        case 5:
                            return;
                        }
                        pick = tblA[winF];
                        if (sCamNN == 2) {
                            if (pick == 0) {
                                if (camera->half == 0)
                                    pick = 3;
                                else
                                    pick = 4;
                            }
                            if (pick == 1) {
                                if (camera->half == 0)
                                    pick = 4;
                                else
                                    pick = 3;
                            }
                        }
                        if (GameMode.game_type == 4)
                            pick = tblB[winF];
                        child = GetChild(joint, 0, 9, -1, -1, -1);
                        HiddenParts(child, 0);
                        jobjDisp(child);
                        child = GetChild(joint, 0, pick, -1, -1, -1);
                        HiddenParts(child, 0);
                        jobjDisp(child);
                    }
                }
            }
        }
    }
}
extern s32 gameContinueTm;
extern void SetAnimArc2(ModelArchive *archive, s32 model, s32 selected);

void sDispContinue(void *enable)
{
    s32 time;
    HSD_JObj *yes;
    HSD_JObj *count;
    static s32 cont;
    s32 i;
    HSD_JObj *base;
    s32 dead;
    HSD_JObj *no;

    if (gameEndF < 9) {
        cont = 0;
        return;
    }
    if (dspContinue == 0)
        return;
    if (enable == (void *)0)
        return;
    if (gameEndF >= 11)
        return;
    if (GameMode.game_type != 1)
        return;
    base = dspContinue->models[0];
    yes = dspContinue->models[1];
    no = dspContinue->models[2];
    count = dspContinue->models[3];
    if (base == (void *)0)
        return;
    if (yes == (void *)0)
        return;
    if (no == (void *)0)
        return;
    if (count == (void *)0)
        return;
    if (jobjAnim(yes) != 0)
        SetAnimArc2(dspContinue, 1, GameMode.win == 3 ? 2 : 1);
    if (jobjAnim(no) != 0)
        SetAnimArc2(dspContinue, 2, GameMode.win == 0 ? 2 : 1);
    jobjAnim(count);
    switch (cont) {
    case 0:
        SetAnimArc2(dspContinue, 0, 1);
        SetAnimArc2(dspContinue, 3, 1);
        cont = 1;
        break;
    case 1:
        jobjAnim(base);
        if (gameEndF == 10) {
            SetAnimArc2(dspContinue, 0, 2);
            SetAnimArc2(dspContinue, 3, 2);
            cont = 2;
        }
        break;
    case 2:
        if (jobjAnim(base) != 0) {
            gameEndF = 12;
            return;
        }
        break;
    }
    for (i = 0; i < 4; i++) {
        HiddenParts(GetChild(base, 0, i, -1, -1, -1), i != 0);
        dead = 1;
    }
    jobjDispAll(base);
    if (gameContinueTm > 0) {
        jobjDispAll(yes);
        jobjDispAll(no);
    } else if (GameMode.win == 3) {
        jobjDispAll(yes);
    } else {
        jobjDispAll(no);
    }
    time = gameContinueTm / 60;
    if (time >= 0 && time < 10) {
        for (i = 0; i < 10; i++) {
            HiddenParts(GetChild(count, 0, i, -1, -1, -1), time != i);
            dead = 1;
        }
    }
    if (GameMode.win == 3 && gameEndF == 10)
        return;
    jobjDispAll(count);
}
static void sDispMessage(DispCamera *camera)
{
    sDispMessageMe((MessageCamera *)camera);
    sDispContinue(camera);
}

void PartnerFaceV(s32 message, s32 duration, s32 mode, s32 value)
{
    dspPartnerMsg = message;
    dspPartnerMsgTm = duration;
    dspPartnerMsgM = mode;
    dspPartnerMsgV = value;
}

void PartnerFace(s32 message, s32 duration)
{
    PartnerFaceV(message, duration, -1, -1);
}
#define PAD_START(p) (*(s8 *)((u8 *)(p) + 0x4E))
extern void sPutFontRectC(char *text, HSD_JObj *top_left, HSD_JObj *bottom_right, GXColor color, GXColor shadow, s32 alpha, s32 flags);

void sDispPause(PauseCamera *camera)
{
    HSD_JObj *child;
    HSD_JObj *root;
    HSD_JObj *info;
    HSD_JObj *select;
    s32 mine;
    HSD_JObj *cursor;
    PadState *pad;
    HSD_JObj *end;

    if (camera == (void *)0)
        return;
    if (dspPause == 0)
        return;
    root = dspPause->models[0];
    select = dspPause->models[7];
    info = dspPause->models[8];
    cursor = dspPause->models[9];
    if (root == 0)
        return;
    pad = sPad1;
    if (pause == 2)
        pad = sPad2;
    if (camera->pad == pad)
        mine = 1;
    else
        mine = 0;
    sDispGameFilter(2, (pause == 0 || PAD_START(pad) != 0) ? 0 : 1);
    if (pause == 0)
        return;
    if (pause == 0 || PAD_START(pad) == 0) {
        HiddenParts(GetChild(root, 0, 0, -1, -1, -1), 1);
        HiddenParts(GetChild(root, 0, 1, -1, -1, -1), 0);
        HiddenParts(GetChild(root, 0, 2, -1, -1, -1), GameMode.game_type != 2 || pause != 1);
        HiddenParts(GetChild(root, 0, 3, -1, -1, -1), GameMode.game_type != 2 || pause != 2);
        if (mine != 0) {
            if (jobjAnim(root) != 0) {
                SetAnimArc2(dspPause, 0, 1);
                jobjAnim(root);
            }
        }
        jobjDispAll(root);
        (void)mine;
        if (select != 0 && cursor != 0) {
            if (mine != 0) {
                child = GetChild(select, 0, 0, -1, -1, -1);
                if (jobjAnim(child) != 0) {
                    sSetAnim1(dspPause, 7, child, menuRet == 0 ? 2 : 1);
                    jobjAnim(child);
                }
                child = GetChild(select, 0, 1, -1, -1, -1);
                if (jobjAnim(child) != 0) {
                    sSetAnim1(dspPause, 7, child, menuRet == 1 ? 2 : 1);
                    jobjAnim(child);
                }
            }
            jobjDispAll(select);
            if (GameMode.game_type == 2) {
                child = GetChild(select, 0, menuRet, pause, -1, -1);
                jobjCopyPos(child, cursor);
                sMakeMatrix(cursor);
                jobjDispAll(GetChild(cursor, 0, 2 - pause, -1, -1, -1));
            }
        }
        if (GameMode.game_type == 4 && info != 0) {
            if (jobjAnim(info) != 0) {
                SetAnimArc2(dspPause, 8, 1);
                jobjAnim(info);
            }
            jobjDispAll(info);
            sMakeMatrix(info);
            child = GetChild(info, 0, 0, -1, -1, -1);
            end = GetChild(info, 0, 0, 0, -1, -1);
            child = GetChild(info, 0, 1, -1, -1, -1);
            end = GetChild(info, 0, 1, 0, -1, -1);
            sPutFontRectC(mgI.victory_text, child, end, (GXColor){ 255, 255, 255, 255 }, (GXColor){ 0, 0, 0, 0 }, 0, 2);
        }
    }
}
typedef struct PausePad {
    u8 pad_00[0x42];
    s8 field_42;
    u8 pad_43[5];
    s8 field_48;
    u8 pad_49[5];
    s8 field_4E;
    u8 pad_4F[8];
    s8 field_57;
    s8 field_58;
    u8 pad_59[4];
    s8 pressed_a;
    s8 pressed_b;
} PausePad;

extern void SeOn(s32 id);

void sDispPauseProc(void)
{
    HSD_JObj *root;
    HSD_JObj *cursor;
    HSD_JObj *select;
    HSD_JObj *extra;
    PausePad *pad;
    s32 index;

    if (dspPause != 0) {
        root = dspPause->models[0];
        cursor = dspPause->models[7];
        select = dspPause->models[8];
        extra = dspPause->models[9];
        if (root != 0) {
            if (pBak != pause) {
                SetAnimArc2(dspPause, 0, 0);
                if (cursor != 0)
                    SetAnimArc2(dspPause, 7, 0);
                if (select != 0)
                    SetAnimArc2(dspPause, 8, 0);
                jobjAnim(root);
                pBak = pause;
                menuRet = 0;
            }
            pad = (PausePad *)sPad1;
            if (pause == 2)
                pad = (PausePad *)sPad2;
            if (pad->field_4E == 0) {
                if (pad->field_58 != 0 || pad->field_57 != 0) {
                    menuRet = 1 - menuRet;
                    SeOn(0x71);
                }
                if (pad->field_48 != 0 && pad->field_42 == 0) {
                    index = -1;
                    if (menuRet == 1) {
                        if (GameMode.game_type == 2) {
                            if (pause != 0)
                                index = pause - 1;
                        } else {
                            index = 0;
                        }
                        gameTime = 0;
                        if (index != -1) {
                            Zoid[index].health = 0;
                            Zoid[index].knockback[0] = 3.0f;
                            Zoid[index].knockback[1] = 0.0f;
                            Zoid[index].knockback[2] = 0.0f;
                            Zoid[index].requested_action = 0x22;
                        }
                    }
                    pause = 0;
                    pad->pressed_a = 0;
                    SeOn(0x10);
                }
                if (pad->pressed_b != 0 && pause != 0) {
                    pause = 0;
                    SeOn(0x6c);
                }
            }
        }
    }
}
extern s32 bpTm;
extern s32 bp[12];

void sDispBP(void *enable)
{
    char text[20];
    HSD_JObj *root;
    HSD_JObj *child;
    s32 total;
    s32 i;

    if (dspBP == 0)
        return;
    root = dspBP->models[0];
    if (enable == 0)
        return;
    if (((s32 *)&GameMode)[1] != 1)
        return;
    if (((s32 *)&GameMode)[3] != 2)
        return;
    if (gameEndF != 11)
        return;
    if (bpTm == 0) {
        SetAnimArc2(dspBP, 0, 0);
        bpTm = 1;
    }
    if (jobjAnim(root) != 0)
        SetAnimArc2(dspBP, 0, 1);
    if (root != 0) {
        for (i = 0; i < 5; i++)
            HiddenParts(GetChild(root, 0, 9, i, -1, -1), ((s32 *)&GameMode)[14] - 1 != i);
    }
    jobjDispAll(root);
    sMakeMatrix(root);
    for (i = 0, total = 0; i < 6; i++) {
        s32 index[6] = { 0, 1, 3, 4, 5, 6 };
        child = GetChild(root, 0, index[i], 0, -1, -1);
        if (i == 5)
            sprintf(text, "%06d", (&bp[6])[i]);
        else
            sprintf(text, "%04d", (&bp[6])[i]);
        if (i != 5)
            total += (&bp[6])[i];
        bp[11] = total;
        putAscJobj(child, 3, text);
    }
}
#undef lit_1819
#define lit_1819 "%06d"
void sDispNext(DispCamera *enable)
{
    HSD_JObj *joint;
    HSD_JObj *child;
    s32 index;

    if (enable == 0)
        return;
    if (dspMessage == 0)
        return;
    joint = dspMessage->models[5];
    if (gameEndF != 13)
        return;
    if (*(s32 *)((u8 *)&GameMode + 0x18) == 11)
        index = 6;
    else
        index = 5;
    child = GetChild(joint, 0, index, -1, -1, -1);
    if (jobjAnim(joint) != 0)
        SetAnimArc2(dspMessage, 5, 1);
    sMakeMatrix(joint);
    jobjDispAll(child);
}
void sDispDemoPress(DispCamera *enable)
{
    if (enable == 0)
        return;
    if (dspDemoPress == 0)
        return;
    if (GameMode.game_type != 9)
        return;
    if ((vsyncN & 0x10) == 0)
        return;
    jobjDispAll(dspDemoPress->models[0]);
}
extern void HSD_CObjGetScissor(struct HSD_CObj *cobj, u16 *scissor);
extern void HSD_CObjSetScissor(struct HSD_CObj *cobj, u16 *scissor);
extern s32 HSD_CObjSetCurrent(struct HSD_CObj *cobj);
extern void HSD_CObjEndCurrent(void);
extern void HSD_LObjSetupInit(struct HSD_CObj *cobj);
extern void HSD_LObjSetCurrentAll(struct HSD_LObj *lobj);
extern void GXSetCullMode(s32 mode);
extern void HSD_SetZSortMode(s32 a, s32 b);
extern void sDispFilter(DispCamera *camera, ZoidState *zoid);
extern void sDispDemoPress(DispCamera *camera);
extern void sDispLoadPartsMessage(void);
extern void sDispLoadPartsContinue(void);
extern void sDispLoadPartsBP(void);
extern void sDispMessage(DispCamera *camera);
extern void sDispNext(DispCamera *camera);

void sDisp(DispCamera *camera)
{
    u16 scissor[6];
    s32 filtered;
    struct HSD_CObj *cobj;

    if (camera == (void *)0)
        return;
    cobj = camera->cobj;
    if (cobj == (void *)0)
        return;
    filtered = 0;
    HSD_CObjGetScissor(camera->cobj, scissor);
    if (dsp1p != 0)
        HSD_CObjSetScissor(dsp1p->camera, scissor);
    if (dspMessage != 0)
        HSD_CObjSetScissor(dspMessage->camera, scissor);
    HSD_SetZSortMode(0, 0);
    if (dsp1p != 0 && HSD_CObjSetCurrent(dsp1p->camera) != 0) {
        HSD_LObjSetCurrentAll(camera->lights);
        HSD_LObjSetupInit(dsp1p->camera);
        GXSetCullMode(0);
        sDispFilter(camera, camera->zoid);
        filtered = 1;
        sDispFilterG();
        if (gameEndF < 3)
            sDisp1pA(camera);
        sDispPause((PauseCamera *)camera);
        sDispDemoPress(camera);
        HSD_CObjEndCurrent();
    }
    if (gameEndF >= 4)
        sDispPurgePartsGame();
    if (gameEndF >= 4)
        sDispLoadPartsMessage();
    if (gameEndF >= 9)
        sDispLoadPartsContinue();
    if (gameEndF == 11 && GameMode.game_type == 1)
        sDispLoadPartsBP();
    if (dspMessage != 0 && HSD_CObjSetCurrent(dspMessage->camera) != 0) {
        HSD_LObjSetCurrentAll(camera->lights);
        HSD_LObjSetupInit(dspMessage->camera);
        GXSetCullMode(0);
        if (filtered == 0)
            sDispFilterG();
        if (gameEndF >= 4)
            sDispMessage(camera);
        sDispFilterB();
        sDispBP(camera);
        sDispNext(camera);
        HSD_CObjEndCurrent();
        GXSetCullMode(2);
    }
    HSD_SetZSortMode(1, 1);
}

void dispAnim(void *arg)
{
    sDispPartnerAnim(arg);
    sDispOrderAnim(arg);
}
extern void *camTmp;

void sDispAnim(void)
{
    void *camera;
    s32 i;

    if (dsp1p != 0)
        jobjAnim(dsp1p->models[14]);
    for (i = 0; i < 4; i++) {
        camera = (u8 *)sCamera + i * 0x590;
        camTmp = camera;
        if (*(void **)((u8 *)camTmp + 0x244) != 0)
            dispAnim(camera);
    }
    camTmp = sCamera;
}
#pragma force_active on
ModelArchive *dspFilter;
s32 dspPartnerMsgF;
s32 dspPartnerMsgTm2;
s32 dspPartnerMsgTm;
s32 dspPartnerMsg;
ModelArchive *dsp1p;
s32 dspOrderAI;
u32 dspOrderTm;
s32 gameTime;
u16 dlgzoidno;
u16 dlgpartsno;
u16 dlgpersonno;
static s32 pBak = -1;
#pragma force_active off
}
