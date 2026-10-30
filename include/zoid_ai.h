#ifndef ZOIDS_ZOID_AI_H
#define ZOIDS_ZOID_AI_H

#include "zoid.h"

enum AiCommandKind {
    AI_CMD_NONE = 0,
    AI_CMD_SEARCH_ENEMY = 1,
    AI_CMD_FIRE1 = 2,
    AI_CMD_FIRE_ON = 3,
    AI_CMD_FIRE_OFF = 4,
    AI_CMD_MELEE = 5,
    AI_CMD_JUMP = 7,
    AI_CMD_STEP = 8,
    AI_CMD_RUN = 9,
    AI_CMD_SET_LEN = 10,
    AI_CMD_SET_LEN_IN = 11,
    AI_CMD_SET_LEN_OUT = 12,
    AI_CMD_SET_ANGLE_SPD = 13,
    AI_CMD_SET_ANGLE_ON = 14,
    AI_CMD_LOOK = 15,
    AI_CMD_TARGET_NEXT = 16,
    AI_CMD_NEXT_WEAPON = 19,
    AI_CMD_SET_MISSILE_DISTANCE = 20,
    AI_CMD_INIT = 23,
    AI_CMD_SET_MISSILE_STEP_ANGLE = 25,
    AI_CMD_SET_NEAR_ACTION = 26,
    AI_CMD_SET_FACED_ACTION = 27,
    AI_CMD_SET_ATTACKED_ACTION = 28,
    AI_CMD_SET_4C = 29,
    AI_CMD_SET_STEP_INTERVAL = 30,
    AI_CMD_SET_98 = 31
};

enum AiCommandLabel {
    AI_LABEL_GOTO = -1,
    AI_LABEL_SCRIPT = -2
};

typedef struct AiCommand {
    s32 label;
    s32 reserved[3];
    s32 command;
    f32 value;
    s16 duration;
} AiCommand;

typedef struct AiData {
    s32 script;
    u8 reserved_04[0x10];
    s32 angle_command;
    f32 angle_value;
    u8 reserved_1C[0xC];
    s32 field_28;
    s32 field_2C;
    s16 command_timer;
    s16 missile_distance;
    s16 field_34;
    s16 field_36;
    s16 ground_rise;
    s16 angle_to_target;
    s16 missile_range;
    s16 melee_range;
    s16 terrain_distance;
    s16 missile_step_angle;
    s16 side;
    s16 near_action;
    s16 faced_action;
    s16 attacked_action;
    s32 field_4C;
    s16 melee_burst;
    s16 melee_cooldown_time;
    s32 target_ahead;
    s32 target_facing_me;
    s32 obstacle_ahead;
    s32 avoiding_obstacle;
    s32 under_attack;
    s16 field_68;
    s16 stopped_timer;
    s16 fire_timer;
    s16 weapon_timer;
    s16 step_timer;
    s16 step_interval;
    s16 melee_left;
    s16 melee_cooldown;
    s16 field_78;
    s16 len_out_timer;
    s16 len_in_timer;
    u8 reserved_7E[2];
    s32 weapon_ready;
    s32 enemy_found;
    s32 field_88;
    s32 field_8C;
    s32 field_90;
    s32 rapid_fire;
    s32 field_98;
    s32 command_index;
} AiData;

typedef char AiDataSizeCheck[sizeof(AiData) == 0xA0 ? 1 : -1];

extern AiCommand aiCom0[56];
extern AiCommand aiCom1[54];
extern AiCommand aiCom2[64];
extern AiCommand aiCom3[87];
extern AiData aiDat[16];
extern AiCommand aiCom;

void aiPrologue(s32 index);
void aiEpilogue(s32 index);
void ai(s32 index);
s32 aiNextWeapon(s32 index);
void ZoidAI(void);
void zdExecAI(ZoidState *zoid);

f32 zdAI_ComeMissile(s32 index);
f32 zdAI_goY(s32 index, s32 degrees, f32 distance);
s32 zdAI_AngZoidEM(s32 index);
s32 zdAI_AngZoidME(s32 index);
s32 zdAI_goWall(s32 index, s32 distance, s32 degrees);
s32 zdAI_IsAtack(s32 index);
s32 zdAI_IsAtackMine(s32 index);
s32 zdAI_IsTarget(s32 index);
s32 zdAI_Look(s32 index);
s32 zdAI_SearchEnemy(s32 index);
s32 zdAI_WarningArea(s32 index);
s32 zdAI_WeaponBullet(s32 index);
s32 zdAI_WeaponGetID(s32 index);
void zdAI_Atack1(s32 index);
void zdAI_goPoint(s32 index, f32 x, f32 z);
void zdAI_Jump(s32 index);
void zdAI_Run(s32 index, s32 degrees);
void zdAI_SetAngle(s32 index, s32 angle_mode, s32 degrees, s32 speed);
void zdAI_SetAngleON(s32 index, s32 degrees);
void zdAI_SetAngleSpd(s32 index, s32 speed);
void zdAI_SetLen(s32 index, f32 distance);
void zdAI_SetLen1(s32 index, u32 radius_mode, f32 distance);
void zdAI_SetLenIn(s32 index, f32 distance);
void zdAI_SetLenOut(s32 index, f32 distance);
void zdAI_SetLockZoid(s32 index);
void zdAI_SetZoid(s32 index);
void zdAI_SetZoid0(s32 index, s32 target);
void zdAI_Step(s32 index, s32 degrees);
void zdAI_TargetNext(s32 index);
void zdAI_TargetSet2(s32 index);
void zdAI_WeaponChg(s32 index);
void zdAI_WeaponFire1(s32 index);
void zdAI_WeaponFireOFF(s32 index);
void zdAI_WeaponFireON(s32 index);

#endif
