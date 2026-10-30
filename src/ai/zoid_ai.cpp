extern "C" {
#pragma readonly_strings on
#pragma bool off
#include "types.h"

struct HSD_JObj {
    u8 pad_00[0x44];
    f32 matrix[3][4];
};

typedef struct GroundPolygon {
    u16 vertex_count;
    u16 material;
    u16 normal_index;
    u16 reserved_06;
    u16 vertices[4];
    Vec3 normal;
    f32 plane_offset;
} GroundPolygon;

typedef struct GroundMask {
    u8 bits[4];
} GroundMask;

extern GroundPolygon *grdcPolygon;
extern GroundMask *grdcMat;

typedef struct ZoidState {
    u8 pad_0000[0x40];
    struct HSD_JObj *joint;
    u8 pad_0044[0x4CC];
    void *head_hit;
    u8 pad_0514[0x98];
    void *neck_hit;
    u8 pad_05B0[0xD44];
    s32 field_1360;
    u8 pad_12F8[0x4];
    s32 weapon_mount;
    u8 pad_1300[0x868];
    f32 position[3];
    u8 pad_1B74[0x2C];
    f32 step_heading;
    f32 field_1C54;
    f32 field_1C58;
    u8 pad_1BAC[0x8];
    f32 stick_angle;
    u8 pad_1BB8[0x8];
    f32 heading;
    u8 pad_1BC4[0x10];
    f32 field_1C90[3];
    u8 pad_1BE0[0xD0];
    s32 requested_action;
    s32 action;
    u8 pad_1CB8[0x8];
    s32 action_phase;
    u8 pad_1CC4[0x8];
    s32 jump_phase;
    u8 pad_1CD0[0x94];
    s32 death_started;
    u8 pad_1D68[0x8];
    s32 field_1E78;
    u8 pad_1D74[0x9];
    u8 slot;
    u8 target_pending;
    u8 target_slot;
    u8 pad_1D80[0x1A];
    u16 ground_polygon;
    u8 pad_1D9C[0x44];
    f32 distances[16];
    u8 pad_1E20[0x10];
    u8 attack_in_range[16];
    u8 target_visibility[16];
    f32 target_angles[16];
    u8 pad_1E90[0x64];
    f32 field_205C;
    s32 field_2060;
    u8 pad_1EFC[0x30];
    void *anim_data;
    void *ai;
    s32 ai_active;
    u8 pad_1F38[0x54];
} ZoidState;

typedef char ZoidStateSizeCheck[sizeof(ZoidState) == 0x1F8C ? 1 : -1];

extern ZoidState Zoid[16];

typedef struct AI3 {
    s32 nearest_enemies[16];
    s32 mode;
    s32 flags;
    s32 target_index;
    f32 destination[3];
    f32 field_58[3];
    f32 target_distance;
    f32 field_68;
    f32 target_angle;
    s32 turn_speed;
    u8 pad_74[4];
    f32 goal[3];
    u8 pad_84[0x10];
    s32 field_94;
    f32 field_A0;
} AI3;

extern f64 __frsqrte(f64);
extern f32 atan2f(f32 y, f32 x);
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern void angLimit(f32 *angle);
extern f32 sLenPoint3dR(f32 *point);
extern void zdChgNormal(ZoidState *zoid);
extern void zdFreeAng(ZoidState *zoid, f32 angle);
extern void ai(s32 index);
extern f32 zdColPosY(f32 *pos, s32 mode);
extern s32 sCamNN;

typedef struct GameModeZA {
    u8 pad_00[0x3C];
    s32 field_3C;
    u8 pad_40[0x8];
} GameModeZA;

extern GameModeZA GameMode;

static inline f32 sqrtf(f32 x, volatile f32 *y)
{
    static const f64 _half = 0.5;
    static const f64 _three = 3.0;

    if (x > 0.0f) {
        f64 guess = __frsqrte((f64)x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        *y = (f32)(x * guess);
        return *y;
    }
    return x;
}

static inline f32 absf(f32 *value)
{
    *(u32 *)value &= 0x7FFFFFFF;
    return *value;
}

static inline f32 absv(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

static inline f32 angleY(f32 x, f32 z)
{
    return atan2f(x, z);
}

static inline f32 wsinf(f32 x)
{
    return sinf(x);
}

static inline f32 wcosf(f32 x)
{
    return cosf(x);
}

extern s32 zdFaceAtk(ZoidState *zoid, f32 distance, f32 degrees);
extern f32 sMisZdAng(ZoidState *zoid, f32 *angle);
extern f32 zdGoY(ZoidState *zoid, f32 angle, f32 distance);
extern void zdMotion(ZoidState *zoid, s32 motion);
extern f32 sMisZd(ZoidState *zoid);
extern s32 sWeaponGetNumber(ZoidState *zoid, s32 mount);
extern void ZoidTargetSet(ZoidState *zoid, s32 index);
extern void zdStep1(ZoidState *zoid, s32 degrees);
extern s32 sWeaponGetMountBullet(ZoidState *zoid, s32 index);
extern void zdWeaponChgSlot(ZoidState *zoid, s32 slot);
extern void zdWeaponChg(ZoidState *zoid);
extern void zdWeaponFire1(ZoidState *zoid);
extern void zdWeaponFireON(ZoidState *zoid);
extern void zdWeaponFireOFF(ZoidState *zoid);
extern void zdAtack1(ZoidState *zoid);
static void zdExecAI(ZoidState *zoid)
{
    f32 base;
    f32 d[3];
    f32 angle;
    f32 turn;
    s32 pad0;
    f32 abs0;
    f32 pad1;
    f32 pad2;
    volatile f32 root;
    f32 abs1;
    f32 abs2;
    AI3 *ai;
    ZoidState *target;
    f32 *dest;
    s32 index;
    f32 dist;
    f32 len;
    f32 sum;
    f32 t;
    s32 i;
    f32 measured_distance;
    f32 desired_distance;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    ai = (AI3 *)zoid->ai;
    if (ai == (void *)0)
        return;
    if (zoid->field_1360 != 0) {
        if (zoid->requested_action == 2)
            zoid->requested_action = 0;
        zoid->field_1C54 = 0.0f;
        return;
    }
    index = ai->target_index;
    if (index != -1) {
        (void)0;
    }
    if (index == -1)
        target = 0;
    else
        target = &Zoid[index];
    if (target != (void *)0 && target->joint == (void *)0)
        target = 0;
    desired_distance = ai->target_distance;
    dist = desired_distance;
    switch (ai->mode) {
    case 0:
        return;
    case 1:
        dest = ai->destination;
        base = 0.0f;
        break;
    case 2:
        if (target == (void *)0)
            return;
        base = target->heading;
        dest = target->position;
        break;
    case 3:
        zoid->step_heading = zoid->heading + 2.0 * (3.1415926 * (f32)ai->turn_speed) / 360.0;
        zoid->field_1C54 = 1.0f;
        angLimit(&zoid->step_heading);
        return;
    case 16:
        zoid->step_heading = ai->field_68;
        zoid->field_1C54 = 1.0f;
        zoid->stick_angle = 0.0f;
        return;
    case 4:
        zoid->step_heading = zoid->heading + 2.0 * (3.1415926 * (f32)ai->turn_speed) / 360.0;
        angLimit(&zoid->step_heading);
        ai->field_68 = zoid->step_heading;
        zoid->field_1C54 = 1.0f;
        ai->mode = 16;
        zoid->stick_angle = 0.0f;
        return;
    case 5:
    case 6:
        angLimit(&ai->target_angle);
        base = ai->target_angle - zoid->heading;
        angLimit(&base);
        if (0.0f == base) {
            zdChgNormal(zoid);
            zoid->step_heading = zoid->heading;
            if (ai->mode == 5) {
                ai->mode = 999;
                return;
            }
        }
        abs0 = base;
        if (absf(&abs0) < 2.0 * (3.1415926 * 2) / 360.0 && ai->mode == 6) {
            zoid->step_heading = zoid->heading;
            zoid->field_1C54 = 1.0f;
            ai->mode = 999;
            return;
        }
        if (zoid->field_1C90[2] > 0.01) {
            zoid->field_1C54 = 0.0f;
            return;
        }
        zoid->field_1C58 = ai->target_angle;
        if (base > 0.0f) {
            zoid->requested_action = 7;
            return;
        } else {
            zoid->requested_action = 6;
            return;
        }
    case 999:
    default:
        return;
    }
    if (ai->flags == 0) {
        zoid->field_1C54 = 0.0f;
        return;
    }
    if (ai->flags & 0x100) {
        if (dist < 1.0f)
            dist = 1.0f;
        sum = 0.0f;
        for (i = 0; i < 3; i++) {
            t = dest[i] - ai->field_58[i];
            d[i] = t;
            sum += t * t;
        }
        sum = sqrtf(sum, &root);
        if (0.0f == sum) {
            for (i = 0; i < 3; i++)
                ai->goal[i] = ai->field_58[i];
        } else {
            for (i = 0; i < 3; i++)
                ai->goal[i] = ai->field_58[i] - dist * d[i] / sum;
        }
    } else {
        for (i = 0; i < 3; i++)
            d[i] = zoid->position[i] - dest[i];
        angle = angleY(d[0], d[2]);
        turn = angle;
        len = sLenPoint3dR(d);
        pad0 = 0;
        ai->field_A0 = len;
        measured_distance = ai->field_A0;
        (void)measured_distance;
        switch (ai->flags & 0xF) {
        case 1:
            len = dist;
            break;
        case 2:
            if (len >= dist)
                len = dist - 0.2;
            break;
        case 3:
            if (len <= dist)
                len = 0.2 + dist;
            break;
        }
        switch (ai->flags & 0xF0) {
        case 0x10:
            turn = base + ai->target_angle - angle;
            angLimit(&turn);
            if (turn > 2.0 * (3.1415926 * 30) / 360.0)
                turn = 2.0 * (3.1415926 * 30) / 360.0;
            if (turn < -(2.0 * (3.1415926 * 30) / 360.0))
                turn = -(2.0 * (3.1415926 * 30) / 360.0);
            angle = turn + angle;
            break;
        case 0x20:
            angle += 2.0 * (3.1415926 * (f32)ai->turn_speed) / 360.0;
            break;
        }
        angLimit(&angle);
        if (len < 0.0f)
            len = 0.0f;
        ai->goal[0] = len * wsinf(angle) + dest[0];
        ai->goal[2] = len * wcosf(angle) + dest[2];
        ai->goal[1] = dest[1];
    }
    for (i = 0; i < 3; i++)
        d[i] = ai->goal[i] - zoid->position[i];
    angle = angleY(d[0], d[2]);
    base = angle - zoid->heading;
    angLimit(&base);
    abs1 = base;
    if (absf(&abs1) > 2.0 * (3.1415926 * 80) / 360.0) {
        if (base > 0.0f)
            zoid->requested_action = 7;
        else
            zoid->requested_action = 6;
        zoid->field_1C58 = angle;
        zoid->field_1C54 = 0.0f;
        return;
    }
    if (zoid->action >= 3 && zoid->action <= 11)
        return;
    zoid->step_heading = angle;
    zoid->field_1C54 = 0.2f;
    if (dist > 3.0f)
        zoid->field_1C54 = 1.0f;
    if (dist <= 0.1)
        zoid->field_1C54 = 0.0f;
    dist = sLenPoint3dR(d);
    if (dist <= 3.0f && (abs2 = base, absf(&abs2)) > 2.0 * (3.1415926 * 20) / 360.0) {
        if (base > 0.0f)
            zoid->requested_action = 7;
        else
            zoid->requested_action = 6;
        zoid->field_1C54 = 0.0f;
        zoid->field_1C58 = angle;
        return;
    }
}
void ZoidAI(void)
{
    ZoidState *zoid;
    s32 first = 1;
    s32 index;

    if (sCamNN != 1) {
        first = 2;
    }
    if (GameMode.field_3C != 0) {
        first = 0;
    }
    Zoid[0].ai_active = 0;
    Zoid[1].ai_active = 0;
    for (index = first; index < 16; index++) {
        zoid = &Zoid[index];
        if (zoid->joint != (void *)0 && zoid->death_started == 0) {
            zoid->ai_active = 1;
            ai(index);
        }
    }
    for (index = 0; index < first; index++) {
        zoid = &Zoid[index];
        if (zoid->joint != 0) {
            ((AI3 *)zoid->ai)->mode = 0;
        }
    }
    for (index = first; index < 16; index++) {
        zoid = &Zoid[index];
        if (zoid->death_started == 0) {
            zdExecAI(zoid);
        }
    }
}
void zdAI_WaitMotion(s32 index, s32 motion)
{
    ZoidState *zoid;
    AI3 *ai;

    if (index < 0) {
        return;
    }
    if (index >= 16) {
        return;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return;
    }
    ai = (AI3 *)zoid->ai;
    if (ai == (void *)0) {
        return;
    }
    ai->mode = 0;
    zdMotion(zoid, motion);
}

void zdAI_WeaponChg(s32 index)
{
    if (index >= 0 && index < 16) {
        zdWeaponChg(&Zoid[index]);
    }
}

void zdAI_WeaponChgSlot(s32 index, s32 slot)
{
    if (index >= 0 && index < 16 && slot <= 9) {
        zdWeaponChgSlot(&Zoid[index], slot);
    }
}

void zdAI_WeaponFire1(s32 index)
{
    if (index >= 0 && index < 16) {
        zdWeaponFire1(&Zoid[index]);
    }
}

void zdAI_WeaponFireON(s32 index)
{
    if (index >= 0 && index < 16) {
        zdWeaponFireON(&Zoid[index]);
    }
}

void zdAI_WeaponFireOFF(s32 index)
{
    if (index >= 0 && index < 16) {
        zdWeaponFireOFF(&Zoid[index]);
    }
}

s32 zdAI_WeaponBullet(s32 index)
{
    ZoidState *zoid;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    zoid = &Zoid[index];
    return sWeaponGetMountBullet(zoid, zoid->weapon_mount);
}

void zdAI_Atack1(s32 index)
{
    if (index >= 0 && index < 16) {
        zdAtack1(&Zoid[index]);
    }
}
s32 zdAI_SearchEnemy(s32 index)
{
    ZoidState *zoid;
    s32 j;
    s32 *list;
    s32 i;
    ZoidState *other;
    s32 a;
    s32 b;

    if (index < 0) {
        return -1;
    }
    if (index >= 16) {
        return -1;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return -1;
    }
    list = ((AI3 *)zoid->ai)->nearest_enemies;
    for (i = 0; i < 16; i++) {
        list[i] = -1;
    }
    for (i = 0, j = 0; i < 16; i++) {
        other = &Zoid[i];
        if (other->joint != (void *)0 && zoid != other && zoid->slot != other->slot && zoid->target_visibility[i] != 0) {
            list[j] = i;
            j++;
        }
    }
    for (i = 0; i < 16; i++) {
        a = list[i];
        if (a != -1) {
            for (j = i; j < 16; j++) {
                b = list[j];
                if (b != -1 && j != i) {
                    if (zoid->distances[a] < zoid->distances[b]) {
                    } else {
                        list[i] = b;
                        list[j] = a;
                        a = b;
                    }
                }
            }
        }
    }
    return list[0];
}

static void goFrom(ZoidState *zoid)
{
    if (zoid == 0) {
        return;
    }
    if (zoid->joint == 0) {
        return;
    }
    if (zoid->death_started != 0) {
        return;
    }
    if (zoid->action == 0) {
        return;
    }
    if (zoid->action != 0) {
        return;
    }
    if (zoid->jump_phase != 0) {
        return;
    }
    if (zoid->action == 0x1B) {
        return;
    }
    if (zoid->action == 0x20) {
        return;
    }
    if (zoid->action == 0x21) {
        return;
    }
    if (zoid->action == 0x1E) {
        return;
    }
    if (zoid->action == 0x1D) {
        return;
    }
    if (zoid->action == 0x1C) {
        return;
    }
    if (zoid->action == 0x1F) {
        return;
    }
    if (zoid->action == 0x22) {
        return;
    }
    if (zoid->action == 0x27) {
        return;
    }
    if (zoid->action == 0x28) {
        return;
    }
    if (zoid->action == 0x25) {
        return;
    }
    if (zoid->action == 0x24) {
        return;
    }
    if (zoid->action == 0x23) {
        return;
    }
    if (zoid->action == 0x26) {
        return;
    }
    if (zoid->action == 0x1A) {
        return;
    }
    if (zoid->action == 0x19) {
        return;
    }
    if (zoid->action == 0x17) {
        return;
    }
    if (zoid->action == 0x18) {
        return;
    }
    if (zoid->action == 0x30) {
        return;
    }
    if (zoid->action == 0x2F) {
        return;
    }
    if (zoid->action == 0x2D) {
        return;
    }
    if (zoid->action == 0x2E) {
        return;
    }
    zoid->action = 0;
}
void zdAI_goPoint(s32 index, f32 x, f32 z)
{
    ZoidState *zoid;
    AI3 *ai;

    if (index >= 0 && index < 16) {
        zoid = &Zoid[index];
        if (zoid->joint != (void *)0) {
            ai = (AI3 *)zoid->ai;
            if (ai != (void *)0) {
                goFrom(zoid);
                ai->mode = 1;
                ai->flags = 2;
                ai->target_distance = 5.0f;
                ai->destination[0] = x;
                ai->destination[2] = z;
                ai->destination[1] = 100.0f;
                ai->destination[1] = zdColPosY(ai->destination, 0);
            }
        }
    }
}
void zdAI_rotPoint(s32 index, f32 x, f32 z, f32 distance, f32 angle)
{
    ZoidState *zoid;
    AI3 *ai;

    if (index < 0) {
        return;
    }
    if (index >= 16) {
        return;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return;
    }
    ai = (AI3 *)zoid->ai;
    if (ai == (void *)0) {
        return;
    }
    goFrom(zoid);
    ai->mode = 1;
    ai->flags = 0x21;
    ai->turn_speed = (s32)(angle / (2.0 * 3.1415926) * 360.0);
    ai->target_distance = distance;
    ai->destination[0] = x;
    ai->destination[2] = z;
    ai->destination[1] = 100.0f;
    ai->destination[1] = 100.0f + zdColPosY(ai->destination, 0);
    ai->destination[1] = zdColPosY(ai->destination, 0);
}
void zdAI_SetZoid0(s32 index, s32 target)
{
    AI3 *ai;
    ZoidState *zoid;
    ZoidState *other;

    if (index < 0) {
        return;
    }
    if (index >= 16) {
        return;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return;
    }
    if (target == -1) {
        return;
    }
    other = &Zoid[target];
    if (other->joint == (void *)0) {
        return;
    }
    ai = (AI3 *)zoid->ai;
    if (ai == (void *)0) {
        return;
    }
    goFrom(zoid);
    ai->target_index = target;
    ai->mode = 2;
    ai->flags = 0;
}

void zdAI_SetLockZoid(s32 index)
{
    ZoidState *zoid;
    s32 slot;

    if (index >= 0 && index < 16) {
        zoid = &Zoid[index];
        if (zoid->joint != (void *)0) {
            slot = zoid->target_slot;
            zdAI_SetZoid0(index, slot);
        }
    }
}
s32 zdAI_IsTurn(s32 index)
{
    ZoidState *zoid;
    AI3 *ai;
    s32 mode;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return 0;
    }
    ai = (AI3 *)zoid->ai;
    mode = ai->mode;
    switch (mode) {
    case 4:
    case 5:
    case 6:
        return 1;
    }
    return 0;
}
void zdAI_Run(s32 index, s32 degrees)
{
    ZoidState *zoid;
    AI3 *ai;

    if (index >= 0 && index < 16) {
        zoid = &Zoid[index];
        if (zoid->joint != (void *)0) {
            ai = (AI3 *)zoid->ai;
            goFrom(zoid);
            ai->mode = 3;
            ai->flags = 0;
            ai->turn_speed = degrees;
            ai->target_distance = 0.0f;
        }
    }
}
void zdAI_RunAng(s32 index, s32 degrees)
{
    ZoidState *zoid;
    AI3 *ai;

    if (index < 0) {
        return;
    }
    if (index >= 16) {
        return;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return;
    }
    ai = (AI3 *)zoid->ai;
    goFrom(zoid);
    ai->mode = 4;
    ai->flags = 0;
    ai->turn_speed = degrees;
    ai->target_distance = 0.0f;
}
void zdAI_StopAngRun(s32 index, s32 degrees)
{
    ZoidState *zoid;
    AI3 *ai;

    if (index < 0) {
        return;
    }
    if (index >= 16) {
        return;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return;
    }
    ai = (AI3 *)zoid->ai;
    if (ai->mode == 6) {
        return;
    }
    goFrom(zoid);
    ai->mode = 6;
    ai->flags = 0;
    ai->target_angle = zoid->heading + (2.0 * (3.1415926 * (f32)degrees)) / 360.0;
}
void zdAI_SetLen1(s32 index, f32 distance, u32 radius_mode)
{
    ZoidState *zoid;
    AI3 *ai;

    (void)0;
    (void)0;
    (void)0;
    (void)0;
    (void)0;
    if (index < 0) {
        return;
    }
    if (index >= 16) {
        return;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return;
    }
    ai = (AI3 *)zoid->ai;
    ai->target_distance = distance;
    ai->flags = (ai->flags & ~0xF) | radius_mode;
}
void zdAI_SetLenIn(s32 index, f32 distance)
{
    zdAI_SetLen1(index, distance, 2);
}
void zdAI_SetLenOut(s32 index, f32 distance)
{
    zdAI_SetLen1(index, distance, 3);
}
void zdAI_SetLen(s32 index, f32 distance)
{
    zdAI_SetLen1(index, distance, 1);
}
void zdAI_SetAngle(s32 index, s32 angle_mode, s32 degrees, s32 speed)
{
    ZoidState *zoid;
    AI3 *ai;

    (void)0;
    (void)0;
    (void)0;
    (void)0;
    (void)0;
    if (index >= 0 && index < 16) {
        zoid = &Zoid[index];
        if (zoid->joint != (void *)0) {
            ai = (AI3 *)zoid->ai;
            goFrom(zoid);
            ai->mode = 2;
            ai->flags = (ai->flags & ~0xF0) | (angle_mode << 4);
            ai->target_angle = 2.0 * (3.1415926 * (f32)degrees) / 360.0;
            ai->turn_speed = speed;
        }
    }
}
void zdAI_SetAngleON(s32 index, s32 degrees)
{
    zdAI_SetAngle(index, 1, degrees, 0);
}
void zdAI_SetAngleSpd(s32 index, s32 speed)
{
    zdAI_SetAngle(index, 2, 0, speed);
}

void zdAI_TargetSet(s32 index, s32 target)
{
    ZoidState *zoid;

    if (index < 16) {
        zoid = &Zoid[index];
        if (zoid->joint != (void *)0) {
            if (index < 0) {
                zoid->target_pending = 0;
            } else {
                ZoidTargetSet(zoid, target);
            }
        }
    }
}

void zdAI_Jump(s32 index)
{
    ZoidState *zoid;

    if (index >= 0 && index < 16) {
        zoid = &Zoid[index];
        if (zoid->joint != (void *)0) {
            zoid->requested_action = 2;
        }
    }
}

s32 zdAI_IsAtack(s32 index)
{
    ZoidState *zoid;
    ZoidState *other;
    s32 i;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        other = &Zoid[i];
        if (other->joint != (void *)0 && zoid->attack_in_range[i] == 1) {
            return 1;
        }
    }
    return 0;
}

f32 zdAI_ComeMissile(s32 index)
{
    ZoidState *zoid;
    f32 result;

    result = -1.0f;
    if (index < 0) {
        return result;
    }
    if (index >= 16) {
        return result;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return result;
    }
    return sMisZd(zoid);
}

f32 zdAI_ComeMissileAng(s32 index, s32 *out)
{
    ZoidState *zoid;
    f32 angle;
    f32 result;

    result = -1.0f;
    if (out != 0) {
        *out = 0;
    }
    if (index < 0) {
        return result;
    }
    if (index >= 16) {
        return result;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return result;
    }
    result = sMisZdAng(zoid, &angle);
    if (out != 0) {
        *out = (s32)(180.0f * angle / 3.1415926);
    }
    return result;
}
void zdAI_Step(s32 index, s32 degrees)
{
    ZoidState *zoid;

    if (index >= 0 && index < 16) {
        zoid = &Zoid[index];
        if (zoid->joint != (void *)0) {
            zdStep1(zoid, degrees);
        }
    }
}
s32 zdAI_Look(s32 index)
{
    ZoidState *zoid;
    ZoidState *target;
    s32 slot;
    f32 angle;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return 0;
    }
    if (zoid->target_pending == 0) {
        return 0;
    }
    slot = zoid->target_slot;
    if (slot < 0) {
        return 0;
    }
    if (slot >= 16) {
        return 0;
    }
    target = &Zoid[slot];
    if (target->joint == (void *)0) {
        return 0;
    }
    if (zoid == target) {
        return 0;
    }
    if (zoid->field_1C54 > 1.0f) {
        zoid->field_1C54 = 0.0f;
        return 0;
    }
    if (zoid->field_1C90[0] > 1.0f) {
        zoid->field_1C90[0] -= 0.1;
        if (zoid->field_1C90[0] < 0.0f) {
            zoid->field_1C90[0] = 0.0f;
        }
        return 0;
    }
    angle = zoid->target_angles[slot];
    if (absv(angle) < 2.0 * (3.1415926 * 5) / 180.0) {
        zdChgNormal(zoid);
        return 1;
    }
    if (angle > 0.0f) {
        zoid->requested_action = 0xB;
    } else {
        zoid->requested_action = 0xA;
    }
    zoid->field_1C54 = 1.0f;
    return 0;
}

f32 zdAI_goY(s32 index, s32 degrees, f32 distance)
{
    ZoidState *zoid;

    if (index < 0) {
        return 0.0f;
    }
    if (index >= 16) {
        return 0.0f;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return 0.0f;
    }
    return zdGoY(zoid, (2.0 * (3.1415926 * (f64)(f32)degrees)) / 360.0, distance);
}
s32 zdAI_goWall(s32 index, s32 distance, s32 degrees)
{
    ZoidState *zoid;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return 0;
    }
    return zdFaceAtk(zoid, (f32)distance, (f32)(2.0 * (3.1415926 * (f32)degrees) / 360.0));
}

s32 zdAI_WeaponGetID(s32 index)
{
    ZoidState *zoid;
    s32 mount;

    if (index < 0) {
        return -1;
    }
    if (index >= 16) {
        return -1;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return -1;
    }
    mount = zoid->weapon_mount;
    return sWeaponGetNumber(zoid, mount);
}

s32 zdAI_AngZoidME2(s32 index, s32 target)
{
    ZoidState *zoid;
    ZoidState *other;
    f32 degrees;
    s32 result;

    if (index < 0) {
        return -1;
    }
    if (index >= 16) {
        return -1;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return -1;
    }
    if (target < 0) {
        return -1;
    }
    if (target >= 16) {
        return -1;
    }
    other = &Zoid[target];
    if (other->joint == (void *)0) {
        return -1;
    }
    degrees = 180.0 * zoid->target_angles[target] / 3.1415926;
    result = (s32)degrees;
    return result;
}

s32 zdAI_AngZoidEM(s32 index)
{
    ZoidState *zoid;
    ZoidState *target;
    s32 target_index;
    f32 degrees;
    s32 result;

    if (index < 0) {
        return -1;
    }
    if (index >= 16) {
        return -1;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return -1;
    }
    target_index = zoid->target_slot;
    if (target_index < 0) {
        return -1;
    }
    if (target_index >= 16) {
        return -1;
    }
    target = &Zoid[target_index];
    if (target->joint == (void *)0) {
        return -1;
    }
    degrees = 180.0 * target->target_angles[index] / 3.1415926;
    result = (s32)degrees;
    return result;
}

s32 zdAI_IsAtackMineN(s32 index)
{
    ZoidState *zoid;
    ZoidState *other;
    s32 action;
    s32 i;

    if (index < 0) {
        return -1;
    }
    if (index >= 16) {
        return -1;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        if (i == index) {
            continue;
        }
        other = &Zoid[i];
        if (other->joint == (void *)0) {
            continue;
        }
        if (other->attack_in_range[index] == 0) {
            continue;
        }
        action = other->action;
        if (action >= 13 && action <= 22) {
            return i;
        }
    }
    return -1;
}

s32 zdAI_IsAtackMine(s32 index)
{
    return zdAI_IsAtackMineN(index) != -1;
}

s32 zdAI_IsTarget(s32 index)
{
    ZoidState *zoid;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return 0;
    }
    if (zoid->target_pending == 0) {
        return 0;
    }
    return 1;
}
s32 zdAI_WarningArea(s32 index)
{
    s32 polygon_index;
    s32 material;
    GroundMask *material_mask;
    GroundMask *mask;
    f32 offset;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    {
        ZoidState *zoid = &Zoid[index];
        if (zoid->joint == (void *)0) {
            return 0;
        }
        offset = absv(zoid->joint->matrix[0][3]);
        if (offset > 2900.0f) {
            return 1;
        }
        offset = absv(zoid->joint->matrix[2][3]);
        if (offset > 2900.0f) {
            return 1;
        }
        polygon_index = zoid->ground_polygon;
        if (polygon_index == 0xFFFF) {
            return 1;
        }
        material = grdcPolygon[polygon_index].material;
        material_mask = &grdcMat[material];
        mask = material_mask;
        if (zoid->field_1E78 > 60) {
            return 1;
        }
        if (mask->bits[0] & 4) {
            return 1;
        }
        return 0;
    }
}
#undef lit_1132
#define lit_1132 "E5@"
void zdAI_UseOption(s32 index)
{
    ZoidState *zoid;
    AI3 *ai;

    if (index < 0) {
        return;
    }
    if (index >= 16) {
        return;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return;
    }
    ai = (AI3 *)zoid->ai;
    if (ai != 0) {
        ai->field_94 = 1;
    }
}
s32 zdAI_coHead(s32 index)
{
    ZoidState *zoid;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return 0;
    }
    return zoid->head_hit != 0 || zoid->neck_hit != 0;
}

s32 zdAI_lenStep(s32 index)
{
    ZoidState *zoid;
    void *anim;
    s32 value;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return 0;
    }
    anim = zoid->anim_data;
    if (anim == 0) {
        return 0;
    }
    value = *(s32 *)((u8 *)anim + 0xd0);
    return value / 2;
}

void zdAI_pointAng(s32 index, s32 a, s32 b, s32 *out_x, s32 *out_z)
{
    ZoidState *zoid;
    f32 pos[3];
    f32 angle;
    s32 i;
    f32 sine_angle;
    f32 cosine_angle;

    if (out_x == 0 || out_z == 0)
        return;
    if (index < 0)
        return;
    if (index >= 16)
        return;
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0)
        return;
    for (i = 0; i < 3; i++) {
        pos[i] = zoid->position[i];
    }
    angle = zoid->heading + 2.0 * (3.1415926 * (f32)a) / 360.0;
    angLimit(&angle);
    sine_angle = angle;
    pos[0] += (f32)b * sinf(sine_angle);
    cosine_angle = angle;
    pos[2] += (f32)b * cosf(cosine_angle);
    *out_x = (s32)pos[0];
    *out_z = (s32)pos[2];
}

void zdAI_stopNow(s32 index)
{
    ZoidState *zoid;
    AI3 *ai;

    if (index < 0) {
        return;
    }
    if (index >= 16) {
        return;
    }
    zoid = &Zoid[index];
    if (zoid->joint == (void *)0) {
        return;
    }
    ai = (AI3 *)zoid->ai;
    if (ai != 0) {
        zoid->field_1C90[0] = 0.0f;
        zoid->field_1C90[2] = 0.0f;
        zoid->field_1C90[1] = 0.0f;
        zoid->field_1C54 = 0.0f;
        ai->mode = 999;
    }
}

f32 zdAI_lenPoint(s32 index, f32 x, f32 z)
{
    ZoidState *zoid;
    volatile f32 root;

    if (index < 0) {
        return 0.0f;
    }
    if (index >= 16) {
        return 0.0f;
    }
    zoid = &Zoid[index];
    x = x - zoid->position[0];
    z = z - zoid->position[2];
    return sqrtf(x * x + z * z, &root);
}

s32 zdAI_angPoint(s32 index, f32 x, f32 z)
{
    ZoidState *zoid;
    f32 angle;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    zoid = &Zoid[index];
    if (x == zoid->position[0] && z == zoid->position[2]) {
        return 0;
    }
    x = x - zoid->position[0];
    z = z - zoid->position[2];
    angle = atan2f(x, z);
    angle = angle - zoid->heading;
    return (s32)(360.0 * (angle / (2.0 * 3.1415926)));
}
s32 zdAI_freeAng(s32 index, s32 angle)
{
    ZoidState *zoid;
    AI3 *ai;
    f32 base;
    f32 turn;
    s32 action;

    if (index < 0) {
        return 1;
    }
    if (index >= 16) {
        return 1;
    }
    zoid = &Zoid[index];
    if (zoid->joint == 0) {
        return 1;
    }
    if (zoid->field_1360 != 0) {
        return 0;
    }
    ai = (AI3 *)zoid->ai;
    ai->mode = 999;
    action = zoid->action;
    if (zoid->field_2060 == -1) {
        base = zoid->field_205C;
        turn = base - zoid->heading;
        angLimit(&turn);
        if (action >= 3 && action <= 11) {
            zoid->field_1C54 = 1.0f;
            if (absv(turn) < 2.0 * (3.1415926 * 5) / 180.0) {
                zoid->field_1C54 = 0.0f;
                zoid->heading = zoid->field_205C;
                zdChgNormal(zoid);
                return 1;
            }
            return 0;
        }
        if (turn == 0.0f) {
            return 1;
        }
        if (absv(turn) < 2.0 * (3.1415926 * 5) / 180.0) {
            zoid->field_1C54 = 0.0f;
            zoid->heading = base;
            zdChgNormal(zoid);
            return 1;
        }
        if (turn > 0.0f) {
            zoid->requested_action = 5;
        } else {
            zoid->requested_action = 4;
        }
        if (action == 0) {
            zoid->action = zoid->requested_action;
            zoid->action_phase = 0;
        }
        zoid->field_1C54 = 1.0f;
        return 0;
    }
    zoid->field_1C90[0] = 0.0f;
    zdFreeAng(zoid, (f32)angle);
    return 0;
}
}
