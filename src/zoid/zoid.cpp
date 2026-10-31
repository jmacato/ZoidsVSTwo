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
typedef struct CoObj {
    HSD_JObj *joint, *base_joint;
    f32 minimum_scale, maximum_scale;
    s32 object_contact;
    u8 pad_14[4];
    s32 ground_contact;
    u8 reserved_1C[8];
    u16 ground_polygon_index;
    u8 reserved_26[0xE];
    f32 inverse[3][4];
    u32 field_64;
    s32 health, maximum_health;
    f32 position[3];
    f32 base_position[3];
    f32 movement[3];
    f32 ground_y;
    f32 field_9C;
} CoObj;
#include "aobj.h"
#include "archive.h"
typedef f32 Mtx[3][4];
typedef struct HSD_Shadow HSD_Shadow;
#include "game_zoid_data.h"

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
    void *field_0028;
    void *camera;
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
    s8 option_use_state;
    s8 option_use_previous;
    s16 option_energy_max;
    s16 option_energy;
    s16 option_start_cost;
    s16 option_hold_cost;
    s16 option_recharge;
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
    s32 weapon_mount_count;
    s32 selected_weapon_mount;
    s32 field_1374;
    WeaponMount weapon_mounts[20];
    WeaponMountEntry mount_extra[20];
    WeaponSlot weapon_slots[30];
    HSD_JObj *camera_joint;
    f32 clip_point[3];
    f32 field_1A40[3];
    s32 ground_turn;
    f32 field_1A78;
    HSD_JObj *field_1A7C;
    f32 field_1A80[3];
    s32 field_1A98;
    f32 field_1A9C[3];
    f32 field_1AA8;
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
    HSD_JObj *field_1BEC;
    f32 ground_height;
    f32 field_19FC;
    f32 field_1BF8;
    s32 field_1BFC;
    s32 foot_contacts;
    s32 grounded;
    s32 grounded_frames;
    f32 max_ground_depth;
    f32 position[3];
    f32 field_1C24[3];
    f32 height_offset;
    s32 field_1C34;
    f32 height_offset_speed;
    f32 child_translation_y;
    s32 field_1C40;
    s32 movement_flags;
    s32 field_1C48;
    u8 pad_1B9C[0x4];
    f32 step_heading;
    u8 pad_1BA4[0x1C];
    f32 heading;
    f32 high_angle;
    f32 field_1C84;
    u8 pad_1BCC[0x4];
    f32 field_1C8C;
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
    s32 jump_attack_phase;
    f32 velocity[3];
    s32 field_1DD4;
    s32 current_animation;
    u8 pad_1CE8[0x8];
    s32 attack_index;
    s32 field_1DF8;
    u8 pad_1CF8[0x34];
    s32 anim1_frames;
    f32 animation_end_frame;
    s32 animation_stopped;
    s32 animation_looped;
    f32 anim_current_frame;
    s32 animation_frame;
    u8 pad_1D44[0x2];
    u16 enemy_smoke_timer;
    s32 field_1E50;
    s32 field_1E54;
    s32 health_bar_timer;
    s32 field_1E5C;
    s32 field_1E60;
    s32 field_1E64;
    s32 field_1E68;
    s32 death_started;
    s32 lock_frames;
    s32 field_1E74;
    s32 field_1E78;
    s32 field_1E7C;
    s32 rumble_timer;
    u8 index;
    u8 slot;
    u8 target_pending;
    u8 target_slot;
    u8 pad_1D80[0x2];
    u8 field_1E9A;
    u8 pad_1D83[0x1];
    s32 field_1E9C;
    s32 field_1EA0;
    u8 pad_1D8C[0x8];
    u16 field_1EAE;
    u16 field_1EB0;
    u16 field_1EB2;
    u16 ground_polygon;
    u8 pad_1D9C[0x4];
    GroundMask *ground_material;
    f32 *ground_normal;
    s32 option_active;
    s32 stel_state;
    s32 previous_stel_state;
    u8 pad_1DB4[0x8];
    s32 interpolation_remaining;
    s32 interpolation_duration;
    s32 kills;
    s32 field_1EF0;
    s32 field_1EF4;
    f32 field_1EF8;
    f32 search_range;
    s32 armor;
    s32 field_1F04;
    f32 distances[16];
    u8 contact[16];
    u8 attack_in_range[0x10];
    u8 target_visibility[16];
    f32 target_angles[16];
    f32 field_1FF8[24];
    f32 field_2058;
    f32 field_205C;
    s32 field_2060;
    u16 *slider_info;
    u16 slider_positions[20];
    void *field_2090;
    void *anim_data;
    struct ZoidAIState *ai;
    s32 ai_active;
    s32 field_20A0;
    s32 gene_x;
    s32 gene_z;
    s32 field_20B4;
    s32 field_20B8;
    s32 field_20BC;
    s32 field_20C0;
    s32 field_20C4;
    s32 radar_range_bonus;
    s32 search_range_bonus;
    u8 pad_1F60[0x4];
    s32 armor_bonus;
    s32 field_20D8;
    s32 speed_bonus;
    s32 bullet_power_bonus;
    s32 melee_power_bonus;
    s32 turn_bonus;
    s32 field_20EC;
    u8 pad_1F80[0x4];
    s32 field_20F4;
    s32 field_20F8;
} ZoidState;

typedef char ZoidStateSizeCheck[sizeof(ZoidState) == 0x1F98 ? 1 : -1];

typedef struct PartDat3 {
    u8 pad_0000[0x78];
    s16 hit_a;
    u8 pad_007A[0xE0 - 0x7A];
    s16 hit_b;
    u8 pad_00E2[0x124 - 0xE2];
} PartDat3;
typedef struct WpInv3 {
    HSD_JObj *joint;
    f32 inverse[3][4];
    u8 pad_34[4];
} WpInv3;
typedef struct Weapon3 {
    s32 active;
    u8 pad_04[8];
    s32 type;
    u8 pad_10[0x34 - 0x10];
    u32 scope;
    ZoidState *owner;
    u8 pad_3C[0x144 - 0x3C];
    WpInv3 inverses[12];
    u8 pad_3E4[0x3F8 - (0x144 + 12 * 0x38)];
    s32 bullet_kind;
    u8 pad_3FC[0x404 - 0x3FC];
    s16 drain;
    u8 pad_406[0x6D8 - 0x406];
} Weapon3;
typedef char Weapon3SizeCheck[sizeof(Weapon3) == 0x6D8 ? 1 : -1];
extern PartDat3 Tea_partsdat[];
extern Weapon3 sWeapon[100];
extern s32 gameStartF;
extern s32 gametime;
extern s32 bitTBL[];

extern s32 sprintf(char *buffer, const char *format, ...);
extern void OSReport(const char *format, ...);
extern void __assert(const char *file, s32 line, const char *expr);
extern void MemCpy(u8 *source, u8 *destination, s32 size);
extern void free(void *ptr);
extern void sMakeMatrix(HSD_JObj *joint);
extern void ZoidCopyAnim(ZoidState *zoid);
extern s32 usbForce;
extern s32 sZoidHG;
extern s32 fexistU(char *name, s32 usb);
extern HSD_Archive *TEA_USBFileArchiveLoad(char *name, void **data);
extern HSD_Archive *sLoad(char *name, void **data);
extern void *LoadArc2USB(char *name);
extern void Str2Lows(char *name);
extern HSD_JObj *HSD_JObjLoadJoint(HSD_JObjDesc *description);
extern s32 jobjAnim(HSD_JObj *joint);
extern JointPose *makeJobj(void);
extern void stCopyObj(HSD_JObj *joint, JointPose *pose);
extern void stCopyMatMobj(HSD_JObj *joint, JointPose *pose);
extern HSD_JObj *searchJoint(ZoidState *zoid, const char *name);
extern void searchJointFoot(ZoidState *zoid, FootJoint *foot, const char *name);
extern void searchJointCoObj(ZoidState *zoid, void *part, const char *name, HSD_JObj *joint);
extern void zdCalcCoDmg(ZoidState *zoid);
extern void HiddenParts(HSD_JObj *joint, s32 hidden);
extern void *getAnimAobj(HSD_JObj *joint);
extern void ZoidInitParam(ZoidState *zoid);
extern void *malloc2(u32 size);
extern void ZeroMemory(void *data, u32 size);
extern u8 *sReadFileLenU(char *name, s32 usb, s32 *length);
extern void paramInit(void *control);
extern char *strcpy(char *dst, const char *src);
extern void ZoidPurge(ZoidState *zoid);
extern void HSD_JObjAddAnimAll(HSD_JObj *joint, void *a, void *b, void *c);

#define ZS32(off) (*(s32 *)((u8 *)zoid + (off)))
#define ZJ(off) (*(HSD_JObj **)((u8 *)zoid + (off)))
#define CS32(c, off) (*(s32 *)((u8 *)(c) + (off)))
extern s32 sWeaponEndF(s32 weapon);
extern s32 sWeaponGetAnim(s32 weapon);
extern void sWeaponLoopOFF(s32 weapon);
extern void sWeaponSetAnim(s32 weapon, s32 animation);
extern void sWeaponStopOFF(s32 weapon);
extern void zdWeaponChg(ZoidState *zoid);
extern void zdWeaponFireON_(ZoidState *zoid, s32 selected);
extern void sWeaponLoopON(s32 weapon);
extern void zdWeaponFireVoice(ZoidState *zoid);
extern void zdWeaponFireOFF_(ZoidState *zoid, s32 mount);
extern s32 sWeaponStopF(s32 weapon);
extern void ZoidLinerAnimNextXor(HSD_JObj *joint, JointPose *pose, f32 fraction, ZoidState *zoid);
extern void zdSetXYZ(ZoidState *zoid);
extern void zdSearchTop1(f32 *top, HSD_JObj *joint);
extern s32 zdWp1SliderN(ZoidState *zoid, s32 index, s32 value);
extern void zdCoObjOFF(void *object);
extern void sdDisp1(HSD_JObj *joint);
extern void ftSmoke(ZoidState *zoid, FootJoint *foot, s32 frame);
extern s32 sWeaponColor(s32 weapon, s32 color, s32 unused);
extern void ik3dAnim(ZoidState *zoid, FootJoint *foot);
extern void sEffectSet(s32 type, s32 mode, f32 *position, HSD_JObj *source, struct ZoidState *zoid);
extern s32 ZoidColor(ZoidState *zoid, s32 color, s32 flags);
extern void ZoidSetAnim(ZoidState *z, s32 n);
extern void HSD_AObjSetRate(HSD_AObj *animation, f32 value);
extern void HSD_AObjSetCurrentFrame(HSD_AObj *animation, f32 frame);
extern HSD_JObj *jobjWithAobj(HSD_JObj *joint);
extern JointPose *stSearchJobj(HSD_JObj *joint, JointPose *pose, HSD_JObj *target);
extern void ZoidCopyAnim1(HSD_JObj *joint, JointPose *pose);
extern JointPose *zdSearchBAK(HSD_JObj *joint, JointPose *pose, HSD_JObj *target);
extern void *steMobj;
extern ModelArchive *steFile;
extern void ZoidCopyMatSet(ZoidState *zoid, void *material);
extern void zdWeaponSightClear(ZoidState *zoid, s32 selected);
extern void ZoidCopyMatRet(ZoidState *zoid);
extern ModelArchive *csrFile;
extern ModelArchive *sdTest;
extern s32 csrDispON;
extern s32 csrNum;
extern void jobjDispAll(HSD_JObj *joint);
extern s32 gameEndF;
extern void zdVoiceSc(ZoidState *z, s32 no);
extern void stCopyMatRet(HSD_JObj *joint, JointPose *pose);
extern void stCopyMatReset(HSD_JObj *joint, JointPose *pose);
extern void zdWeaponForceFire1_(ZoidState *zoid, s32 selected);
typedef struct GameZoidV2 {
    s16 type;
    s16 weapons[28];
    s8 color;
    s8 weapon_colors[28];
    u8 pad_57;
    s16 person;
    u8 pad_5A[2];
    s16 option;
    s16 additional_parts[3];
    u8 pad_64[2];
} GameZoidV2;
typedef char GameZoidV2SizeCheck[sizeof(GameZoidV2) == 0x66 ? 1 : -1];
typedef struct VoiceEntry {
    u8 flags;
    s8 kind;
    u16 value;
    s8 choices[4];
} VoiceEntry;
typedef struct CamTmpV2 {
    u8 pad_000[0x244];
    void *cobj;
    u8 pad_248[0xB2];
    s8 lod_flag;
    u8 pad_2FB[0x9];
    f32 zoid_distance[16];
} CamTmpV2;
extern u8 sCamera[];
extern CamTmpV2 *camTmp;
extern void *binB(void *pack, s32 index);
extern void camVoiceOn(s32 person, s32 voice);
extern void sWeaponMount(s32 part, ZoidState *zoid, s32 slot, s32 mount);
extern void zdOptionUse3(ZoidState *zoid);
extern f32 sColRealPt(Vec3 *start, Vec3 *end, GroundMask *mask);
extern u32 PSMTXInverse(Mtx src, Mtx inv);
extern void zdCoObjMtxI(void *object);
extern void HSD_MtxGetRotation(f32 matrix[3][4], Vec3 *rotation);
extern void HSD_MtxGetScale(f32 matrix[3][4], Vec3 *scale);
extern void HSD_MtxGetTranslate(Mtx matrix, Vec3 *translation);
extern s32 HSD_MtxInverseConcat(Mtx inverse, Mtx source, Mtx destination);
extern void PSMTXCopy(f32 source[3][4], f32 destination[3][4]);
void ZoidAllPutXZ(void);
#pragma force_active on
u64 Zoid[16176];
u32 zdsMotTBL[1338] = {
    0x00000000, 0x00000000, 0x00000002, 0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000002, 0x00000002, 0x00000096, 0x00000096, 0x00000096, 0x00000096, 0x00000094, 0x00000094, 
    0x00000003, 0x00000096, 0x00000094, 0x00000094, 0x00000003, 0x00000096, 0x00000094, 0x00000094, 
    0x00000003, 0x000000F2, 0x000000F0, 0x000000F0, 0x000000F3, 0x000000F2, 0x000000F0, 0x000000F0, 
    0x000000F3, 0x000000F2, 0x000000F0, 0x000000F0, 0x000000F3, 0x000000F2, 0x000000F0, 0x000000F0, 
    0x000000F3, 0x000000F3, 0x000000F3, 0x000000F3, 0x0000000B, 0x0000000B, 0x0000000B, 0x0000000B, 
    0x0000000B, 0x0000000B, 0x000000F3, 0x000000F3, 0x000000F3, 0x000000F3, 0x000000F3, 0x000000F3, 
    0x000000F1, 0x000000F3, 0x000000E2, 0x000000F0, 0x000000F0, 0x000000F2, 0x000000F2, 0x0000000A, 
    0x0000000A, 0x000000FA, 0x000000FA, 0x0000000A, 0x000000FA, 0x0000000A, 0x000000FA, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x00000008, 0x0000000A, 
    0x000000FA, 0x000000F0, 0x0000000A, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 
    0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 
    0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 
    0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 
    0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 
    0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 
    0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 
    0x00000008, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 
    0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x0000000A, 0x00000000, 
    0x00000000, 0x00000000, 
};
#pragma force_active off
#define Zoid ((ZoidState *)Zoid)
extern void *searchMObj(HSD_JObj *jobj);
void ZoidPrologue(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        ZeroMemory(&Zoid[i], sizeof(ZoidState));
        Zoid[i].index = i;
    }
    steMobj = 0;
    if (steFile == (void *)0) {
        steFile = LoadArc2("zoid/stels_mat.dat");
        if (steFile != 0) {
            steMobj = searchMObj(steFile->models[0]);
        }
    }
    sdTest = LoadArc2("debug/c_.dat");
}
#undef Zoid
#undef lit_387
#define lit_387 "zoid/stels_mat.dat"
#undef lit_388
#define lit_388 "debug/c_.dat"
#define Zoid ((ZoidState *)Zoid)
extern void sWeaponUnMountAll(ZoidState *zoid);
extern void ZoidCopyMatReset(ZoidState *zoid);
extern void stStJobjUnref(JointPose *pose);
extern void jobjUnrefAll(HSD_JObj *joint);
extern void sArchivePurge(struct HSD_Archive *archive);
void ZoidPurge(ZoidState *zoid)
{
    s32 i;

    if (zoid == (void *)0)
        return;
    if (zoid->name[0] == 0)
        return;
    OSReport("zoid purge! [%s] \n", zoid);
    zoid->field_1E9A = 0;
    sWeaponUnMountAll(zoid);
    ZoidCopyMatReset(zoid);
    if (zoid->pose != 0) {
        stStJobjUnref(zoid->pose);
        zoid->pose = 0;
    }
    if (zoid->field_0038 != 0) {
        PurgeArc2((ModelArchive *)zoid->field_0038);
        zoid->field_0038 = 0;
    }
    if (zoid->joint != 0) {
        jobjUnrefAll(zoid->joint);
        zoid->joint = 0;
    }
    if (zoid->ai != 0) {
        free(zoid->ai);
        zoid->ai = 0;
    }
    if (zoid->anim_data != 0) {
        free(zoid->anim_data);
        zoid->anim_data = 0;
    }
    if (zoid->slider_info != 0) {
        free(zoid->slider_info);
        zoid->slider_info = 0;
    }
    if (zoid->field_2090 != 0) {
        free(zoid->field_2090);
        zoid->field_2090 = 0;
    }
    if (zoid->archive != 0) {
        sArchivePurge(zoid->archive);
        zoid->archive = 0;
    }
    if (zoid->field_0028 != 0)
        free(zoid->field_0028);
    zoid->field_0028 = 0;
    if (zoid->field_003C != 0)
        free(zoid->field_003C);
    zoid->field_003C = 0;
    ZeroMemory(zoid, sizeof(ZoidState));
    for (i = 0; i < 16; i++) {
        if (zoid == &Zoid[i]) {
            zoid->index = i;
        }
    }
}
#undef Zoid
#undef lit_424
#define lit_424 "zoid purge! [%s] \n"
#pragma force_active on
static const char lit_464[32] = "joint error [%s] \n ============";
static const char lit_586[12] = "zoid/%s.dat";
#pragma force_active off
#define Zoid ((ZoidState *)Zoid)
void ZoidPurgeAll(void)
{
    s32 i;
    for (i = 0; i < 16; i++) {
        ZoidPurge(&Zoid[i]);
    }
}
#undef Zoid
extern void sWeaponPurge(s32 weapon);
void ZoidWeaponPurge(ZoidState *zoid)
{
    s32 i;
    s32 weapon;
    for (i = 0; i < 30; i++) {
        weapon = zoid->weapon_slots[i].weapon;
        if (weapon != -1)
            sWeaponPurge(weapon);
    }
}

void searchJointFoot(ZoidState *zoid, FootJoint *foot, const char *part)
{
    foot->joint = searchJoint(zoid, part);
    if (foot->joint != (void *)0) {
        zoid->foot_count++;
        foot->offset = foot->joint->translation.y;
    }
}
#pragma force_active on
static const char lit_463[6] = "%s_%s";
#pragma force_active off
typedef struct CoObj3 {
    HSD_JObj *joint;
    HSD_JObj *base_joint;
    f32 minimum_scale;
    f32 maximum_scale;
    f32 radius;
    u8 pad_14[0x54];
    s32 health;
    s32 maximum_health;
} CoObj3;

extern f64 __frsqrte(f64);

#pragma push
#pragma dont_inline off
static inline f32 sjSqrtf(f32 x)
{
    static const f64 _half = 0.5;
    static const f64 _three = 3.0;
    volatile f32 y;

    if (x > 0.0f) {
        f64 guess = __frsqrte((f64)x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        y = (f32)(x * guess);
        return y;
    }
    return x;
}

static inline f32 sjGetScaleX(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 899, "jobj");
    return jobj->scale.x;
}

static inline f32 sjGetScaleY(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 911, "jobj");
    return jobj->scale.y;
}

static inline f32 sjGetScaleZ(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 923, "jobj");
    return jobj->scale.z;
}

void searchJointCoObj(ZoidState *zoid, void *coobj, const char *part, HSD_JObj *base_joint)
{
#define object ((CoObj3 *)coobj)
    HSD_JObj *joint;
    f32 y;
    f32 z;
    f32 x;
    f32 minimum;
    f32 maximum;

    object->health = 0;
    object->maximum_health = 0;
    joint = searchJoint(zoid, part);
    object->joint = joint;
    if (joint != (void *)0) {
        object->maximum_health = 100;
        object->health = 100;
        x = sjGetScaleX(joint);
        y = sjGetScaleY(joint);
        z = sjGetScaleZ(joint);
        minimum = x;
        maximum = x;
        if (maximum < y)
            maximum = y;
        if (maximum < z)
            maximum = z;
        if (minimum > y)
            minimum = y;
        if (minimum > z)
            minimum = z;
        object->minimum_scale = minimum;
        object->maximum_scale = maximum;
        object->radius = sjSqrtf(x * x + y * y + z * z) / 2.0f;
        object->base_joint = base_joint;
    }
}
#undef object
#pragma pop
#undef lit_485
#define lit_485 "jobj.h"
#undef lit_486
#define lit_486 "jobj"
#define Zoid ((ZoidState *)Zoid)

void ZoidCopyAnim1(HSD_JObj *joint, JointPose *pose)
{
    s32 row, column;
    if (joint != (void *)0 && pose != (void *)0) {
        pose->rotation[0] = joint->rotation[0];
        pose->rotation[1] = joint->rotation[1];
        pose->rotation[2] = joint->rotation[2];
        pose->translation.x = joint->translation.x;
        pose->translation.y = joint->translation.y;
        pose->translation.z = joint->translation.z;
        pose->scale.x = joint->scale.x;
        pose->scale.y = joint->scale.y;
        pose->scale.z = joint->scale.z;
        for (row = 0; row < 3; row++) {
            for (column = 0; column < 4; column++) {
                pose->matrix[row][column] = joint->matrix[row][column];
            }
        }
        if ((joint->flags & 0x1000) != 0)
            return;
        joint = joint->child;
        pose = pose->child;
        while (joint != 0) {
            if (joint == (void *)0 || pose == (void *)0)
                return;
            ZoidCopyAnim1(joint, pose);
            joint = joint->next;
            pose = pose->next;
        }
    }
}
#undef Zoid

void ZoidCopyAnim(ZoidState *zoid)
{
    ZoidCopyAnim1(zoid->joint, zoid->pose);
}
extern f32 sLiner(f32 value, f32 target, f32 fraction);

void ZoidLinerAnimXor(HSD_JObj *joint, JointPose *pose, f32 fraction, ZoidState *zoid)
{
    s32 index;
    s32 row;
    s32 col;

    if (joint == (void *)0) {
        return;
    }
    if (pose == (void *)0) {
        return;
    }
    for (index = 0; index < 10; index++) {
        if (zoid->attack_objects[index].joint == joint) {
            return;
        }
    }
    if (zoid->spik_joints[5] == joint) {
        return;
    }
    joint->translation.x = sLiner(joint->translation.x, pose->translation.x, fraction);
    joint->translation.y = sLiner(joint->translation.y, pose->translation.y, fraction);
    joint->translation.z = sLiner(joint->translation.z, pose->translation.z, fraction);
    joint->rotation[0] = sLiner(joint->rotation[0], pose->rotation[0], fraction);
    joint->rotation[1] = sLiner(joint->rotation[1], pose->rotation[1], fraction);
    joint->rotation[2] = sLiner(joint->rotation[2], pose->rotation[2], fraction);
    joint->scale.x = sLiner(joint->scale.x, pose->scale.x, fraction);
    joint->scale.y = sLiner(joint->scale.y, pose->scale.y, fraction);
    joint->scale.z = sLiner(joint->scale.z, pose->scale.z, fraction);
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 4; col++) {
            joint->matrix[row][col] = sLiner(joint->matrix[row][col], pose->matrix[row][col], fraction);
        }
    }
    if (joint->flags & 0x1000) {
        return;
    }
    joint = joint->child;
    pose = pose->child;
    while (joint != 0) {
        if (joint == (void *)0 || pose == (void *)0) {
            return;
        }
        ZoidLinerAnimXor(joint, pose, fraction, zoid);
        joint = joint->next;
        pose = pose->next;
    }
}
void ZoidLinerAnimNextXor(HSD_JObj *joint, JointPose *pose, f32 fraction, ZoidState *zoid)
{
    while (joint != 0 && pose != 0) {
        ZoidLinerAnimXor(joint, pose, fraction, zoid);
        joint = joint->next;
        pose = pose->next;
    }
}

void ZoidCopyMatRet(ZoidState *zoid)
{
    stCopyMatRet(zoid->joint, zoid->pose);
}
extern void stCopyMatSet(HSD_JObj *joint, void *material);

void ZoidCopyMatSet(ZoidState *zoid, void *material)
{
    stCopyMatSet(zoid->joint, material);
}

void ZoidCopyMatReset(ZoidState *zoid)
{
    stCopyMatReset(zoid->joint, zoid->pose);
}

void setCoDmg2(CoObj *first, CoObj *second, CoDamage *damage)
{
    f32 first_ratio;
    f32 second_ratio;
    (void)0.5f;
    damage->maximum_health = 0;
    damage->health = 0;
    if (first->maximum_health != 0) {
        first_ratio = (f32)(first->health / first->maximum_health);
        damage->health = first->health;
        damage->maximum_health = first->maximum_health;
    } else {
        first_ratio = 1000.f;
    }
    if (second->maximum_health != 0) {
        second_ratio = (f32)(second->health / second->maximum_health);
    } else {
        second_ratio = 1000.f;
    }
    if (first_ratio > second_ratio) {
        damage->health = second->health;
        damage->maximum_health = second->maximum_health;
    }
    if (damage->health < 0) {
        damage->health = 0;
    }
}
extern u32 sZoidEx;

#define Zoid ((ZoidState *)Zoid)

void zdCalcCoDmg(ZoidState *zoid)
{
    f32 ratio;

    setCoDmg2((CoObj *)zoid->head_coobj, (CoObj *)zoid->neck_coobj, &zoid->damages[0]);
    setCoDmg2((CoObj *)zoid->bust_coobj, (CoObj *)zoid->hip_coobj, &zoid->damages[1]);
    setCoDmg2((CoObj *)zoid->l_arm1_coobj, (CoObj *)zoid->l_arm2_coobj, &zoid->damages[2]);
    setCoDmg2((CoObj *)zoid->r_arm1_coobj, (CoObj *)zoid->r_arm2_coobj, &zoid->damages[3]);
    setCoDmg2((CoObj *)zoid->l_leg1_coobj, (CoObj *)zoid->l_leg2_coobj, &zoid->damages[4]);
    setCoDmg2((CoObj *)zoid->r_leg1_coobj, (CoObj *)zoid->r_leg2_coobj, &zoid->damages[5]);
    if (zoid->max_health <= 0) {
        zoid->max_health = 0;
        zoid->super_attack_enabled = 0;
    } else {
        ratio = (f32)zoid->health / (f32)zoid->max_health;
        zoid->super_attack_enabled = ratio < 0.3333333;
    }
    if ((s32)sZoidEx != 0) {
        zoid->super_attack_enabled = 1;
        if (zoid->super_attack_cooldown > 60) {
            zoid->super_attack_cooldown = 60;
        }
    }
}
#undef Zoid
void ZoidInitParam(ZoidState *zoid)
{
    s32 type;

    if (zoid == 0) {
        return;
    }
    if (zoid->joint == 0) {
        return;
    }
    type = zoid->type;
    zoid->health = Tea_zoiddat[type].health;
    *(s32 *)((u8 *)zoid + 0x148) = zoid->health;
    zoid->max_health = zoid->health;
    zoid->field_1E54 = 0x384;
    zdCalcCoDmg(zoid);
    zoid->requested_action = 0;
    zoid->action = 0;
    zoid->jump_phase = 0;
    zoid->death_pending = 0;
    zoid->death_started = 0;
    zoid->field_1E5C = 0;
    zoid->field_1E74 = 0;
}
typedef struct Param {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0C;
    s32 arr10[8];
    s32 f30[2];
    s32 f38;
    s32 f3C;
    s32 f40;
    s32 f44;
    s32 f48;
    s32 f4C;
    s32 f50;
    u8 pad_54[0x98 - 0x54];
    s32 f98;
    s32 f9C;
    s32 fA0;
    s32 fA4;
    s32 fA8;
    s32 fAC;
    s32 fB0;
    s32 fB4;
    s32 fB8;
    s32 fBC;
    s32 fC0;
    s32 fC4;
    s32 fC8;
    s32 fCC;
    s32 fD0;
    u8 pad_D4[0xE8 - 0xD4];
    f32 fE8;
    f32 fEC;
    f32 fF0;
    s32 fF4;
    u8 pad_F8[0xFC - 0xF8];
    s32 fFC;
    u8 pad_100[0x190 - 0x100];
    s32 f190;
} Param;


#define p ((Param *)control)
void paramInit(void *control)
{
    s32 i;

    p->f00 = 1;
    p->f04 = 1;
    p->f08 = 2;
    p->f0C = 2;
    for (i = 0; i < 8; i++) {
        p->arr10[i] = 1;
    }
    p->f38 = 1;
    p->f40 = 1;
    p->f48 = 1;
    p->f50 = 1;
    p->f98 = 0;
    p->fA0 = 10;
    p->fA4 = 15;
    p->fE8 = 0.0f;
    p->fF0 = 25.0f;
    p->fEC = 7.0f;
    p->f9C = 0x78;
    p->fA8 = 9;
    p->fAC = 0x28;
    p->fB0 = 10;
    p->fB4 = 0x1F;
    p->fB8 = 0x12;
    p->fBC = 0x47;
    p->fC0 = 12;
    p->fC4 = 0x190;
    p->fF4 = 4;
    p->fC8 = 0x28;
    p->fCC = 0x14;
    p->fFC = 3;
    p->fD0 = 0x28;
    p->f190 = 4;
}
#undef p
#pragma dont_inline off
static inline f32 HSD_JObjGetTranslationY(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_485, 1021, lit_486);
    return jobj->translation.y;
}

static inline f32 HSD_JObjGetTranslationZ(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_485, 1033, lit_486);
    return jobj->translation.z;
}

s32 ZoidInit_(ZoidState *zoid, s32 type, s32 usb)
{
    char path[120];
    u8 *ai;
    void *data;
    HSD_JObj *look;
    ZoidControlData *control;
    ModelAnimationTable *scene;
    HSD_Archive *archive;
    s32 j;
    s32 i;
    CameraMountInfo *entry;
    u8 *buf2;
    u8 *cam;
    u8 *slider;

    if (usbForce != 0) {
        usb = 1;
    }
    HSD_JObj *look2;
    ZoidPurge(zoid);
    zoid->field_1E9A = 0;
    ZS32(0x1D94) = 0;
    data = 0;
    if (sZoidHG != 0) {
        sprintf(path, "zoid/HG/%s.dat", Tea_zoiddat[type].model_name);
        if (fexistU(path, usb) == 0) {
            sZoidHG = 0;
        }
    }
    if (sZoidHG == 0) {
        sprintf(path, lit_586, Tea_zoiddat[type].model_name);
    }
    OSReport("%s\n", path);
    if (fexistU(path, usb) == 0) {
        return 0;
    }
    if (usb != 0) {
        archive = TEA_USBFileArchiveLoad(path, &data);
    } else {
        archive = sLoad(path, &data);
    }
    zoid->archive = archive;
    zoid->type = type;
    if (archive == (void *)0) {
        return 0;
    }
    HSD_ArchiveLoadDepends(archive, 0, 0, 0);
    HSD_ArchiveResolveReference(archive, 0);
    ZS32(0x3C) = (s32)data;
    scene = (ModelAnimationTable *)HSD_ArchiveGetPublicAddress(archive, "scene_data");
    zoid->animations = scene;
    if (scene == (void *)0) {
        ZoidPurge(zoid);
        return 0;
    }
    strcpy(zoid->name, Tea_zoiddat[type].model_name);
    Str2Lows(zoid->name);
    sprintf(path, "zoid/%s_b.dat", Tea_zoiddat[type].model_name);
    if (usb != 0) {
        zoid->field_0038 = LoadArc2USB(path);
    } else {
        zoid->field_0038 = LoadArc2(path);
    }
    if (zoid->field_0038 == 0) {
        ZoidPurge(zoid);
        return 0;
    }
    if (scene->models != 0) {
        zoid->joint = HSD_JObjLoadJoint(scene->models[0]->joint);
        HSD_JObjAddAnimAll(zoid->joint,
                           scene->models[0]->joint_animations != 0 ? scene->models[0]->joint_animations[0] : (void *)0,
                           scene->models[0]->material_animations != 0 ? scene->models[0]->material_animations[0] : (void *)0,
                           scene->models[0]->shape_animations != 0 ? scene->models[0]->shape_animations[0] : (void *)0);
    }
    jobjAnim(zoid->joint);
    sMakeMatrix(zoid->joint);
    zoid->pose = makeJobj();
    stCopyObj(zoid->joint, zoid->pose);
    stCopyMatMobj(zoid->joint, zoid->pose);
    ZoidCopyAnim(zoid);
    char *names[28] = {
        "mtp_0_joint", "mtp_1_joint", "mtp_2_joint", "mtp_3_joint", "mtp_4_joint", "mtp_5_joint", "mtp_6_joint",
        "mtp_7_joint", "mtp_8_joint", "mtp_9_joint", "BUST1_joint", "HIP1_joint", "L_ARM1_joint", "R_ARM1_joint",
        "L_ARM2_joint", "R_ARM2_joint", "L_HAND_joint", "R_HAND_joint", "NECK1_joint", "HEAD1_joint", "L_LEG1_joint",
        "L_LEG2_joint", "R_LEG1_joint", "R_LEG2_joint", "L_FOOT_joint", "R_FOOT_joint", "TEAL1_joint", 0
    };
    for (i = 0; i < 30; i++) {
        zoid->weapon_slots[i].joint = 0;
    }
    for (i = 0; names[i] != 0; i++) {
        zoid->weapon_slots[i].joint = searchJoint(zoid, names[i]);
    }
    ZJ(0x1B44) = searchJoint(zoid, "scope_joint");
    zoid->f_base_joint = searchJoint(zoid, "f_base_joint");
    zoid->b_base_joint = searchJoint(zoid, "b_base_joint");
    if (zoid->f_base_joint == 0) {
        zoid->f_base_joint = searchJoint(zoid, "base_joint");
    }
    if (zoid->b_base_joint == 0) {
        zoid->b_base_joint = searchJoint(zoid, "base_joint");
    }
    zoid->foot_count = 0;
    searchJointFoot(zoid, &zoid->feet[0], "R_glove_joint");
    searchJointFoot(zoid, &zoid->feet[1], "L_glove_joint");
    searchJointFoot(zoid, &zoid->feet[2], "R_shoe_joint");
    searchJointFoot(zoid, &zoid->feet[3], "L_shoe_joint");
    zoid->field_1ACC = searchJoint(zoid, "senter_joint");
    if (zoid->field_1ACC != 0) {
        zoid->senter_translation_y = HSD_JObjGetTranslationY(zoid->field_1ACC);
        zoid->senter_translation_z = HSD_JObjGetTranslationZ(zoid->field_1ACC);
    }
    ZJ(0x1260) = searchJoint(zoid, "berut1_joint");
    ZJ(0x1264) = searchJoint(zoid, "cap_joint");
    ZJ(0x1268) = searchJoint(zoid, "collar_joint");
    ZJ(0x126C) = searchJoint(zoid, "jacket_joint");
    ZJ(0x1270) = searchJoint(zoid, "berut1_joint");
    zoid->tail_base_joints[0] = searchJoint(zoid, "fly_joint");
    for (i = 1; i < 8; i++) {
        zoid->tail_base_joints[i] = zoid->tail_base_joints[0];
    }
    ZJ(0x80) = searchJoint(zoid, "to_fly_joint");
    ZJ(0x9C) = searchJoint(zoid, "to_to_fly_joint");
    ZJ(0xB8) = searchJoint(zoid, "to_to_to_fly_joint");
    ZJ(0xD4) = searchJoint(zoid, "to_to_to_to_fly_joint");
    ZJ(0xF0) = searchJoint(zoid, "to_to_to_to_to_fly_joint");
    ZJ(0x10C) = searchJoint(zoid, "to_to_to_to_to_to_fly_joint");
    ZJ(0x128) = searchJoint(zoid, "to_to_to_to_to_to_to_fly_joint");
    ZJ(0x144) = zoid->tail_base_joints[0];
    zoid->buckle_joint = searchJoint(zoid, "buckle_joint");
    ZJ(0x1A34) = searchJoint(zoid, "berut1_joint");
    ZJ(0x1A38) = searchJoint(zoid, "jacket_joint");
    ZJ(0x1A3C) = searchJoint(zoid, "collar_joint");
    ZJ(0x1A40) = searchJoint(zoid, "NAVEL_joint");
    searchJointCoObj(zoid, zoid->body_coobj, "c_BODY_joint", ZJ(0x1260));
    searchJointCoObj(zoid, zoid->head_coobj, "c_head_joint", ZJ(0x1264));
    searchJointCoObj(zoid, zoid->neck_coobj, "c_neck_joint", ZJ(0x1268));
    searchJointCoObj(zoid, zoid->bust_coobj, "c_bust_joint", ZJ(0x126C));
    searchJointCoObj(zoid, zoid->hip_coobj, "c_hip_joint", ZJ(0x1270));
    searchJointCoObj(zoid, zoid->tail_coobjs[0], "c_tail_joint", zoid->tail_base_joints[0]);
    char tail[200];
    for (i = 1; i < 8; i++) {
        sprintf(tail, "c_tail%d_joint", i);
        searchJointCoObj(zoid, zoid->tail_coobjs[i], tail, zoid->tail_base_joints[i]);
    }
    searchJointCoObj(zoid, zoid->l_arm1_coobj, "c_l_arm1_joint", zoid->feet[1].joint);
    searchJointCoObj(zoid, zoid->l_arm2_coobj, "c_l_arm2_joint", zoid->feet[1].joint);
    searchJointCoObj(zoid, zoid->r_arm1_coobj, "c_r_arm1_joint", zoid->feet[0].joint);
    searchJointCoObj(zoid, zoid->r_arm2_coobj, "c_r_arm2_joint", zoid->feet[0].joint);
    searchJointCoObj(zoid, zoid->l_leg1_coobj, "c_l_leg1_joint", zoid->feet[3].joint);
    searchJointCoObj(zoid, zoid->l_leg2_coobj, "c_l_leg2_joint", zoid->feet[3].joint);
    searchJointCoObj(zoid, zoid->r_leg1_coobj, "c_r_leg1_joint", zoid->feet[2].joint);
    searchJointCoObj(zoid, zoid->r_leg2_coobj, "c_r_leg2_joint", zoid->feet[2].joint);
    searchJointCoObj(zoid, zoid->l_foot_coobj, "c_l_foot_joint", zoid->feet[3].joint);
    searchJointCoObj(zoid, zoid->r_foot_coobj, "c_r_foot_joint", zoid->feet[2].joint);
    zdCalcCoDmg(zoid);
    static char *tbl[6] = { "L_spip_joint", "R_spip_joint", "L_spik_joint", "R_spik_joint", "up_WP_joint", "att_E_joint" };
    for (i = 0; i < 6; i++) {
        zoid->spik_joints[i] = searchJoint(zoid, tbl[i]);
        OSReport("joint spik [%s] is %s \n", tbl[i], zoid->spik_joints[i] != 0 ? "on " : "off");
    }
    char attack[200];
    for (i = 0; i < 10; i++) {
        sprintf(attack, "a_%1d_joint", i);
        zoid->attack_objects[i].joint = searchJoint(zoid, attack);
        HiddenParts(zoid->attack_objects[i].joint, 1);
    }
    OSReport("atack... read ! ok \n");
    if (zoid->joint != 0) {
        zoid->aobj = (HSD_AObj *)getAnimAobj(zoid->joint->child);
    }
    *(f32 *)((u8 *)zoid + 0x1B40) = 1.0f + zoid->senter_translation_y;
    ZS32(0x1D90) = 2;
    zoid->field_1EF8 = Tea_zoiddat[type].radar_range;
    ZoidInitParam(zoid);

    s32 length;
    sprintf(path, "zoid/idp/zoid_%03d.prm", type);
    buf2 = (u8 *)malloc2(0x710);
    control = (ZoidControlData *)buf2;
    zoid->anim_data = control;
    if (control != 0) {
        ZeroMemory(buf2, 0x710);
        paramInit(control);
        buf2 = sReadFileLenU(path, usb, &length);
        if (buf2 == 0) {
            sprintf(path, "zoid/%s.prm", Tea_zoiddat[type].model_name);
            buf2 = sReadFileLenU(path, usb, &length);
        }
        if (buf2 != 0) {
            MemCpy(buf2, (u8 *)control, length);
            free(buf2);
        }
        if (CS32(control, 0x190) == 0) {
            CS32(control, 0x190) = 4;
        }
        if (CS32(control, 0xD0) == 0) {
            CS32(control, 0xD0) = 40;
        }
        if (CS32(control, 0xD4) == 0) {
            CS32(control, 0xD4) = 150;
        }
        if (CS32(control, 0xD8) == 0) {
            CS32(control, 0xD8) = 150;
        }
        if (CS32(control, 0xE4) == 0) {
            CS32(control, 0xE4) = 15;
        }
        if (CS32(control, 0x308) == 0) {
            CS32(control, 0x308) = 250;
        }
        for (i = 4; i < 10; i++) {
            control->field_0140[i] = 0;
        }
    }
    zoid->weapon_mount = -1;
    zoid->selected_weapon_mount = 1;
    ai = (u8 *)malloc2(0xA0);
    if (ai != 0) {
        ZeroMemory(ai, 0xA0);
        zoid->ai = (ZoidAIState *)ai;
    }
    u8 *data2;
    s32 length2;
    sprintf(path, "zoid/mdp/%s.cam", Tea_zoiddat[type].model_name);
    zoid->field_2090 = malloc2(0x164);
    if (zoid->field_2090 != 0) {
        cam = (u8 *)(s32)zoid->field_2090;
        ZeroMemory(cam, 0x164);
        if (CS32(cam, 0x158) == 0) {
            CS32(cam, 0x158) = 60;
        }
        if (CS32(cam, 0x15C) == 0) {
            CS32(cam, 0x15C) = 90;
        }
        if (CS32(cam, 0x160) == 0) {
            CS32(cam, 0x160) = 120;
        }
        data2 = sReadFileLenU(path, usb, &length2);
        if (data2 != 0) {
            MemCpy(data2, (u8 *)(s32)zoid->field_2090, length2);
            free(data2);
        }
    }
    if (zoid->field_2090 != 0) {
        for (i = 0; i < 20; i++) {
            s32 angles[20] = { 180, 172, 171, 204, 180, 205, 153, 180, 180, 158, 214, 153, 221, 157, 221, 148, 220, 230, 140, 180 };
            s32 distances[20] = { 32, 17, 14, 27, 34, 20, 15, 23, 1, 14, 32, 36, 21, 41, 68, 41, 91, 22, 25, 6 };
            s32 heights[20] = { 39, 28, 27, 30, 41, 35, 30, 43, 35, 24, 38, 40, 46, 50, 65, 63, 85, 30, 30, 40 };
            entry = (CameraMountInfo *)((s32)zoid->field_2090 + i * 16);
            if (entry->height == 0 || zoid->weapon_slots[entry->joint_slot].joint == 0) {
                entry->angle = angles[i];
                entry->height = heights[i];
                entry->distance = distances[i];
                if (*(void **)((u8 *)zoid + 0x1948) != 0) {
                    entry->joint_slot = 19;
                } else {
                    for (j = 0; j < 27; j++) {
                        if (zoid->weapon_slots[j].joint != 0) {
                            entry->joint_slot = j;
                            break;
                        }
                    }
                }
            }
            if (entry->angle == 0) {
                entry->angle = 180;
            }
            if (entry->height == 0) {
                entry->height = 30;
            }
            if (entry->distance == 0) {
                entry->distance = 10;
            }
        }
        if (CS32((s32)zoid->field_2090, 0x154) == 0) {
            CS32((s32)zoid->field_2090, 0x154) = 90;
        }
    }
    for (i = 0; i < 20; i++) {
        zoid->slider_positions[i] = 0;
    }
    s32 length3;
    sprintf(path, "zoid/mdp/%s.p_a", Tea_zoiddat[type].model_name);
    slider = (u8 *)malloc2(0x732);
    zoid->slider_info = (u16 *)slider;
    if (slider != 0) {
        ZeroMemory(slider, 0x732);
        for (i = 0; i < 200; i++) {
            *(u16 *)((u8 *)zoid->slider_info + 0x142 + i * 2) = i;
            *((u8 *)zoid->slider_info + 0x2D2 + i) = 0;
            *((u8 *)zoid->slider_info + 0x39A + i) = 0;
            if (zdsMotTBL[i] & 0x20) {
                *((u8 *)zoid->slider_info + 0x2D2 + i) |= 8;
            }
            if (zdsMotTBL[i] & 0x10) {
                *((u8 *)zoid->slider_info + 0x2D2 + i) |= 4;
            }
            if (zdsMotTBL[i] & 0x40) {
                *((u8 *)zoid->slider_info + 0x2D2 + i) |= 2;
            }
            if (zdsMotTBL[i] & 0x4) {
                *((u8 *)zoid->slider_info + 0x2D2 + i) |= 1;
            }
            if (zdsMotTBL[i] & 0x8) {
                *((u8 *)zoid->slider_info + 0x2D2 + i) |= 0xF0;
            }
            if (zdsMotTBL[i] & 0x1) {
                *((u8 *)zoid->slider_info + 0x39A + i) |= 2;
            }
            if (zdsMotTBL[i] & 0x2) {
                *((u8 *)zoid->slider_info + 0x39A + i) |= 1;
            }
        }
        if (*(u16 *)((u8 *)zoid->slider_info + 0x702) == 0) {
            *(u16 *)((u8 *)zoid->slider_info + 0x702) = 18;
        }
        if (*(u16 *)((u8 *)zoid->slider_info + 0x706) == 0) {
            *(u16 *)((u8 *)zoid->slider_info + 0x706) = 18;
        }
        if (*(u16 *)((u8 *)zoid->slider_info + 0x704) == 0) {
            *(u16 *)((u8 *)zoid->slider_info + 0x704) = 30;
        }
        if (*(u16 *)((u8 *)zoid->slider_info + 0x708) == 0) {
            *(u16 *)((u8 *)zoid->slider_info + 0x708) = 30;
        }
    }
    if (zoid->slider_info != 0) {
        slider = sReadFileLenU(path, usb, &length3);
        if (slider != 0) {
            MemCpy(slider, (u8 *)zoid->slider_info, length3);
            free(slider);
        }
    }
    if (CS32(zoid->anim_data, 0xF4) != 2) {
        ZS32(0x1B34) = zoid->foot_count;
    } else {
        ZS32(0x1B34) = 2;
    }
    for (i = 0; i < 30; i++) {
        zoid->weapon_slots[i].weapon = -1;
    }
    for (i = 0; i < 20; i++) {
        zoid->weapon_mounts[i].active = 0;
        for (j = 0; j < 4; j++) {
            zoid->weapon_mounts[i].entries[j].slot = -1;
            zoid->weapon_mounts[i].entries[j].active = 0;
        }
        zoid->mount_extra[i].slot = -1;
        zoid->mount_extra[i].active = 0;
    }
    for (i = 0; i < 3; i++) {
        zoid->additional_parts[i] = -1;
    }
    HSD_JObj *buckle;
    buckle = zoid->buckle_joint;
    sMakeMatrix(zoid->joint);
    if (buckle != 0) {
        *(f32 *)((u8 *)zoid + 0x1B50) = buckle->matrix[1][3] - zoid->joint->matrix[1][3];
    }
    look = 0;
    if (look == 0) look = *(HSD_JObj **)zoid->bust_coobj;
    if (look == 0) look = *(HSD_JObj **)zoid->hip_coobj;
    if (look == 0) look = *(HSD_JObj **)zoid->neck_coobj;
    if (look == 0) look = *(HSD_JObj **)zoid->head_coobj;
    if (look == 0) look = ZJ(0x1A34);
    if (look == 0) look = ZJ(0x1A38);
    if (look == 0) look = ZJ(0x1A3C);
    if (look == 0) look = ZJ(0x1A30);
    if (look == 0) look = ZJ(0x1A38);
    if (look == 0) OSReport("jobjLook error \n");
    if (look == 0) look = zoid->joint;
    ZJ(0x19C4) = look;
    ZoidControlData *ctrl2;
    ctrl2 = (ZoidControlData *)zoid->anim_data;
    switch (CS32(ctrl2, 0xFC)) {
    case 0:
        look2 = ZJ(0x1A40);
        break;
    case 1:
        look2 = 0;
        break;
    case 2:
        look2 = ZJ(0x1A3C);
        break;
    case 3:
        look2 = ZJ(0x1A38);
        break;
    case 4:
        look2 = ZJ(0x1A34);
        break;
    case 5:
        look2 = ZJ(0x1A30);
        break;
    default:
        look2 = 0;
        break;
    }
    ZJ(0x19E8) = look2;
    ZS32(0x1CC4) = -1;
    ZS32(0x1CC8) = 0;
    OSReport("===================zoid OK============\n");
    return 1;
}
#pragma dont_inline on
#undef lit_657
#define lit_657 "mtp_0_joint"
#undef lit_658
#define lit_658 "mtp_1_joint"
#undef lit_659
#define lit_659 "mtp_2_joint"
#undef lit_660
#define lit_660 "mtp_3_joint"
#undef lit_661
#define lit_661 "mtp_4_joint"
#undef lit_662
#define lit_662 "mtp_5_joint"
#undef lit_663
#define lit_663 "mtp_6_joint"
#undef lit_664
#define lit_664 "mtp_7_joint"
#undef lit_665
#define lit_665 "mtp_8_joint"
#undef lit_666
#define lit_666 "mtp_9_joint"
#undef lit_667
#define lit_667 "BUST1_joint"
#undef lit_668
#define lit_668 "HIP1_joint"
#undef lit_669
#define lit_669 "L_ARM1_joint"
#undef lit_670
#define lit_670 "R_ARM1_joint"
#undef lit_671
#define lit_671 "L_ARM2_joint"
#undef lit_672
#define lit_672 "R_ARM2_joint"
#undef lit_673
#define lit_673 "L_HAND_joint"
#undef lit_674
#define lit_674 "R_HAND_joint"
#undef lit_675
#define lit_675 "NECK1_joint"
#undef lit_676
#define lit_676 "HEAD1_joint"
#undef lit_677
#define lit_677 "L_LEG1_joint"
#undef lit_678
#define lit_678 "L_LEG2_joint"
#undef lit_679
#define lit_679 "R_LEG1_joint"
#undef lit_680
#define lit_680 "R_LEG2_joint"
#undef lit_681
#define lit_681 "L_FOOT_joint"
#undef lit_682
#define lit_682 "R_FOOT_joint"
#undef lit_683
#define lit_683 "TEAL1_joint"
#undef lit_716
#define lit_716 "L_spip_joint"
#undef lit_717
#define lit_717 "R_spip_joint"
#undef lit_718
#define lit_718 "L_spik_joint"
#undef lit_719
#define lit_719 "R_spik_joint"
#undef lit_720
#define lit_720 "up_WP_joint"
#undef lit_721
#define lit_721 "att_E_joint"
#undef lit_892
#define lit_892 "zoid/HG/%s.dat"
#undef lit_893
#define lit_893 "%s\n"
#undef lit_894
#define lit_894 "scene_data"
#undef lit_895
#define lit_895 "zoid/%s_b.dat"
#undef lit_896
#define lit_896 "scope_joint"
#undef lit_897
#define lit_897 "f_base_joint"
#undef lit_898
#define lit_898 "b_base_joint"
#undef lit_899
#define lit_899 "base_joint"
#undef lit_900
#define lit_900 "R_glove_joint"
#undef lit_901
#define lit_901 "L_glove_joint"
#undef lit_902
#define lit_902 "R_shoe_joint"
#undef lit_903
#define lit_903 "L_shoe_joint"
#undef lit_904
#define lit_904 "senter_joint"
#undef lit_905
#define lit_905 "berut1_joint"
#undef lit_906
#define lit_906 "cap_joint"
#undef lit_907
#define lit_907 "collar_joint"
#undef lit_908
#define lit_908 "jacket_joint"
#undef lit_909
#define lit_909 "fly_joint"
#undef lit_910
#define lit_910 "to_fly_joint"
#undef lit_911
#define lit_911 "to_to_fly_joint"
#undef lit_912
#define lit_912 "to_to_to_fly_joint"
#undef lit_913
#define lit_913 "to_to_to_to_fly_joint"
#undef lit_914
#define lit_914 "to_to_to_to_to_fly_joint"
#undef lit_915
#define lit_915 "to_to_to_to_to_to_fly_joint"
#undef lit_916
#define lit_916 "to_to_to_to_to_to_to_fly_joint"
#undef lit_917
#define lit_917 "buckle_joint"
#undef lit_918
#define lit_918 "NAVEL_joint"
#undef lit_919
#define lit_919 "c_BODY_joint"
#undef lit_920
#define lit_920 "c_head_joint"
#undef lit_921
#define lit_921 "c_neck_joint"
#undef lit_922
#define lit_922 "c_bust_joint"
#undef lit_923
#define lit_923 "c_hip_joint"
#undef lit_924
#define lit_924 "c_tail_joint"
#undef lit_925
#define lit_925 "c_tail%d_joint"
#undef lit_926
#define lit_926 "c_l_arm1_joint"
#undef lit_927
#define lit_927 "c_l_arm2_joint"
#undef lit_928
#define lit_928 "c_r_arm1_joint"
#undef lit_929
#define lit_929 "c_r_arm2_joint"
#undef lit_930
#define lit_930 "c_l_leg1_joint"
#undef lit_931
#define lit_931 "c_l_leg2_joint"
#undef lit_932
#define lit_932 "c_r_leg1_joint"
#undef lit_933
#define lit_933 "c_r_leg2_joint"
#undef lit_934
#define lit_934 "c_l_foot_joint"
#undef lit_935
#define lit_935 "c_r_foot_joint"
#undef lit_936
#define lit_936 "joint spik [%s] is %s \n"
#undef lit_937
#define lit_937 "on "
#undef lit_938
#define lit_938 "off"
#undef lit_939
#define lit_939 "a_%1d_joint"
#undef lit_940
#define lit_940 "atack... read ! ok \n"
#undef lit_942
#define lit_942 "zoid/idp/zoid_%03d.prm"
#undef lit_943
#define lit_943 "zoid/%s.prm"
#undef lit_944
#define lit_944 "zoid/mdp/%s.cam"
#undef lit_945
#define lit_945 "zoid/mdp/%s.p_a"
#undef lit_946
#define lit_946 "jobjLook error \n"
#undef lit_947
#define lit_947 "===================zoid OK============\n"

s32 ZoidInit(ZoidState *zoid, s32 type)
{
    return ZoidInit_(zoid, type, 0);
}

s32 ZoidInitC(ZoidState *zoid, s32 type, s32 color)
{
    s32 ok = ZoidInit_(zoid, type, 0);
    if (ok == 0) {
        return 0;
    }
    ZoidSetAnim(zoid, 103);
    jobjAnim(zoid->joint);
    OSReport("zd color =%d \n", color);
    return ZoidColor(zoid, color, 0);
}
#undef lit_954
#define lit_954 "zd color =%d \n"
#define Zoid ((ZoidState *)Zoid)

void zdLoadSuperAtackWp(ZoidState *zoid)
{
    s32 index = 0;
    s32 type = zoid->type;
    s32 part;
    s32 slot;
    s32 w;
    slot = Tea_zoiddat[type].default_parts[index * 2 + 20];
    part = Tea_zoiddat[type].default_parts[index * 2 + 21];

    if (part > 0 && slot >= 0) {
        sWeaponMount(part, zoid, slot, 0x13);
        w = zoid->weapon_slots[slot].weapon;
        OSReport("mount [%s] -- ", zoid);
    }
}
#undef Zoid
#undef lit_960
#define lit_960 "mount [%s] -- "

void zdLoadOption1(ZoidState *zoid, s32 part)
{
    s32 type = zoid->type;
    s32 slot = Tea_zoiddat[type].option_one;

    if (slot <= 27) {
        zoid->option_type = ZOID_OPTION_SHIELD;
        zoid->option_energy_max = 2000;
        zoid->option_energy = 0;
        zoid->option_start_cost = 500;
        zoid->option_hold_cost = 2;
        zoid->option_recharge = 3;
        sWeaponMount(part, zoid, slot, WEAPON_MOUNT_SHIELD);
    }
}
void zdLoadOption2(ZoidState *zoid, s32 part)
{
    s32 slot;
    s32 type;

    type = zoid->type;
    slot = Tea_zoiddat[type].option_two;
    if (slot <= 27) {
        zoid->option_type = 2;
        zoid->option_energy_max = 2000;
        zoid->option_energy = 0;
        zoid->option_start_cost = 500;
        zoid->option_hold_cost = 2;
        zoid->option_recharge = 6;
        sWeaponMount(part, zoid, slot, 11);
    }
}

void zdLoadOption3(ZoidState *zoid)
{
    zoid->option_type = ZOID_OPTION_CAMOUFLAGE;
    zoid->option_energy_max = 2000;
    zoid->option_energy = 0;
    zoid->option_start_cost = 500;
    zoid->option_hold_cost = 1;
    zoid->option_recharge = 3;
}

void zdLoadOption4(ZoidState *zoid)
{
    zoid->option_type = ZOID_OPTION_SMOKE;
    zoid->option_energy_max = 300;
    zoid->option_energy = 0;
    zoid->option_start_cost = 60;
    zoid->option_hold_cost = 3;
    zoid->option_recharge = 1;
    sWeaponMount(277, zoid, 11, WEAPON_MOUNT_SMOKE_MINE);
}

void zdLoadOption5(ZoidState *zoid)
{
    zoid->option_type = ZOID_OPTION_MINE;
    zoid->option_energy_max = 500;
    zoid->option_energy = 0;
    zoid->option_start_cost = 500;
    zoid->option_hold_cost = 10;
    zoid->option_recharge = 1;
    sWeaponMount(399, zoid, 11, WEAPON_MOUNT_SMOKE_MINE);
}
extern void zdLoadOption2(ZoidState *zoid, s32 part);

void zdLoadOption(ZoidState *zoid, s32 part)
{
    u32 flags;
    s32 type;

    if (part != -1 && zoid != (void *)0 && zoid->joint != (void *)0) {
        zoid->field_1EA0 = part;
        flags = *(u32 *)((u8 *)&Tea_partsdat[part] + 0x64);
        zoid->booster_on = 0;
        type = ZOID_OPTION_NONE;
        if (flags & 1)
            type = ZOID_OPTION_CAMOUFLAGE;
        if (flags & 2)
            type = ZOID_OPTION_BOOSTER;
        if (flags & 4)
            type = ZOID_OPTION_SHIELD;
        if (flags & 0x10)
            type = ZOID_OPTION_MINE;
        if (flags & 0x20)
            type = ZOID_OPTION_SMOKE;
        zoid->option_type = type;
        if (zoid->option_type == ZOID_OPTION_SHIELD)
            zdLoadOption1(zoid, part);
        if (zoid->option_type == ZOID_OPTION_BOOSTER)
            zdLoadOption2(zoid, part);
        if (zoid->option_type == ZOID_OPTION_CAMOUFLAGE)
            zdLoadOption3(zoid);
        if (zoid->option_type == ZOID_OPTION_SMOKE)
            zdLoadOption4(zoid);
        if (zoid->option_type == ZOID_OPTION_MINE)
            zdLoadOption5(zoid);
        OSReport("option set!------ [no.%d] ---------\n", type);
    }
}
#undef lit_997
#define lit_997 "option set!------ [no.%d] ---------\n"
extern GameZoidV2 GameZoid[16];

void GameZoidInit(void)
{
    s32 i;
    for (i = 0; i < 16; i++) {
        GameZoid[i].type = -1;
    }
}
typedef struct ZoidInfoV2 {
    s32 type;
    s16 additional_parts[3];
    s16 health_bonus;
    u8 reserved_0C[0xC];
    s16 weapon_slots[10];
    s16 weapon_parts[10];
    u8 reserved_40[0xF];
    s8 color;
    s8 person;
} ZoidInfoV2;

s32 infoLoadZoid0(ZoidState *zoid, void *info)
{
    if (zoid == (void *)0) {
        return 0;
    }
    if (info == (void *)0) {
        return 0;
    }
    ZoidInit_(zoid, ((ZoidInfoV2 *)info)->type, 0);
    zoid->max_health += ((ZoidInfoV2 *)info)->health_bonus;
    zoid->health += ((ZoidInfoV2 *)info)->health_bonus;
    zoid->person = ((ZoidInfoV2 *)info)->person;
    ZoidColor(zoid, ((ZoidInfoV2 *)info)->color, 0);
    return 1;
}
typedef struct ZoidInfo1 {
    s32 type;
    s16 additional_parts[3];
    u8 reserved_0A[0xE];
    s16 weapon_slots[10];
    s16 weapon_parts[10];
    s8 weapon_colors[10];
    s16 option;
    u8 code_hi;
    u8 reserved_4D;
    u8 code_lo;
} ZoidInfo1;

extern void sWeaponMount(s32 part, ZoidState *zoid, s32 slot, s32 mount);
extern s32 zdWeaponColor(ZoidState *zoid, s32 slot, s32 color);

#define info ((ZoidInfo1 *)data)
s32 infoLoadZoid1(ZoidState *zoid, void *data)
{
    s32 i;
    s32 slot;
    s32 part;

    if (zoid == (void *)0) {
        return 0;
    }
    if (info == (void *)0) {
        return 0;
    }
    zoid->field_20A0 = info->code_lo + (info->code_hi << 8);
    for (i = 0; i < 10; i++) {
        slot = info->weapon_slots[i];
        part = info->weapon_parts[i];
        if (slot != 27 && slot != -1 && part > 0) {
            sWeaponMount(part, zoid, slot, i);
            zdWeaponColor(zoid, slot, info->weapon_colors[i]);
        }
    }
    zdLoadOption(zoid, info->option);
    for (i = 0; i < 3; i++) {
        zoid->additional_parts[i] = info->additional_parts[i];
    }
    return 1;
}
#undef info
typedef struct ZoidInfo {
    s32 type;
    s16 additional_parts[3];
    s16 health_bonus;
    u8 reserved_0C[0xC];
    s16 weapon_slots[10];
    s16 weapon_parts[10];
    s8 weapon_colors[10];
    u8 reserved_4A[0x2];
    u8 code_hi;
    u8 reserved_4D;
    u8 code_lo;
    s8 color;
    s8 person;
} ZoidInfo;

extern void sWeaponMount(s32 part, ZoidState *zoid, s32 slot, s32 mount);
extern s32 zdWeaponColor(ZoidState *zoid, s32 slot, s32 color);

#define info ((ZoidInfo *)data)
s32 infoLoadZoid(ZoidState *zoid, void *data)
{
    s32 i;
    s32 slot;
    s32 part;

    if (zoid == (void *)0)
        return 0;
    if (info == (void *)0)
        return 0;
    ZoidInit_(zoid, info->type, 0);
    zoid->max_health += info->health_bonus;
    zoid->health += info->health_bonus;
    zoid->person = info->person;
    zoid->field_20A0 = info->code_lo + (info->code_hi << 8);
    ZoidColor(zoid, info->color, 0);
    for (i = 0; i < 10; i++) {
        slot = info->weapon_slots[i];
        part = info->weapon_parts[i];
        if (slot != 27 && slot != -1 && part > 0) {
            sWeaponMount(part, zoid, slot, i);
            zdWeaponColor(zoid, slot, info->weapon_colors[i]);
        }
    }
    for (i = 0; i < 3; i++)
        zoid->additional_parts[i] = info->additional_parts[i];
    return 1;
}
#undef info
extern GameZoidV2 GameZoid[16];
extern void sWeaponMount(s32 part, ZoidState *zoid, s32 slot, s32 mount);
extern s32 zdWeaponColor(ZoidState *zoid, s32 slot, s32 color);
extern void zdLoadSuperAtackWp(ZoidState *zoid);
extern void zdLoadOption(ZoidState *zoid, s32 option);

void ZoidInit1(ZoidState *zoid, s32 index, s32 slot)
{
    s32 i;
    s32 type;
    s32 mount;
    s32 color;
    s32 part;

    if (zoid == 0)
        return;
    type = GameZoid[index].type;
    if (type == -1)
        return;
    ZoidInit(zoid, type);
    zoid->slot = slot;
    zoid->person = GameZoid[index].person;
    color = GameZoid[index].color;
    if (color != 0)
        ZoidColor(zoid, color, 0);
    for (i = 0; i < 28; i++) {
        if (zoid->weapon_slots[i].joint != 0) {
            part = GameZoid[index].weapons[i];
            if (part >= 0) {
                color = GameZoid[index].weapon_colors[i];
                for (mount = 0; mount < 10; mount++) {
                    if (part == Tea_zoiddat[type].mount_parts[mount])
                        break;
                }
                if (mount == 10)
                    mount = 9;
                sWeaponMount(part, zoid, i, mount);
                zdWeaponColor(zoid, i, color);
                OSReport("GameZoid %d WpColor=%d\n", type, color);
            }
        }
    }
    zdLoadSuperAtackWp(zoid);
    zdLoadOption(zoid, GameZoid[index].option);
    for (i = 0; i < 3; i++)
        zoid->additional_parts[i] = GameZoid[index].additional_parts[i];
}
#undef lit_1081
#define lit_1081 "GameZoid %d WpColor=%d\n"
#include "game_mode.h"

typedef struct MapUnitV2 {
    u8 pad_00[0x20];
    s32 kind;
    u8 pad_24[0xB0 - 0x24];
    s32 x;
    s32 heading;
    s32 z;
    s8 enabled;
    u8 pad_BD[3];
    s32 info;
    u8 pad_C4[0x10C - 0xC4];
    u8 f10C;
    u8 pad_10D;
    u8 f10E;
    u8 pad_10F[2];
    s8 field_111;
    u8 pad_112[0x24C - 0x112];
} MapUnitV2;
typedef struct MapGeneV2 {
    s8 active;
    u8 pad_01[3];
    s32 info;
    u8 pad_08[0x58 - 0x08];
    s32 x;
    s32 z;
    s32 frames;
    s32 count;
} MapGeneV2;
typedef struct MapInfoV2 {
    u8 pad_00[0x12C];
    MapUnitV2 units[17];
    u8 pad_2838[0x2C6C - 0x2838];
    MapGeneV2 genes[10];
} MapInfoV2;
extern MapInfoV2 mgI;
extern GameZoidV2 GameZoid[16];
extern void ZoidInit1(ZoidState *zoid, s32 index, s32 team);
extern s32 infoLoadZoid0(ZoidState *zoid, void *info);
extern s32 infoLoadZoid1(ZoidState *zoid, void *info);
extern s32 infoLoadZoid(ZoidState *zoid, void *info);
extern f32 zdColPosY(f32 *point, u16 *selector);
#define Zoid ((ZoidState *)Zoid)

void ZoidInitAll(void)
{
    ZoidState *zoid;
    ZoidState *zoid2;
    ZoidState *zoid3;
    s32 cnt;
    s32 slots[17];
    s32 start;
    s32 n;
    s32 team;
    MapUnitV2 *unit2;
    MapGeneV2 *gene;
    s32 type;
    s32 human;
    MapUnitV2 *unit;
    s32 result;
    s32 i;

    cnt = 0;
    if (GameMode.game_type == 9) {
        for (i = 0; i < 16; i++) {
            GameZoid[i].type = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        type = GameZoid[i].type;
        if (type == -1) {
            continue;
        }
        if (i < 5) {
            team = 0;
        } else {
            team = 1;
        }
        if (team == 1 && (GameMode.game_type == 1 || GameMode.game_type == 4)) {
            continue;
        }
        n = (i % 5) * 2 + team;
        ZoidInit1(&Zoid[n], i, team);
    }
    for (i = 0; i < 17; i++) {
        unit = &mgI.units[i];
        switch (unit->kind) {
        case 0:
        case 2:
            break;
        case 1:
        case 3:
        case 4:
            zoid = 0;
            if (unit->kind == 1) {
                zoid = 0;
                if (unit->enabled != 0) {
                    human = unit->field_111;
                    start = human + 1;
                    if (GameMode.game_type == 9) {
                        start &= 1;
                    }
                    for (n = start; n < 16; n += 2) {
                        if (Zoid[n].joint == (void *)0) {
                            break;
                        }
                    }
                    if (n >= 16) {
                        continue;
                    }
                    if (Zoid[n].joint != 0) {
                        continue;
                    }
                    zoid = &Zoid[n];
                    result = infoLoadZoid0(zoid, &unit->info);
                    zoid->field_20A0 = unit->f10E + unit->f10C * 256;
                    if (result == 0) {
                        slots[i] = -1;
                        zoid = 0;
                    } else {
                        slots[i] = n;
                    }
                    zoid->slot = (human + 1) & 1;
                    OSReport("zd=%d grp=%d\n", n, zoid->slot);
                }
            }
            if (unit->kind == 3) {
                zoid = &Zoid[cnt];
                cnt++;
            }
            if (unit->kind == 4) {
                zoid = &Zoid[2];
                Zoid[2].field_20A0 = unit->f10E + unit->f10C * 256;
            }
            if (zoid != 0) {
                zoid->position[0] = unit->x;
                zoid->position[2] = unit->z;
                zoid->position[1] = 100.0f;
                zoid->position[1] = 1.0f + zdColPosY(zoid->position, 0);
                zoid->heading = 2 * (3.1415926 * (f32)unit->heading) / 360.0;
            }
            break;
        }
    }
    for (i = 0; i < 17; i++) {
        unit2 = &mgI.units[i];
        switch (unit2->kind) {
        case 0:
            break;
        case 1:
            if (unit2->kind == 1 && unit2->enabled != 0) {
                n = slots[i];
                if (n != -1 && Zoid[n].joint != 0) {
                    zoid2 = &Zoid[n];
                    infoLoadZoid1(zoid2, &unit2->info);
                }
            }
            break;
        case 2:
            break;
        }
    }
    for (i = 0; i < 10; i++) {
        gene = &mgI.genes[i];
        if (gene->active == 0) {
            continue;
        }
        if (gene->count <= 0) {
            continue;
        }
        for (n = 1; n < 16; n += 2) {
            zoid3 = &Zoid[n];
            if (zoid3->joint == 0) {
                break;
            }
        }
        if (n >= 16) {
            break;
        }
        infoLoadZoid(zoid3, &gene->info);
        zoid3->gene_x = gene->x;
        team = mgI.units[0].kind;
        zoid3->gene_z = gene->z;
        zoid3->position[0] = gene->x;
        zoid3->position[2] = gene->z;
        zoid3->position[1] = 100.0f;
        zoid3->position[1] = 1.0f + zdColPosY(zoid3->position, 0);
        zoid3->field_20BC = gene->count - 1;
        zoid3->field_20B4 = gene->frames * 60;
        zoid3->field_20B8 = zoid3->field_20B4;
    }
    if (GameMode.game_type == 3) {
        if (GameMode.win == 0) {
            Zoid[1].field_20A0 = Zoid[1].person + 120;
        } else {
            Zoid[1].field_20A0 = Zoid[1].person + 260;
        }
    }
    zoid2 = zoid2;
    zoid2 = zoid2;
    zoid2 = zoid2;
    zoid2 = zoid2;
    zoid = zoid;
    zoid = zoid;
    zoid = zoid;
}
#undef Zoid
#undef lit_1187
#define lit_1187 "zd=%d grp=%d\n"
extern void *sReadFileU(char *name, s32 usb);

void zdVoiceScript(ZoidState *zoid)
{
    char path[100];
    if (zoid->joint != 0) {
        sprintf(path, "chara/v_sc/man_%02d.sc", zoid->person);
        zoid->field_0028 = sReadFileU(path, 0);
        OSReport("voice script [%s] %s \n", path, zoid->field_0028 != 0 ? "OK" : "GGG not dound");
    }
}
#undef lit_1198
#define lit_1198 "chara/v_sc/man_%02d.sc"
#undef lit_1199
#define lit_1199 "voice script [%s] %s \n"
#undef lit_1200
#define lit_1200 "OK"
#undef lit_1201
#define lit_1201 "GGG not dound"
extern void zdVoiceScript(ZoidState *zoid);

#define Zoid ((ZoidState *)Zoid)
void ZoidVoiceScript(void)
{
    s32 i;
    ZoidState *zoid;
    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint != 0)
            zdVoiceScript(zoid);
    }
}
#undef Zoid
#include "random.h"
#define Zoid ((ZoidState *)Zoid)

s32 zdVoiceScriptCalc(VoiceEntry *entry, ZoidState *zoid, s32 a, s32 b)
{
    s32 i;
    s32 type;
    s32 typeA;
    s32 typeB;
    s32 person;
    s32 personA;
    s32 personB;
    s32 f68;
    s32 f64;
    s8 *choices;
    s8 result;

    if (entry == 0) {
        return -1;
    }
    type = zoid->type;
    person = zoid->person;
    f68 = zoid->field_1E68;
    f64 = zoid->field_1E64;
    typeA = Zoid[a].type;
    typeB = Zoid[b].type;
    personA = Zoid[a].person;
    personB = Zoid[b].person;
    for (i = 0; i < 20; i++, entry++) {
        choices = entry->choices;
        switch (entry->kind) {
        case 0:
            return -1;
        case 1:
            break;
        case 2:
            if (entry->value == person) break;
            continue;
        case 3:
            if (entry->value == type) break;
            continue;
        case 4:
            if (entry->value == personA) break;
            continue;
        case 5:
            if (entry->value == typeA) break;
            continue;
        case 6:
            if (entry->value == personB) break;
            continue;
        case 7:
            if (entry->value == typeB) break;
            continue;
        case 8:
            if (entry->value != f68) continue;
            zoid->field_1E68 = -1;
            break;
        case 9:
            if (entry->value != f64) continue;
            zoid->field_1E64 = -1;
            break;
        default:
            continue;
        }
        if (entry->flags & 1) {
            result = choices[HSD_Randi(4)];
        } else {
            result = choices[0];
        }
        return result;
    }
    return -1;
}
#undef Zoid
#include "game_mode.h"
extern s32 zdVoiceScriptCalc(VoiceEntry *entry, ZoidState *zoid, s32 a, s32 b);
#define Zoid ((ZoidState *)Zoid)

void zdVoiceSc(ZoidState *zoid, s32 id)
{
    void *pack;
    VoiceEntry *script;
    VoiceEntry *entries;
    s32 flag;
    s32 team;
    s32 index;
    s32 i;
    ZoidState *other;
    s32 target;
    s32 voice;

    if (zoid == 0) {
        return;
    }
    if (zoid->joint == 0) {
        return;
    }
    pack = zoid->field_0028;
    if (pack == 0) {
        return;
    }
    script = (VoiceEntry *)binB(pack, id);
    if (script == 0) {
        OSReport("no script ---------[%d] \n", id);
    }
    if (script == 0) {
        return;
    }
    entries = script;
    flag = 0;
    if (GameMode.game_type == 2) {
        flag = 1;
    }
    if (GameMode.game_type == 3) {
        flag = 1;
    }
    if (flag == 0) {
        if (zoid == &Zoid[0]) {
            flag = 1;
        }
        if (zoid == &Zoid[2]) {
            flag = 1;
        }
    }
    if (flag == 0) {
        if (camTmp == 0) {
            return;
        }
        if (camTmp->zoid_distance[zoid->index] > 500.0f) {
            return;
        }
    }
    team = zoid->slot;
    index = -1;
    for (i = 0; i < 16; i++) {
        other = &Zoid[i];
        if (other != zoid && other->joint != 0 && team == other->slot) {
            index = i;
            break;
        }
    }
    target = zoid->target_slot;
    if (zoid->target_pending == 0) {
        target = -1;
    }
    if (target != -1 && Zoid[target].joint != 0) {
        target = -1;
    }
    voice = zdVoiceScriptCalc(entries, zoid, index, target);
    if (voice != -1) {
        camVoiceOn(zoid->person, voice);
    }
}
#undef Zoid
#undef lit_1296
#define lit_1296 "no script ---------[%d] \n"

void zdVoiceScG(ZoidState *zoid, s32 voice)
{
    if (gameEndF == 0) {
        zdVoiceSc(zoid, voice);
    }
}
typedef struct PartDataS {
    u8 pad_0000[0x50];
    s32 field_50;
    u8 pad_0054[0x4];
    s32 field_58;
    u8 pad_005C[0xC];
    s16 category;
    s16 armor_bonus;
    s16 power;
    u8 pad_006E[0x4];
    s16 radar_range_bonus;
    u8 pad_0074[0x70];
    s16 field_E4;
    s16 speed_bonus;
    s16 search_range_bonus;
    s16 turn_bonus;
    u8 pad_00EC[0x124 - 0xEC];
} PartDataS;

void zdCalcWeaponS(ZoidState *zoid, s32 part)
{
    if (part > 0) {
        PartDataS *data = (PartDataS *)&Tea_partsdat[part];
        zoid->field_20C4 += data->field_58;
        zoid->radar_range_bonus += data->radar_range_bonus;
        zoid->search_range_bonus += data->search_range_bonus;
        zoid->armor_bonus += data->armor_bonus;
        zoid->field_20D8 += data->field_E4;
        zoid->speed_bonus += data->speed_bonus;
        zoid->turn_bonus += data->turn_bonus;
        if (data->category == 3) {
            zoid->bullet_power_bonus += data->power;
            zoid->melee_power_bonus += data->power;
        }
        zoid->field_20F4 += data->field_50;
    }
}
extern void zdCalcWeaponS(ZoidState *zoid, s32 part);

void zdCalcWeapon(ZoidState *zoid, s32 weapon)
{
    Weapon3 *entry;

    if (zoid != 0 && weapon >= 0) {
        entry = &sWeapon[weapon];
        if (entry->active != 0) {
            zdCalcWeaponS(zoid, entry->type);
        }
    }
}
#include "game_person_data.h"

void zdCalcMan(ZoidState *zoid, s32 person)
{
    if (zoid != 0 && person >= 0) {
        PersonData *pilot = &Tea_persondat[person];
        zoid->turn_bonus += pilot->turn_bonus;
        zoid->melee_power_bonus += pilot->melee_power_bonus;
        zoid->bullet_power_bonus += pilot->bullet_power_bonus;
    }
}
void zdCalcPre(ZoidState *zoid)
{
    zoid->field_20C0 = 0;
    zoid->field_20C4 = 0;
    zoid->radar_range_bonus = 0;
    zoid->search_range_bonus = 0;
    *(s32 *)((u8 *)zoid + 0x1F6C) = 0;
    zoid->armor_bonus = 0;
    zoid->field_20D8 = 0;
    zoid->speed_bonus = 0;
    zoid->bullet_power_bonus = 0;
    zoid->melee_power_bonus = 0;
    zoid->turn_bonus = 0;
    zoid->field_20F4 = 0;
}
typedef struct ZoidDataE {
    u8 pad_000[0x34];
    u8 us_pad_0034[0x14];
    u8 pad_000_us[0x3C - 0x34];
    s32 field_003C;
    u8 pad_040[0x4];
    s32 field_0044;
    u8 pad_048[0xC];
    s16 armor;
    s16 speed;
    u8 pad_058[0x78A - 0x58];
    s16 turn;
    u8 pad_78C[0x4];
    s16 radar_range;
    s16 search_range;
    u8 pad_794[0x7E8 - 0x794];
} ZoidDataE;
typedef struct AnimDataE {
    u8 pad_000[0xBC];
    s32 speed;
} AnimDataE;

#define Zoid ((ZoidState *)Zoid)

void zdCalcEpi(ZoidState *zoid)
{
    ZoidDataE *data = &((ZoidDataE *)Tea_zoiddat)[zoid->type];
    AnimDataE *anim = (AnimDataE *)zoid->anim_data;

    zoid->field_1EF8 = (f32)(data->radar_range + zoid->radar_range_bonus);
    zoid->search_range = (f32)(data->search_range + zoid->search_range_bonus);
    zoid->armor = data->armor + zoid->armor_bonus;
    if (zoid->armor < 1) {
        zoid->armor = 1;
    }
    if (anim != 0) {
        zoid->field_1F04 = anim->speed + zoid->speed_bonus;
    } else {
        zoid->field_1F04 = data->speed + zoid->speed_bonus;
    }
    zoid->field_20F8 = data->field_003C + zoid->field_20F4;
    zoid->field_20C0 = data->field_0044 + zoid->field_20C4;
    if (zoid->field_20C0 < 0) {
        zoid->field_20C0 = 0;
    }
    zoid->field_20EC = data->turn + zoid->turn_bonus;
}
#undef Zoid
extern void zdCalcPre(ZoidState *zoid);
extern void zdCalcWeapon(ZoidState *zoid, s32 weapon);
extern void zdCalcWeaponS(ZoidState *zoid, s32 part);
extern void zdCalcMan(ZoidState *zoid, s32 person);
extern void zdCalcEpi(ZoidState *zoid);

void zdCalc(ZoidState *zoid)
{
    s32 i;

    if (zoid != 0 && zoid->joint != 0) {
        zdCalcPre(zoid);
        for (i = 0; i < 30; i++) {
            if (zoid->weapon_slots[i].joint != 0 && zoid->weapon_slots[i].weapon != -1) {
                zdCalcWeapon(zoid, zoid->weapon_slots[i].weapon);
            }
        }
        for (i = 0; i < 3; i++) {
            zdCalcWeaponS(zoid, zoid->additional_parts[i]);
        }
        if (zoid->field_1EA0 > 0) {
            zdCalcWeaponS(zoid, zoid->field_1EA0);
        }
        zdCalcMan(zoid, zoid->person);
        zdCalcEpi(zoid);
        OSReport("add power z%d \222e=%d , \212i\223\254=%d \n", zoid->index,
                 zoid->bullet_power_bonus, zoid->melee_power_bonus);
    }
}
#undef lit_1350
#define lit_1350 "add power z%d \x92" "e=%d , \x8ai\x93\xac=%d \n"
#define Zoid ((ZoidState *)Zoid)
void ZoidCalcAll(void)
{
    s32 i;
    for (i = 0; i < 16; i++) {
        zdCalc(&Zoid[i]);
    }
}
#undef Zoid
extern u8 IveSave[];
extern void zdCalcWeaponS(ZoidState *zoid, s32 part);
extern void zdCalcMan(ZoidState *zoid, s32 person);
extern void zdCalcPre(ZoidState *zoid);
extern void zdCalcEpi(ZoidState *zoid);

s32 zdCalcFarm(ZoidState *zoid, s32 slot, s32 a, s32 b)
{
    s32 index;
    s32 i;
    s32 w;
    u8 *base;

    if (zoid == 0)
        return 0;
    if (slot < 0)
        return 0;
    if (slot >= 4)
        slot = 0;
    base = IveSave + slot * 0x1a90;
    index = *(s16 *)(base + 0x15f6);
    if (a != -1)
        index = a;
    if (b == -2) {
        b = b;
    }
    if (b == -3) {
        b = b;
    }
    if (index < 0)
        index = 0;
    zoid->type = *(s16 *)(base + 4 + index * 2);
    zdCalcPre(zoid);
    for (i = 0; i < 10; i++) {
        w = *(s16 *)(base + 0xb0e + index * 0x38 + i * 2);
        if (w != -1)
            zdCalcWeaponS(zoid, w);
    }
    for (i = 0; i < 3; i++) {
        do { w = *(s16 *)(base + 0x11da + index * 6 + i * 2); } while (0);
        if (w != -1)
            zdCalcWeaponS(zoid, w);
    }
    if (*(s16 *)(base + 0x119e + index * 2) > 0)
        zdCalcWeaponS(zoid, *(s16 *)(base + 0x119e + index * 2));
    zdCalcMan(zoid, -1);
    zdCalcEpi(zoid);
    return index;
}
void zdCalcZoid(ZoidState *zoid, s32 type, s32 arg)
{
    ZoidData *data;
    s32 part;
    s32 i;
    s32 weapon;

    if (zoid == 0) {
        return;
    }
    zoid->type = type;
    data = &Tea_zoiddat[type];
    zdCalcPre(zoid);
    for (i = 0; i < 8; i++) {
        part = data->default_parts[i];
        if (part != -1) {
            zdCalcWeaponS(zoid, part);
        }
    }
    weapon = -1;
    if (data->weapon_permissions[8] != 0) {
        weapon = data->default_parts[8];
    }
    if (data->weapon_permissions[9] != 0) {
        weapon = data->default_parts[9];
    }
    if (data->weapon_permissions[10] != 0) {
        weapon = 0x115;
    }
    if (data->weapon_permissions[11] != 0) {
        weapon = 0x18E;
    }
    if (weapon > 0) {
        zdCalcWeaponS(zoid, weapon);
    }
    zdCalcMan(zoid, -1);
    zdCalcEpi(zoid);
    arg = arg;
}
#pragma force_active on
static const f64 lit_1423[1] = { 0.01 };
#pragma force_active off
#include "ground_collision.h"
f32 zdColPosY(f32 *point, u16 *selector)
{
    GroundMask mask;
    GroundMask tmp = {{7, 0, 0, 0}};
    mask = tmp;
    return sColPosAndY((Vec3 *)point, selector, &mask);
}
f32 zdPosY(ZoidState *zoid, u16 *selector)
{
    f32 pos[3];
    HSD_JObj *camera = zoid->camera_joint;
    s32 i;

    for (i = 0; i < 3; i++) {
        pos[i] = zoid->position[i];
    }
    if (camera != 0) {
        pos[1] = camera->matrix[1][3];
    }
    return zdColPosY(pos, selector);
}
#include "ground_collision.h"

extern f32 zdPosY(ZoidState *zoid, u16 *selector);

f32 zdGetBasePoly(ZoidState *zoid, f32 *point)
{
    GroundMask *mask = 0;
    Vec3 *normal = 0;
    u16 selector;
    GroundMask filter;
    GroundMask *cell;
    u32 material;
    f32 y;
    f32 result;

    y = zdPosY(zoid, &selector);
    if (selector != GROUND_POLYGON_NONE) {
        material = grdcPolygon[selector].material;
        cell = &grdcMat[material];
        mask = cell;
        if (zoid->ai_active == 0) {
            if (mask->bits[0] & 4) {
                zoid->field_1E74 = 1;
                if (zoid->death_started == 0)
                    zoid->death_started = 1;
            }
        } else {
            if (mask->bits[0] & 6)
                zoid->field_1E78++;
            else
                zoid->field_1E78 = 0;
        }
        if (mask->bits[2] & 2)
            zoid->field_1E7C = 1;
        normal = &grdcNormal[grdcPolygon[selector].normal_index];
    }
    zoid->ground_material = mask;
    result = y;
    zoid->ground_polygon = selector;
    zoid->ground_normal = (f32 *)normal;
    filter = (GroundMask){{8, 0, 0, 0}};
    y = sColPosAndY((Vec3 *)zoid->position, &selector, &filter);
    if (selector != GROUND_POLYGON_NONE) {
        material = grdcPolygon[selector].material;
        cell = &grdcMat[material];
        mask = cell;
        if (point[1] <= y) {
            if (mask->bits[0] & 8) {
                zoid->health = 0;
                zoid->knockback[1] = 3.0f;
                zoid->knockback[0] = 0.0f;
                zoid->knockback[2] = 0.0f;
            }
        }
    }
    return result;
}
extern void sGlobal2Local(HSD_JObj *joint);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *joint);

#pragma push
#pragma dont_inline off
static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_485, 605, lit_486);
    return !(jobj->flags & 0x00800000) && (jobj->flags & 0x40);
}

static inline void HSD_JObjSetMtxDirty(HSD_JObj *jobj)
{
    if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
        HSD_JObjSetMtxDirtySub(jobj);
    }
}

static inline void HSD_JObjSetScaleX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_485, 805, lit_486);
    jobj->scale.x = value;
    if (!(jobj->flags & 0x02000000)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}

static inline void HSD_JObjSetScaleY(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_485, 819, lit_486);
    jobj->scale.y = value;
    if (!(jobj->flags & 0x02000000)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}

static inline void HSD_JObjSetScaleZ(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_485, 833, lit_486);
    jobj->scale.z = value;
    if (!(jobj->flags & 0x02000000)) {
        HSD_JObjSetMtxDirty(jobj);
    }
}

void ZoidTo(ZoidState *zoid, ZoidLimb *limb, s32 ground)
{
    HSD_JObj *joint;
    f32 y;
    f32 *position;
    f32 saved[3];
    s32 i;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    if (limb == (void *)0)
        return;
    joint = limb->joint;
    if (joint == (void *)0)
        return;
    position = limb->position;
    for (i = 0; i < 3; i++)
        saved[i] = joint->matrix[i][3];
    for (i = 0; i < 3; i++)
        position[i] = joint->matrix[i][3];
    y = zdColPosY(position, 0);
    if (position[1] < y)
        position[1] = y;
    for (i = 0; i < 3; i++)
        joint->matrix[i][3] = position[i];
    sGlobal2Local(joint);
    HSD_JObjSetScaleX(joint, 1.0f);
    HSD_JObjSetScaleY(joint, 1.0f);
    HSD_JObjSetScaleZ(joint, 1.0f);
}
#pragma pop
extern void sLinier3d(f32 *current, f32 *target, f32 fraction);

#pragma push
#pragma dont_inline off
void ZoidToF(ZoidLimb *limb, ZoidLimb *target, f32 rate)
{
    HSD_JObj *joint;
    HSD_JObj *target_joint;
    f32 *to;
    f32 *from;
    s32 i;

    if (limb == (void *)0) {
        return;
    }
    if (target == (void *)0) {
        return;
    }
    joint = limb->joint;
    target_joint = target->joint;
    if (joint == (void *)0) {
        return;
    }
    if (target_joint == (void *)0) {
        return;
    }
    from = (f32 *)target;
    to = (f32 *)limb;
    sLinier3d(to, from, rate);
    for (i = 0; i < 3; i++) {
        joint->matrix[i][3] = to[i];
    }
    sGlobal2Local(joint);
    HSD_JObjSetScaleX(joint, 1.0f);
    HSD_JObjSetScaleY(joint, 1.0f);
    HSD_JObjSetScaleZ(joint, 1.0f);
}
#pragma pop
extern void sLinier3d(f32 *current, f32 *target, f32 fraction);

#pragma push
#pragma dont_inline off
void ZoidToDownF(ZoidLimb *limb, ZoidLimb *target, f32 rate)
{
    HSD_JObj *joint;
    HSD_JObj *target_joint;
    f32 *to;
    f32 *from;
    s32 k;

    if (limb == (void *)0)
        return;
    if (target == (void *)0)
        return;
    joint = limb->joint;
    target_joint = target->joint;
    if (joint == (void *)0)
        return;
    if (target_joint == (void *)0)
        return;
    to = target->position;
    from = limb->position;
    to[1] = zdColPosY(to, 0);
    sLinier3d(from, to, rate);
    for (k = 0; k < 3; k++)
        joint->matrix[k][3] = from[k];
    sGlobal2Local(joint);
    HSD_JObjSetScaleX(joint, 1.0f);
    HSD_JObjSetScaleY(joint, 1.0f);
    HSD_JObjSetScaleZ(joint, 1.0f);
}
#pragma pop
extern s32 gameStartTm;

typedef struct FlyLimbs {
    u8 pad_000[0x4C];
    ZoidLimb limbs[9];
} FlyLimbs;

#define Zoid ((ZoidState *)Zoid)

void ZoidToFly(ZoidState *zoid)
{
    s32 flag;
    f32 rate;
    s32 i;

    flag = 0;
    rate = 0.1f;
    if (zoid->action >= 0xD && zoid->action <= 0x16) {
        flag = 1;
    }
    if (flag != 0) {
        rate = 1.0f;
    }
    if (gameStartTm < 4 && zoid->action == 0x32) {
        rate = 1.0f;
    }
    ZoidTo(zoid, &((FlyLimbs *)zoid)->limbs[8], flag);
    if (zoid->death_pending != 0) {
        ZoidToDownF(&((FlyLimbs *)zoid)->limbs[1], &((FlyLimbs *)zoid)->limbs[8], rate);
    } else {
        ZoidToF(&((FlyLimbs *)zoid)->limbs[1], &((FlyLimbs *)zoid)->limbs[8], rate);
    }
    for (i = 2; i < 8; i++) {
        if (zoid->death_pending != 0) {
            ZoidToDownF(&((FlyLimbs *)zoid)->limbs[i], &((FlyLimbs *)zoid)->limbs[i - 1], rate);
        } else {
            ZoidToF(&((FlyLimbs *)zoid)->limbs[i], &((FlyLimbs *)zoid)->limbs[i - 1], rate);
        }
    }
}
#undef Zoid
#pragma push
#pragma dont_inline off
#pragma auto_inline off
static inline void HSD_JObjSetTranslateX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_485, 952, lit_486);
    jobj->translation.x = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
            HSD_JObjSetMtxDirtySub(jobj);
        }
    }
}

static inline void HSD_JObjSetTranslateY(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_485, 966, lit_486);
    jobj->translation.y = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
            HSD_JObjSetMtxDirtySub(jobj);
        }
    }
}

static inline void HSD_JObjSetTranslateZ(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_485, 980, lit_486);
    jobj->translation.z = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
            HSD_JObjSetMtxDirtySub(jobj);
        }
    }
}

static inline void HSD_JObjSetRotationX(HSD_JObj *jobj, f32 x)
{
    jobj ? (void)0 : __assert(lit_485, 676, lit_486);
    !(jobj->flags & 0x20000) ? (void)0 : __assert(lit_485, 677, "!(jobj->flags & JOBJ_USE_QUATERNION)");
    jobj->rotation[0] = x;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
            HSD_JObjSetMtxDirtySub(jobj);
        }
    }
}

static inline void HSD_JObjSetRotationY(HSD_JObj *jobj, f32 y)
{
    jobj ? (void)0 : __assert(lit_485, 696, lit_486);
    !(jobj->flags & 0x20000) ? (void)0 : __assert(lit_485, 697, "!(jobj->flags & JOBJ_USE_QUATERNION)");
    jobj->rotation[1] = y;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
            HSD_JObjSetMtxDirtySub(jobj);
        }
    }
}

static inline void HSD_JObjSetRotationZ(HSD_JObj *jobj, f32 z)
{
    jobj ? (void)0 : __assert(lit_485, 716, lit_486);
    !(jobj->flags & 0x20000) ? (void)0 : __assert(lit_485, 717, "!(jobj->flags & JOBJ_USE_QUATERNION)");
    jobj->rotation[2] = z;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
            HSD_JObjSetMtxDirtySub(jobj);
        }
    }
}

static inline f32 absf(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

void zdSetXYZ(ZoidState *zoid)
{
    HSD_JObjSetTranslateX(zoid->joint, zoid->position[0]);
    HSD_JObjSetTranslateY(zoid->joint, zoid->position[1] + zoid->height_offset);
    HSD_JObjSetTranslateZ(zoid->joint, zoid->position[2]);
    HSD_JObjSetRotationY(zoid->joint, zoid->heading);
    HSD_JObjSetRotationX(zoid->joint, zoid->high_angle);
}
#pragma pop
#undef lit_1627
#define lit_1627 "!(jobj->flags & JOBJ_USE_QUATERNION)"
void *getAnimAobj(HSD_JObj *joint)
{
    void *aobj;

    if (joint == (void *)0) {
        return 0;
    }
    if (joint->animation != 0) {
        return joint->animation;
    }
    for (joint = joint->child; joint != 0; joint = joint->next) {
        aobj = getAnimAobj(joint);
        if (aobj != 0) {
            return aobj;
        }
    }
    return 0;
}

void setAnimRate(HSD_JObj *joint, f32 value)
{
    if (joint != (void *)0) {
        if (joint->animation != 0) {
            HSD_AObjSetRate(joint->animation, value);
        }
        for (joint = joint->child; joint != 0; joint = joint->next) {
            setAnimRate(joint, value);
        }
    }
}

void setAnimFrame(HSD_JObj *joint, f32 value)
{
    if (joint != (void *)0) {
        if (joint->animation != 0) {
            HSD_AObjSetCurrentFrame(joint->animation, value);
        }
        for (joint = joint->child; joint != 0; joint = joint->next) {
            setAnimFrame(joint, value);
        }
    }
}

void setAnimStop(HSD_JObj *joint)
{
    setAnimRate(joint, 0.0f);
}

s32 zdGetAnimNum(ZoidState *zoid, HSD_JObj *joint)
{
    JointPose *pose;
    joint = jobjWithAobj(joint);
    pose = stSearchJobj(zoid->joint, zoid->pose, joint);
    if (pose == (void *)0)
        return -1;
    return pose->animation_index;
}
extern void HSD_JObjRemoveAnim(HSD_JObj *joint);
extern void HSD_JObjAddAnim(HSD_JObj *joint, HSD_AnimJoint *animation, HSD_MatAnimJoint *material, HSD_ShapeAnimJoint *shape);

void zdHSD_JObjAddAnim(HSD_JObj *joint, JointPose *pose, s32 number,
                       HSD_AnimJoint *animation, HSD_MatAnimJoint *material,
                       HSD_ShapeAnimJoint *shape)
{
    if (joint != (void *)0) {
        HSD_JObjRemoveAnim(joint);
        HSD_JObjAddAnim(joint, animation, material, shape);
        pose->animation_index = number;
    }
}
struct HSD_AnimJoint {
    struct HSD_AnimJoint *child, *next;
    void *aobjdesc;
    void *robj_anim;
    u32 flags;
};
struct HSD_MatAnimJoint {
    struct HSD_MatAnimJoint *child, *next;
    void *matanim;
};
struct HSD_ShapeAnimJoint {
    struct HSD_ShapeAnimJoint *child, *next;
    void *shapeanimdobj;
};

extern void zdHSD_JObjAddAnim(HSD_JObj *joint, JointPose *pose, s32 number,
                             HSD_AnimJoint *animation, HSD_MatAnimJoint *material,
                             HSD_ShapeAnimJoint *shape);

#define Zoid ((ZoidState *)Zoid)

void zdHSD_JObjAddAnimAll(HSD_JObj *joint, JointPose *pose, s32 number,
                         HSD_AnimJoint *animation, HSD_MatAnimJoint *material,
                         HSD_ShapeAnimJoint *shape)
{
    if (joint != (void *)0) {
        zdHSD_JObjAddAnim(joint, pose, number, animation, material, shape);
        joint = joint ? joint->child : 0;
        animation = animation ? animation->child : 0;
        pose = pose ? pose->child : 0;
        material = material ? material->child : 0;
        shape = shape ? shape->child : 0;
        while (joint != 0 && animation != 0) {
            zdHSD_JObjAddAnimAll(joint, pose, number, animation, material, shape);
            joint = joint->next;
            animation = animation ? animation->next : 0;
            pose = pose ? pose->next : 0;
            material = material ? material->next : 0;
            shape = shape ? shape->next : 0;
        }
    }
}
#undef Zoid
void zdHSD_JObjAddAnimAll2s(HSD_JObj *joint, JointPose *pose, s32 number, s32 animation, s32 material, s32 shape)
{
    if (joint != (void *)0) {
        joint = joint ? joint->child : 0;
        animation = (u32)animation ? *(s32 *)animation : 0;
        pose = pose ? pose->child : 0;
        material = (u32)material ? *(s32 *)material : 0;
        shape = (u32)shape ? *(s32 *)shape : 0;
        zdHSD_JObjAddAnimAll(joint, pose, number, (HSD_AnimJoint *)animation, (HSD_MatAnimJoint *)material,
                             (HSD_ShapeAnimJoint *)shape);
    }
}
typedef struct AnimTablesVS3 {
    s32 pad_0000;
    s32 *table_a;
    s32 *table_b;
    s32 *table_c;
} AnimTablesVS3;

typedef struct AnimRefVS3 {
    AnimTablesVS3 *tables;
} AnimRefVS3;

typedef struct AnimLinkVS3 {
    AnimRefVS3 *ref;
} AnimLinkVS3;

typedef struct AnimSetVS3 {
    u8 pad_0000[0x24];
    AnimLinkVS3 *link;
} AnimSetVS3;

extern void zdHSD_JObjAddAnimAll2s(HSD_JObj *joint, JointPose *pose, s32 index, s32 a, s32 b, s32 c);

#define Zoid ((ZoidState *)Zoid)

void sAddAnimAllXor2ANIM(ZoidState *zoid, s32 index)
{
    AnimLinkVS3 *link;
    s32 *table_a;
    s32 *table_b;
    s32 *table_c;
    AnimSetVS3 *set;

    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    set = (AnimSetVS3 *)zoid->field_0038;
    if (set == 0) {
        return;
    }
    link = set->link;
    if (link == (void *)0) {
        return;
    }
    table_a = link->ref->tables->table_a;
    table_b = link->ref->tables->table_b;
    table_c = link->ref->tables->table_c;
    if (index > 0x65) {
        index = 0;
    }
    zdHSD_JObjAddAnimAll2s(zoid->joint, zoid->pose, index,
                           table_a != 0 ? table_a[index] : 0,
                           table_b != 0 ? table_b[index] : 0,
                           table_c != 0 ? table_c[index] : 0);
}
#undef Zoid
typedef struct ZoidAnimRyV2 {
    u8 pad_0000[0x1D04];
    f32 child_matrix[3][4];
    f32 heading_delta;
} ZoidAnimRyV2;

void zdAddAnimRy(ZoidState *zoid)
{
    HSD_JObj *joint;
    s32 i;

    if (zoid != (void *)0) {
        joint = zoid->joint;
        if (joint != (void *)0 && (joint = joint->child) != (void *)0) {
            for (i = 0; i < 3; i++) {
                zoid->position[i] = ((ZoidAnimRyV2 *)zoid)->child_matrix[i][3];
            }
            zoid->heading += ((ZoidAnimRyV2 *)zoid)->heading_delta;
            zoid->height_offset = 0.0f;
        }
    }
}

void zdClearAtkTouch(ZoidState *zoid)
{
    s32 i, j;
    for (i = 0; i < 10; i++) {
        ZoidAttackObject *attack = &zoid->attack_objects[i];
        if (attack != (void *)0 && attack->joint != 0) {
            for (j = 0; j < 16; j++)
                attack->touched[j] = 0;
        }
    }
}
extern void camSeTm(void *camera, s16 id, Vec3 *position, s32 volume);
extern void zdEffectMount(ZoidState *zoid, s32 mount, s32 effect);

void zdAnimEffect(ZoidState *zoid, s32 animation)
{
    s32 effect = Tea_zoiddat[zoid->type].animation_effects[animation];
    s32 mount = Tea_zoiddat[zoid->type].animation_effect_mounts[animation];
    s32 sound = Tea_zoiddat[zoid->type].animation_sounds[animation];
    s32 volume = ((s16 *)Tea_zoiddat[zoid->type].reserved_0238)[animation];

    if (sound > 0 && gameEndF <= 2) {
        camSeTm(camTmp, sound, (Vec3 *)zoid->position, volume);
    }
    if (effect < 0x88 && effect > 0 && mount < 0x1B && mount >= 0) {
        zdEffectMount(zoid, mount, effect);
    }
}
extern void zdAnimEffect(ZoidState *zoid, s32 anim);
extern void zdAddAnimRy(ZoidState *zoid);
extern void sAddAnimAllXor2ANIM(ZoidState *zoid, s32 animation);
extern void HSD_JObjReqAnimAll(HSD_JObj *joint, f32 frame);
extern void ZoidAnim(ZoidState *zoid);

#pragma push
#pragma dont_inline off
static inline f32 HSD_AObjGetEndFrame(HSD_AObj *aobj)
{
    aobj ? (void)0 : __assert("aobj.h", 171, "aobj");
    return aobj->end_frame;
}

void ZoidSetAnim(ZoidState *zoid, s32 animation)
{
    void *data;
    s32 current;
    ModelAnimationTable *animations;
    HSD_AObj *aobj;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    if (animation < 0) {
        OSReport("zoid anim setAnim=(%d)error \n", animation);
        return;
    }
    switch (animation) {
    case 1:
    case 2:
    case 10:
    case 11:
    case 12:
        break;
    default:
        zoid->field_2060 = 0;
    }
    zoid->field_1C48 = 0;
    if (animation > 0x65)
        animation = 0;
    switch (animation) {
    case 0x43:
        zoid->attack_index = 0;
        break;
    case 0x45:
        zoid->attack_index = 1;
        break;
    case 0x47:
        zoid->attack_index = 2;
        break;
    case 0x49:
        zoid->attack_index = 3;
        break;
    case 0x4B:
        zoid->attack_index = 4;
        break;
    case 0x41:
        zoid->attack_index = 5;
        break;
    case 0x42:
        zoid->attack_index = 5;
        break;
    case 0x3E:
        zoid->attack_index = 7;
        break;
    default:
        zoid->attack_index = -1;
        break;
    }
    zdAnimEffect(zoid, animation);
    data = zoid->anim_data;
    zdClearAtkTouch(zoid);
    current = zoid->current_animation;
    if (zdsMotTBL[current] & 8)
        zdAddAnimRy(zoid);
    zoid->interpolation_remaining = 15;
    zoid->interpolation_duration = 15;
    zoid->field_1C40 = 0;
    zoid->current_animation = animation;
    animations = zoid->animations;
    (void)-1.0f;
    if (animations->models != (void *)0 && animations->models[0]->joint_animations != (void *)0) {
        sAddAnimAllXor2ANIM(zoid, animation);
        zoid->aobj = (HSD_AObj *)getAnimAobj(zoid->joint->child);
        if (zoid->aobj == 0) {
            zoid->animation_end_frame = 0.0f;
        } else {
            aobj = zoid->aobj;
            zoid->animation_end_frame = HSD_AObjGetEndFrame(aobj);
        }
        zoid->animation_stopped = 0;
        HSD_JObjReqAnimAll(zoid->joint, 0.0f);
        zoid->field_1A40[0] = 0.0f;
        zoid->field_1A40[1] = 0.0f;
        zoid->field_1A40[2] = 0.0f;
        setAnimRate(zoid->joint->child, 1.0f);
        ZoidAnim(zoid);
    }
}
#pragma pop
#undef lit_1692
#define lit_1692 "aobj.h"
#undef lit_1693
#define lit_1693 "aobj"
#undef lit_1813
#define lit_1813 "zoid anim setAnim=(%d)error \n"
#pragma force_active on
u32 wpTbl[42] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 
    0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000002, 0x00000002, 
    0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000001, 
    0x00000002, 0x00000002, 0x00000002, 0x00000000, 0x00000001, 0x00000001, 0x00000001, 0x00000002, 
    0x00000001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 
};
#pragma force_active off

void zdLinierTm(ZoidState *zoid, s32 duration)
{
    zoid->interpolation_remaining = duration;
    zoid->interpolation_duration = duration;
}
#pragma push
#pragma dont_inline off
#pragma auto_inline off
void zdClearAnimRy(HSD_JObj *joint, u32 flags)
{
    if (joint != (void *)0 && (joint = joint->child) != (void *)0) {
        if (flags & 4)
            HSD_JObjSetRotationY(joint, 0.f);
        if (flags & 0x10)
            HSD_JObjSetTranslateY(joint, 0.f);
        if (flags & 0x20)
            HSD_JObjSetTranslateX(joint, 0.f);
        if (flags & 0x40)
            HSD_JObjSetTranslateZ(joint, 0.f);
    }
}
#pragma pop
#pragma push
#pragma dont_inline off
#pragma auto_inline off
void zdAtkTransOFF(ZoidState *zoid)
{
    HSD_JObj *joint;
    f32 x;
    f32 y;
    f32 z;

    if (zoid != 0 && zoid->joint != 0) {
        joint = zoid->joint->child;
        if (joint != 0) {
            if (zoid->ground_turn == 0)
                return;
            zoid->ground_turn--;
            x = zoid->field_1A40[0];
            HSD_JObjSetTranslateX(joint, x);
            y = zoid->field_1A40[1];
            HSD_JObjSetTranslateY(joint, y);
            z = zoid->field_1A40[2];
            HSD_JObjSetTranslateZ(joint, z);
        }
    }
}
#pragma pop
#pragma force_active on
static const f64 lit_1883[1] = { 0.001 };
namespace { __declspec(section ".sdata2") extern const f32 lit_1885[1] = { 30.0f }; }
static const f64 lit_1886[1] = { 60.0 };
#pragma force_active off
namespace { extern const f32 fps60[1]; }
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *joint);
extern void sSetRxRzF(ZoidState *zoid, FootJoint *front, FootJoint *back, f32 *rx, f32 *rz);
extern void sLinear1D(f32 *value, f32 target, f32 step);

#pragma push
#pragma dont_inline off
void calcBaseF(ZoidState *zoid)
{
    ZoidControlData *control;
    u8 *info;
    f32 rx;
    f32 rz;
    f32 step;
    f32 limit;

    if (zoid && zoid->joint != 0 && zoid->f_base_joint != 0) {
        info = (u8 *)zoid->slider_info;
        control = (ZoidControlData *)zoid->anim_data;
        if (info != 0) {
            step = 2.0 * (3.1415926 * ((f32) * (u16 *)(info + 0x702) / fps60[0])) / 360.0;
            limit = 2.0 * (3.1415926 * (f32) * (u16 *)(info + 0x704)) / 360.0;
            if (control->field_00F8 == 1) {
                HSD_JObjSetRotationX(zoid->f_base_joint, zoid->ground_rx);
                HSD_JObjSetRotationZ(zoid->f_base_joint, zoid->ground_rz);
            } else {
                if (control->field_00F4 != 2) {
                    sSetRxRzF(zoid, &zoid->feet[1], &zoid->feet[0], &rx, &rz);
                    if (rx > limit) {
                        rx = limit;
                    }
                    if (rz > limit) {
                        rz = limit;
                    }
                    if (rx < -limit) {
                        rx = -limit;
                    }
                    if (rz < -limit) {
                        rz = -limit;
                    }
                    sLinear1D(&zoid->f_base_rx, rx, step);
                    sLinear1D(&zoid->f_base_rz, rz, step);
                    HSD_JObjSetRotationX(zoid->f_base_joint, zoid->f_base_rx);
                    HSD_JObjSetRotationZ(zoid->f_base_joint, zoid->f_base_rz);
                } else {
                    HSD_JObjSetRotationX(zoid->f_base_joint, zoid->ground_rx);
                    HSD_JObjSetRotationZ(zoid->f_base_joint, zoid->ground_rz);
                }
            }
        }
    }
}
#pragma pop

extern u32 HSD_AObjGetFlags(HSD_AObj *aobj);
extern void setAnimStop(HSD_JObj *joint);
extern void HSD_JObjAnimAll(HSD_JObj *joint);
extern void zdAtkTransOFF(ZoidState *zoid);
extern void zdAnimEffect(ZoidState *zoid, s32 anim);
extern void zdClearAnimRy(HSD_JObj *joint, u32 flags);

#pragma push
#pragma dont_inline off
static inline f32 HSD_AObjGetCurrentFrame(HSD_AObj *aobj)
{
    aobj ? (void)0 : __assert(lit_1692, 149, lit_1693);
    return aobj->current_frame;
}

void ZoidAnim(ZoidState *zoid)
{
    u32 *motion = zdsMotTBL;
    s32 loop;
    HSD_JObj *joint;
    f32 frame;

    if (zoid != (void *)0 && zoid->joint != (void *)0 && zoid->animation_stopped == 0 && zoid->death_pending == 0) {
        if (zoid->aobj == 0) {
            frame = 0.0f;
        } else {
            frame = HSD_AObjGetCurrentFrame(zoid->aobj);
        }
        joint = zoid->joint;
        HSD_JObjAnimAll(joint);
        zdAtkTransOFF(zoid);
        zoid->animation_stopped = 0;
        if (zoid->aobj == 0) {
            zoid->anim_current_frame = 0.0f;
        } else {
            zoid->anim_current_frame = HSD_AObjGetCurrentFrame(zoid->aobj);
        }
        zoid->animation_looped = 0;
        loop = 0;
        if (HSD_AObjGetFlags(zoid->aobj) & 0x04000000) {
            if (zoid->current_animation == 5 || zoid->current_animation == 4) {
                zdClearAtkTouch(zoid);
            }
            zoid->animation_looped = 1;
            loop = 1;
            if (!(motion[zoid->current_animation] & 2)) {
                zdAnimEffect(zoid, zoid->current_animation);
            }
        }
        if (loop != 0 && (motion[zoid->current_animation] & 2)) {
            setAnimRate(zoid->joint, 1.0f);
            setAnimFrame(zoid->joint, frame);
            setAnimStop(zoid->joint);
            setAnimRate(zoid->joint, 0.0f);
            HSD_JObjAnimAll(zoid->joint);
            zdAtkTransOFF(zoid);
            zoid->animation_stopped = 1;
        }
        zoid->child_translation_y = HSD_JObjGetTranslationY(zoid->joint->child);
        if (zoid->ground_normal_count != 0) {
            zoid->field_1C40 = 0;
        } else {
            zoid->field_1C40++;
        }
        if (zoid->child_translation_y > 0.3) {
            zoid->movement_flags = (zoid->movement_flags << 1) | 1;
            zoid->field_1C48 = 1;
        }
        if (zoid->child_translation_y < lit_1423[0]) {
            zoid->movement_flags = zoid->movement_flags << 1;
        }
        zoid->movement_flags = (u8)zoid->movement_flags;
        if (motion[zoid->current_animation] & 0x80) {
            zdClearAnimRy(zoid->joint, motion[zoid->current_animation]);
        }
        zdSetXYZ(zoid);
    }
}
#pragma pop

void zdLinier(ZoidState *zoid)
{
    zdSetXYZ(zoid);
    if (zoid->interpolation_remaining != 0 && zoid->interpolation_duration != 0) {
        f32 fraction = ((f32)zoid->interpolation_duration - (f32)zoid->interpolation_remaining)
                       / (f32)zoid->interpolation_duration;

        ZoidLinerAnimNextXor(zoid->joint->child->child, zoid->pose->child->child, fraction, zoid);
        zoid->interpolation_remaining--;
    }
    if (zoid->interpolation_remaining < 0) {
        zoid->interpolation_remaining = 0;
    }
    if (zoid->interpolation_duration < 0) {
        zoid->interpolation_duration = 0;
    }
}

JointPose *zdSearchBAK(HSD_JObj *joint, JointPose *pose, HSD_JObj *target)
{
    JointPose *found;

    if (joint == (void *)0) {
        return 0;
    }
    if (pose == (void *)0) {
        return 0;
    }
    if (joint == target) {
        return pose;
    }
    joint = joint ? joint->child : 0;
    pose = pose ? pose->child : 0;
    while (joint != 0) {
        found = zdSearchBAK(joint, pose, target);
        if (found != 0) {
            return found;
        }
        joint = joint ? joint->next : 0;
        pose = pose ? pose->next : 0;
    }
    return 0;
}

void zdLinierReset(ZoidState *zoid, HSD_JObj *joint)
{
    JointPose *pose;
    JointPose *search_pose;
    HSD_JObj *search_joint;
    if (joint != (void *)0) {
        search_pose = zoid->pose;
        search_joint = zoid->joint;
        pose = zdSearchBAK(search_joint, search_pose, joint);
        if (pose != (void *)0)
            ZoidCopyAnim1(joint, pose);
    }
}

void sGlobal2Local(HSD_JObj *joint)
{
    Mtx local;

    if (joint->parent != 0)
        HSD_MtxInverseConcat(joint->parent->matrix, joint->matrix, local);
    else
        PSMTXCopy(joint->matrix, local);
    HSD_MtxGetTranslate(local, &joint->translation);
    HSD_MtxGetRotation(local, (Vec3 *)&joint->rotation);
    HSD_MtxGetScale(local, &joint->scale);
}

void ik3dAnim(ZoidState *zoid, FootJoint *foot)
{
    register HSD_JObj *joint;
    register s32 n;

    if (zoid == (ZoidState *)0)
        return;
    if (foot == (FootJoint *)0)
        return;
    n = 0;
    joint = foot->joint;
    if (joint == (HSD_JObj *)0)
        return;
    foot->height = joint->matrix[1][3];
}

void zdFootBak(ZoidState *zoid)
{
    ik3dAnim(zoid, &zoid->feet[2]);
    ik3dAnim(zoid, &zoid->feet[3]);
    ik3dAnim(zoid, &zoid->feet[0]);
    ik3dAnim(zoid, &zoid->feet[1]);
}
#include "ground_collision.h"

extern void sGlobal2Local(HSD_JObj *joint);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *joint);
extern f32 zdColPosY(f32 *point, u16 *selector);
extern void PSMTXRotRad(f32 m[3][4], s8 axis, f32 rad);
extern void PSMTXMultVecSR(f32 m[3][4], Vec3 *src, Vec3 *dst);
extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
extern f32 angLimitAbs(f32 value, f32 limit);

#pragma push
#pragma dont_inline off
static inline void ik3dSetTranslateX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_485, 952, lit_486);
    jobj->translation.x = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
            HSD_JObjSetMtxDirtySub(jobj);
        }
    }
}

static inline f32 ik3dGetRotationX(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_485, 748, lit_486);
    return jobj->rotation[0];
}

static inline f32 ik3dGetRotationY(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_485, 762, lit_486);
    return jobj->rotation[1];
}

static inline f32 ik3dGetRotationZ(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_485, 776, lit_486);
    return jobj->rotation[2];
}

static inline f32 ik3dFabsf(f32 *v)
{
    return (*(u32 *)v &= 0x7FFFFFFF, *v);
}

void ik3d(ZoidState *zoid, FootJoint *foot)
{
    Vec3 point;
    f32 ry;
    f32 ty;
    u16 selector;
    Vec3 normal;
    Vec3 front;
    f32 m[3][4];
    f32 diff;
    f32 (*mtx)[4];
    HSD_JObj *joint;
    Vec3 *n;
    f32 offset;
    f32 y;
    f32 val;
    f32 zero_a;
    f32 zero_b;
    s32 mode;
    s32 hit;
    s32 i;

    (void)0.3;
    hit = 0;
    if (foot == (void *)0)
        return;
    foot->flags &= ~FOOT_FLAG_CONTACT;
    joint = foot->joint;
    if (joint == (void *)0)
        return;
    mode = zoid->current_animation;
    if (mode == 2)
        mode = 1;
    offset = foot->offset;
    mtx = joint->matrix;
    ty = HSD_JObjGetTranslationY(joint);
    point.x = mtx[0][3];
    point.y = foot->height - offset;
    point.z = mtx[2][3];
    y = zdColPosY(&point.x, &selector);
    if (y > zoid->field_1AA8) {
        selector = GROUND_POLYGON_NONE;
        y = point.y - 10.0f;
    }
    if (zoid->field_1DD4 != 0)
        mtx[1][3] = (mtx[1][3] + (y + offset)) / 2.0f;
    foot->ground_polygon_index = selector;
    foot->ground = y;
    foot->depth = y - point.y;
    if (zoid->death_pending != 0)
        return;
    zero_a = 0.0f;
    zero_b = 0.0f;
    ry = ik3dGetRotationY(joint);
    if (mode != 1) {
        if (foot->field_2C < 0.0f)
            foot->field_2C += 0.1;
        if (foot->field_2C >= 0.0f)
            foot->field_2C = 0.0f;
    }
    if (y >= point.y)
        hit = 1;
    diff = y - mtx[1][3];
    if (ik3dFabsf(&diff) < 0.06)
        hit = 1;
    if (y > mtx[1][3])
        hit = 1;
    if (hit == 0) {
        foot->rot_z = 1.0f;
        foot->rot_x = 1.0f;
    }
    foot->no_contact_frames++;
    if (hit != 0) {
        zoid->grounded = 1;
        foot->no_contact_frames = 0;
        val = 0.002 + (y + offset);
        mtx[1][3] = val;
        foot->flags |= FOOT_FLAG_CONTACT | FOOT_FLAG_SUPPORTED;
        if (selector != GROUND_POLYGON_NONE) {
            n = &grdcNormal[grdcPolygon[selector].normal_index];
            zoid->ground_normal_count++;
            for (i = 0; i < 3; i++)
                zoid->ground_normal_sum[i] += (&n->x)[i];
            normal.x = n->x;
            normal.y = n->y;
            normal.z = n->z;
            front.x = mtx[0][1];
            front.y = mtx[1][1];
            front.z = mtx[2][1];
            PSMTXRotRad(m, 'Y', -zoid->heading);
            PSMTXMultVecSR(m, &normal, &normal);
            PSMTXMultVecSR(m, &front, &front);
            if (normal.x < front.x) {
                if (foot->rot_z < 0.0f)
                    foot->rot_z = -0.95 * foot->rot_z;
                if (foot->rot_z < lit_1883[0])
                    foot->rot_z = 0.001f;
                PSMTXRotRad(m, 'Z', 0.017453292f * foot->rot_z);
                PSMTXConcat(mtx, m, mtx);
            }
            if (normal.x > front.x) {
                if (foot->rot_z > 0.0f)
                    foot->rot_z = -0.95 * foot->rot_z;
                if (foot->rot_z > -0.001)
                    foot->rot_z = -0.001f;
                PSMTXRotRad(m, 'Z', 0.017453292f * foot->rot_z);
                PSMTXConcat(mtx, m, mtx);
            }
            if (normal.z > front.z) {
                if (foot->rot_x < 0.0f)
                    foot->rot_x = -0.95 * foot->rot_x;
                if (foot->rot_x < lit_1883[0])
                    foot->rot_x = 0.001f;
                PSMTXRotRad(m, 'X', 0.017453292f * foot->rot_x);
                PSMTXConcat(mtx, m, mtx);
            }
            if (normal.z < front.z) {
                if (foot->rot_x > 0.0f)
                    foot->rot_x = -0.95 * foot->rot_x;
                if (foot->rot_x > -0.001)
                    foot->rot_x = -0.001f;
                PSMTXRotRad(m, 'X', 0.017453292f * foot->rot_x);
                PSMTXConcat(mtx, m, mtx);
            }
            foot->field_14 = zero_a;
            foot->field_18 = zero_b;
        }
        ik3dSetTranslateX(joint, 0.0f);
        sGlobal2Local(joint);
        zero_a = ik3dGetRotationX(joint);
        zero_b = ik3dGetRotationZ(joint);
        zero_a = angLimitAbs(zero_a, 0.7853981f);
        zero_b = angLimitAbs(zero_b, 0.7853981f);
        HSD_JObjSetRotationX(joint, zero_a);
        HSD_JObjSetRotationZ(joint, zero_b);
    } else {
        if (mtx[1][3] > point.y + zoid->senter_translation_y) {
            mtx[1][3] = point.y + zoid->senter_translation_y;
            ik3dSetTranslateX(joint, 0.0f);
            sGlobal2Local(joint);
        }
    }
    if (foot->no_contact_frames > 15) {
        if (foot->no_contact_frames > 600)
            foot->no_contact_frames = 600;
        foot->flags &= ~FOOT_FLAG_SUPPORTED;
    }
}
#pragma pop
#pragma push
#pragma dont_inline off
void ik3dOn(ZoidState *zoid, FootJoint *foot)
{
    HSD_JObj *joint;
    f32 (*matrix)[4];
    f32 offset;
    f32 y;
    Vec3 point;
    u16 selector;

    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    joint = foot->joint;
    if (joint == (void *)0) {
        return;
    }
    matrix = joint->matrix;
    offset = foot->offset;
    point.x = matrix[0][3];
    point.y = foot->height - offset;
    point.z = matrix[2][3];
    y = zdColPosY(&point.x, &selector);
    if (y > zoid->field_1AA8) {
        y = point.y - 10.0f;
        selector = GROUND_POLYGON_NONE;
    }
    foot->ground_polygon_index = selector;
    foot->ground = y;
    if (zoid->death_pending != 0) {
        return;
    }
    matrix[1][3] = (y + offset) - 10.0f;
    foot->depth = 0.0f;
    ik3dSetTranslateX(foot->joint, 0.0f);
    sGlobal2Local(foot->joint);
}
#pragma pop
extern f32 *grdcGetNormal(u16 selector);

void sSetRxRzF(ZoidState *zoid, FootJoint *front, FootJoint *back, f32 *rx, f32 *rz)
{
    f32 position[3];
    u16 polygon;
    Vec3 *normal;
    s32 i;

    if (back->joint == (void *)0) {
        return;
    }
    for (i = 0; i < 3; i++) {
        position[i] = (front->joint->matrix[i][3] + back->joint->matrix[i][3]) / 2.0f;
    }
    zdColPosY(position, &polygon);
    normal = (Vec3 *)grdcGetNormal(polygon);
    if (normal == (void *)0) {
        return;
    }
    sColGetRxRz(normal, zoid->heading, rx, rz);
}

void chkDeep(FootJoint *foot, s32 *found, f32 *minimum)
{
    if (!(foot->depth > 0.0f)) return;
    *found = 1;
    if (foot->depth < *minimum) {
        *minimum = foot->depth;
    }
}
void chkDeepCo(void *coobj, s32 *found, f32 *depth)
{
    f32 d;
    if (*(HSD_JObj **)coobj != 0) {
        d = *(f32 *)((u8 *)coobj + 0x94) - (*(f32 *)((u8 *)coobj + 0x98) - *(f32 *)((u8 *)coobj + 0x08));
        if (d > 0.0f) {
            *found = 1;
            if (d > *depth)
                *depth = d;
        }
    }
}
#pragma push
#pragma dont_inline off
void calcBaseB(ZoidState *zoid)
{
    ZoidControlData *control;
    u16 *slider;

    if (zoid && zoid->joint != 0 && (control = (ZoidControlData *)zoid->anim_data) != 0 && (slider = zoid->slider_info) != 0 && zoid->b_base_joint != 0) {
        if (control->field_00F8 == 1) {
            HSD_JObjSetRotationX(zoid->b_base_joint, zoid->ground_rx);
            HSD_JObjSetRotationZ(zoid->b_base_joint, zoid->ground_rz);
        } else if (control->field_00F4 != 0) {
            HSD_JObjSetRotationX(zoid->b_base_joint, zoid->ground_rx);
            HSD_JObjSetRotationZ(zoid->b_base_joint, zoid->ground_rz);
        }
    }
    slider = slider;
}
#pragma pop
namespace { __declspec(section ".sdata2") extern const f32 fps60[1] = { 60.0f }; }
#include "ground_collision.h"

#define DEG(x) (2.0 * (3.1415926 * (x)) / 360.0)

extern f32 zdGetBasePoly(ZoidState *zoid, f32 *point);
extern f32 zdColPosY(f32 *point, u16 *selector);
extern void sLinier3d(f32 *current, f32 *target, f32 fraction);
extern void sLinear1D(f32 *value, f32 target, f32 step);
extern void chkDeep(FootJoint *foot, s32 *found, f32 *minimum);
extern void chkDeepCo(void *coobj, s32 *found, f32 *depth);
extern f32 sLenPoint3dR(f32 *point);
extern void zdSetXYZ(ZoidState *zoid);
extern void calcBaseF(ZoidState *zoid);
extern void calcBaseB(ZoidState *zoid);
extern void ik3d(ZoidState *zoid, FootJoint *foot);

void ZoidPutXZ(ZoidState *zoid)
{
    u16 poly;
    HSD_JObj *joint;
    f32 rx;
    f32 rz;
    s32 lifted;
    f32 dead;
    f32 deep;
    s32 found;
    f32 cur[3];
    f32 prev[3];
    f32 back[3];
    f32 top[3];
    f32 test[3];
    f32 pos[3];
    u16 selector;
    Vec3 *normal;
    u8 unused0[20];
    s32 landed;
    u8 *slider;
    s32 count;
    f32 rate;
    f32 low;
    f32 diff;
    f32 dx;
    f32 add;
    f32 add2;
    f32 sum;
    f32 limit;
    f32 y;
    f32 len;
    f32 hit;
    f32 frac;
    f32 probe;
    s32 mode;
    u8 *control;
    HSD_JObj *jobj;
    GroundPolygon *polygon;
    s32 any;
    u8 unused1[16];
    s32 supported;
    s32 all_supported;
    s32 any2;
    s32 i;
    s32 j;
    s32 m;
    s32 k;
    s32 n;

    landed = 0;
    joint = zoid->joint;
    lifted = 0;
    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    control = (u8 *)zoid->anim_data;
    if (control == 0)
        return;
    slider = (u8 *)zoid->slider_info;
    if (slider == 0)
        return;
    rate = DEG((f32)((f32) * (u16 *)(slider + 0x706) / lit_1886[0]));
    limit = DEG((f32) * (u16 *)(slider + 0x708));
    mode = zoid->current_animation;
    dead = 2.0 + 2.0f * zoid->senter_translation_y;
    if (mode == 2)
        mode = 1;
    y = zdGetBasePoly(zoid, zoid->position);
    poly = zoid->ground_polygon;
    zoid->ground_height = y;
    jobj = zoid->field_1A7C;
    if (jobj != 0) {
        for (i = 0; i < 3; i++) {
            cur[i] = zoid->position[i];
            prev[i] = zoid->field_1C24[i];
            top[i] = jobj->matrix[i][3];
        }
        for (k = 0; k < 10; k++) {
            frac = 0.1 * (10 - k);
            for (i = 0; i < 3; i++) {
                back[i] = prev[i];
                test[i] = top[i];
            }
            sLinier3d(back, cur, frac);
            for (i = 0; i < 3; i++)
                test[i] -= cur[i] - back[i];
            hit = zdColPosY(test, 0);
            if (!(hit > test[1]))
                break;
        }
        for (i = 0; i < 3; i++)
            zoid->position[i] = back[i];
        for (j = 0; j < 3; j++)
            pos[j] = jobj->matrix[j][3];
        probe = zdColPosY(pos, &selector);
        if (selector != -1)
            polygon = &grdcPolygon[selector];
        else
            polygon = 0;
        if (polygon != 0)
            normal = &grdcNormal[polygon->normal_index];
        else
            normal = 0;
        rx = probe - jobj->matrix[1][3];
        if (rx > 1.0f)
            rx = 0.0f;
        if (rx > 0.0f) {
            if (zoid->death_pending != 0 && rx > 0.025)
                rx = 0.025f;
            zoid->position[1] += rx;
        }
    }
    diff = y - zoid->position[1];
    if (diff < zoid->field_19FC - 0.2) {
        lifted = 1;
        diff = zoid->field_19FC - 0.2;
        y = zoid->position[1] + diff;
    }
    low = y;
    if (*(s32 *)(control + 0xF8) != 0) {
        any = 0;
        if (*(s32 *)(control + 0xF8) == 2) {
            if (mode == 4 || mode == 5 || mode == 6)
                any = 1;
        } else {
            any = 1;
        }
        if (any) {
            zoid->height_offset = 0.0f;
            landed = 1;
            if (zoid->position[1] < y)
                zoid->position[1] = y;
        }
    }
    if (zoid->field_1C34 == 0) {
        found = 0;
        deep = 100.0f;
        sum = 0.0f;
        count = 0;
        supported = 0;
        all_supported = 1;
        if (landed == 0) {
            if (*(s32 *)(control + 0xF4) != 2) {
                if (zoid->feet[0].joint != 0) {
                    chkDeep(&zoid->feet[0], &found, &deep);
                    if (low > zoid->feet[0].ground)
                        low = zoid->feet[0].ground;
                    count++;
                    sum += zoid->feet[0].ground;
                    if (!(zoid->feet[0].flags & FOOT_FLAG_SUPPORTED))
                        all_supported = 0;
                    else
                        supported++;
                }
                if (zoid->feet[1].joint != 0) {
                    chkDeep(&zoid->feet[1], &found, &deep);
                    if (low > zoid->feet[1].ground)
                        low = zoid->feet[1].ground;
                    count++;
                    sum += zoid->feet[1].ground;
                    if (!(zoid->feet[1].flags & FOOT_FLAG_SUPPORTED))
                        all_supported = 0;
                    else
                        supported++;
                }
            }
            if (zoid->feet[2].joint != 0) {
                chkDeep(&zoid->feet[2], &found, &deep);
                if (low > zoid->feet[2].ground)
                    low = zoid->feet[2].ground;
                count++;
                sum += zoid->feet[2].ground;
                if (!(zoid->feet[2].flags & FOOT_FLAG_SUPPORTED))
                    all_supported = 0;
                else
                    supported++;
            }
            if (zoid->feet[3].joint != 0) {
                chkDeep(&zoid->feet[3], &found, &deep);
                if (low > zoid->feet[3].ground)
                    low = zoid->feet[3].ground;
                count++;
                sum += zoid->feet[3].ground;
                if (!(zoid->feet[3].flags & FOOT_FLAG_SUPPORTED))
                    all_supported = 0;
                else
                    supported++;
            }
            if (zoid->feet[2].joint != 0 && zoid->feet[3].joint != 0) {
                if (zoid->feet[2].depth < 0.0f && zoid->feet[3].depth < 0.0f)
                    found = 0;
            }
            if (count < 1) {
                (void)0;
            } else {
                sum = sum / count;
            }
            if (*(s32 *)(control + 0x104)) {
                (void)0;
            }
            add = 0.0002f;
            any2 = 0;
            if (all_supported == 0) {
                if (found == 0 || !(deep > 0.2))
                    any2 = 1;
            }
            if (zoid->feet[3].joint != 0 && !(zoid->feet[3].flags & FOOT_FLAG_CONTACT))
                any2 = 1;
            if (zoid->feet[2].joint != 0 && !(zoid->feet[2].flags & FOOT_FLAG_CONTACT))
                any2 = 1;
            if (any2) {
                if (zoid->height_offset_speed >= 0.0f)
                    zoid->height_offset_speed = 0.0f;
                if (mode == 4 || mode == 5)
                    add *= 0.1;
                zoid->height_offset_speed -= add;
                if (count - supported >= 2)
                    zoid->height_offset_speed -= add;
            }
            if (zoid->height_offset_speed > lit_1423[0])
                zoid->height_offset_speed = 0.01f;
            if (zoid->height_offset_speed < -0.01)
                zoid->height_offset_speed = -0.01f;
            zoid->height_offset += zoid->height_offset_speed;
        }
        if (found != 0) {
            add2 = 0.0f;
            if (deep > lit_1423[0])
                add2 = 0.0001f;
            if (deep > 0.1)
                add2 = 0.0007f;
            if (deep > 0.3)
                add2 = 0.002f;
            if (deep > 0.7)
                add2 = 0.007f;
            if (deep > 1.0f)
                add2 = 0.01f;
            if (zoid->field_1DD4 != 0)
                add2 = deep / 10.0;
            zoid->height_offset += add2;
            zoid->height_offset_speed = 0.0f;
        }
        found = 0;
        deep = 0.0f;
        chkDeepCo(zoid->bust_coobj, &found, &deep);
        if (slider[0x70B] & 4) {
            chkDeepCo(zoid->hip_coobj, &found, &deep);
            chkDeepCo(zoid->neck_coobj, &found, &deep);
        }
        if (found != 0 && (slider[0x70B] & 4))
            zoid->height_offset += deep;
        if (zoid->field_1DD4 != 0)
            zoid->field_1DD4--;
        if (mode > 3 && landed != 0)
            supported = 1;
        if (supported == 0 || lifted != 0) {
            zoid->field_1BFC++;
            if (zoid->field_1360 != 0)
                zoid->field_1BFC = 0;
            if ((slider[0x70F] & 4) && *(void **)(zoid->bust_coobj + 0x18) != 0)
                zoid->field_1BFC = 0;
            if ((slider[0x70F] & 8) && *(void **)(zoid->hip_coobj + 0x18) != 0)
                zoid->field_1BFC = 0;
            if (zoid->field_1BFC > *(s32 *)(control + 0xE4) && zoid->field_1BE4 == 0
                && *(s32 *)(control + 0xF8) == 0 && zoid->action != 2 && zoid->action != 1) {
                zoid->action = 1;
                zoid->jump_phase = 32;
                zoid->velocity[0] = 0.0f;
                zoid->velocity[1] = -0.001f;
                zoid->velocity[2] = 0.0f;
                zoid->field_1BFC = 0;
                zoid->action = 2;
                if (0.0f == zoid->displacement[1])
                    zoid->displacement[1] = -0.001f;
                len = sLenPoint3dR(zoid->displacement);
                if (len > 0.7) {
                    for (n = 0; n < 3; n++)
                        zoid->displacement[n] *= 0.7 / len;
                }
                return;
            }
        } else {
            zoid->field_1BFC = 0;
        }
        if (zoid->height_offset < low - y - 2.0f)
            zoid->height_offset = low - y - 2.0f;
        zoid->position[1] = y;
    }
    if (y > zoid->ground_height && zoid->height_offset < 0.0f) {
        dx = y - zoid->ground_height;
        if (dx > -zoid->height_offset) {
            y += zoid->height_offset;
            zoid->height_offset = 0.0f;
        } else {
            y = zoid->ground_height;
            zoid->height_offset += dx;
        }
        zoid->position[1] = y;
    }
    rx = 0.0f;
    rz = 0.0f;
    if (poly != GROUND_POLYGON_NONE)
        sColGetRxRz(&grdcNormal[grdcPolygon[poly].normal_index], zoid->heading, &rx, &rz);
    if (rx > limit)
        rx = limit;
    if (rz > limit)
        rz = limit;
    if (rx < -limit)
        rx = -limit;
    if (rz < -limit)
        rz = -limit;
    if (*(s32 *)(control + 0xF8) == 1) {
        if (zoid->position[1] > 2.0f + zoid->ground_height) {
            rx = 0.0f;
            rz = 0.0f;
        }
    }
    sLinear1D(&zoid->ground_rx, rx, rate);
    sLinear1D(&zoid->ground_rz, rz, rate);
    zdSetXYZ(zoid);
    calcBaseF(zoid);
    calcBaseB(zoid);
    zdSetXYZ(zoid);
    sMakeMatrix(zoid->joint);
    zoid->ground_normal_count = 0;
    for (m = 0; m < 3; m++)
        zoid->ground_normal_sum[m] = 0.0f;
    if (*(s32 *)(control + 0xF4) != 2) {
        ik3d(zoid, &zoid->feet[0]);
        ik3d(zoid, &zoid->feet[1]);
    }
    ik3d(zoid, &zoid->feet[2]);
    ik3d(zoid, &zoid->feet[3]);
    if (zoid->field_1BFC < 0)
        zoid->field_1BFC = 0;
    (void)sum;
    (void)sum;
    (void)y;
}

void ZoidPutLandingXZ(ZoidState *zoid)
{
    HSD_JObj *joint;
    f32 rx;
    f32 rz;
    f32 dx;
    f32 dz;
    f32 ground_sum;
    f32 weight_sum;
    f32 y;
    f32 low;
    u16 selector;
    ZoidControlData *control;
    s32 count;
    s32 i;

    joint = zoid->joint;
    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    control = (ZoidControlData *)zoid->anim_data;
    if (control == 0) {
        return;
    }
    y = zdGetBasePoly(zoid, zoid->position);
    selector = zoid->ground_polygon;
    count = 0;
    zoid->ground_height = y;
    low = y;
    weight_sum = 0.0f;
    ground_sum = 0.0f;
    if (control->field_00F4 != 2) {
        if (zoid->feet[0].joint != 0) {
            weight_sum += 1.0f;
            ground_sum += zoid->feet[0].ground;
            if (zoid->feet[0].flags & FOOT_FLAG_SUPPORTED) {
                count++;
            }
            if (low > zoid->feet[0].ground) {
                low = zoid->feet[0].ground;
            }
        }
        if (zoid->feet[1].joint != 0) {
            weight_sum += 1.0f;
            ground_sum += zoid->feet[1].ground;
            if (zoid->feet[1].flags & FOOT_FLAG_SUPPORTED) {
                count++;
            }
            if (low > zoid->feet[1].ground) {
                low = zoid->feet[1].ground;
            }
        }
    }
    if (zoid->feet[2].joint != 0) {
        weight_sum += 1.0f;
        ground_sum += zoid->feet[2].ground;
        if (zoid->feet[2].flags & FOOT_FLAG_SUPPORTED) {
            count++;
        }
        if (low > zoid->feet[2].ground) {
            low = zoid->feet[2].ground;
        }
    }
    if (zoid->feet[3].joint != 0) {
        weight_sum += 1.0f;
        ground_sum += zoid->feet[3].ground;
        if (zoid->feet[3].flags & FOOT_FLAG_SUPPORTED) {
            count++;
        }
        if (low > zoid->feet[3].ground) {
            low = zoid->feet[3].ground;
        }
    }
    rx = 0.0f;
    rz = 0.0f;
    if (selector != GROUND_POLYGON_NONE) {
        sColGetRxRz(&grdcNormal[grdcPolygon[selector].normal_index], zoid->heading, &rx, &rz);
    }
    dx = rx - zoid->ground_rx;
    dz = rz - zoid->ground_rz;
    if (dx < 0.5235987666666666) {
        if (dx > lit_1423[0]) {
            dx = 0.01f;
        }
    } else {
        dx = 0.1f;
    }
    if (dx > -0.5235987666666666) {
        if (dx < -0.01) {
            dx = -0.01f;
        }
    } else {
        dx = -0.1f;
    }
    zoid->ground_rx += dx;
    zoid->ground_rz += dz;
    if (zoid->position[1] < low - 1.0f) {
        zoid->position[1] = low - 1.0f;
    }
    if (zoid->position[1] < y) {
        zoid->height_offset += zoid->position[1] - y;
        zoid->position[1] = y;
        if (zoid->height_offset < -0.1) {
            zoid->height_offset += lit_1423[0];
        }
    }
    zdSetXYZ(zoid);
    sMakeMatrix(zoid->joint);
    zoid->ground_normal_count = 0;
    for (i = 0; i < 3; i++) {
        zoid->ground_normal_sum[i] = 0.0f;
    }
    if (control->field_00F4 != 2) {
        ik3d(zoid, &zoid->feet[0]);
        ik3d(zoid, &zoid->feet[1]);
    }
    ik3d(zoid, &zoid->feet[2]);
    ik3d(zoid, &zoid->feet[3]);
    zoid->foot_contacts = count;
    if (count != 0) {
        if ((zoid->jump_phase & 0xF0) == 32) {
            zoid->field_1DD4 = 60;
        }
    }
}
#pragma push
#pragma dont_inline off
void ZoidPutJumpingXZ(ZoidState *zoid)
{
    ZoidControlData *control;
    HSD_JObj *joint;
    s32 i;

    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    control = (ZoidControlData *)zoid->anim_data;
    if (zoid->height_offset < 0.0f) {
        zoid->height_offset += 0.02;
    }
    if (zoid->height_offset > 0.0f) {
        zoid->height_offset -= 0.02;
    }
    joint = zoid->joint;
    zdSetXYZ(zoid);
    if (zoid->f_base_joint != (void *)0 && control->field_00F4 == 4) {
        if (zoid->f_base_rx > 0.0f) {
            zoid->f_base_rx -= 5 * 3.1415926 / 180.0;
        }
        if (zoid->f_base_rx < 0.0f) {
            zoid->f_base_rx += 5 * 3.1415926 / 180.0;
        }
        if (zoid->f_base_rz > 0.0f) {
            zoid->f_base_rz -= 5 * 3.1415926 / 180.0;
        }
        if (zoid->f_base_rz < 0.0f) {
            zoid->f_base_rz += 5 * 3.1415926 / 180.0;
        }
        HSD_JObjSetRotationX(zoid->f_base_joint, zoid->f_base_rx);
        HSD_JObjSetRotationZ(zoid->f_base_joint, zoid->f_base_rz);
    }
    if (zoid->b_base_joint != (void *)0 && control->field_00F4 != 0) {
        if (zoid->ground_rx > 0.0f) {
            zoid->ground_rx -= 5 * 3.1415926 / 180.0;
        }
        if (zoid->ground_rx < 0.0f) {
            zoid->ground_rx += 5 * 3.1415926 / 180.0;
        }
        if (zoid->ground_rz > 0.0f) {
            zoid->ground_rz -= 5 * 3.1415926 / 180.0;
        }
        if (zoid->ground_rz < 0.0f) {
            zoid->ground_rz += 5 * 3.1415926 / 180.0;
        }
        HSD_JObjSetRotationX(zoid->b_base_joint, zoid->ground_rx);
        HSD_JObjSetRotationZ(zoid->b_base_joint, zoid->ground_rz);
    }
    zdSetXYZ(zoid);
    sMakeMatrix(zoid->joint);
    zoid->ground_normal_count = 0;
    for (i = 0; i < 3; ) {
        zoid->ground_normal_sum[i++] = 0.0f;
    }
    if (control->field_00F4 != 2) {
        ik3d(zoid, &zoid->feet[0]);
        ik3d(zoid, &zoid->feet[1]);
    }
    ik3d(zoid, &zoid->feet[2]);
    ik3d(zoid, &zoid->feet[3]);
}
#pragma pop
extern void angLimit(f32 *angle);

#define Zoid ((ZoidState *)Zoid)
void ZoidAllAnim(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (Zoid[i].joint != (void *)0) {
            angLimit(&Zoid[i].heading);
            ZoidCopyAnim(&Zoid[i]);
            ZoidAnim(&Zoid[i]);
        }
    }
}
#undef Zoid
#define Zoid ((ZoidState *)Zoid)

void ZoidAllPutXZ(void)
{
    s32 i;
    s32 state;
    s32 group;
    ZoidState *zoid;
    u8 *anim;

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint != (void *)0) {
            anim = (u8 *)zoid->anim_data;
            if (anim == 0 || *(s32 *)(anim + 0xF8) == 3)
                continue;
            state = zoid->jump_phase;
            group = state & 0xF0;
            if (state == 0) {
                ZoidPutXZ(&Zoid[i]);
            } else if (state <= 3 && zoid->field_1C90[2]) {
                ZoidPutJumpingXZ(&Zoid[i]);
            } else if (group == 16) {
                ZoidPutJumpingXZ(&Zoid[i]);
            } else if (group == 32) {
                ZoidPutLandingXZ(&Zoid[i]);
            } else if (group == 48) {
                ZoidPutLandingXZ(&Zoid[i]);
            } else {
                ZoidPutLandingXZ(&Zoid[i]);
            }
        }
    }
}
#undef Zoid
namespace { extern const f64 lit_2740[1]; }
extern void stSetFlag(HSD_JObj *joint, JointPose *pose, f32 alpha);
extern void sWeaponSetPer(ZoidState *zoid, f32 alpha);

void zdTrans1(ZoidState *zoid)
{
    stSetFlag(zoid->joint, zoid->pose, *(f32 *)((u8 *)zoid + 0x1DC4));
    sWeaponSetPer(zoid, *(f32 *)((u8 *)zoid + 0x1DC4));
    if (lit_2740[0] == *(f32 *)((u8 *)zoid + 0x1DC4))
        *(s32 *)((u8 *)zoid + 0x1DC0) &= ~1;
    else
        *(s32 *)((u8 *)zoid + 0x1DC0) |= 1;
}
void ZoidTrans(ZoidState *zoid, f32 transformation)
{
    *(f32 *)((u8 *)zoid + 0x1DC4) = transformation;
}

void zdSetMatStel(ZoidState *zoid)
{
    if (zoid != (ZoidState *)0 && zoid->joint != (HSD_JObj *)0) {
        ZoidCopyMatSet(zoid, steMobj);
    }
}

void zdSetMatStelOFF(ZoidState *zoid)
{
    if (zoid != (ZoidState *)0 && zoid->joint != (HSD_JObj *)0) {
        ZoidCopyMatRet(zoid);
    }
}
#pragma force_active on
namespace { __declspec(section ".sdata2") extern const f64 lit_2740[1] = { 1.0 }; }
#pragma force_active off
void zdStelON(ZoidState *zoid)
{
    s32 state;
    f32 t;

    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    state = zoid->stel_state;
    if (state == ZOID_STEL_OFF) {
        ZoidTrans(zoid, 1.0f);
        zoid->stel_state = ZOID_STEL_FADING;
    } else if (state == ZOID_STEL_FADING) {
        t = *(f32 *)((u8 *)zoid + 0x1DC4);
        t -= 0.04;
        if (t < 0.0f) {
            t = 0.0f;
            zoid->stel_state = ZOID_STEL_ON;
        }
        ZoidTrans(zoid, t);
    }
}
namespace { extern const f64 lit_2740[1]; }

void zdStelOFF(ZoidState *zoid)
{
    s32 state;
    f32 t;

    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    state = zoid->stel_state;
    if (state == ZOID_STEL_ON) {
        ZoidTrans(zoid, 0.0f);
        zoid->stel_state = ZOID_STEL_FADING;
    } else if (state == ZOID_STEL_FADING) {
        t = *(f32 *)((u8 *)zoid + 0x1DC4);
        t += 0.02;
        if (t > lit_2740[0]) {
            t = 1.0f;
            zoid->stel_state = ZOID_STEL_OFF;
        }
        ZoidTrans(zoid, t);
    }
}
extern void HSD_JObjSetFlags(HSD_JObj *joint, u32 flags);

void zdCoObjOFF(void *object)
{
    HSD_JObj *joint;
    if (object != (void *)0) {
        joint = *(HSD_JObj **)object;
        if (joint != (void *)0)
            HSD_JObjSetFlags(joint, 0x10);
    }
}

void ZoidCoObjOFF(ZoidState *zoid)
{
    s32 i;

    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    zdCoObjOFF(&zoid->head_coobj);
    zdCoObjOFF(&zoid->neck_coobj);
    zdCoObjOFF(&zoid->bust_coobj);
    zdCoObjOFF(&zoid->hip_coobj);
    for (i = 0; i < 8; i++) {
        zdCoObjOFF(&zoid->tail_coobjs[i]);
    }
    zdCoObjOFF(&zoid->l_arm1_coobj);
    zdCoObjOFF(&zoid->l_arm2_coobj);
    zdCoObjOFF(&zoid->r_arm1_coobj);
    zdCoObjOFF(&zoid->r_arm2_coobj);
    zdCoObjOFF(&zoid->l_leg1_coobj);
    zdCoObjOFF(&zoid->l_leg2_coobj);
    zdCoObjOFF(&zoid->r_leg1_coobj);
    zdCoObjOFF(&zoid->r_leg2_coobj);
    zdCoObjOFF(&zoid->l_foot_coobj);
    zdCoObjOFF(&zoid->r_foot_coobj);
}
extern void HSD_JObjClearFlags(HSD_JObj *joint, u32 flags);
extern void HSD_JObjSetFlags(HSD_JObj *joint, u32 flags);
extern void HSD_JObjDisp(HSD_JObj *jobj, void *mtx, u32 flags, u32 rendermode);

void zdCoSmallDisp(u8 *co)
{
    HSD_JObj *joint;

    if (co == (void *)0) {
        return;
    }
    joint = *(HSD_JObj **)co;
    if (joint == (void *)0) {
        return;
    }
    HSD_JObjClearFlags(joint, 0x10);
    HSD_JObjDisp(joint, 0, 7, 0);
    HSD_JObjSetFlags(joint, 0x10);
}
#define Zoid ((ZoidState *)Zoid)
extern void zdCoSmallDisp(u8 *co);

void ZoidSmallDisp(ZoidState *zoid)
{
    s32 i;

    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    zdCoSmallDisp(zoid->head_coobj);
    zdCoSmallDisp(zoid->neck_coobj);
    zdCoSmallDisp(zoid->bust_coobj);
    zdCoSmallDisp(zoid->hip_coobj);
    for (i = 0; i < 8; i++) {
        zdCoSmallDisp(zoid->tail_coobjs[i]);
    }
    zdCoSmallDisp(zoid->l_arm1_coobj);
    zdCoSmallDisp(zoid->l_arm2_coobj);
    zdCoSmallDisp(zoid->r_arm1_coobj);
    zdCoSmallDisp(zoid->r_arm2_coobj);
    zdCoSmallDisp(zoid->l_leg1_coobj);
    zdCoSmallDisp(zoid->l_leg2_coobj);
    zdCoSmallDisp(zoid->r_leg1_coobj);
    zdCoSmallDisp(zoid->r_leg2_coobj);
    zdCoSmallDisp(zoid->l_foot_coobj);
    zdCoSmallDisp(zoid->r_foot_coobj);
}
#undef Zoid
#define Zoid ((ZoidState *)Zoid)
extern void zdStelON(ZoidState *zoid);
extern void zdStelOFF(ZoidState *zoid);
extern void zdTrans1(ZoidState *zoid);
extern void ZoidTrans(ZoidState *zoid, f32 alpha);

void ZoidTransMode(void)
{
    ZoidState *zoid;
    s32 i;

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint != (void *)0) {
            s32 state;
            s32 previous;

            if (zoid->option_active != 0) {
                zdStelON(zoid);
            } else {
                zdStelOFF(zoid);
            }
            state = zoid->stel_state;
            previous = zoid->previous_stel_state;
            if (previous == state) {
                if (state == ZOID_STEL_FADING) {
                    zdTrans1(zoid);
                }
            } else {
                if (previous == ZOID_STEL_ON) {
                    zdSetMatStelOFF(zoid);
                }
                if (previous == ZOID_STEL_FADING) {
                    ZoidTrans(zoid, 1.0f);
                }
                ZoidCopyMatRet(zoid);
                if (state == ZOID_STEL_FADING) {
                    zdTrans1(zoid);
                }
                if (state == ZOID_STEL_ON) {
                    zdSetMatStel(zoid);
                }
                zoid->previous_stel_state = state;
            }
        }
    }
}
#undef Zoid
extern void PSMTXMultVec(f32 matrix[3][4], Vec3 *source, Vec3 *destination);
extern s32 sCamNN;
namespace { extern const f32 lit_1885[1]; extern const f32 lit_2857[1]; extern const f32 lit_2858[1]; }

#define Zoid ((ZoidState *)Zoid)

s32 zdClip(s32 flag, f32 matrix[3][4], ZoidState *zoid, Vec3 *out)
{
    f32 scale;
    f32 limit;
    f32 height;
    s32 i;

    height = *(f32 *)((u8 *)zoid->body_coobj + 0x10);
    scale = 0.0f;
    limit = lit_2857[0];
    if (height > lit_1885[0]) {
        scale = 1.0f;
        limit += height;
    }
    for (i = 0; i < 3; i++) {
        zoid->clip_point[i] = ((HSD_JObj *)zoid->camera_joint)->matrix[i][3];
    }
    PSMTXMultVec(matrix, (Vec3 *)zoid->clip_point, out);
    if (out->z >= height * scale + lit_2857[0]) {
        return 1;
    }
    if (flag == 0 && out->z < lit_2858[0]) {
        return 1;
    }
    if (sCamNN == 1) {
        if (out->x + out->z >= limit) {
            return 1;
        }
        if (out->x - out->z <= -limit) {
            return 1;
        }
    }
    return 0;
}
#undef Zoid
extern void *HSD_CObjGetViewingMtxPtr(void *cobj);
extern void HSD_JObjDispAll(HSD_JObj *joint, s32 a, s32 b, s32 c);
extern void angLimit(f32 *angle);
extern u32 dbgZoidColi;
extern s32 zdClip(s32 lod, f32 (*view)[4], ZoidState *zoid, Vec3 *point);
extern void ZoidSmallDisp(ZoidState *zoid);

#define Zoid ((ZoidState *)Zoid)

void ZoidAllDisp3(void)
{
    s32 i;
    f32 (*view)[4];
    Vec3 point;
    s32 lod;
    ZoidState *zoid;
    ZoidControlData *control;
    s32 flag;
    s32 stealth;

    view = (f32 (*)[4])HSD_CObjGetViewingMtxPtr(camTmp->cobj);
    for (i = 0; i < 16; i++) {
        stealth = 0;
        if ((zoid = &Zoid[i])->joint != (void *)0) {
            lod = camTmp->lod_flag;
            control = (ZoidControlData *)zoid->anim_data;
            if (zoid->stel_state == 2) {
                stealth = 1;
            }
            if (stealth == 0) {
                continue;
            }
            if (zdClip(lod, view, &Zoid[i], &point) != 0) {
                continue;
            }
            angLimit(&Zoid[i].heading);
            zdSetXYZ(zoid);
            flag = camTmp->zoid_distance[i] < (f32)control->detail_distance;
            if (flag) {
                flag = !dbgZoidColi;
            }
            if (lod != 0) {
                flag = 1;
            }
            if (flag) {
                ZoidCoObjOFF(zoid);
                HSD_JObjDispAll(Zoid[i].joint, 0, 7, 0);
            } else {
                ZoidSmallDisp(&Zoid[i]);
            }
            zoid = zoid;
            stealth = stealth;
        }
    }
}
#undef Zoid
extern void zdDispCursor(void);

#define Zoid ((ZoidState *)Zoid)

void ZoidAllDisp(void)
{
    s32 i;
    f32 (*view)[4];
    Vec3 point;
    s32 lod;
    ZoidState *zoid;
    ZoidControlData *control;
    s32 flag;

    view = (f32 (*)[4])HSD_CObjGetViewingMtxPtr(camTmp->cobj);
    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (Zoid[i].joint != (void *)0) {
            lod = camTmp->lod_flag;
            control = (ZoidControlData *)zoid->anim_data;
            if (zoid->stel_state != 0) {
                continue;
            }
            if (zdClip(lod, view, &Zoid[i], &point) != 0) {
                continue;
            }
            angLimit(&Zoid[i].heading);
            flag = camTmp->zoid_distance[i] < (f32)control->detail_distance;
            if (flag) {
                flag = !dbgZoidColi;
            }
            if (lod != 0) {
                flag = 1;
            }
            if (flag) {
                ZoidCoObjOFF(zoid);
                HSD_JObjDispAll(Zoid[i].joint, 0, 7, 0);
            } else {
                ZoidSmallDisp(&Zoid[i]);
            }
        }
    }
    zdDispCursor();
}
#undef Zoid
#define Zoid ((ZoidState *)Zoid)

void ZoidAllDispXLU(void)
{
    s32 i;
    f32 (*view)[4];
    Vec3 point;
    s32 lod;
    ZoidState *zoid;
    ZoidControlData *control;
    s32 flag;

    view = (f32 (*)[4])HSD_CObjGetViewingMtxPtr(camTmp->cobj);
    for (i = 0; i < 16; i++) {
        if ((zoid = &Zoid[i])->joint != (void *)0) {
            lod = camTmp->lod_flag;
            control = (ZoidControlData *)zoid->anim_data;
            if (zoid->stel_state != 1) {
                continue;
            }
            if (zdClip(lod, view, &Zoid[i], &point) != 0) {
                continue;
            }
            flag = camTmp->zoid_distance[i] < (f32)control->detail_distance;
            if (flag) {
                flag = !dbgZoidColi;
            }
            if (lod != 0) {
                flag = 1;
            }
            if (flag) {
                ZoidCoObjOFF(zoid);
                HSD_JObjDispAll(Zoid[i].joint, 0, 7, 0);
            } else {
                ZoidSmallDisp(zoid);
            }
        }
    }
}
#undef Zoid

void ZoidAllDBG(void)
{
}
#include "missile.h"

void zdEffectMount(ZoidState *zoid, s32 mount, s32 effect)
{
    HSD_JObj *joint;
    s32 mode;
    s32 kind;

    if (effect <= 0) {
        return;
    }
    if (zoid == 0) {
        return;
    }
    if (zoid->joint == 0) {
        return;
    }
    if (mount >= 27) {
        return;
    }
    if (mount < 0) {
        return;
    }
    if (effect >= 0x88) {
        return;
    }
    joint = zoid->joint;
    kind = Tea_effectdat[effect].kind;
    mode = 2;
    if (kind == EFFECT_KIND_ZOID) {
        mode = 3;
    } else {
        joint = zoid->weapon_slots[mount].joint;
        if (joint == 0) {
            return;
        }
    }
    sEffectSet(effect, mode, 0, joint, zoid);
}
typedef struct ZoidMountV2 {
    u8 pad_0000[0x1C7C];
    s32 mounts[5][3];
} ZoidMountV2;

void zdEffectMountTm(ZoidState *zoid, s32 a, s32 b, s32 mode)
{
    s32 i;
    s32 *mount;
    if (zoid != 0 && zoid->joint != 0) {
        if (mode == 0) {
            mode = 1;
        }
        for (i = 0; i < 5; i++) {
            mount = ((ZoidMountV2 *)zoid)->mounts[i];
            if (mount[0] == 0) {
                mount[0] = mode;
                mount[1] = a;
                mount[2] = b;
                return;
            }
        }
    }
}
typedef struct ZoidMountIntV2 {
    u8 pad_0000[0x1C7C];
    s32 mounts[5][3];
} ZoidMountIntV2;

void zdEffectMountInt(ZoidState *zoid)
{
    s32 i;
    s32 *entry;

    if (zoid != 0 && zoid->joint != 0) {
        for (i = 0; i < 5; i++) {
            entry = &((ZoidMountIntV2 *)zoid)->mounts[i][0];
            if (entry[0] != 0) {
                entry[0] = entry[0] - 1;
                if (entry[0] <= 0) {
                    entry[0] = 0;
                    zdEffectMount(zoid, entry[1], entry[2]);
                    return;
                }
            }
        }
    }
}

void ftSmoke(ZoidState *zoid, FootJoint *foot, s32 frame)
{
    HSD_JObj *joint;
    u32 flags;

    if (zoid == (void *)0) {
        return;
    }
    if (foot == (void *)0) {
        return;
    }
    joint = foot->joint;
    if (joint == (void *)0) {
        return;
    }
    flags = foot->flags;
    if ((flags & FOOT_FLAG_CONTACT) && !(flags & FOOT_FLAG_PREVIOUS_CONTACT) && frame > 3) {
        sEffectSet(56, 1, 0, joint, 0);
    }
    if (flags & FOOT_FLAG_CONTACT) {
        flags |= FOOT_FLAG_PREVIOUS_CONTACT;
    } else {
        flags &= ~FOOT_FLAG_PREVIOUS_CONTACT;
    }
    foot->flags = flags;
}

void zdSmoke(ZoidState *zoid)
{
    s32 frame;
    s32 flag;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        frame = zoid->current_animation;
        flag = 1;
        if (frame == 1)
            frame = 2;
        ftSmoke(zoid, &zoid->feet[0], 0);
        ftSmoke(zoid, &zoid->feet[1], 0);
        ftSmoke(zoid, &zoid->feet[2], frame);
        ftSmoke(zoid, &zoid->feet[3], frame);
    }
}
extern void zdSmoke(ZoidState *zoid);

#define Zoid ((ZoidState *)Zoid)
void ZoidAllSmoke(void)
{
    s32 i;
    for (i = 0; i < 16; i++) {
        zdSmoke(&Zoid[i]);
    }
}
#undef Zoid
typedef struct CoObjMtxV2 {
    HSD_JObj *joint;
    u8 reserved_04[0x30];
    f32 inverse[3][4];
} CoObjMtxV2;

void zdCoObjMtxI(void *object)
{
    if (object != (void *)0 && ((CoObjMtxV2 *)object)->joint != (void *)0)
        PSMTXInverse(((CoObjMtxV2 *)object)->joint->matrix, ((CoObjMtxV2 *)object)->inverse);
}

void ZoidCoObjMtxI(ZoidState *zoid)
{
    s32 i;
    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        zdCoObjMtxI(zoid->body_coobj);
        zdCoObjMtxI(zoid->bust_coobj);
        zdCoObjMtxI(zoid->neck_coobj);
        zdCoObjMtxI(zoid->head_coobj);
        zdCoObjMtxI(zoid->hip_coobj);
        for (i = 0; i < 8; i++)
            zdCoObjMtxI(zoid->tail_coobjs[i]);
        zdCoObjMtxI(zoid->l_arm1_coobj);
        zdCoObjMtxI(zoid->r_arm1_coobj);
        zdCoObjMtxI(zoid->l_arm2_coobj);
        zdCoObjMtxI(zoid->r_arm2_coobj);
        zdCoObjMtxI(zoid->l_leg1_coobj);
        zdCoObjMtxI(zoid->l_leg2_coobj);
        zdCoObjMtxI(zoid->r_leg1_coobj);
        zdCoObjMtxI(zoid->r_leg2_coobj);
        for (i = 0; i < 10; i++) {
            ZoidAttackObject *attack = &zoid->attack_objects[i];
            if (attack->joint != (void *)0)
                PSMTXInverse(attack->joint->matrix, attack->inverse);
        }
    }
}
extern void ZoidCoObjMtxI(ZoidState *zoid);
extern void sMissileZoid(void);

#define Zoid ((ZoidState *)Zoid)
void ZoidCoObjAll(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        ZoidCoObjMtxI(&Zoid[i]);
    }
    for (i = 0; i < 16; i++) {
    }
    sMissileZoid();
}
#undef Zoid
extern void HSD_JObjSetupMatrixSub(HSD_JObj *joint);

#pragma push
#pragma dont_inline off
#pragma auto_inline off
void sMakeMatrix(HSD_JObj *joint)
{
    if (joint != (void *)0) {
        if (joint != 0 && HSD_JObjMtxIsDirty(joint))
            HSD_JObjSetupMatrixSub(joint);
        for (joint = joint->child; joint != 0; joint = joint->next)
            sMakeMatrix(joint);
    }
}
#pragma pop
#define Zoid ((ZoidState *)Zoid)
void ZoidAllMat(void)
{
    s32 i;
    for (i = 0; i < 16; i++) {
        if (Zoid[i].joint != (void *)0) {
            zdSetXYZ(&Zoid[i]);
            sMakeMatrix(Zoid[i].joint);
        }
    }
}
#undef Zoid
#define object ((CoObj *)co)
void ZoidCoBAK(ZoidState *zoid, u8 *co)
{
    s32 axis;

    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == 0) {
        return;
    }
    if (object == (void *)0) {
        return;
    }
    if (object->joint == (void *)0) {
        return;
    }
    for (axis = 0; axis < 3; axis++) {
        object->movement[axis] = object->joint->matrix[axis][3] - object->position[axis];
        object->position[axis] = object->joint->matrix[axis][3];
        if (object->base_joint != 0) {
            object->base_position[axis] = object->base_joint->matrix[axis][3];
        }
    }
    object->field_9C = object->joint->matrix[1][3];
}
#undef object
void zdSearchPosPoly(ZoidState *zoid)
{
    u16 selector;
    GroundPolygon *polygon;
    s32 vertex;
    s32 i;

    if (zoid != 0 && zoid->joint != 0) {
        zdColPosY(zoid->position, &selector);
        if (selector == 0xFFFF) {
            polygon = grdcPolygon;
            vertex = polygon->vertices[0];
            for (i = 0; i < 3; i++) {
                zoid->position[i] = ((f32 *)&grdcVtx[vertex])[i];
            }
        }
    }
}
extern void ZoidCoBAK(ZoidState *zoid, u8 *co);

void zdCoStartPos(ZoidState *zoid)
{
    s32 axis;

    zdSetXYZ(zoid);
    sMakeMatrix(zoid->joint);
    ZoidCoBAK(zoid, zoid->body_coobj);
    ZoidCoBAK(zoid, zoid->head_coobj);
    ZoidCoBAK(zoid, zoid->neck_coobj);
    ZoidCoBAK(zoid, zoid->bust_coobj);
    ZoidCoBAK(zoid, zoid->hip_coobj);
    ZoidCoBAK(zoid, zoid->tail_coobjs[0]);
    ZoidCoBAK(zoid, zoid->l_arm1_coobj);
    ZoidCoBAK(zoid, zoid->r_arm1_coobj);
    ZoidCoBAK(zoid, zoid->l_leg1_coobj);
    ZoidCoBAK(zoid, zoid->r_leg1_coobj);
    ZoidCoBAK(zoid, zoid->l_arm2_coobj);
    ZoidCoBAK(zoid, zoid->r_arm2_coobj);
    ZoidCoBAK(zoid, zoid->l_leg2_coobj);
    ZoidCoBAK(zoid, zoid->r_leg2_coobj);
    for (axis = 0; axis < 3; axis++) {
        zoid->field_1C24[axis] = zoid->position[axis];
    }
}
#pragma force_active on
namespace { __declspec(section ".sdata2") extern const f32 lit_2857[1] = { 20.0f }; }
namespace { __declspec(section ".sdata2") extern const f32 lit_2858[1] = { -600.0f }; }
#pragma force_active off
#define Zoid ((ZoidState *)Zoid)

void ZoidInertia(ZoidState *zoid)
{
    s32 axis;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        zoid->displacement[1] -= 0.025;
        if (zoid->grounded != 0 || zoid->field_1EB0 != 0 || zoid->field_1EB2 != 0 || zoid->field_1EAE != 0xFFFF) {
            for (axis = 0; axis < 3; axis++) {
                if (axis == 1) {
                    zoid->displacement[1] -= 0.005000000000000001;
                }
                zoid->displacement[axis] *= 0.59;
            }
        }
        if (zoid->field_1EB0 != 0 || zoid->field_1EB2 != 0) {
            for (axis = 0; axis < 3; axis++) {
                zoid->displacement[axis] = 0.0f;
            }
        }
        for (axis = 0; axis < 3; axis++) {
            zoid->position[axis] += zoid->displacement[axis];
        }
    }
}
#undef Zoid
extern void sWeaponSearchSightClear(Weapon3 *weapon);

void zdWeaponSightClear(ZoidState *zoid, s32 selected)
{
    WeaponMount *mount;
    WeaponMountEntry *entry;
    WeaponSlot *weapon_slot;
    s32 index;
    s32 slot;
    s32 weapon;

    if (zoid != 0 && zoid->joint != 0 && selected != -1) {
        mount = &zoid->weapon_mounts[selected];
        if (mount->active != 0) {
            for (index = 0; index < 4; index++) {
                entry = &mount->entries[index];
                if (entry->active != 0) {
                    slot = entry->slot;
                    if (slot != -1) {
                        weapon_slot = &zoid->weapon_slots[slot];
                        weapon = weapon_slot->weapon;
                        sWeaponSearchSightClear(&sWeapon[weapon]);
                    }
                }
            }
        }
    }
}

void zdWeaponSightClearAll(ZoidState *zoid)
{
    s32 i;
    for (i = 0; i < 20; i++) {
        zdWeaponSightClear(zoid, i);
    }
}
extern void zdWeaponSightClearAll(ZoidState *zoid);

#define Zoid ((ZoidState *)Zoid)

void ZoidSearchNextTarget(ZoidState *zoid)
{
    ZoidState *other;
    s32 group;
    s32 index;
    s32 i;

    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    if (zoid->death_started != 0) {
        return;
    }
    group = zoid->slot;
    index = zoid->target_slot;
    for (i = 0; i < 16; i++) {
        index++;
        if (index >= 16) {
            index = 0;
        }
        if (index < 0) {
            index = 0;
        }
        other = &Zoid[index];
        if (other == (void *)0) {
            continue;
        }
        if (other->joint == (void *)0) {
            continue;
        }
        if (group == other->slot) {
            continue;
        }
        if (other->death_started != 0) {
            continue;
        }
        if (other->stel_state == 2) {
            continue;
        }
        if (zoid->target_visibility[index] == 0) {
            continue;
        }
        if (zoid->search_range < zoid->distances[index]) {
            continue;
        }
        zoid->target_slot = index;
        zoid->target_pending = 1;
        zdWeaponSightClearAll(zoid);
        return;
    }
    zoid->target_pending = 0;
}
#undef Zoid
#define Zoid ((ZoidState *)Zoid)

void ZoidTargetSet(ZoidState *zoid, s32 index)
{
    ZoidState *target;
    s32 slot;
    s32 target_slot;

    if (zoid != (void *)0 && zoid->joint != (void *)0 && index >= 0 && index < 16) {
        target = &Zoid[index];
        if (target->joint != (void *)0 && target->stel_state != ZOID_STEL_ON &&
            zoid->target_visibility[index] != 0 &&
            !(zoid->search_range < zoid->distances[index])) {
            slot = zoid->slot;
            target_slot = target->slot;
            if (slot != target_slot) {
                zoid->target_slot = index;
                zoid->target_pending = 1;
            }
        }
    }
}
#undef Zoid
#define Zoid ((ZoidState *)Zoid)

#pragma push
#pragma dont_inline off
void zdLen(s32 a, s32 b)
{
    s32 k;
    f32 len = 0.0f;
    f32 diff;

    if (a != b) {
        for (k = 0; k < 3; k++) {
            diff = Zoid[a].joint->matrix[k][3] - Zoid[b].joint->matrix[k][3];
            len += diff * diff;
        }
        len = sjSqrtf(len);
    }
    Zoid[a].distances[b] = len;
    Zoid[b].distances[a] = len;
}
#pragma pop
#undef Zoid
typedef struct ZoidDataA {
    u8 reserved_000[0x34];
    u8 us_pad_0034[0x14];
    u8 reserved_000_us[0x6B2 - 0x34];
    s16 attack_ranges[12];
    s16 attack_angles_a[12];
    s16 attack_angles[12];
    u8 reserved_6FA[0x7E8 - 0x6FA];
} ZoidDataA;
extern f32 ZoidCalcAngle(ZoidState *zoid, Vec3 *target);
#define Zoid ((ZoidState *)Zoid)

#pragma push
#pragma dont_inline off
#pragma auto_inline off
void zdAtk(ZoidState *zoid)
{
    ZoidState *target;
    s32 attack;
    s32 index;
    f32 range;
    f32 half_angle;
    f32 other_angle;
    f32 angle;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        attack = 0;
        if (zoid->action == 13) attack = 0;
        if (zoid->action == 14) attack = 1;
        if (zoid->action == 15) attack = 2;
        if (zoid->action == 16) attack = 3;
        if (zoid->action == 17) attack = 4;
        if (zoid->action == 18) attack = 5;
        if (zoid->action == 19) attack = 6;
        if (zoid->action == 20) attack = 7;
        if (zoid->action == 21) attack = 8;
        if (zoid->action == 22) attack = 9;
        range = ((ZoidDataA *)Tea_zoiddat)[zoid->type].attack_ranges[attack];
        other_angle = 2.0 * (3.1415926 * (f32)((ZoidDataA *)Tea_zoiddat)[zoid->type].attack_angles_a[attack]) / 360.0;
        half_angle = 2.0 * (3.1415926 * (f32)((ZoidDataA *)Tea_zoiddat)[zoid->type].attack_angles[attack]) / 360.0 / 2.0;
        for (index = 0; index < 16; index++) {
            target = &Zoid[index];
            zoid->attack_in_range[index] = 0;
            if (target->joint != (void *)0 && target->death_started == 0 &&
                (u32)target->slot != (u32)zoid->slot && !(range < zoid->distances[index])) {
                angle = ZoidCalcAngle(zoid, (Vec3 *)target->position);
                if (angle != 100.0f) {
                    if (!(absf(angle) > half_angle))
                        zoid->attack_in_range[index] = 1;
                }
            }
        }
    }
}
#pragma pop
#undef Zoid
#define Zoid ((ZoidState *)Zoid)

void ZoidLen(void)
{
    s32 i;
    s32 j;
    ZoidState *zoid;
    ZoidState *other;
    f32 angle;

    for (i = 0; i < 16; i++) {
        if (Zoid[i].joint != (void *)0) {
            for (j = i; j < 16; j++) {
                if (Zoid[j].joint != (void *)0) {
                    zdLen(i, j);
                }
            }
        }
    }
    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint != (void *)0) {
            zdAtk(zoid);
            for (j = 0; j < 16; j++) {
                other = &Zoid[j];
                if (i != j && other->joint != (void *)0) {
                    angle = ZoidCalcAngle(zoid, (Vec3 *)other->position);
                    if (angle != 100.0) {
                        zoid->target_angles[j] = angle;
                    }
                }
            }
        }
    }
}
#undef Zoid
extern void HSD_JObjSetFlags(HSD_JObj *joint, u32 flags);
extern void HSD_JObjClearFlags(HSD_JObj *joint, u32 flags);
extern void HSD_ShadowAddObject(HSD_Shadow *shadow, HSD_JObj *joint);

#define co ((CoObj *)object)
s32 zdShadowLo(void *shadow, u8 *object)
{
    s32 visible;
    u16 polygon_index;
    GroundPolygon *polygon;
    Vec3 *normal;

    if (co == (void *)0) {
        return 1;
    }
    if (co->joint == (void *)0) {
        return 1;
    }
    visible = 1;
    polygon_index = co->ground_polygon_index;
    if (polygon_index != 0xFFFF) {
        polygon = &grdcPolygon[polygon_index];
        normal = &grdcNormal[polygon->normal_index];
        if (normal->y < 0.5) {
            visible = 0;
        }
    }
    if (visible == 0) {
        HSD_JObjSetFlags(co->joint, 0x10);
        return 0;
    }
    HSD_JObjClearFlags(co->joint, 0x10);
    HSD_ShadowAddObject((HSD_Shadow *)shadow, co->joint);
    return 1;
}
#undef co
extern void HSD_JObjSetFlags(HSD_JObj *joint, u32 flags);
extern void HSD_JObjClearFlags(HSD_JObj *joint, u32 flags);
extern void HSD_ShadowAddObject(HSD_Shadow *shadow, HSD_JObj *joint);

#define co ((CoObj *)object)
void zdShadowLoChk(void *shadow, u8 *object)
{
    u16 polygon;
    f32 position[3];
    GroundMask filter;
    GroundPolygon *poly;
    Vec3 *normal;
    s32 index;
    s32 ok;
    f32 ground;

    if (co != 0 && co->joint != 0) {
        for (index = 0; index < 3; index++) {
            position[index] = co->joint->matrix[index][3];
        }
        filter = (GroundMask){{0x1B, 0, 0, 0}};
        ground = sColPosAndY((Vec3 *)position, &polygon, &filter);
        ok = 1;
        if (ground > position[1]) {
            ok = 0;
        }
        if (polygon != 0xFFFF) {
            poly = &grdcPolygon[polygon];
            normal = &grdcNormal[poly->normal_index];
            if (normal->y < 0.5) {
                ok = 0;
            }
        }
        if (ok == 0) {
            HSD_JObjSetFlags(co->joint, 0x10);
        } else {
            HSD_JObjClearFlags(co->joint, 0x10);
            HSD_ShadowAddObject((HSD_Shadow *)shadow, co->joint);
        }
    }
}
#undef co
extern s32 zdShadowLo(void *shadow, u8 *object);
extern void zdShadowLoChk(void *shadow, u8 *object);

#define Zoid ((ZoidState *)Zoid)

void ZoidShadowLo(void *shadow, ZoidState *zoid)
{
    s32 i;

    if (zoid->joint != (void *)0) {
        if (zdShadowLo(shadow, zoid->bust_coobj) != 0) {
            if (zdShadowLo(shadow, zoid->neck_coobj) != 0) {
                zdShadowLo(shadow, zoid->head_coobj);
            }
        }
        zdShadowLo(shadow, zoid->hip_coobj);
        for (i = 0; i < 8; i++) {
            zdShadowLo(shadow, zoid->tail_coobjs[i]);
        }
        zdShadowLo(shadow, zoid->l_arm1_coobj);
        zdShadowLo(shadow, zoid->l_arm2_coobj);
        zdShadowLo(shadow, zoid->r_arm1_coobj);
        zdShadowLo(shadow, zoid->r_arm2_coobj);
        zdShadowLo(shadow, zoid->l_leg1_coobj);
        zdShadowLo(shadow, zoid->l_leg2_coobj);
        zdShadowLo(shadow, zoid->r_leg1_coobj);
        zdShadowLo(shadow, zoid->r_leg2_coobj);
        zdShadowLoChk(shadow, zoid->l_foot_coobj);
        zdShadowLoChk(shadow, zoid->r_foot_coobj);
    }
}
#undef Zoid
void ZoidCoObjMove(CoObj *object, f32 *movement, f32 damage)
{
    f32 (*world)[4];
    f32 (*base)[4];
    s32 axis;

    if (object != (void *)0 && object->joint != (void *)0 && object->base_joint != (void *)0) {
        world = object->joint->matrix;
        base = object->base_joint->matrix;
        for (axis = 0; axis < 3; axis++) {
            world[axis][3] += movement[axis];
            base[axis][3] += movement[axis];
        }
        PSMTXInverse(world, object->inverse);
        sGlobal2Local(object->base_joint);
        object->health -= (s32)damage;
        if (object->health < 0) {
            object->health = 0;
        }
    }
}
typedef struct CoObjCC {
    HSD_JObj *joint;
    HSD_JObj *base_joint;
    u8 reserved_08[0x14 - 0x08];
    u32 object_contact;
    u8 reserved_18[0x34 - 0x18];
    f32 inverse[3][4];
} CoObjCC;

extern s32 sTouchCube(HSD_JObj *a, HSD_JObj *b);
extern s32 sChkCubeInCube(f32 matrix[3][4], f32 inverse[3][4]);
extern u32 PSMTXInverse(f32 src[3][4], f32 dst[3][4]);

#pragma push
#pragma dont_inline off
static inline f32 ccSqrtf(f32 x)
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

s32 ZoidCoObjCoObj(CoObjCC *a, CoObjCC *b, s32 flags)
{
    f32 pa[3];
    f32 pb[3];
    f32 mid[3];
    f32 d[3];
    f32 s[3];
    f32 len;
    s32 i;
    s32 k;

    if (a == (void *)0)
        return 0;
    if (b == (void *)0)
        return 0;
    if (a->joint == (void *)0)
        return 0;
    if (b->joint == (void *)0)
        return 0;
    if (sTouchCube(a->joint, b->joint) == 0)
        return 0;
    if (sChkCubeInCube(a->joint->matrix, b->inverse) == 0 && sChkCubeInCube(b->joint->matrix, a->inverse) == 0)
        return 0;
    if (a->base_joint == (void *)0)
        return 1;
    if (b->base_joint == (void *)0)
        return 1;
    len = 0.0f;
    for (k = 0; k < 3; k++) {
        pa[k] = a->joint->matrix[k][3];
        pb[k] = b->joint->matrix[k][3];
        mid[k] = (pa[k] + pb[k]) / 2.0f;
        d[k] = pb[k] - pa[k];
        len += d[k] * d[k];
    }
    len = ccSqrtf(len) / 0.02;
    if (0.0f == len)
        return 1;
    if (flags == 0)
        return 1;
    a->object_contact |= 1;
    b->object_contact |= 1;
    for (i = 1; i < 20; i++) {
        for (k = 0; k < 3; k++) {
            s[k] = i * (d[k] / len);
            a->joint->matrix[k][3] = pa[k] - s[k];
            b->joint->matrix[k][3] = pb[k] + s[k];
        }
        PSMTXInverse(a->joint->matrix, a->inverse);
        PSMTXInverse(b->joint->matrix, b->inverse);
        if (sChkCubeInCube(a->joint->matrix, b->inverse) == 0 && sChkCubeInCube(b->joint->matrix, a->inverse) == 0)
            break;
    }
    for (k = 0; k < 3; k++) {
        a->base_joint->matrix[k][3] -= s[k];
        b->base_joint->matrix[k][3] += s[k];
    }
    sGlobal2Local(a->base_joint);
    sGlobal2Local(b->base_joint);
    return 1;
}
#pragma pop
#define Zoid ((ZoidState *)Zoid)

void ZoidZdCoObjBodyAll(void)
{
    s32 dead;
    s32 i;
    ZoidState *zoid;
    ZoidState *other;
    s32 j;

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];

        if (zoid->joint != (void *)0 && *(HSD_JObj **)zoid->body_coobj != (void *)0) {
            ZoidCoObjMtxI(zoid);
        }
    }
    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];

        if (zoid->joint != (void *)0 && *(HSD_JObj **)zoid->body_coobj != (void *)0) {
            for (j = 0; j < 16; j++) {
                if (i != j && (other = &Zoid[j])->joint != (void *)0 && *(HSD_JObj **)other->body_coobj != (void *)0) {
                    dead = dead;
                    dead = 0;
                    other->contact[i] = (zoid->contact[j] = ZoidCoObjCoObj((CoObjCC *)zoid->body_coobj, (CoObjCC *)other->body_coobj, 0));
                }
            }
        }
    }
}
#undef Zoid

s32 zdCoObj(ZoidState *zoid, CoObjCC *obj)
{
    s32 i;
    s32 secondary_hit;
    s32 hit;

    if (zoid == (void *)0)
        return 0;
    if (zoid->joint == (void *)0)
        return 0;
    if (obj == (void *)0)
        return 0;
    if (obj->joint == (void *)0)
        return 0;
    secondary_hit = 0;
    hit = 0;
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->head_coobj, 1)) {
        secondary_hit = 1;
    }
    for (i = 0; i < 8; i++) {
        if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->tail_coobjs[i], 1)) {
            secondary_hit = 1;
        }
    }
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->l_arm2_coobj, 1)) {
        secondary_hit = 1;
    }
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->r_arm2_coobj, 1)) {
        secondary_hit = 1;
    }
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->l_leg2_coobj, 1)) {
        secondary_hit = 1;
    }
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->r_leg2_coobj, 1)) {
        secondary_hit = 1;
    }
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->neck_coobj, 1) != 0)
        hit = 1;
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->bust_coobj, 1) != 0)
        hit = 1;
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->hip_coobj, 1) != 0)
        hit = 1;
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->l_arm1_coobj, 1) != 0)
        hit = 1;
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->r_arm1_coobj, 1) != 0)
        hit = 1;
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->l_leg1_coobj, 1) != 0)
        hit = 1;
    if (ZoidCoObjCoObj(obj, (CoObjCC *)zoid->r_leg1_coobj, 1) != 0)
        hit = 1;
    return hit;
}

s32 ZoidZdCoObj(ZoidState *zoid, ZoidState *other)
{
    s32 hit;
    s32 i;
    s32 secondary_hit;

    if (zoid == (void *)0) {
        return 0;
    }
    if (zoid->joint == (void *)0) {
        return 0;
    }
    if (other == (void *)0) {
        return 0;
    }
    if (other->joint == (void *)0) {
        return 0;
    }
    if (sTouchCube(*(HSD_JObj **)zoid->body_coobj, *(HSD_JObj **)other->body_coobj) == 0) {
        return 0;
    }
    secondary_hit = 0;
    hit = 0;
    if (zdCoObj(zoid, (CoObjCC *)&other->head_coobj)) {
        secondary_hit = 1;
    }
    for (i = 0; i < 8; i++) {
        if (zdCoObj(zoid, (CoObjCC *)other->tail_coobjs[i])) {
            secondary_hit = 1;
        }
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->l_arm2_coobj)) {
        secondary_hit = 1;
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->r_arm2_coobj)) {
        secondary_hit = 1;
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->l_leg2_coobj)) {
        secondary_hit = 1;
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->r_leg2_coobj)) {
        secondary_hit = 1;
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->neck_coobj) != 0) {
        hit = 1;
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->bust_coobj) != 0) {
        hit = 1;
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->hip_coobj) != 0) {
        hit = 1;
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->l_arm1_coobj) != 0) {
        hit = 1;
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->r_arm1_coobj) != 0) {
        hit = 1;
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->l_leg1_coobj) != 0) {
        hit = 1;
    }
    if (zdCoObj(zoid, (CoObjCC *)&other->r_leg1_coobj) != 0) {
        hit = 1;
    }
    return hit;
}
extern s32 sChkCubeInSphere(f32 (*matrix)[4], f32 (*inverse)[4]);

#define Zoid ((ZoidState *)Zoid)

s32 zdZdCoAtk(CoObj *co, ZoidAttackObject *attack, s32 effect, s32 damage)
{
    s32 hit;
    f32 position[3];
    s32 i;
    f32 (*inverse)[4];

    if (co == (void *)0) {
        return 0;
    }
    if (co->joint == (void *)0) {
        return 0;
    }
    if (attack == (void *)0) {
        return 0;
    }
    inverse = attack->inverse;
    hit = sChkCubeInSphere(co->joint->matrix, inverse);
    if (hit == 0) {
        return 0;
    }
    if (effect != -1) {
        for (i = 0; i < 3; i++) {
            position[i] = (co->joint->matrix[i][3] + attack->joint->matrix[i][3]) / 2.0f;
        }
        sEffectSet(effect, 0, position, 0, 0);
    }
    co->health -= damage;
    if (co->health < 0) {
        co->health = 0;
    }
    co->field_64 = 1;
    return hit;
}
#undef Zoid
extern s32 sTouchCube(HSD_JObj *a, HSD_JObj *b);
extern s32 zdZdCoAtk(CoObj *co, ZoidAttackObject *attack, s32 effect, s32 damage);

s32 ZoidZdCoAtack(void *atk, ZoidState *zoid, s32 effect, s32 flags)
{
#define attack ((ZoidAttackObject *)atk)
    s32 i;

    if (attack == (void *)0) {
        return 0;
    }
    if (attack->joint == (void *)0) {
        return 0;
    }
    if (zoid == (void *)0) {
        return 0;
    }
    if (zoid->joint == (void *)0) {
        return 0;
    }
    if (sTouchCube(*(HSD_JObj **)zoid->body_coobj, attack->joint) == 0) {
        return 0;
    }
    if (zdZdCoAtk((CoObj *)&zoid->head_coobj, attack, effect, flags) != 0) {
        return 1;
    }
    if (zdZdCoAtk((CoObj *)&zoid->neck_coobj, attack, effect, flags) != 0) {
        return 1;
    }
    if (zdZdCoAtk((CoObj *)&zoid->bust_coobj, attack, effect, flags) != 0) {
        return 1;
    }
    if (zdZdCoAtk((CoObj *)&zoid->hip_coobj, attack, effect, flags) != 0) {
        return 1;
    }
    for (i = 0; i < 8; i++) {
        if (zdZdCoAtk((CoObj *)&zoid->tail_coobjs[i], attack, effect, flags) != 0) {
            return 2;
        }
    }
    if (zdZdCoAtk((CoObj *)&zoid->l_arm1_coobj, attack, effect, flags) != 0) {
        return 2;
    }
    if (zdZdCoAtk((CoObj *)&zoid->r_arm1_coobj, attack, effect, flags) != 0) {
        return 2;
    }
    if (zdZdCoAtk((CoObj *)&zoid->l_leg1_coobj, attack, effect, flags) != 0) {
        return 2;
    }
    if (zdZdCoAtk((CoObj *)&zoid->r_leg1_coobj, attack, effect, flags) != 0) {
        return 2;
    }
    return 0;
#undef attack
}
s32 hitCalc(ZoidState *attacker, s32 damage, ZoidState *zoid)
{
    if (attacker == 0)
        return damage;
    if (zoid == 0)
        return damage;
    if (attacker->joint == 0)
        return damage;
    if (zoid->joint == 0)
        return damage;
    damage += attacker->melee_power_bonus;
    damage -= zoid->armor;
    if (damage < 10)
        damage = 10;
    return damage;
}
s32 hitCalc2(ZoidState *attacker, s32 damage, ZoidState *target)
{
    if (attacker == 0)
        return damage;
    if (target == 0)
        return damage;
    if (attacker->joint == 0)
        return damage;
    if (target->joint == 0)
        return damage;
    damage -= target->armor;
    if (damage < 10)
        damage = 10;
    return damage;
}

void setDmg123(ZoidState *src, ZoidState *dst, s32 index, s32 mode)
{
    s32 kind;
    u8 *p;
    s32 a;
    s32 b;
    u16 *slider;
    s32 flag;
    s32 type;
    s32 prev_type;

    if (src == 0)
        return;
    if (dst == 0)
        return;
    if (src->joint == 0)
        return;
    if (dst->joint == 0)
        return;
    if (index < 0)
        return;
    if (index > 11)
        return;
    slider = src->slider_info;
    if (slider == 0)
        return;
    a = src->field_20C0;
    b = dst->field_20C0;
    type = src->type;
    p = (u8 *)slider + index * 6 + 0x6ba;
    prev_type = dst->type;
    if (a * 2 < b) {
        if (mode != 0)
            kind = (s8)p[2];
        else
            kind = (s8)p[5];
    } else if (a / 2 > b) {
        if (mode != 0)
            kind = (s8)p[0];
        else
            kind = (s8)p[3];
    } else {
        if (mode != 0)
            kind = (s8)p[1];
        else
            kind = (s8)p[4];
    }
    if (kind == 0)
        return;
    dst->field_1E64 = type;
    dst->field_1E68 = src->person;
    flag = 1;
    if (!(dst->field_1E5C & 4) && kind == 1)
        flag = 0;
    if (dst->health == 0)
        dst->field_1E5C |= 4;
    if (kind == 1)
        dst->field_1E5C |= 2;
    if (kind == 2)
        dst->field_1E5C |= 4;
    if (flag != 0)
        dst->field_1E60 = src->index;
}
s32 zdAtkZdZdDmgHp(ZoidState *zoid)
{
    s32 idx;
    s32 dmg;
    s32 type;

    dmg = 5;
    idx = zoid->action - 13;
    type = zoid->type;
    if (zoid->current_animation == 65) {
        (void)0;
    }
    if (zoid->current_animation == 66) {
        (void)0;
    }
    if (zoid->current_animation == 62) {
        (void)0;
    }
    idx = zoid->attack_index;
    if (idx >= 0 && idx <= 11) {
        dmg = *(s16 *)((u8 *)Tea_zoiddat + type * 0x7FC + idx * 2 + 0x756);
    } else {
        dmg = 10;
    }
    return dmg;
}
extern f32 sLenPoint3dR(f32 *point);
extern s32 hitCalc(ZoidState *attacker, s32 damage, ZoidState *zoid);
extern void setDmg123(ZoidState *attacker, ZoidState *zoid, s32 index, s32 flag);
extern void zdVoiceScG(ZoidState *zoid, s32 voice);

#define Zoid ((ZoidState *)Zoid)

void ZoidCoAtackDmg(ZoidState *attacker, void *attack, ZoidState *zoid, s32 flag)
{
    s32 type;
    s32 index;
    s32 damage;
    s32 health;
    s32 i;
    f32 length;
    f32 scale;
    f32 delta[3];

    if (attacker == 0) {
        return;
    }
    if (zoid == 0) {
        return;
    }
    type = attacker->type;
    if (zoid->field_1E50 != 0) {
        return;
    }
    i = damage * index - index;
    i = (zoid->rumble_timer = 15);
    for (i = 0; i < 3; i++) {
        delta[i] = zoid->position[i] - ((ZoidAttackObject *)attack)->joint->matrix[i][3];
    }
    length = sLenPoint3dR(delta);
    if (0.0f == length) {
        for (i = 0; i < 3; i++) {
            delta[i] = zoid->position[i] - attacker->position[i];
        }
        length = sLenPoint3dR(delta);
    }
    if (0.0f == length) {
        for (i = 0; i < 3; i++) {
            delta[i] = attacker->joint->matrix[i][0];
        }
        length = sLenPoint3dR(delta);
    }
    if (0.0f == length) {
        return;
    }
    if (attacker->current_animation == 65) {
        i = i;
    }
    if (attacker->current_animation == 66) {
        i = i;
    }
    if (attacker->current_animation == 62) {
        i = i;
    }
    index = attacker->attack_index;
    if (index >= 0 && index <= 11) {
        damage = Tea_zoiddat[type].attack_damage[index];
    } else {
        damage = 10;
        index = 11;
    }
    setDmg123(attacker, zoid, index, flag == 1);
    damage = hitCalc(attacker, damage, zoid);
    scale = (f32)damage / length;
    if (damage > 0) {
        zoid->health_bar_timer = 60;
    }
    if (zoid->field_1E5C != 0) {
        for (i = 0; i < 3; i++) {
            zoid->knockback[i] += scale * delta[i];
            zoid->field_1D20[i] = zoid->knockback[i];
        }
    }
    if (attacker->slot == zoid->slot && attacker->slot == Zoid[0].slot && zoid->health != 0) {
        zdVoiceScG(zoid, 14);
    }
    health = zoid->health;
    zoid->health = zoid->health - damage;
    if (zoid->health < 0) {
        zoid->health = 0;
    }
    if (zoid->health == 0 && health != 0 && attacker->slot != zoid->slot) {
        attacker->kills++;
        attacker->field_1EF0 += zoid->max_health;
        if (attacker->weapon_mount == 19) {
            attacker->field_1EF4 += zoid->max_health;
        }
    }
}
#undef Zoid
#pragma push
#pragma dont_inline off
#pragma auto_inline off
f32 zdCoCoMinR0(u8 *pa, u8 *pb, Vec3 *direction, void *amax, void *bmax)
{
#define a ((CoObj3 *)pa)
#define b ((CoObj3 *)pb)
#define a_max ((s32)amax)
#define b_max ((s32)bmax)
    HSD_JObj *ja;
    HSD_JObj *jb;
    f32 distance;
    f32 radius;
    f32 depth;
    f32 delta;
    s32 i;

    if (a == (void *)0) {
        return 0.0f;
    }
    if (b == (void *)0) {
        return 0.0f;
    }
    ja = *(HSD_JObj **)a;
    jb = *(HSD_JObj **)b;
    if (ja == (void *)0) {
        return 0.0f;
    }
    if (jb == (void *)0) {
        return 0.0f;
    }
    distance = 0.0f;
    for (i = 0; i < 3; i++) {
        delta = ja->matrix[i][3] - jb->matrix[i][3];
        distance += delta * delta;
        ((f32 *)direction)[i] = delta;
    }
    distance = sjSqrtf(distance);
    if (0.0f == distance) {
        return 0.0f;
    }
    radius = a->minimum_scale;
    if (a_max == 1) {
        radius = a->maximum_scale;
    }
    if (b_max == 0) {
        radius += b->minimum_scale;
    } else {
        radius += b->maximum_scale;
    }
    depth = radius - distance;
    if (depth <= 0.0f) {
        return 0.0f;
    }
    for (i = 0; i < 3; i++) {
        ((f32 *)direction)[i] = ((f32 *)direction)[i] * depth / distance;
    }
    return depth;
#undef a
#undef b
#undef a_max
#undef b_max
}
#pragma pop
extern f32 zdCoCoMinR0(u8 *a, u8 *b, Vec3 *direction, void *c, void *d);
extern f32 sLenPoint3dR(f32 *point);
namespace { extern const f64 lit_2740[1]; }

f32 zdCoCoMinR(ZoidState *a, ZoidState *b)
{
    f32 distance[6][6];
    u8 *parts_a[9];
    u8 *parts_b[9];
    Vec3 direction;
    Vec3 push;
    u8 *part_a;
    u8 *part_b;
    u8 *info_a;
    u8 *info_b;
    u8 *ctrl_a;
    u8 *ctrl_b;
    void *vec_a;
    void *vec_b;
    s32 flag_a;
    s32 flag_b;
    s32 i;
    s32 j;
    s32 k;
    f32 d;
    f32 max;
    f32 d2;
    f32 plus;
    f32 minus;
    f32 mass_a;
    f32 mass_b;
    f32 total;

    part_a = 0;
    part_b = 0;
    info_a = (u8 *)a->slider_info;
    ctrl_a = (u8 *)a->field_2090;
    ctrl_b = (u8 *)b->field_2090;
    vec_a = *(void **)(ctrl_a + 0x150);
    vec_b = *(void **)(ctrl_b + 0x150);
    info_b = (u8 *)b->slider_info;
    if (info_a != 0) {
        if (info_a[0x70D] == 0)
            part_a = a->hip_coobj;
        if (info_a[0x70D] == 2)
            part_a = a->bust_coobj;
    }
    if (info_b != 0) {
        if (info_b[0x70D] == 0)
            part_b = b->hip_coobj;
        if (info_b[0x70D] == 2)
            part_b = b->bust_coobj;
    }
    flag_a = 0;
    if (part_a != 0 && *(u32 *)(part_a + 0x18) != 0)
        flag_a = 1;
    flag_b = 0;
    if (part_b != 0 && *(u32 *)(part_b + 0x18) != 0)
        flag_b = 1;
    parts_a[0] = a->hip_coobj;
    parts_b[0] = b->hip_coobj;
    parts_a[1] = a->bust_coobj;
    parts_b[1] = b->bust_coobj;
    parts_a[2] = a->neck_coobj;
    parts_b[2] = b->neck_coobj;
    parts_a[3] = a->r_leg1_coobj;
    parts_b[3] = b->r_leg1_coobj;
    parts_a[4] = a->l_leg1_coobj;
    parts_b[4] = b->l_leg1_coobj;
    parts_a[5] = a->head_coobj;
    parts_b[5] = b->head_coobj;
    for (k = 0; k < 3; k++)
        (&push.x)[k] = 0.0f;
    max = -1.0f;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            d = zdCoCoMinR0(parts_a[i], parts_b[j], &direction, vec_a, vec_b);
            distance[i][j] = d;
            if (0.0f != d && (-1.0f == max || max < d)) {
                max = d;
                for (k = 0; k < 3; k++)
                    (&push.x)[k] = (&direction.x)[k];
            }
        }
    }
    if (flag_a != 0 && flag_b != 0)
        return 0.0f;
    if (max <= 0.0f)
        return 0.0f;
    d = sLenPoint3dR(&push.x);
    if (d <= lit_1423[0])
        return 0.0f;
    if (max > lit_2740[0]) {
        max = 1.0f;
        for (k = 0; k < 3; k++)
            (&push.x)[k] *= max / d;
    }
    mass_a = a->field_20C0;
    mass_b = b->field_20C0;
    total = mass_a + mass_b;
    if (total <= 0.1) {
        mass_a = 1.0f;
        mass_b = 1.0f;
        total = 2.0f;
    }
    if (flag_a != 0) {
        mass_a = 0.0f;
        mass_b = 1.0f;
        total = 1.0f;
    }
    if (flag_b != 0) {
        mass_a = 1.0f;
        mass_b = 0.0f;
        total = 1.0f;
    }
    mass_a = mass_a / total;
    mass_b = mass_b / total;
    for (k = 0; k < 3; k++) {
        d2 = (&push.x)[k];
        plus = d2;
        minus = d2;
        a->position[k] += plus * mass_b;
        b->position[k] -= minus * mass_a;
    }
    plus = zdColPosY(a->position, 0);
    if (a->position[1] < plus)
        a->position[1] = plus;
    minus = zdColPosY(b->position, 0);
    if (b->position[1] < minus)
        b->position[1] = minus;
    return max;
}
extern s32 sChkCubeInSphere(f32 (*matrix)[4], f32 (*inverse)[4]);

#define object ((CoObj3 *)coobj)
#define target ((ZoidAttackObject *)attack)
s32 zdWpCoAtk(u8 *coobj, void *attack, s32 kind, s32 damage)
{
    s32 hit;
    f32 position[3];
    s32 i;

    if (object == (void *)0) {
        return 0;
    }
    if (object->joint == (void *)0) {
        return 0;
    }
    if (target == (void *)0) {
        return 0;
    }
    if (target->joint == 0) {
        return 0;
    }
    {
        f32 (*inverse)[4] = target->inverse;
        hit = sChkCubeInSphere(object->joint->matrix, inverse);
    }
    if (hit == 0) {
        return 0;
    }
    if (kind != -1) {
        for (i = 0; i < 3; i++) {
            position[i] = (object->joint->matrix[i][3] + target->joint->matrix[i][3]) / 2.0f;
        }
        sEffectSet(kind, 0, position, 0, 0);
    }
    object->health -= damage;
    if (object->health < 0) {
        object->health = 0;
    }
    *(s32 *)((u8 *)object + 0x64) = 1;
    return hit;
}
#undef object
#undef target
extern s32 zdWpCoAtk(u8 *coobj, void *attack, s32 effect, s32 amount);
extern s32 sTouchCube(HSD_JObj *a, HSD_JObj *b);


s32 ZoidWpCoAtk(void *attack, ZoidState *zoid, s32 effect, s32 damage)
{
    s32 i;
    s32 amount;
    f32 ratio;

    if (attack == (void *)0)
        return 0;
    if (((ZoidAttackObject *)attack)->joint == (void *)0)
        return 0;
    if (zoid == (void *)0)
        return 0;
    if (zoid->joint == (void *)0)
        return 0;
    if (sTouchCube(*(HSD_JObj **)zoid->body_coobj, ((ZoidAttackObject *)attack)->joint) == 0)
        return 0;
    if (zoid->armor < 1)
        zoid->armor = 1;
    ratio = 10.0 * (f32)damage / (f32)zoid->armor;
    amount = ratio;
    if (zdWpCoAtk(zoid->head_coobj, attack, effect, amount) != 0)
        return 1;
    if (zdWpCoAtk(zoid->neck_coobj, attack, effect, amount) != 0)
        return 1;
    if (zdWpCoAtk(zoid->bust_coobj, attack, effect, amount) != 0)
        return 1;
    if (zdWpCoAtk(zoid->hip_coobj, attack, effect, amount) != 0)
        return 1;
    for (i = 0; i < 8; i++) {
        if (zdWpCoAtk(zoid->tail_coobjs[i], attack, effect, amount) != 0)
            return 1;
    }
    if (zdWpCoAtk(zoid->l_arm1_coobj, attack, effect, amount) != 0)
        return 1;
    if (zdWpCoAtk(zoid->r_arm1_coobj, attack, effect, amount) != 0)
        return 1;
    if (zdWpCoAtk(zoid->l_leg1_coobj, attack, effect, amount) != 0)
        return 1;
    if (zdWpCoAtk(zoid->r_leg1_coobj, attack, effect, amount) != 0)
        return 1;
    return 0;
}
#define Zoid ((ZoidState *)Zoid)

void ZoidCoWpAtackDmg(Weapon3 *weapon, void *co, ZoidState *zoid)
{
    s32 part_damage;
    f32 length;
    s32 health;
    f32 scale;
    s32 damage;
    ZoidState *owner;
    s32 i;
    f32 delta[3];

    if (weapon != (void *)0 && zoid->field_1E50 == 0) {
        owner = weapon->owner;
        zoid->rumble_timer = 15;
        for (i = 0; i < 3; i++) {
            delta[i] = zoid->position[i] - (*(HSD_JObj **)co)->matrix[i][3];
        }
        length = sLenPoint3dR(delta);
        if (0.0f != length) {
            for (i = 0; i < 3; i++) {
                delta[i] = delta[i] / length;
            }
            part_damage = Tea_partsdat[weapon->type].hit_a;
            damage = hitCalc(owner, part_damage, zoid);
            scale = 1.0f;
            if (part_damage & 1) {
                scale = 0.2f;
            }
            if (damage > 0) {
                zoid->health_bar_timer = 60;
                for (i = 0; i < 3; i++) {
                    zoid->knockback[i] += scale * delta[i];
                }
            }
            if (owner != 0 && owner->slot == zoid->slot && (u8)owner->slot == Zoid[0].slot && zoid->health != 0) {
                zdVoiceScG(zoid, 14);
            }
            health = zoid->health;
            zoid->health = zoid->health - damage;
            if (zoid->health < 0) {
                zoid->health = 0;
            }
            if (health != 0 && zoid->health == 0 && owner != 0 && owner->slot != zoid->slot) {
                owner->kills++;
                owner->field_1EF0 += zoid->max_health;
                if (owner->weapon_mount == 19) {
                    owner->field_1EF4 += zoid->max_health;
                }
            }
            if (owner != 0 && weapon->drain != 0 && owner->option_type == 1) {
                owner->option_energy -= 900;
                if (owner->option_energy < 0) {
                    owner->option_energy = 0;
                }
            }
        }
    }
    part_damage = part_damage;
}
#undef Zoid
extern s32 zdAtkZdZdDmgHp(ZoidState *zoid);
extern s32 ZoidZdCoAtack(void *attack, ZoidState *zoid, s32 effect, s32 damage);
extern void ZoidCoAtackDmg(ZoidState *zoid, void *attack, ZoidState *target, s32 hit);
extern s32 ZoidWpCoAtk(void *attack, ZoidState *zoid, s32 a, s32 b);
extern void ZoidCoWpAtackDmg(Weapon3 *weapon, void *inverse, ZoidState *zoid);
extern s32 ZoidZdCoObj(ZoidState *zoid, ZoidState *other);
extern f32 zdCoCoMinR(ZoidState *zoid, ZoidState *other);
#define Zoid ((ZoidState *)Zoid)

#pragma push
#pragma dont_inline off
static inline f32 HSD_JObjGetScaleX(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_485, 899, lit_486);
    return jobj->scale.x;
}

static inline f32 HSD_JObjGetScaleY(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_485, 911, lit_486);
    return jobj->scale.y;
}

static inline f32 HSD_JObjGetScaleZ(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_485, 923, lit_486);
    return jobj->scale.z;
}

void ZoidZdAll(void)
{
    s32 team;
    s32 attack_index;
    s32 type;
    s32 damage;
    s32 attack_effect;
    s32 weapon_b;
    s32 weapon_a;
    s32 i;
    ZoidState *other2;
    HSD_JObj *ajoint;
    s32 k2;
    s32 hit;
    Weapon3 *weapon;
    ZoidState *other;
    ZoidState *zoid;
    ZoidAttackObject *attack;
    s32 j;
    HSD_JObj *jobj;
    s32 k;
    f32 ratio;
    f32 range;

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        hit = 0;
        if (zoid->joint == (void *)0) {
            continue;
        }
        attack_index = -1;
        type = zoid->type;
        if (zoid->action >= 13 && zoid->action <= 22) {
            attack_index = zoid->action - 13;
        }
        if (zoid->jump_phase != 0 && zoid->jump_attack_phase != 0) {
            attack_index = 5;
        }
        if (attack_index != -1) {
            attack_effect = Tea_zoiddat[type].attack_hit_effects[attack_index];
        } else {
            attack_effect = Tea_zoiddat[type].attack_hit_effects[11];
        }
        damage = zdAtkZdZdDmgHp(zoid);
        for (j = 0; j < 16; j++) {
            if (i == j || zoid->contact[j] == 0 || (other = &Zoid[j])->joint == (void *)0 || other->field_1E50 != 0) {
                continue;
            }
            for (k = 0; k < 10; k++) {
                attack = &zoid->attack_objects[k];
                (void)hit;
                (void)hit;
                if (attack == (void *)0 || (ajoint = attack->joint) == (void *)0) {
                    continue;
                }
                if ((s8)attack->touched[j] != 0) {
                    continue;
                }
                if (ajoint->scale.x <= 0.1 || ajoint->scale.y <= 0.1 || ajoint->scale.z <= 0.1) {
                    continue;
                }
                if (gameStartF != 0) {
                    continue;
                }
                if (gametime < 15) {
                    continue;
                }
                if (other->armor < 1) {
                    other->armor = 1;
                }
                ratio = 10.0 * (f32)damage / (f32)other->armor;
                if ((hit = ZoidZdCoAtack(attack, other, attack_effect, (s32)ratio))) {
                    attack->touched[j] = 1;
                    ZoidCoAtackDmg(zoid, attack, other, hit);
                }
            }
            if (hit != 0) {
                break;
            }
        }
    }

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint == (void *)0) {
            continue;
        }
        hit = 0;
        for (j = 0; j < 100; j++) {
            weapon = &sWeapon[j];
            (void)other;
            if (weapon->active == 0 || weapon->owner == zoid) {
                continue;
            }
            for (k = 0; k < 12; k++) {
                jobj = weapon->inverses[k].joint;
                weapon_a = Tea_partsdat[weapon->type].hit_b;
                weapon_b = Tea_partsdat[weapon->type].hit_a;
                if (jobj == (void *)0) {
                    continue;
                }
                if (HSD_JObjGetScaleX(jobj) < 0.1) {
                    continue;
                }
                if (HSD_JObjGetScaleY(jobj) < 0.1) {
                    continue;
                }
                if (HSD_JObjGetScaleZ(jobj) < 0.1) {
                    continue;
                }
                if (gameStartF != 0) {
                    continue;
                }
                if (gametime < 15) {
                    continue;
                }
                if ((hit = ZoidWpCoAtk(&weapon->inverses[k], zoid, weapon_a, weapon_b))) {
                    ZoidCoWpAtackDmg(weapon, &weapon->inverses[k], zoid);
                    break;
                }
            }
            if (hit != 0) {
                break;
            }
        }
    }

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint != (void *)0) {
            zoid->field_1EB2 = zoid->field_1EB0;
            zoid->field_1EB0 = 0;
        }
    }
    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint == (void *)0) {
            continue;
        }
        for (j = i; j < 16; j++) {
            if (i == j || zoid->contact[j] == 0 || (other = &Zoid[j])->joint == (void *)0) {
                continue;
            }
            if (ZoidZdCoObj(zoid, other) == 0) {
                continue;
            }
            zdCoCoMinR(zoid, other);
            zoid->field_1EB0 |= bitTBL[j];
            other->field_1EB0 |= bitTBL[i];
            ZoidCopyAnim(zoid);
            ZoidCopyAnim(other);
            zoid->interpolation_remaining = 10;
            zoid->interpolation_duration = 15;
            other->interpolation_remaining = 10;
            other->interpolation_duration = 15;
        }
    }

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint == (void *)0) {
            continue;
        }
        type = zoid->type;
        range = zoid->field_1EF8;
        for (j = 0; j < 16; j++) {
            zoid->target_visibility[j] = 0;
            if (i == j || (other = &Zoid[j])->joint == (void *)0) {
                continue;
            }
            if (zoid->slot == other->slot) {
                zoid->target_visibility[j] = 1;
            } else if (!(range < zoid->distances[j]) && other->stel_state != 2 && zoid->enemy_smoke_timer == 0) {
                zoid->target_visibility[j] = 1;
            }
        }
    }

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint == (void *)0) {
            continue;
        }
        team = zoid->slot;
        if (zoid->enemy_smoke_timer != 0) {
            continue;
        }
        for (j = 0; j < 16; j++) {
            if (i == j || zoid->target_visibility[j] == 0) {
                continue;
            }
            for (k2 = 0; k2 < 16; k2++) {
                if (i != k2 && j != k2 && team == (other2 = &Zoid[k2])->slot) {
                    other2->target_visibility[j] = 1;
                    (void)other2;
                }
            }
        }
    }

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint == (void *)0) {
            continue;
        }
        if (zoid->target_pending != 0) {
            if (zoid->target_visibility[zoid->target_slot] == 0) {
                zoid->target_pending = 0;
            }
            if (zoid->distances[zoid->target_slot] > zoid->search_range) {
                zoid->target_pending = 0;
            }
        }
    }
}
#pragma pop
#undef Zoid
#define co ((CoObj *)object)
void zdDownBodyCoMove(void *object, f32 *move)
{
    s32 i;
    HSD_JObj *base;

    if (co == 0) {
        return;
    }
    if (co->joint == 0) {
        return;
    }
    if (move == 0) {
        return;
    }
    base = co->base_joint;
    if (base == 0) {
        return;
    }
    for (i = 0; i < 3; i++) {
        base->matrix[i][3] += move[i];
        co->joint->matrix[i][3] += move[i];
    }
    sGlobal2Local(co->base_joint);
}
#undef co
extern void zdDownBodyCoMove(void *object, f32 *move);

void zdDownBodyMove(ZoidState *zoid, f32 *move)
{
    s32 i;

    if (zoid == 0)
        return;
    if (zoid->joint == 0)
        return;
    if (move == 0)
        return;
    for (i = 0; i < 3; i++) {
        zoid->joint->matrix[i][3] += move[i];
        zoid->position[i] += move[i];
    }
    zdDownBodyCoMove(zoid->head_coobj, move);
    zdDownBodyCoMove(zoid->neck_coobj, move);
    zdDownBodyCoMove(zoid->bust_coobj, move);
    zdDownBodyCoMove(zoid->hip_coobj, move);
    zdDownBodyCoMove(zoid->l_arm1_coobj, move);
    zdDownBodyCoMove(zoid->r_arm1_coobj, move);
    zdDownBodyCoMove(zoid->l_leg1_coobj, move);
    zdDownBodyCoMove(zoid->r_leg1_coobj, move);
    zdDownBodyCoMove(zoid->l_arm2_coobj, move);
    zdDownBodyCoMove(zoid->r_arm2_coobj, move);
    zdDownBodyCoMove(zoid->l_leg2_coobj, move);
    zdDownBodyCoMove(zoid->r_leg2_coobj, move);
    zdDownBodyCoMove(zoid->tail_coobjs[0], move);
    zdDownBodyCoMove(zoid->tail_coobjs[1], move);
}
extern f32 *grdcGetNormal(u16 selector);
extern void zdDownBodyMove(ZoidState *zoid, f32 *move);

#pragma push
#pragma dont_inline off
f32 zdDownBody(ZoidState *zoid, CoObj *co)
{
    Vec3 position;
    u16 sel;
    f32 slide[3];
    f32 radius;
    f32 floor_y;
    f32 depth;
    f32 s;
    f32 *normal;
    s32 i;

    if (zoid == (void *)0)
        return 0.0f;
    if (zoid->joint == (void *)0)
        return 0.0f;
    if (co == (void *)0)
        return 0.0f;
    if (co->joint == (void *)0)
        return 0.0f;
    if (co->base_joint == (void *)0)
        return 0.0f;
    radius = co->minimum_scale;
    for (i = 0; i < 3; i++) {
        (&position.x)[i] = co->joint->matrix[i][3];
    }
    floor_y = zdColPosY(&position.x, &sel);
    depth = floor_y - (position.y - radius);
    if (depth < -0.1)
        depth = -0.1f;
    if (depth > 0.1)
        depth = 0.1f;
    co->base_joint->matrix[1][3] += depth;
    if (co->base_joint->matrix[1][3] < floor_y)
        co->base_joint->matrix[1][3] = floor_y;
    normal = grdcGetNormal(sel);
    if (normal != 0 && normal[1] < 0.5) {
        for (i = 0; i < 3; i++) {
            s = 0.02 * normal[i];
            slide[i] = s;
        }
        zdDownBodyMove(zoid, slide);
    }
    sGlobal2Local(co->base_joint);
    HSD_JObjSetScaleX(co->base_joint, 1.0f);
    HSD_JObjSetScaleY(co->base_joint, 1.0f);
    HSD_JObjSetScaleZ(co->base_joint, 1.0f);
    return depth;
}
#pragma pop
namespace { extern const f64 lit_4774[1]; }

#pragma push
#pragma dont_inline off
void zdDownBodyIK(ZoidState *zoid, CoObj *co, f32 offset)
{
    Vec3 position;
    u16 sel;
    f32 slide[3];
    f32 floor_y;
    f32 depth;
    f32 s;
    f32 *normal;
    HSD_JObj *joint;
    s32 i;

    joint = co->joint;
    if (joint == 0)
        return;
    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    for (i = 0; i < 3; i++) {
        (&position.x)[i] = joint->matrix[i][3];
    }
    floor_y = zdColPosY(&position.x, &sel);
    depth = floor_y - (position.y + offset);
    if (depth < -0.1)
        depth = -0.1f;
    if (depth > 0.1)
        depth = 0.1f;
    joint->matrix[1][3] += depth;
    normal = grdcGetNormal(sel);
    if (normal != 0 && normal[1] < 0.5) {
        for (i = 0; i < 3; i++) {
            s = lit_4774[0] * normal[i];
            slide[i] = s;
        }
        zdDownBodyMove(zoid, slide);
    }
    sGlobal2Local(joint);
    HSD_JObjSetScaleX(joint, 1.0f);
    HSD_JObjSetScaleY(joint, 1.0f);
    HSD_JObjSetScaleZ(joint, 1.0f);
}
#pragma pop
namespace { extern const f32 lit_4813[1]; }
extern f32 beSide(f32 value, f32 target, f32 step);

void ZoidDownBody(ZoidState *zoid)
{
    f32 cur[3];
    void *anim;
    u16 *info;
    HSD_JObj *joint;
    s32 i;
    f32 y;
    f32 ground;
    f32 sum;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    info = zoid->slider_info;
    anim = zoid->anim_data;
    if (info == 0)
        return;
    if (zoid->death_pending == 0)
        return;
    joint = 0;
    if (joint == 0)
        joint = *(HSD_JObj **)zoid->hip_coobj;
    if (joint == 0)
        joint = zoid->buckle_joint;
    if (joint == 0)
        joint = zoid->camera_joint;
    y = zoid->position[1];
    ground = zdColPosY(zoid->position, 0);
    if (y > ground) {
        if (y - ground < 1.0f)
            zoid->launch_velocity[1] = 0.0f;
        if (zoid->launch_velocity[1] > 0.0f)
            zoid->launch_velocity[1] = 0.0f;
        zoid->launch_velocity[1] -= 0.025;
        zoid->position[1] += zoid->launch_velocity[1];
    }
    if (zoid->ground_normal != 0) {
        zoid->position[0] += lit_1423[0] * zoid->ground_normal[0];
        zoid->position[2] += lit_1423[0] * zoid->ground_normal[2];
    }
    y = y;
    sum = 0.0f;
    sum += zdDownBody(zoid, (CoObj *)zoid->head_coobj);
    sum += zdDownBody(zoid, (CoObj *)zoid->neck_coobj);
    sum += zdDownBody(zoid, (CoObj *)zoid->hip_coobj);
    sum += zdDownBody(zoid, (CoObj *)zoid->tail_coobjs[0]);
    sum += zdDownBody(zoid, (CoObj *)zoid->tail_coobjs[1]);
    zdDownBodyIK(zoid, (CoObj *)&zoid->feet[0], 0.0f);
    zdDownBodyIK(zoid, (CoObj *)&zoid->feet[1], 0.0f);
    zdDownBodyIK(zoid, (CoObj *)&zoid->feet[2], zoid->feet[2].offset);
    zdDownBodyIK(zoid, (CoObj *)&zoid->feet[3], zoid->feet[3].offset);
    if (zoid->death_started == 0)
        zoid->position[1] += sum / 10.0f;
    zdSetXYZ(zoid);
    sMakeMatrix(zoid->joint);
    for (i = 0; i < 3; i++)
        cur[i] = joint->matrix[i][3];
    ground = zdColPosY(cur, 0);
    y = ground - (cur[1] - 0.7);
    if (y > 0.0f) {
        zoid->height_offset += y;
        if (zoid->height_offset > 2.0f)
            zoid->height_offset = 2.0f;
    }
    if (((u8 *)info)[0x70B] & 4)
        zoid->height_offset = beSide(zoid->height_offset, 0.0f, lit_4813[0]);
    zdSetXYZ(zoid);
    sMakeMatrix(zoid->joint);
}
void zdGrdCoON(ZoidState *zoid, CoObj *co, s32 flag)
{
    u16 selector;
    f32 position[3];
    f32 depth;
    f32 height;
    s32 axis;

    if (co != (void *)0 && co->joint != (void *)0) {
        for (axis = 0; axis < 3; axis++) {
            position[axis] = co->joint->matrix[axis][3];
        }
        depth = zdColPosY(position, &selector);
        if (selector == 0xFFFF) {
            return;
        }
        co->ground_y = depth;
        height = depth - position[1];
        if (height >= 0.0f) {
            zoid->grounded = 1;
            if (zoid->max_ground_depth < height) {
                zoid->max_ground_depth = height;
            }
        }
        if (flag != 0 && (u32)co->ground_contact != 0) {
            zoid->grounded = 1;
        }
    }
}
extern void zdGrdCoON(ZoidState *zoid, CoObj *object, s32 mode);

void ZoidGrdON(ZoidState *zoid)
{
    ZoidControlData *control;

    control = (ZoidControlData *)zoid->anim_data;
    if (control != 0 && zoid != (void *)0 && zoid->joint != (void *)0) {
        zoid->grounded = 0;
        zoid->max_ground_depth = 0.0f;
        zdGrdCoON(zoid, (CoObj *)zoid->body_coobj, 0);
        zdGrdCoON(zoid, (CoObj *)zoid->head_coobj, control->ground_modes[0] >= 2);
        zdGrdCoON(zoid, (CoObj *)zoid->neck_coobj, control->ground_modes[1] >= 2);
        zdGrdCoON(zoid, (CoObj *)zoid->bust_coobj, 1);
        zdGrdCoON(zoid, (CoObj *)zoid->hip_coobj, 1);
        zdGrdCoON(zoid, (CoObj *)zoid->l_arm1_coobj, 1);
        zdGrdCoON(zoid, (CoObj *)zoid->r_arm1_coobj, 1);
        zdGrdCoON(zoid, (CoObj *)zoid->l_leg1_coobj, 1);
        zdGrdCoON(zoid, (CoObj *)zoid->r_leg1_coobj, 1);
        zdGrdCoON(zoid, (CoObj *)zoid->l_arm2_coobj, 0);
        zdGrdCoON(zoid, (CoObj *)zoid->r_arm2_coobj, 0);
        zdGrdCoON(zoid, (CoObj *)zoid->l_leg2_coobj, 0);
        zdGrdCoON(zoid, (CoObj *)zoid->r_leg2_coobj, 0);
        if (zoid->grounded != 0) {
            zoid->grounded_frames++;
        } else {
            zoid->grounded_frames = 0;
        }
    }
}

s32 zdWeaponWpON(ZoidState *zoid, WeaponMount *mount)
{
    WeaponMountEntry *entry;
    s32 count;
    s32 i;
    register s32 weapon;
    register s32 animation;
    register s32 next;
    s32 slot;
    WeaponSlot *weaponSlot;

    if (mount->active == 0) {
        return 0;
    }
    count = 0;
    for (i = 0; i < 4; i++) {
        entry = &mount->entries[i];
        if (entry->active == 0) {
            continue;
        }
        slot = entry->slot;
        if (slot == -1) {
            entry->active = 0;
            continue;
        }
        weaponSlot = &zoid->weapon_slots[slot];
        weapon = weaponSlot->weapon;
        if (weapon == -1) {
            continue;
        }
        if (sWeaponEndF(weapon) != 0) {
            animation = sWeaponGetAnim(weapon);
            if (animation == 1) {
                sWeaponStopOFF(weapon);
                sWeaponLoopON(weapon);
                continue;
            }
            if (animation == 0) {
                next = 1;
            } else {
                next = 0;
            }
            if (animation == 2) {
                if (sWeaponStopF(weapon) == 0) {
                    continue;
                }
                next = 1;
            }
            sWeaponSetAnim(weapon, next);
        }
        count++;
    }
    return count;
}

s32 zdWeaponWpOFF(ZoidState *zoid, WeaponMount *mount)
{
    WeaponMountEntry *entry;
    s32 count;
    s32 i;
    s32 weapon;
    s32 animation;
    s32 slot;
    s32 next;
    WeaponSlot *weaponSlot;

    if (mount->active == 0) {
        return 0;
    }
    count = 0;
    for (i = 0; i < 4; i++) {
        entry = &mount->entries[i];
        if (entry->active == 0) {
            continue;
        }
        slot = entry->slot;
        if (slot == -1) {
            entry->active = 0;
            continue;
        }
        weaponSlot = &zoid->weapon_slots[slot];
        weapon = weaponSlot->weapon;
        if (weapon == -1) {
            continue;
        }
        sWeaponStopOFF(weapon);
        animation = sWeaponGetAnim(weapon);
        if (animation == 4) {
            continue;
        }
        sWeaponLoopON(weapon);
        if (sWeaponEndF(weapon) != 0) {
            animation = sWeaponGetAnim(weapon);
            if (animation == 4) {
                continue;
            }
            if (animation == 3) {
                next = 4;
            } else {
                next = 3;
            }
            sWeaponSetAnim(weapon, next);
        }
        count++;
    }
    return count;
}

s32 zdWeaponWpForceOFF(ZoidState *zoid, WeaponMount *mount)
{
    s32 weapon;
    WeaponMountEntry *entry;
    s32 i;
    s32 slot;
    s32 animation;
    WeaponSlot *weaponSlot;
    s32 count;

    if (mount->active == 0) {
        return 0;
    }
    count = 0;
    for (i = 0; i < 4; i++) {
        entry = &mount->entries[i];
        if (entry->active == 0) {
            continue;
        }
        slot = entry->slot;
        if (slot == -1) {
            entry->active = 0;
            continue;
        }
        weaponSlot = &zoid->weapon_slots[slot];
        weapon = weaponSlot->weapon;
        if (weapon == -1) {
            continue;
        }
        sWeaponStopOFF(weapon);
        animation = sWeaponGetAnim(weapon);
        if (animation == 4) {
            continue;
        }
        switch (animation) {
        case 0:
        case 1:
        case 2:
            sWeaponSetAnim(weapon, 4);
            break;
        case 3:
            sWeaponLoopON(weapon);
            if (sWeaponEndF(weapon) != 0) {
                sWeaponSetAnim(weapon, 4);
            }
            break;
        }
        count++;
    }
    return count;
}
#define Zoid ((ZoidState *)Zoid)
extern void sWeaponSearchSight(Weapon3 *weapon);

void zdSearchSight(ZoidState *zoid, s32 index)
{
    s32 slot;
    s32 weapon;
    WeaponSlot *weapon_slot;
    WeaponMount *mount;
    s32 i;
    WeaponMountEntry *entry;
    Weapon3 *state;

    if (zoid != (void *)0 && zoid->joint != (void *)0 && index != -1) {
        mount = &zoid->weapon_mounts[index];
        if (mount->active != 0) {
            for (i = 0; i < 4; i++) {
                entry = &mount->entries[i];
                if (entry->active != 0) {
                    slot = entry->slot;
                    if (slot != -1) {
                        weapon_slot = &zoid->weapon_slots[slot];
                        weapon = weapon_slot->weapon;
                        if (weapon != -1) {
                            state = &sWeapon[weapon];
                            if (state->bullet_kind == 1) {
                                sWeaponSearchSight(state);
                            }
                        }
                    }
                }
            }
        }
    }
}
#undef Zoid
s32 zdWeaponScope(ZoidState *zoid, s32 index)
{
    WeaponMount *mount;
    s32 i;
    s32 slot;
    s32 weapon;
    WeaponMountEntry *entry;
    Weapon3 *state;
    WeaponSlot *weapon_slot;

    if (zoid == 0) {
        return 0;
    }
    if (zoid->joint == 0) {
        return 0;
    }
    if (index == -1) {
        return 0;
    }
    mount = &zoid->weapon_mounts[index];
    if (mount->active == 0) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        entry = &mount->entries[i];
        if (entry->active == 0) {
            continue;
        }
        slot = entry->slot;
        if (slot == -1) {
            continue;
        }
        weapon_slot = &zoid->weapon_slots[slot];
        weapon = weapon_slot->weapon;
        if (weapon == -1) {
            continue;
        }
        state = &sWeapon[weapon];
        if (state->scope != 0) {
            return state->scope;
        }
    }
    return 0;
}
typedef struct SliderEntry {
    u16 range;
    u8 joint;
    u8 type;
    s16 min;
    s16 max;
    u8 pad[8];
} SliderEntry;

#pragma push
#pragma dont_inline off
static inline void wpSetScaleX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_485, 805, lit_486);
    jobj->scale.x = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
            HSD_JObjSetMtxDirtySub(jobj);
        }
    }
}

static inline void wpSetScaleY(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_485, 819, lit_486);
    jobj->scale.y = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
            HSD_JObjSetMtxDirtySub(jobj);
        }
    }
}

static inline void wpSetScaleZ(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert(lit_485, 833, lit_486);
    jobj->scale.z = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj)) {
            HSD_JObjSetMtxDirtySub(jobj);
        }
    }
}

s32 zdWp1SliderN(ZoidState *zoid, s32 index, s32 value)
{
    HSD_JObj *jobj;
    s32 joint_index;
    s32 type;
    s32 range;
    f32 min;
    f32 max;
    f32 result;

    if (zoid == 0) {
        return 0;
    }
    if (zoid->slider_positions[index] == value) {
        return 0;
    }
    zoid->slider_positions[index] = value;
    if (zoid->joint == 0) {
        return 0;
    }
    if (zoid->slider_info == 0) {
        return 0;
    }
    joint_index = (s8)((SliderEntry *)zoid->slider_info)[index].joint;
    type = (s8)((SliderEntry *)zoid->slider_info)[index].type;
    if (joint_index == 0) {
        return 0;
    }
    jobj = zoid->spik_joints[joint_index - 1];
    if (jobj == 0) {
        ((SliderEntry *)zoid->slider_info)[index].joint = 0;
        return 0;
    }
    if (index) {
        (void)0;
    }
    range = ((SliderEntry *)zoid->slider_info)[0].range;
    if (range == 0) {
        return 0;
    }
    range = range;
    min = ((SliderEntry *)zoid->slider_info)[index].min;
    max = ((SliderEntry *)zoid->slider_info)[index].max;
    result = min + (max - min) * (f32)value / (f32)range;
    if (type <= 2) {
        result = 2.0 * (3.1415926 * result) / 360.0;
    }
    switch (type) {
    case 0:
        HSD_JObjSetRotationX(jobj, result);
        break;
    case 1:
        HSD_JObjSetRotationY(jobj, result);
        break;
    case 2:
        HSD_JObjSetRotationZ(jobj, result);
        break;
    case 3:
        wpSetScaleX(jobj, result);
        break;
    case 4:
        wpSetScaleY(jobj, result);
        break;
    case 5:
        wpSetScaleZ(jobj, result);
        break;
    }
    return 0;
}
#pragma pop

s32 zdWp1Slider(ZoidState *zoid, s32 index, s32 flag)
{
    s32 value;
    s32 count;

    if (zoid == 0) {
        return 0;
    }
    if (zoid->joint == 0) {
        return 0;
    }
    if (zoid->slider_info == 0) {
        return 0;
    }
    count = zoid->slider_info[0];
    if (count == 0) {
        return 0;
    }
    value = zoid->slider_positions[index];
    if (flag != 0) {
        if (value == count) {
            return 0;
        }
        value += 1;
        if (value > count) {
            value = count;
        }
    } else {
        if (value == 0) {
            return 0;
        }
        value -= 1;
        if (value < 0) {
            value = 0;
        }
    }
    zdWp1SliderN(zoid, index, value);
    return 1;
}

s32 zdWpSlider(ZoidState *zoid, s32 slot)
{
    s32 i;
    s32 target;
    s32 changed;

    if (zoid == 0) {
        return 0;
    }
    if (zoid->joint == 0) {
        return 0;
    }
    target = slot + 1;
    if (slot == -1) {
        target = -1;
    }
    if (slot == 0x13) {
        target = 0;
    }
    changed = 0;
    for (i = 0; i < 11; i++) {
        if (i != target) {
            if (zdWp1Slider(zoid, i, 0) != 0) {
                changed = 1;
            }
        }
    }
    if (changed != 0) {
        return 1;
    }
    if (target == -1) {
        return 0;
    }
    if (zdWp1Slider(zoid, target, 1) != 0) {
        return 1;
    }
    return 0;
}
extern s32 zdWpSlider(ZoidState *zoid, s32 mount);
extern s32 zdWeaponWpForceOFF(ZoidState *zoid, WeaponMount *mount);
extern s32 zdWeaponWpOFF(ZoidState *zoid, WeaponMount *mount);
extern s32 zdWeaponWpON(ZoidState *zoid, WeaponMount *mount);
extern void zdWeaponFireOFF(ZoidState *zoid);
extern void zdWeaponFire1(ZoidState *zoid);
extern void zdSearchSight(ZoidState *zoid, s32 index);
extern void ZoidSetAnim(ZoidState *zoid, s32 animation);
extern s32 zdGetAnimNum(ZoidState *zoid, HSD_JObj *joint);
extern void zdVoiceScG(ZoidState *zoid, s32 voice);

void zdWeaponActLoop(ZoidState *zoid)
{
    ZoidControlData *control;
    u8 *slot_info;
    u8 *info;
    s32 next_index;
    s32 want;
    s32 next;
    s32 ret;
    s32 slot_kind;
    s32 anim_num;
    s32 i;
    s32 idx;
    s32 cur;
    WeaponMount *mount;
    s32 state;
    HSD_JObj *target;
    s32 type;
    s32 kind;
    s32 end;
    s32 off;
    s32 anim;
    s32 mode;

    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    next_index = zoid->weapon_mount + 1;
    info = (u8 *)zoid->slider_info;
    if (zoid->weapon_mount == -1) {
        next_index = -1;
    }
    if (zoid->weapon_mount == 19) {
        next_index = 0;
    }
    slot_info = 0;
    if (next_index != -1) {
        slot_info = (u8 *)(info + 2 + next_index * 16);
    }
    zoid->field_1360 = 0;
    zoid->field_1374 = 0;
    zoid->animation_frame = 0;
    control = (ZoidControlData *)zoid->anim_data;
    if (zoid->weapon_mount != -1) {
        zoid->field_1364 = zdWpSlider(zoid, zoid->weapon_mount);
        type = control->field_0140[zoid->weapon_mount];
        if (type == 2 && zoid->action != 0 && zoid->field_1DB0 < 15) {
            return;
        }
    } else {
        zdWpSlider(zoid, -1);
    }
    if (zoid->health == 0) {
        zoid->selected_weapon_mount = -1;
    }
    if (zoid->requested_action == 27 || zoid->requested_action == 34 || (zoid->action >= 27 && zoid->action <= 33) || (zoid->action >= 34 && zoid->action <= 40)) {
        zoid->super_attack_input = 0;
        if (zoid->selected_weapon_mount == 19) {
            zoid->selected_weapon_mount = -1;
        }
        for (i = 0; i < 11; i++) {
            idx = i;
            if (i == 10) {
                idx = 19;
            }
            zdWeaponWpForceOFF(zoid, &zoid->weapon_mounts[idx]);
            zoid->weapon_mounts[idx].state = 0;
        }
        if (zoid->selected_weapon_mount == -1) {
            return;
        }
        type = control->field_0140[zoid->selected_weapon_mount];
        state = zoid->weapon_mounts[zoid->selected_weapon_mount].state;
        kind = ((s32 (*)[7])wpTbl)[type][state];
        if (type == 2 || kind == 2) {
            zoid->selected_weapon_mount = -1;
            return;
        }
        return;
    }
    if ((zoid->requested_action >= 27 && zoid->requested_action <= 33) || (zoid->action >= 34 && zoid->action <= 40)) {
        slot_kind = zoid->weapon_slots[zoid->weapon_mount].field_04;
        zoid->selected_weapon_mount = -1;
        if (slot_kind == 6) {
            zoid->weapon_mount = -1;
            return;
        }
    }
    if (zoid->weapon_mount == 19 && zoid->selected_weapon_mount == -1) {
        zoid->super_attack_input = 0;
    }
    if (zoid->super_attack_cooldown != 0) {
        zoid->super_attack_cooldown--;
    }
    if (zoid->super_attack_input != 0 && zoid->super_attack_enabled != 0 && zoid->super_attack_cooldown == 0) {
        zoid->selected_weapon_mount = 19;
        zoid->face_lock = 1;
    }
    cur = zoid->weapon_mount;
    want = zoid->selected_weapon_mount;
    if (cur == -1) {
        cur = want;
        zoid->weapon_mount = cur;
        if (cur != -1) {
            mount = &zoid->weapon_mounts[cur];
            mount->state = 0;
        }
    }
    if (cur == -1) {
        zoid->field_1360 = 0;
        zoid->field_1374 = 0;
        return;
    }
    mount = &zoid->weapon_mounts[cur];
    if (cur >= 10) {
        mount->active = 1;
    }
    state = mount->state;
    anim = cur * 5 + 82;
    if (cur == 19) {
        anim = 77;
    }
    type = control->field_0140[cur];
    if (type == 2 || type == 3 || type == 4) {
        if (zoid->jump_phase != 0) {
            if (slot_info == 0) {
                return;
            }
            if ((s8)slot_info[6] != -1) {
                return;
            }
        }
        if (zoid->action == 26) {
            return;
        }
        if (zoid->action == 25) {
            return;
        }
        if (zoid->action == 23) {
            return;
        }
        if (zoid->action == 24) {
            return;
        }
    }
    kind = ((s32 (*)[7])wpTbl)[type][state];
    target = 0;
    target = target;
    zoid->animation_frame = kind;
    if (kind == 2 && state != 0) {
        zoid->field_1360 = 1;
    }
    if (kind == 2) {
        target = zoid->joint->child;
    }
    if (kind == 2) {
        if (zoid->field_1C48 == 0) {
            zoid->field_1C34 = 0;
        }
    } else {
        zoid->field_1360 = 0;
        zoid->field_1374 = 0;
    }
    if (zoid->field_1360 != 0 && zoid->weapon_mount == 19) {
        if ((info[0x711] & 1) && *(void **)(zoid->head_coobj + 0x18) != 0) {
            zoid->ground_turn = 1;
        }
        if ((info[0x711] & 2) && *(void **)(zoid->neck_coobj + 0x18) != 0) {
            zoid->ground_turn = 1;
        }
        if ((info[0x711] & 4) && *(void **)(zoid->bust_coobj + 0x18) != 0) {
            zoid->ground_turn = 1;
        }
    }
    if (cur != want) {
        if (cur < 10 && mount->active == 0) {
            mount->state = 0;
            zdWeaponFireOFF(zoid);
            zoid->weapon_mount = want;
            return;
        }
        if (target == (void *)0) {
            mount->state = 0;
        } else {
            end = 0;
            if (kind == 0) {
                end = 1;
            }
            if (kind == 2) {
                end = zoid->animation_looped | zoid->animation_stopped;
                zoid->field_1374 = 1;
            }
            if (state != 0) {
                if (state == 3) {
                    if (kind == 2) {
                        if (*(void **)(zoid->head_coobj + 0x18) != 0 || *(void **)(zoid->neck_coobj + 0x18) != 0) {
                            zoid->ground_turn = 15;
                        }
                        if (zoid->ground_turn != 0) {
                            zoid->ground_turn = 15;
                        }
                    }
                    if (end == 0) {
                        return;
                    }
                }
                if (state == 2 || state == 3 || end != 0) {
                    next = 3;
                    if (state == 4) {
                        next = 4;
                        if (kind == 2) {
                            ZoidSetAnim(zoid, 1);
                        }
                    } else if (kind == 2) {
                        ZoidSetAnim(zoid, anim + next);
                    }
                    if (next == 3) {
                        mount->state = 4;
                    } else {
                        mount->state = 0;
                        state = 0;
                    }
                }
            }
        }
        if (cur == 19 && zoid->mount_has_weapon[19] == 0) {
            off = 0;
            for (i = 0; i < 10; i++) {
                if (zdWeaponWpOFF(zoid, &zoid->weapon_mounts[i])) {
                    off = 1;
                }
            }
            if (off != 0) {
                return;
            }
        }
        if (zdWeaponWpOFF(zoid, mount) != 0) {
            return;
        }
        if (state != 0) {
            return;
        }
        if (cur == 19) {
            zoid->face_lock = 0;
            zoid->requested_action = 0;
        }
        zoid->weapon_mount = want;
        mount->state = 0;
        return;
    }
    zdSearchSight(zoid, zoid->weapon_mount);
    if (cur == 19 && zoid->mount_has_weapon[19] == 0) {
        for (i = 0; i < 10; i++) {
            zdSearchSight(zoid, i);
        }
    }
    if (kind == 2) {
        mode = zoid->current_animation;
        if (mode == 2) {
            mode = 1;
        }
        if (mode >= 77 && mode <= 157) {
            mode = 1;
        }
        if (state == 0) {
            if (mode != 1 || zoid->field_1C90[2] > 0.1 || zoid->field_1C90[0] > 0.1) {
                return;
            }
        }
        zoid->field_1374 = 1;
        if (zoid->requested_action == 13) {
            zoid->requested_action = 0;
        }
        if (zoid->requested_action >= 23 && zoid->requested_action <= 26) {
            zoid->requested_action = 0;
        }
        if (zoid->requested_action == 2) {
            zoid->requested_action = 0;
        }
    }
    ret = zdWeaponWpON(zoid, mount);
    if (cur == 19 && zoid->mount_has_weapon[19] == 0) {
        for (i = 0; i < 10; i++) {
            zdWeaponWpON(zoid, &zoid->weapon_mounts[i]);
        }
    }
    if (target == 0 && mount->state < 2) {
        mount->state = 2;
        return;
    }
    if (state == 2) {
        if (type == 3 || type == 4) {
            s32 off2;

            off2 = 0;
            if (zoid->jump_phase != 0) {
                off2 = 1;
            }
            if (zoid->action == 26) {
                off2 = 1;
            }
            if (zoid->action == 25) {
                off2 = 1;
            }
            if (zoid->action == 23) {
                off2 = 1;
            }
            if (zoid->action == 24) {
                off2 = 1;
            }
            if (off2 != 0) {
                return;
            }
        }
        if (cur == 19) {
            if (zoid->super_attack_input == 0) {
                if (ret != 0) {
                    return;
                }
                zoid->super_attack_cooldown = 600;
                zdWeaponFire1(zoid);
                mount->state = 3;
                zdVoiceScG(zoid, 6);
                return;
            }
            zdWeaponFireOFF(zoid);
        }
    }
    if (state == 3 || state == 5) {
        if (target != 0) {
            anim_num = zdGetAnimNum(zoid, target);
            if (kind == 2) {
                anim_num = zoid->current_animation;
            }
            if (anim_num != anim + 2) {
                if (kind == 2) {
                    ZoidSetAnim(zoid, anim + 2);
                }
                return;
            }
        }
        if (state == 3) {
            if (cur == 19) {
                zoid->selected_weapon_mount = -1;
            }
            if (kind == 2) {
                if (*(void **)(zoid->head_coobj + 0x18) != 0 || *(void **)(zoid->neck_coobj + 0x18) != 0) {
                    zoid->ground_turn = 15;
                }
            }
            if (zoid->ai != 0 && *(s32 *)((u8 *)zoid->ai + 0x40) != 0 && state == 3 && zoid->animation_looped != 0) {
                mount->state = 4;
            }
            return;
        }
        end = 0;
        if (kind == 2) {
            end = zoid->animation_looped;
        }
        if (end != 0) {
            mount->state = 6;
        }
    }
    if (mount->state == 6) {
        if (kind == 2) {
            ZoidSetAnim(zoid, anim + 1);
        }
        mount->state = 2;
        state = 2;
    }
    kind = ((s32 (*)[7])wpTbl)[type][state];
    if (state < 2 || state == 4) {
        end = 0;
        if (kind == 2) {
            end = zoid->animation_looped;
        }
        if (end != 0 || state == 0) {
            if (kind == 2) {
                ZoidSetAnim(zoid, anim + state);
            }
            mount->state++;
            if (state == 0) {
                mount->state = 1;
            }
            if (state == 1) {
                mount->state = 2;
            }
        }
        return;
    }
    mount->state = 2;
}

s32 zdWeaponNow(ZoidState *zoid)
{
    s32 count;
    s32 i;
    s32 j;
    WeaponMount *mount;
    WeaponMountEntry *entry;

    if (zoid == (void *)0)
        return 0;
    if (zoid->joint == (void *)0)
        return 0;
    if (zoid->death_started != 0)
        return 0;
    if (zoid->selected_weapon_mount == WEAPON_MOUNT_SUPER_ATTACK)
        return 0;
    if (zoid->selected_weapon_mount == -1)
        return 0;
    if (zoid->selected_weapon_mount > 9)
        zoid->selected_weapon_mount = 0;
    count = 0;
    for (i = 0; i < 10; i++) {
        mount = &zoid->weapon_mounts[i];
        if (mount->active == 0)
            continue;
        for (j = 0; j < 4; j++) {
            entry = &mount->entries[j];
            if (entry->active != 0) {
                if (entry->slot != -1)
                    break;
                entry->active = 0;
            }
        }
        if (j == 4)
            mount->active = 0;
        else
            count++;
    }
    zoid->weapon_mount_count = count;
    if (count == 0)
        return 1;
    if (zoid->weapon_mounts[zoid->selected_weapon_mount].active != 0)
        return 1;
    zoid->selected_weapon_mount++;
    if (zoid->selected_weapon_mount > 9)
        zoid->selected_weapon_mount = 0;
    return 0;
}
#define Zoid ((ZoidState *)Zoid)

s32 zdWeaponChg0(ZoidState *zoid)
{
    WeaponMountEntry *entry;
    s32 j;
    WeaponMount *mount;
    s32 i;
    s32 count;

    if (zoid == (void *)0) {
        return 0;
    }
    if (zoid->joint == (void *)0) {
        return 0;
    }
    if (zoid->death_started != 0) {
        return 0;
    }
    count = 0;
    for (i = 0; i < 10; i++) {
        mount = &zoid->weapon_mounts[i];
        if (mount->active != 0) {
            for (j = 0; j < 4; j++) {
                entry = &mount->entries[j];
                if (entry->active != 0) {
                    if (entry->slot == -1) {
                        entry->active = 0;
                    } else {
                        break;
                    }
                }
            }
            if (j == 4) {
                mount->active = 0;
            } else {
                count++;
            }
        }
    }
    zoid->weapon_mount_count = count;
    return count;
}
#undef Zoid
#include "game_mode.h"

extern s32 zdWeaponChg0(ZoidState *zoid);
extern void camSe(void *camera, s32 id, Vec3 *position);
extern void zdWeaponSightClearAll(ZoidState *zoid);

void zdWeaponChg(ZoidState *zoid)
{
    s32 index;
    s32 prev;
    s32 play;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    if (zoid->death_started != 0)
        return;
    prev = zoid->weapon_mount;
    zoid->weapon_mount_count = zdWeaponChg0(zoid);
    if (zoid->weapon_mount_count < 1)
        return;
    if (zoid->weapon_mount_count > 0) {
        for (index = 0; index < 10; index++) {
            zoid->selected_weapon_mount++;
            if (zoid->selected_weapon_mount > 9)
                zoid->selected_weapon_mount = 0;
            if (zoid->weapon_mounts[zoid->selected_weapon_mount].active != 0)
                break;
        }
    }
    if (zoid->selected_weapon_mount == -1)
        return;
    if (prev == zoid->selected_weapon_mount) {
        if (zoid->ai_active != 0)
            return;
        if (zoid->weapon_mount_count > 1)
            return;
        zoid->selected_weapon_mount = -1;
    }
    play = 1;
    if (GameMode.game_type == 9)
        play = 0;
    if (zoid->camera == 0)
        play = 0;
    if (zoid->ai != 0 && zoid->ai_active != 0)
        play = 0;
    if (play != 0)
        camSe(zoid->camera, 0x5b, (Vec3 *)&zoid->position);
    zdWeaponSightClearAll(zoid);
}
#define Zoid ((ZoidState *)Zoid)

void zdWeaponChgSlot(ZoidState *zoid, s32 slot)
{
    s32 i;

    if (zoid != (void *)0 && zoid->joint != (void *)0 && zoid->death_started == 0) {
        zoid->weapon_mount_count = zdWeaponChg0(zoid);
        if (zoid->weapon_mount_count >= 1 && zoid->weapon_mount_count > 0) {
            zoid->selected_weapon_mount = slot;
            if (slot != -1) {
                for (i = 0; i < 10; i++) {
                    if (zoid->selected_weapon_mount > 9) {
                        zoid->selected_weapon_mount = 0;
                    }
                    if (*(s32 *)&zoid->weapon_mounts[zoid->selected_weapon_mount] != 0) {
                        break;
                    }
                    zoid->selected_weapon_mount = zoid->selected_weapon_mount + 1;
                }
            }
        }
    }
}
#undef Zoid

s32 zdWeaponColor(ZoidState *zoid, s32 slot, s32 color)
{
    if (zoid == 0) {
        return 0;
    }
    if (zoid->joint == 0) {
        return 0;
    }
    if (slot < 0) {
        return 0;
    }
    if (slot >= 30) {
        return 0;
    }
    if (sWeaponColor(zoid->weapon_slots[slot].weapon, color, 0) == 0) {
        return 0;
    }
    return 1;
}
#define Zoid ((ZoidState *)Zoid)
void zdWeaponFireVoice(ZoidState *zoid)
{
    if (zoid != 0 && zoid->joint != 0 && *(s32 *)((u8 *)zoid + 0x1B9C) == 0) {
        if (GameMode.game_type == 9) {
            return;
        }
        if (GameMode.game_type == 1 || GameMode.game_type == 4) {
            if (zoid != &Zoid[0]) {
                return;
            }
        }
        *(s32 *)((u8 *)zoid + 0x1B9C) = 300;
        zdVoiceScG(zoid, 3);
    }
}
#undef Zoid

void zdWeaponForceFireOnOff_(ZoidState *zoid, s32 selected, s32 on)
{
    WeaponMount *mount = &zoid->weapon_mounts[selected];
    s32 index;
    s32 animation;
    s32 slot;
    s32 ended;
    s32 next;

    if (mount->active != 0 && (on == 0 || zoid->super_attack_input == 0)) {
        for (index = 0; index < 4; index++) {
            if (mount->entries[index].active != 0 && (slot = mount->entries[index].slot) != -1) {
                if (zoid->death_started != 0) {
                    on = 0;
                    slot = slot;
                } else {
                    animation = sWeaponGetAnim(zoid->weapon_slots[slot].weapon);
                    ended = sWeaponEndF(zoid->weapon_slots[slot].weapon);
                    next = animation;
                    if (on != 0) {
                        switch (animation) {
                        case 0:
                            if (ended != 0) next = 1;
                            break;
                        case 1:
                            next = 2;
                            break;
                        case 2:
                            break;
                        case 3:
                            if (ended != 0) next = 0;
                            break;
                        case 4:
                            next = 0;
                            break;
                        default:
                            next = 0;
                            break;
                        }
                    } else {
                        switch (animation) {
                        case 0:
                            if (ended != 0) next = 3;
                            break;
                        case 1:
                            next = 3;
                            break;
                        case 2:
                            if (ended != 0) next = 3;
                            break;
                        case 3:
                            if (ended != 0) next = 4;
                            break;
                        case 4:
                            next = 4;
                            break;
                        default:
                            next = 4;
                            break;
                        }
                    }
                    if (animation != next || next == 4)
                        sWeaponSetAnim(zoid->weapon_slots[slot].weapon, next);
                    sWeaponLoopOFF(zoid->weapon_slots[slot].weapon);
                    sWeaponStopOFF(zoid->weapon_slots[slot].weapon);
                }
            }
        }
    }
}

void zdWeaponForceFire1_(ZoidState *zoid, s32 selected)
{
    WeaponMount *mount = &zoid->weapon_mounts[selected];
    s32 index;
    s32 slot;

    if (mount->active != 0 && zoid->super_attack_input == 0) {
        for (index = 0; index < 4; index++) {
            if (mount->entries[index].active != 0) {
                slot = mount->entries[index].slot;
                if (slot != -1) {
                    if (zoid->death_started != 0) {
                        sWeaponLoopOFF(zoid->weapon_slots[slot].weapon);
                    } else {
                        if (sWeaponGetAnim(zoid->weapon_slots[slot].weapon) != 2) {
                            sWeaponSetAnim(zoid->weapon_slots[slot].weapon, 2);
                        }
                        sWeaponLoopOFF(zoid->weapon_slots[slot].weapon);
                        sWeaponStopOFF(zoid->weapon_slots[slot].weapon);
                    }
                }
            }
        }
    }
}

void zdWeaponFire1_(ZoidState *zoid, s32 selected)
{
    WeaponMount *mount = &zoid->weapon_mounts[selected];
    s32 slot;
    s32 index;

    if (mount->active != 0 && zoid->super_attack_input == 0) {
        for (index = 0; index < 4; index++) {
            if (mount->entries[index].active != 0) {
                slot = mount->entries[index].slot;
                if (slot != -1) {
                    if (zoid->death_started != 0) {
                        sWeaponLoopOFF(zoid->weapon_slots[slot].weapon);
                    } else {
                        if (sWeaponGetAnim(zoid->weapon_slots[slot].weapon) == 1) {
                            sWeaponSetAnim(zoid->weapon_slots[slot].weapon, 2);
                            sWeaponLoopOFF(zoid->weapon_slots[slot].weapon);
                            sWeaponStopOFF(zoid->weapon_slots[slot].weapon);
                        } else {
                            sWeaponStopOFF(zoid->weapon_slots[slot].weapon);
                            sWeaponLoopOFF(zoid->weapon_slots[slot].weapon);
                        }
                    }
                }
            }
        }
    }
}
#define Zoid ((ZoidState *)Zoid)

void zdWeaponFire1(ZoidState *zoid)
{
    WeaponMount *mount;
    s32 input;
    ZoidControlData *control;
    s32 state;
    s32 index;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        control = (ZoidControlData *)zoid->anim_data;
        if (control->field_0100 == 0) {
            if (zoid->field_1DA0 != -1)
                return;
        }
        if (zoid->weapon_mount == -1)
            return;
        if (zoid->selected_weapon_mount == -1)
            return;
        if (zoid->selected_weapon_mount != zoid->weapon_mount)
            return;
        mount = &zoid->weapon_mounts[zoid->weapon_mount];
        if (mount->active == 0)
            return;
        input = control->field_0140[zoid->weapon_mount];
        if (input == 3 || input == 4) {
            if (zoid->jump_phase != 0)
                return;
            if (zoid->action == 0x1a)
                return;
            if (zoid->action == 0x19)
                return;
            if (zoid->action == 0x17)
                return;
            if (zoid->action == 0x18)
                return;
        }
        state = mount->state;
        if (zoid == &Zoid[0])
            OSReport("fire-1 md=%d \n", state);
        if (state == 2 || state == 3 || state == 4 || state == 5) {
            if (state != 3)
                mount->state = 3;
            if (zoid->weapon_mount != WEAPON_MOUNT_SUPER_ATTACK)
                zdWeaponFireVoice(zoid);
            zoid->anim1_frames = 0;
            if (zoid->weapon_mount == WEAPON_MOUNT_SUPER_ATTACK && zoid->mount_has_weapon[19] == 0) {
                for (index = 0; index < 10; index++)
                    zdWeaponFire1_(zoid, index);
            } else {
                zdWeaponFire1_(zoid, zoid->weapon_mount);
            }
        }
    }
}
#undef Zoid
#undef lit_5655
#define lit_5655 "fire-1 md=%d \n"

void zdWeaponFireON_(ZoidState *zoid, s32 selected)
{
    WeaponMount *mount = &zoid->weapon_mounts[selected];
    s32 slot;
    s32 index;
    s32 animation;

    if (mount->active != 0) {
        zoid->anim1_frames = 0;
        for (index = 0; index < 4; index++) {
            if (mount->entries[index].active != 0) {
                slot = mount->entries[index].slot;
                if (slot != -1) {
                    if (zoid->death_started != 0) {
                        sWeaponLoopOFF(zoid->weapon_slots[slot].weapon);
                    } else {
                        animation = sWeaponGetAnim(zoid->weapon_slots[slot].weapon);
                        if (animation == 2) {
                            sWeaponLoopON(zoid->weapon_slots[slot].weapon);
                            sWeaponStopOFF(zoid->weapon_slots[slot].weapon);
                        }
                        if (animation == 1) {
                            zdWeaponFireVoice(zoid);
                            sWeaponSetAnim(zoid->weapon_slots[slot].weapon, 2);
                            sWeaponLoopON(zoid->weapon_slots[slot].weapon);
                            sWeaponStopOFF(zoid->weapon_slots[slot].weapon);
                        }
                    }
                }
            }
        }
    }
}

void zdWeaponFireON(ZoidState *zoid)
{
    WeaponMount *mount;
    ZoidControlData *control;
    s32 state;
    s32 index;
    s32 control_state;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        control = (ZoidControlData *)zoid->anim_data;
        if (zoid->weapon_mount == -1) {
            zdWeaponChg(zoid);
            return;
        }
        if (zoid->weapon_mount != -1 && zoid->selected_weapon_mount != -1 &&
            zoid->selected_weapon_mount == zoid->weapon_mount) {
            if (zoid->selected_weapon_mount != WEAPON_MOUNT_SUPER_ATTACK) {
                if (zoid->action < 13 || zoid->action > 22) {
                    mount = &zoid->weapon_mounts[zoid->selected_weapon_mount];
                    if (zoid->selected_weapon_mount == WEAPON_MOUNT_SUPER_ATTACK) {
                        mount->active = 1;
                    }
                    if (zoid->selected_weapon_mount != WEAPON_MOUNT_SUPER_ATTACK && mount->active != 0) {
                        control_state = control->field_0140[zoid->selected_weapon_mount];
                        if (control_state == 3 || control_state == 4) {
                            if (zoid->jump_phase != 0) {
                                return;
                            }
                            if (zoid->action == 26) {
                                return;
                            }
                            if (zoid->action == 25) {
                                return;
                            }
                            if (zoid->action == 23) {
                                return;
                            }
                            if (zoid->action == 24) {
                                return;
                            }
                        }
                        state = mount->state;
                        if (state == 2 || state == 3 || state == 4 || state == 5) {
                            if (state != 3) {
                                mount->state = 3;
                            }
                            if (zoid->selected_weapon_mount == WEAPON_MOUNT_SUPER_ATTACK &&
                                zoid->mount_has_weapon[WEAPON_MOUNT_SUPER_ATTACK] == 0) {
                                for (index = 0; index < 10; index++) {
                                    zdWeaponFireON_(zoid, index);
                                }
                            } else {
                                zdWeaponFireON_(zoid, zoid->selected_weapon_mount);
                            }
                        }
                    }
                }
            }
        }
    }
}

void zdWeaponFireOFF_(ZoidState *zoid, s32 selected)
{
    WeaponMount *mount = &zoid->weapon_mounts[selected];
    s32 index;
    s32 slot;
    s32 anim;

    if (mount->active != 0) {
        for (index = 0; index < 4; index++) {
            if (mount->entries[index].active != 0) {
                slot = mount->entries[index].slot;
                if (slot != -1) {
                    anim = sWeaponGetAnim(zoid->weapon_slots[slot].weapon);
                    if (anim == 2) {
                        sWeaponLoopOFF(zoid->weapon_slots[slot].weapon);
                    }
                }
            }
        }
    }
}

void zdWeaponFireOFF(ZoidState *zoid)
{
    WeaponMount *mount;
    s32 state;
    s32 index;
    void *data;
    s32 value;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        data = zoid->anim_data;
        if (zoid->weapon_mount == -1) {
            return;
        }
        mount = &zoid->weapon_mounts[zoid->weapon_mount];
        if (zoid->selected_weapon_mount == WEAPON_MOUNT_SUPER_ATTACK) {
            return;
        }
        if (zoid->weapon_mount == WEAPON_MOUNT_SUPER_ATTACK) {
            return;
        }
        if (mount->active == 0) {
            return;
        }
        state = mount->state;
        if (state == 3) {
            value = *(s32 *)((u8 *)data + 0x140 + zoid->weapon_mount * 4);
            if (zoid->animation_frame == 2 && zoid->animation_looped == 0) {
                return;
            }
            mount->state = 6;
        }
        if (state == 5) {
            mount->state = 6;
        }
        if (zoid->weapon_mount == WEAPON_MOUNT_SUPER_ATTACK && zoid->mount_has_weapon[WEAPON_MOUNT_SUPER_ATTACK] == 0) {
            for (index = 0; index < 10; index++) {
                zdWeaponFireOFF_(zoid, index);
            }
        } else {
            zdWeaponFireOFF_(zoid, zoid->weapon_mount);
        }
    }
}

void zdOptionUseOnOff1(ZoidState *zoid, s32 on)
{
    zdWp1Slider(zoid, 0xb, on);
    zdWeaponForceFireOnOff_(zoid, WEAPON_MOUNT_SHIELD, on);
}

void zdOptionUseOnOff2(ZoidState *zoid, s32 on)
{
    zoid->booster_on = on;
    zdWp1Slider(zoid, 0xC, on);
    zdWeaponForceFireOnOff_(zoid, WEAPON_MOUNT_BOOSTER, on);
}

void zdOptionUse3(ZoidState *zoid)
{
    zoid->option_active = 1;
}

void zdOptionUse4(ZoidState *zoid)
{
    zdWeaponForceFire1_(zoid, WEAPON_MOUNT_SMOKE_MINE);
}

void zdOptionUse5(ZoidState *zoid)
{
    zdWeaponForceFire1_(zoid, WEAPON_MOUNT_SMOKE_MINE);
}

void zdOptionUse(ZoidState *zoid)
{
    if (zoid->option_type == ZOID_OPTION_CAMOUFLAGE)
        zdOptionUse3(zoid);
    if (zoid->option_type == ZOID_OPTION_SMOKE)
        zdOptionUse4(zoid);
    if (zoid->option_type == ZOID_OPTION_MINE)
        zdOptionUse5(zoid);
}
extern void zdOptionUseOnOff1(ZoidState *zoid, s32 on);
extern void zdOptionUseOnOff2(ZoidState *zoid, s32 on);

void zdOptionUseOnOff(ZoidState *zoid, s32 on)
{
    if (zoid->option_type == 1)
        zdOptionUseOnOff1(zoid, on);
    if (zoid->option_type == 2)
        zdOptionUseOnOff2(zoid, on);
}

void zdOptionUseOK(ZoidState *zoid)
{
    s32 motion = zoid->current_animation;
    switch (zoid->option_type) {
    case 1:
        if (motion == 7) {
            zoid->option_use_state = 0;
        }
        break;
    }
}
extern void zdOptionUseOK(ZoidState *zoid);
extern void zdOptionUse(ZoidState *zoid);
extern void zdOptionUseOnOff(ZoidState *zoid, s32 on);

void zdOptionLoop(ZoidState *zoid)
{
    void *ai;
    s32 add;

    ai = zoid->ai;
    zoid->option_active = 0;
    if (zoid->option_energy_max == 0)
        return;
    if (zoid->super_attack_input != 0)
        zoid->option_use_state = 0;
    if (zoid->option_energy < 0)
        zoid->option_energy = 0;
    zdOptionUseOK(zoid);
    if (ai != 0 && *(s32 *)((u8 *)ai + 0x94) != 0)
        zoid->option_use_state = 1;
    if (zoid->death_started != 0)
        zoid->option_use_state = 0;
    if (zoid->option_use_state != 0) {
        if (zoid->option_energy == 0)
            zoid->option_use_state = 0;
        if (zoid->option_use_previous == 0 && zoid->option_energy_max != zoid->option_energy)
            zoid->option_use_state = 0;
        if (zoid->option_use_state == 0 && ai != 0 && *(s32 *)((u8 *)ai + 0x94) != 0)
            *(s32 *)((u8 *)ai + 0x94) = 0;
    }
    if (zoid->option_use_state != 0) {
        if (zoid->option_use_previous != 0)
            zoid->option_energy -= zoid->option_hold_cost;
        else
            zoid->option_energy -= zoid->option_start_cost;
        zdOptionUse(zoid);
    } else {
        add = zoid->option_recharge;
        if (*(s32 *)((u8 *)zoid + 0x180) != 0 && *(s32 *)((u8 *)zoid + 0x17C) == 0)
            add /= 2;
        if (add == 0 && zoid->option_recharge != 0)
            add = 1;
        zoid->option_energy += add;
        if (zoid->option_energy >= zoid->option_energy_max)
            zoid->option_energy = zoid->option_energy_max;
    }
    zdOptionUseOnOff(zoid, zoid->option_use_state);
    zoid->option_use_previous = zoid->option_use_state;
}
void zdSmokeRecv(ZoidState *zoid)
{
    if (zoid != 0 && zoid->joint != 0) {
        if (*(u16 *)((u8 *)zoid + 0x1D50) != 0) {
            (*(u16 *)((u8 *)zoid + 0x1D50))--;
        }
        if (zoid->enemy_smoke_timer != 0) {
            zoid->enemy_smoke_timer--;
            zoid->target_pending = 0;
            *(s32 *)((u8 *)zoid->ai + 0x48) = -1;
        }
    }
}
#define Zoid ((ZoidState *)Zoid)
void zdLockOnOff(ZoidState *zoid)
{
    s32 index;
    ZoidState *target;

    if (zoid == 0) {
        return;
    }
    if (zoid->joint == 0) {
        return;
    }
    if (zoid->death_started != 0) {
        return;
    }
    if (zoid->target_pending == 0) {
        zoid->target_slot = 0;
    } else {
        index = zoid->target_slot;
        if (index < 0 || index >= 16) {
            index = -1;
        }
        if (index != -1) {
            target = &Zoid[index];
            if (target->lock_frames > 60) {
                index = -1;
            }
        }
        if (index == -1) {
            zoid->target_slot = 0;
            zoid->target_pending = 0;
        }
    }
}
#undef Zoid
extern void zdLockOnOff(ZoidState *zoid);
extern s32 zdWeaponNow(ZoidState *zoid);
extern void zdWeaponActLoop(ZoidState *zoid);
extern void zdOptionLoop(ZoidState *zoid);

#define Zoid ((ZoidState *)Zoid)
void ZoidWeaponActLoop(void)
{
    s32 i;
    ZoidState *zoid;
    s32 weapon_available;

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint != 0) {
            zdLockOnOff(zoid);
            zdWeaponNow(zoid);
            zdWeaponActLoop(zoid);
            zdOptionLoop(zoid);
            zdSmokeRecv(zoid);
            if (zoid->weapon_mount != -1) {
                weapon_available = 1;
            }
        }
    }
}
#undef Zoid
void zdAtack1(ZoidState *zoid)
{
    if (zoid == (void *)0) return;
    if (zoid->joint == (void *)0) return;
    zoid->requested_action = 13;
}
namespace { extern const f64 lit_5938[1]; extern const f64 lit_5939[1]; extern const f64 lit_5940[1]; }
extern void angLimit(f32 *angle);
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);


#pragma push
#pragma dont_inline off
#pragma auto_inline off
void zdStep0f(ZoidState *zoid, f32 angle)
{
    s32 busy;
    f32 magnitude;
    f32 heading;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        if (zoid->action == 0x2a) return;
        if (zoid->action == 0x2b) return;
        if (zoid->requested_action >= 0x1b && zoid->requested_action <= 0x28) return;
        if (zoid->action >= 0x1b && zoid->action <= 0x28) return;
        busy = 0;
        if (zoid->action == 0x17) busy = 1;
        if (zoid->action == 0x18) busy = 1;
        if (zoid->action == 0x19) busy = 1;
        if (zoid->action == 0x1a) busy = 1;
        if (zoid->action == 0x2d) busy = 1;
        if (zoid->action == 0x2e) busy = 1;
        if (zoid->action == 0x2f) busy = 1;
        if (zoid->action == 0x30) busy = 1;
        if (zoid->action >= 0xd && zoid->action <= 0x16) busy = 1;
        if (zoid->jump_phase != 0) busy = 1;
        if (busy != 0) {
            zoid->requested_action = 0;
        } else {
            angLimit(&angle);
            heading = zoid->heading + angle;
            zoid->launch_velocity[0] = lit_5938[0] * zoid->displacement[0] + sinf(heading);
            zoid->launch_velocity[2] = lit_5938[0] * zoid->displacement[2] + cosf(heading);
            magnitude = absf(angle);
            zoid->requested_action = 0x2d;
            if (magnitude < lit_5939[0]) {
                zoid->requested_action = 0x2d;
            } else if (magnitude > lit_5940[0]) {
                zoid->requested_action = 0x2e;
            } else if (angle > 0.0f) {
                zoid->requested_action = 0x30;
            } else {
                zoid->requested_action = 0x2f;
            }
        }
    }
}
#pragma pop
void zdStep1(ZoidState *zoid, s32 degrees)
{
    if (zoid == (void *)0) {
        return;
    }
    if (zoid->joint == (void *)0) {
        return;
    }
    if (zoid->action == 0x2a) {
        return;
    }
    if (zoid->action == 0x2b) {
        return;
    }
    zoid->step_heading = (2.0 * (3.1415926 * (f32)degrees)) / 360.0 + zoid->heading;
    angLimit(&zoid->step_heading);
    zoid->requested_action = 0x17;
}

f32 zdGoY(ZoidState *zoid, f32 angle, f32 distance)
{
    f32 origin[3];
    f32 target[3];
    s32 axis;
    f32 a0;
    f32 a1;

    if (zoid == (void *)0) {
        return 0.0f;
    }
    if (zoid->joint == (void *)0) {
        return 0.0f;
    }
    for (axis = 0; axis < 3; axis++) {
        origin[axis] = zoid->position[axis];
        target[axis] = zoid->position[axis];
    }
    origin[1] = zoid->ground_height;
    target[1] = zoid->ground_height;
    a0 = angle + zoid->heading;
    target[0] += distance * sinf(a0);
    a1 = angle + zoid->heading;
    target[2] += distance * cosf(a1);
    target[1] = zdColPosY(target, 0);
    return target[1] - origin[1];
}
extern f32 atan2f(f32 y, f32 x);

f32 zdGoYA(ZoidState *zoid, f32 angle, f32 distance)
{
    register f32 result;
    f32 ny;
    f32 origin[3];
    f32 target[3];
    u16 selector;
    f32 mtx[3][4];
    Vec3 normal;
    s32 axis;
    f32 pb;
    f32 sin_angle;
    f32 cos_angle;
    f32 dy;

    if (zoid == (void *)0)
        return 0.0f;
    if (zoid->joint == (void *)0)
        return 0.0f;
    if (0.0f == distance) {
        Vec3 *ground;
        if (zoid->ground_polygon == GROUND_POLYGON_NONE)
            return 0.0f;
        ground = &grdcNormal[grdcPolygon[zoid->ground_polygon].normal_index];
        PSMTXRotRad(mtx, 'Y', -zoid->heading - angle);
        PSMTXMultVecSR(mtx, ground, &normal);
        ny = normal.y;
        result = -atan2f(normal.z, ny);
        return result;
    }
    for (axis = 0; axis < 3; axis++) {
        origin[axis] = zoid->position[axis];
        target[axis] = zoid->position[axis];
    }
    origin[1] = zoid->ground_height;
    target[1] = zoid->ground_height;
    sin_angle = angle + zoid->heading;
    target[0] += distance * sinf(sin_angle);
    cos_angle = angle + zoid->heading;
    target[2] += distance * cosf(cos_angle);
    target[1] = zdColPosY(target, &selector);
    dy = target[1] - origin[1];
    pb = atan2f(dy, distance);
    return pb;
}

void zdGetBodyPt(ZoidState *zoid, Vec3 *point)
{
    HSD_JObj *joint;
    s32 i;

    for (i = 0; i < 3; i++) {
        ((f32 *)point)[i] = zoid->position[i];
    }
    joint = zoid->buckle_joint;
    if (joint == (void *)0) {
        joint = *(HSD_JObj **)zoid->bust_coobj;
    }
    if (joint == (void *)0) {
        joint = *(HSD_JObj **)zoid->hip_coobj;
    }
    if (joint == (void *)0) {
        joint = *(HSD_JObj **)zoid->body_coobj;
    }
    if (joint == (void *)0) {
        joint = *(HSD_JObj **)zoid->bust_coobj;
    }
    if (joint == (void *)0) {
        return;
    }
    for (i = 0; i < 3; i++) {
        ((f32 *)point)[i] = joint->matrix[i][3];
    }
}
extern void ik3dOn(ZoidState *zoid, FootJoint *foot);
extern void HSD_JObjAnimAll(HSD_JObj *joint);

void zdChgNormal(ZoidState *zoid)
{
    ZoidControlData *control;
    Vec3 body;
    Vec3 body2;
    f32 ground;
    f32 depth;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        control = (ZoidControlData *)zoid->anim_data;
        zdSetXYZ(zoid);
        sMakeMatrix(zoid->joint);
        zdGetBodyPt(zoid, &body);
        ZoidSetAnim(zoid, 1);
        zdLinierTm(zoid, 30);
        zoid->height_offset_speed = 0.0f;
        zoid->displacement[1] = 0.0f;
        ground = zdColPosY(zoid->position, 0);
        depth = ground - zoid->position[1];
        if (depth > 0.0f) {
            zoid->field_1C24[1] = ground;
            zoid->position[1] = ground;
            zoid->height_offset -= depth;
        }
        zoid->height_offset = 0.0f;
        zdSetXYZ(zoid);
        HSD_JObjAnimAll(zoid->joint);
        zdSetXYZ(zoid);
        body2.y = zoid->field_1BF8 + (zoid->position[1] + zoid->height_offset);
        zoid->field_1C34 = 0;
        depth = body2.y - body.y;
        if (depth > 0.0f) {
            zoid->height_offset -= depth;
        }
        zdSetXYZ(zoid);
        sMakeMatrix(zoid->joint);
        ik3dOn(zoid, &zoid->feet[2]);
        ik3dOn(zoid, &zoid->feet[3]);
        if (control->field_00F4 != 2) {
            ik3dOn(zoid, &zoid->feet[0]);
            ik3dOn(zoid, &zoid->feet[1]);
        }
        zdLinierReset(zoid, zoid->feet[2].joint);
        zdLinierReset(zoid, zoid->feet[3].joint);
        if (control->field_00F4 != 2) {
            zdLinierReset(zoid, zoid->feet[0].joint);
            zdLinierReset(zoid, zoid->feet[1].joint);
        }
        zoid->action = 0;
        zoid->requested_action = 0;
        zoid->displacement[0] = 0.0f;
        zoid->displacement[2] = 0.0f;
    }
}

void zDispCursorPrologue(void)
{
    csrFile = LoadArc2("debug/mtp_grid.dat");
}
#undef lit_6045
#define lit_6045 "debug/mtp_grid.dat"

void zDispCursorEpilogue(void)
{
    PurgeArc2(csrFile);
    csrFile = 0;
}
extern void SetAnimArc2(ModelArchive *archive, s32 model, s32 animation);

#pragma push
#pragma dont_inline off
void zdDispCursorSet(ZoidState *zoid, s32 slot, s32 unused)
{
    f32 matrix[3][4];
    f32 (*source)[4];
    s32 i;
    s32 j;
    WeaponSlot *weapon;

    if (csrFile != (void *)0 && zoid != (void *)0 && zoid->joint != (void *)0) {
        csrDispON = 0;
        if (slot >= 0) {
            weapon = &zoid->weapon_slots[slot];
            if (weapon->joint != (void *)0) {
                csrDispON = 1;
                source = weapon->joint->matrix;
                slot = 0;
                if (csrNum != slot) {
                    SetAnimArc2(csrFile, 0, slot);
                    csrNum = slot;
                }
                jobjAnim(csrFile->models[0]);
                for (i = 0; i < 3; i++) {
                    for (j = 0; j < 4; j++) {
                        matrix[i][j] = source[i][j];
                        csrFile->models[0]->matrix[i][j] = source[i][j];
                    }
                }
                sGlobal2Local(csrFile->models[0]);
                HSD_JObjSetTranslateX(csrFile->models[0], matrix[0][3]);
                HSD_JObjSetTranslateY(csrFile->models[0], matrix[1][3]);
                HSD_JObjSetTranslateZ(csrFile->models[0], matrix[2][3]);
                sMakeMatrix(csrFile->models[0]);
            }
        }
    }
    unused = unused;
}
#pragma pop

void zdDispCursor(void)
{
    if (csrFile != (ModelArchive *)0 && csrDispON != 0)
        jobjDispAll(csrFile->models[0]);
}
#define Zoid ((ZoidState *)Zoid)

u8 *skipPicData(u8 *data)
{
    s32 alpha;
    u8 chained;
    s32 run;
    s32 i;
    s32 x;
    u16 width;
    u32 magic;
    u16 height;
    s32 y;

    magic = *(u32 *)data;
    data += 4;
    if (magic != 0x5380F634) {
        return 0;
    }
    width = *(u16 *)(data + 88);
    height = *(u16 *)(data + 90);
    data += 100;
    alpha = 0;
    do {
        chained = data[0];
        alpha |= data[3];
        data += 4;
    } while (chained != 0);
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x += run) {
            run = *data++;
            if (run == 128) {
                run = *(u16 *)data;
                data += 5;
            } else if (run > 127) {
                run -= 127;
                data += 3;
            } else {
                run++;
                for (i = 0; i < run; i++) {
                    data += 3;
                }
            }
        }
        if (alpha & 0x10) {
            for (x = 0; x < width; x += run) {
                run = *data++;
                if (run == 128) {
                    run = *(u16 *)data;
                    data += 3;
                } else if (run > 127) {
                    run -= 127;
                    data += 1;
                } else {
                    run++;
                    for (i = 0; i < run; i++) {
                        data += 1;
                    }
                }
            }
        }
    }
    return data;
}
#undef Zoid
typedef struct PicHeader {
    u8 reserved_00[0x10];
    s32 format;
    u8 reserved_14[4];
    s32 width;
    s32 height;
    u8 reserved_20[0x2C];
    u16 colors;
} PicHeader;
extern s32 calc_doltexture_size(u32 format, s32 width, s32 height);
extern void putErr(char *s);
extern void DCFlushRange(void *memory, u32 size);
extern void GXInvalidateTexAll(void);

#define Zoid ((ZoidState *)Zoid)

s32 zdSetTex(u8 *data, u8 *texture, u32 size)
{
    PicHeader *pic;
    PicHeader *hdr;
    s32 colors;
    u8 *image;
    u32 tsize;

    pic = (PicHeader *)skipPicData(data);
    hdr = pic;
    if (hdr->format != 10) {
        colors = 256;
    } else {
        colors = hdr->colors;
        if (colors > 1024) {
            putErr("tex-size  large      >1024");
            return 0;
        }
    }
    image = (u8 *)pic + 744 + (colors - 256) * 2;
    tsize = calc_doltexture_size(hdr->format, hdr->width, hdr->height);
    tsize = tsize;
    if (tsize != size) {
        putErr("tex size \201\202 file-tex-size");
        OSReport("tex size=%d(%d,%d,[%d])    != tsize=%d \n", tsize, hdr->width, hdr->height, hdr->format, size);
        if (hdr->format != 14) {
            putErr("tex size \201\202 file-tex-size    file-tex not CMPR");
            OSReport("file -tex   not CMPR\n");
        }
        return 0;
    }
    OSReport("headsize=%d    size = %d \n", 744, tsize);
    MemCpy(image, texture, tsize);
    DCFlushRange(image, tsize);
    DCFlushRange(texture, tsize);
    GXInvalidateTexAll();
    return 1;
}
#undef Zoid
#undef lit_6148
#define lit_6148 "tex-size  large      >1024"
#undef lit_6149
#define lit_6149 "tex size \x81\x82 file-tex-size"
#undef lit_6150
#define lit_6150 "tex size=%d(%d,%d,[%d])    != tsize=%d \n"
#undef lit_6151
#define lit_6151 "tex size \x81\x82 file-tex-size    file-tex not CMPR"
#undef lit_6152
#define lit_6152 "file -tex   not CMPR\n"
#undef lit_6153
#define lit_6153 "headsize=%d    size = %d \n"
namespace { extern const char lit_6185[6]; extern const char lit_6187[5]; }
typedef struct ColorImageDesc {
    u8 *image;
    u16 width;
    u16 height;
    s32 format;
} ColorImageDesc;
typedef struct ColorTObj {
    u8 reserved_00[0x58];
    ColorImageDesc *imagedesc;
} ColorTObj;
extern s32 SetTex2(void *data, void *dest, s32 size);
extern void putErr(char *s);
extern void *sReadFileU(char *name, s32 usb);
extern s32 calc_doltexture_size(u32 format, s32 width, s32 height);
extern ColorTObj *searchTObj(void *obj);
extern s32 zdSetTex(u8 *data, u8 *texture, u32 size);

s32 ZoidColor(ZoidState *zoid, s32 color, s32 mode)
{
    char path[256];
    ColorImageDesc *desc;
    u8 *image;
    ColorTObj *tobj;
    u8 *data;
    HSD_JObj *joint;
    u32 tsize;

    if (color < 0)
        return 0;
    if (zoid == (void *)0)
        return 0;
    if (zoid->joint == (void *)0)
        return 0;
    if (zoid->field_1E9A == color)
        return 1;
    joint = *(HSD_JObj **)zoid->body_coobj;
    if (joint == (void *)0) {
        putErr("zoidColor : Zoid.coBody not found.");
        return 0;
    }
    tobj = searchTObj(joint);
    if (tobj == (void *)0) {
        putErr("zoidColor : tobj not found.");
        return 0;
    }
    desc = tobj->imagedesc;
    if (desc == (void *)0) {
        putErr("zoidColor : imagedesc(tobj) not found.");
        return 0;
    }
    data = data;
    data = data;
    tsize = calc_doltexture_size(desc->format, desc->width, desc->height);
    image = desc->image;
    sprintf(path, "zoid/all_tex/%s_%c.tpl", zoid, color + 'A');
    if ((data = (u8 *)sReadFileU(path, mode))) {
        if (SetTex2(data, image, tsize) != 0) {
            zoid->field_1E9A = color;
            free(data);
            OSReport(lit_6185);
            return 1;
        }
    } else {
        sprintf(path, "zoid/all_tex/%s_%c.pic", zoid, color + 'A');
        if ((data = (u8 *)sReadFileU(path, mode)) && zdSetTex(data, image, tsize) != 0) {
            zoid->field_1E9A = color;
            free(data);
            OSReport(lit_6187);
            return 1;
        }
    }
    if (data == (void *)0) {
        putErr("zoidColor : file not found.");
        return 0;
    }
    OSReport("text set. error   zd-tex (%d,%d,[%d])\n", desc->width, desc->height, desc->format);
    if (desc->format != 14) {
        putErr("zoidColor : set Tex  error    zoid-tex not CMPR");
        OSReport("zoid -tex   not CMPR\n");
    }
    free(data);
    return 0;
}
#undef lit_6181
#define lit_6181 "zoidColor : Zoid.coBody not found."
#undef lit_6182
#define lit_6182 "zoidColor : tobj not found."
#undef lit_6183
#define lit_6183 "zoidColor : imagedesc(tobj) not found."
#undef lit_6184
#define lit_6184 "zoid/all_tex/%s_%c.tpl"
#undef lit_6186
#define lit_6186 "zoid/all_tex/%s_%c.pic"
#undef lit_6188
#define lit_6188 "zoidColor : file not found."
#undef lit_6189
#define lit_6189 "text set. error   zd-tex (%d,%d,[%d])\n"
#undef lit_6190
#define lit_6190 "zoidColor : set Tex  error    zoid-tex not CMPR"
#undef lit_6191
#define lit_6191 "zoid -tex   not CMPR\n"
#include "gx_tev.h"
extern GXColor SkyAmbient;
extern void jobjAmbient(HSD_JObj *joint, GXColor color);

void zdAmbientSetSky(ZoidState *zoid)
{
    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        jobjAmbient(zoid->joint, SkyAmbient);
        OSReport("set ambient[%s] ", zoid->name);
    }
}
#undef lit_6208
#define lit_6208 "set ambient[%s] "
#pragma force_active on
static const u32 lit_6474[252] = {
    0x43BB8000, 0xC3BB8000, 0x00000000, 0x43BB8000, 0x433B0000, 0x00000000, 0x433B0000, 0xC33B0000, 
    0x00000000, 0x00000000, 0x433B0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x433B0000, 0xC3BB8000, 0x00000000, 0x433B0000, 
    0xC46A4000, 0x00000000, 0x00000000, 0xC3BB8000, 0x00000000, 0x00000000, 0xC46A4000, 0x00000000, 
    0x433B0000, 0x43BB8000, 0x00000000, 0x440C8000, 0x440C8000, 0x00000000, 0x433B0000, 0x440C8000, 
    0x00000000, 0x440C8000, 0x443B8000, 0x00000000, 0x43BB8000, 0xC3BB8000, 0x00000000, 0x43BB8000, 
    0x433B0000, 0x00000000, 0x433B0000, 0xC33B0000, 0x00000000, 0x00000000, 0x433B0000, 0x00000000, 
    0xC3BB8000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xC3BB8000, 0x433B0000, 
    0x00000000, 0x00000000, 0x433B0000, 0x00000000, 0xC3BB8000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0xC3BB8000, 0x433B0000, 0x00000000, 0x00000000, 0x433B0000, 0x00000000, 
    0xC33B0000, 0xC33B0000, 0x00000000, 0x433B0000, 0x433B0000, 0x00000000, 0x433B0000, 0xC33B0000, 
    0x00000000, 0xC33B0000, 0x433B0000, 0x00000000, 0xC33B0000, 0xC33B0000, 0x00000000, 0x433B0000, 
    0x433B0000, 0x00000000, 0x433B0000, 0xC33B0000, 0x00000000, 0xC33B0000, 0x433B0000, 0x00000000, 
    0xC3BB8000, 0x00000000, 0x00000000, 0xC3BB8000, 0x43BB8000, 0x00000000, 0xC33B0000, 0x00000000, 
    0x00000000, 0xC33B0000, 0x43BB8000, 0x00000000, 0xC3BB8000, 0x00000000, 0x00000000, 0xC3BB8000, 
    0x43BB8000, 0x00000000, 0xC33B0000, 0x00000000, 0x00000000, 0xC33B0000, 0x43BB8000, 0x00000000, 
    0xC33B0000, 0xC33B0000, 0x00000000, 0x433B0000, 0x433B0000, 0x00000000, 0x433B0000, 0xC33B0000, 
    0x00000000, 0xC33B0000, 0x433B0000, 0x00000000, 0xC33B0000, 0xC33B0000, 0x00000000, 0x433B0000, 
    0x433B0000, 0x00000000, 0x433B0000, 0xC33B0000, 0x00000000, 0xC33B0000, 0x433B0000, 0x00000000, 
    0xC4048000, 0x43FA0000, 0x00000000, 0x443B8000, 0xC4020000, 0x00000000, 0x43340000, 0xC3EB0000, 
    0x00000000, 0x41200000, 0xC3E60000, 0x00000000, 0xC4048000, 0x43FA0000, 0x00000000, 0x443B8000, 
    0xC4020000, 0x00000000, 0x43340000, 0xC3EB0000, 0x00000000, 0x41200000, 0xC3E60000, 0x00000000, 
    0xC4048000, 0x444A8000, 0x00000000, 0x44610000, 0xC4020000, 0x00000000, 0x43340000, 0xC3EB0000, 
    0x00000000, 0x41200000, 0xC3E60000, 0x00000000, 0xC33B0000, 0xC33B0000, 0x00000000, 0x433B0000, 
    0x433B0000, 0x00000000, 0x433B0000, 0xC33B0000, 0x00000000, 0xC33B0000, 0x433B0000, 0x00000000, 
    0xC33B0000, 0xC33B0000, 0x00000000, 0x433B0000, 0x433B0000, 0x00000000, 0x433B0000, 0xC33B0000, 
    0x00000000, 0xC33B0000, 0x433B0000, 0x00000000, 0xC33B0000, 0xC33B0000, 0x00000000, 0x433B0000, 
    0x433B0000, 0x00000000, 0x433B0000, 0xC33B0000, 0x00000000, 0xC33B0000, 0x433B0000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
#pragma force_active off
extern void zdAmbientSetSky(ZoidState *zoid);

#define Zoid ((ZoidState *)Zoid)
void ZoidAmbientSetSkyAll(void)
{
    s32 i;
    for (i = 0; i < 16; i++) {
        zdAmbientSetSky(&Zoid[i]);
    }
}
#undef Zoid
extern f64 __frsqrte(f64);
extern void PSMTXRotTrig(f32 m[3][4], s32 axis, f32 sin, f32 cos);
extern void PSMTXCopy(f32 src[3][4], f32 dst[3][4]);
extern void HSD_JObjDispAll(HSD_JObj *joint, s32 a, s32 b, s32 c);

#pragma push
#pragma dont_inline off
static inline f32 sdSqrtf(f32 x)
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

#define sdSetMtxDirty(jobj) \
    do { \
        if ((jobj) != 0 && !HSD_JObjMtxIsDirty(jobj)) \
            HSD_JObjSetMtxDirtySub(jobj); \
    } while (0)

static inline void sdJObjCopyMtx(HSD_JObj *jobj, f32 mtx[3][4])
{
    jobj ? (void)0 : __assert(lit_485, 1173, lit_486);
    mtx ? (void)0 : __assert(lit_485, 1174, "mtx");
    PSMTXCopy(mtx, jobj->matrix);
    jobj->flags |= 0x03800000;
    sdSetMtxDirty(jobj);
}

static inline void sdJObjClearUserMtx(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert(lit_485, 1199, lit_486);
    jobj->flags &= ~0x03800000;
    sdSetMtxDirty(jobj);
}

void sdDisp1(HSD_JObj *joint)
{
    Vec3 *normal;
    f32 m[3][4];
    f32 x;
    f32 rot[3][4];
    f32 z;
    s32 j;
    f32 pos[3];
    u16 selector;
    f32 len;
    GroundPolygon *poly;
    f32 y;
    s32 i;

    if (joint == (void *)0)
        return;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++)
            m[i][j] = joint->matrix[i][j];
        pos[i] = m[i][3];
    }
    for (j = 0; j < 3; j++) {
        m[1][j] *= 0.1;
        m[0][j] *= 2.0f;
        m[2][j] *= 2.0f;
    }
    m[1][3] = zdColPosY(pos, &selector);
    poly = 0;
    if (selector != GROUND_POLYGON_NONE)
        poly = &grdcPolygon[selector];
    if (poly != 0) {
        normal = &grdcNormal[poly->normal_index];
        x = normal->x;
        y = normal->y;
        len = sdSqrtf(x * x + y * y);
        if (len > 0.0f) {
            x /= len;
            y /= len;
            PSMTXRotTrig(rot, 'Z', -x, -y);
            PSMTXConcat(m, rot, m);
        }
        y = normal->y;
        z = normal->z;
        len = sdSqrtf(z * z + y * y);
        if (len > 0.0f) {
            z /= len;
            y /= len;
            PSMTXRotTrig(rot, 'X', -z, -y);
            PSMTXConcat(m, rot, m);
        }
        y = normal->y;
        x = normal->x;
        z = normal->z;
        len = sdSqrtf(x * x + y * y + z * z);
        if (len) {
            y /= len;
            for (j = 0; j < 3; j++) {
                m[0][j] /= y;
                m[2][j] /= y;
            }
        }
    }
    sdJObjCopyMtx(sdTest->models[0], m);
    HSD_JObjDispAll(sdTest->models[0], 0, 7, 0);
    sdJObjClearUserMtx(sdTest->models[0]);
}
#undef sdSetMtxDirty
#pragma pop

void sdZoidParts(ZoidState *zoid)
{
    s32 i;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        sdDisp1(*(HSD_JObj **)zoid->head_coobj);
        sdDisp1(*(HSD_JObj **)zoid->neck_coobj);
        sdDisp1(*(HSD_JObj **)zoid->bust_coobj);
        sdDisp1(*(HSD_JObj **)zoid->hip_coobj);
        for (i = 0; i < 8; i++) {
            sdDisp1(*(HSD_JObj **)zoid->tail_coobjs[i]);
        }
        sdDisp1(*(HSD_JObj **)zoid->l_arm1_coobj);
        sdDisp1(*(HSD_JObj **)zoid->l_arm2_coobj);
        sdDisp1(*(HSD_JObj **)zoid->r_arm1_coobj);
        sdDisp1(*(HSD_JObj **)zoid->r_arm2_coobj);
        sdDisp1(*(HSD_JObj **)zoid->l_leg1_coobj);
        sdDisp1(*(HSD_JObj **)zoid->l_leg2_coobj);
        sdDisp1(*(HSD_JObj **)zoid->r_leg1_coobj);
        sdDisp1(*(HSD_JObj **)zoid->r_leg2_coobj);
    }
}
typedef struct SdPEV2 {
    u8 pad_00[8];
    u8 z_comp;
} SdPEV2;
typedef struct SdMObjV2 {
    u8 pad_00[0x10];
    SdPEV2 *pe;
} SdMObjV2;
typedef struct SdDObjV2 {
    u8 pad_00[8];
    SdMObjV2 *mobj;
} SdDObjV2;
extern s32 nowShadow;
extern void sdZoidParts(ZoidState *zoid);

#define Zoid ((ZoidState *)Zoid)
void sdZoidAll(void)
{
    s32 i;

    if (nowShadow == 1) {
        ((SdDObjV2 *)sdTest->models[0]->display_object)->mobj->pe->z_comp = 1;
        for (i = 0; i < 16; i++) {
            sdZoidParts(&Zoid[i]);
        }
    }
}
#undef Zoid
namespace { extern const f32 lit_4347[1]; extern const f32 lit_6311[1]; extern const f32 lit_6316[1]; }
extern f32 sColPtAll(Vec3 *start, Vec3 *end, GroundMask *mask);

s32 zdFaceAtk(ZoidState *zoid, f32 distance, f32 offset)
{
    Vec3 origin;
    Vec3 target;
    Vec3 start;
    f32 angle;
    HSD_JObj *joint;
    s32 i;
    f32 res;
    f32 two = 2.0f;
    f32 sine_angle;
    f32 t1;
    f32 t2;
    f32 t3;

    joint = 0;
    if (joint == 0) {
        joint = *(HSD_JObj **)zoid->head_coobj;
    }
    if (joint == 0) {
        joint = *(HSD_JObj **)zoid->neck_coobj;
    }
    if (joint == 0) {
        joint = *(HSD_JObj **)zoid->bust_coobj;
    }
    if (joint == 0) {
        joint = *(HSD_JObj **)zoid->hip_coobj;
    }
    if (joint == 0) {
        joint = (HSD_JObj *)zoid->camera_joint;
    }
    if (joint == 0) {
        joint = zoid->joint->child;
    }
    if (joint == 0) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        ((f32 *)&origin)[i] = joint->matrix[i][3];
        ((f32 *)&target)[i] = ((f32 *)&origin)[i];
        ((f32 *)&start)[i] = ((f32 *)&origin)[i];
    }
    angle = zoid->heading + offset;
    angLimit(&angle);
    sine_angle = angle;
    target.x += distance * sinf(sine_angle);
    t1 = angle;
    target.z += distance * cosf(t1);
    t2 = angle;
    start.x -= two * sinf(t2);
    t3 = angle;
    start.z -= two * cosf(t3);
    target.y += distance / 2.0f;
    {
        GroundMask mask2;
        GroundMask mask = *(const GroundMask *)lit_6311;

        mask2 = mask;
        res = sColPtAll(&start, &target, &mask2);
    }
    if (lit_4347[0] != res) {
        return 1;
    }
    {
        GroundMask mask2;
        GroundMask mask = *(const GroundMask *)lit_6316;

        mask2 = mask;
        res = sColPt(&target, &start, &mask2);
    }
    if (lit_4347[0] != res) {
        return 1;
    }
    return 0;
}
namespace { extern const f32 lit_4347[1]; extern const f32 lit_6343[1]; extern const f32 lit_6352[1]; }

f32 zdFreeLen(ZoidState *zoid, f32 length, f32 offset)
{
    Vec3 b;
    Vec3 a;
    f32 ang;
    GroundMask copy;
    HSD_JObj *joint;
    f32 sine_angle;
    f32 angle;
    s32 i;
    f32 r;

    joint = *(HSD_JObj **)zoid->bust_coobj;
    if (joint == 0) {
        joint = *(HSD_JObj **)zoid->hip_coobj;
    }
    if (joint == 0) {
        joint = *(HSD_JObj **)zoid->neck_coobj;
    }
    if (joint == 0) {
        joint = (HSD_JObj *)zoid->camera_joint;
    }
    if (joint == 0) {
        return 0.0f;
    }
    for (i = 0; i < 3; i++) {
        (&b.x)[i] = joint->matrix[i][3];
        (&a.x)[i] = joint->matrix[i][3];
    }
    ang = zoid->heading + offset;
    angLimit(&ang);
    sine_angle = ang;
    a.x = a.x + length * sinf(sine_angle);
    angle = ang;
    a.z = a.z + length * cosf(angle);
    {
        GroundMask mask;
        *(u32 *)&mask = *(const u32 *)lit_6343;
        copy = mask;
        r = sColRealPt(&b, &a, &copy);
    }
    if (lit_4347[0] == r) {
        r = lit_6352[0];
    }
    return r * length;
}
namespace { extern const f64 lit_6374[1]; }
extern f32 zdFreeLen(ZoidState *zoid, f32 length, f32 offset);
extern void angLimit(f32 *angle);

f32 zdFreeAng(ZoidState *zoid, f32 length)
{
    s32 count;
    s32 index;
    s32 i;
    f32 best_value;
    f32 angle;

    if (zoid == 0) {
        return 0.0f;
    }
    if (zoid->joint == 0) {
        return 0.0f;
    }
    count = zoid->field_2060;
    if (count <= 0) {
        for (i = 0; i < 24; i++) {
            zoid->field_1FF8[i] = 0.0f;
        }
        zoid->field_2060 = 1;
        return 100.0f;
    } else {
        index = count - 1;
        angle = 2.0 * (3.1415926 * (f32)(lit_6374[0] * (f32)index)) / 360.0;
        zoid->field_1FF8[index] = zdFreeLen(zoid, length, angle);
        if (count != 24) {
            zoid->field_2060 = zoid->field_2060 + 1;
            return 100.0f;
        } else {
            zoid->field_2060 = -1;
            index = 0;
            best_value = 0.0f;
            for (i = 0; i < 24; i++) {
                if (best_value >= zoid->field_1FF8[i]) {
                } else {
                    best_value = zoid->field_1FF8[i];
                    index = i;
                }
            }
            angle = 2.0 * (3.1415926 * (f32)(lit_6374[0] * (f32)index)) / 360.0;
            zoid->field_2058 = angle;
            zoid->field_205C = zoid->heading + angle;
            angLimit(&zoid->field_205C);
            return angle;
        }
    }
}
void zdMotion(ZoidState *zoid, s32 motion)
{
    if (zoid == 0)
        return;
    if (zoid->joint == 0)
        return;
    if (zoid->death_started != 0)
        return;
    if (zoid->action != 0)
        return;
    if (zoid->requested_action != 0)
        return;
    *(s32 *)((u8 *)zoid + 0x1CF8) = motion;
    zoid->requested_action = 0x31;
}
namespace { extern const f32 lit_4347[1]; extern const f32 lit_6424[1]; }

s32 zdCol(ZoidState *zoid, f32 *point, f32 radius)
{
    f32 end[3];
    f32 start[3];
    f32 length;
    s32 i;
    GroundMask mask;
    f32 hit;
    GroundMask tmp;

    if (zoid == 0)
        return 0;
    if (zoid->joint == 0)
        return 0;
    if (radius == 0.0f)
        return 0;
    length = sLenPoint3dR(point);
    if (length == 0.0f)
        return 0;
    for (i = 0; i < 3; i++) {
        end[i] = zoid->clip_point[i] + radius * point[i] / length;
        start[i] = zoid->clip_point[i] - 2.0f * point[i] / length;
    }
    *(u32 *)&tmp = *(const u32 *)lit_6424;
    mask = tmp;
    hit = sColPt((Vec3 *)start, (Vec3 *)end, &mask);
    if (lit_4347[0] == hit)
        return 0;
    return 1;
}
void zdSearchTop1(f32 *top, HSD_JObj *joint)
{
    if (joint == 0) return;
    if (top == 0) return;
    if (joint->matrix[1][3] > *top) {
        *top = joint->matrix[1][3];
    }
}

void zdSearchTop(ZoidState *zoid)
{
    f32 top;

    if (zoid != 0 && zoid->joint != 0) {
        top = zoid->position[1];
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->head_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->neck_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->bust_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->hip_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->l_arm1_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->l_arm2_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->r_arm1_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->r_arm2_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->l_leg1_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->l_leg2_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->r_leg1_coobj));
        zdSearchTop1(&top, (*(HSD_JObj **)zoid->r_leg2_coobj));
        zdSearchTop1(&top, zoid->feet[3].joint);
        zdSearchTop1(&top, zoid->feet[2].joint);
        zdSearchTop1(&top, zoid->feet[1].joint);
        zdSearchTop1(&top, zoid->feet[0].joint);
        zoid->field_1AA8 = top;
    }
}
extern void zdSearchTop(ZoidState *zoid);

#define Zoid ((ZoidState *)Zoid)
void ZoidSearchTop(void)
{
    s32 i;
    for (i = 0; i < 16; i++) {
        zdSearchTop(&Zoid[i]);
    }
}
#undef Zoid
typedef struct CoObjPolyV2 {
    HSD_JObj *joint;
    u8 reserved_04[0x10];
    s32 ground_contact;
    s32 object_contact;
    s32 field_1C;
    u8 reserved_20[4];
    u16 ground_polygon_index;
} CoObjPolyV2;

void zdGroundCoPolyClear(ZoidState *zoid, u8 *co)
{
    if (zoid == (void *)0)
        return;
    if (((CoObjPolyV2 *)co)->joint == (void *)0)
        return;
    ((CoObjPolyV2 *)co)->ground_polygon_index = 0xFFFF;
    ((CoObjPolyV2 *)co)->field_1C = ((CoObjPolyV2 *)co)->object_contact;
    ((CoObjPolyV2 *)co)->object_contact = 0;
    ((CoObjPolyV2 *)co)->ground_contact = 0;
}
extern void zdGroundCoPolyClear(ZoidState *zoid, u8 *co);

void zdGroundPolyClear(ZoidState *zoid)
{
    s32 part;

    zdGroundCoPolyClear(zoid, zoid->body_coobj);
    zdGroundCoPolyClear(zoid, zoid->bust_coobj);
    zdGroundCoPolyClear(zoid, zoid->neck_coobj);
    zdGroundCoPolyClear(zoid, zoid->head_coobj);
    for (part = 0; part < 8; part++)
        zdGroundCoPolyClear(zoid, zoid->tail_coobjs[part]);
    zdGroundCoPolyClear(zoid, zoid->hip_coobj);
    zdGroundCoPolyClear(zoid, zoid->l_arm1_coobj);
    zdGroundCoPolyClear(zoid, zoid->r_arm1_coobj);
    zdGroundCoPolyClear(zoid, zoid->l_leg1_coobj);
    zdGroundCoPolyClear(zoid, zoid->r_leg1_coobj);
    zdGroundCoPolyClear(zoid, zoid->l_arm2_coobj);
    zdGroundCoPolyClear(zoid, zoid->r_arm2_coobj);
    zdGroundCoPolyClear(zoid, zoid->l_leg2_coobj);
    zdGroundCoPolyClear(zoid, zoid->r_leg2_coobj);
    zdGroundCoPolyClear(zoid, zoid->l_foot_coobj);
    zdGroundCoPolyClear(zoid, zoid->r_foot_coobj);
}
typedef struct CoObjInvV2 {
    u8 reserved_00[0x34];
    f32 inverse[3][4];
} CoObjInvV2;

extern s32 sCubeInPoly(s32 count, Vec3 *vertices);

s32 zdGrdCoPo00(CoObj *object, GroundPolygon *polygon)
{
    Vec3 local[4];
    s32 count;
    s32 i;
    s32 index;

    if (object == 0)
        return 0;
    count = polygon->vertex_count;
    for (i = 0; i < count; i++) {
        index = polygon->vertices[i];
        PSMTXMultVec(((CoObjInvV2 *)object)->inverse, &grdcVtx[index], &local[i]);
    }
    return sCubeInPoly(count, local);
}
extern f32 atan2f(f32 y, f32 x);

#pragma push
#pragma dont_inline off
#pragma auto_inline off
f32 zdCoGrdRot(ZoidState *zoid, CoObj *object, Vec3 *direction, s32 count)
{
#define joint ((HSD_JObj **)object)
#define dir ((f32 *)direction)
    f32 x;
    f32 position[3];
    f32 delta[3];
    f32 angle;
    f32 next[3];
    s32 i;
    f32 turn;
    f32 angle2;
    f32 z;
    f32 scale;
    f32 diff;

    if (joint == 0)
        return 0.0f;
    if (*joint == 0)
        return 0.0f;
    for (i = 0; i < 3; i++) {
        position[i] = zoid->position[i];
        delta[i] = (*joint)->matrix[i][3] - position[i];
        next[i] = (f32)count * dir[i] + delta[i];
    }
    if (0.0f == delta[0] && 0.0f == delta[2])
        return 0.0f;
    if (delta[0] == next[0] && delta[2] == next[2])
        return 0.0f;
    z = delta[2];
    x = delta[0];
    angle = atan2f(x, z);
    z = next[2];
    angle2 = atan2f(next[0], z);
    angle = angle;
    angle2 = angle2;
    scale = 0.02f;
    turn = angle2 - angle;
    diff = angle - zoid->heading;
    angLimit(&diff);
    if (absf(diff) > 0.5235987666666666)
        scale = 0.0003f;
    if (absf(diff) > 1.3962633777777778)
        scale = 0.0007f;
    turn *= scale;
    scale = scale;
    angLimit(&turn);
    return turn;
#undef joint
#undef dir
}
#pragma pop
#pragma force_active on
namespace { __declspec(section ".sdata2") extern const f32 lit_4347[1] = { 10000.0f }; }
#pragma force_active off
u16 setCoPoly(s16 index)
{
    GroundPolygon *polygon;
    u16 normal;

    if ((u16)index == 0xFFFF) {
        return index;
    }
    polygon = &grdcPolygon[(u16)index];
    normal = polygon->normal_index;
    if (normal == 0xFFFF) {
        return normal;
    }
    if (grdcNormal[normal].y > 0.85) {
        return 0xFFFF;
    }
    return index;
}
extern s32 zdGrdCoPo00(CoObj *object, GroundPolygon *polygon);
extern void sGlobal2Local(HSD_JObj *joint);
extern u32 PSMTXInverse(f32 src[3][4], f32 dst[3][4]);
extern f32 zdCoGrdRot(ZoidState *zoid, CoObj *object, Vec3 *direction, s32 count);
extern u16 setCoPoly(s16 index);

#pragma push
#pragma dont_inline off
static inline f32 zdHalf(void)
{
    return 0.5f;
}

s32 zdGroundCoPoly(ZoidState *zoid, CoObj *object, s16 index, GroundPolygon *polygon, s32 mode, s32 flag)
{
    void *anim;
    f32 pos[3];
    f32 line[3];
    f32 dummy[3];
    f32 cur[3];
    f32 prev[3];
    f32 back[3];
    f32 top[3];
    f32 test[3];
    f32 len;
    f32 dist;
    f32 y;
    f32 diff;
    f32 rate;
    f32 push;
    s32 i;
    s32 k;
    Vec3 *normal;

    if (object->joint == (void *)0)
        return 0;
    if (object->base_joint == 0)
        return 0;
    anim = zoid->anim_data;
    for (i = 0; i < 3; i++)
        pos[i] = object->joint->matrix[i][3];
    len = lit_4347[0];
    if (zdGrdCoPo00(object, polygon) == 0) {
        if (zoid->current_animation > 2) {
            GroundMask mask2;
            GroundMask mask = {{0x17, 0, 0, 0}};
            f32 unused[3];
            mask2 = mask;
            len = sColPt00((Vec3 *)object->position, (Vec3 *)pos, polygon, &mask2);
        }
        if (lit_4347[0] == len)
            return 0;
        if (flag != 0)
            object->ground_polygon_index = index;
        object->ground_contact |= 1;
        if (mode == 0)
            return 1;
        zoid->grounded = 1;
        return 1;
    }
    if (flag != 0)
        object->ground_polygon_index = index;
    object->ground_contact |= 1;
    if (mode == 0)
        return 1;
    zoid->grounded = 1;
    normal = &grdcNormal[polygon->normal_index];
    if (mode == 2 || mode == 3) {
        for (i = 0; i < 3; i++) {
            cur[i] = zoid->position[i];
            prev[i] = zoid->field_1C24[i];
            top[i] = object->joint->matrix[i][3];
        }
        for (k = 0; k < 10; k++) {
            dist = 0.1 * (10 - k);
            for (i = 0; i < 3; i++) {
                back[i] = prev[i];
                test[i] = top[i];
            }
            sLinier3d(back, cur, dist);
            for (i = 0; i < 3; i++)
                test[i] -= cur[i] - back[i];
            y = zdColPosY(test, 0);
            if (!(y > test[1]))
                break;
        }
        for (i = 0; i < 3; i++)
            zoid->position[i] = back[i];
        if (flag != 0 && zoid->field_1EAE == 0xFFFF)
            zoid->field_1EAE = setCoPoly(index);
        zdSetXYZ(zoid);
    }
    if (object->base_joint != 0) {
        push = 0.2f;
        for (k = 0; k < 40; k++) {
            if (flag != 0) {
                for (i = 0; i < 3; i++) {
                    object->base_joint->matrix[i][3] += push * (&normal->x)[i];
                    object->joint->matrix[i][3] += push * (&normal->x)[i];
                }
            } else {
                for (i = 0; i < 3; i++) {
                    object->base_joint->matrix[i][3] += push * -(&normal->x)[i];
                    object->joint->matrix[i][3] += push * -(&normal->x)[i];
                }
            }
            PSMTXInverse(object->joint->matrix, object->inverse);
            if (zdGrdCoPo00(object, polygon) == 0)
                break;
        }
        sGlobal2Local(object->base_joint);
        HSD_JObjSetScaleX(object->base_joint, 1.0f);
        HSD_JObjSetScaleY(object->base_joint, 1.0f);
        HSD_JObjSetScaleZ(object->base_joint, 1.0f);
    }
    zoid->field_1C8C += zdCoGrdRot(zoid, object, normal, k);
    if (mode >= 2)
        i = 0;
    rate = 0.07f;
    if (mode <= 1)
        rate = 0.001f;
    if (flag != 0) {
        for (i = 0; i < 3; i++) {
            if (i == 1 && mode == 3)
                continue;
            zoid->joint->matrix[i][3] += (f32)k * (rate * (&normal->x)[i]) * zdHalf();
            zoid->position[i] += (f32)k * (rate * (&normal->x)[i]) * zdHalf();
        }
    } else {
        for (i = 0; i < 3; i++) {
            if (i == 1 && mode == 3)
                continue;
            zoid->joint->matrix[i][3] += (f32)k * (rate * -(&normal->x)[i]) * zdHalf();
            zoid->position[i] += (f32)k * (rate * -(&normal->x)[i]) * zdHalf();
        }
    }
    if (mode >= 2) {
        for (i = 0; i < 3; i++) {
            if (i == 1)
                continue;
            diff = 0.3 * (zoid->joint->child->matrix[i][3]
                                  - *(f32 *)(*(u8 **)((u8 *)zoid->pose + 4) + 0x40 + i * 16));
            zoid->position[i] -= diff;
            zoid->joint->matrix[i][3] -= diff;
        }
    }
    zdSetXYZ(zoid);
    return 1;
}
#pragma pop
void zdGrCoMovePoly(ZoidState *zoid, u16 index, s32 flag)
{
}
extern void zdGrCoMovePoly(ZoidState *zoid, u16 index, s32 flag);
extern void angLimit(f32 *angle);

s32 zdGroundPoly(ZoidState *zoid, s16 index, GroundPolygon *polygon, s32 flag)
{
    s32 *data = (s32 *)zoid->anim_data;
    s32 part;
    f32 scale;
    s32 dead;
    f32 angle;
    const f64 turn_limit = 2.0 * 3.1415926 * 0.5 / 360.0;

    scale = 0.0f;
    if (data != 0)
        scale = *(s32 *)((u8 *)data + 0xCC);
    zoid->field_1C8C = 0.0f;
    if (zdGroundCoPoly(zoid, (CoObj *)zoid->body_coobj, index, polygon, 0, flag) == 0)
        return 0;
    zdGroundCoPoly(zoid, (CoObj *)zoid->bust_coobj, index, polygon, data[2], flag);
    zdGroundCoPoly(zoid, (CoObj *)zoid->neck_coobj, index, polygon, data[1], flag);
    zdGroundCoPoly(zoid, (CoObj *)zoid->head_coobj, index, polygon, data[0], flag);
    for (part = 0; part < 8; part++)
        zdGroundCoPoly(zoid, (CoObj *)zoid->tail_coobjs[part], index, polygon, (data + 4)[part], flag);
    dead = 0;
    zdGroundCoPoly(zoid, (CoObj *)zoid->hip_coobj, index, polygon, data[3], flag);
    zdGroundCoPoly(zoid, (CoObj *)zoid->l_arm1_coobj, index, polygon, data[14], flag);
    zdGroundCoPoly(zoid, (CoObj *)zoid->r_arm1_coobj, index, polygon, data[16], flag);
    zdGroundCoPoly(zoid, (CoObj *)zoid->l_leg1_coobj, index, polygon, data[18], flag);
    zdGroundCoPoly(zoid, (CoObj *)zoid->r_leg1_coobj, index, polygon, data[20], flag);
    zdGroundCoPoly(zoid, (CoObj *)zoid->l_arm2_coobj, index, polygon, data[15], flag);
    zdGroundCoPoly(zoid, (CoObj *)zoid->r_arm2_coobj, index, polygon, data[17], flag);
    zdGroundCoPoly(zoid, (CoObj *)zoid->l_leg2_coobj, index, polygon, data[19], flag);
    zdGroundCoPoly(zoid, (CoObj *)zoid->r_leg2_coobj, index, polygon, data[21], flag);
    zdGrCoMovePoly(zoid, zoid->field_1EAE, flag);
    angle = zoid->field_1C8C;
    angLimit(&angle);
    if (angle < -turn_limit)
        angle = -turn_limit;
    if (angle > turn_limit)
        angle = turn_limit;
    zoid->field_1C8C = angle;
    zoid->heading = zoid->heading + angle;
    return 1;
}
s32 zdGroundTBL(ZoidState *zoid, u16 *table)
{
    HSD_JObj *joint;
    s32 material;
    GroundMask *mask;
    s32 unused;
    s32 hit;
    GroundPolygon *polygon;
    u16 index;

    joint = 0;
    if (joint == 0) {
        joint = zoid->field_1A7C;
    }
    if (joint == 0) {
        joint = zoid->joint->child;
    }
    if (joint == 0) {
        joint = zoid->camera_joint;
    }
    if (joint == 0) {
        unused = 0;
    }
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    (void)unused;
    hit = 0;
    while (1) {
        index = *table;
        table++;
        if (index == 0xFFFF) {
            return hit;
        }
        polygon = &grdcPolygon[index];
        material = polygon->material;
        mask = &grdcMat[material];
        if ((mask->bits[0] & 0x17) != 0) {
            if (zdGroundPoly(zoid, index, polygon, 1)) {
                hit = 1;
            }
        }
    }
}
extern void PSMTXMultVecArray(f32 m[3][4], Vec3 *src, Vec3 *dst, u32 count);
extern Vec3 CubeTBL[9];
extern void ZoidCoObjMtxI(ZoidState *zoid);
extern s32 zdGroundTBL(ZoidState *zoid, u16 *table);
extern void sLinier3d(f32 *current, f32 *target, f32 fraction);

s32 ZoidGround(ZoidState *zoid)
{
    u16 *list[9];
    Vec3 points[9];
    f32 cur[3];
    f32 prev[3];
    s32 i;
    s32 j;
    s32 n;
    u16 *table;
    s32 hit;
    u8 *co;
    s32 dead;
    void *anim;
    f32 sweep;

    dead = 0;
    if (zoid == (void *)0)
        return 0;
    anim = zoid->anim_data;
    if (anim == 0)
        return 0;
    zoid->field_1EAE = GROUND_POLYGON_NONE;
    co = zoid->body_coobj;
    if (*(HSD_JObj **)co == (void *)0)
        return 0;
    PSMTXMultVecArray((*(HSD_JObj **)co)->matrix, CubeTBL, points, 9);
    for (i = 0, n = 0; i < 9; i++) {
        table = sColPosTBL(&points[i]);
        if (table != (void *)0) {
            for (j = 0; j < n; j++) {
                if (list[j] == table)
                    break;
            }
            if (j == n)
                list[n++] = table;
        }
    }
    if (n == 0)
        return 0;
    for (i = n; i < 9; i++)
        list[i] = 0;
    ZoidCoObjMtxI(zoid);
    hit = 0;
    for (i = 0; i < n; i++) {
        table = list[i];
        if (table != (void *)0 && zdGroundTBL(zoid, table) != 0)
            hit = 1;
    }
    zdSetXYZ(zoid);
    sMakeMatrix(zoid->joint);
    if (zoid->field_1A7C != 0) {
        GroundMask dead_mask = {{3, 0, 0, 0}};

        for (i = 0; i < 3; i++) {
            cur[i] = zoid->field_1A7C->matrix[i][3];
            prev[i] = zoid->field_1A80[i];
        }
        zoid->field_1A98 = 0;
        {
            GroundMask mask2;
            GroundMask mask = {{0x17, 0, 0, 0}};

            mask2 = mask;
            sweep = sColPt((Vec3 *)prev, (Vec3 *)cur, &mask2);
        }
        if (lit_4347[0] != sweep) {
            sLinier3d(prev, cur, sweep);
            zoid->field_1A98 = 1;
            zoid->field_1C90[0] = 0.0f;
            zoid->field_1C90[1] = 0.0f;
            zoid->field_1C90[2] = 0.0f;
        }
        if (zoid->field_1A98 != 0) {
            (void)0;
        }
        if (*(u32 *)(zoid->head_coobj + 0x18) & 1) {
            (void)0;
        }
        zdSetXYZ(zoid);
    }
    dead = dead;
    return hit;
}
#pragma force_active on
static ModelArchive *sdTest;
static u32 dbgZoidColi;
u32 sZoidEx;
s32 sZoidHG;
ModelArchive *steFile;
void *steMobj;
static s32 csrDispON;
static ModelArchive *csrFile;
static s32 csrNum = -1;
#pragma explicit_zero_data on
static u8 gap_802F14F4[4] = { 0x00, 0x00, 0x00, 0x00 };
#pragma explicit_zero_data off
CamTmpV2 *camTmp = (CamTmpV2 *)&sCamera;
u32 sCamNS2 = 0x00000001;
namespace { __declspec(section ".sdata2") extern const f64 lit_4774[1] = { 0.015 }; }
namespace { __declspec(section ".sdata2") extern const f32 lit_4813[1] = { 0.075f }; }
namespace { __declspec(section ".sdata2") extern const f64 lit_5938[1] = { 0.8 }; }
namespace { __declspec(section ".sdata2") extern const f64 lit_5939[1] = { 0.78539815 }; }
namespace { __declspec(section ".sdata2") extern const f64 lit_5940[1] = { 2.35619445 }; }
namespace { __declspec(section ".sdata2") extern const char lit_6185[6] = "tex2\n"; }
namespace { __declspec(section ".sdata2") extern const char lit_6187[5] = "tex\n"; }
namespace { __declspec(section ".sdata2") extern const f32 lit_6311[1] = { 2.7105054e-20f }; }
namespace { __declspec(section ".sdata2") extern const f32 lit_6316[1] = { 2.7105054e-20f }; }
namespace { __declspec(section ".sdata2") extern const f32 lit_6343[1] = { 2.7105054e-20f }; }
namespace { __declspec(section ".sdata2") extern const f32 lit_6352[1] = { 1.01f }; }
namespace { __declspec(section ".sdata2") extern const f64 lit_6374[1] = { 15.0 }; }
namespace { __declspec(section ".sdata2") extern const f32 lit_6424[1] = { 1.6155871e-27f }; }
#pragma force_active off
}
