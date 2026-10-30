#ifndef ZOIDS_MISSILE_H
#define ZOIDS_MISSILE_H

#include "model_archive.h"
#include "angle.h"

struct ZoidState;
struct HSD_LObj;

/* Record of etc/n_Bullet.dat. */
typedef struct BulletDat {
    char name[8];
    s32 damage;
    s16 speed;
    u8 reserved_0E[4];
    s16 kind;
    u8 reserved_14[2];
    s16 trace_rate;
    u8 reserved_18[2];
    s16 zoid_hit;
    s16 hit_effect;
    s16 fire_effect;
    s16 se;
    u8 reserved_22[6];
    s16 turn_angle;
    u8 reserved_2A[0x22];
} BulletDat;

typedef char BulletDatSizeCheck[sizeof(BulletDat) == 0x4C ? 1 : -1];

/* BulletDat.kind, named after the misProc handler it selects in sMissileSet. */
enum BulletKind {
    BULLET_KIND_STRAIGHT,
    BULLET_KIND_AT20,
    BULLET_KIND_PARABOLA,
    BULLET_KIND_FLOAT,
    BULLET_KIND_GROUND,
    BULLET_KIND_05
};

/* BulletDat.zoid_hit values that select a hit test in sMissileZoid. */
enum MissileZoidHit {
    MISSILE_ZOID_HIT_SMOKE = 5,
    MISSILE_ZOID_HIT_MINE = 6
};

/* Missile.proc: the misProc handler. */
enum MissileProc {
    MISSILE_PROC_00,
    MISSILE_PROC_01,
    MISSILE_PROC_02,
    MISSILE_PROC_03,
    MISSILE_PROC_AT20,
    MISSILE_PROC_AT50,
    MISSILE_PROC_STRAIGHT,
    MISSILE_PROC_PARABOLA,
    MISSILE_PROC_FLOAT,
    MISSILE_PROC_GROUND
};

#define MISSILE_FLAG_ACTIVE 1

typedef struct Missile {
    s32 flags;
    s32 bullet;
    s32 kind;
    s32 zoid_hit;
    s32 hit_ground;
    s32 animation;
    ModelArchive *archive;
    HSD_JObj *joint;
    struct ZoidState *owner;
    s32 owner_weapon_mount;
    struct ZoidState *target_zoid;
    struct Missile *target_missile;
    void *field_30; /* no writer in src/; only cleared by ZeroMemory in sMissilePrologue and sMissileSet */
    f32 speed;
    f32 turn_rate;
    f32 matrix[3][4];
    f32 previous[3][4];
    f32 step_length;
    f32 inverse[3][4];
    s32 update_count; /* incremented by sMove, sHitOn and sAtack */
    s32 damage;
    s32 purge_timer;
    s32 hit;
    f32 max_speed;
    f32 accel;
    u8 reserved_E8[4];
    s32 proc;
    s32 proc_delay;
    HSD_JObj *beam_joints[7];
    f32 beam_positions[7][3];
    HSD_JObj *a1_joint;
    struct HSD_LObj *light;
    u8 hit_zoids[16];
    s32 se;
    s32 se_playing;
    u32 se_voice;
} Missile;

typedef char MissileSizeCheck[sizeof(Missile) == 0x188 ? 1 : -1];

/* Record of etc/n_Effect.dat. */
typedef struct EffectDat {
    char name[8];
    s16 kind;
    s16 trace_rate;
    u8 reserved_0C[2];
    s16 se;
    u8 reserved_10[0xA];
} EffectDat;

typedef char EffectDatSizeCheck[sizeof(EffectDat) == 0x1A ? 1 : -1];

/* EffectDat.kind values that select a per-frame handler in sEffectMove. */
enum EffectKind {
    EFFECT_KIND_FOLLOW = 1,
    EFFECT_KIND_TRACE = 2,
    EFFECT_KIND_ZOID = 3
};

typedef struct EffectJoint {
    HSD_JObj *joint;
    f32 position[3];
} EffectJoint;

typedef struct Effect {
    HSD_JObj *root;
    HSD_JObj *a1_joint;
    s32 type;
    ModelArchive *archive;
    s32 mode; /* 0: position argument, 1: source matrix at spawn, 2: follows source, 3: 2 plus sEffectZoid */
    f32 position[3];
    HSD_JObj *source;
    struct ZoidState *zoid;
    EffectJoint joints[18];
    f32 history[60][3];
    s32 se;
    s32 se_playing;
    u32 se_voice;
} Effect;

typedef char EffectSizeCheck[sizeof(Effect) == 0x424 ? 1 : -1];

extern BulletDat Tea_bulletdat[131];
extern EffectDat Tea_effectdat[];
extern Missile missileStart[200];
extern Effect sEffect[100];
extern ModelArchive *efctFile[110];
extern ModelArchive *sEfctLight;

extern void sMissileSetAnim(Missile *missile, s32 animation);
extern void sMissilePurge(Missile *missile);
extern void msFlyTrace(Missile *missile);
extern void sEffectSet(s32 type, s32 mode, f32 *position, HSD_JObj *source, struct ZoidState *zoid);
extern void sEffectPurge(Effect *effect);
extern void sMissileFire(struct ZoidState *owner, struct ZoidState *target_zoid, Missile *target_missile, f32 matrix[3][4], s32 bullet, s32 extra_damage);

#endif
