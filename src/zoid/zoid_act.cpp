extern "C" {
#pragma readonly_strings on
#pragma bool off
#include "types.h"
#include "joint.h"
#include "zoid_collision.h"
#include "ground_collision.h"
#include "math_support.h"

#define DEG(x) (2.0 * (3.1415926 * (x)) / 360.0)

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
    u8 pad_0000[0x20];
    s32 type;
    u8 pad_0024[0x8];
    void *camera;
    u8 pad_0030[0x10];
    HSD_JObj *joint;
    void *pose;
    u8 pad_0048[0x100];
    s32 field_0150;
    s32 health;
    s32 max_health;
    u8 pad_0154[0x2];
    u8 option_use_state;
    u8 pad_0157[0xB];
    s16 booster_on;
    u8 pad_0164[0x8];
    CoDamage damages[6];
    u8 pad_01B4[0x2A8];
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
    u8 pad_1128[0x138];
    HSD_JObj *body_base_joint;
    u8 pad_1264[0x8];
    HSD_JObj *bust_base_joint;
    u8 pad_1270[0x30];
    s32 face_lock;
    u8 pad_12A4[0x50];
    s32 field_1360;
    u8 pad_12F8[0x4];
    s32 weapon_mount;
    u8 pad_1300[0x8];
    s32 field_1374;
    u8 pad_130C[0x558];
    struct {
        HSD_JObj *joint;
        u8 reserved_04[0x8];
    } weapon_slots[8];
    u8 pad_18C4[0x100];
    s32 camera_joint;
    u8 pad_19C8[0xC];
    f32 field_1A40[3];
    s32 ground_turn;
    f32 field_1A78;
    HSD_JObj *field_1A7C;
    f32 field_1A80[3];
    u8 pad_19F8[0x4];
    f32 field_1A9C[3];
    u8 pad_1A08[0x1C];
    HSD_JObj *f_base_joint;
    HSD_JObj *b_base_joint;
    HSD_JObj *field_1ACC;
    HSD_JObj *buckle_joint;
    u8 pad_1A34[0x4];
    HSD_JObj *field_1AD8;
    HSD_JObj *field_1ADC;
    u8 pad_1A40[0x4];
    f32 senter_translation_y;
    f32 senter_translation_z;
    f32 senter_roll;
    FootJoint feet[4];
    f32 ground_normal_sum[3];
    s32 ground_normal_count;
    u8 pad_1B30[0x8];
    s32 death_pending;
    s32 field_1BE4;
    u8 pad_1B40[0x8];
    f32 ground_height;
    f32 field_19FC;
    u8 pad_1B50[0x4];
    s32 field_1BFC;
    s32 foot_contacts;
    s32 grounded;
    s32 grounded_frames;
    u8 pad_1B64[0x4];
    f32 position[3];
    f32 field_1C24[3];
    f32 height_offset;
    s32 field_1C34;
    f32 height_offset_speed;
    u8 pad_1B8C[0x8];
    s32 movement_flags;
    u8 pad_1B98[0x4];
    s32 field_1C4C;
    f32 step_heading;
    f32 field_1C54;
    f32 field_1C58;
    u8 pad_1BAC[0x8];
    f32 stick_angle;
    f32 field_1C74;
    u8 pad_1BBC[0x4];
    f32 heading;
    f32 high_angle;
    f32 field_1C84;
    f32 target_heading;
    u8 pad_1BD0[0x4];
    f32 field_1C90[3];
    f32 field_1C9C;
    f32 steer;
    f32 f_base_ry;
    f32 face_yaw;
    f32 face_pitch;
    u8 pad_1BF4[0x4];
    f32 ground_rz;
    u8 pad_1BFC[0x10];
    f32 field_1CE8;
    f32 slide_speed;
    f32 field_1CF0;
    s32 slide_frames;
    s32 field_1CF8;
    f32 launch_velocity[3];
    f32 knockback[3];
    f32 field_1D20[3];
    f32 field_1D2C;
    f32 displacement[3];
    f32 speed;
    f32 previous_displacement[3];
    f32 displacement_change[3];
    u8 pad_1C70[0x3C];
    s32 field_1DA0;
    s32 requested_action;
    s32 action;
    s32 field_1DAC;
    s32 field_1DB0;
    s32 action_phase;
    s32 gene_frames;
    u32 field_1DBC;
    s32 jump_phase;
    s32 jump_attack_phase;
    f32 velocity[3];
    s32 field_1DD4;
    s32 current_animation;
    s32 previous_animation;
    s32 field_1DEC;
    s32 attack_index;
    s32 field_1DF8;
    f32 child_matrix[3][4];
    f32 heading_delta;
    s32 anim1_frames;
    u8 pad_1D30[0x4];
    s32 animation_stopped;
    s32 animation_looped;
    u8 pad_1D3C[0xC];
    s32 field_1E50;
    s32 field_1E54;
    s32 health_bar_timer;
    s32 field_1E5C;
    u8 pad_1D58[0xC];
    s32 death_started;
    s32 lock_frames;
    u8 pad_1D6C[0xC];
    s32 rumble_timer;
    u8 pad_1D7C[0x1];
    u8 slot;
    u8 target_pending;
    u8 target_slot;
    s8 attack_target;
    u8 pad_1D81[0x3];
    s32 field_1BA4;
    u8 pad_1D88[0xC];
    u16 field_1EAE;
    u16 field_1EB0;
    u16 field_1EB2;
    u16 ground_polygon;
    u16 field_1EBA;
    u8 pad_1D9E[0x6];
    Vec3 *ground_normal;
    u8 pad_1DA8[0x4];
    s32 stel_state;
    u8 pad_1DB0[0xC];
    s32 interpolation_remaining;
    s32 interpolation_duration;
    u8 pad_1DC4[0x18];
    s32 field_1F04;
    f32 distances[16];
    u8 pad_1E20[0x10];
    u8 attack_in_range[16];
    u8 pad_1E40[0x10];
    f32 target_angles[16];
    u8 pad_1E90[0x6C];
    u16 *slider_info;
    u8 pad_1F00[0x28];
    void *field_2090;
    void *anim_data;
    struct ZoidAIState *ai;
    s32 ai_active;
    u8 pad_1F38[0x4];
    s32 gene_x;
    s32 gene_z;
    s32 field_20B4;
    s32 field_20B8;
    s32 field_20BC;
    u8 pad_1F50[0x1C];
    s32 speed_bonus;
    u8 pad_1F70[0x8];
    s32 turn_bonus;
    u8 pad_1F7C[0x10];
} ZoidState;

typedef char ZoidStateSizeCheck[sizeof(ZoidState) == 0x1F8C ? 1 : -1];

extern ZoidState Zoid[];
extern u32 vsyncN;
extern f64 sin(f64 x);
extern void ZoidSetAnim(ZoidState *zoid, s32 anim);
extern void setAnimRate(HSD_JObj *joint, f32 rate);

extern void flyZoid(ZoidState *zoid);
extern void dmgZoid(ZoidState *zoid);
extern f32 sLenPoint3dR(f32 *point);
extern void ZoidToFly(ZoidState *zoid);
extern void ZoidSiseiFace(ZoidState *zoid);
extern f32 zdColPosY(f32 *point, u16 *selector);
extern void zdChgNormal(ZoidState *zoid);
extern void zdLinierTm(ZoidState *zoid, s32 frames);
extern void ZoidWalk(ZoidState *zoid);
extern void ZoidSuberi(ZoidState *zoid);
extern void zdZuri(ZoidState *zoid);
extern void ZoidJump(ZoidState *zoid);
extern void zdModeGene(ZoidState *zoid);
extern void ZoidTurnR(ZoidState *zoid, s32 mode);
extern void ZoidTurnL(ZoidState *zoid, s32 mode);
extern void ZoidStop1(ZoidState *zoid);
extern void ZoidDash1(ZoidState *zoid);
extern void ZoidActAtack(ZoidState *zoid);
extern void ZoidActStep(ZoidState *zoid);
extern void ZoidActDmg2(ZoidState *zoid);
extern void ZoidActDmg3(ZoidState *zoid);
extern void ZoidActDeadly(ZoidState *zoid);
extern void ZoidActDead(ZoidState *zoid);
extern void ZoidActDeath(ZoidState *zoid);
extern void ZoidActMotion(ZoidState *zoid);
extern void ZoidActWin(ZoidState *zoid);
extern void angLimit(f32 *angle);
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern f32 atan2f(f32 y, f32 x);

static inline f32 absf(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

static inline f32 vector_heading(f32 x, f32 z)
{
    return atan2f(x, z);
}
extern void ZoidInertia(ZoidState *zoid);
typedef struct ZoidDat3 {
    u8 reserved_000[0x652];
    s16 attack_effects[12][2];
    s16 attack_effect_mounts[12][2];
    s16 attack_range[24];
    u8 reserved_6E2[0x18];
    s16 attack_tm[24];
    u8 reserved_72A[0xBE];
} ZoidDat3;
extern ZoidDat3 Tea_zoiddat[];
extern void __assert(const char *file, s32 line, const char *expr);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *joint);
#define JOBJ_MTX_INDEP_SRT 0x02000000
#define JOBJ_USER_DEF_MTX 0x800000
#define JOBJ_MTX_DIRTY 0x40
#define JOBJ_USE_QUATERNION 0x20000
static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 605, "jobj");
    return !(jobj->flags & JOBJ_USER_DEF_MTX) && (jobj->flags & JOBJ_MTX_DIRTY);
}
#define HSD_JObjSetMtxDirty(jobj) \
    do { \
        if ((jobj) != 0 && !HSD_JObjMtxIsDirty(jobj)) \
            HSD_JObjSetMtxDirtySub(jobj); \
    } while (0)
static inline void HSD_JObjSetRotationX(HSD_JObj *jobj, f32 x)
{
    jobj ? (void)0 : __assert("jobj.h", 676, "jobj");
    !(jobj->flags & JOBJ_USE_QUATERNION) ? (void)0 : __assert("jobj.h", 677, "!(jobj->flags & JOBJ_USE_QUATERNION)");
    jobj->rotation[0] = x;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}
static inline void HSD_JObjSetRotationY(HSD_JObj *jobj, f32 y)
{
    jobj ? (void)0 : __assert("jobj.h", 696, "jobj");
    !(jobj->flags & JOBJ_USE_QUATERNION) ? (void)0 : __assert("jobj.h", 697, "!(jobj->flags & JOBJ_USE_QUATERNION)");
    jobj->rotation[1] = y;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}
static inline void HSD_JObjSetTranslateX(HSD_JObj *jobj, f32 x)
{
    jobj ? (void)0 : __assert("jobj.h", 952, "jobj");
    jobj->translation.x = x;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}
static inline void HSD_JObjSetTranslateY(HSD_JObj *jobj, f32 y)
{
    jobj ? (void)0 : __assert("jobj.h", 966, "jobj");
    jobj->translation.y = y;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}
static inline void HSD_JObjSetTranslateZ(HSD_JObj *jobj, f32 z)
{
    jobj ? (void)0 : __assert("jobj.h", 980, "jobj");
    jobj->translation.z = z;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

s32 zdLakkaTest2(ZoidState *zoid)
{
    if (zoid == (void *)0) {
        return 0;
    }
    if (zoid->joint == (void *)0) {
        return 0;
    }
    if ((zoid->movement_flags & 3) != 2) {
        return 0;
    }
    if (zoid->ground_normal_count != 0) {
        return 0;
    }
    if (*(void **)(zoid->r_arm2_coobj + 0x18) != 0) {
        return 0;
    }
    if (*(void **)(zoid->r_arm1_coobj + 0x18) != 0) {
        return 0;
    }
    if (*(void **)(zoid->l_arm2_coobj + 0x18) != 0) {
        return 0;
    }
    if (*(void **)(zoid->l_arm1_coobj + 0x18) != 0) {
        return 0;
    }
    if (*(void **)(zoid->r_leg2_coobj + 0x18) != 0) {
        return 0;
    }
    if (*(void **)(zoid->r_leg1_coobj + 0x18) != 0) {
        return 0;
    }
    if (*(void **)(zoid->l_leg2_coobj + 0x18) != 0) {
        return 0;
    }
    if (*(void **)(zoid->l_leg1_coobj + 0x18) != 0) {
        return 0;
    }
    return 1;
}
typedef struct ZoidAnim3 {
    u8 pad_00[0xA8];
    s32 anim7_rate;
    u8 pad_AC[4];
    s32 anim4_rate;
    s32 anim5_min_speed;
    s32 anim5_rate;
    u8 pad_BC[4];
    s32 anim6_rate;
    s32 max_steer;
    u8 pad_C8[0x2C];
    s32 f0F4;
    s32 f0F8;
} ZoidAnim3;

static inline f32 fabsf(f32 *v)
{
    return (*(u32 *)v &= 0x7FFFFFFF, *v);
}

void ZoidWalkSpeed(ZoidState *zoid)
{
    s32 anim;
    s32 cur;
    s32 half;
    s32 broken;
    s32 slow;
    s32 sub;
    f32 max;
    f32 limit;
    ZoidAnim3 *control;
    f32 speed;
    f32 steer;
    f32 diff;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    anim = zoid->current_animation;
    half = 0;
    control = (ZoidAnim3 *)zoid->anim_data;
    cur = zoid->current_animation;
    speed = zoid->field_1C90[0];
    steer = zoid->steer;
    if ((anim == 2 || anim == 4 || anim == 5) && control->f0F8 == 0)
        half = 1;
    if (control->f0F4 == 0)
        half = 0;
    if (zoid->feet[2].joint == 0 && zoid->feet[3].joint == 0)
        half = 0;
    if (half != 0 && zoid->feet[2].joint != 0 && zoid->feet[2].no_contact_frames < 90)
        half = 0;
    if (half != 0 && zoid->feet[3].joint != 0 && zoid->feet[3].no_contact_frames < 90)
        half = 0;
    if (half != 0 && zoid->feet[0].joint != 0 && zoid->feet[0].no_contact_frames < 90)
        half = 0;
    if (half != 0 && zoid->feet[1].joint != 0 && zoid->feet[1].no_contact_frames < 90)
        half = 0;
    if (half != 0) {
        zoid->field_1C90[1] *= 0.5f;
        zoid->field_1C90[0] *= 0.5f;
        zoid->field_1C90[2] *= 0.5f;
    }
    broken = 0;
    if (zoid->damages[4].maximum_health != 0 && zoid->damages[4].health == 0)
        broken |= 1;
    if (zoid->damages[5].maximum_health != 0 && zoid->damages[5].health == 0)
        broken |= 2;
    if (zoid->damages[2].maximum_health != 0 && zoid->damages[2].health == 0)
        broken |= 4;
    if (zoid->damages[3].maximum_health != 0 && zoid->damages[3].health == 0)
        broken |= 8;
    if (zoid->damages[0].maximum_health != 0 && zoid->damages[0].health == 0)
        broken |= 16;
    if ((broken & 3) && (anim == 4 || anim == 5) && anim != 4) {
        slow = 0;
        if (broken & 1)
            slow += 5;
        if (broken & 2)
            slow += 5;
    }
    if ((broken & 0x10) && (anim == 4 || anim == 5)) {
        angLimit(&steer);
        if (speed >= 0.5) {
            steer = steer / 2.0;
            steer = DEG(10) * ((1.0 + sin(3.141592 * (vsyncN * 2) / 120.0)) / 2.0) + steer;
            if (steer > DEG(175))
                steer = DEG(175);
            if (steer < -DEG(175))
                steer = -DEG(175);
        }
    }
    if (0.0f < speed && speed < 0.1)
        speed = 0.0f;
    if (speed < -0.3)
        speed = -0.3f;
    if (-0.05 < speed && speed < 0.0f)
        speed = 0.0f;
    if (speed < 0.5) {
        diff = steer;
        if (fabsf(&diff) < 0.02)
            steer = 0.0f;
    }
    sub = 0;
    if (broken & 0xC)
        sub = 10;
    limit = DEG((f32)(control->max_steer + zoid->turn_bonus - sub));
    if (limit < 0.0f)
        limit = 0.0f;
    zoid->field_1C84 = limit;
    if (steer > limit)
        steer = limit;
    if (steer < -limit)
        steer = -limit;
    if (0.0f == speed)
        steer = 0.0f;
    if (0.0f != steer && 0.0f == speed) {
        speed = 0.1f;
        if (zoid->field_1C90[1] < 0.1)
            zoid->field_1C90[1] = 0.1f;
    }
    zoid->field_1C90[0] = speed;
    speed = zoid->field_1C90[1];
    if (0.0f < speed && speed < 0.1)
        speed = 0.0f;
    if (-0.05 < speed && speed < 0.0f)
        speed = 0.0f;
    max = (zoid->field_1F04 - slow) / 100.0;
    if (speed > max && zoid->booster_on == 0)
        speed = max;
    if (speed > zoid->field_1F04 / 100.0)
        anim = 6;
    else if (speed > control->anim5_min_speed / 100.0)
        anim = 5;
    else if (speed >= 0.1)
        anim = 4;
    else if (0.0f == speed)
        anim = 1;
    else if (speed < 0.0f)
        anim = 7;
    if (zoid->booster_on == 0 && anim == 6)
        anim = 5;
    if (anim == 6 || anim == 5 || anim == 4) {
        if (anim != cur) {
            if (cur == 6 || cur == 5 || cur == 4) {
                if (zoid->field_1DF8 < 15) {
                    zoid->field_1DF8++;
                    anim = cur;
                } else {
                    zoid->field_1DF8 = 0;
                }
            } else {
                zoid->field_1DF8 = 120;
            }
        } else {
            zoid->field_1DF8++;
        }
    } else {
        zoid->field_1DF8++;
    }
    if (anim != 1)
        zoid->anim1_frames = 0;
    if (anim == 1 && zoid->anim1_frames > 600) {
        zoid->anim1_frames = 0;
        anim = 2;
    }
    if (zoid->current_animation == 2 && anim == 1)
        anim = 2;
    zoid->anim1_frames++;
    if (zoid->current_animation == 2 && zoid->animation_stopped != 0)
        anim = 1;
    if (anim != zoid->current_animation)
        ZoidSetAnim(zoid, anim);
    if (anim == 7)
        setAnimRate(zoid->joint, (f32)-control->anim7_rate * speed);
    if (anim == 4)
        setAnimRate(zoid->joint, (f32)control->anim4_rate * speed);
    if (anim == 5)
        setAnimRate(zoid->joint, 0.1 * control->anim5_rate * speed);
    if (anim == 6)
        setAnimRate(zoid->joint, 0.1 * control->anim6_rate * speed);
    zoid->steer = steer;
    if (zoid->field_1C90[2] > 0.0f)
        zoid->heading += 0.1 * zoid->steer;
    if (zoid->field_1C90[2] < 0.0f)
        zoid->heading -= 0.1 * zoid->steer;
}

#pragma push
#pragma auto_inline off
f32 ZoidCalcAngle(ZoidState *zoid, Vec3 *point)
{
    f32 dx;
    f32 dy;
    f32 dz;
    f32 theta;
    f32 angle;

    if (zoid == (void *)0) {
        return 100.0f;
    }
    if (zoid->joint == (void *)0) {
        return 100.0f;
    }
    dx = point->x - zoid->position[0];
    dy = point->y - zoid->position[1];
    dz = point->z - zoid->position[2];
    if (dx == 0.0f && dz == 0.0f) {
        return 100.0f;
    }
    theta = atan2f(dx, dz);
    angle = theta - zoid->heading;
    angLimit(&angle);
    return angle;
}
#pragma pop
#define GETPOS(z, v) \
    joint = 0; \
    if (joint == 0 && (z)->body_base_joint != 0) \
        joint = (z)->body_base_joint; \
    for (i = 0; i < 3; i++) { \
        if (joint != 0) \
            (v)[i] = joint->matrix[i][3]; \
        else \
            (v)[i] = (z)->position[i]; \
    }

void zdFace(ZoidState *zoid)
{
    f32 yaw;
    f32 pitch;
    f32 pos[3];
    f32 tpos[3];
    f32 angle;
    f32 dy;
    f32 dx;
    f32 dz;
    f32 dist;
    ZoidState *target;
    f32 limit;
    f32 yaw_step = 7.0f * (3.14159265f / 180.0f);
    f32 pitch_limit = 60.0f * (3.14159265f / 180.0f);
    f32 pitch_step = 0.2f * (3.14159265f / 180.0f);
    u8 *data;
    HSD_JObj *joint;
    s32 i;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    data = (u8 *)zoid->anim_data;
    limit = 2.0 * (3.1415926 * (f32)*(s32 *)(data + 0xcc)) / 360.0;
    if (zoid->face_lock != 0)
        limit = 0.0f;
    if (zoid->target_pending != 0) {
        target = &Zoid[zoid->target_slot];
        if (target->joint == (void *)0) {
            yaw = 0.0f;
            pitch = 0.0f;
        } else {
            GETPOS(zoid, pos)
            GETPOS(target, tpos)
            dz = tpos[2] - pos[2];
            dx = tpos[0] - pos[0];
            dy = tpos[1] - pos[1];
            dist = square_root(dx * dx + dz * dz);
            angle = atan2f(dx, dz);
            yaw = angle - zoid->heading;
            angLimit(&yaw);
            pitch = atan2f(-dy, dist);
            angLimit(&pitch);
        }
    } else {
        yaw = 0.0f;
        pitch = 0.0f;
    }
    if (yaw < 0.0f && yaw < -limit)
        yaw = -limit;
    if (yaw > 0.0f && yaw > limit)
        yaw = limit;
    if (pitch < 0.0f && pitch < -pitch_limit)
        pitch = -pitch_limit;
    if (pitch > 0.0f && pitch > pitch_limit)
        pitch = pitch_limit;
    angLimit(&zoid->face_yaw);
    angLimit(&zoid->face_pitch);
    if (zoid->face_yaw > yaw) {
        zoid->face_yaw -= yaw_step;
        if (zoid->face_yaw < yaw)
            zoid->face_yaw = yaw;
    }
    if (zoid->face_yaw < yaw) {
        zoid->face_yaw += yaw_step;
        if (zoid->face_yaw > yaw)
            zoid->face_yaw = yaw;
    }
    if (zoid->face_pitch > pitch) {
        zoid->face_pitch -= pitch_step;
        if (zoid->face_pitch < pitch)
            zoid->face_pitch = pitch;
    }
    if (zoid->face_pitch < pitch) {
        zoid->face_pitch += pitch_step;
        if (zoid->face_pitch > pitch)
            zoid->face_pitch = pitch;
    }
}

void ZoidSiseiFace(ZoidState *zoid)
{
    f32 saveA = zoid->face_yaw;
    f32 saveB = zoid->face_pitch;
    f32 yawA;
    f32 yawB;

    zdFace(zoid);
    yawA = zoid->face_yaw;
    if (zoid->buckle_joint != 0)
        HSD_JObjSetRotationY(zoid->buckle_joint, 8.0 * yawA / 53.0);
    if (zoid->field_1AD8 != 0)
        HSD_JObjSetRotationY(zoid->field_1AD8, 20.0 * yawA / 53.0);
    if (zoid->field_1ADC != 0)
        HSD_JObjSetRotationY(zoid->field_1ADC, 25.0 * yawA / 53.0);
    yawB = zoid->face_pitch;
    if (zoid->buckle_joint != 0)
        HSD_JObjSetRotationX(zoid->buckle_joint, 8.0 * yawB / 53.0);
    if (zoid->field_1AD8 != 0)
        HSD_JObjSetRotationX(zoid->field_1AD8, 20.0 * yawB / 53.0);
    if (zoid->field_1ADC != 0)
        HSD_JObjSetRotationX(zoid->field_1ADC, 25.0 * yawB / 53.0);
}
#undef lit_732
#define lit_732 "jobj.h"
#undef lit_733
#define lit_733 "jobj"
#undef lit_734
#define lit_734 "!(jobj->flags & JOBJ_USE_QUATERNION)"
#pragma push
#pragma auto_inline off
s32 ZoidSisei(ZoidState *zoid)
{
    f32 angle;

    if (zoid->field_1ACC != 0) {
        angle = zoid->senter_roll;
        HSD_JObjSetTranslateX(zoid->field_1ACC, zoid->senter_translation_y * sinf(angle));
        HSD_JObjSetTranslateY(zoid->field_1ACC, zoid->senter_translation_y * cosf(angle));
        HSD_JObjSetTranslateZ(zoid->field_1ACC, zoid->senter_translation_z - absf(zoid->senter_translation_y * sinf(angle)));
    }
    if (zoid->f_base_joint != 0) {
        HSD_JObjSetRotationY(zoid->f_base_joint, zoid->f_base_ry);
    }
    ZoidSiseiFace(zoid);
}
#pragma pop
void ZoidWalkJ(ZoidState *zoid)
{
    f32 dx;
    f32 dz;
    Vec3 position;
    f32 height;
    u16 selector;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        dx = zoid->displacement[0];
        dz = zoid->displacement[2];
        position.x = zoid->position[0] + dx;
        position.y = zoid->position[1];
        position.z = zoid->position[2] + dz;
        height = zdColPosY((f32 *)&position, &selector);
        zoid->ground_polygon = selector;
        if (selector == 0xFFFF) {
            zoid->heading += 3.0f;
        } else {
            zoid->position[0] += dx;
            zoid->position[2] += dz;
            ZoidSisei(zoid);
        }
    }
}
extern s32 zdFaceAtk(ZoidState *zoid, f32 distance, f32 offset);
extern u32 zdsMotTBL[];

void ZoidWalkSumuSpd(ZoidState *zoid)
{
    u8 *control;
    f32 maximum;
    s32 locked;
    u8 *slider;
    s32 reached;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        control = (u8 *)zoid->anim_data;
        if (control == (void *)0) {
            return;
        }
        slider = (u8 *)zoid->slider_info;
        if (slider == 0) {
            return;
        }
        zoid->field_1F04 = *(s32 *)(control + 0xBC) + zoid->speed_bonus;
        maximum = (f64)*(s32 *)(control + 0x9C) / 100.0;
        if (zoid->booster_on == 0) {
            maximum = (f64)zoid->field_1F04 / 100.0;
        }
        if (zoid->field_1C90[0] > maximum) {
            zoid->field_1C90[0] = maximum;
        }
        if (zoid->field_1C90[0] < (f64)-*(s32 *)(control + 0xAC) / 100.0) {
            zoid->field_1C90[0] = (f64)-*(s32 *)(control + 0xAC) / 100.0;
        }
        if (zoid->field_1C90[1] != zoid->field_1C90[0]) {
            if (zoid->field_1C90[1] > zoid->field_1C90[0]) {
                zoid->field_1C90[1] -= (f32)*(s32 *)(control + 0xD8) / 10000.0;
                if (zoid->field_1C90[1] < zoid->field_1C90[0]) {
                    zoid->field_1C90[1] = zoid->field_1C90[0];
                }
            }
            if (zoid->field_1C90[1] < zoid->field_1C90[0]) {
                zoid->field_1C90[1] += (f32)*(s32 *)(control + 0xD4) / 10000.0;
                if (zoid->field_1C90[1] > zoid->field_1C90[0]) {
                    zoid->field_1C90[1] = zoid->field_1C90[0];
                }
            }
        }
        locked = 0;
        if ((zdsMotTBL[zoid->current_animation] & 1) != 0) {
            locked = 1;
        }
        if (locked != 0) {
            if (zoid->animation_stopped != 0) {
                locked = 0;
            }
            if (zoid->current_animation == 0x42) {
                locked = 0;
            }
            if (zoid->current_animation == 0x41) {
                locked = 0;
            }
        }
        reached = 0;
        if (slider[0x70A] < 1) {
            slider[0x70A] = 1;
        }
        if (slider[0x70A] != 0) {
            if (zdFaceAtk(zoid, (f32)(s32)slider[0x70A], 0.0f) != 0) {
                reached = 1;
            }
        }
        if ((slider[0x70B] & 1) != 0 && *(u16 *)((u8 *)zoid + 0x51C) != 0xFFFF) {
            reached = 1;
        }
        if (reached != 0) {
            zoid->field_1C90[1] *= 0.5;
        }
        if (locked == 0) {
            ZoidWalkSpeed(zoid);
        }
    }
}
#pragma force_active on
static const f64 lit_848[1] = { 1.0471975333333332 };
#pragma force_active off
extern s32 ZoidSisei(ZoidState *zoid);
extern f64 cos(f64 x);

static inline f32 wsinf(f32 x)
{
    return sinf(x);
}

static inline f32 wcosf(f32 x)
{
    return cosf(x);
}

void ZoidWalkSusumu(ZoidState *zoid)
{
    f32 move;
    f32 dy;
    f32 next[3];
    f32 cur[3];
    f32 delta;
    u16 selector2;
    f32 speed;
    f32 vec[3];
    f32 length;
    f32 slope;
    f32 adjust[3];
    u8 *control;
    f32 target;
    f32 limit;
    f32 angle;
    f32 gap;
    f32 roll;
    f32 dx;
    f32 dz;
    f32 v;
    f32 step;
    s32 anim;
    u16 normal;
    s32 index;
    s32 blocked;
    s32 i;
    s32 j;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        control = (u8 *)zoid->anim_data;
        target = zoid->steer;
        if (zoid->f_base_joint != 0) {
            if (absf(zoid->f_base_ry - target) < DEG(5)) {
                zoid->f_base_ry = target;
            }
            if (zoid->f_base_ry < target) {
                zoid->f_base_ry += DEG(3);
                if (zoid->f_base_ry > target) {
                    zoid->f_base_ry = target;
                }
            }
            if (zoid->f_base_ry > target) {
                zoid->f_base_ry -= DEG(3);
                if (zoid->f_base_ry < target) {
                    zoid->f_base_ry = target;
                }
            }
            if (zoid->f_base_ry > DEG((f32) * (s32 *)(control + 0xC8))) {
                zoid->f_base_ry = DEG((f32) * (s32 *)(control + 0xC8));
            }
            if (zoid->f_base_ry < -DEG((f32) * (s32 *)(control + 0xC8))) {
                zoid->f_base_ry = DEG((f32) - *(s32 *)(control + 0xC8));
            }
        }
        if (zoid->field_1ACC != 0) {
            angle = zoid->senter_roll;
            if (zoid->current_animation == 5 || zoid->current_animation == 4) {
                limit = 10.0f * (zoid->steer * zoid->field_1C90[2]);
            } else {
                limit = 0.0f;
            }
            if (limit < DEG((f32) - *(s32 *)(control + 0x98))) {
                limit = DEG((f32) - *(s32 *)(control + 0x98));
            }
            if (limit > DEG((f32) * (s32 *)(control + 0x98))) {
                limit = DEG((f32) * (s32 *)(control + 0x98));
            }
            if (angle > limit) {
                angle -= DEG(1);
                if (angle < limit) {
                    angle = limit;
                }
            }
            if (angle < limit) {
                angle += DEG(1);
                if (angle > limit) {
                    angle = limit;
                }
            }
            zoid->senter_roll = angle;
        }
        ZoidSisei(zoid);
        if (zoid->b_base_joint != 0) {
            roll = zoid->ground_rz + zoid->senter_roll;
            zoid->field_1CE8 = roll;
            (void)-roll;
            speed = absf(roll) - DEG(45);
            if (speed > 0.0f) {
                zoid->slide_speed = roll > 0.0f ? speed : -speed;
            } else {
                zoid->slide_speed = 0.0f;
            }
        }
        if (zoid->field_1CF8 != 0) {
            zoid->field_1CF8--;
            zoid->field_1C90[0] *= 0.9;
            zoid->field_1C90[2] *= 0.9;
            zoid->field_1C90[1] *= 0.9;
        }
        for (i = 0; i < 3; i++) {
            cur[i] = zoid->position[i];
        }
        cur[1] = zdColPosY(cur, 0);
        next[0] = zoid->field_1C90[2] * wsinf(zoid->heading) + cur[0];
        next[1] = cur[1];
        next[2] = zoid->field_1C90[2] * wcosf(zoid->heading) + cur[2];
        next[1] = zdColPosY(next, &selector2);
        if (selector2 == GROUND_POLYGON_NONE) {
            zoid->heading += 3.0f;
            for (i = 0; i < 3; i++) {
                zoid->position[i] = zoid->field_1C24[i];
            }
            return;
        }
        for (i = 0; i < 3; i++) {
            vec[i] = next[i] - cur[i];
        }
        normal = zoid->ground_polygon;
        length = square_root(vec[0] * vec[0] + vec[2] * vec[2]);
        slope = vec[1];
        if (length != 0.0f) {
            blocked = 0;
            if (normal != GROUND_POLYGON_NONE) {
                index = grdcPolygon[normal].normal_index;
                if (slope > 0.0f && grdcNormal[index].y <= cos(lit_848[0])) {
                    blocked = 1;
                }
            }
            if (slope / length > 1.6) {
                blocked = 1;
            }
            if (blocked != 0) {
                zoid->field_1C90[2] = 0.0f;
                zoid->field_1C90[1] = 0.0f;
                for (i = 0; i < 3; i++) {
                    zoid->position[i] = zoid->field_1C24[i];
                }
                return;
            }
        }
        dx = next[0] - zoid->position[0];
        dy = next[1] - zoid->ground_height;
        (void)-dy;
        dz = next[2] - zoid->position[2];
        target = dx * dx + dy * dy + dz * dz;
        delta = zoid->field_1C90[1] * zoid->field_1C90[1];
        gap = delta - target;
        if (zoid->field_1C90[1] >= 0.0f && zoid->field_1C90[2] >= 0.0f) {
            if (gap > 0.0f) {
                zoid->field_1C90[2] += 0.05;
                if (zoid->field_1C90[2] > zoid->field_1C90[1]) {
                    zoid->field_1C90[2] = zoid->field_1C90[1];
                }
            }
            if (gap < 0.0f) {
                zoid->field_1C90[2] -= 0.05;
                if (zoid->field_1C90[2] < 0.0f) {
                    zoid->field_1C90[2] = 0.0f;
                }
                if (zoid->field_1C90[2] < zoid->field_1C90[1]) {
                    zoid->field_1C90[2] = zoid->field_1C90[1];
                }
            }
        } else if (zoid->field_1C90[1] >= 0.0f && zoid->field_1C90[2] < 0.0f) {
            zoid->field_1C90[2] += 0.05;
        } else if (zoid->field_1C90[1] < 0.0f && zoid->field_1C90[2] >= 0.0f) {
            zoid->field_1C90[2] -= 0.05;
        } else if (zoid->field_1C90[1] < 0.0f && zoid->field_1C90[2] < 0.0f) {
            if (gap > 0.0f) {
                zoid->field_1C90[2] -= 0.05;
                if (zoid->field_1C90[2] < zoid->field_1C90[1]) {
                    zoid->field_1C90[2] = zoid->field_1C90[1];
                }
            }
            if (gap < 0.0f) {
                zoid->field_1C90[2] += 0.05;
                if (zoid->field_1C90[2] > 0.0f) {
                    zoid->field_1C90[2] = 0.0f;
                }
                if (zoid->field_1C90[2] > zoid->field_1C90[1]) {
                    zoid->field_1C90[2] = zoid->field_1C90[1];
                }
            }
        }
        anim = zoid->current_animation;
        move = zoid->field_1C90[2];
        if (anim == 2 || anim == 1) {
            move = 0.0f;
        }
        dx = move * wsinf(zoid->heading);
        dz = move * wcosf(zoid->heading);
        if (zoid->ground_normal_count != 0) {
            step = 0.3f;
            for (j = 0; j < 3; j++) {
                v = zoid->ground_normal_sum[j] /= (f32)zoid->ground_normal_count;
                if (v > 0.0f) {
                    v -= step;
                    if (v < 0.0f) {
                        v = 0.0f;
                    }
                }
                if (v < 0.0f) {
                    v += step;
                    if (v > 0.0f) {
                        v = 0.0f;
                    }
                }
                adjust[j] = 0.1 * v;
            }
            dx += adjust[0];
            dz += adjust[2];
        }
        zoid->position[0] += dx;
        zoid->position[2] += dz;
    }
}
void ZoidSusumuV(ZoidState *zoid)
{
    s32 i;
    f32 length;
    f32 ground_y;
    f32 depth;
    u16 selector;
    void *control;
    f32 scaled[3];

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        control = zoid->anim_data;
        ZoidSisei(zoid);
        length = sLenPoint3dR(zoid->launch_velocity);
        if (length > 2.8) {
            for (i = 0; i < 3; i++) {
                scaled[i] = 2.8 * zoid->launch_velocity[i] / length;
            }
        } else {
            for (i = 0; i < 3; i++) {
                scaled[i] = zoid->launch_velocity[i];
            }
        }
        for (i = 0; i < 3; i++) {
            zoid->position[i] += scaled[i];
        }
        zoid->launch_velocity[1] -= 0.04;
        length = sLenPoint3dR(zoid->launch_velocity);
        if (length > 2.8) {
            for (i = 0; i < 3; i++) {
                zoid->launch_velocity[i] = 2.8 * zoid->launch_velocity[i] / length;
            }
        }
        ground_y = zdColPosY(zoid->position, &selector);
        depth = ground_y - zoid->position[1];
        if (depth > 0.0f) {
            zoid->height_offset_speed = 0.0f;
            zoid->height_offset -= depth;
            zoid->position[1] += depth;
        }
        zoid->ground_polygon = selector;
    }
}
void ZoidSusumuStepV(ZoidState *zoid)
{
    s32 i;
    f32 length;
    f32 ground_y;
    f32 depth;
    u16 selector;
    void *control;
    f32 scaled[3];

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        control = zoid->anim_data;
        ZoidSisei(zoid);
        length = sLenPoint3dR(zoid->launch_velocity);
        if (length > 1.8) {
            for (i = 0; i < 3; i++) {
                scaled[i] = 1.8 * zoid->launch_velocity[i] / length;
            }
        } else {
            for (i = 0; i < 3; i++) {
                scaled[i] = zoid->launch_velocity[i];
            }
        }
        for (i = 0; i < 3; i++) {
            zoid->position[i] += scaled[i];
        }
        zoid->launch_velocity[1] -= 0.04;
        length = sLenPoint3dR(zoid->launch_velocity);
        if (length > 1.8) {
            for (i = 0; i < 3; i++) {
                zoid->launch_velocity[i] = 1.8 * zoid->launch_velocity[i] / length;
            }
        }
        ground_y = zdColPosY(zoid->position, &selector);
        depth = ground_y - zoid->position[1];
        if (depth > 0.0f) {
            zoid->height_offset_speed = 0.0f;
            zoid->height_offset -= depth;
            zoid->position[1] += depth;
        }
        zoid->ground_polygon = selector;
    }
}
#pragma push
#pragma auto_inline off
void ZoidWalk(ZoidState *zoid)
{
    ZoidWalkSumuSpd(zoid);
    ZoidWalkSusumu(zoid);
}
#pragma pop
typedef struct ZoidControlSpan {
    u8 pad[0x98];
    s32 max_senter_roll;
} ZoidControlSpan;

void zdVaSet(ZoidState *zoid)
{
    ZoidControlSpan *control;
    f32 speed;
    f32 target;
    f32 current;
    f32 difference;
    f32 step = 2.0 * 3.1415926 * 2.0 / 360.0;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        speed = zoid->field_1C54;
        control = (ZoidControlSpan *)zoid->anim_data;
        if (speed > 1.0f) {
            speed = 1.0f;
        }
        difference = zoid->step_heading - zoid->heading;
        target = speed * sinf(difference);
        angLimit(&target);
        if (target < DEG((f32)-control->max_senter_roll)) {
            target = DEG((f32)-control->max_senter_roll);
        }
        if (target > DEG((f32)control->max_senter_roll)) {
            target = DEG((f32)control->max_senter_roll);
        }
        current = zoid->senter_roll;
        if (target > current) {
            current += step;
            if (target < current) {
                current = target;
            }
        }
        if (target < current) {
            current -= step;
            if (target > current) {
                current = target;
            }
        }
        zoid->senter_roll = current;
    }
}
typedef struct ZoidAIMode {
    s32 nearest_enemies[16];
    s32 mode;
} ZoidAIMode;

void zdJumpMove(ZoidState *zoid)
{
    f32 length;
    f32 t;
    f32 sine_angle;
    f32 cosine_angle;
    ZoidAIMode *ai;
    s32 i;

    length = sLenPoint3dR(zoid->velocity);
    if (length > 1.7) {
        for (i = 0; i < 3; i++) {
            zoid->velocity[i] *= 1.7 / length;
        }
    }
    length = sLenPoint3dR(zoid->displacement);
    if (length > 1.7) {
        for (i = 0; i < 3; i++) {
            zoid->displacement[i] *= 1.7 / length;
        }
    }
    zoid->velocity[1] -= 0.025;
    zoid->position[0] += zoid->velocity[0];
    zoid->position[1] += zoid->velocity[1];
    zoid->position[2] += zoid->velocity[2];
    ai = (ZoidAIMode *)zoid->ai;
    if (ai != 0 && ai->mode == 0) {
        ai = 0;
    }
    if (ai == 0) {
        t = zoid->field_1C54 / 200.0f;
        if (t > 1.0f) {
            t = 1.0f;
        }
        sine_angle = zoid->step_heading;
        zoid->position[0] += t * sinf(sine_angle);
        cosine_angle = zoid->step_heading;
        zoid->position[2] += t * cosf(cosine_angle);
    }
}
#pragma push
#pragma auto_inline off
void ZoidSuberi(ZoidState *zoid)
{
    f32 speed;
    f32 a;
    f32 b;
    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        zoid->slide_frames++;
        speed = zoid->slide_speed;
        if (0.0f == speed) {
            zoid->slide_frames = 0;
            return;
        }
        a = -zoid->heading;
        zoid->position[0] -= 1.5 * (zoid->slide_speed * cosf(a));
        b = -zoid->heading;
        zoid->position[2] -= 1.5 * (zoid->slide_speed * sinf(b));
    }
}
#pragma pop
extern void zdEffectMountTm(ZoidState *zoid, s32 mount, s32 effect, s32 tm);

#pragma push
#pragma auto_inline off
void zdAtackSetEffect(ZoidState *zoid, s32 attack)
{
    s32 i;
    s32 effect;
    s32 mount;
    s32 tm;

    tm = Tea_zoiddat[zoid->type].attack_tm[attack];
    for (i = 0; i < 2; i++) {
        effect = Tea_zoiddat[zoid->type].attack_effects[attack][i];
        mount = Tea_zoiddat[zoid->type].attack_effect_mounts[attack][i];
        if (effect > 0 && zoid->weapon_slots[mount].joint != (void *)0) {
            zdEffectMountTm(zoid, mount, effect, tm);
        }
    }
}
#pragma pop
typedef struct JumpControlData {
    u8 reserved_000[0xF4];
    s32 field_00F4;
    s32 field_00F8;
} JumpControlData;
extern void zdAtackSetEffect(ZoidState *zoid, s32 effect);
extern void zdVoiceScG(ZoidState *zoid, s32 voice);
extern void sEffectSet(s32 type, s32 mode, f32 *position, HSD_JObj *source, ZoidState *zoid);
extern void zdJumpMove(ZoidState *zoid);
extern void zdVaSet(ZoidState *zoid);
extern void ZoidWalkJ(ZoidState *zoid);
extern f64 cos(f64 x);

void ZoidJump(ZoidState *zoid)
{
    f32 vy;
    void *cam;
    f32 power[41] = {
        0.0f, 0.3f, 0.34f, 0.4f, 0.47f, 0.5f, 0.57f, 0.6f, 0.66f, 0.69f, 0.7f, 0.76f, 0.79f, 0.8f,
        0.831f, 0.865f, 0.9f, 0.93f, 0.96f, 0.985f, 1.0f, 1.03f, 1.06f, 1.08f, 1.1f, 1.12f, 1.14f,
        1.16f, 1.18f, 1.2f, 1.22f, 1.24f, 1.26f, 1.28f, 1.3f, 1.32f, 1.34f, 1.36f, 1.38f, 1.4f, 0.0f,
    };
    s32 state;
    JumpControlData *control;
    s32 anim;
    s32 mode;
    f32 turn;
    f32 dx;
    f32 push;
    f32 dz;
    Vec3 *normal;

    cam = zoid->camera;
    zoid->field_1C34 = 0;
    if (zoid->requested_action == 1 || zoid->requested_action == 2) {
        zoid->requested_action = 0;
    }
    state = zoid->jump_phase;
    vy = zoid->velocity[1];
    control = (JumpControlData *)zoid->anim_data;
    turn = zoid->field_1C90[0];
    if (state == 0x11 || (state == 0x21 && zoid->grounded == 0)) {
        if (zoid->requested_action == 0xd && zoid->field_1DB0 < 8) {
            zoid->requested_action = 0;
        }
        if (zoid->jump_attack_phase == 0 && zoid->requested_action == 0xd) {
            ZoidSetAnim(zoid, 0x41);
            zdAtackSetEffect(zoid, 5);
            zdVoiceScG(zoid, 0x12);
            zoid->requested_action = 0;
            zoid->jump_attack_phase = 1;
        }
        zoid->requested_action = 0;
    }
    if (zoid->jump_attack_phase == 1 && (state == 0x21 || state == 0x11) && zoid->animation_stopped != 0) {
        ZoidSetAnim(zoid, 0x42);
        zoid->requested_action = 0;
        zoid->jump_attack_phase = 2;
    }
    if (zoid->jump_attack_phase != 0) {
        zoid->field_1DA0 = 5;
    }
    switch (state) {
    case 1:
    case 2:
        zoid->field_1CF0 = ((HSD_JObj *)zoid->camera_joint)->matrix[1][3];
        if (control->field_00F8 != 1) {
            sEffectSet(0x3c, 1, 0, zoid->f_base_joint, 0);
        }
        zoid->jump_attack_phase = 0;
        zoid->velocity[0] = 0.0f;
        zoid->velocity[2] = 0.0f;
        zoid->velocity[1] = power[*(s32 *)((u8 *)control + ((state - 1) << 2) + 0xa0)];
        if (control->field_00F8 != -1 && zoid->ground_polygon != 0xffff) {
            normal = &grdcNormal[grdcPolygon[zoid->ground_polygon].normal_index];
            zoid->velocity[0] = 0.01 * normal->x;
            zoid->velocity[2] = 0.01 * normal->z;
            zoid->velocity[1] *= normal->y;
        }
        if (zoid->field_1ACC != 0) {
            push = 0.02 * zoid->senter_roll;
            dx = push * sin(DEG(90) + zoid->heading);
            dz = push * cos(DEG(90) + zoid->heading);
            zoid->velocity[0] += dx;
            zoid->velocity[2] += dz;
        }
        anim = 13;
        if (turn > 0.0f) {
            anim = 17;
        } else if (turn < 0.0f) {
            anim = 21;
        }
        ZoidSetAnim(zoid, anim);
        zoid->jump_phase = 3;
        break;
    case 3:
        if (zoid->animation_stopped != 0 || zoid->animation_looped != 0) {
            anim = 14;
            if (turn > 0.0f) {
                anim = 18;
            } else if (turn < 0.0f) {
                anim = 22;
            }
            zoid->jump_phase = 16;
            ZoidSetAnim(zoid, anim);
            zoid->jump_attack_phase = 0;
        }
        break;
    case 16:
        anim = 14;
        if (turn > 0.0f) {
            anim = 18;
        } else if (turn < 0.0f) {
            anim = 22;
        }
        if (zoid->current_animation != anim) {
            ZoidSetAnim(zoid, anim);
            zdLinierTm(zoid, 30);
        }
        zoid->jump_phase = 17;
        zoid->gene_frames = 0;
        break;
    case 17:
        zoid->gene_frames++;
        zdJumpMove(zoid);
        if (zoid->velocity[1] < 0.0f) {
            zoid->jump_phase = 32;
        }
        if (zoid->velocity[1] < 0.5) {
            zoid->jump_phase = 18;
        }
        break;
    case 18:
        zdJumpMove(zoid);
        if (zoid->velocity[1] < 0.0f) {
            zoid->jump_phase = 32;
        }
        if (zoid->jump_attack_phase == 0) {
            anim = 15;
            if (turn > 0.0f) {
                anim = 19;
            } else if (turn < 0.0f) {
                anim = 23;
            }
            if (zoid->current_animation != anim) {
                ZoidSetAnim(zoid, anim);
                zdLinierTm(zoid, 240);
            }
        }
        break;
    case 32:
        if (zoid->jump_attack_phase == 0) {
            anim = 15;
            if (turn > 0.0f) {
                anim = 19;
            } else if (turn < 0.0f) {
                anim = 23;
            }
            if (zoid->current_animation != anim) {
                ZoidSetAnim(zoid, anim);
                zdLinierTm(zoid, 240);
            }
        }
        zoid->jump_phase = 33;
    case 33:
        zdVaSet(zoid);
        zdJumpMove(zoid);
        mode = 0;
        if (control->field_00F4 != 0) {
            if (zoid->feet[0].joint != 0 && (zoid->feet[0].flags & 1)) {
                mode = 1;
            }
            if (zoid->feet[1].joint != 0 && (zoid->feet[1].flags & 1)) {
                mode = 1;
            }
            if (zoid->feet[2].joint != 0 && (zoid->feet[2].flags & 1)) {
                mode = 1;
            }
            if (zoid->feet[3].joint != 0 && (zoid->feet[3].flags & 1)) {
                mode = 1;
            }
            if (*(u32 *)(zoid->r_leg2_coobj + 0) != 0 && (*(u32 *)(zoid->r_leg2_coobj + 0x14) != 0 || *(u32 *)(zoid->r_leg2_coobj + 0x18) != 0)) {
                mode = 1;
            }
            if (*(u32 *)(zoid->r_leg1_coobj + 0) != 0 && (*(u32 *)(zoid->r_leg1_coobj + 0x14) != 0 || *(u32 *)(zoid->r_leg1_coobj + 0x18) != 0)) {
                mode = 1;
            }
            if (*(u32 *)(zoid->l_leg2_coobj + 0) != 0 && (*(u32 *)(zoid->l_leg2_coobj + 0x14) != 0 || *(u32 *)(zoid->l_leg2_coobj + 0x18) != 0)) {
                mode = 1;
            }
            if (*(u32 *)(zoid->l_leg1_coobj + 0) != 0 && (*(u32 *)(zoid->l_leg1_coobj + 0x14) != 0 || *(u32 *)(zoid->l_leg1_coobj + 0x18) != 0)) {
                mode = 1;
            }
        } else if (zoid->foot_contacts != 0 || zoid->grounded != 0) {
            mode = 1;
        }
        if (zoid->displacement[1] > 0.0f) {
            mode = 1;
        }
        if (*(u32 *)(zoid->bust_coobj + 0x18) != 0) {
            mode = 1;
        }
        if (control->field_00F8 == 1) {
            if (zoid->field_1CF0 >= ((HSD_JObj *)zoid->camera_joint)->matrix[1][3]) {
                mode = 1;
            }
            if (zoid->position[1] <= zoid->ground_height) {
                mode = 1;
            }
        }
        if (mode != 0) {
            zoid->jump_phase = 48;
            zoid->field_1DD4 = 60;
        }
        break;
    case 48:
        zoid->jump_attack_phase = 0;
        if (zoid->velocity[1] > -1.2) {
            zoid->action = 0;
            zoid->jump_phase = 0;
            if (zoid->velocity[1] < -0.8 && control->field_00F8 != 1) {
                sEffectSet(0x3c, 1, 0, zoid->f_base_joint, 0);
            }
            ZoidSetAnim(zoid, 1);
        } else {
            anim = 16;
            if (turn > 0.0f) {
                anim = 20;
            } else if (turn < 0.0f) {
                anim = 24;
            }
            if (control->field_00F8 != -1) {
                sEffectSet(0x3c, 1, 0, zoid->f_base_joint, 0);
            }
            ZoidSetAnim(zoid, anim);
            zoid->jump_phase = 49;
        }
        break;
    case 49:
        zoid->field_1BFC = 0;
        if (zoid->jump_attack_phase != 0) {
            zoid->feet[0].no_contact_frames = 0;
            zoid->feet[1].no_contact_frames = 0;
        }
        zoid->jump_attack_phase = 0;
        if (zoid->animation_stopped != 0 || zoid->animation_looped != 0) {
            zoid->jump_phase = 0;
            zoid->action = 0;
            ZoidSetAnim(zoid, 1);
        }
        break;
    }
    mode = 2;
    if (zoid->jump_phase == 1) {
        mode = 1;
    }
    if (zoid->jump_phase == 48) {
        mode = 0;
    }
    if (zoid->jump_phase == 49) {
        mode = 0;
    }
    if (mode == 2) {
        ZoidWalkJ(zoid);
    }
    if (mode == 1) {
        ZoidWalk(zoid);
    }
}
#pragma force_active on
static const u32 lit_1601[5] = {
    0x00000043, 0x00000045, 0x00000047, 0x00000049, 0x0000004B, 
};
static const u32 lit_1602[5] = {
    0x00000044, 0x00000046, 0x00000048, 0x0000004A, 0x0000004C, 
};
static const char lit_1849[14] = "float cancel\n";
#pragma force_active off
extern void HSD_JObjAnimAll(HSD_JObj *joint);

void ZoidTurnR(ZoidState *zoid, s32 mode)
{
    u8 *control;
    f32 turn;
    f32 speed;
    f32 diff;

    control = (u8 *)zoid->field_2090;
    if (control == 0) {
        turn = 90.0f;
    } else {
        turn = (f32) * (s32 *)(control + 0x154);
    }
    if (turn <= 0.0f) {
        zoid->action = 0;
        return;
    }
    speed = absf(zoid->field_1C54);
    switch (zoid->action_phase) {
    case 0:
        ZoidSetAnim(zoid, 12);
        zoid->action_phase = 1;
        break;
    case 1:
        if (mode == 0 && speed < 0.04) {
            zoid->animation_stopped = 1;
        }
        if (mode == 0 && zoid->target_pending != 0 && zoid->ai_active == 0) {
            diff = zoid->step_heading - zoid->heading;
            angLimit(&diff);
            if (diff >= 0.0f) {
                zoid->animation_stopped = 1;
            }
        }
        if (zoid->ai_active == 0 && zoid->target_pending == 0 && zoid->field_1C74 < 0.0f) {
            zoid->animation_stopped = 1;
        }
        if (mode == 2 && zoid->target_pending != 0 && zoid->target_slot < 16 &&
            absf(zoid->target_angles[zoid->target_slot]) < DEG(5)) {
            zoid->animation_stopped = 1;
        }
        if (mode == 3) {
            diff = zoid->field_1C58 - zoid->heading;
            angLimit(&diff);
            if (diff >= 0.0f) {
                zdChgNormal(zoid);
                zoid->requested_action = 7;
                break;
            }
        }
        if (zoid->animation_stopped != 0 || zoid->animation_looped != 0) {
            zoid->action = 0;
            *(f32 *)(*(u8 **)((u8 *)zoid->pose + 4) + 0x10) = 0.0f;
            ZoidSetAnim(zoid, 1);
            HSD_JObjAnimAll(zoid->joint);
        } else {
            zoid->heading -= DEG(turn / 20.0f);
        }
        if (mode == 3) {
            diff = zoid->field_1C58 - zoid->heading;
            angLimit(&diff);
            if (diff >= 0.0f) {
                zoid->heading = zoid->field_1C58;
                zdChgNormal(zoid);
            }
        }
        break;
    }
}
extern void HSD_JObjAnimAll(HSD_JObj *joint);

void ZoidTurnL(ZoidState *zoid, s32 mode)
{
    u8 *control;
    f32 turn;
    f32 speed;
    f32 diff;

    control = (u8 *)zoid->field_2090;
    if (control == 0) {
        turn = 90.0f;
    } else {
        turn = (f32) * (s32 *)(control + 0x154);
    }
    if (turn <= 0.0f) {
        zoid->action = 0;
        return;
    }
    speed = absf(zoid->field_1C54);
    switch (zoid->action_phase) {
    case 0:
        ZoidSetAnim(zoid, 11);
        zoid->action_phase = 1;
        break;
    case 1:
        if (mode == 0 && speed < 0.04) {
            zoid->animation_stopped = 1;
        }
        if (mode == 0 && zoid->target_pending != 0 && zoid->ai_active == 0) {
            diff = zoid->step_heading - zoid->heading;
            angLimit(&diff);
            if (diff <= 0.0f) {
                zoid->animation_stopped = 1;
            }
        }
        if (zoid->ai_active == 0 && zoid->target_pending == 0 && zoid->field_1C74 > 0.0f) {
            zoid->animation_stopped = 1;
        }
        if (mode == 2 && zoid->target_pending != 0 && zoid->target_slot < 16 &&
            absf(zoid->target_angles[zoid->target_slot]) < DEG(5)) {
            zoid->animation_stopped = 1;
        }
        if (mode == 3) {
            diff = zoid->field_1C58 - zoid->heading;
            angLimit(&diff);
            if (diff <= 0.0f) {
                zdChgNormal(zoid);
                zoid->requested_action = 6;
                break;
            }
        }
        if (zoid->animation_stopped != 0 || zoid->animation_looped != 0) {
            zoid->action = 0;
            *(f32 *)(*(u8 **)((u8 *)zoid->pose + 4) + 0x10) = 0.0f;
            ZoidSetAnim(zoid, 1);
            HSD_JObjAnimAll(zoid->joint);
        } else {
            zoid->heading += DEG(turn / 20.0f);
        }
        if (mode == 3) {
            diff = zoid->field_1C58 - zoid->heading;
            angLimit(&diff);
            if (diff <= 0.0f) {
                zoid->heading = zoid->field_1C58;
                zdChgNormal(zoid);
            }
        }
        break;
    }
}
extern void HSD_JObjAnimAll(HSD_JObj *joint);

void ZoidStop1(ZoidState *zoid)
{
    switch (zoid->action_phase) {
    case 0:
        ZoidSetAnim(zoid, 9);
        zoid->action_phase = 1;
        break;
    default:
        if (zoid->animation_stopped != 0 || zoid->animation_looped != 0) {
            zoid->action = 0;
            zoid->requested_action = 0;
            ZoidSetAnim(zoid, 1);
            HSD_JObjAnimAll(zoid->joint);
            zoid->field_1C90[2] = 0.0f;
            zoid->field_1C90[0] = 0.0f;
            zoid->field_1C90[1] = 0.0f;
            zoid->step_heading = zoid->heading;
            break;
        }
        zoid->action_phase++;
        ZoidWalkSusumu(zoid);
        if (zoid->action_phase < 30) {
            zoid->field_1C90[0] *= 0.9;
            zoid->field_1C90[0] -= 0.01;
            if (zoid->field_1C90[0] < 0.0f) {
                zoid->field_1C90[0] = 0.0f;
            }
            zoid->field_1C90[2] *= 0.9;
            zoid->field_1C90[2] -= 0.01;
            if (zoid->field_1C90[2] < 0.0f) {
                zoid->field_1C90[2] = 0.0f;
            }
        } else {
            zoid->field_1C90[0] *= 0.7;
            zoid->field_1C90[0] -= 0.02;
            if (zoid->field_1C90[0] < 0.0f) {
                zoid->field_1C90[0] = 0.0f;
            }
            zoid->field_1C90[2] *= 0.7;
            zoid->field_1C90[2] -= 0.02;
            if (zoid->field_1C90[2] < 0.0f) {
                zoid->field_1C90[2] = 0.0f;
            }
        }
        if (zoid->action_phase > 57) {
            zoid->field_1C90[2] = 0.0f;
            zoid->field_1C90[0] = 0.0f;
            zoid->field_1C90[1] = 0.0f;
        }
    }
}

void ZoidDash1(ZoidState *zoid)
{
    switch (zoid->action_phase) {
    case 0:
        ZoidSetAnim(zoid, 8);
        zoid->action_phase = 1;
        break;
    default:
        if (zoid->animation_stopped != 0 || zoid->animation_looped != 0) {
            zoid->action = 0;
            zoid->requested_action = 0;
            ZoidSetAnim(zoid, 1);
            HSD_JObjAnimAll(zoid->joint);
        } else {
            zoid->action_phase++;
            ZoidWalkSusumu(zoid);
            if (zoid->action_phase < 30) {
                zoid->field_1C90[0] *= 1.1;
                zoid->field_1C90[0] += 0.01;
                zoid->field_1C90[2] *= 1.1;
                zoid->field_1C90[2] += 0.01;
            } else {
                zoid->field_1C90[0] *= 1.7;
                zoid->field_1C90[2] *= 1.7;
            }
        }
        break;
    }
}
#pragma force_active on
static const f32 lit_1494[1] = { -1.0f };
static const f32 lit_1519[1] = { 0.026179938f };
static const f32 lit_1520[1] = { 0.5235988f };
static const f32 lit_1521[1] = { 15.0f };
static const f64 lit_1572[1] = { 10.0 };
static const f32 lit_1651[1] = { 3.761582e-37f };
static const f32 lit_1845[1] = { 0.08726646f };
static const f32 lit_1846[1] = { 5.0f };
static const f32 lit_1847[1] = { 2.0f };
static const f32 lit_1848[1] = { 10000.0f };
static const f32 lit_1850[1] = { 0.9f };
#pragma force_active off
void stepMove(ZoidState *zoid)
{
    ZoidAIMode *ai;
    union {
        f32 f;
        u32 u;
    } rate;
    union {
        f32 f;
        u32 u;
    } mag;
    f32 limit;
    f32 step;

    ai = (ZoidAIMode *)zoid->ai;
    if (ai != 0 && ai->mode == 0) {
        ai = 0;
    }
    if (ai != 0) {
        return;
    }
    rate.f = zoid->field_1C74;
    rate.u &= 0x7FFFFFFF;
    if (rate.f < 0.6) {
        zoid->field_1DBC &= 0xFFFFFFFE;
    }
    if (zoid->field_1DBC & 1) {
        return;
    }
    if (zoid->action_phase == 0) {
        return;
    }
    step = (f32)(4.0 * -(2.0 * (3.1415926 * zoid->field_1C74) / 360.0));
    limit = (f32)(0.1 * zoid->field_1C84);
    mag.f = step;
    mag.u &= 0x7FFFFFFF;
    if (mag.f > limit) {
        if (step > 0.0f) {
            step = limit;
        } else {
            step = -limit;
        }
    }
    zoid->heading += step;
    angLimit(&zoid->heading);
}
#include "game_mode.h"
typedef struct StepControlData {
    u8 reserved_000[0xF8];
    s32 field_00F8;
} StepControlData;
extern void ZoidSusumuStepV(ZoidState *zoid);
extern void stepMove(ZoidState *zoid);
extern s32 zdCol(ZoidState *zoid, f32 *vec, f32 distance);
extern void zdVoiceScG(ZoidState *zoid, s32 voice);

void ZoidActStep(ZoidState *zoid)
{
    s32 busy;
    u8 *slider;
    s32 voice;
    s32 below;
    s32 tblA[4] = { 25, 29, 33, 37 };
    s32 tblB[4] = { 26, 30, 34, 38 };
    s32 tblC[4] = { 27, 31, 35, 39 };
    s32 tblD[4] = { 28, 32, 36, 40 };
    StepControlData *control;

    busy = 0;
    slider = (u8 *)zoid->slider_info;
    control = (StepControlData *)zoid->anim_data;
    if (slider != 0) {
        if (zoid->requested_action == 0x17)
            busy = 1;
        if (zoid->requested_action == 0x18)
            busy = 1;
        if (zoid->requested_action == 0x19)
            busy = 1;
        if (zoid->requested_action == 0x1a)
            busy = 1;
        if (zoid->requested_action == 0x2d)
            busy = 1;
        if (zoid->requested_action == 0x2e)
            busy = 1;
        if (zoid->requested_action == 0x2f)
            busy = 1;
        if (zoid->requested_action == 0x30)
            busy = 1;
        if (busy != 0)
            zoid->requested_action = 0;
        if (zoid->action_phase >= 0x10 && zoid->action_phase < 0x30)
            ZoidSusumuStepV(zoid);
        switch (zoid->action_phase) {
        case 0:
            busy = 0;
            zoid->field_1DBC |= 1;
            zoid->field_1CF0 = ((HSD_JObj *)zoid->camera_joint)->matrix[1][3];
            if (slider[0x70c] == 0) {
                if (control->field_00F8 == 1) {
                    if (zdCol(zoid, zoid->launch_velocity, 10.0f) != 0)
                        busy = 1;
                }
            } else {
                if (zdCol(zoid, zoid->launch_velocity, (f32)slider[0x70c]) != 0)
                    busy = 1;
            }
            if (busy != 0) {
                zoid->action_phase = 0x99;
                break;
            }
            ZoidSetAnim(zoid, tblA[zoid->action - 0x17]);
            zoid->action_phase = 1;
            voice = 1;
            if (GameMode.game_type == 1 || GameMode.game_type == 4) {
                if (zoid->slot != Zoid[0].slot)
                    voice = 0;
                if (zoid == &Zoid[2])
                    voice = 0;
            }
            if (voice != 0)
                zdVoiceScG(zoid, 0x1c);
            break;
        case 1:
            if (zoid->animation_stopped == 0 && zoid->animation_looped == 0)
                break;
        case 0x10:
            ZoidSetAnim(zoid, tblB[zoid->action - 0x17]);
            zoid->action_phase = 0x11;
            break;
        case 0x11:
            stepMove(zoid);
            if (*(void **)(zoid->bust_coobj + 0x18) != 0)
                zoid->launch_velocity[1] = 0.0f;
            if (*(void **)(zoid->hip_coobj + 0x18) != 0)
                zoid->launch_velocity[1] = 0.0f;
            if (*(void **)(zoid->neck_coobj + 0x18) != 0)
                zoid->launch_velocity[1] = 0.0f;
            if (zoid->launch_velocity[1] > 0.0f)
                break;
        case 0x20:
            ZoidSetAnim(zoid, tblC[zoid->action - 0x17]);
            zoid->action_phase = 0x21;
            break;
        case 0x21:
            if (zoid->action == 0x18) {
                zoid->field_1C90[2] = 0.0f;
                zoid->field_1C90[1] = 0.0f;
                zoid->field_1C90[0] = 0.0f;
            }
            stepMove(zoid);
            if (control->field_00F8 == 1) {
                below = 0;
                if (zoid->field_1CF0 >= ((HSD_JObj *)zoid->camera_joint)->matrix[1][3])
                    below = 1;
                if (zoid->position[1] <= zdColPosY(zoid->position, 0))
                    below = 1;
                if (below != 0)
                    zoid->action_phase = 0x30;
                break;
            }
            if (zoid->ground_normal_count == 0 && zoid->grounded == 0)
                break;
            zoid->grounded_frames = 0;
            zoid->action_phase = 0x30;
        case 0x22:
            zoid->grounded_frames = zoid->grounded_frames + 1;
            if (zoid->grounded_frames < 6)
                break;
        case 0x30:
            ZoidSetAnim(zoid, tblD[zoid->action - 0x17]);
            zoid->action_phase = 0x31;
            break;
        case 0x31:
            if (zoid->animation_stopped == 0 && zoid->animation_looped == 0)
                break;
        default:
            zdChgNormal(zoid);
            break;
        }
        zoid->field_1C74 = 0.0f;
    }
}
extern void PartnerFace(s32 face, s32 frames);

void ZoidActDmg2(ZoidState *zoid)
{
    s32 motions[6] = {44, 45, 46, 47, 48, 49};
    register s32 i;
    u8 *anim;

    anim = (u8 *)zoid->anim_data;
    if (anim != 0 && *(s32 *)(anim + 0x100) != 0) {
        zoid->field_1E50 = 60;
        for (i = 0; i < 3; i++) {
            zoid->knockback[i] = 0.0f;
            zoid->field_1D20[i] = 0.0f;
        }
        zoid->field_1E5C = 0;
    }
    zoid->gene_frames += 1;
    switch (zoid->action_phase) {
    case 0:
        zoid->gene_frames = 0;
        ZoidSetAnim(zoid, motions[zoid->action - 0x1C]);
        zoid->action_phase = 1;
        if (Zoid[2].joint != 0 && zoid == &Zoid[2] && zoid->slot == 0) {
            PartnerFace(4, 60);
        }
        for (i = 0; i < 3; i++) {
            zoid->field_1D20[i] = 0.0f;
        }
        break;
    case 1:
        if (zoid->gene_frames > 120) {
            zoid->action_phase = 0x99;
        }
        if (zoid->animation_stopped == 0 && zoid->animation_looped == 0) {
            break;
        }
    default:
        zoid->action = 0;
        zdChgNormal(zoid);
        zdLinierTm(zoid, 60);
        break;
    }
}
typedef struct ZoidCtl3 {
    u8 pad[0x158];
    s32 f158;
    s32 f15c;
    s32 f160;
} ZoidCtl3;

extern void ZoidSusumuV(ZoidState *zoid);
extern void zdSetXYZ(ZoidState *zoid);
extern void zdAtackSetEffect(ZoidState *zoid, s32 effect);
extern void zdVoiceScG(ZoidState *zoid, s32 voice);
extern void HSD_JObjAnimAll(HSD_JObj *joint);

void ZoidActDmg3(ZoidState *zoid)
{
    s32 hit[6] = {50, 51, 52, 53, 55, 54};
    s32 i;
    s32 j;
    s32 k;
    u32 *c;
    ZoidCtl3 *control;
    ZoidAnim3 *anim;
    u8 *slider;

    control = (ZoidCtl3 *)zoid->field_2090;
    anim = (ZoidAnim3 *)zoid->anim_data;
    slider = (u8 *)zoid->slider_info;
    if (control == 0) {
        return;
    }
    if (slider == 0) {
        return;
    }
    c = (u32 *)zoid->bust_coobj;
    if (*c == 0) {
        c = (u32 *)zoid->neck_coobj;
    }
    if (*c == 0) {
        c = (u32 *)zoid->hip_coobj;
    }
    if (*c) {
        (void)0;
    }
    zoid->displacement[0] = 0.0f;
    zoid->displacement[1] = 0.0f;
    zoid->displacement[2] = 0.0f;
    if (zoid->requested_action == 27) {
        zoid->requested_action = 0;
    }
    if (zoid->requested_action == 34) {
        zoid->requested_action = 0;
    }
    zoid->death_pending = 0;
    if (zoid->action_phase > 0 && zoid->action_phase <= 48) {
        ZoidSusumuV(zoid);
        zoid->field_1BFC = 0;
    }
    if (zoid->field_1E50 < 2) {
        zoid->field_1E50 = 2;
    }
    switch (zoid->action_phase) {
    case 0:
        ZoidSetAnim(zoid, hit[zoid->action - 35]);
        zoid->interpolation_remaining = 15;
        zoid->interpolation_duration = 15;
        zoid->action_phase = 1;
        zoid->field_1E50 = 30;
        for (i = 0; i < 3; i++) {
            zoid->knockback[i] = 0.0f;
            zoid->field_1D20[i] = 0.0f;
        }
        zoid->gene_frames = 0;
    case 1:
        if (slider[0x70b] & 4) {
            zoid->height_offset = 0.0f;
        }
        zoid->death_pending = 0;
        zoid->field_1C34 = 0;
        if (zoid->animation_looped == 0 && zoid->animation_stopped == 0) {
            break;
        }
    case 16:
        ZoidSetAnim(zoid, 56);
        zoid->action_phase = 17;
        zoid->position[1] = zoid->position[1] + lit_1572[0] * (0.4 * ((f32)control->f158 / 100.0));
        zoid->grounded = 0;
        if (control->f158 == 0) {
            zoid->action_phase = 32;
        }
        zdSetXYZ(zoid);
        break;
    case 17:
        zoid->gene_frames++;
        if (zoid->gene_frames > 600) {
            zoid->action_phase = 32;
        }
        if (zoid->grounded == 0) {
            break;
        }
    case 32:
        for (j = 0; j < 3; j++) {
            zoid->launch_velocity[j] *= 0.7;
        }
        zoid->gene_frames = 0;
        zoid->launch_velocity[1] *= 0.3;
        zoid->action_phase = 33;
    case 33:
        zoid->launch_velocity[1] *= 0.3;
        zoid->field_1C34 = 0;
        for (k = 0; k < 3; k++) {
            zoid->launch_velocity[k] *= 0.9;
        }
        if (zoid->animation_stopped == 0 && zoid->animation_looped == 0) {
            break;
        }
        zoid->launch_velocity[1] *= 0.3;
    case 48:
        zoid->field_1C34 = 0;
        zoid->action_phase = 49;
        zoid->field_1DBC = 0;
        zoid->launch_velocity[1] *= 0.3;
    case 49:
        zoid->death_pending = 1;
        if (zoid->health <= 0) {
            zoid->health = 0;
            zoid->launch_velocity[1] = 0.0f;
            zoid->action = 42;
            zoid->action_phase = 0;
            zoid->requested_action = 42;
            break;
        }
        if ((s32)zoid->field_1DBC >= control->f15c) {
            if (zoid->requested_action == 13) {
                zdAtackSetEffect(zoid, 7);
                zoid->action_phase = 80;
                zdVoiceScG(zoid, 5);
                break;
            } else if (zoid->requested_action == 2) {
                if (zoid->field_1E50 < 60) {
                    zoid->field_1E50 = 60;
                }
                zoid->action_phase = 96;
                break;
            }
        }
        zoid->field_1DBC++;
        if ((s32)zoid->field_1DBC > control->f160) {
            zoid->action_phase = 64;
        }
        break;
    case 64:
        zoid->field_1C34 = 0;
        ZoidSetAnim(zoid, 57);
        zoid->action_phase = 65;
    case 65:
        zoid->field_1C34 = 0;
        if (zoid->field_1E50 < 60) {
            zoid->field_1E50 = 60;
        }
        if (zoid->animation_stopped == 0 && zoid->animation_looped == 0) {
            break;
        }
    default:
    case 153:
        zoid->launch_velocity[0] = 0.0f;
        zoid->launch_velocity[1] = 0.0f;
        zoid->launch_velocity[2] = 0.0f;
        zoid->displacement[0] = 0.0f;
        zoid->displacement[1] = 0.0f;
        zoid->displacement[2] = 0.0f;
        zoid->knockback[0] = 0.0f;
        zoid->knockback[1] = 0.0f;
        zoid->knockback[2] = 0.0f;
        zoid->field_1D20[0] = 0.0f;
        zoid->field_1D20[1] = 0.0f;
        zoid->field_1D20[2] = 0.0f;
        zoid->action = 0;
        zoid->requested_action = 0;
        ZoidSetAnim(zoid, 1);
        HSD_JObjAnimAll(zoid->joint);
        break;
    case 80:
        zoid->requested_action = 0;
        ZoidSetAnim(zoid, 62);
        zoid->action_phase = 81;
    case 81:
        if (zoid->animation_stopped == 0 && zoid->animation_looped == 0) {
            break;
        }
        zoid->action_phase = 153;
        break;
    case 96:
        zoid->requested_action = 0;
        ZoidSetAnim(zoid, 58);
        zoid->action_phase = 97;
    case 97:
        if (zoid->animation_stopped == 0 && zoid->animation_looped == 0) {
            break;
        }
        ZoidSetAnim(zoid, 59);
        zoid->launch_velocity[1] = 0.55f;
        zoid->launch_velocity[0] = 0.0f;
        zoid->launch_velocity[2] = 0.0f;
        if (zoid->ground_normal) {
            zoid->launch_velocity[0] = zoid->ground_normal->x;
            zoid->launch_velocity[2] = zoid->ground_normal->z;
        }
        zoid->action_phase = 98;
        zoid->gene_frames = 0;
    case 98:
        ZoidSusumuV(zoid);
        if (zoid->launch_velocity[1] > 0.0f) {
            break;
        }
        ZoidSetAnim(zoid, 60);
        zoid->action_phase = 99;
    case 99:
        zoid->gene_frames++;
        if (zoid->gene_frames > 300) {
            zoid->action_phase = 100;
        }
        if (anim->f0F8 == 1 && zoid->ground_height >= zoid->position[1]) {
            zoid->action_phase = 100;
            break;
        }
        ZoidSusumuV(zoid);
        if (zoid->grounded == 0 && zoid->ground_normal_count == 0) {
            break;
        }
    case 100:
        zoid->launch_velocity[0] = 0.0f;
        zoid->launch_velocity[1] = 0.0f;
        zoid->launch_velocity[2] = 0.0f;
        zoid->displacement[0] = 0.0f;
        zoid->displacement[1] = 0.0f;
        zoid->displacement[2] = 0.0f;
        ZoidSetAnim(zoid, 61);
        zoid->action_phase = 101;
    case 101:
        zoid->launch_velocity[0] = 0.0f;
        zoid->launch_velocity[1] = 0.0f;
        zoid->launch_velocity[2] = 0.0f;
        zoid->displacement[0] = 0.0f;
        zoid->displacement[1] = 0.0f;
        zoid->displacement[2] = 0.0f;
        if (zoid->animation_stopped != 0 || zoid->animation_looped != 0) {
            zoid->action_phase = 153;
        }
        break;
    }
}

void ZoidActDeadly(ZoidState *zoid)
{
    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        switch (zoid->action_phase) {
        case 0:
            ZoidSetAnim(zoid, 0x34);
            zoid->action_phase = 1;
            break;
        case 1:
            if (zoid->animation_stopped != 0 || zoid->animation_looped != 0) {
                ZoidSetAnim(zoid, 0x38);
                zoid->action_phase = 2;
            } else {
                break;
            }
        case 2:
            if (zoid->animation_stopped != 0 || zoid->animation_looped != 0) {
                ZoidSetAnim(zoid, 0x38);
                zoid->action_phase = 3;
            } else {
                break;
            }
        case 3:
            if (zoid->animation_stopped != 0 || zoid->animation_looped != 0) {
                ZoidSetAnim(zoid, 0x38);
                zoid->action_phase = 4;
            } else {
                break;
            }
        case 4:
            setAnimRate(zoid->joint, 0.0f);
            zoid->death_pending = 1;
            break;
        }
    }
}
extern void sEffectSet(s32 type, s32 mode, f32 *position, HSD_JObj *source, ZoidState *zoid);
extern void PartnerFace(s32 face, s32 frames);
extern s32 sCamNN;

void ZoidActDead(ZoidState *zoid)
{
    HSD_JObj *joint;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        zoid->lock_frames = 0;
        zoid->death_pending = 1;
        if (zoid->death_started == 0) {
            zoid->death_started = 1;
            joint = zoid->buckle_joint;
            if (joint == 0) {
                joint = zoid->bust_base_joint;
            }
            if (joint == 0) {
                joint = zoid->joint;
            }
            sEffectSet(105, 2, 0, joint, zoid);
            sEffectSet(47, 3, 0, joint, zoid);
            if (zoid == &Zoid[2] && sCamNN == 1 && Zoid[2].joint != 0 &&
                Zoid[0].slot == Zoid[2].slot) {
                PartnerFace(7, 120);
            }
        }
        zoid->action = 0x2B;
    }
}
extern void sMakeMatrix(HSD_JObj *joint);
extern void zdFootBak(ZoidState *zoid);
extern void ZoidCopyAnim(ZoidState *zoid);
extern void zdCoStartPos(ZoidState *zoid);

void zdGenePos(ZoidState *zoid)
{
    s32 i;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        zoid->position[0] = (f32)zoid->gene_x;
        zoid->position[2] = (f32)zoid->gene_z;
        zoid->position[1] = 100.0f;
        zoid->position[1] = zdColPosY(zoid->position, &zoid->ground_polygon) + 10.0f;
        for (i = 0; i < 3; i++) {
            zoid->field_1A9C[i] = zoid->position[i];
            zoid->field_1C24[i] = zoid->position[i];
            zoid->field_1A80[i] = zoid->position[i];
            zoid->displacement[i] = 0.0f;
            zoid->knockback[i] = 0.0f;
            zoid->field_1D20[i] = 0.0f;
            if (zoid->field_1A7C != 0) {
                zoid->field_1A7C->matrix[i][3] = zoid->position[i];
            }
        }
        zdSetXYZ(zoid);
        sMakeMatrix(zoid->joint);
        zdFootBak(zoid);
        ZoidCopyAnim(zoid);
        zdCoStartPos(zoid);
    }
}
#pragma push
#pragma auto_inline off
void zdModeGene(ZoidState *zoid)
{
    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        zoid->gene_frames++;
        zdGenePos(zoid);
        if (zoid->gene_frames > 15) {
            zoid->action = 0;
            zoid->requested_action = 2;
            zoid->death_pending = 0;
            zoid->position[1] = 100.0f;
            zoid->position[1] = 1.0f + zdColPosY(zoid->position, (u16 *)0);
            zoid->position[0] = (f32)zoid->gene_x;
            zoid->position[2] = (f32)zoid->gene_z;
        }
    }
}
#pragma pop
extern void OSReport(const char *fmt, ...);
extern void ZoidCopyAnim(ZoidState *zoid);
extern void ZoidInitParam(ZoidState *zoid);
extern s32 gameEndF;
extern s32 winF;
extern void sMakeMatrix(HSD_JObj *joint);
extern void zdCoStartPos(ZoidState *zoid);
extern void zdEffectPurge(ZoidState *zoid);
extern void zdSetXYZ(ZoidState *zoid);

void ZoidActDeath(ZoidState *zoid)
{
    s32 max_health;
    s32 gene_x;
    s32 gene_z;
    s32 i;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    zoid->death_pending = 1;
    if (gameEndF != 0)
        return;
    if (winF != 0)
        return;
    zoid->lock_frames++;
    if (zoid->field_20BC == 0)
        return;
    if (zoid->field_20B8 != 0)
        zoid->field_20B8--;
    if (zoid->field_20B8 != 0)
        return;
    zoid->option_use_state = 0;
    gene_x = zoid->gene_x;
    gene_z = zoid->gene_z;
    OSReport("geneX=%d geneZ=%d \n", gene_x, gene_z);
    zdEffectPurge(zoid);
    max_health = zoid->max_health;
    ZoidInitParam(zoid);
    zoid->gene_frames = 0;
    zoid->death_pending = 1;
    zoid->gene_x = gene_x;
    zoid->gene_z = gene_z;
    zoid->health = max_health;
    zoid->max_health = max_health;
    zoid->field_20B8 = zoid->field_20B4;
    zoid->field_20BC--;
    zoid->position[0] = zoid->gene_x;
    zoid->position[2] = zoid->gene_z;
    zoid->position[1] = 100.0f;
    zoid->position[1] = 1.0f + zdColPosY(zoid->position, 0);
    for (i = 0; i < 3; i++) {
        zoid->field_1A9C[i] = zoid->position[i];
        zoid->field_1C24[i] = zoid->position[i];
        zoid->field_1A80[i] = zoid->position[i];
        zoid->displacement[i] = 0.0f;
        (&zoid->knockback[0])[i] = 0.0f;
        zoid->field_1D20[i] = 0.0f;
    }
    zdSetXYZ(zoid);
    sMakeMatrix(zoid->joint);
    ZoidCopyAnim(zoid);
    zdCoStartPos(zoid);
    ZoidSetAnim(zoid, 3);
    sMakeMatrix(zoid->joint);
    zoid->position[0] = zoid->gene_x;
    zoid->position[2] = zoid->gene_z;
    zoid->position[1] = zdColPosY(zoid->position, &zoid->ground_polygon);
    zdSetXYZ(zoid);
    sMakeMatrix(zoid->joint);
    ZoidCopyAnim(zoid);
    zdCoStartPos(zoid);
    zoid->position[0] = zoid->gene_x;
    zoid->position[2] = zoid->gene_z;
    zoid->position[1] = zdColPosY(zoid->position, &zoid->ground_polygon);
    zdSetXYZ(zoid);
    zoid->action = 0x33;
}
#undef lit_2197
#define lit_2197 "geneX=%d geneZ=%d \n"
#pragma push
#pragma auto_inline off

void ZoidActMotion(ZoidState *zoid)
{
    s32 changed;

    changed = 0;
    if (zoid->current_animation != zoid->field_1DEC) {
        changed = 1;
    }
    if (zoid->animation_looped != 0) {
        changed = 1;
    }
    if (zoid->animation_stopped != 0) {
        changed = 1;
    }
    if (changed != 0) {
        ZoidSetAnim(zoid, zoid->field_1DEC);
    }
}
#pragma pop
#pragma push
#pragma auto_inline off

void ZoidActWin(ZoidState *zoid)
{
    s32 changed;

    changed = 0;
    if (zoid->current_animation != 3) {
        changed = 1;
    }
    if (changed != 0) {
        if (zoid == Zoid) {
            OSReport("mot-Win\n");
        }
        ZoidSetAnim(zoid, 3);
    }
    zoid->field_1C34 = 0;
}
#pragma pop
#undef lit_2218
#define lit_2218 "mot-Win\n"
#pragma force_active on
static const char lit_2285[9] = "foot=%X\n";
#pragma force_active off
#pragma push
#pragma auto_inline off
void flyZoid(ZoidState *zoid)
{
    register u8 *control;

    if (zoid != 0 && zoid->joint != 0) {
        control = (u8 *)zoid->anim_data;
        if (*(s32 *)(control + 0xF8) == 1) {
            switch (zoid->requested_action) {
            case 0xC:
            case 0x2C:
                zoid->requested_action = 0;
                break;
            }
        }
    }
}
#pragma pop
#pragma push
#pragma auto_inline off

void dmgZoid(ZoidState *zoid)
{
    void *control;

    if (zoid != 0 && zoid->joint != 0) {
        control = zoid->anim_data;
        if (zoid->requested_action >= 0x22 && zoid->requested_action <= 0x28 && zoid->action != 0) {
            zoid->jump_phase = 0;
            switch (zoid->action) {
            case 0x1:
            case 0x2:
            case 0xC:
            case 0xD:
            case 0xE:
            case 0xF:
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x29:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
            case 0x30:
                zoid->action = 0;
                break;
            }
        }
    }
}
#pragma pop
#pragma push
#pragma auto_inline off
void zdZuri(ZoidState *zoid)
{
    if (zoid->field_1C90[2] > 0.1) {
        return;
    }
    if (zoid->field_1C90[2] > -0.1) {
        return;
    }
}
#pragma pop
void ZoidMove(ZoidState *zoid)
{
    s32 damage_level;
    u8 *control;
    u8 *slider;
    u8 *table;
    u8 *mountp;
    s32 flag;
    s32 i;
    s32 j;
    f32 magnitude;
    f32 step;
    f32 relative;
    f32 relative2;
    f32 relative3;
    f32 knockback_length;
    f32 magnitude2;
    f32 magnitude3;
    f32 length;
    f32 sum;
    f32 v;
    Vec3 *normal;
    s32 index;

    knockback_length = 0.0f;
    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        if (zoid->action != zoid->field_1DAC) {
            zoid->field_1DAC = zoid->action;
            zoid->field_1DB0 = 0;
        } else {
            zoid->field_1DB0++;
        }
        if (zoid->death_started == 0) {
            zoid->lock_frames = 0;
        }
        flyZoid(zoid);
        zoid->field_1DA0 = -1;
        mountp = 0;
        slider = (u8 *)zoid->slider_info;
        control = (u8 *)zoid->anim_data;
        table = (u8 *)zoid->field_2090;
        if (slider != 0) {
            if (zoid->weapon_mount == 0x13) {
                mountp = slider + 2;
            } else {
                mountp = (u8 *)((zoid->weapon_mount + 1) * 16 + 2);
                mountp = (u8 *)((u32)slider + (u32)mountp);
            }
        }
        damage_level = 0;
        if (zoid->field_1E5C != 0) {
            for (i = 0; i < 3; i++) {
                zoid->knockback[i] = zoid->field_1D20[i];
                zoid->field_1D20[i] = 0.0f;
            }
            if (zoid->field_1E5C & 2) {
                damage_level = 1;
            }
            if (zoid->field_1E5C & 4) {
                damage_level = 2;
            }
            if (zoid->health == 0) {
                damage_level = 2;
            }
        } else {
            knockback_length = sLenPoint3dR(zoid->knockback);
            if (knockback_length >= 1.0 && zoid->field_1D2C < 1.0f) {
                damage_level = 1;
            }
            if (knockback_length > 1.6666666f) {
                damage_level = 2;
            }
            if (knockback_length && zoid->health == 0 && zoid->action != 0x2A && zoid->action != 0x2B
                && zoid->death_started == 0) {
                damage_level = 2;
            }
            zoid->field_1D2C = knockback_length;
        }
        if (damage_level == 1 && zoid->jump_phase != 0) {
            if (zoid->field_1EB0 == 0 || zoid->field_1EB2 == 0) {
                damage_level = 2;
            }
        }
        ZoidToFly(zoid);
        for (i = 0; i < 3; i++) {
            zoid->field_1A9C[i] = zoid->joint->child->matrix[i][3];
            zoid->field_1C24[i] = zoid->position[i];
            if (zoid->field_1A7C != 0) {
                zoid->field_1A80[i] = zoid->field_1A7C->matrix[i][3];
            }
        }
        if (zoid->health <= 0) {
            zoid->health = 0;
        }
        if (zoid->health == 0) {
            if (zoid->field_1E54 != 0) {
                zoid->field_1E54--;
            }
            if (zoid->field_1E54 == 0) {
                if (zoid->action != 0x2A && zoid->action != 0x2B && zoid->death_started == 0) {
                    zoid->launch_velocity[1] = 0.0f;
                    zoid->action = 0x2A;
                    zoid->requested_action = 0x2A;
                    zoid->action_phase = 0;
                }
            }
        }
        if (*(s32 *)(control + 0x100) != 0) {
            if (damage_level == 1 && zoid->attack_index != -1) {
                zoid->field_1E50 = 0x78;
                damage_level = 0;
            }
            if (zoid->health == 0) {
                zoid->field_1BE4 = 1;
            }
            if (zoid->health > 0 && damage_level == 2) {
                damage_level = 1;
            }
            if (damage_level != 0) {
                for (i = 0; i < 3; i++) {
                    zoid->knockback[i] = 0.0f;
                    zoid->field_1D20[i] = 0.0f;
                    zoid->field_1E5C = 0;
                }
            }
            if (zoid->action == 0x2A || zoid->action == 0x2B) {
                damage_level = 0;
                knockback_length = 0.0f;
                if (zoid->requested_action == 0x1B) {
                    zoid->requested_action = 0;
                }
                if (zoid->requested_action == 0x22) {
                    zoid->requested_action = 0;
                }
            }
        }
        if (zoid->health == 0) {
            damage_level = 2;
        }
        if (damage_level != 0) {
            zoid->requested_action = 0x1B;
            if (damage_level == 2) {
                zoid->requested_action = 0x22;
                switch (zoid->action) {
                case 0x22: case 0x23: case 0x24: case 0x25: case 0x26:
                case 0x27: case 0x28: case 0x29: case 0x2A: case 0x2B:
                    break;
                default:
                    zoid->action = 0;
                    zoid->action_phase = 0;
                    zoid->gene_frames = 0;
                    zdChgNormal(zoid);
                    zdLinierTm(zoid, 0x3C);
                    zoid->death_pending = 0;
                    zoid->field_1C34 = 0;
                    zoid->requested_action = 0x22;
                    break;
                }
            }
            if (damage_level == 1) {
                if (zoid->action >= 0x1B && zoid->action <= 0x21) {
                    zoid->action = 0;
                    zoid->action_phase = 0;
                }
            }
        }
        if (knockback_length > 0.0f && zoid->health <= 0) {
            zoid->requested_action = 0x22;
            zoid->health = 0;
            if (zoid->action >= 0xD && zoid->action <= 0x16) {
                zoid->action = 0;
            }
            if (zoid->action >= 0x2C && zoid->action <= 0x30) {
                zoid->action = 0;
            }
        }
        dmgZoid(zoid);
        if (zoid->field_1374 != 0) {
            zoid->field_1C34 = 1;
            if ((s32)mountp[6] == 1) {
                zoid->field_1C34 = 0;
                if (zoid->position[1] < zoid->ground_height || zoid->grounded != 0) {
                    zoid->field_1BFC *= 0;
                }
            }
            if (zoid->face_lock != 0) {
                ZoidSiseiFace(zoid);
            }
            return;
        }
        zoid->death_pending = 0;
        switch (zoid->action) {
        case 0:
            zoid->field_1C34 = 0;
            ZoidWalk(zoid);
            ZoidSuberi(zoid);
            zdZuri(zoid);
            zoid->jump_phase = 0;
            break;
        case 1:
        case 2:
            flag = 0;
            if (*(s32 *)(control + 0xA4) == 0) {
                flag = 1;
            }
            if (flag != 0) {
                zoid->requested_action = 0;
                zoid->action = 0;
                zoid->jump_phase = 0;
                break;
            }
            ZoidJump(zoid);
            break;
        case 0x33:
            zoid->field_1C34 = 0;
            zdModeGene(zoid);
            break;
        case 3:
            zoid->field_1C34 = 0;
            zoid->action = 0;
            break;
        case 4:
            zoid->field_1C34 = 0;
            ZoidTurnR(zoid, 0);
            break;
        case 8:
            zoid->field_1C34 = 0;
            ZoidTurnR(zoid, 1);
            break;
        case 10:
            zoid->field_1C34 = 0;
            ZoidTurnR(zoid, 2);
            break;
        case 6:
            zoid->field_1C34 = 0;
            ZoidTurnR(zoid, 3);
            break;
        case 5:
            zoid->field_1C34 = 0;
            ZoidTurnL(zoid, 0);
            break;
        case 9:
            zoid->field_1C34 = 0;
            ZoidTurnL(zoid, 1);
            break;
        case 11:
            zoid->field_1C34 = 0;
            ZoidTurnL(zoid, 2);
            break;
        case 7:
            zoid->field_1C34 = 0;
            ZoidTurnL(zoid, 3);
            break;
        case 12:
            zoid->field_1C34 = 0;
            ZoidStop1(zoid);
            break;
        case 0x2C:
            zoid->field_1C34 = 0;
            ZoidDash1(zoid);
            break;
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
            zoid->field_1DA0 = zoid->action - 13;
            zoid->field_1C34 = 1;
            zoid->field_1DD4 = 0;
            ZoidActAtack(zoid);
            break;
        case 23:
        case 24:
        case 25:
        case 26:
            zoid->field_1C34 = 1;
            ZoidActStep(zoid);
            break;
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
            zoid->field_1C34 = 0;
            ZoidActDmg2(zoid);
            break;
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
            zoid->field_1C34 = 1;
            ZoidActDmg3(zoid);
            break;
        case 41:
            if (zoid->health == 0) {
                zoid->action = 0;
                zoid->requested_action = 0x22;
            }
            zoid->field_1C34 = 1;
            ZoidActDeadly(zoid);
            break;
        case 42:
            zoid->field_1C34 = 1;
            ZoidActDead(zoid);
            if (zoid->health == 0) {
                return;
            }
            break;
        case 43:
            zoid->field_1C34 = 1;
            ZoidActDeath(zoid);
            if (zoid->health == 0) {
                return;
            }
            break;
        case 49:
            zoid->field_1C34 = 1;
            ZoidActMotion(zoid);
            break;
        case 50:
            zoid->field_1C34 = 1;
            ZoidActWin(zoid);
            break;
        default:
            zoid->field_1C34 = 0;
            break;
        }
        if (zoid->action != 0) {
            return;
        }
        switch (zoid->requested_action) {
        case 0:
            break;
        case 1:
        case 2:
            zoid->jump_phase = zoid->requested_action;
            if (*(s32 *)(control + 0xA4) == 0) {
                zoid->jump_phase = 0;
            }
            if (zoid->jump_phase != 0 && *(s32 *)(control + 0xF4) != 0
                && zoid->ground_height < zoid->position[1] - 0.5) {
                if (zoid->feet[2].joint != 0 && !(zoid->feet[2].flags & 1)) {
                    zoid->jump_phase = 0;
                }
                if (zoid->feet[3].joint != 0 && !(zoid->feet[3].flags & 1)) {
                    zoid->jump_phase = 0;
                }
            }
            if (zoid->position[1] > (f32) * (s32 *)(control + 0xA4) + zdColPosY(zoid->position, 0)) {
                if (*(s32 *)(control + 0xF8) == 0) {
                    zoid->jump_phase = 0;
                } else if (zoid->grounded != 0 || zoid->field_1EAE != 0xFFFF) {
                    zoid->jump_phase = 0;
                }
            }
            if (zoid->jump_phase == 0) {
                zoid->requested_action = 0;
                break;
            }
            zoid->action_phase = 0;
            zoid->action = zoid->requested_action;
            zoid->requested_action = 0;
            break;
        case 0x33:
            zoid->action = zoid->requested_action;
            zoid->action_phase = 0;
            zoid->requested_action = 0;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            zoid->action = zoid->requested_action;
            zoid->action_phase = 0;
            zoid->requested_action = 0;
            break;
        case 12:
            zoid->action = zoid->requested_action;
            zoid->action_phase = 0;
            zoid->requested_action = 0;
            break;
        case 13:
            if (slider[0x710] != 0 && zoid->ground_height < zoid->joint->child->matrix[1][3]) {
                break;
            }
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
            if (zoid->field_1DD4 > 0x1E) {
                break;
            }
            if (0.0f != zoid->high_angle) {
                break;
            }
            if (*(s32 *)(control + 0xF4) == 4 && *(s32 *)(control + 0xF8) == 0 && zoid->grounded == 0) {
                break;
            }
            zoid->field_1A78 = 1.0f;
            zoid->action = zoid->requested_action;
            zoid->action_phase = 0;
            zoid->requested_action = 0;
            break;
        case 23:
        case 24:
        case 25:
        case 26:
            step = zoid->step_heading;
            relative = step - zoid->heading;
            angLimit(&relative);
            magnitude = absf(relative);
            if (magnitude < 0.78539815) {
                zoid->action = 0x17;
            } else if (magnitude > DEG(135)) {
                zoid->action = 0x18;
            } else if (relative > 0.0f) {
                zoid->action = 0x1A;
            } else {
                zoid->action = 0x19;
            }
            if (magnitude > 1.5707963 && *(u16 *)(zoid->hip_coobj + 0x24) != 0xFFFF) {
                zoid->requested_action = 0;
                zoid->action = 0;
                break;
            }
            zoid->launch_velocity[0] = zoid->displacement[0] + sinf(step);
            zoid->launch_velocity[2] = zoid->displacement[2] + cosf(step);
        case 45:
        case 46:
        case 47:
        case 48:
            if (zoid->requested_action == 0x2D) {
                zoid->action = 0x17;
            }
            if (zoid->requested_action == 0x2E) {
                zoid->action = 0x18;
            }
            if (zoid->requested_action == 0x2F) {
                zoid->action = 0x19;
            }
            if (zoid->requested_action == 0x30) {
                zoid->action = 0x1A;
            }
            zoid->launch_velocity[1] = 0.01 *(f32) * (s32 *)(control + 0xD0) + zoid->displacement[1];
            if (zoid->launch_velocity[1] > 1.0f) {
                zoid->launch_velocity[1] = 1.0f;
            }
            if (zoid->ground_polygon != GROUND_POLYGON_NONE) {
                index = grdcPolygon[zoid->ground_polygon].normal_index;
                normal = &grdcNormal[index];
                zoid->launch_velocity[0] += normal->x;
                zoid->launch_velocity[2] += normal->z;
                zoid->launch_velocity[1] *= normal->y;
            }
            zoid->action_phase = 0;
            zoid->requested_action = 0;
            if (zoid->position[1] > 1.0f + zoid->ground_height) {
                flag = 0;
                if (*(s32 *)(control + 0xF8) == 0) {
                    flag = 1;
                } else if (zoid->grounded != 0 || zoid->field_1EAE != 0xFFFF) {
                    flag = 1;
                }
                if (zoid->action == 0x17 && *(u16 *)(zoid->hip_coobj + 0x24) != 0xFFFF) {
                    flag = 1;
                }
                if (flag != 0) {
                    zoid->action = 0;
                }
            }
            break;
        case 27:
            if (absf(zoid->knockback[1]) >= absf(zoid->knockback[0])
                && absf(zoid->knockback[1]) >= absf(zoid->knockback[2])) {
                if (zoid->knockback[1] > 0.0f) {
                    zoid->action = 0x21;
                } else {
                    zoid->action = 0x20;
                }
            } else {
                relative2 = vector_heading(zoid->knockback[0], zoid->knockback[2]) - zoid->heading;
                angLimit(&relative2);
                magnitude2 = absf(relative2);
                if (relative2 > 0.0f) {
                    zoid->action = 0x1E;
                } else {
                    zoid->action = 0x1D;
                }
                if (magnitude2 < 0.78539815) {
                    zoid->action = 0x1F;
                }
                if (magnitude2 > DEG(135)) {
                    zoid->action = 0x1C;
                }
            }
            zoid->knockback[0] += zoid->displacement[0];
            zoid->knockback[2] += zoid->displacement[2];
            for (i = 0; i < 3; i++) {
                zoid->launch_velocity[i] = zoid->knockback[i];
            }
            if (zoid->field_1E5C != 0) {
                zoid->field_1E5C = 0;
                for (i = 0; i < 3; i++) {
                    zoid->knockback[i] = 0.0f;
                    zoid->field_1D20[i] = 0.0f;
                }
            }
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
            zoid->action_phase = 0;
            zoid->requested_action = 0;
            break;
        case 34:
            length = sLenPoint3dR(zoid->knockback);
            if (length > 0.22) {
                zoid->knockback[0] = lit_1847[0] * (zoid->knockback[0] / length);
                zoid->knockback[1] = lit_1847[0] * (zoid->knockback[1] / length);
                zoid->knockback[2] = lit_1847[0] * (zoid->knockback[2] / length);
            }
            zoid->knockback[1] = 0.4 *((f32) * (s32 *)(table + 0x158) / 100.0);
            if (absf(zoid->knockback[1]) >= absf(zoid->knockback[0])
                && absf(zoid->knockback[1]) >= absf(zoid->knockback[2])) {
                zoid->knockback[1] = 0.4 *((f32) * (s32 *)(table + 0x158) / 100.0);
                if (zoid->knockback[1] > 0.0f) {
                    zoid->action = 0x28;
                } else {
                    zoid->action = 0x27;
                }
            } else {
                relative3 = vector_heading(zoid->knockback[0], zoid->knockback[2]) - zoid->heading;
                angLimit(&relative3);
                magnitude3 = absf(relative3);
                if (relative3 > 0.0f) {
                    zoid->action = 0x25;
                } else {
                    zoid->action = 0x24;
                }
                if (magnitude3 < 0.78539815) {
                    zoid->action = 0x26;
                }
                if (magnitude3 > DEG(135)) {
                    zoid->action = 0x23;
                }
                zoid->knockback[1] = 0.4 *((f32) * (s32 *)(table + 0x158) / 100.0);
            }
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
            sum = 0.0f;
            for (j = 0; j < 3; j++) {
                v = zoid->knockback[j];
                zoid->launch_velocity[j] = v;
                sum += v * v;
            }
            sum = square_root(sum);
            if (sum > lit_1847[0]) {
                for (j = 0; j < 3; j++) {
                    v = zoid->knockback[j];
                    zoid->launch_velocity[j] = lit_1847[0] * v / sum;
                }
            }
            v = (f32) * (s32 *)(table + 0x158) * (f32) * (s32 *)(table + 0x158) / 100.0 / 100.0;
            zoid->launch_velocity[0] *= v;
            zoid->launch_velocity[2] *= v;
            zoid->launch_velocity[1] = (f32) * (s32 *)(table + 0x158) / 100.0;
            zoid->launch_velocity[0] += zoid->displacement[0];
            zoid->launch_velocity[2] += zoid->displacement[2];
            zoid->action_phase = 0;
            zoid->requested_action = 0;
            if (zoid->field_1E5C != 0) {
                zoid->field_1E5C = 0;
                for (i = 0; i < 3; i++) {
                    zoid->knockback[i] = 0.0f;
                    zoid->field_1D20[i] = 0.0f;
                }
            }
            break;
        case 41:
        case 42:
        case 43:
            zoid->launch_velocity[1] = 0.0f;
            zoid->action = zoid->requested_action;
            zoid->action_phase = 0;
            zoid->requested_action = 0;
            break;
        case 49:
            zoid->action = zoid->requested_action;
            zoid->action_phase = 0;
            zoid->requested_action = 0;
            break;
        case 50:
            if (zoid->foot_contacts != 0 || zoid->grounded != 0) {
                zoid->action = zoid->requested_action;
                zoid->action_phase = 0;
                zoid->requested_action = 0;
            }
            break;
        }
    }
    (void)slider;
}
void zdCalcRot(ZoidState *zoid)
{
    s32 fast = 0;
    s32 back;
    s32 ok;
    s32 turn_right;
    s32 turn_left;
    s32 walk;
    ZoidState *target;
    f32 turn;
    f32 step;
    f32 heading;
    f32 turnAbs;
    f32 diff;
    f32 relative;
    f32 relativeAbs;
    f32 speedAbs;
    f32 dx;
    f32 dz;
    f32 tmp = 0.1f;

    if (zoid != (void *)0 && zoid->joint != (void *)0 && zoid->jump_phase == 0 && zoid->action == 0 && zoid->field_1360 == 0) {
        walk = zoid->current_animation;
        if (walk == 2) {
            walk = 1;
        }
        step = zoid->step_heading;
        back = 0;
        (void)back;
        turn_right = 0;
        turn_left = 0;
        heading = zoid->heading;
        diff = step - heading;
        angLimit(&diff);
        angLimit(&zoid->stick_angle);
        if (diff < -DEG(60)) {
            turn_right = 1;
        }
        if (diff > lit_848[0]) {
            turn_left = 1;
        }
        if (*(s32 *)((u8 *)zoid->ai + 0x40) != 0) {
            ok = 1;
        } else {
            ok = 1;
        }
        turnAbs = absf(zoid->stick_angle);
        turn = zoid->field_1C54;
        speedAbs = absf(turn);
        if (speedAbs > 0.2) {
            fast = 1;
        }
        if (zoid->current_animation == 4 && zoid->field_1C9C - zoid->field_1C90[0] > 0.1 && zoid->field_1C9C > 0.0f) {
            zoid->requested_action = 12;
            zoid->action_phase = 0;
            return;
        }
        if (walk == 1 && zoid->field_1C90[0] - zoid->field_1C9C > 0.005 && zoid->field_1C90[0] > 0.0f) {
            zoid->requested_action = 44;
            zoid->action_phase = 0;
            return;
        }
        if (zoid->target_pending != 0) {
            target = &Zoid[zoid->target_slot];
            if (*(s32 *)((u8 *)zoid->ai + 0x40) == 2) {
                target = &Zoid[*(s32 *)((u8 *)zoid->ai + 0x74)];
            }
            dz = target->position[2] - zoid->position[2];
            dx = target->position[0] - zoid->position[0];
            heading = atan2f(dx, dz);
            zoid->target_heading = heading;
            relative = heading - zoid->heading;
            angLimit(&relative);
            relativeAbs = absf(relative);
        }
        if (zoid->current_animation < 4 && ok != 0 && 0.0f == zoid->field_1C90[0]) {
            if (turn_right != 0) {
                if (fast != 0) {
                    zoid->requested_action = 4;
                    zoid->steer = 0.0f;
                }
                return;
            }
            if (turn_left != 0) {
                if (fast != 0) {
                    zoid->requested_action = 5;
                    zoid->steer = 0.0f;
                }
                return;
            }
        }
        if (zoid->target_pending != 0) {
            if (0.0f == zoid->field_1C90[0] && 0.0f == zoid->field_1C9C && 0.0f == zoid->field_1C90[1]) {
                if (*(s32 *)((u8 *)zoid->ai + 0x40) == 0 && zoid->field_1BA4 != 2) {
                    if (relative > DEG(90)) {
                        zoid->requested_action = 9;
                        return;
                    }
                    if (relative < -DEG(90)) {
                        zoid->requested_action = 8;
                        return;
                    }
                }
                if (back != 0) {
                    (void)0;
                }
            }
            if (zoid->field_1BA4 == 0 && turnAbs > DEG(120) && fast != 0) {
                if (zoid->field_1C90[0] > 0.2) {
                    zoid->requested_action = 12;
                    zoid->action_phase = 0;
                    return;
                }
                if (absf(diff) > DEG(90)) {
                    turn = -turn;
                    diff += 3.1415926;
                    angLimit(&diff);
                    step += 3.1415926;
                    angLimit(&step);
                    zoid->step_heading = step;
                }
            }
        }
        if (ok == 0) {
            turn = 0.0f;
        }
        if (diff < 0.0f) {
            if (diff < -DEG(2)) {
                zoid->steer = diff;
            } else {
                zoid->steer = 0.0f;
            }
        } else if (diff > DEG(2)) {
            zoid->steer = diff;
        } else {
            zoid->steer = 0.0f;
        }
        if (turn < 0.0f) {
            zoid->steer = -zoid->steer;
        }
        zoid->field_1C90[0] = turn;
    }
}
#pragma push
#pragma auto_inline off
void ZoidCount(ZoidState *zoid)
{
    if (zoid == 0) {
        return;
    }
    if (zoid->field_1C4C != 0) {
        zoid->field_1C4C--;
    }
}
#pragma pop
void ZoidSpdA(ZoidState *zoid)
{
    f32 sum;
    f32 value;
    f64 scale;
    s32 i;

    if (zoid != 0 && zoid->joint != 0) {
        sum = 0.0f;
        for (i = 0; i < 3; i++) {
            zoid->previous_displacement[i] = zoid->displacement[i];
            zoid->displacement[i] = zoid->position[i] - zoid->field_1C24[i];
            value = zoid->displacement[i];
            sum = value * value + sum;
            zoid->displacement_change[i] = zoid->displacement[i] - zoid->previous_displacement[i];
        }
        zoid->speed = square_root(sum);
        if (zoid->field_1EB0 != 0 || zoid->field_1EB2 != 0) {
            for (i = 0; i < 3; i++) {
                zoid->displacement[i] = 0.0f;
            }
        }
    }
}
#include "game_mode.h"
extern s32 sCamNN;
extern s32 gameEndF;
extern void zdCalcRot(ZoidState *zoid);
extern void sColGetPolygonCenter(u16 polygon, f32 *center);
extern void sLinear1D(f32 *value, f32 target, f32 step);
extern void zdLinier(ZoidState *zoid);
extern void zdSetXYZ(ZoidState *zoid);
extern void sMakeMatrix(HSD_JObj *joint);
extern void zdFootBak(ZoidState *zoid);
extern void zdGroundPolyClear(ZoidState *zoid);
extern void ZoidDownBody(ZoidState *zoid);
extern void ZoidGround(ZoidState *zoid);
extern void sGlobal2Local(HSD_JObj *joint);
extern void ZoidSpdA(ZoidState *zoid);
extern void ZoidCoBAK(ZoidState *zoid, u8 *object);
extern void ZoidGrdON(ZoidState *zoid);
extern void ZoidCount(ZoidState *zoid);
extern void zdCalcCoDmg(ZoidState *zoid);
extern void zdEffectMountInt(ZoidState *zoid);
extern void zdVoiceSc(ZoidState *zoid, s32 voice);
extern void zdVoiceScG(ZoidState *zoid, s32 voice);
extern void PartnerFace(s32 face, s32 frames);

void ZoidAllMove(void)
{
    ZoidState *zoid;
    s32 n;
    s32 m;
    s32 k;
    s32 j;
    s32 i;
    ZoidState *partner;
    s32 best;
    s32 count;
    f32 dead;
    f32 after;
    f32 before;
    u16 selector;

    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint == 0)
            continue;
        if (zoid->rumble_timer != 0)
            zoid->rumble_timer--;
        if (zoid->field_1E50 != 0)
            zoid->field_1E50--;
        if (zoid->health_bar_timer != 0)
            zoid->health_bar_timer--;
        angLimit(&zoid->heading);
        zdCalcRot(zoid);
        angLimit(&zoid->heading);
        ZoidMove(zoid);
        if (zoid->ground_polygon == GROUND_POLYGON_NONE) {
            for (j = 0; j < 3; j++)
                zoid->position[j] = zoid->field_1C24[j];
            sColPosY((Vec3 *)zoid->position, &selector);
            if (selector == GROUND_POLYGON_NONE) {
                if (zoid->position[0] < 0.0f)
                    zoid->position[0] += 1.0f;
                if (zoid->position[2] < 0.0f)
                    zoid->position[2] += 1.0f;
                if (zoid->position[0] > 0.0f)
                    zoid->position[0] -= 1.0f;
                if (zoid->position[2] > 0.0f)
                    zoid->position[2] -= 1.0f;
            }
            if (zoid->field_1EBA != GROUND_POLYGON_NONE)
                sColGetPolygonCenter(zoid->field_1EBA, zoid->position);
        } else {
            zoid->field_1EBA = zoid->ground_polygon;
        }
        zdLinier(zoid);
        zdSetXYZ(zoid);
        sMakeMatrix(zoid->joint);
        zdFootBak(zoid);
        zdGroundPolyClear(zoid);
        if (zoid->death_pending != 0)
            ZoidDownBody(zoid);
        if (zoid->death_pending == 0)
            ZoidGround(zoid);
        angLimit(&zoid->heading);
        zoid->previous_animation = zoid->current_animation;
        zoid->field_1C9C = zoid->field_1C90[0];
        zoid->field_19FC = zoid->position[1] - zoid->field_1C24[1];
        for (m = 0; m < 4; m++) {
            for (n = 0; n < 3; n++)
                zoid->child_matrix[n][m] = zoid->joint->child->matrix[n][m];
        }
        sGlobal2Local(zoid->joint->child);
        zoid->field_1A40[0] = zoid->joint->child->translation.x;
        zoid->field_1A40[1] = zoid->joint->child->translation.y;
        zoid->field_1A40[2] = zoid->joint->child->translation.z;
        zoid->heading_delta = zoid->joint->child->rotation[1];
        ZoidSpdA(zoid);
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
        ZoidGrdON(zoid);
        ZoidCount(zoid);
        for (k = 0; k < 3; k++) {
            dead = 1.0f / 600.0f;
            sLinear1D(&zoid->knockback[k], 0.0f, dead);
        }
        zdCalcCoDmg(zoid);
        zdEffectMountInt(zoid);
    }
    zoid = Zoid;
    partner = &Zoid[2];
    if (zoid->joint != 0 && partner->joint != 0 && sCamNN == 1 && zoid->slot == partner->slot && gameEndF == 0
        && GameMode.game_type == 1) {
        if (partner->max_health == 0)
            partner->max_health = 100;
        before = (f32)partner->field_0150 / (f32)partner->max_health;
        after = (f32)partner->health / (f32)partner->max_health;
        if (before >= 0.5 && after < 0.5) {
            zdVoiceScG(partner, 12);
            PartnerFace(4, 60);
        }
        if (partner->field_0150 != 0 && partner->health == 0 && zoid->health != 0)
            zdVoiceSc(partner, 15);
        if (zoid->field_0150 != 0 && zoid->health == 0 && partner->health != 0)
            zdVoiceSc(zoid, 9);
    }
    if (GameMode.game_type == 1 || GameMode.game_type == 4) {
        i = 0;
        count = 0;
        best = -1;
        for (; i < 16; i++) {
            zoid = &Zoid[i];
            if (zoid->joint != 0 && zoid->slot != Zoid[0].slot) {
                if (zoid->health == 0 && zoid->field_0150 != 0)
                    best = i;
                if (zoid->health != 0)
                    count++;
            }
        }
        if (best != -1)
            zdVoiceSc(&Zoid[best], 8);
    }
    for (i = 0; i < 16; i++) {
        zoid = &Zoid[i];
        if (zoid->joint != 0)
            zoid->field_0150 = zoid->health;
    }
}

void ZoidInertiaAtk(ZoidState *zoid)
{
    s32 axis;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        if (zoid->field_1EB0 != 0 || zoid->field_1EB2 != 0 || zoid->field_1EAE != 0xFFFF) {
            zoid->ground_turn = 0xF;
            for (axis = 0; axis < 3; axis++)
                zoid->displacement[axis] = 0.0f;
        }
        if (zoid->ground_turn != 0) {
            zoid->displacement[0] = 0.0f;
            zoid->displacement[2] = 0.0f;
            if (zoid->displacement[1] > 0.0f)
                zoid->displacement[0] = 0.0f;
        }
        ZoidInertia(zoid);
    }
}
s32 zdAtkSub(ZoidState *zoid)
{
    register s32 i;
    register ZoidState *target;
    f32 best;
    register f32 d;
    f32 distance;

    if (zoid != (void *)0 && zoid->joint != (void *)0 && zoid->attack_target <= -1) {
        best = lit_1494[0];
        for (i = 0; i < 16; i++) {
            target = &Zoid[i];
            if (target->joint != (void *)0 && target->stel_state != 2 && zoid->attack_in_range[i] != 0 && !((d = zoid->distances[i]) <= 0.0f)) {
                if (lit_1494[0] == best || distance > d) {
                    distance = zoid->distances[i];
                    best = i;
                }
            }
        }
        zoid->attack_target = best;
    }
}
extern f32 sLenJobj(HSD_JObj *a, HSD_JObj *b);
extern f32 ZoidCalcAngle(ZoidState *zoid, Vec3 *target);

void zdAtkSubFor(ZoidState *zoid, s32 attack)
{
    ZoidState *target;
    f32 angle;
    f32 limit;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        if (zoid->attack_target != -1 && zoid->attack_target != -2 && zoid->action_phase != 3) {
            limit = lit_1519[0];
            if (zoid->action_phase == 0) {
                limit = lit_1520[0];
            }
            target = &Zoid[zoid->attack_target];
            if (target->joint != (void *)0 &&
                (zoid->action_phase == 0 || !(sLenJobj(zoid->joint->child, target->joint->child) < lit_1521[0]))) {
                angle = ZoidCalcAngle(zoid, (Vec3 *)target->position);
                if (angle > limit) {
                    angle = limit;
                }
                if (angle < -limit) {
                    angle = -limit;
                }
                zoid->heading += angle;
                angLimit(&zoid->heading);
            }
        }
    }
    attack = attack;
}
extern void HSD_JObjSetupMatrixSub(HSD_JObj *joint);
extern void sMakeMatrix(HSD_JObj *joint);
extern f32 sLenPoint3d(Vec3 *first, Vec3 *second);
extern f32 beSide(f32 value, f32 target, f32 step);

static inline f32 HSD_JObjGetTranslationZ(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 1033, "jobj");
    return jobj->translation.z;
}

static inline void HSD_JObjSetupMatrix(HSD_JObj *jobj)
{
    if (!jobj || !HSD_JObjMtxIsDirty(jobj))
        return;
    HSD_JObjSetupMatrixSub(jobj);
}

void zdAtkSubForPull(ZoidState *zoid, s32 attack)
{
    f32 z;
    f32 scale;
    f32 range;
    f32 dist;
    f32 value;
    ZoidState *target;
    u8 *data;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        if (zoid->field_1A78 > 1.0f) {
            zoid->field_1A78 = 1.0f;
        }
        if (zoid->field_1A78 < 0.0f) {
            zoid->field_1A78 = 0.0f;
        }
        if (zoid->attack_target < 0) {
            scale = zoid->field_1A78;
            z = HSD_JObjGetTranslationZ(zoid->joint->child);
            HSD_JObjSetTranslateZ(zoid->joint->child, z * scale);
            sMakeMatrix(zoid->joint);
            HSD_JObjSetupMatrix(zoid->joint);
        } else {
            target = &Zoid[zoid->attack_target];
            if (target->joint == (void *)0) {
                return;
            }
            data = (u8 *)zoid->anim_data;
            if (zoid->ground_turn != 0) {
                return;
            }
            if (*(s16 *)((u8 *)Tea_zoiddat + zoid->type * 0x7E8 + attack * 2 + 0x75A) == 0) {
                return;
            }
            range = *(s16 *)((u8 *)Tea_zoiddat + zoid->type * 0x7E8 + attack * 2 + 0x6B2);
            if (0.0f == range) {
                return;
            }
            range = range + (f32)*(s32 *)(data + 0x2e4) / lit_1572[0];
            if (0.0f == range) {
                return;
            }
            dist = sLenPoint3d((Vec3 *)zoid->position, (Vec3 *)target->position);
            if (dist < 1.0f) {
                dist = 1.0f;
            }
            scale = dist / range;
            if (scale < 0.1) {
                scale = 0.1f;
            }
            if (scale > 1.0f) {
                scale = 1.0f;
            }
            zoid->field_1A78 = beSide(zoid->field_1A78, scale, 0.1f);
            z = HSD_JObjGetTranslationZ(zoid->joint->child);
            value = z * zoid->field_1A78;
            HSD_JObjSetTranslateZ(zoid->joint->child, value);
            sMakeMatrix(zoid->joint);
            HSD_JObjSetupMatrix(zoid->joint);
        }
    }
}
#pragma push
#pragma auto_inline off
void coBakAtk(ZoidState *zoid)
{
    s32 axis;

    for (axis = 0; axis < 3; axis++)
        zoid->displacement[axis] = *(f32 *)((u8 *)zoid + 0x4E4 + axis * 4);
    ZoidInertiaAtk(zoid);
}
#pragma pop
typedef struct ZoidLakkaCtl {
    u8 reserved_000[0xF8];
    s32 field_00F8;
} ZoidLakkaCtl;

extern s32 zdLakkaTest2(ZoidState *zoid);
extern void ZoidInertiaAtk(ZoidState *zoid);

s32 zdAtkSubLakka(ZoidState *zoid)
{
    s32 i;
    ZoidLakkaCtl *control;
    f32 lift;

    control = (ZoidLakkaCtl *)zoid->anim_data;
    if (control->field_00F8 == 1) {
        return 0;
    }
    if (zoid->ground_turn != 0) {
        for (i = 0; i < 3; i++) {
            zoid->displacement[i] = 0.0f;
        }
    }
    if ((zoid->movement_flags & 1) == 0 && zoid->grounded == 0) {
        zoid->displacement[1] -= 0.025;
        zoid->position[1] += zoid->displacement[1];
    }
    if (zdLakkaTest2(zoid) == 0) {
        return 0;
    }
    setAnimRate(zoid->joint, 0.0f);
    for (i = 0; i < 3; i++) {
        zoid->displacement[i] = *(f32 *)((u8 *)zoid + 0x4E4 + i * 4);
    }
    ZoidInertiaAtk(zoid);
    lift = zoid->height_offset;
    zoid->position[1] += lift;
    zoid->height_offset -= lift;
    zoid->field_1C24[1] += lift;
    zoid->field_1BFC = 0;
    return 1;
}
#include "game_mode.h"
typedef struct ZoidControlData {
    u8 reserved_000[0xF4];
    s32 field_00F4;
    s32 field_00F8;
    u8 reserved_0FC[0x94];
    s32 attack_chain_length;
} ZoidControlData;
typedef struct AtkTbl5 {
    s32 v[5];
} AtkTbl5;
extern void OSReport(const char *fmt, ...);
extern f32 beSide(f32 value, f32 target, f32 step);
extern s32 ZoidSisei(ZoidState *zoid);
extern s32 zdAtkSub(ZoidState *zoid);
extern void zdAtkSubFor(ZoidState *zoid, s32 attack);
extern void zdAtkSubForPull(ZoidState *zoid, s32 attack);
extern s32 zdFaceAtk(ZoidState *zoid, f32 distance, f32 offset);
extern f32 zdGoYA(ZoidState *zoid, f32 a, f32 b);
extern f32 sColPtJobj(u8 *a, u8 *b, GroundMask *mask);
extern void coBakAtk(ZoidState *zoid);
extern void zdAtackSetEffect(ZoidState *zoid, s32 effect);
extern void zdSetXYZ(ZoidState *zoid);
extern void sMakeMatrix(HSD_JObj *joint);
extern void ZoidInertiaAtk(ZoidState *zoid);
extern s32 zdAtkSubLakka(ZoidState *zoid);
extern void zdVoiceScG(ZoidState *zoid, s32 voice);

void ZoidActAtack(ZoidState *zoid)
{
    AtkTbl5 tblA = *(const AtkTbl5 *)lit_1601;
    AtkTbl5 tblB = *(const AtkTbl5 *)lit_1602;
    f32 saved[3];
    f32 pos[3];
    GroundMask mask;
    GroundMask tmp;
    f32 lift;
    f32 dist;
    f32 range;
    s32 i;
    s32 ok;
    ZoidControlData *control;
    u8 *camera;
    s32 attack;
    u8 *slider;
    s32 busy;
    s32 flags;
    s32 voice;
    s32 grounded;
    s32 turn;

    if (zoid == (void *)0)
        return;
    control = (ZoidControlData *)zoid->anim_data;
    if (control == (void *)0)
        return;
    slider = (u8 *)zoid->slider_info;
    if (slider == 0)
        return;
    camera = (u8 *)zoid->camera;
    if (zoid->requested_action == 2)
        zoid->requested_action = 0;
    if (zoid->requested_action == 1)
        zoid->requested_action = 0;
    busy = 0;
    if (zoid->requested_action == 0x1b)
        busy = 1;
    if (zoid->requested_action == 0x22)
        busy = 1;
    if (busy != 0)
        zoid->action_phase = 0x99;
    turn = zoid->ground_turn;
    attack = zoid->action - 0xd;
    turn = turn;
    turn = turn;
    slider = slider;
    busy = busy;
    flags = flags;
    zoid->face_yaw = beSide(zoid->face_yaw, 0.0f, lit_1845[0]);
    zoid->face_pitch = beSide(zoid->face_pitch, 0.0f, lit_1845[0]);
    zoid->senter_roll = beSide(zoid->senter_roll, 0.0f, lit_1845[0]);
    zoid->f_base_ry = beSide(zoid->f_base_ry, 0.0f, lit_1845[0]);
    ZoidSisei(zoid);
    if (zoid->action_phase == 0)
        zoid->attack_target = -1;
    zdAtkSub(zoid);
    zdAtkSubFor(zoid, attack);
    zoid->jump_attack_phase = 0;
    zdAtkSubForPull(zoid, attack);
    range = (f32)Tea_zoiddat[zoid->type].attack_range[attack];
    zoid->gene_frames = zoid->gene_frames + 1;
    switch (zoid->action_phase) {
    case 0:
        lift = zoid->height_offset;
        if (lift > 0.0f) {
            zoid->position[1] = zoid->position[1] + lift;
            zoid->height_offset = zoid->height_offset - lift;
            zoid->field_1C24[1] = zoid->field_1C24[1] + lift;
        }
        if (camera != 0)
            *(s32 *)(camera + 0x34) = 600;
        if (zoid->action == 0xd) {
            zoid->gene_frames = 0;
        } else if (zoid->field_1EB0 == 0 && zoid->field_1EAE == 0xffff) {
            zoid->ground_turn = 0;
        }
        ok = 1;
        dist = lit_1846[0];
        if (slider[0x70b] & 2) {
            dist = range;
            if (dist < lit_1846[0])
                dist = lit_1846[0];
        }
        if (zdFaceAtk(zoid, dist, 0.0f) != 0)
            ok = 0;
        if (zdGoYA(zoid, 0.0f, 0.5f) > 1.0f)
            ok = 0;
        if (zdGoYA(zoid, 0.0f, 1.0f) > 1.0f)
            ok = 0;
        if (zdGoYA(zoid, 0.0f, lit_1847[0]) > 1.0f)
            ok = 0;
        if (zdGoYA(zoid, 0.0f, lit_1846[0]) > 1.0f)
            ok = 0;
        if (zoid->action != 0xd && zoid->attack_target != -1) {
            if (sColPtJobj((u8 *)zoid->camera_joint, (u8 *)Zoid[zoid->attack_target].camera_joint, (mask = (tmp = *(const GroundMask *)lit_1651), &mask)) == *(const f32 *)lit_1848)
                ok = 1;
        }
        if (ok == 0) {
            zoid->action_phase = 0x99;
            for (i = 0; i < 3; i++)
                saved[i] = zoid->joint->child->matrix[i][3];
            ZoidSetAnim(zoid, 1);
            for (i = 0; i < 3; i++)
                zoid->position[i] = saved[i];
            zdSetXYZ(zoid);
            sMakeMatrix(zoid->joint);
            return;
        }
        ok = 1;
        if (control->field_00F4 == 4 && control->field_00F8 == 0) {
            if (zoid->grounded == 0 && zoid->field_1EB0 == 0)
                ok = 0;
        }
        if (control->field_00F8 == 1)
            ok = 1;
        if (ok == 0 && zoid->action != 0xd && zoid->gene_frames < 0x1e)
            ok = 1;
        if (ok == 0) {
            coBakAtk(zoid);
            zoid->action_phase = 0x99;
            OSReport(lit_1849);
            return;
        }
        zoid->gene_frames = 0;
        ZoidSetAnim(zoid, tblA.v[zoid->action - 0xd]);
        if (zoid->field_1EB0 != 0)
            zoid->ground_turn = 0xf;
        zdAtackSetEffect(zoid, attack);
        zoid->action_phase = 1;
        zdAtkSub(zoid);
        if (zoid->action == 0xd) {
            voice = 1;
            if (GameMode.game_type == 1 && zoid == &Zoid[2] && zoid->slot == Zoid[0].slot)
                voice = 0;
            if (voice != 0)
                zdVoiceScG(zoid, zoid->action + 5);
        }
        zoid->field_1C90[0] = 0.0f;
        zoid->field_1C90[1] = 0.0f;
        zoid->field_1C90[2] = 0.0f;
        for (i = 0; i < 3; i++) {
            zoid->displacement[i] = 0.0f;
            zoid->field_1C24[i] = zoid->position[i];
        }
        break;
    case 0x11:
        setAnimRate(zoid->joint, 0.0f);
        if (zoid->ground_turn != 0)
            zoid->ground_turn = 0xf;
        if (zoid->field_1EAE != 0xffff || zoid->field_1EB0 != 0 || *(u32 *)(zoid->r_leg2_coobj + 0x1c) != 0 || *(u32 *)(zoid->l_leg2_coobj + 0x1c) != 0) {
            zoid->ground_turn = 0xf;
            zoid->action_phase = 0x13;
            zoid->grounded_frames = 0;
            for (i = 0; i < 3; i++)
                zoid->displacement[i] = 0.0f;
            break;
        }
        if (zoid->grounded != 0) {
            zoid->action_phase = 0x12;
            zoid->grounded_frames = 0;
            zoid->ground_turn = 0xf;
            if (control->field_00F8 == 2) {
                zoid->action_phase = 0x13;
                zoid->grounded_frames = 0;
                for (i = 0; i < 3; i++)
                    zoid->displacement[i] = 0.0f;
            }
        }
        ZoidInertiaAtk(zoid);
        zoid->field_1BFC = 0;
        break;
    case 0x12:
        setAnimRate(zoid->joint, 0.0f);
        if (zoid->field_1EAE != 0xffff || zoid->field_1EB0 != 0) {
            zoid->ground_turn = 0xf;
            zoid->action_phase = 0x13;
            zoid->grounded_frames = 0;
            for (i = 0; i < 3; i++)
                zoid->displacement[i] = 0.0f;
            break;
        }
        zoid->grounded_frames = zoid->grounded_frames + 1;
        ok = 0;
        if (control->field_00F4 != 0) {
            if (*(u32 *)(zoid->r_leg2_coobj + 0) != 0 && (*(u32 *)(zoid->r_leg2_coobj + 0x14) != 0 || *(u32 *)(zoid->r_leg2_coobj + 0x18) != 0))
                ok = 1;
            if (*(u32 *)(zoid->l_leg2_coobj + 0) != 0 && (*(u32 *)(zoid->l_leg2_coobj + 0x14) != 0 || *(u32 *)(zoid->l_leg2_coobj + 0x18) != 0))
                ok = 1;
            if (*(u32 *)(zoid->head_coobj + 0) != 0 && (*(u32 *)(zoid->head_coobj + 0x14) != 0 || *(u32 *)(zoid->head_coobj + 0x18) != 0))
                ok = 1;
            if ((zoid->feet[2].flags & 1) || (zoid->feet[3].flags & 1))
                ok = 1;
        }
        if (zoid->grounded_frames > 6)
            ok = 1;
        if (zoid->grounded != 0)
            ok = 1;
        if (ok != 0) {
            zoid->displacement[0] = 0.0f;
            zoid->displacement[2] = 0.0f;
            if (zoid->displacement[1] > 0.0f)
                zoid->displacement[1] = lit_1850[0];
            zoid->displacement[1] = lit_1850[0];
            zoid->action_phase = 0x13;
        } else {
            ZoidInertiaAtk(zoid);
            zoid->field_1BFC = 0;
        }
        break;
    case 1:
        if (camera != 0)
            *(s32 *)(camera + 0x34) = 600;
        if (range > lit_1847[0]) {
            grounded = 1;
            flags = slider[0x70e];
            if (zoid->grounded == 0)
                grounded = 0;
            if (grounded != 0) {
                if ((flags & 1) && *(u16 *)(zoid->head_coobj + 0x24) != 0xffff)
                    zoid->ground_turn = 1;
                if ((flags & 2) && *(u16 *)(zoid->neck_coobj + 0x24) != 0xffff)
                    zoid->ground_turn = 1;
                if ((flags & 4) && *(u16 *)(zoid->bust_coobj + 0x24) != 0xffff)
                    zoid->ground_turn = 1;
            }
        }
        if (zoid->ground_turn != 0)
            zoid->ground_turn = 0xf;
        ok = 0;
        if (zdAtkSubLakka(zoid) != 0) {
            zoid->action_phase = 0x11;
            setAnimRate(zoid->joint, 0.0f);
            break;
        }
        if (zoid->animation_stopped == 0 && zoid->animation_looped == 0)
            ok = 1;
        if (ok != 0)
            break;
    case 0x13:
        zoid->gene_frames = 0;
        if (zoid->requested_action == 0xd && attack + 1 < control->attack_chain_length) {
            zoid->action++;
            zoid->action_phase = 0;
            zoid->requested_action = 0;
            zoid->gene_frames = 0;
            break;
        }
        zoid->action_phase = 2;
    case 2:
        if (zoid->field_1EAE != 0xffff || zoid->field_1EB0 != 0)
            turn = 0xf;
        zoid->attack_target = -2;
        ZoidSetAnim(zoid, tblB.v[zoid->action - 0xd]);
        if (turn != 0)
            zoid->ground_turn = 0xf;
        zoid->displacement[0] = 0.0f;
        zoid->displacement[1] = 0.0f;
        zoid->displacement[2] = 0.0f;
        zoid->action_phase = 3;
        zoid->gene_frames = 0;
        break;
    case 3:
        zoid->attack_target = -2;
        zoid->gene_frames = zoid->gene_frames + 1;
        if (zdAtkSubLakka(zoid) != 0) {
            zoid->action_phase = 0x31;
            zoid->gene_frames = 0;
            setAnimRate(zoid->joint, 0.0f);
            break;
        }
        if (zoid->gene_frames > 0x12c) {
            zoid->action_phase = 0x99;
            break;
        }
        if (zoid->animation_stopped == 0 && zoid->animation_looped == 0)
            break;
    default:
        zoid->action = 0;
        if (zoid->requested_action == 0x1b) {
            zdChgNormal(zoid);
            zoid->requested_action = 0x1b;
        } else if (zoid->requested_action == 0x22) {
            zdChgNormal(zoid);
            zoid->requested_action = 0x22;
        } else {
            zoid->requested_action = 0;
            zdChgNormal(zoid);
            if (turn != 0)
                zoid->ground_turn = 0xf;
            if (zoid->field_1EAE == 0xffff && turn != 0) {
                for (i = 0; i < 3; i++) {
                    zoid->displacement[i] = 0.0f;
                    zoid->launch_velocity[i] = 0.0f;
                }
            }
            if (camera != 0)
                *(s32 *)(camera + 0x34) = 600;
            zoid->field_1DA0 = -1;
        }
        break;
    case 0x31:
        if (zoid->displacement[1] > 0.0f)
            zoid->displacement[1] = 0.0f;
        if (zoid->launch_velocity[1] > 0.0f)
            zoid->launch_velocity[1] = 0.0f;
        if (zoid->grounded != 0) {
            zoid->action_phase = 3;
            zoid->grounded_frames = 0;
        }
        if (zoid->gene_frames > 0x12c)
            zoid->action_phase = 0x99;
        ZoidInertiaAtk(zoid);
        zoid->field_1BFC = 0;
        break;
    }
}
}
