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
#include "math_support.h"
#include "angle.h"
#include "lobj.h"

typedef struct FootJoint {
    HSD_JObj *joint;
    f32 offset;
    f32 ground;
    f32 height;
    f32 depth;
    f32 field_14;
    f32 field_18;
    f32 rot_z;
    f32 rot_x;
    u32 flags;
    s32 no_contact_frames;
    f32 field_2C;
    u16 ground_polygon_index;
    u8 reserved_32[2];
} FootJoint;

typedef char FootJointSizeCheck[sizeof(FootJoint) == 0x34 ? 1 : -1];

typedef struct ZoidState {
    char name[0x20];
    s32 type;
    s32 person;
    u8 pad_0028[0x8];
    struct HSD_Archive *archive;
    ModelAnimationTable *animations;
    void *field_0038;
    void *field_003C;
    HSD_JObj *joint;
    JointPose *pose;
    struct HSD_AObj *aobj;
    u8 pad_004C[0x34];
    HSD_JObj *field_0080;
    u8 pad_0084[0x18];
    HSD_JObj *field_009C;
    u8 pad_00A0[0x18];
    HSD_JObj *field_00B8;
    u8 pad_00BC[0x18];
    HSD_JObj *field_00D4;
    u8 pad_00D8[0x18];
    HSD_JObj *field_00F0;
    u8 pad_00F4[0x18];
    HSD_JObj *field_010C;
    u8 pad_0110[0x18];
    HSD_JObj *field_0128;
    u8 pad_012C[0x18];
    HSD_JObj *field_0144;
    u8 pad_0148[0x4];
    s32 health;
    s32 max_health;
    s16 option_type;
    u8 option_use_state;
    u8 pad_0157[0xB];
    s16 booster_on;
    s16 additional_parts[3];
    u8 pad_016A[0x2];
    CoDamage damages[6];
    ZoidAttackObject attack_objects[10];
    u8 body_coobj[0x9C];
    u8 head_coobj[0x9C];
    u8 neck_coobj[0x9C];
    u8 bust_coobj[0x9C];
    u8 hip_coobj[0x9C];
    u8 tail_coobjs[8][0x9C];
    u8 l_arm1_coobj[0x9C];
    u8 l_arm2_coobj[0x9C];
    u8 r_arm1_coobj[0x9C];
    u8 r_arm2_coobj[0x9C];
    u8 l_leg1_coobj[0x9C];
    u8 l_leg2_coobj[0x9C];
    u8 r_leg1_coobj[0x9C];
    u8 r_leg2_coobj[0x9C];
    u8 l_foot_coobj[0x9C];
    u8 r_foot_coobj[0x9C];
    HSD_JObj *body_base_joint;
    HSD_JObj *head_base_joint;
    HSD_JObj *neck_base_joint;
    HSD_JObj *bust_base_joint;
    HSD_JObj *hip_base_joint;
    HSD_JObj *tail_base_joints[8];
    s32 super_attack_enabled;
    s32 super_attack_cooldown;
    s32 super_attack_input;
    s32 face_lock;
    s32 mount_has_weapon[20];
    s32 field_1360;
    s32 field_1364;
    s32 weapon_mount;
    u8 pad_1300[0x4];
    s32 selected_weapon_mount;
    s32 field_1374;
    WeaponMount weapon_mounts[20];
    WeaponMountEntry mount_extra[20];
    WeaponSlot weapon_slots[30];
    HSD_JObj *camera_joint;
    u8 pad_19C8[0x18];
    s32 ground_turn;
    f32 field_1A78;
    HSD_JObj *field_1A7C;
    f32 field_1A80[3];
    u8 pad_19F8[0x4];
    f32 field_1A9C[3];
    u8 pad_1A08[0x4];
    HSD_JObj *spik_joints[6];
    HSD_JObj *f_base_joint;
    HSD_JObj *b_base_joint;
    HSD_JObj *field_1ACC;
    HSD_JObj *buckle_joint;
    HSD_JObj *field_1AD4;
    HSD_JObj *field_1AD8;
    HSD_JObj *field_1ADC;
    HSD_JObj *field_1AE0;
    f32 senter_translation_y;
    f32 senter_translation_z;
    u8 pad_1A4C[0x4];
    FootJoint feet[4];
    f32 ground_normal_sum[3];
    s32 ground_normal_count;
    s32 foot_count;
    s32 field_1B34;
    s32 death_pending;
    s32 field_1BE4;
    f32 field_1BE8;
    void *field_1BEC;
    f32 ground_height;
    f32 field_19FC;
    f32 field_1BF8;
    s32 field_1BFC;
    s32 foot_contacts;
    s32 grounded;
    u8 pad_1B60[0x8];
    f32 position[3];
    f32 field_1C24[3];
    f32 height_offset;
    s32 field_1C34;
    f32 height_offset_speed;
    u8 pad_1B8C[0xC];
    s32 field_1C48;
    u8 pad_1B9C[0x4];
    f32 step_heading;
    f32 field_1C54;
    f32 field_1C58;
    f32 stick_steady_frames;
    f32 stick_frames;
    f32 stick_angle;
    f32 field_1C74;
    f32 field_1C78;
    f32 heading;
    f32 high_angle;
    f32 field_1C84;
    u8 pad_1BCC[0x8];
    f32 field_1C90[3];
    u8 pad_1BE0[0x4];
    f32 steer;
    u8 pad_1BE8[0x4];
    f32 face_yaw;
    f32 face_pitch;
    f32 ground_rx;
    f32 ground_rz;
    f32 f_base_rx;
    f32 f_base_rz;
    u8 pad_1C04[0x1C];
    f32 launch_velocity[3];
    u8 us_pad_1C2C[0xC];
    f32 knockback[3];
    f32 field_1D20[3];
    f32 field_1D2C;
    f32 displacement[3];
    u8 pad_1C54[0x58];
    s32 field_1DA0;
    s32 requested_action;
    s32 action;
    s32 field_1DAC;
    s32 field_1DB0;
    s32 action_phase;
    s32 gene_frames;
    u8 pad_1CC8[0x4];
    s32 jump_phase;
    u8 pad_1CD0[0x4];
    f32 velocity[3];
    s32 field_1DD4;
    s32 current_animation;
    u8 pad_1CE8[0x8];
    s32 attack_index;
    s32 field_1DF8;
    u8 pad_1CF8[0x34];
    s32 anim1_frames;
    u8 pad_1D30[0x4];
    s32 animation_stopped;
    s32 animation_looped;
    u8 pad_1D3C[0x4];
    s32 animation_frame;
    u8 pad_1D44[0x4];
    s32 field_1E50;
    s32 field_1E54;
    u8 pad_1D50[0x4];
    s32 field_1E5C;
    u8 pad_1D58[0xC];
    s32 death_started;
    s32 lock_frames;
    u8 pad_1D6C[0xC];
    s32 rumble_timer;
    u8 index;
    u8 slot;
    u8 target_pending;
    u8 target_slot;
    s8 attack_target;
    u8 pad_1D81[0x1];
    u8 field_1E9A;
    u8 pad_1D83[0x1];
    s32 field_1E9C;
    s32 field_1EA0;
    u8 pad_1D8C[0x8];
    u16 field_1EAE;
    u16 field_1EB0;
    u16 field_1EB2;
    u16 ground_polygon;
    u8 pad_1D9C[0x34];
    f32 field_1EF8;
    u8 pad_1DD4[0x8];
    s32 field_1F04;
    f32 distances[16];
    u8 pad_1E20[0x30];
    f32 target_angles[16];
    u8 pad_1E90[0x6C];
    u16 *slider_info;
    u16 slider_positions[20];
    void *field_2090;
    void *anim_data;
    struct ZoidAIState *ai;
    u8 pad_1F34[0x44];
    s32 turn_bonus;
    u8 pad_1F7C[0x10];
} ZoidState;

typedef char ZoidStateSizeCheck[sizeof(ZoidState) == 0x1F98 ? 1 : -1];

typedef struct PadState2 {
    u8 pad_00[0x40];
    s8 held_40;
    s8 held_41;
    s8 held_42;
    u8 pad_43[2];
    s8 held_45;
    u8 pad_46;
    s8 held_47;
    u8 pad_48[7];
    s8 pressed_4f;
    s8 pressed_50;
    s8 pressed_51;
    s8 pressed_52;
    u8 pad_53[8];
    s8 pressed_5b;
    s8 pressed_5c;
    s8 pressed_5d;
    s8 pressed_5e;
    u8 pad_5f;
    s8 pressed_60;
    s8 pressed_61;
    u8 pad_62[2];
    f32 stick_x;
    f32 stick_y;
    f32 substick_x;
    f32 substick_y;
    u8 pad_74[0x10];
    s8 rumble;
} PadState2;

typedef struct CameraState2 {
    s32 mode;
    s32 half;
    struct HSD_LObj *lights;
    struct HSD_Shadow *shadow;
    f32 focus_heading[3];
    u8 reserved_01C[0x8];
    f32 focus_elevation;
    u8 reserved_028[0x4];
    f32 focus_distance;
    s32 mount_index;
    s32 mount_timer;
    u8 reserved_038[0x14];
    s8 mount_blocked[20];
    s8 mount_hidden[20];
    Vec3 mount_positions[20];
    s32 mount_times[20];
    s32 field_1B4;
    u8 reserved_1B8[0x4];
    s32 field_1BC;
    HSD_JObj *field_1C0;
    ZoidState *zoid;
    s32 target_index;
    f32 prev_zoid_pos[3];
    f32 prev_target_pos[3];
    s32 field_1E4;
    PadState2 *pad;
    u8 reserved_1EC[0x4];
    Vec3 relative_origin;
    Vec3 focus;
    Vec3 field_208;
    f32 eye[3];
    f32 interest[3];
    f32 saved_eye[3];
    f32 saved_interest[3];
    void *cobj;
    Vec3 direction;
    f32 planar[2];
    f32 planar_length;
    f32 yaw;
    f32 projection[7];
    f32 viewport[6];
    f32 view[3][4];
    f32 inverse_view[3][4];
    s8 substick_held;
    s8 field_2F9;
    s8 scope_on;
    u8 pad_2FB[1];
    f32 fov;
    void *scope;
    f32 zoid_distance[16];
    u8 reserved_344[0x240];
    s32 range_index;
    s32 target_marker_count;
    s32 range_timer;
} CameraState2;

typedef char CameraState2SizeCheck[sizeof(CameraState2) == 0x590 ? 1 : -1];

typedef struct AiState2 {
    u8 pad_00[0x40];
    s32 field_40;
    u8 pad_44[0x8C - 0x44];
    s32 previous_command;
    s32 command;
} AiState2;

typedef struct PersonData2 {
    u8 pad_00[0x38];
    s16 field_38;
    u8 pad_3A[0x68 - 0x3A];
} PersonData2;

typedef struct RrrBuf {
    u8 data[512] __attribute__((aligned(32)));
} RrrBuf;

extern RrrBuf rrrbuf;
extern ZoidState Zoid[16];
extern s32 sCamMuteki;
extern s32 gameTime;
extern s32 padBy;
extern s32 rumble;
extern s32 nowStage;
extern u32 vsyncN;
extern s32 dspPartnerMsgTm;
extern s32 dspPartnerMsgTm2;
extern s32 dspPartnerMsgF;
extern s32 dspOrderAI;
extern s32 dspOrderTm;
extern s8 mgI[];
extern PersonData2 Tea_persondat[];

extern void sCameraTargetZoid(CameraState2 *camera, ZoidState *zoid);
extern void sEffectSet(s32 type, s32 mode, f32 *position, void *source, ZoidState *zoid);
extern void camDispZoid(CameraState2 *camera, ZoidState *zoid, u8 *control);
extern s32 zdSatkOk(ZoidState *zoid);
extern void zdWeaponChg(ZoidState *zoid);
extern void zdWeaponFireON(ZoidState *zoid);
extern void zdWeaponFireOFF(ZoidState *zoid);
extern void zdAtack1(ZoidState *zoid);
extern void ZoidSearchNextTarget(ZoidState *zoid);
extern void zdStep0f(ZoidState *zoid, f32 angle);
extern void *zdWeaponScope(ZoidState *zoid, s32 mount);
extern void PartnerFaceV(s32 face, s32 frames, s32 person, s32 voice);
extern s32 HSD_Randi(s32 maximum);
extern void zdSuperAtackAng(ZoidState *zoid, CameraState2 *camera);
extern f32 atan2f(f32 y, f32 x);
extern void angLimit(f32 *angle);

typedef struct GroundMask {
    u8 bits[4];
} GroundMask;

typedef struct CamMine {
    s32 mode;
    s32 half;
    u8 reserved_08[8];
    f32 focus_heading;
    u8 reserved_14[0x38];
    s8 mount_blocked[20];
    s8 mount_hidden[20];
    f32 mount_positions[20][3];
    s32 mount_times[20];
} CamMine;

typedef struct CamMount {
    s32 slot;
    s32 angle;
    s32 distance;
    s32 height;
} CamMount;

extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern f32 sColPosAndY(Vec3 *point, u16 *selector, GroundMask *mask);
extern f32 sColPtAll(f32 *first, f32 *second, GroundMask *mask);
extern ModelArchive *dspBP;
extern ModelArchive *dspMessage;
extern ModelArchive *dspOrder;
extern ModelArchive *dspPause;
extern ModelArchive *dspWarn;
extern s32 vcSeekMode;

extern void camSameTime(CameraState2 *camera);
extern void camInEnemy(CameraState2 *camera, ZoidState *zoid);
extern void searchCamBetterOn(CameraState2 *camera);
extern void searchCamBetterOff(CameraState2 *camera);
extern s32 searchCamBetterOnAtk(CameraState2 *camera, ZoidState *zoid, s32 slot);
extern void setCamTgtLockON(CameraState2 *camera);
extern void setCamTgtLockOFF(CameraState2 *camera);
extern void sCameraCalcET(CameraState2 *camera);
extern void camPara(CameraState2 *camera);
extern void camLimit(CameraState2 *camera);
extern s32 sCamBallOut(CameraState2 *camera, f32 radius);
extern s32 linierCamLimitXZ(CameraState2 *camera, f32 limit);
extern void linierCamXZ(CameraState2 *camera, s32 mode);
extern void camCockpit(CameraState2 *camera, ZoidState *zoid);
extern f32 HSD_CObjGetFov(void *cobj);
extern void HSD_CObjSetFov(void *cobj, f32 fov);
extern void HSD_CObjSetInterest(void *cobj, Vec3 *interest);
extern void HSD_CObjSetEyePosition(void *cobj, Vec3 *position);
extern void sCameraCalc(CameraState2 *camera);
extern f32 sColPt(Vec3 *start, Vec3 *end, GroundMask *mask);
extern f32 acosf(f32 x);
extern f32 sLenJobj(HSD_JObj *a, HSD_JObj *b);
extern s32 sctblLen;
extern s32 sCamNN;
extern s32 sCamNS;
extern s32 camZdLimit2(CameraState2 *camera, ZoidState *zoid);
extern s32 chkPos(f32 *a, f32 *b);

typedef struct IntSeE {
    f32 distance;
    s16 se;
    f32 x;
    f32 z;
    s32 handle;
    s32 timer;
    s32 delay;
} IntSeE;

extern IntSeE IntSe[32];
extern char vcSeekStr[200];
extern char vcSeekNow[200];
extern u8 vcSeekIfo[60];
extern s32 vcSeekMode2;
extern s16 TEA_VOICEVOL;
extern void sReadPre(void);
extern s32 DVDClose(void *info);
extern s32 StrCmp(char *a, char *b);
extern void TEA_VoiceFilePlay(char *name, u8 vol, s32 flag);
extern s32 camVoiceSeekFile(char *name);
extern void HSD_AudioStreamVolume(s32 a);
extern void HSD_AudioStreamStop(s32 a);
extern s32 DVDConvertPathToEntrynum(char *path);
extern s32 DVDFastOpen(s32 entry, void *info);
extern s32 DVDReadAsyncPrio(void *info, void *buf, s32 len, s32 off, void *cb, s32 prio);
extern char *strcpy(char *d, const char *s);
extern s32 camVoiceSeekCB(void *arg);
extern void OSReport(const char *format, ...);
extern void camVoiceClose(void);
extern void sCameraReset(CameraState2 *camera);
extern s32 camVoiceSeek(const char *name);
extern void ZeroMemory(void *p, s32 n);
extern int sprintf(char *s, const char *format, ...);
extern void GXGetProjectionv(f32 *ptr);
extern void GXGetViewportv(f32 *vp);
extern void HSD_CObjGetViewingMtx(void *cobj, f32 (*mtx)[4]);
extern f32 (*HSD_CObjGetInvViewingMtxPtr(void *cobj))[4];
extern void PSMTXCopy(f32 (*src)[4], f32 (*dst)[4]);
extern f32 PSVECMag(Vec3 *vector);
#define sCam ((CameraState2 *)sCamera)

typedef struct ScreenPoint {
    f32 x, y;
} ScreenPoint;
extern s32 nowShadow;
extern void *camTmp;
extern void shadowRendering(void *shadow, s32 index);
extern struct HSD_Shadow *makeShadow(HSD_LObj *light);
extern void HSD_ShadowAddObject(struct HSD_Shadow *shadow, HSD_JObj *jobj);
extern void HSD_ShadowSetActive(struct HSD_Shadow *shadow, int active);
extern s32 TEA_SeOnF(u16 se, f32 x, f32 z);
extern s32 HSD_AudioSFXCheck(s32 handle);
extern void camVoiceSeekInt(void);
extern void GXProject(f32 x, f32 y, f32 z, f32 mtx[3][4], f32 *pm, f32 *vp, f32 *sx, f32 *sy, f32 *sz);
extern s32 sctblMin1a[4];
extern s32 sctblMin2a[4];
extern s32 sctblMin2b[4];
extern s32 sctblMin3a[4];
extern s32 sctblMin3b[4];
extern s32 sctblMax1a[4];
extern s32 sctblMax2a[4];
extern s32 sctblMax2b[4];
extern s32 sctblMax3a[4];
extern s32 sctblMax3b[4];
extern s32 sCameraIn2(CameraState2 *camera, HSD_JObj *joint);
extern f32 sLenPoint3dR(Vec3 *point);
extern s32 GameMode[];
extern void sCameraRela(CameraState2 *camera, Vec3 *position, Vec3 *relative);
extern s32 SE_TableList[];
extern f32 sLiner(f32 a, f32 b, f32 t);
void sCameraCtrl(CameraState2 *camera);
#pragma force_active on
u64 sCamera[712];
IntSeE IntSe[32];
char vcSeekStr[200];
char vcSeekNow[200];
u8 vcSeekIfo[60];
RrrBuf rrrbuf;
static u32 camNormal_pos_wobjdesc[5] = {
    0x00000000, 0x41A00000, 0x41A00000, 0x41A00000, 0x00000000, 
};
static u32 camNormal_interest_wobjdesc[5] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
u32 cameraNormal[14] = {
    0x00000000, 0x00000001, 0x00000280, 0x000001E0, 0x00000280, 0x000001E0, 0x80129578, 0x8012958C, 
    0x00000000, 0x00000000, 0x3F8CCCCD, 0x465AC000, 0x42700000, 0x3FAA3D71, 
};
s32 sctblMin1a[4] = {
    50, 590, 30, 400, 
};
s32 sctblMin2a[4] = {
    50, 590, 25, 190, 
};
s32 sctblMin2b[4] = {
    50, 590, 265, 440, 
};
s32 sctblMin3a[4] = {
    5, 315, 50, 400, 
};
s32 sctblMin3b[4] = {
    325, 635, 50, 400, 
};
s32 sctblMax1a[4] = {
    130, 400, 100, 360, 
};
s32 sctblMax2a[4] = {
    130, 400, 50, 170, 
};
s32 sctblMax2b[4] = {
    130, 400, 290, 420, 
};
s32 sctblMax3a[4] = {
    25, 305, 100, 360, 
};
s32 sctblMax3b[4] = {
    335, 625, 100, 360, 
};
static u32 mnStage_1727[16] = {
    0x8011CF24, 0x00000004, 0x802F14D0, 0x00000000, 0x8011CF30, 0x00000004, 0x802F1508, 0x00000000, 
    0x802F3500, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
u32 radarLen[269] = {
    0x00000BB8, 0x000007D0, 0x000007D0, 0x00000BB8, 0x00000BB8, 0x000007D0, 0x000008FC, 0x000008FC, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000834, 0x00000DAC, 0x00000BB8, 0x000003E8, 0x000009C4, 0x000009C4, 0x00000708, 0x00000DAC, 
    0x000004B0, 0x000006A4, 0x000009C4, 0x000009C4, 0x000008FC, 0x000009C4, 0x00000BB8, 0x00001F40, 
    0x00000320, 0x00000320, 0x00000BB8, 0x00000320, 0x000009C4, 0x00000BB8, 0x00000320, 0x000009C4, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x000006A4, 0x00000DAC, 0x000004B0, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
    0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 0x00000BB8, 
};
static const u64 lit_462[4] = {
    0x00000140000000F0ULL, 0x0000014000F001E0ULL, 0x01400280000000F0ULL, 0x0140028000F001E0ULL, 
};
static const u64 lit_463[4] = {
    0x00000140000000F0ULL, 0x0000014000F001E0ULL, 0x01400280000000F0ULL, 0x0140028000F001E0ULL, 
};
namespace { __declspec(section ".sdata2") extern const f32 lit_377[1] = { 0.0f }; }
#pragma force_active off
namespace { extern const f32 lit_377[1]; }
extern PadState2 sPad[];
extern u32 sCamNormal;
extern void *HSD_CObjLoadDesc(void *desc);

void sCameraInit(void)
{
    s32 i;

    sCamNormal = (u32)HSD_CObjLoadDesc(&cameraNormal);
    for (i = 0; i < 4; i++) {
        ZeroMemory(&sCam[i], sizeof(CameraState2));
        sCam[i].pad = &sPad[i];
        sCam[i].field_1B4 = 3;
        sCam[i].focus_heading[0] = lit_377[0];
        sCam[i].focus_heading[2] = 0.2617994f;
        sCam[i].focus_elevation = lit_377[0];
        sCam[i].focus_distance = lit_377[0];
    }
}
#pragma push
#pragma dont_inline off
namespace { extern const f32 lit_377[1]; }
extern void HSD_ShadowRemove(struct HSD_Shadow *shadow);

typedef struct CamObjInfo CamObjInfo;
typedef struct CamObj {
    CamObjInfo *class_info;
    u16 reference_count;
} CamObj;
struct CamObjInfo {
    u8 pad[0x30];
    void (*release)(CamObj *object);
    void (*destroy)(CamObj *object);
};

static inline s32 camera_reference_decrement(void *object)
{
    s32 result;

    result = (u64)(((CamObj *)object)->reference_count == (u16)0xFFFF);
    if (result) {
        return result;
    }
    result = (((CamObj *)object)->reference_count == (u16)0);
    ((CamObj *)object)->reference_count--;
    return result;
}

static inline void camera_unreference(void *object)
{
    if (object != 0 && camera_reference_decrement(object)) {
        if (object != 0) {
            ((CamObj *)object)->class_info->release((CamObj *)object);
            ((CamObj *)object)->class_info->destroy((CamObj *)object);
        }
    }
}

void sCameraReset(CameraState2 *camera)
{
    register void *camera_unreference_object;

    u32 i;

    if (camera != (void *)0) {
        if (camera->shadow != 0) {
            HSD_ShadowRemove(camera->shadow);
            camera->shadow = 0;
        }
        if (camera->cobj != 0) {
            {
                camera_unreference_object = camera->cobj;
                camera_unreference(camera_unreference_object);
            }
            camera->cobj = 0;
        }
        camera->zoid = 0;
        camera->field_1C0 = 0;
        camera->focus_distance = lit_377[0];
        *(f32 *)camera->reserved_01C = lit_377[0];
        camera->focus_heading[0] = lit_377[0];
        camera->focus_heading[2] = 2 * (3.1415926 * 10.0f) / 360.0;
        camera->field_2F9 = 0;
        camera->scope_on = 0;
        for (i = 0; (i << 20) != 0; i++) {
            *(s32 *)((u8 *)camera + 0x164 + 4 * i) = 120;
        }
    }
}
#pragma pop
namespace { extern const f32 lit_377[1]; extern const f64 lit_489[1]; extern const f64 lit_490[1]; extern const f32 lit_491[1]; }
#pragma dont_inline off
static inline f32 calcSqrt(f32 value)
{
    if (value > lit_377[0]) {
        f64 estimate = __frsqrte(value);
        volatile f32 result;

        estimate = lit_489[0] * estimate * (lit_490[0] - estimate * estimate * value);
        estimate = lit_489[0] * estimate * (lit_490[0] - estimate * estimate * value);
        estimate = lit_489[0] * estimate * (lit_490[0] - estimate * estimate * value);
        estimate = lit_489[0] * estimate * (lit_490[0] - estimate * estimate * value);
        result = value * estimate;
        return result;
    }
    return value;
}

void sCameraCalcET(CameraState2 *camera)
{
    ZoidState *zoid;
    HSD_JObj *joint;
    void *anim;
    s32 *mount;
    u8 *control;
    s32 axis;
    f32 planar;
    f32 length;
    f32 z;
    f32 x;

    if (camera != (void *)0 && camera->cobj != (void *)0 && (zoid = camera->zoid) != (void *)0
        && (control = (u8 *)zoid->field_2090, anim = zoid->anim_data, mount = (s32 *)(control + camera->mount_index * 16),
            (joint = zoid->weapon_slots[*mount].joint) != (void *)0)) {
        for (axis = 0; axis < 3; axis++)
            (&camera->direction.x)[axis] = (&camera->focus.x)[axis] - joint->matrix[axis][3];
        z = camera->direction.z;
        x = camera->direction.x;
        planar = calcSqrt(z * z + x * x);
        length = sLenPoint3dR(&camera->direction);
        if (lit_377[0] != length) {
            camera->focus_elevation = acosf(planar / length);
            camera->focus_heading[0] = atan2f(x, z);
        }
    }
}
#pragma dont_inline on
void sCameraRela(CameraState2 *camera, Vec3 *position, Vec3 *relative)
{
    f32 dx;
    f32 dz;
    f32 cosine;
    f32 sine;
    f32 cosine_angle;
    f32 sine_angle;
    ZoidState *zoid;

    cosine_angle = camera->yaw;
    cosine = cosf(cosine_angle);
    sine_angle = camera->yaw;
    sine = sinf(sine_angle);
    zoid = camera->zoid;
    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        dz = position->z - camera->eye[2];
        dx = position->x - camera->eye[0];
        relative->x = -dx * cosine + dz * sine;
        relative->z = dx * sine + dz * cosine;
    }
}
#pragma force_active on
static const u32 lit_446[2] = {
    0x00000140, 0x000001E0, 
};
static const u32 lit_447[2] = {
    0x01400280, 0x000001E0, 
};
static const u32 lit_448[2] = {
    0x00000140, 0x000001E0, 
};
static const u32 lit_449[2] = {
    0x01400280, 0x000001E0, 
};
static const f32 lit_453[1] = { 0.665f };
static const u64 lit_454[1] = {
    0x00000280000000F0ULL, 
};
static const u64 lit_455[1] = {
    0x0000028000F001E0ULL, 
};
static const u64 lit_456[1] = {
    0x00000280000000F0ULL, 
};
static const u64 lit_457[1] = {
    0x0000028000F001E0ULL, 
};
static const f32 lit_461[1] = { 2.66f };
namespace { __declspec(section ".sdata2") extern const f64 lit_489[1] = { 0.5 }; }
namespace { __declspec(section ".sdata2") extern const f64 lit_490[1] = { 3.0 }; }
namespace { __declspec(section ".sdata2") extern const f32 lit_491[1] = { 1.0f }; }
#pragma force_active off
#pragma dont_inline off
static inline f32 offCos(f32 x)
{
    return cosf(x);
}

void setCamTgtLockOFF(CameraState2 *camera)
{
    s32 *mount;
    u8 *control;
    ZoidState *zoid;
    HSD_JObj *joint;
    s32 i;
    f32 angle;
    f32 position[3];
    void *anim;
    f32 distance;
    f32 horizontal;

    if (camera != (void *)0 && (zoid = camera->zoid) != (void *)0 && zoid->joint != (void *)0
        && (control = (u8 *)zoid->field_2090, anim = zoid->anim_data, mount = (s32 *)(control + camera->mount_index * 16),
            (joint = zoid->weapon_slots[*mount].joint) != (void *)0)) {
        angle = zoid->heading;
        distance = camera->focus_distance;
        if (distance < lit_489[0])
            distance = 0.5f;
        camera->focus_heading[0] = angle;
        angLimit(&angle);
        for (i = 0; i < 3; i++)
            position[i] = joint->matrix[i][3];
        position[0] += distance * sinf(angle);
        position[2] += distance * offCos(angle);
        position[1] += 2.0f;
        horizontal = distance;
        (void)horizontal;
        for (i = 0; i < 3; i++)
            (&camera->focus.x)[i] = position[i];
    }
}
#pragma dont_inline on
void setCamTgtLockON(CameraState2 *camera)
{
    s32 *mount;
    u8 *control;
    ZoidState *zoid;
    HSD_JObj *joint;
    ZoidState *enemy;
    void *anim;
    f32 ratio;
    f32 length;
    f32 weight;
    f32 joint_weight;
    s32 i;

    if (camera != (void *)0 && (zoid = camera->zoid) != (void *)0 && zoid->joint != (void *)0
        && (control = (u8 *)zoid->field_2090, anim = zoid->anim_data, mount = (s32 *)(control + camera->mount_index * 16),
            (joint = zoid->weapon_slots[*mount].joint) != (void *)0)) {
        enemy = &Zoid[zoid->target_slot];
        if (enemy == (void *)0) {
            zoid->target_pending = 0;
            return;
        }
        ratio = camera->focus_distance / 200.0f;
        length = sLenJobj(enemy->camera_joint, joint);
        if (length * ratio < 10.0f) {
            if (length > lit_491[0])
                ratio = (f32)(10.0 / length);
        }
        weight = ratio;
        joint_weight = lit_491[0] - weight;
        for (i = 0; i < 3; i++)
            (&camera->focus.x)[i] = enemy->camera_joint->matrix[i][3] * weight + joint->matrix[i][3] * joint_weight;
    }
}
void camPara(CameraState2 *camera)
{
    ZoidState *zoid;
    CameraMountInfo *mount;
    CameraMountInfo *mounts;
    HSD_JObj *joint;
    void *anim;
    s32 i;

    if (camera == (void *)0)
        return;
    zoid = camera->zoid;
    if (zoid == (void *)0)
        return;
    mounts = (CameraMountInfo *)zoid->field_2090;
    anim = zoid->anim_data;
    mount = mounts + camera->mount_index;
    joint = zoid->weapon_slots[mount->joint_slot].joint;
    if (joint == (void *)0)
        return;
    for (i = 0; i < 3; i++)
        (&camera->relative_origin.x)[i] = (&camera->mount_positions[camera->mount_index].x)[i];
}
s32 sCameraIn2(CameraState2 *cam, HSD_JObj *jobj)
{
    f32 pp[3];
    s32 sc[2];
    s32 dead;
    s32 outcode;
    s32 *min;
    s32 *max;

    if (jobj == (void *)0)
        return 0;
    GXProject(jobj->matrix[0][3], jobj->matrix[1][3], jobj->matrix[2][3], cam->view, cam->projection, cam->viewport, &pp[0], &pp[1], &pp[2]);
    sc[0] = (s32)pp[0];
    sc[1] = (s32)pp[1];
    if (pp[2] >= 1.0)
        return 0xf;
    min = sctblMin1a;
    max = sctblMax1a;
    if (sCamNN == 2) {
        if (sCamNS == 0) {
            if (cam->half == 0) {
                min = sctblMin2a;
                max = sctblMax2a;
            } else {
                min = sctblMin2b;
                max = sctblMax2b;
            }
        } else {
            if (cam->half == 0) {
                min = sctblMin3a;
                max = sctblMax3a;
            } else {
                min = sctblMin3b;
                max = sctblMax3b;
            }
        }
    }
    dead = 0;
    if (sc[0] < min[0]) {
        outcode = 2;
        return outcode;
    }
    if (sc[0] > min[1]) {
        outcode = 8;
        return outcode;
    }
    if (sc[1] < min[2])
        return 1;
    if (sc[1] > min[3])
        return 4;
    if (sc[0] > max[0] && sc[0] < max[1] && sc[1] > max[2] && sc[1] < max[3])
        return 0x10;
    return 0;
}
s32 camZdLimit2(CameraState2 *camera, ZoidState *zoid)
{
    s32 result;
    s32 index;
    u8 *control;
    s32 *mount;
    HSD_JObj *joint;
    s32 hit;

    if (zoid == (void *)0)
        return 0;
    if (zoid->joint == (void *)0)
        return 0;
    if (camera == (void *)0)
        return 0;
    index = camera->mount_index;
    control = (u8 *)zoid->field_2090;
    mount = (s32 *)(control + index * 16);
    joint = zoid->weapon_slots[*mount].joint;
    if (camera->zoid == zoid && joint != 0 && mount[2] < 10)
        return sCameraIn2(camera, joint);
    result = 0;
    hit = sCameraIn2(camera, zoid->joint);
    result |= hit;
    hit = sCameraIn2(camera, zoid->head_base_joint);
    result |= hit;
    hit = sCameraIn2(camera, zoid->hip_base_joint);
    result |= hit;
    hit = sCameraIn2(camera, zoid->feet[2].joint);
    result |= hit;
    hit = sCameraIn2(camera, *(HSD_JObj **)zoid->tail_coobjs[0]);
    result |= hit;
    return result;
}
void camInEnemy(CameraState2 *camera, ZoidState *zoid)
{
    ZoidState *target;
    s32 i;
    Vec3 pa;
    Vec3 pb;
    Vec3 pc;
    void *anim;
    void *control;
    Vec3 pd;
    f32 len;
    f32 ang;

    if (zoid == (void *)0)
        return;
    anim = zoid->anim_data;
    control = zoid->field_2090;
    if (zoid->target_pending == 0)
        return;
    target = &Zoid[zoid->target_slot];
    if (target == 0)
        return;
    if (target->joint == 0)
        return;
    if (zoid->camera_joint == 0 || target->camera_joint == 0)
        OSReport("jobjLook=NULL\n");
    for (i = 0; i < 20; i++)
        camera->mount_hidden[i] = 0;
}
#undef lit_725
#define lit_725 "jobjLook=NULL\n"
#pragma force_active on
static const char lit_727[21] = "01234567890123456789";
static const u64 lit_1453[2] = {
    0x0000000A00000276ULL, 0x0000000A000001B8ULL, 
};
static const u64 lit_1454[2] = {
    0x0000000A00000276ULL, 0x0000000A000000E6ULL, 
};
static const u64 lit_1455[2] = {
    0x0000000A00000276ULL, 0x000000FA000001D6ULL, 
};
static const u64 lit_1456[2] = {
    0x0000008200000190ULL, 0x0000006400000168ULL, 
};
static const u64 lit_1457[2] = {
    0x0000008200000190ULL, 0x0000003C000000B4ULL, 
};
static const u64 lit_1458[2] = {
    0x0000008200000190ULL, 0x0000012C000001A4ULL, 
};
s32 sctblLen = 20;
#pragma force_active off
static char *str00 = (char *)lit_727;

#pragma dont_inline off
static inline f32 cameraSin(f32 angle)
{
    return sinf(angle);
}

static inline f32 cameraCos(f32 angle)
{
    return cosf(angle);
}

void camInMine(CamMine *camera, ZoidState *zoid)
{
    s32 k;
    s32 i;
    HSD_JObj *joint;
    CamMount *mount;
    ZoidState *target;
    s32 slot;
    CamMount *control;
    CamMount *mounts;
    f32 distance;
    f32 height;
    f32 offset;
    f32 fraction;
    f32 range;
    f32 y;
    f32 mine[3];
    f32 pos[3];
    f32 angle;
    void *anim;

    if (zoid == (void *)0)
        return;
    mounts = (CamMount *)zoid->field_2090;
    control = mounts;
    anim = zoid->anim_data;
    for (i = 0; i < 20; i++) {
        mount = &control[i];
        joint = zoid->weapon_slots[mount->slot].joint;
        if (joint == (void *)0) {
            camera->mount_blocked[i] = 1;
            continue;
        }
        if (zoid->target_pending != 0)
            angle = camera->focus_heading + (f32)mount->angle * 3.1415926 * 2.0 / 360.0;
        else
            angle = zoid->heading + (f32)mount->angle * 3.1415926 * 2.0 / 360.0;
        angLimit(&angle);
        for (k = 0; k < 3; k++) {
            camera->mount_positions[i][k] = joint->matrix[k][3];
            mine[k] = joint->matrix[k][3];
        }
        distance = (f32)mount->distance;
        height = (f32)mount->height;
        if (distance <= 5.0f && height < 50.0f)
            height = 50.0f;
        camera->mount_positions[i][0] += distance * cameraSin(angle);
        camera->mount_positions[i][2] += distance * cameraCos(angle);
        camera->mount_positions[i][1] += height - 30.0;
        if (zoid->target_pending != 0) {
            target = &Zoid[slot = zoid->target_slot];
            range = zoid->distances[slot];
            if (range != lit_377[0] && mount->distance != 0 && target->camera_joint != 0 && zoid->camera_joint != 0) {
                offset = (f32)mount->distance * ((target->camera_joint->matrix[1][3] - zoid->camera_joint->matrix[1][3]) / range);
                if (offset < -15.0f)
                    offset = -15.0f;
                camera->mount_positions[i][1] -= offset;
                {
                    GroundMask copy;
                    GroundMask mask = {{0, 255, 0, 0}};

                    copy = mask;
                    y = lit_491[0] + sColPosAndY((Vec3 *)camera->mount_positions[i], 0, &copy);
                    if (y > camera->mount_positions[i][1])
                        camera->mount_positions[i][1] = y;
                }
            }
        }
        for (k = 0; k < 3; k++)
            pos[k] = camera->mount_positions[i][k];
        {
            GroundMask copy2;
            GroundMask mask2 = {{0, 255, 0, 0}};

            copy2 = mask2;
            fraction = sColPtAll(mine, camera->mount_positions[i], &copy2);
        }
        camera->mount_blocked[i] = 10000.0f != fraction;
        if (10000.0f == fraction)
            camera->mount_blocked[i] = 0;
        else
            camera->mount_blocked[i] = 1;
        str00[i] = camera->mount_blocked[i] ? 'x' : '0' + i % 10;
        if (camera->mount_blocked[i]) {
            (void)0;
        }
    }
    for (i = 0; i < 20; i++) {
        if (camera->mount_blocked[i])
            camera->mount_times[i] = 0;
    }
}
#pragma dont_inline on
#undef lit_797
#define lit_797 "F\x1c@"
void camLimit(CameraState2 *camera)
{
    ZoidState *zoid;
    ZoidState *target;
    s32 zoid_flags;
    s32 target_flags;
    f32 middle;

    middle = sctblLen;
    if (sCamNN == 2) {
        (void)0;
    }
    if (sCamNN == 2 && sCamNS == 0) {
        middle = sctblLen / 2;
    }
    if (camera != (void *)0 && (zoid = camera->zoid, zoid != (void *)0) && zoid->joint != (void *)0) {
        zoid_flags = camZdLimit2(camera, zoid);
        if (zoid->target_pending != 0) {
            target = &Zoid[zoid->target_slot];
            if (target != 0 && target->joint == (void *)0) {
                target = 0;
            }
            target_flags = camZdLimit2(camera, target);
        } else {
            target_flags = 0x10;
        }
        if (camera->field_1E4 <= 0x3C) {
            if (target_flags & 0xF) {
                camera->focus_distance += lit_489[0];
            } else if (zoid_flags & 0xF) {
                camera->field_1BC = 0x3C;
                camera->focus_distance -= 0.2;
            } else {
                if (camera->focus_distance > 100.0f) {
                    camera->focus_distance -= 0.02;
                }
                if (camera->focus_distance < 100.0f) {
                    camera->focus_distance += 0.02;
                }
            }
            if (camera->focus_distance <= lit_377[0]) {
                camera->focus_distance = lit_377[0];
            }
            if (camera->focus_distance >= 200.0f) {
                camera->focus_distance = 200.0f;
            }
        }
    }
}

s32 chkPos(f32 *a, f32 *b)
{
    s32 i;
    f32 diff;
    for (i = 0; i < 3; i++) {
        diff = a[i] - b[i];
        if (i == 1)
            diff = diff / 10.0f;
        if ((s32)diff != 0)
            return 0;
    }
    return 1;
}
void camSameTime(CameraState2 *camera)
{
    s32 slot;
    ZoidState *zoid;
    ZoidState *enemy;
    s32 i;

    if (camera != 0 && (zoid = camera->zoid) != 0 && zoid->joint != 0) {
        camera->field_1E4++;
        if (chkPos(camera->prev_zoid_pos, zoid->position) == 0)
            camera->field_1E4 = 0;
        slot = zoid->target_slot;
        if (slot < 0 || slot >= 16)
            zoid->target_pending = 0;
        enemy = 0;
        if (zoid->target_pending != 0) {
            enemy = &Zoid[slot];
            if (enemy->joint == 0)
                enemy = 0;
        }
        if (enemy != 0) {
            if (chkPos(camera->prev_target_pos, enemy->position) == 0)
                camera->field_1E4 = 0;
        }
        for (i = 0; i < 3; i++) {
            camera->prev_zoid_pos[i] = zoid->position[i];
            if (enemy != 0)
                camera->prev_target_pos[i] = enemy->position[i];
        }
    }
}
void sCameraLenZoid(CameraState2 *camera)
{
    s32 i;
    s32 axis;

    for (i = 0; i < 16; i++) {
        ZoidState *zoid = &Zoid[i];
        camera->zoid_distance[i] = 1000000000.0F;
        if (zoid->joint != (void *)0) {
            Vec3 delta;
            for (axis = 0; axis < 3; axis++)
                ((f32 *)&delta)[axis] = camera->eye[axis] - zoid->camera_joint->matrix[axis][3];
            camera->zoid_distance[i] = PSVECMag(&delta);
        }
    }
}
s32 linierCamLimitXZ(CameraState2 *camera, f32 radius)
{
    Vec3 *interest = (Vec3 *)camera->interest;
    Vec3 *focus = &camera->focus;
    Vec3 *eye = (Vec3 *)camera->eye;
    Vec3 *origin = &camera->relative_origin;
    f32 limit = radius * radius;
    f32 dist;
    f32 x, y, z;
    z = origin->z - eye->z;
    x = origin->x - eye->x;
    y = origin->y - eye->y;
    dist = x * x + z * z + y * y;
    if (dist > limit)
        return 1;
    z = focus->z - interest->z;
    x = focus->x - interest->x;
    y = focus->y - interest->y;
    dist = x * x + z * z + y * y;
    if (dist > limit / 2.0F / 2.0F)
        return 1;
    return 0;
}
void linierCamXZ(CameraState2 *camera, s32 snap)
{
    s32 i;
    f32 eye_rate = 0.3f;
    f32 interest_rate = 0.35f;

    if (snap == 0) {
        snap *= 0.1;
    }
    for (i = 0; i < 3; i++) {
        camera->interest[i] = sLiner((&camera->focus.x)[i], camera->interest[i], interest_rate);
        camera->eye[i] = sLiner((&camera->relative_origin.x)[i], camera->eye[i], eye_rate);
    }
}
#pragma dont_inline off
s32 sCamBallOut(CameraState2 *camera, f32 radius)
{
    f32 length;
    f32 delta[3];
    s32 axis;

    for (axis = 0; axis < 3; axis++)
        delta[axis] = (&camera->focus.x)[axis] - (&camera->relative_origin.x)[axis];
    length = calcSqrt(delta[0] * delta[0] + delta[2] * delta[2]);
    if (length > radius)
        return;
    if (lit_377[0] != length) {
        for (axis = 0; axis < 3; axis++)
            (&camera->focus.x)[axis] = (&camera->relative_origin.x)[axis] + radius * delta[axis] / length;
    }
}
#pragma dont_inline on

void camCockpit(CameraState2 *camera, ZoidState *zoid)
{
    HSD_JObj *joint;
    s32 axis;

    if (camera == (void *)0)
        return;
    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    joint = zoid->weapon_slots[19].joint;
    if (joint == 0)
        return;
    for (axis = 0; axis < 3; axis++) {
        f32 translation = joint->matrix[axis][3];
        camera->eye[axis] = translation;
        camera->interest[axis] = translation + 10.0F * joint->matrix[axis][0];
    }
}

void searchCamBetterOff(CameraState2 *camera)
{
    s32 i;

    for (i = 0; i < 20; i++) {
        if (camera->mount_times[i] >= 60) {
            if (!camera->mount_blocked[i])
                break;
        }
    }
    if (i == 20) {
        for (i = 0; i < 20; i++) {
            if (!camera->mount_blocked[i])
                break;
        }
    }
    if (i == 20)
        i = 0;
    if (camera->mount_index != i) {
        camera->mount_index = i;
        camera->field_1E4 = 0;
        camera->mount_timer = 0;
        camera->focus_distance = 10.0F;
        camera->field_1BC = 60;
    }
}
s32 searchCamBetterOnAtk(CameraState2 *camera, ZoidState *zoid, s32 slot)
{
    s32 mount;
    s32 flag;
    s32 index;
    f32 angle;
    f32 magnitude;

    mount = camera->mount_index;
    flag = 0;
    if (mount == 0x12 || mount == 0x13)
        flag = 1;
    if (zoid == 0)
        return 0x14;
    if (zoid->joint == 0)
        return 0x14;
    if (zoid->field_1DA0 == -1)
        return 0x14;
    if (zoid->field_1DA0 == 5)
        return 0x14;
    if (zoid->jump_phase != 0)
        return 0x14;
    if (zoid->action == 0)
        return 0x14;
    if (zoid->field_1DD4 != 0)
        return 0x14;
    if (slot == -1)
        return 0x14;
    if (zoid->distances[slot] > 60.0f)
        return 0x14;
    if (camera->mount_blocked[mount] != 0)
        return 0x14;
    if (flag != 0)
        return mount;
    if (zoid->attack_target != slot)
        return 0x14;
    angle = zoid->target_angles[slot];
    magnitude = angle;
    *(u32 *)&magnitude &= 0x7FFFFFFF;
    if (magnitude > 1.2217304555555555)
        return 0x14;
    if (angle >= lit_377[0])
        index = 0x12;
    else
        index = 0x13;
    if (camera->mount_blocked[index] != 0 || camera->mount_hidden[index] != 0)
        return 0x14;
    camera->mount_index = 0;
    return index;
}
void searchCamBetterOn(CameraState2 *camera)
{
    s32 dead;
    s32 slot;
    s32 i;
    s32 j;
    ZoidState *zoid;

    dead = 0;
    slot = -1;
    zoid = camera->zoid;
    if (zoid != 0)
        slot = zoid->target_slot;
    i = 20;
    dead = 0;
    j = searchCamBetterOnAtk(camera, zoid, slot);
    if (j != 20)
        i = j;
    if (i == 20) {
        for (i = 0, j = 10; i < 20; i++, j++) {
            if (j >= 20)
                j = 0;
            if (camera->mount_times[j] >= 60 && !camera->mount_blocked[j] && !camera->mount_hidden[j])
                break;
        }
    }
    if (i == 20) {
        for (i = 0, j = 10; i < 20; i++, j++) {
            if (j >= 20)
                j = 0;
            if (camera->mount_times[j] >= 60 && !camera->mount_blocked[j])
                break;
        }
    }
    if (i == 20) {
        for (i = 0, j = 10; i < 20; i++, j++) {
            if (j >= 20)
                j = 0;
            if (camera->mount_times[j] >= 60 && !camera->mount_hidden[j])
                break;
        }
    }
    if (i == 20) {
        for (i = 0, j = 10; i < 20; i++, j++) {
            if (j >= 20)
                j = 0;
            if (!camera->mount_blocked[j])
                break;
        }
    }
    if (i == 20)
        j = 10;
    if (camera->mount_index != j) {
        camera->mount_index = j;
        camera->field_1E4 = 0;
        camera->mount_timer = 0;
        camera->focus_distance = 45.0F;
        if (slot != -1) {
            if (camera->zoid_distance[slot] > 200.0f)
                camera->focus_distance = 10.0f;
        }
        camera->field_1BC = 60;
    }
}

void sCameraON(CameraState2 *camera)
{
    s32 linierCamXZ_mode;

    s32 flag;
    s32 i;
    void *table;
    ZoidState *zoid;
    s32 pad1;
    u8 *entry;
    s32 pad2;
    s32 mount;
    GroundMask mask = {{0, 255, 0, 0}};

    if (camera != (void *)0 && camera->cobj != (void *)0 && (zoid = camera->zoid, zoid != (void *)0)) {
        table = zoid->field_2090;
        if (table != 0)
            entry = (u8 *)table + camera->mount_index * 16;
        else
            entry = 0;
        flag = 0;
        for (i = 0; i < 20; i++) {
            camera->mount_times[i]++;
            if (camera->mount_times[i] > 36000)
                camera->mount_times[i] = 36000;
        }
        camSameTime(camera);
        camInMine((CamMine *)camera, zoid);
        camInEnemy(camera, zoid);
        camera->mount_timer++;
        mount = camera->mount_index;
        if (mount == 20)
            mount = 0;
        if (zoid->target_pending != 0) {
            if (camera->mount_blocked[mount] != 0 || camera->mount_hidden[mount] != 0)
                flag = 1;
        } else {
            if (camera->mount_blocked[mount] != 0)
                flag = 1;
        }
        if (camera->mount_timer > 120)
            flag = 1;
        if (camera->mount_timer > 60 && entry != 0 && *(s32 *)(entry + 8) <= 5)
            flag = 1;
        if (camera->mount_timer > 60 && zoid->target_pending != 0 && camera->mount_index < 10)
            flag = 1;
        if (flag != 0) {
            if (zoid->target_pending != 0)
                searchCamBetterOn(camera);
            else
                searchCamBetterOff(camera);
        }
        if ((GameMode[1] == 2 || GameMode[1] == 3) && GameMode[2] == 1)
            camera->mount_index = 13;
        if (zoid->target_pending != 0) {
            setCamTgtLockON(camera);
            sCameraCalcET(camera);
        } else {
            setCamTgtLockOFF(camera);
        }
        angLimit(camera->focus_heading);
        camPara(camera);
        camLimit(camera);
        if (camera->field_1BC != 0)
            camera->field_1BC--;
        pad1 = 3;
        pad2 = 3;
        sCamBallOut(camera, 10.0f);
        if (linierCamLimitXZ(camera, 5.0f) != 0)
            camera->field_1BC = 180;
        if (zoid->current_animation == 5)
            camera->field_1BC = 180;
        if (zoid->current_animation == 8)
            camera->field_1BC = 180;
        if (camera->field_1BC != 0) {
            linierCamXZ_mode = 1;
            linierCamXZ(camera, linierCamXZ_mode);
        }
        if (mount == 20)
            camCockpit(camera, camera->zoid);
        if (60.0f != HSD_CObjGetFov(camera->cobj))
            HSD_CObjSetFov(camera->cobj, 60.0f);
        HSD_CObjSetInterest(camera->cobj, (Vec3 *)camera->interest);
        HSD_CObjSetEyePosition(camera->cobj, (Vec3 *)camera->eye);
        sCameraCalc(camera);
        for (i = 0; i < 3; i++) {
            camera->saved_eye[i] = camera->eye[i];
            camera->saved_interest[i] = camera->interest[i];
        }
    }
}
void sCameraSetShadow(CameraState2 *camera)
{
    HSD_LObj *light;
    HSD_LObj *next;

    if (camera != (void *)0 && camera->cobj != (void *)0) {
        light = camera->lights;
        while (light != 0) {
            if ((light->flags & LOBJ_TYPE_MASK) != LOBJ_AMBIENT) {
                camera->shadow = makeShadow(light);
                HSD_ShadowAddObject(camera->shadow, camera->zoid->joint);
                HSD_ShadowSetActive(camera->shadow, 0);
                break;
            }
            if (light == (void *)0)
                next = 0;
            else
                next = light->next;
            light = next;
        }
    }
}
void sCameraSetShadowAll(void)
{
    s32 i;
    for (i = 0; i < 4; i++)
        sCameraSetShadow(&sCam[i]);
}
void camDispZoid(CameraState2 *camera, ZoidState *zoid, u8 *control)
{
}
void zdSuperAtackAng(ZoidState *zoid, CameraState2 *camera)
{
    ZoidControlData *data;
    f32 angle;
    s32 kind;
    f32 limit;
    s32 flag;
    s32 mount;
    s32 state;

    flag = 0;
    limit = 0.3490658402442932f;
    if (zoid == 0)
        return;
    if (zoid->joint == 0)
        return;
    data = (ZoidControlData *)zoid->anim_data;
    if (data == 0)
        return;
    mount = zoid->weapon_mount;
    if (mount == -1) {
        kind = 0;
        state = 0;
    } else {
        state = zoid->weapon_mounts[mount].state;
        kind = data->field_0140[mount];
    }
    if (zoid->animation_frame == 0)
        kind = 0;
    if (mount == -1)
        kind = 0;
    if (kind == 2) {
        if (state == 2 || state == 3 || state == 1 || state == 6)
            flag = 1;
    }
    if (mount == -1)
        flag = 0;
    if (flag != 0) {
        angle = -zoid->field_1C74 * (2.0 * (3.1415926 * (f32)(*(s32 *)((u8 *)data + 0xDC) / 1000.0)) / 360.0);
        if (camera->scope_on != 0)
            angle = angle * camera->fov / 60.0;
        angle = angle + zoid->heading;
        angLimit(&angle);
        zoid->heading = angle;
    }
    if (flag != 0) {
        angle = -zoid->field_1C78 * (2.0 * (3.1415926 * (f32)(*(s32 *)((u8 *)data + 0xDC) / 1000.0)) / 360.0);
        if (camera->scope_on != 0)
            angle = angle * camera->fov / 60.0;
        angle = angle + zoid->high_angle;
    } else {
        angle = zoid->high_angle;
        if (angle < lit_377[0]) {
            angle += 0.026179938333333336;
            if (angle > lit_377[0])
                angle = lit_377[0];
        }
        if (angle > lit_377[0]) {
            angle = angle - 0.026179938333333336;
            if (angle < lit_377[0])
                angle = lit_377[0];
        }
    }
    angLimit(&angle);
    if (angle > limit)
        angle = limit;
    if (angle < -limit)
        angle = -limit;
    zoid->high_angle = angle;
}
s32 zdSatkOk(ZoidState *zoid)
{
    s32 anim;
    s32 mode;

    if (zoid == 0)
        return 0;
    if (zoid->joint == 0)
        return 0;
    if (zoid->weapon_mount == 0x13 && *(s32 *)((u8 *)zoid + 0x1738) == 2)
        return 1;
    mode = zoid->action;
    switch (mode) {
    case 1:
    case 2:
    case 23:
    case 24:
    case 25:
    case 26:
        return 0;
    }
    anim = zoid->current_animation;
    switch (anim) {
    case 4:
    case 5:
    case 8:
        return 0;
    }
    if (zoid->field_1C90[2] > 0.1 || zoid->field_1C90[0] > 0.1)
        return 0;
    if (zoid->action != 0)
        return 0;
    if (zoid->field_1DAC != 0)
        return 0;
    if (zoid->field_1DB0 <= 0xF)
        return 0;
    if (zoid->field_1DD4 != 0)
        return 0;
    return 1;
}
#pragma dont_inline off
static inline f32 absf(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

static inline f32 camSqrt(f32 value)
{
    if (value > lit_377[0]) {
        f64 estimate = __frsqrte(value);
        volatile f32 result;

        estimate = lit_489[0] * estimate * (lit_490[0] - estimate * estimate * value);
        estimate = lit_489[0] * estimate * (lit_490[0] - estimate * estimate * value);
        estimate = lit_489[0] * estimate * (lit_490[0] - estimate * estimate * value);
        estimate = lit_489[0] * estimate * (lit_490[0] - estimate * estimate * value);
        result = value * estimate;
        return result;
    }
    return value;
}

void sCameraCtrl(CameraState2 *camera)
{
    s32 i;
    s32 old;
    AiState2 *ai;
    u8 *control;
    s32 command;
    ZoidState *zoid;
    s32 index;
    s32 roll;
    s32 saved;
    s8 held;
    f32 angle;
    s8 started;
    s32 mode;
    s32 old_scope;
    f32 delta;
    s32 person;
    void *scope;
    s32 mount;
    PadState2 *pad;
    ZoidState *partner;
    f32 scale;
    f32 x;
    f32 y;
    f32 length;
    f32 yaw;
    f32 previous;
    f32 magnitude;
    f32 sx;
    f32 sy;

    if (camera != (void *)0 && camera->cobj != (void *)0 && (pad = camera->pad) != (void *)0) {
        zoid = camera->zoid;
        control = 0;
        if (zoid != 0) {
            control = (u8 *)zoid->anim_data;
        }
        if (sCamMuteki != 0) {
            zoid->health = zoid->max_health;
            gameTime = -1;
        }
        if (sCamMuteki == 2) {
            for (i = 0; i < 16; i++) {
                if (Zoid[i].joint != (void *)0) {
                    Zoid[i].health = Zoid[i].max_health;
                }
            }
        }
        scale = (f32)padBy / 100.0;
        if (zoid->booster_on != 0) {
            scale = 4.0 * ((f32)padBy / 100.0);
        }
        index = camera->target_index;
        saved = camera->field_1B4;
        sCameraTargetZoid(camera, &Zoid[index]);
        if (zoid != 0) {
            pad->rumble = zoid->rumble_timer != 0;
        } else {
            pad->rumble = 0;
        }
        if (rumble == 0) {
            pad->rumble = 0;
        }
        if (nowStage == 16 && vsyncN % 30 == 0) {
            sEffectSet(0x6A, 1, 0, zoid->joint, zoid);
        }
        camDispZoid(camera, zoid, control);
        camera->field_1B4 = saved;
        zoid->super_attack_input = pad->held_42 != 0 && pad->held_45 != 0;
        if ((s8)zoid->option_use_state != 0) {
            zoid->super_attack_input = 0;
        }
        if (zoid->death_started != 0) {
            zoid->super_attack_input = 0;
        }
        if (zdSatkOk(zoid) == 0) {
            zoid->super_attack_input = 0;
        }
        if (pad->pressed_5e != 0) {
            camera->zoid->requested_action = 2;
        }
        if (pad->pressed_61 != 0) {
            zdWeaponChg(zoid);
        }
        if (zoid->super_attack_input == 0) {
            if (pad->held_42 != 0 && zoid->field_1DA0 == -1 && zoid->weapon_mount == zoid->selected_weapon_mount) {
                zdWeaponFireON(zoid);
            } else {
                zdWeaponFireOFF(zoid);
            }
            if (pad->pressed_60 != 0) {
                zdAtack1(zoid);
            }
        }
        if (camera->scope_on != 0) {
            if (pad->held_40 != 0) {
                camera->fov += 0.3;
            }
            if (pad->held_41 != 0) {
                camera->fov -= 0.3;
            }
            zoid->target_pending = 0;
            zoid->requested_action = 0;
        } else {
            if (pad->pressed_5c != 0) {
                ZoidSearchNextTarget(camera->zoid);
            }
            if (pad->pressed_5b != 0 && camera->zoid != 0) {
                camera->zoid->target_pending = 0;
            }
        }
        if (camera->fov > 45.0f) {
            camera->fov = 45.0f;
        }
        if (camera->fov < lit_491[0]) {
            camera->fov = lit_491[0];
        }
        zoid->option_use_state = pad->held_47;
        if (zoid->death_started != 0) {
            zoid->option_use_state = 0;
        }
        sy = pad->substick_y;
        sx = pad->substick_x;
        magnitude = sx * sx + sy * sy;
        held = camera->substick_held;
        if (magnitude < 0.1) {
            held = 0;
        }
        if (magnitude > 0.3) {
            held = 1;
        }
        started = held != 0 && camera->substick_held == 0;
        camera->substick_held = held;
        if (started != 0) {
            angle = atan2f(-sx, sy);
            if (control != 0 && *(s32 *)(control + 0xF8) != 3) {
                zdStep0f(zoid, angle + camera->yaw - zoid->heading);
            } else if (absf(sx) > absf(sy)) {
                zdStep0f(zoid, angle + camera->yaw - zoid->heading);
            }
        }
        if (control != 0 && *(s32 *)(control + 0xF8) == 3) {
            if (absf(sx) < absf(sy)) {
                zoid->high_angle = sy;
            }
        }
        if (zoid != 0 && zoid->index == 0 && Zoid[2].joint != 0 && zoid->slot == Zoid[2].slot) {
            partner = &Zoid[2];
            ai = (AiState2 *)partner->ai;
            if (partner->death_started != 0) {
                ai = 0;
            }
            if (ai != 0 && mgI[0x3218] == 0) {
                old = ai->previous_command;
                dspPartnerMsgTm2++;
                if (dspPartnerMsgF == 0) {
                    ai->previous_command = 0;
                }
                if (dspPartnerMsgTm == 0) {
                    ai->command = 0;
                    command = -1;
                    if (pad->pressed_51 != 0) {
                        command = 3;
                    }
                    if (pad->pressed_52 != 0) {
                        command = 4;
                    }
                    if (pad->pressed_51 != 0) {
                        ai->command = 3;
                    }
                    if (pad->pressed_52 != 0) {
                        ai->command = 4;
                    }
                    if (pad->pressed_4f != 0) {
                        if (zoid->target_pending != 0) {
                            command = 1;
                        }
                        if (zoid->target_pending != 0) {
                            ai->command = 1;
                        }
                    }
                    if (ai->command == 1 && zoid->target_pending == 0) {
                        command = 0;
                    }
                    if (ai->command == 1 && zoid->target_pending == 0) {
                        ai->command = 0;
                    }
                    if (pad->pressed_50 != 0) {
                        command = 2;
                    }
                    if (pad->pressed_50 != 0) {
                        ai->command = 2;
                    }
                    if (zoid->death_started != 0) {
                        ai->command = old;
                    }
                    if (old != ai->command && ai->command != 0) {
                        person = partner->person;
                        if (dspPartnerMsgF == 0) {
                            roll = HSD_Randi(0x65);
                            if (roll <= Tea_persondat[person].field_38) {
                                dspPartnerMsgTm2 = 0;
                                PartnerFaceV(5, 60, person, 11);
                            } else {
                                ai->command = old;
                                PartnerFaceV(4, 60, person, 10);
                            }
                            dspPartnerMsgF = 0x258;
                        } else {
                            ai->command = old;
                            PartnerFaceV(4, 60, person, 10);
                        }
                    }
                    if (ai->command != 0) {
                        ai->previous_command = ai->command;
                    }
                    if (command != -1) {
                        dspOrderAI = command;
                        dspOrderTm = 0x78;
                    }
                    if (dspPartnerMsgTm2 > 0x258 && ai->command == 0 && dspOrderAI > 0) {
                        dspPartnerMsgTm2 = 0;
                        ai->command = dspOrderAI;
                        if (ai->command == 1 && zoid->target_pending == 0) {
                            ai->command = 0;
                        }
                    }
                }
            }
        }
        if (control != 0) {
            mount = zoid->weapon_mount;
            old_scope = camera->scope_on;
            mode = *(s32 *)(control + 0x140 + mount * 4);
            camera->scope_on = zoid->animation_frame == 2;
            scope = zdWeaponScope(zoid, mount);
            if (scope == 0) {
                scope = zoid->field_1BEC;
            }
            if (scope == 0) {
                camera->scope_on = 0;
            }
            if (zoid->field_1364 != 0) {
                camera->scope_on = 0;
            }
            if (mount != zoid->selected_weapon_mount) {
                camera->scope_on = 0;
            } else if (zoid->weapon_mounts[mount].state <= 1) {
                camera->scope_on = 0;
            }
            camera->scope = scope;
            if (camera->scope_on != 0 && old_scope == 0) {
                camera->fov = 45.0f;
            }
        }
        y = pad->stick_y * scale;
        x = pad->stick_x * scale;
        length = camSqrt(x * x + y * y);
        if (length < 0.16) {
            zdSuperAtackAng(zoid, camera);
            if (((AiState2 *)zoid->ai)->field_40 != 0) {
                return;
            }
            zoid->field_1C54 = lit_377[0];
            zoid->field_1C74 = lit_377[0];
            zoid->field_1C78 = lit_377[0];
            return;
        }
        zoid->field_1C54 = length;
        angle = atan2f(-x, y);
        previous = zoid->stick_angle;
        angLimit(&angle);
        yaw = camera->yaw;
        zoid->step_heading = angle + yaw;
        zoid->stick_angle = angle;
        zoid->field_1C74 = x;
        zoid->field_1C78 = y;
        angLimit(&zoid->step_heading);
        delta = angle - previous;
        angLimit(&delta);
        if (lit_377[0] == length) {
            zoid->stick_frames = lit_377[0];
        } else {
            zoid->stick_frames += lit_491[0];
        }
        if (lit_377[0] == length) {
            zoid->stick_steady_frames = lit_377[0];
        } else if (absf(delta) > DEG2RAD_D(10.0)) {
            zoid->stick_steady_frames = lit_377[0];
        } else {
            zoid->stick_steady_frames += lit_491[0];
        }
        zdSuperAtackAng(zoid, camera);
    }
}
#pragma dont_inline on
void sCameraMatrix(CameraState2 *camera)
{
    f32 (*inverse)[4];

    GXGetProjectionv(camera->projection);
    GXGetViewportv(camera->viewport);
    HSD_CObjGetViewingMtx(((CameraState2 *)camTmp)->cobj, camera->view);
    inverse = HSD_CObjGetInvViewingMtxPtr(((CameraState2 *)camTmp)->cobj);
    PSMTXCopy(inverse, camera->inverse_view);
}

s32 sCameraPt2d(CameraState2 *camera, Vec3 *position, s32 *screen_point)
{
    f32 depth;
    ScreenPoint screen;
    GXProject(position->x, position->y, position->z,
              camera->view, camera->projection, camera->viewport,
              &screen.x, &screen.y, &depth);
    screen_point[0] = screen.x;
    screen_point[1] = screen.y;
    if (depth < 1.0)
        return 1;
    return 0;
}
s32 sCameraJobjPt2d(CameraState2 *camera, HSD_JObj *joint, s32 *screen_point)
{
    f32 depth;
    ScreenPoint screen;
    if (joint == 0)
        return 0;
    GXProject(joint->matrix[0][3], joint->matrix[1][3], joint->matrix[2][3],
              camera->view, camera->projection, camera->viewport,
              &screen.x, &screen.y, &depth);
    screen_point[0] = screen.x;
    screen_point[1] = screen.y;
    if (depth < 1.0)
        return 1;
    return 0;
}
void sCameraShadowAll(void)
{
    s32 i;

    if (nowShadow == 0) {
        for (i = 0; i < 4; i++) {
            camTmp = &sCam[i];
            if (sCam[i].cobj != (void *)0 && sCam[i].lights != (void *)0 && sCam[i].shadow != (void *)0)
                shadowRendering(sCam[i].shadow, sCam[i].target_index);
        }
    }
}
void sCameraPurge(void)
{
    s32 i;
    CameraState2 *camera;
    for (i = 0; i < 4; i++) {
        camera = &sCam[i];
        sCameraReset(camera);
    }
}

void IntSePrologue(void)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        IntSe[i].se = 0;
    }
    vcSeekStr[0] = 0;
    vcSeekNow[0] = 0;
    vcSeekMode = 0;
    ZeroMemory(vcSeekIfo, 60);
}

s32 camVoiceSeekCB(void *arg)
{
    s32 a;
    s32 x;
    s32 b;
    if (vcSeekMode == 1)
        vcSeekMode = 2;
    x = (s32)arg;
    return 0;
}

s32 camVoiceSeekFile(char *name)
{
    s32 entry;

    sReadPre();
    HSD_AudioStreamVolume(0);
    HSD_AudioStreamStop(0);
    vcSeekNow[0] = 0;
    entry = DVDConvertPathToEntrynum(name);
    if (entry == -1)
        return 0;
    if (DVDFastOpen(entry, vcSeekIfo) == 0)
        return 0;
    vcSeekMode = 1;
    if (DVDReadAsyncPrio(vcSeekIfo, &rrrbuf, 0x100, 0, (void *)camVoiceSeekCB, 0) == 0) {
        DVDClose(vcSeekIfo);
        vcSeekMode = 0;
        return 0;
    }
    strcpy(vcSeekNow, name);
    return 1;
}

void camVoiceClose(void)
{
    sReadPre();
    HSD_AudioStreamVolume(0);
    HSD_AudioStreamStop(0);
    vcSeekStr[0] = 0;
    if (vcSeekMode != 0) {
        DVDClose(vcSeekIfo);
        vcSeekMode = 0;
    }
}

void camVoiceSeekInt(void)
{
    switch (vcSeekMode) {
    case 0:
        if (vcSeekStr[0] != 0)
            camVoiceSeekFile(vcSeekStr);
        break;
    case 1:
        break;
    case 2:
        DVDClose(vcSeekIfo);
        if (StrCmp(vcSeekStr, vcSeekNow) != 0) {
            sReadPre();
            TEA_VoiceFilePlay(vcSeekNow, TEA_VOICEVOL, 0);
            vcSeekStr[0] = 0;
        }
        vcSeekNow[0] = 0;
        vcSeekMode = 3;
        vcSeekMode2 = 15;
        break;
    case 3:
        if (vcSeekMode2 != 0)
            vcSeekMode2--;
        else
            vcSeekMode = 0;
        break;
    default:
        vcSeekMode = 0;
        break;
    }
}
s32 camVoiceSeek(const char *name)
{
    strcpy(vcSeekStr, name);
    return 0;
}
void camVoiceOn(s32 a, s32 b)
{
    char buf[256];

    sprintf(buf, "voice/%02d/V%02d_%02d.adp", a, a, b);
    camVoiceSeek(buf);
}
#undef lit_1542
#define lit_1542 "voice/%02d/V%02d_%02d.adp"
#pragma force_active on
static const char lit_1728[9] = "\x92" "e\x83\x89\x83" "C\x83g";
static const char lit_1729[17] = "\x83G\x83t\x83" "F\x83N\x83g(\x82\xdc\x82\xbe)";
#pragma force_active off
void camVoiceNow(s32 a, s32 b)
{
    char buf[256];

    sprintf(buf, lit_1542, a, a, b);
    sReadPre();
    TEA_VoiceFilePlay(buf, TEA_VOICEVOL, 0);
}

void IntSeClose(void)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        if (IntSe[i].se != 0 && IntSe[i].delay != -1) {
            IntSe[i].delay = -1;
            IntSe[i].timer = 0;
            IntSe[i].se = 0;
        }
    }
    camVoiceClose();
}
void IntSeClear(void)
{
    s32 i;
    s32 flag;
    u32 handle;

    flag = 0;
    camVoiceSeekInt();
    if (vsyncN & 1)
        flag = 1;
    for (i = 0; i < 32; i++) {
        if (IntSe[i].se != 0 && IntSe[i].delay != -1) {
            if (IntSe[i].delay == 0) {
                if (flag == 0) {
                    IntSe[i].delay = -1;
                    handle = TEA_SeOnF((u16)IntSe[i].se, IntSe[i].x, IntSe[i].z);
                    IntSe[i].handle = handle;
                    flag = 1;
                }
            } else {
                IntSe[i].delay--;
            }
        }
    }
    for (i = 0; i < 32; i++) {
        if (IntSe[i].se != 0 && IntSe[i].delay == -1) {
            if (IntSe[i].timer != 0)
                IntSe[i].timer--;
            if (IntSe[i].timer == 0 && (vsyncN & 3) == 0) {
                handle = HSD_AudioSFXCheck(IntSe[i].handle);
                if (handle == -1) {
                    IntSe[i].timer = 0;
                    IntSe[i].se = 0;
                }
            }
        }
    }
}
s32 IntSeSetTm(s16 se, f32 distance, f32 x, f32 z, s32 delay)
{
    s32 i;
    s32 dead;

    if (delay == -1)
        delay = 0;
    for (i = 0; i < 32; i++) {
        if (IntSe[i].se != 0 && se == IntSe[i].se && delay == IntSe[i].delay && IntSe[i].distance <= 1.3 * distance)
            return 0;
    }
    for (i = 0; i < 32; i++) {
        if (IntSe[i].se == 0) {
            IntSe[i].distance = distance;
            IntSe[i].se = se;
            IntSe[i].x = x;
            IntSe[i].z = z;
            IntSe[i].timer = 15;
            IntSe[i].delay = delay;
            IntSe[i].handle = -1;
            dead = -1;
            return dead;
        }
    }
    dead = dead;
    return 0;
}
s32 camSeTm(CameraState2 *camera, s16 sound, Vec3 *point, s32 volume)
{
    Vec3 relative;
    f32 distance;
    s16 id;

    if (sound <= 0)
        return 0;
    if (GameMode[1] != 2) {
        sCameraRela(camera, point, &relative);
        distance = relative.x * relative.x + relative.z * relative.z;
        if (distance > 90000.0F)
            return 0;
    } else {
        relative.x = lit_377[0];
        relative.z = 100.0F;
        distance = 100.0F;
    }
    id = SE_TableList[sound];
    if (id < 0)
        return 0;
    return IntSeSetTm(id, distance, relative.x, -relative.z, volume);
}
#undef lit_1623
#define lit_1623 "G\xaf\xc8"
s32 camSe(CameraState2 *camera, s16 sound, Vec3 *point)
{
    return camSeTm(camera, sound, point, -1);
}
s32 camSeTmJobj(CameraState2 *camera, s16 sound, HSD_JObj *joint, s32 time)
{
    f32 point[3];
    s32 i;
    if (joint == 0)
        return 0;
    for (i = 0; i < 3; i++)
        point[i] = joint->matrix[i][3];
    return camSeTm(camera, sound, (Vec3 *)point, time);
}
void camOnScope(CameraState2 *camera)
{
    ZoidState *zoid;
    HSD_JObj *joint;
    f32 (*matrix)[4];
    f32 eye[3];
    f32 interest[3];
    s32 i;

    zoid = camera->zoid;
    if (zoid == 0)
        return;
    if (zoid->joint == 0)
        return;
    joint = (HSD_JObj *)camera->scope;
    if (joint == 0)
        return;
    matrix = joint->matrix;
    for (i = 0; i < 3; i++) {
        eye[i] = matrix[i][3];
        interest[i] = matrix[i][3] + 10.0F * matrix[i][0];
    }
    for (i = 0; i < 3; i++) {
        camera->interest[i] = interest[i];
        camera->eye[i] = eye[i];
    }
    HSD_CObjSetFov(camera->cobj, camera->fov);
    HSD_CObjSetInterest(camera->cobj, (Vec3 *)camera->interest);
    HSD_CObjSetEyePosition(camera->cobj, (Vec3 *)camera->eye);
    sCameraCalc(camera);
}
typedef struct StartEntry {
    s32 index;
    s32 field_04;
    s32 radius;
    s32 height;
} StartEntry;

extern s32 gameStartTm;

#pragma dont_inline off
static inline GroundMask startMaskA(void)
{
    return (GroundMask){{3, 0, 0, 0}};
}

static inline GroundMask startMaskB(void)
{
    return (GroundMask){{0, 255, 0, 0}};
}

static inline f32 startPt(f32 *eye, f32 *interest)
{
    GroundMask maskB;
    maskB = startMaskB();
    return sColPt((Vec3 *)eye, (Vec3 *)interest, &maskB);
}

void camOnStart(CameraState2 *camera)
{
    s32 idx;
    s32 i;
    s32 moved;
    StartEntry *entry;
    HSD_JObj *joint;
    StartEntry *table;
    s32 index;
    s32 radius;
    s32 height;
    f32 (*m)[4];
    f32 angle;
    void *data;
    f32 floor_y;
    GroundMask maskA;
    f32 sine_angle;
    f32 cosine_angle;
    ZoidState *zoid;

    idx = 0;
    if (camera != 0 && camera->cobj != 0 && (zoid = camera->zoid, zoid != 0) && zoid->joint != 0
        && (table = (StartEntry *)zoid->field_2090, data = zoid->anim_data, table != 0)) {
        if (zoid->action != 50) {
            zoid->requested_action = 50;
            zoid->action = 50;
        }
        entry = &table[idx];
        index = entry->index;
        radius = entry->radius;
        height = entry->height - 30;
        joint = zoid->weapon_slots[index].joint;
        if (joint == (void *)0)
            joint = zoid->joint;
        m = (f32 (*)[4])((u8 *)joint + 0x44);
        angle = 3.1415926 + zoid->heading;
        angle += 2.0 * (3.1415926 * (f32)(360.0 * gameStartTm / 300.0)) / 360.0;
        angLimit(&angle);
        for (i = 0; i < 3; i++) {
            camera->eye[i] = m[i][3];
            camera->interest[i] = m[i][3];
        }
        sine_angle = angle;
        camera->eye[0] += radius * sinf(sine_angle);
        cosine_angle = angle;
        camera->eye[2] += radius * cosf(cosine_angle);
        camera->eye[1] += height;
        maskA = startMaskA();
        floor_y = 5.0f + sColPosAndY((Vec3 *)camera->eye, 0, &maskA);
        if (camera->eye[1] < floor_y)
            camera->eye[1] = floor_y;
        moved = 0;
        i = 0;
        while (10000.0f != startPt(camera->eye, camera->interest)) {
            camera->eye[1] += lit_491[0];
            i++;
            moved = 1;
            if (i > 10)
                break;
        }
        if (moved != 0)
            camera->eye[1] += 0.2;
        if (60.0f != HSD_CObjGetFov(camera->cobj))
            HSD_CObjSetFov(camera->cobj, 60.0f);
        HSD_CObjSetInterest(camera->cobj, (Vec3 *)camera->interest);
        HSD_CObjSetEyePosition(camera->cobj, (Vec3 *)camera->eye);
        sCameraCalc(camera);
    }
}
#pragma dont_inline on
void camOnWin(CameraState2 *camera)
{
    s32 idx;
    s32 i;
    StartEntry *entry;
    HSD_JObj *joint;
    StartEntry *table;
    s32 index;
    s32 radius;
    s32 height;
    f32 (*m)[4];
    f32 angle;
    void *data;
    f32 sine_angle;
    f32 cosine_angle;
    ZoidState *zoid;

    idx = 0;
    if (camera != 0 && camera->cobj != 0 && (zoid = camera->zoid, zoid != 0) && zoid->joint != 0
        && camera->field_2F9 != 0
        && (table = (StartEntry *)zoid->field_2090, table != 0)) {
        data = zoid->anim_data;
        entry = &table[idx];
        index = entry->index;
        radius = entry->radius;
        height = entry->height - 30;
        joint = zoid->weapon_slots[index].joint;
        if (joint == (void *)0)
            joint = zoid->joint;
        m = (f32 (*)[4])((u8 *)joint + 0x44);
        angle = (f32)(f64)zoid->heading;
        angle = angle + 2.0 * (3.1415926 * (f32)(360.0 * vsyncN / 300.0)) / 360.0;
        angLimit(&angle);
        for (i = 0; i < 3; i++) {
            camera->eye[i] = m[i][3];
            camera->interest[i] = m[i][3];
        }
        sine_angle = angle;
        camera->eye[0] += radius * sinf(sine_angle);
        cosine_angle = angle;
        camera->eye[2] += radius * cosf(cosine_angle);
        camera->eye[1] += height;
        if (60.0f != HSD_CObjGetFov(camera->cobj))
            HSD_CObjSetFov(camera->cobj, 60.0f);
        HSD_CObjSetInterest(camera->cobj, (Vec3 *)camera->interest);
        HSD_CObjSetEyePosition(camera->cobj, (Vec3 *)camera->eye);
        sCameraCalc(camera);
    }
}
#pragma force_active on
s32 sCamMuteki;
s32 vcSeekMode2;
s32 vcSeekMode;
s32 sCamNS;
u32 sCamNN2;
s32 sCamNN;
u32 sCamNormal;
u32 sLightEfct = 0x00000001;
static const char lit_1730[4] = "esc";
#pragma force_active off
}
