extern "C" {
#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"
#include "joint.h"
#include "aobj.h"
#include "model_archive.h"
#include "object.h"
#include "ground_collision.h"

typedef struct BulletDat {
    u8 pad_00[8];
    s32 damage;
    s16 speed;
    u8 pad_0E[4];
    s16 kind;
    u8 pad_14[2];
    s16 trace_rate;
    u8 pad_18[2];
    s16 zoid_hit;
    s16 hit_effect;
    s16 fire_effect;
    s16 se;
    u8 pad_22[6];
    s16 turn_angle;
    u8 pad_2A[0x22];
} BulletDat;
typedef f32 MisMtx[3][4];
typedef struct MisAi3 {
    u8 pad_00[0x84];
    s32 missile_hit;
} MisAi3;
typedef struct MisZoid3 {
    u8 pad_0000[0x40];
    HSD_JObj *joint;
    u8 pad_0044[0x14C - 0x44];
    s32 health;
    s32 max_health;
    s16 option_type;
    u8 option_use_state;
    u8 option_use_previous;
    s16 option_energy_max;
    s16 option_energy;
    u8 pad_015C[0x45C - 0x15C];
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
    u8 pad_1128[0x12FC - 0x1128];
    s32 weapon_mount;
    u8 pad_1300[0x19C4 - 0x1300];
    HSD_JObj *camera_joint;
    u8 pad_19C8[0x1B68 - 0x19C8];
    f32 position[3];
    u8 pad_1B74[0x1C2C - 0x1B74];
    f32 knockback[3];
    u8 pad_1C38[0x1D44 - 0x1C38];
    s16 ally_smoke_timer;
    s16 enemy_smoke_timer;
    s32 field_1E50;
    u8 pad_1D4C[4];
    s32 health_bar_timer;
    u8 pad_1D54[0x1D78 - 0x1D54];
    s32 rumble_timer;
    u8 index;
    u8 slot;
    u8 pad_1D7E[0x1DBC - 0x1D7E];
    s32 interpolation_remaining;
    s32 interpolation_duration;
    s32 kills;
    s32 field_1EF0;
    s32 field_1EF4;
    u8 pad_1DD0[0x1DD8 - 0x1DD0];
    s32 armor;
    u8 pad_1DDC[0x1F30 - 0x1DDC];
    MisAi3 *ai;
    u8 pad_1F34[0x1F50 - 0x1F34];
    s32 field_20C0;
    u8 pad_1F54[0x1F8C - 0x1F54];
} MisZoid3;
typedef struct Missile3 {
    s32 flags;
    s32 bullet;
    s32 kind;
    s32 zoid_hit;
    s32 hit_ground;
    s32 animation;
    ModelArchive *archive;
    HSD_JObj *joint;
    MisZoid3 *owner;
    s32 owner_weapon_mount;
    MisZoid3 *target_zoid;
    struct Missile3 *target_missile;
    s32 lock;
    void *field_3C;
    f32 speed;
    f32 turn_rate;
    f32 matrix[3][4];
    f32 previous[3][4];
    f32 step_length;
    f32 inverse[3][4];
    s32 update_count;
    s32 damage;
    s32 purge_timer;
    s32 hit;
    f32 max_speed;
    f32 accel;
    u8 pad_EC[4];
    s32 proc;
    s32 proc_delay;
    HSD_JObj *beam_joints[7];
    f32 beam_positions[7][3];
    HSD_JObj *a1_joint;
    struct HSD_LObj *light;
    s8 hit_zoids[16];
    s32 se;
    s32 se_playing;
    u32 se_voice;
} Missile3;

typedef struct WeaponInverse3 {
    HSD_JObj *joint;
    f32 inverse[3][4];
    u8 pad_34[0x38 - 0x34];
} WeaponInverse3;
typedef struct WeaponState3 {
    s32 active;
    u8 pad_04[0x38 - 0x04];
    MisZoid3 *owner;
    u8 pad_3C[0x144 - 0x3C];
    WeaponInverse3 inverses[12];
    u8 pad_3E4[0x404 - 0x3E4];
    s16 drain;
    u8 pad_406[0x6D8 - 0x406];
} WeaponState3;

extern WeaponState3 sWeapon[100];
extern ModelArchive *missileFile[130];
extern Missile3 missileStart[200];
extern void *missBinB2;
extern s32 misALL;
extern s32 nowMJ;
extern s32 sLightMis;
extern s32 misMAX;
extern s32 nowMX;
extern BulletDat Tea_bulletdat[];
extern s32 SE_TableList[];
extern ModelArchive *sEfctLight;
extern void ZeroMemory(void *ptr, u32 size);
extern HSD_JObj *CopyJoint2(void *archive, s32 model);
extern HSD_JObj *searchJobjJoint(void *archive, HSD_JObj *joint, char *name);
extern void PSMTXRotRad(MisMtx m, char axis, f32 rad);
extern void PSMTXConcat(MisMtx a, MisMtx b, MisMtx ab);
extern void sGlobal2Local(HSD_JObj *joint);
extern void sMakeMatrix(HSD_JObj *joint);
extern struct HSD_LObj *LoadArc2Light(void *archive, s32 index);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *joint);
extern void __assert(const char *file, s32 line, const char *expr);
extern f32 HSD_Randf(void);
extern void sMissileSetAnim(Missile3 *missile, s32 animation);
extern void sEffectSet(s32 type, s32 mode, f32 *position, HSD_JObj *source, void *zoid);
extern void PSMTXMultVec(MisMtx m, Vec3 *src, Vec3 *dst);
extern s32 HSD_Randi(s32 maximum);
extern void HSD_JObjRemoveAnimAll(HSD_JObj *joint);
extern void HSD_JObjAddAnimAll(HSD_JObj *joint, struct HSD_AnimJoint *anim, struct HSD_MatAnimJoint *mat, struct HSD_ShapeAnimJoint *shape);
extern void HSD_JObjReqAnimAll(HSD_JObj *joint, f32 frame);
extern void setAnimRate(HSD_JObj *joint, f32 rate);
extern void jobjUnrefAll(HSD_JObj *joint);
extern void HSD_LObjAddCurrent(struct HSD_LObj *lobj);
extern HSD_AObj *getAnimAobj(HSD_JObj *joint);
extern void HSD_JObjAnimAll(HSD_JObj *joint);
extern u32 HSD_AObjGetFlags(HSD_AObj *aobj);
extern void setAnimFrame(HSD_JObj *joint, f32 frame);
extern s32 usbForce;
extern void *sReadFileU(char *name, s32 mode);
extern ModelArchive *sMissileLoad(s32 index, BulletDat *dat);
extern void *binB2(void *pack, s32 index, s32 *size);
extern ModelArchive *LoadArc2B(void *data, s32 size, BulletDat *name);
extern void HSD_JObjDispAll(HSD_JObj *joint, void *cobj, u32 flags, u32 rendermode);

#pragma dont_inline off
static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 605, "jobj");
    return !(jobj->flags & 0x800000) && (jobj->flags & 0x40);
}
#define HSD_JObjSetMtxDirty(jobj) \
    do { \
        if ((jobj) != 0 && !HSD_JObjMtxIsDirty(jobj)) \
            HSD_JObjSetMtxDirtySub(jobj); \
    } while (0)
static inline f32 HSD_JObjGetScaleX(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 899, "jobj");
    return jobj->scale.x;
}
static inline void HSD_JObjSetScaleX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 805, "jobj");
    jobj->scale.x = value;
    if (!(jobj->flags & 0x02000000))
        HSD_JObjSetMtxDirty(jobj);
}
static inline void HSD_JObjSetTranslateX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 952, "jobj");
    jobj->translation.x = value;
    if (!(jobj->flags & 0x02000000))
        HSD_JObjSetMtxDirty(jobj);
}
static inline void HSD_JObjSetTranslateY(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 966, "jobj");
    jobj->translation.y = value;
    if (!(jobj->flags & 0x02000000))
        HSD_JObjSetMtxDirty(jobj);
}
static inline void HSD_JObjSetTranslateZ(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 980, "jobj");
    jobj->translation.z = value;
    if (!(jobj->flags & 0x02000000))
        HSD_JObjSetMtxDirty(jobj);
}
#pragma dont_inline on
extern f32 sLiner(f32 a, f32 b, f32 t);
extern void free(void *memory);
extern void sAtack(Missile3 *missile);
extern void sHitOn(Missile3 *missile);
extern void sMissilePrologueU(s32 usb);
extern void sMissilePurge(Missile3 *missile);
extern void sMove(Missile3 *missile);
extern f32 sMissileZoid1a(Missile3 *missile, MisZoid3 *zoid, void *object, s32 push);
extern void ZoidCopyAnim(MisZoid3 *zoid);
extern s32 hitCalc2(MisZoid3 *attacker, s32 damage, MisZoid3 *zoid);
extern void zdVoiceSc(MisZoid3 *zoid, s32 voice);
extern MisZoid3 Zoid[];
extern s32 chk12(u8 *object, HSD_JObj *target, f32 range);
extern f32 sColPosY(Vec3 *point, u16 *selector);
extern f32 sLenPoint3dR(Vec3 *point);
extern void HSD_LObjSetPosition(struct HSD_LObj *lobj, Vec3 *position);
extern u32 PSMTXInverse(MisMtx src, MisMtx inv);
extern void misProc(Missile3 *missile);
extern void msFlyTrace(Missile3 *missile);
extern s32 sAObjAnimeAll(HSD_JObj *joint);
extern s32 sDivLineVn(Vec3 *start, Vec3 *end, s32 axis, f32 value, Vec3 *out);
extern f32 sLenJobj(HSD_JObj *first, HSD_JObj *second);
extern f32 atan2f(f32 y, f32 x);
extern void sMissileZoid1Smoke(Missile3 *missile, MisZoid3 *zoid);
extern s32 sMissileAtk(Missile3 *missile, WeaponInverse3 *target);
extern f32 sLenPoint3d(Vec3 *first, Vec3 *second);
extern void sLinier3d(Vec3 *first, Vec3 *second, f32 distance);
extern void LoadArcAnimModel2(ModelArchive *archive, HSD_JObj *joint, s32 model, s32 animation);
extern f32 ZoidCalcAngle(MisZoid3 *zoid, Vec3 *point);
typedef struct MisCoObj {
    HSD_JObj *joint;
    u8 pad_04[0x34 - 0x04];
    f32 inverse[3][4];
} MisCoObj;
extern f32 sColCube4LineConnect(Vec3 *first, Vec3 *second);
extern void ZoidCoObjMove(void *object, Vec3 *move, f32 ratio);
extern HSD_JObj *grdObjGetJObj(s32 index);
extern void misProc00(Missile3 *missile, Vec3 *target);
extern void misProc01(Missile3 *missile, Vec3 *target);
extern void misProc02(Missile3 *missile, Vec3 *target);
extern void misProcAT20(Missile3 *missile, Vec3 *target);
extern void misProcAT50(Missile3 *missile, Vec3 *target);
#pragma force_active on
static ModelArchive *missileFile[130];
Missile3 missileStart[200];
#pragma force_active off
ModelArchive *sMissileLoad(s32 index, BulletDat *name)
{
    void *data;
    s32 size;
    ModelArchive *result;
    data = binB2(missBinB2, index, &size);
    if (data == 0) {
        return 0;
    }
    result = LoadArc2B(data, size, name);
    return result;
}
void sMissilePrologueU(s32 usb)
{
    s32 i;

    if (usbForce != 0) {
        usb = 1;
    }
    missBinB2 = sReadFileU("missile/am.b", usb);
    if (missBinB2 != 0) {
        ZeroMemory(missileStart, 79200);
        ZeroMemory(missileFile, 0x208);
        for (i = 0; i < 130; i++) {
            missileFile[i] = sMissileLoad(i, &Tea_bulletdat[i]);
        }
    }
}
#undef lit_385
#define lit_385 "missile/am.b"

void sMissilePrologue(void)
{
    sMissilePrologueU(0);
}

void sMissileEpilogue(void)
{
    s32 i;

    for (i = 0; i < 130; i++) {
        if (missileFile[i] != 0) {
            PurgeArc2(missileFile[i]);
        }
        missileFile[i] = 0;
    }
    if (missBinB2 != 0) {
        free(missBinB2);
    }
    missBinB2 = 0;
}
void misProc00(Missile3 *missile, Vec3 *target)
{
    Vec3 local;
    MisMtx rot;
    f32 angle;

    PSMTXMultVec(missile->inverse, target, &local);
    if ((missile->update_count & 0x20) && local.x < 0.0f) {
        PSMTXRotRad(rot, 'Z', 0.17453292f);
        PSMTXConcat(missile->matrix, rot, missile->matrix);
    } else {
        angle = 0.0f;
        if (local.z > 0.0f) {
            angle = -0.017453292f;
        }
        if (local.z < 0.0f) {
            angle = 0.017453292f;
        }
        if (0.0f != angle) {
            PSMTXRotRad(rot, 'Y', angle);
            PSMTXConcat(missile->matrix, rot, missile->matrix);
        }
        angle = 0.0f;
        if (local.y > 0.0f) {
            angle = 0.034906585f;
        }
        if (local.y < 0.0f) {
            angle = -0.034906585f;
        }
        if (0.0f != angle) {
            PSMTXRotRad(rot, 'Z', angle);
            PSMTXConcat(missile->matrix, rot, missile->matrix);
        }
    }
}
void misProc01(Missile3 *missile, Vec3 *target)
{
    Vec3 local;
    MisMtx rot;
    f32 angle;

    PSMTXMultVec(missile->inverse, target, &local);
    if ((missile->update_count & 0x10) && local.x < 0.0f) {
        PSMTXRotRad(rot, 'Z', 0.34906584f);
        PSMTXConcat(missile->matrix, rot, missile->matrix);
    } else {
        angle = 0.0f;
        if (local.z > 0.0f) {
            angle = -0.08726646f;
        }
        if (local.z < 0.0f) {
            angle = 0.08726646f;
        }
        if (0.0f != angle) {
            PSMTXRotRad(rot, 'Y', angle);
            PSMTXConcat(missile->matrix, rot, missile->matrix);
        }
        angle = 0.0f;
        if (local.y > 0.0f) {
            angle = -0.17453292f;
        }
        if (local.y < 0.0f) {
            angle = 0.17453292f;
        }
        if (0.0f != angle) {
            PSMTXRotRad(rot, 'X', angle);
            PSMTXConcat(missile->matrix, rot, missile->matrix);
        }
    }
}
void misProc02(Missile3 *missile, Vec3 *target)
{
    Vec3 local;
    MisMtx rot;
    f32 angle;
    s32 elapsed = missile->update_count - missile->proc_delay;

    PSMTXMultVec(missile->inverse, target, &local);
    if (elapsed % 15 == 0) {
        angle = 0.0f;
        if (local.z > 0.0f) {
            angle = -0.017453292f;
        }
        if (local.z < 0.0f) {
            angle = 0.017453292f;
        }
        if (0.0f != angle) {
            PSMTXRotRad(rot, 'Y', angle);
            PSMTXConcat(missile->matrix, rot, missile->matrix);
        }
    }
    angle = 0.0f;
    if (local.y > 0.0f) {
        angle = 0.0017453292f;
    }
    if (local.y < 0.0f) {
        angle = -0.0017453292f;
    }
    if (0.0f != angle) {
        PSMTXRotRad(rot, 'X', angle);
        PSMTXConcat(missile->matrix, rot, missile->matrix);
    }
}
#pragma dont_inline off
static inline f32 absf(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

void misProc03(Missile3 *missile, Vec3 *target)
{
    Vec3 local;
    MisMtx rot;
    f32 angle;
    s32 elapsed = missile->update_count - missile->proc_delay;

    PSMTXMultVec(missile->inverse, target, &local);
    if (elapsed < 60) {
        if (elapsed % 15 == 0 && local.x < 0.0f) {
            PSMTXRotRad(rot, 'Z', 0.34906584f);
            PSMTXConcat(missile->matrix, rot, missile->matrix);
        }
    } else {
        if (elapsed % 15 == 0) {
            angle = 0.0f;
            if (local.z > 0.0f) {
                angle = -0.34906584f;
            }
            if (local.z < 0.0f) {
                angle = 0.17453292f;
            }
            if (local.x > 100.0f && absf(angle) < 0.0017453292482296542) {
                angle = 2.0 * (3.1415926 * (f32)(HSD_Randi(10) - 5)) / 360.0;
            }
            if (0.0f != angle) {
                PSMTXRotRad(rot, 'Y', angle);
                PSMTXConcat(missile->matrix, rot, missile->matrix);
            }
        }
        if (elapsed % 15 == 10) {
            angle = 0.0f;
            if (local.y > 0.0f) {
                angle = 0.34906584f;
            }
            if (local.y < 0.0f) {
                angle = -0.17453292f;
            }
            if (0.0f != angle) {
                PSMTXRotRad(rot, 'Z', angle);
                PSMTXConcat(missile->matrix, rot, missile->matrix);
            }
        }
        PSMTXRotRad(rot, 'X', 0.17453292f);
        PSMTXConcat(missile->matrix, rot, missile->matrix);
    }
}
#pragma dont_inline on
void misProcAT20(Missile3 *missile, Vec3 *target)
{
    Vec3 local;
    MisMtx rot;
    f32 angle;

    PSMTXMultVec(missile->inverse, target, &local);
    angle = 0.0f;
    if (local.z > 0.0f) {
        angle = -missile->turn_rate;
    }
    if (0.0f != angle) {
        PSMTXRotRad(rot, 'Y', angle);
        PSMTXConcat(missile->matrix, rot, missile->matrix);
    }
    angle = 0.0f;
    if (local.y > 0.0f) {
        angle = -0.17453292f;
    }
    if (local.y < 0.0f) {
        angle = 0.17453292f;
    }
    if (0.0f != angle) {
        while (angle) {
            PSMTXRotRad(rot, 'X', angle);
            PSMTXConcat(missile->matrix, rot, missile->matrix);
            PSMTXMultVec(missile->inverse, target, &local);
            angle *= 0.5f;
            if (0.0f == local.y) {
                break;
            }
            if (local.y > 0.0f && angle > 0.0f) {
                angle = -angle;
            }
            if (local.y < 0.0f && angle < 0.0f) {
                angle = -angle;
            }
        }
    }
}
void misProcAT50(Missile3 *missile, Vec3 *target)
{
    Vec3 local;
    MisMtx rot;
    f32 angle;

    PSMTXMultVec(missile->inverse, target, &local);
    if ((missile->update_count & 0x20) && local.x < 0.0f) {
        PSMTXRotRad(rot, 'Z', 0.017453292f);
        PSMTXConcat(missile->matrix, rot, missile->matrix);
    } else {
        angle = 0.0f;
        if (local.z > 0.0f) {
            angle = -0.017453292f;
        }
        if (local.z < 0.0f) {
            angle = 0.017453292f;
        }
        if (0.0f != angle) {
            PSMTXRotRad(rot, 'Y', angle);
            PSMTXConcat(missile->matrix, rot, missile->matrix);
        }
        angle = 0.0f;
        if (local.y > 0.0f) {
            angle = 0.017453292f;
        }
        if (local.y < 0.0f) {
            angle = -0.017453292f;
        }
        if (0.0f != angle) {
            PSMTXRotRad(rot, 'Z', angle);
            PSMTXConcat(missile->matrix, rot, missile->matrix);
        }
    }
}

void misProcStraight(Missile3 *missile)
{
}
#pragma dont_inline off
static inline f32 sqrt4(f32 x, volatile f32 *y)
{
    f64 guess;
    if (x > 0.0f) {
        guess = __frsqrte(x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        *y = x * guess;
        return *y;
    }
    return x;
}

void misProcParabola(Missile3 *missile)
{
    MisMtx rot;
    Vec3 velocity;
    volatile f32 root;
    f32 angle;
    f32 previous;
    f32 current;
    f32 length;
    f32 offset;
    s32 i;

    if (missile->matrix[1][1] < 0.0f) {
        PSMTXRotRad(rot, 'X', 3.1415925f);
        PSMTXConcat(missile->matrix, rot, missile->matrix);
    } else if (0.0f != missile->matrix[1][2]) {
        angle = 0.5235988f;
        if (missile->matrix[1][2] < 0.0f)
            angle = -angle;
        while (angle && missile->matrix[1][2]) {
            if (missile->matrix[1][2] < 0.0f) {
                while (missile->matrix[1][2] < 0.0f) {
                    PSMTXRotRad(rot, 'X', angle);
                    PSMTXConcat(missile->matrix, rot, missile->matrix);
                }
                if (missile->matrix[1][2] > 0.0f)
                    angle = -(angle / 2.0f);
            } else if (missile->matrix[1][2] > 0.0f) {
                while (missile->matrix[1][2] > 0.0f) {
                    PSMTXRotRad(rot, 'X', angle);
                    PSMTXConcat(missile->matrix, rot, missile->matrix);
                }
                if (missile->matrix[1][2] < 0.0f)
                    angle = -(angle / 2.0f);
            }
        }
    }
    for (i = 0; i < 3; i++) {
        (&velocity.x)[i] = missile->matrix[i][0] * missile->speed;
    }
    previous = velocity.y;
    velocity.y = previous - 0.025;
    current = velocity.y;
    missile->speed = sLenPoint3dR(&velocity);
    if (missile->speed > 0.1) {
        length = sqrt4(velocity.x * velocity.x + velocity.z * velocity.z, &root);
        if (length > 0.1) {
            offset = atan2f(length, current);
            angle = atan2f(length, previous) - offset;
            if (angle) {
                PSMTXRotRad(rot, 'Z', angle);
                PSMTXConcat(missile->matrix, rot, missile->matrix);
            }
        } else {
            PSMTXRotRad(rot, 'Z', 0.034906585f);
            PSMTXConcat(missile->matrix, rot, missile->matrix);
        }
    }
}
#pragma dont_inline on

void misProcFloat(Missile3 *missile)
{
}
void misProcGround(Missile3 *missile)
{
    Vec3 position;
    Vec3 local;
    u16 polygon_index;
    s32 normal_index;
    MisMtx rot;
    s32 i;
    Vec3 *normal;
    f32 angle;
    u8 unused[4];

    if (missile->hit_ground == 0) {
        misProcParabola(missile);
        return;
    }
    missile->speed = 0.0f;
    for (i = 0; i < 3; i++) {
        (&position.x)[i] = missile->matrix[i][3];
    }
    sColPosY(&position, &polygon_index);
    if (polygon_index == GROUND_POLYGON_NONE)
        return;
    normal_index = grdcPolygon[polygon_index].normal_index;
    normal = &grdcNormal[normal_index];
    if (normal == 0)
        return;
    for (i = 0; i < 3; i++) {
        (&position.x)[i] -= (&normal->x)[i];
    }

    PSMTXMultVec(missile->inverse, &position, &local);
    angle = 0.0f;
    if (local.z > 0.017453292222222222) {
        angle = -0.017453292f;
    }
    if (local.z < -0.017453292222222222) {
        angle = 0.017453292f;
    }
    if (angle) {
        PSMTXRotRad(rot, 'Y', angle);
        PSMTXConcat(missile->matrix, rot, missile->matrix);
    }
    angle = 0.0f;
    if (local.y > 0.017453292222222222) {
        angle = 0.017453292f;
    }
    if (local.y < -0.017453292222222222) {
        angle = -0.017453292f;
    }
    if (angle) {
        PSMTXRotRad(rot, 'Z', angle);
        PSMTXConcat(missile->matrix, rot, missile->matrix);
    }
}
void misProc(Missile3 *missile)
{
    MisZoid3 *zoid;
    Missile3 *target_missile;
    HSD_JObj *jobj;
    f32 (*mtx)[4];
    Vec3 target_position;
    s32 i;

    switch (missile->proc) {
    case 6:
        misProcStraight(missile);
        break;
    case 7:
        misProcParabola(missile);
        break;
    case 8:
        misProcFloat(missile);
        break;
    case 9:
        misProcGround(missile);
        break;
    default:
        zoid = missile->target_zoid;
        jobj = 0;
        if (zoid != 0) {
            jobj = zoid->camera_joint;
        } else {
            target_missile = missile->target_missile;
            if (target_missile != 0) {
                jobj = target_missile->joint;
            }
        }
        if (jobj == 0) {
            jobj = grdObjGetJObj(missile->lock);
        }
        if (jobj == 0)
            return;
        mtx = jobj->matrix;
        if (mtx == 0)
            return;
        for (i = 0; i < 3; i++) {
            (&target_position.x)[i] = mtx[i][3];
        }
        switch (missile->proc) {
        case 0:
            misProc00(missile, &target_position);
            break;
        case 1:
            misProc01(missile, &target_position);
            break;
        case 2:
            misProc02(missile, &target_position);
            break;
        case 3:
            misProc03(missile, &target_position);
            break;
        case 5:
            misProcAT50(missile, &target_position);
            break;
        case 4:
            misProcAT20(missile, &target_position);
            break;
        }
        break;
    }
}
#pragma dont_inline off
#pragma push
#pragma auto_inline off
static inline f32 HSD_AObjGetCurrFrame(HSD_AObj *aobj)
{
    aobj ? (void)0 : __assert("aobj.h", 149, "aobj");
    return aobj->current_frame;
}

s32 sAObjAnimeAll(HSD_JObj *joint)
{
    HSD_AObj *aobj;
    f32 frame;
    u32 flags;

    if (joint == (void *)0)
        return 0;
    aobj = getAnimAobj(joint);
    if (aobj == (void *)0)
        return 0;
    frame = HSD_AObjGetCurrFrame(aobj);
    HSD_JObjAnimAll(joint);
    flags = HSD_AObjGetFlags(aobj);
    if (!(flags & AOBJ_REWINDED))
        return 0;
    setAnimFrame(joint, frame);
    setAnimRate(joint, 0.0f);
    return 1;
}
#pragma pop
#pragma dont_inline on
#undef lit_662
#define lit_662 "aobj.h"
#undef lit_663
#define lit_663 "aobj"
void sMissileSetAnim(Missile3 *missile, s32 selected)
{
    ModelAnimationTable *table;

    if (missile == (void *)0)
        return;
    if (missile->joint == (void *)0)
        return;
    table = missile->archive->animations;
    if (table->models == (void *)0)
        return;
    if (table->models[0]->joint_animations == (void *)0)
        return;
    HSD_JObjRemoveAnimAll(missile->joint);
    HSD_JObjAddAnimAll(missile->joint,
        table->models[0]->joint_animations != 0 ? table->models[0]->joint_animations[selected] : 0,
        table->models[0]->material_animations != 0 ? table->models[0]->material_animations[selected] : 0,
        table->models[0]->shape_animations != 0 ? table->models[0]->shape_animations[selected] : 0);
    HSD_JObjReqAnimAll(missile->joint, 0.0f);
    missile->animation = selected;
    setAnimRate(missile->joint, 1.0f);
}
#pragma force_active on
void *missBinB2;
s32 misALL;
s32 nowMJ;
#pragma force_active off
#pragma dont_inline off
#pragma auto_inline off
void sMissileSet(MisZoid3 *owner, MisZoid3 *target_zoid, Missile3 *target_missile, s32 lock, MisMtx mtx, s32 bullet, s32 extra_damage)
{
    static s32 cc;
    Missile3 *missile;
    ModelArchive *archive;
    s32 i;
    s32 j;
    s32 k;
    s32 l;
    s32 m;
    s32 se;
    MisMtx rot;

    if (bullet <= 0)
        return;
    if (bullet >= 130)
        return;
    archive = missileFile[bullet];
    if (archive == (void *)0)
        return;
    if (archive->model_data == (void *)0)
        return;
    if (archive->models == (void *)0)
        return;
    nowMJ++;
    for (i = 0; i < 200; i++) {
        if (nowMJ >= 200)
            nowMJ = 0;
        missile = &missileStart[nowMJ];
        if (!(missile->flags & 1))
            break;
        nowMJ++;
    }
    if (i == 200)
        return;
    if (missile->flags & 1)
        return;
    misALL++;
    ZeroMemory(missile, sizeof(Missile3));
    missile->hit_ground = 0;
    missile->bullet = bullet;
    missile->kind = Tea_bulletdat[bullet].kind;
    missile->zoid_hit = Tea_bulletdat[bullet].zoid_hit;
    missile->archive = archive;
    missile->joint = CopyJoint2(archive, 0);
    missile->beam_joints[0] = searchJobjJoint(archive, missile->joint, "beem_joint");
    if (missile->beam_joints[0] != 0) {
        missile->beam_joints[1] = searchJobjJoint(archive, missile->joint, "to_beem_joint");
        missile->beam_joints[2] = searchJobjJoint(archive, missile->joint, "to_to_beem_joint");
        missile->beam_joints[3] = searchJobjJoint(archive, missile->joint, "to_to_to_beem_joint");
        missile->beam_joints[4] = searchJobjJoint(archive, missile->joint, "to_to_to_to_beem_joint");
        missile->beam_joints[5] = searchJobjJoint(archive, missile->joint, "to_to_to_to_to_beem_joint");
        missile->beam_joints[6] = searchJobjJoint(archive, missile->joint, "to_to_to_to_to_to_beem_joint");
    }
    missile->a1_joint = searchJobjJoint(archive, missile->joint, "a_1_joint");
    missile->owner = owner;
    if (owner != 0)
        missile->owner_weapon_mount = owner->weapon_mount;
    missile->target_zoid = target_zoid;
    missile->target_missile = target_missile;
    missile->lock = lock;
    missile->speed = 0.0f;
    missile->update_count = 0;
    missile->purge_timer = 0;
    missile->hit = 0;
    missile->flags = 1;
    for (j = 0; j < 3; j++) {
        for (k = 0; k < 4; k++)
            missile->matrix[j][k] = mtx[j][k];
    }
    PSMTXRotRad(rot, 'X', 3.1415926 * (2.0f * HSD_Randf()));
    PSMTXConcat(missile->matrix, rot, missile->matrix);
    for (l = 0; l < 3; l++) {
        for (m = 0; m < 4; m++)
            missile->joint->matrix[l][m] = missile->matrix[l][m];
    }
    sGlobal2Local(missile->joint);
    HSD_JObjSetTranslateX(missile->joint, mtx[0][3]);
    HSD_JObjSetTranslateY(missile->joint, mtx[1][3]);
    HSD_JObjSetTranslateZ(missile->joint, mtx[2][3]);
    sMakeMatrix(missile->joint);
    for (i = 0; i < 7; i++) {
        if (missile->beam_joints[i] != 0) {
            for (j = 0; j < 3; j++) {
                for (k = 0; k < 3; k++)
                    missile->beam_joints[i]->matrix[j][k] = mtx[j][k];
                missile->beam_joints[i]->matrix[j][3] = 0.1 * i + mtx[j][3];
                missile->beam_positions[i][j] = 0.1 * i + mtx[j][3];
            }
        }
    }
    for (i = 0; i < 7; i++) {
        if (missile->beam_joints[i] != 0) {
            HSD_JObjSetScaleX(missile->beam_joints[i], 1.0f);
            sGlobal2Local(missile->beam_joints[i]);
        }
    }
    sMakeMatrix(missile->joint);
    missile->accel = 1.0f;
    missile->turn_rate = (f32)((f32)Tea_bulletdat[bullet].turn_angle / 60.0) * 3.1415926 * 2.0 / 360.0;
    missile->max_speed = 0.1 * Tea_bulletdat[bullet].speed;
    {
        s32 procs[6] = { 6, 4, 7, 8, 9, 4 };
        missile->proc = procs[Tea_bulletdat[bullet].kind];
    }
    missile->proc_delay = 60;
    if (missile->proc == 7) {
        missile->speed = missile->max_speed;
        missile->proc_delay = 0;
    }
    if (missile->kind == 4) {
        missile->speed = missile->max_speed;
        missile->proc_delay = 0;
        if (missile->speed < 1.0f)
            missile->speed = 1.0f;
    }
    if (owner != 0 && owner->ai != 0)
        owner->ai->missile_hit = 0;
    missile->damage = extra_damage + Tea_bulletdat[bullet].damage;
    sMissileSetAnim(missile, 0);
    sEffectSet(Tea_bulletdat[missile->bullet].fire_effect, 1, 0, missile->joint, 0);
    {
        s32 se_index = Tea_bulletdat[bullet].se;
        se = -1;
        if (se_index >= 0)
            se = SE_TableList[se_index];
    }
    missile->se = se;
    if (sLightMis != 0) {
        missile->light = LoadArc2Light(sEfctLight, cc + 1);
        cc++;
        if (cc >= 3)
            cc = 0;
    }
}
#pragma dont_inline on
#undef lit_790
#define lit_790 "beem_joint"
#undef lit_791
#define lit_791 "to_beem_joint"
#undef lit_792
#define lit_792 "to_to_beem_joint"
#undef lit_793
#define lit_793 "to_to_to_beem_joint"
#undef lit_794
#define lit_794 "to_to_to_to_beem_joint"
#undef lit_795
#define lit_795 "to_to_to_to_to_beem_joint"
#undef lit_796
#define lit_796 "to_to_to_to_to_to_beem_joint"
#undef lit_797
#define lit_797 "a_1_joint"
#undef lit_799
#define lit_799 "jobj.h"
#undef lit_800
#define lit_800 "jobj"
#pragma dont_inline off
static inline f32 sqrt3(f32 x, volatile f32 *y)
{
    f64 guess;
    if (x > 0.0f) {
        guess = __frsqrte(x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        *y = x * guess;
        return *y;
    }
    return x;
}

void sMissileFire(MisZoid3 *owner, MisZoid3 *target_zoid, Missile3 *target_missile, s32 lock, MisMtx matrix, s32 bullet, s32 extra_damage)
{
    f32 copy[3][4];
    s32 i;
    s32 j;
    f32 length;
    volatile f32 root;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++)
            copy[i][j] = matrix[i][j];
    }
    length = sqrt3(matrix[1][0] * matrix[1][0] + matrix[0][0] * matrix[0][0] + matrix[2][0] * matrix[2][0], &root);
    for (i = 0; i < 3; i++)
        copy[i][0] = matrix[i][0] / length;
    sMissileSet(owner, target_zoid, target_missile, lock, copy, bullet, extra_damage);
}
#pragma dont_inline on
#pragma dont_inline off
static inline s32 ref_DEC(HSD_Obj *o)
{
    s32 ret;

    ret = (u64)(o->reference_count == (u16)0xFFFF);
    if (ret != 0) {
        return ret;
    }
    ret = (o->reference_count == (u16)0);
    o->reference_count--;
    return ret;
}

void sMissilePurge(Missile3 *missile)
{
    HSD_Obj *obj;

    if (missile != (void *)0 && missile->flags != 0) {
        misALL--;
        if (missile->joint != 0) {
            setAnimRate(missile->joint, 0.0f);
            HSD_JObjRemoveAnimAll(missile->joint);
            jobjUnrefAll(missile->joint);
            missile->joint = 0;
        }
        if (missile->se_playing != 0) {
            missile->se_playing = 0;
        }
        if (missile->light != 0) {
            obj = (HSD_Obj *)missile->light;
            if (obj != 0) {
                if (ref_DEC(obj)) {
                    if (obj != 0) {
                        ((HSD_Class *)obj)->class_info->release((HSD_Class *)obj);
                        ((HSD_Class *)obj)->class_info->destroy((HSD_Class *)obj);
                    }
                }
            }
        }
        missile->light = 0;
        missile->flags = 0;
    }
}
#pragma dont_inline on

void sMissilePurgeAll(void)
{
    s32 i;
    Missile3 *missile = missileStart;
    for (i = 0; i < 200; i++, missile++) {
        sMissilePurge(missile);
    }
}

void msFlyTrace(Missile3 *missile)
{
    s32 bullet;
    f32 rate;
    s32 i;
    s32 k;
    s32 n;
    s32 j;

    if (missile == (void *)0)
        return;
    if (missile->purge_timer != 0)
        return;
    if (missile->beam_joints[0] == 0)
        return;
    bullet = missile->bullet;
    rate = (f32)Tea_bulletdat[bullet].trace_rate * 0.01;
    for (i = 6; i > 0; i--) {
        if (missile->beam_joints[i] != (void *)0 && missile->beam_joints[i - 1] != (void *)0) {
            for (k = 0; k < 3; k++) {
                missile->beam_positions[i][k] = sLiner(missile->beam_positions[i - 1][k], missile->beam_positions[i][k], rate);
                missile->beam_joints[i]->matrix[k][3] = missile->beam_positions[i][k];
            }
            for (n = 0; n < 3; n++) {
                for (j = 0; j < 3; j++)
                    missile->beam_joints[i]->matrix[n][j] = missile->beam_joints[i - 1]->matrix[n][j];
            }
            sGlobal2Local(missile->beam_joints[i]);
        }
    }
}
void sHitOn(Missile3 *missile)
{
    s32 animation;

    if (missile != (void *)0) {
        missile->update_count++;
        if (missile->purge_timer == 0 && missile->hit != 0) {
            missile->hit++;
            if (sAObjAnimeAll(missile->joint)) {
                missile->purge_timer = 1;
                animation = missile->animation;
                if (animation == 0) {
                    sMissileSetAnim(missile, 1);
                }
                if (animation == 1) {
                    sMissileSetAnim(missile, 2);
                }
                if (animation == 2) {
                    sMissileSetAnim(missile, 2);
                }
            }
            if (missile->se_playing != 0) {
                missile->se_playing = 0;
            }
            msFlyTrace(missile);
        }
    }
}
#pragma dont_inline off

void sMove(Missile3 *missile)
{
    f32 step[3];
    Vec3 position;
    s32 k;
    s32 i;
    s32 j;
    register s32 m;

    if (missile == (void *)0)
        return;
    missile->update_count++;
    if (missile->purge_timer != 0)
        return;
    if (missile->hit != 0)
        return;
    if (missile->se) {
        (void)0;
    }
    for (k = 0; k < 3; k++) {
        for (j = 0; j < 4; j++) {
            missile->previous[k][j] = missile->matrix[k][j];
        }
    }
    if (sAObjAnimeAll(missile->joint) != 0) {
        if (missile->animation == 0) {
            sMissileSetAnim(missile, 1);
            missile->hit = 1;
        } else {
            missile->purge_timer = 1;
            sMissileSetAnim(missile, 2);
        }
    }
    if (missile->kind != 4 && missile->hit_ground != 0)
        missile->speed = 0.0f;
    for (k = 0; k < 3; k++) {
        step[k] = missile->matrix[k][0] * missile->speed;
        missile->matrix[k][3] += step[k];
    }
    if (missile->proc != 7 && missile->kind != 4) {
        missile->speed += missile->accel;
        if (missile->speed > missile->max_speed)
            missile->speed = missile->max_speed;
    }
    PSMTXInverse(missile->matrix, missile->inverse);
    if (missile->proc_delay < missile->update_count)
        misProc(missile);
    HSD_JObjSetTranslateX(missile->joint, missile->matrix[0][3]);
    HSD_JObjSetTranslateY(missile->joint, missile->matrix[1][3]);
    HSD_JObjSetTranslateZ(missile->joint, missile->matrix[2][3]);
    sMakeMatrix(missile->joint);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            missile->joint->matrix[i][j] = missile->matrix[i][j];
        }
    }
    sGlobal2Local(missile->joint);
    if (missile->beam_joints[0] != 0) {
        for (m = 0; m < 3; m++) {
            missile->beam_positions[0][m] = missile->matrix[m][3];
        }
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 4; j++) {
                missile->beam_joints[0]->matrix[i][j] = missile->joint->matrix[i][j];
            }
        }
        sGlobal2Local(missile->beam_joints[0]);
        msFlyTrace(missile);
        for (k = 1; k < 7; k++) {
            if (missile->beam_joints[k] == (void *)0)
                break;
        }
    }
    if (missile->light != 0) {
        position.x = missile->matrix[0][3];
        position.y = missile->matrix[1][3];
        position.z = missile->matrix[2][3];
        HSD_LObjSetPosition(missile->light, &position);
    }
}
#pragma dont_inline on
#pragma dont_inline off
void sAtack(Missile3 *missile)
{
    s32 i;
    Vec3 current;
    Vec3 previous;
    Vec3 next;
    Vec3 hit;
    GroundMask mask;
    f32 distance;
    f32 length;

    if (missile == (void *)0)
        return;
    missile->update_count++;
    if (missile->purge_timer != 0)
        return;
    if (missile->hit != 0)
        return;
    if (missile->kind == 4 && missile->update_count < 3)
        return;
    for (i = 0; i < 3; i++) {
        (&current.x)[i] = missile->matrix[i][3];
        (&previous.x)[i] = missile->previous[i][3];
    }
    length = sLenPoint3d(&current, &previous);
    missile->step_length = length;
    for (i = 0; i < 3; i++) {
        (&next.x)[i] = missile->matrix[i][3];
        (&hit.x)[i] = missile->previous[i][3];
    }
    mask = (GroundMask){{255, 0, 0, 0}};
    distance = sColPt(&hit, &next, &mask);
    if (GROUND_NO_HIT == distance)
        return;
    missile->hit_ground = 1;
    if (missile->kind != 4)
        missile->hit = 1;
    sLinier3d(&hit, &next, distance);
    for (i = 0; i < 3; i++) {
        missile->matrix[i][3] = (&hit.x)[i];
        missile->joint->matrix[i][3] = (&hit.x)[i];
    }
    HSD_JObjSetScaleX(missile->joint, 1.0f);
    sGlobal2Local(missile->joint);
    sMakeMatrix(missile->joint);
    if (missile->kind != 4) {
        sMissileSetAnim(missile, 1);
        LoadArcAnimModel2(missile->archive, missile->joint, 0, 1);
        sEffectSet(Tea_bulletdat[missile->bullet].hit_effect, 1, 0, missile->joint, 0);
        return;
    }
    missile->speed = 0.0f;
}
#pragma dont_inline on
#undef lit_1047
#define lit_1047 "F\x1c@"

void sMissileMove(void)
{
    s32 i;
    Missile3 *missile = missileStart;

    for (i = 0; i < 200; i++, missile++) {
        if (missile->flags & 1) {
            sMove(missile);
            sHitOn(missile);
            sAtack(missile);
        }
    }
    missile = missileStart;
    for (i = 0; i < 200; i++, missile++) {
        if ((missile->flags & 1) && missile->purge_timer != 0) {
            missile->purge_timer++;
            if (missile->purge_timer > 5) {
                sMissilePurge(missile);
            }
        }
    }
}
void sMissileDisp(void)
{
    Missile3 *missile;
    s32 i;

    nowMX = 0;
    missile = missileStart;
    for (i = 0; i < 200; i++, missile++) {
        if ((missile->flags & 1) && missile->purge_timer == 0 && missile->joint != (void *)0) {
            nowMX++;
            HSD_JObjDispAll(missile->joint, 0, 7, 0);
        }
    }
    if (nowMX > misMAX) {
        misMAX = nowMX;
    }
}
f32 sMissileZoid1a(Missile3 *missile, MisZoid3 *zoid, void *object, s32 push)
{
    f32 (*inverse)[4];
    Vec3 current;
    Vec3 previous;
    Vec3 move;
    f32 t;
    f32 ratio;
    f32 dot;
    s32 i;

    if (object == (void *)0)
        return -1.0f;
    if (((MisCoObj *)object)->joint == (void *)0)
        return -1.0f;
    ratio = 0.0f;
    if (zoid != 0 && zoid->joint != 0 && missile != 0) {
        if (zoid->armor < 1)
            zoid->armor = 1;
        ratio = 10.0 * (f32)missile->damage / (f32)zoid->armor;
    }
    inverse = ((MisCoObj *)object)->inverse;
    for (i = 0; i < 3; i++) {
        ((f32 *)&current)[i] = missile->matrix[i][3];
        ((f32 *)&previous)[i] = missile->previous[i][3];
    }
    PSMTXMultVec(inverse, &current, &current);
    PSMTXMultVec(inverse, &previous, &previous);
    t = sColCube4LineConnect(&previous, &current);
    if (-1.0f != t && push != 0) {
        dot = 0.0f;
        for (i = 0; i < 3; i++) {
            ((f32 *)&move)[i] = 2.0f * missile->matrix[i][0];
            dot += ((f32 *)&move)[i] * ((f32 *)&move)[i];
        }
        ZoidCoObjMove(object, &move, ratio);
    }
    return t;
}
s32 sMissileAtk(Missile3 *missile, WeaponInverse3 *target)
{
    Vec3 end;
    Vec3 start;
    f32 t;
    f32 delta;
    f32 (*inverse)[4];
    s32 i;

    if (target == (void *)0)
        return 0;
    if (target->joint == (void *)0)
        return 0;
    inverse = target->inverse;
    for (i = 0; i < 3; i++) {
        (&end.x)[i] = missile->matrix[i][3];
        (&start.x)[i] = missile->previous[i][3];
    }
    PSMTXMultVec(inverse, &end, &end);
    PSMTXMultVec(inverse, &start, &start);
    t = sColCube4LineConnect(&start, &end);
    if (-1.0f == t)
        return 0;
    for (i = 0; i < 3; i++) {
        delta = missile->matrix[i][3] - missile->previous[i][3];
        missile->matrix[i][3] = delta * t + missile->previous[i][3];
    }

    if (missile->hit == 0) {
        missile->hit = 1;
        sMissileSetAnim(missile, 1);
        sEffectSet(Tea_bulletdat[missile->bullet].hit_effect, 1, 0, missile->joint, 0);
    }
    return 1;
}
s32 sMissileZoid1(Missile3 *missile, MisZoid3 *zoid)
{
    f32 t;
    f32 delta;
    f32 rate;
    s32 damage;
    s32 health;
    s32 i;
    MisZoid3 *owner;

    if (zoid == (void *)0)
        return 0;
    if (zoid->joint == (void *)0)
        return 0;
    if (missile == (void *)0)
        return 0;
    if (missile->update_count < 2)
        return 0;
    if (zoid == missile->owner && missile->update_count < 180)
        return 0;
    t = sMissileZoid1a(missile, zoid, zoid->body_coobj, 0);
    if (-1.0f == t)
        return 0;
    t = sMissileZoid1a(missile, zoid, zoid->bust_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->bust_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->neck_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->hip_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->tail_coobjs[0], 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->tail_coobjs[1], 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->tail_coobjs[2], 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->head_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->l_arm1_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->r_arm1_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->l_arm2_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->r_arm2_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->l_leg1_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->r_leg1_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->l_leg2_coobj, 1);
    if (-1.0f == t)
        t = sMissileZoid1a(missile, zoid, zoid->r_leg2_coobj, 1);
    if (-1.0f == t)
        return 0;
    for (i = 0; i < 3; i++) {
        delta = missile->matrix[i][3] - missile->previous[i][3];
        missile->matrix[i][3] = delta * t + missile->previous[i][3];
    }
    if (missile->hit == 0) {
        missile->hit = 1;
        sMissileSetAnim(missile, 1);
        sEffectSet(Tea_bulletdat[missile->bullet].hit_effect, 1, 0, missile->joint, 0);
    }
    if (missile->owner != 0 && missile->owner->ai != 0) {
        missile->owner->ai->missile_hit = missile->owner->slot == zoid->slot ? 3 : 1;
    }
    if (missile->owner != 0 && missile->owner != zoid && (u8)zoid->slot == missile->owner->slot && (u8)zoid->slot == Zoid[0].slot && zoid->health != 0)
        zdVoiceSc(zoid, 14);
    if (zoid->field_1E50 == 0) {
        ZoidCopyAnim(zoid);
        health = zoid->health;
        damage = missile->damage;
        damage = hitCalc2(missile->owner, damage, zoid);
        zoid->interpolation_remaining = 13;
        zoid->interpolation_duration = 15;
        for (i = 0; i < 3; i++) {
            zoid->knockback[i] += missile->matrix[i][0] * damage / 300.0f;
        }
        zoid->health -= damage;
        rate = (f32)damage / 100.0;
        if (rate > 1.0f)
            rate = 1.0f;
        if (zoid->field_20C0 != 0)
            rate = 46.0f * rate / zoid->field_20C0;
        if (rate > 1.0f)
            rate = 1.0f;
        zoid->position[0] += missile->matrix[0][0] * rate;
        zoid->position[2] += missile->matrix[2][0] * rate;
        if (damage > 0)
            zoid->health_bar_timer = 60;
        if (zoid->health < 0)
            zoid->health = 0;
        if (health != 0 && zoid->health == 0) {
            owner = missile->owner;
            if (owner != 0 && zoid->slot != owner->slot) {
                owner->kills++;
                owner->field_1EF0 += zoid->max_health;
                if (missile->owner_weapon_mount == 19)
                    owner->field_1EF4 += zoid->max_health;
            }
        }
        zoid->rumble_timer = 15;
    }
    return 1;
}

void sMissileMis1(Missile3 *missile, Missile3 *other)
{
    Vec3 end;
    Vec3 start;
    f32 radius;
    f32 dist;
    f32 (*inverse)[4];
    s32 i;

    if (other == (void *)0)
        return;
    if (other->joint == (void *)0)
        return;
    if (other->purge_timer != 0)
        return;
    radius = missile->step_length;
    inverse = missile->inverse;
    dist = sLenJobj(missile->joint, other->joint);
    if (dist > radius + other->step_length)
        return;
    for (i = 0; i < 3; i++) {
        (&end.x)[i] = other->matrix[i][3];
        (&start.x)[i] = other->previous[i][3];
    }
    PSMTXMultVec(inverse, &end, &end);
    PSMTXMultVec(inverse, &start, &start);
    if (start.x < -(0.5 + radius) && end.x < -(0.5 + radius))
        return;
    if (start.x > 0.5&& end.x > 0.5)
        return;
    if (start.x < -(0.5 + radius) && sDivLineVn(&start, &end, 0, -(0.5 + radius), &start) == 0)
        return;
    if (end.x < -(0.5 + radius) && sDivLineVn(&start, &end, 0, -(0.5 + radius), &end) == 0)
        return;
    if (start.x > 0.5&& sDivLineVn(&start, &end, 0, 0.5f, &start) == 0)
        return;
    if (end.x > 0.5&& sDivLineVn(&start, &end, 0, 0.5f, &end) == 0)
        return;
    if (start.z < -0.5 && end.z < -0.5)
        return;
    if (start.z > 0.5&& end.z > 0.5)
        return;
    if (start.z < -0.5 && sDivLineVn(&start, &end, 2, -0.5f, &start) == 0)
        return;
    if (end.z < -0.5 && sDivLineVn(&start, &end, 2, -0.5f, &end) == 0)
        return;
    if (start.z > 0.5&& sDivLineVn(&start, &end, 2, 0.5f, &start) == 0)
        return;
    if (end.z > 0.5&& sDivLineVn(&start, &end, 2, 0.5f, &end) == 0)
        return;
    if (start.x < -(0.5 + radius) && end.x < -(0.5 + radius))
        return;
    if (start.x > 0.5&& end.x > 0.5)
        return;
    if (start.y < -0.5 && end.y < -0.5)
        return;
    if (start.y > 0.5&& end.y > 0.5)
        return;
    if (start.z < -0.5 && end.z < -0.5)
        return;
    if (start.z > 0.5&& end.z > 0.5)
        return;
    if (missile->hit == 0) {
        missile->hit = 1;
        sMissileSetAnim(missile, 1);
        sEffectSet(Tea_bulletdat[missile->bullet].hit_effect, 1, 0, missile->joint, 0);
    }
    if (other->hit == 0) {
        other->hit = 1;
        sMissileSetAnim(other, 1);
        sEffectSet(Tea_bulletdat[other->bullet].hit_effect, 1, 0, other->joint, 0);
    }
}
void sMissileZoid1Smoke(Missile3 *missile, MisZoid3 *zoid)
{
    s32 slot;
    HSD_JObj *a1_joint;
    MisZoid3 *owner;
    HSD_JObj *camera_joint;
    f32 scale;
    f32 length;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    camera_joint = zoid->camera_joint;
    if (camera_joint == 0)
        return;
    if (missile == (void *)0)
        return;
    a1_joint = missile->a1_joint;
    owner = missile->owner;
    if (a1_joint == (void *)0)
        return;
    slot = -1;
    if (owner != 0)
        slot = owner->slot;
    if (missile->update_count < 2)
        return;
    if (zoid == missile->owner && missile->update_count < 180)
        return;
    a1_joint ? (void)0 : __assert("jobj.h", 899, "jobj");
    scale = a1_joint->scale.x;
    if (scale < 0.8)
        return;
    length = sLenJobj(a1_joint, camera_joint);
    if (2.0f * length > scale)
        return;
    if (zoid->slot == slot)
        zoid->ally_smoke_timer = 60;
    else
        zoid->enemy_smoke_timer = 180;
}
s32 chk12(u8 *object, HSD_JObj *target, f32 range)
{
    HSD_JObj *joint;
    f32 length;
    if (object == 0)
        return 0;
    joint = *(HSD_JObj **)object;
    if (joint == 0)
        return 0;
    if (target == 0)
        return 0;
    length = sLenJobj(joint, target);
    if (length > range + *(f32 *)(object + 8))
        return 0;
    return 1;
}
#pragma dont_inline off

void sMissileZoid1Mine(Missile3 *missile, MisZoid3 *zoid)
{
    MisZoid3 *owner;
    HSD_JObj *a1_joint;
    HSD_JObj *camera_joint;
    s32 hit_anim;
    s32 health;
    f32 range;
    register f32 dist;
    register f32 delta;
    Vec3 position;
    MisZoid3 *killer;
    s32 i;

    if (zoid == (void *)0)
        return;
    if (zoid->joint == (void *)0)
        return;
    camera_joint = zoid->camera_joint;
    if (camera_joint == 0)
        return;
    if (missile == (void *)0)
        return;
    a1_joint = missile->a1_joint;
    owner = missile->owner;
    if (a1_joint == (void *)0)
        return;
    if (owner) {
        (void)0;
    }
    if (missile->update_count < 120)
        return;
    if ((s32)missile->hit_zoids[zoid->index] == 1)
        return;
    range = HSD_JObjGetScaleX(a1_joint);
    if (range < 0.2)
        return;
    for (i = 0; i < 3; i++) {
        (&position.x)[i] = camera_joint->matrix[i][3];
    }
    position.y = sColPosY(&position, 0);
    dist = 0.0f;
    for (i = 0; i < 3; i++) {
        delta = (&position.x)[i] - a1_joint->matrix[i][3];
        dist += delta * delta;
    }
    range *= 0.5f;
    {
        s32 hit_parts = 0;
        if (chk12(zoid->l_arm2_coobj, a1_joint, range))
            hit_parts |= 1;
        if (chk12(zoid->l_arm1_coobj, a1_joint, range))
            hit_parts |= 2;
        if (chk12(zoid->r_arm2_coobj, a1_joint, range))
            hit_parts |= 4;
        if (chk12(zoid->r_arm1_coobj, a1_joint, range))
            hit_parts |= 8;
        if (chk12(zoid->l_leg2_coobj, a1_joint, range))
            hit_parts |= 16;
        if (chk12(zoid->l_leg1_coobj, a1_joint, range))
            hit_parts |= 32;
        if (chk12(zoid->r_leg2_coobj, a1_joint, range))
            hit_parts |= 64;
        if (chk12(zoid->r_leg1_coobj, a1_joint, range))
            hit_parts |= 128;
        if (chk12(zoid->hip_coobj, a1_joint, range))
            hit_parts |= 128;
        if (hit_parts == 0)
            return;
        missile->hit_zoids[zoid->index] = 1;
        if (missile->hit == 0) {
            missile->hit = 1;
            {
                hit_anim = 1;
                sMissileSetAnim(missile, hit_anim);
            }
        }
        if (zoid->field_1E50 == 0) {
            ZoidCopyAnim(zoid);
            zoid->interpolation_remaining = 13;
            zoid->interpolation_duration = 15;
            for (i = 0; i < 3; i++) {
                (&zoid->knockback[0])[i] = camera_joint->matrix[i][3] - missile->matrix[i][0];
            }
            dist = sLenPoint3dR((Vec3 *)&zoid->knockback[0]);
            if (dist > 0.2) {
                for (i = 0; i < 3; i++) {
                }
            }
            health = zoid->health;
            zoid->health -= missile->damage;
            if (zoid->health < 0)
                zoid->health = 0;
            if (health != 0 && zoid->health == 0) {
                killer = missile->owner;
                if (killer != 0 && zoid->slot != killer->slot) {
                    killer->kills++;
                    killer->field_1EF0 += zoid->max_health;
                }
            }
            zoid->health_bar_timer = 60;
            zoid->rumble_timer = 15;
        }
        sEffectSet(Tea_bulletdat[missile->bullet].hit_effect, 1, 0, missile->joint, 0);
    }
}
#pragma dont_inline on
#pragma dont_inline off
void sMissileZoid(void)
{
    Missile3 *missile = missileStart;
    s32 i;
    s32 j;
    s32 k;
    WeaponState3 *weapon;
    MisZoid3 *owner;
    HSD_JObj *joint;
    Missile3 *other;
    s32 m;

    for (i = 0; i < 200; i++, missile++) {
        if (missile == (void *)0)
            return;
        if (!(missile->flags & 1))
            continue;
        if (missile->purge_timer != 0)
            continue;
        if (missile->zoid_hit != 6 && missile->hit > 1)
            continue;
        if (missile->joint == (void *)0)
            continue;
        if (missile->field_3C != (void *)0)
            continue;
        switch (missile->zoid_hit) {
        case 5:
            for (j = 0; j < 16; j++) {
                sMissileZoid1Smoke(missile, &Zoid[j]);
            }
            break;
        case 6:
            for (j = 0; j < 16; j++) {
                sMissileZoid1Mine(missile, &Zoid[j]);
            }
            break;
        default:
            for (j = 0; j < 16; j++) {
                if (sMissileZoid1(missile, &Zoid[j]) != 0)
                    break;
            }
            for (j = 0; j < 100; j++) {
                weapon = &sWeapon[j];
                owner = weapon->owner;
                if (weapon->active == 0)
                    continue;
                for (k = 0; k < 12; k++) {
                    joint = weapon->inverses[k].joint;
                    if (joint == (void *)0)
                        continue;
                    if (HSD_JObjGetScaleX(joint) < 0.8)
                        continue;
                    if (sMissileAtk(missile, &weapon->inverses[k]) == 0)
                        continue;
                    if (weapon->drain != 0 && owner != 0 && owner->option_type == 1) {
                        owner->option_energy -= 200;
                        if (owner->option_energy < 0)
                            owner->option_energy = 0;
                    }
                    break;
                }
            }
            break;
        }
        if (missile->target_missile != 0)
            sMissileMis1(missile, missile->target_missile);
        other = missileStart;
        for (m = 0; m < 200; m++, other++) {
            if (other == missile)
                continue;
            if (other == (void *)0)
                break;
            if (!(other->flags & 1))
                continue;
            if (other->joint == (void *)0)
                continue;
            if (other->purge_timer != 0)
                continue;
            if (other->field_3C != (void *)0)
                continue;
            if (other->zoid_hit != 6)
                continue;
            sMissileMis1(missile, other);
        }
    }
}
#pragma dont_inline on
#pragma dont_inline off
f32 sMisZdAng(MisZoid3 *zoid, f32 *angle)
{
    Missile3 *missile;
    Missile3 *best;
    MisZoid3 *target;
    f32 nearest;
    f32 length;
    f32 delta;
    f32 sum;
    f32 result;
    s32 i;
    s32 k;
    Vec3 point;
    volatile f32 root;

    nearest = -1.0f;
    best = 0;
    if (zoid == (void *)0)
        return nearest;
    if (zoid->joint == (void *)0)
        return nearest;
    missile = missileStart;
    for (i = 0; i < 200; i++, missile++) {
        if (!(missile->flags & 1))
            continue;
        if (missile->purge_timer != 0)
            continue;
        if (missile->zoid_hit != 6 && missile->hit != 0)
            continue;
        if (missile->joint == (void *)0)
            continue;
        if (missile->field_3C != (void *)0)
            continue;
        target = missile->target_zoid;
        if (missile->zoid_hit != 6) {
            if (target == (void *)0)
                continue;
            if (target->joint == (void *)0)
                continue;
            if (target != zoid)
                continue;
        }
        sum = 0.0f;
        for (k = 0; k < 3; k++) {
            delta = zoid->position[k] - missile->matrix[k][3];
            sum += delta * delta;
        }
        length = sqrt4(sum, &root);
        if (nearest > length || -1.0f == nearest) {
            nearest = length;
            best = missile;
        }
    }
    if (angle == (void *)0 || best == (void *)0 || -1.0f == nearest)
        return nearest;
    for (k = 0; k < 3; k++)
        (&point.x)[k] = best->joint->matrix[k][3];
    result = ZoidCalcAngle(zoid, &point);
    *angle = result;
    return nearest;
}
#pragma dont_inline on
f32 sMisZd(MisZoid3 *zoid)
{
    return sMisZdAng(zoid, 0);
}
void sMissileLight(void)
{
    s32 hidden = 0;
    s32 i;
    s32 count;
    Missile3 *missile = missileStart;
    s32 show;

    i = 0;
    count = 0;
    for (; i < 200; missile++, i++) {
        if (missile == (void *)0) {
            return;
        }
        if (missile->purge_timer == 0 && missile->joint != (void *)0) {
            show = 1;
            if (missile->kind == 4 && missile->hit == 0) {
                show = 0;
            }
            if (missile->kind == 3) {
                show = 0;
            }
            if (hidden != 0) {
                show = 0;
            } else {
                count++;
            }
            if (count >= 3) {
                show = 0;
            }
            if (show != 0) {
                HSD_LObjAddCurrent(missile->light);
            }
        }
    }
}
#pragma force_active on
s32 misMAX;
s32 nowMX;
s32 sLightMis = 1;
#pragma force_active off
}
