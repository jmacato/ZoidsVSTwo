#ifndef ZOIDS_STATE_H
#define ZOIDS_STATE_H

#include "joint.h"
#include "zoid_collision.h"
#include "model_archive.h"

enum ZoidAIMode {
    AI_MODE_NONE,
    AI_MODE_POINT,
    AI_MODE_TARGET,
    AI_MODE_RUN
};

#define AI_RADIUS_MASK 0xF
#define AI_ANGLE_MASK 0xF0
#define AI_ANGLE_SHIFT 4

enum ZoidAIRadius {
    AI_RADIUS_NONE,
    AI_RADIUS_EXACT,
    AI_RADIUS_IN,
    AI_RADIUS_OUT
};

enum ZoidAIAngle {
    AI_ANGLE_NONE,
    AI_ANGLE_KEEP,
    AI_ANGLE_CIRCLE
};

enum ZoidAIPartnerCommand {
    AI_PARTNER_NONE,
    AI_PARTNER_ATTACK_LOCKED,
    AI_PARTNER_COME_NEAR,
    AI_PARTNER_KEEP_DISTANCE,
    AI_PARTNER_CLOSE_COMBAT
};

typedef struct ZoidAIState {
    s32 nearest_enemies[16];
    s32 mode;
    u32 flags;
    s32 target_index;
    Vec3 destination;
    Vec3 field_58;
    f32 target_distance;
    f32 target_angle;
    s32 turn_speed;
    s32 field_70;
    Vec3 goal;
    s32 missile_hit;
    s32 partner_command;
} ZoidAIState;

typedef char ZoidAIStateSizeCheck[sizeof(ZoidAIState) == 0x88 ? 1 : -1];

typedef struct WeaponMountEntry {
    s32 active;
    s32 slot;
    s32 field_08;
} WeaponMountEntry;

typedef struct WeaponMount {
    s32 active;
    s32 state;
    WeaponMountEntry entries[4];
} WeaponMount;

/* weapon_mounts[] indices with a fixed use; zdWeaponChg cycles the selection through 0..9 */
enum ZoidWeaponMountIndex {
    WEAPON_MOUNT_SHIELD = 10,
    WEAPON_MOUNT_BOOSTER = 11,
    WEAPON_MOUNT_SMOKE_MINE = 12,
    WEAPON_MOUNT_SUPER_ATTACK = 19
};

/* option_type values, picked by zdLoadOption from the option part's flags */
enum ZoidOptionType {
    ZOID_OPTION_NONE,
    ZOID_OPTION_SHIELD,
    ZOID_OPTION_BOOSTER,
    ZOID_OPTION_CAMOUFLAGE,
    ZOID_OPTION_SMOKE,
    ZOID_OPTION_MINE
};

/* stel_state values: zdStelON fades the zoid out to ZOID_STEL_ON, zdStelOFF back to ZOID_STEL_OFF */
enum ZoidStelState {
    ZOID_STEL_OFF,
    ZOID_STEL_FADING,
    ZOID_STEL_ON
};

typedef struct WeaponSlot {
    s32 weapon;
    s32 field_04;
    HSD_JObj *joint;
} WeaponSlot;

typedef struct CameraMountInfo {
    s32 joint_slot;
    s32 angle;
    s32 distance;
    s32 height;
} CameraMountInfo;

typedef struct ZoidControlData {
    s32 ground_modes[22];
    u8 reserved_0058[0x40];
    s32 max_senter_roll;
    s32 field_009C;
    s32 field_00A0;
    s32 field_00A4;
    s32 anim6_rate;
    s32 max_reverse_speed;
    s32 anim3_rate;
    s32 anim4_min_speed;
    s32 anim4_rate;
    s32 base_max_speed;
    s32 anim5_rate;
    s32 max_steer;
    s32 max_f_base_ry;
    s32 max_face_yaw;
    s32 step_launch_y;
    s32 acceleration;
    s32 deceleration;
    s32 stick_turn_rate;
    s32 sweep_radius;
    s32 max_unsupported_frames;
    f32 field_00E8;
    f32 field_00EC;
    f32 field_00F0;
    s32 field_00F4;
    s32 field_00F8;
    s32 sweep_joint_index;
    s32 field_0100;
    u8 reserved_0104[0x3C];
    s32 field_0140[20];
    s32 attack_chain_length;
    CameraMountInfo camera_mounts[20];
    u8 reserved_02D4[0x10];
    s32 pull_range_offset;
    u8 reserved_02E8[4];
    s32 dmg3_launch_scale;
    s32 down_input_frames;
    s32 max_down_frames;
    u8 reserved_02F8[0x10];
    s32 detail_distance;
} ZoidControlData;

typedef char ZoidControlDataSizeCheck[sizeof(ZoidControlData) == 0x30C ? 1 : -1];

typedef struct ZoidAttackObject {
    HSD_JObj *joint;
    f32 inverse[3][4];
    u8 touched[16];
} ZoidAttackObject;

typedef char ZoidAttackObjectSizeCheck[sizeof(ZoidAttackObject) == 0x44 ? 1 : -1];

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

/* FootJoint.flags: ik3d sets CONTACT and SUPPORTED on ground contact, clears CONTACT on each call and SUPPORTED after 15 frames without contact */
#define FOOT_FLAG_CONTACT 1
#define FOOT_FLAG_SUPPORTED 2
#define FOOT_FLAG_PREVIOUS_CONTACT 4

typedef struct ZoidLimb {
    f32 position[3];
    u8 reserved_0C[0xC];
    HSD_JObj *joint;
} ZoidLimb;

/* action and requested_action values; ZoidMove calls the handler named in each constant */
enum ZoidAction {
    ZOID_ACTION_WALK,
    ZOID_ACTION_JUMP_1,
    ZOID_ACTION_JUMP_2,
    ZOID_ACTION_TURN,
    ZOID_ACTION_TURN_R0,
    ZOID_ACTION_TURN_L0,
    ZOID_ACTION_TURN_R1,
    ZOID_ACTION_TURN_L1,
    ZOID_ACTION_TURN_R2,
    ZOID_ACTION_TURN_L2,
    ZOID_ACTION_STOP,
    ZOID_ACTION_ATTACK_0,
    ZOID_ACTION_ATTACK_1,
    ZOID_ACTION_ATTACK_2,
    ZOID_ACTION_ATTACK_3,
    ZOID_ACTION_ATTACK_4,
    ZOID_ACTION_ATTACK_5,
    ZOID_ACTION_ATTACK_6,
    ZOID_ACTION_ATTACK_7,
    ZOID_ACTION_ATTACK_8,
    ZOID_ACTION_ATTACK_9,
    ZOID_ACTION_STEP_0,
    ZOID_ACTION_STEP_1,
    ZOID_ACTION_STEP_2,
    ZOID_ACTION_STEP_3,
    ZOID_ACTION_DMG2_0,
    ZOID_ACTION_DMG2_1,
    ZOID_ACTION_DMG2_2,
    ZOID_ACTION_DMG2_3,
    ZOID_ACTION_DMG2_4,
    ZOID_ACTION_DMG2_5,
    ZOID_ACTION_DMG2_6,
    ZOID_ACTION_DMG3_0,
    ZOID_ACTION_DMG3_1,
    ZOID_ACTION_DMG3_2,
    ZOID_ACTION_DMG3_3,
    ZOID_ACTION_DMG3_4,
    ZOID_ACTION_DMG3_5,
    ZOID_ACTION_DMG3_6,
    ZOID_ACTION_DEADLY,
    ZOID_ACTION_DEAD,
    ZOID_ACTION_DEATH,
    ZOID_ACTION_DASH,
    ZOID_ACTION_STEP_REQUEST_0,
    ZOID_ACTION_STEP_REQUEST_1,
    ZOID_ACTION_STEP_REQUEST_2,
    ZOID_ACTION_STEP_REQUEST_3
};

typedef struct ZoidState {
    char name[0x20];
    s32 type;
    s32 person;
    struct CameraState *camera;
    struct HSD_Archive *archive;
    ModelAnimationTable *animations;
    void *archive_data;
    HSD_JObj *joint;
    JointPose *pose;
    struct HSD_AObj *aobj;
    ZoidLimb limbs[9];
    s32 previous_health;
    s32 health;
    s32 max_health;
    s16 option_type;
    u8 option_use_state;
    u8 option_use_previous;
    s16 option_energy_max;
    s16 option_energy;
    s16 option_start_cost;
    s16 option_hold_cost;
    s16 option_recharge;
    s16 booster_on;
    s16 additional_parts[3];
    u8 reserved_0162[2];
    CoDamage damages[6];
    ZoidAttackObject attack_objects[10];
    CoObj body_coobj;
    CoObj head_coobj;
    CoObj neck_coobj;
    CoObj bust_coobj;
    CoObj hip_coobj;
    CoObj tail_coobjs[8];
    CoObj l_arm1_coobj;
    CoObj l_arm2_coobj;
    CoObj r_arm1_coobj;
    CoObj r_arm2_coobj;
    CoObj l_leg1_coobj;
    CoObj l_leg2_coobj;
    CoObj r_leg1_coobj;
    CoObj r_leg2_coobj;
    CoObj l_foot_coobj;
    CoObj r_foot_coobj;
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
    s32 weapon_mount;
    s32 weapon_mount_count;
    s32 selected_weapon_mount;
    s32 field_11E4;
    WeaponMount weapon_mounts[20];
    WeaponMountEntry field_1648[20];
    WeaponSlot weapon_slots[30];
    HSD_JObj *camera_joint;
    Vec3 camera_joint_position;
    Vec3 previous_child_translation;
    f32 pull_scale;
    HSD_JObj *sweep_joint;
    f32 sweep_origin[3];
    s32 sweep_hit;
    HSD_JObj *f_base_joint;
    HSD_JObj *b_base_joint;
    HSD_JObj *senter_joint;
    HSD_JObj *buckle_joint;
    HSD_JObj *berut1_joint;
    HSD_JObj *jacket_joint;
    HSD_JObj *collar_joint;
    HSD_JObj *navel_joint;
    HSD_JObj *animation_exclusion;
    f32 senter_translation_y;
    f32 senter_translation_z;
    f32 senter_roll;
    FootJoint feet[4];
    f32 ground_normal_sum[3];
    s32 ground_normal_count;
    s32 foot_count;
    s32 field_19E8;
    s32 death_pending;
    s32 field_19F0;
    f32 field_19F4;
    f32 ground_height;
    f32 field_19FC;
    f32 buckle_height;
    s32 unsupported_frames;
    s32 foot_contacts;
    s32 grounded;
    s32 grounded_frames;
    f32 max_ground_depth;
    f32 position[3];
    f32 previous_position[3];
    f32 height_offset;
    s32 field_1A34;
    f32 height_offset_speed;
    f32 child_translation_y;
    s32 field_1A40;
    s32 movement_flags;
    f32 step_heading;
    f32 stick_length;
    f32 stick_steady_frames;
    f32 stick_frames;
    f32 stick_angle;
    f32 stick_x;
    f32 stick_y;
    f32 heading;
    f32 pitch;
    f32 target_heading;
    f32 ground_turn;
    f32 field_1A74;
    f32 ramped_speed;
    f32 forward_speed;
    f32 field_1A80;
    f32 steer;
    f32 f_base_ry;
    f32 face_yaw;
    f32 face_pitch;
    f32 ground_rx;
    f32 ground_rz;
    f32 f_base_rx;
    f32 f_base_rz;
    f32 field_1AA4;
    f32 field_1AA8;
    f32 field_1AAC;
    f32 slide_speed;
    s32 slide_frames;
    s32 field_1AB8;
    Vec3 launch_velocity;
    f32 knockback[3];
    f32 displacement[3];
    f32 speed;
    f32 previous_displacement[3];
    f32 displacement_change[3];
    s32 attack_index;
    s32 requested_action;
    s32 action;
    s32 action_phase;
    s32 down_frames;
    s32 jump_phase;
    s32 jump_attack_phase;
    f32 velocity[3];
    s32 field_1B24;
    s32 current_animation;
    s32 previous_animation;
    s32 field_1B30;
    f32 child_matrix[3][4];
    f32 heading_delta;
    s32 anim1_frames;
    f32 animation_end_frame;
    s32 animation_stopped;
    s32 animation_looped;
    f32 animation_frame;
    u16 ally_smoke_timer;
    u16 enemy_smoke_timer;
    s32 invulnerable_timer;
    s32 field_1B84;
    s32 health_bar_timer;
    s32 death_started;
    s32 field_1B90;
    s32 field_1B94;
    s32 rumble_timer;
    u8 index;
    u8 slot;
    u8 target_pending;
    u8 target_slot;
    u8 attack_target;
    u8 reserved_1BA1;
    u8 color;
    u8 reserved_1BA3;
    s32 field_1BA4;
    u8 reserved_1BA8[8];
    u16 ground_polygon;
    u8 reserved_1BB2[2];
    struct GroundMask *ground_material;
    Vec3 *ground_normal;
    s32 option_active;
    s32 stel_state;
    s32 previous_stel_state;
    u32 transparency_flags;
    f32 transformation;
    s32 interpolation_remaining;
    s32 interpolation_duration;
    s32 kills;
    s32 field_1BDC;
    s32 field_1BE0;
    f32 radar_range;
    f32 search_range;
    s32 armor;
    s32 max_speed;
    f32 distances[16];
    u8 contact[16];
    u8 attack_in_range[16];
    u8 target_visibility[16];
    f32 target_angles[16];
    ZoidControlData *control;
    ZoidAIState *ai;
    s32 ai_active;
    s32 radar_range_bonus;
    s32 search_range_bonus;
    s32 field_1CB8;
    s32 armor_bonus;
    s32 field_1CC0;
    s32 speed_bonus;
    s32 bullet_power_bonus;
    s32 melee_power_bonus;
    s32 turn_bonus;
    u8 reserved_1CD4[4];
} ZoidState;

typedef char ZoidStateSizeCheck[sizeof(ZoidState) == 0x1CD8 ? 1 : -1];

extern ZoidState Zoid[16];

#endif
