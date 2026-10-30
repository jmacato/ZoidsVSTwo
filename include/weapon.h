#ifndef ZOIDS_WEAPON_H
#define ZOIDS_WEAPON_H

#include "model_archive.h"

struct Missile;

typedef struct WeaponMarker {
    Vec3 position;
    s32 locked;
    struct ZoidState *target;
    struct Missile *source;
    HSD_JObj *joint; /* target camera_joint the marker follows; 0 when free */
} WeaponMarker;

typedef struct WeaponInverse {
    HSD_JObj *joint;
    f32 inverse[3][4];
    u8 reserved_34[4];
} WeaponInverse;

#define WEAPON_FLAG_LOOP_OFF 8
#define WEAPON_STATE_STOPPED 0x10

typedef struct WeaponState {
    s32 active;
    u32 flags;
    u32 state_flags;
    s32 type;
    ModelArchive *archive;
    s32 model;
    JointPose *pose;
    HSD_JObj *mount_joint;
    HSD_JObj *target_joint;
    struct ZoidState *owner;
    u8 reserved_28[8];
    HSD_JObj *barrels[12]; /* pa_00_joint..pa_11_joint */
    HSD_JObj *trace_joints[13]; /* te_00_joint..te_12_joint */
    Vec3 trace_positions[13];
    WeaponInverse inverses[12]; /* a_0_joint..a_b_joint */
    s32 animation;
    s32 ammo;
    f32 range;
    f32 rate;
    u8 reserved_3E0[4];
    s32 bullet_kind; /* BulletDat.kind of the part's first bullet */
    s32 marker_count;
    s32 marker_cursor;
    s16 drain;
    u8 reserved_3F2[2];
    WeaponMarker markers[20];
} WeaponState;

typedef char WeaponStateSizeCheck[sizeof(WeaponState) == 0x624 ? 1 : -1];

extern WeaponState sWeapon[100];

typedef struct Lsr {
    HSD_JObj *source;
    f32 start[3][4];
    f32 end[3][4];
    ModelArchive *archive;
} Lsr;

typedef char LsrSizeCheck[sizeof(Lsr) == 0x68 ? 1 : -1];

typedef struct Lsr1 {
    f32 start[3][4];
    f32 end[3][4];
    HSD_JObj *joint;
    HSD_JObj *start_joint;
    HSD_JObj *end_joint;
} Lsr1;

typedef char Lsr1SizeCheck[sizeof(Lsr1) == 0x6C ? 1 : -1];

extern Lsr sLsr[11];
extern Lsr1 sLsr1[400];

#endif
