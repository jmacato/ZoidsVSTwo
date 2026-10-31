extern "C" {
#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"
#include "joint.h"
#include "model_archive.h"

typedef struct WeaponSlot {
    s32 weapon;
    s32 field_04;
    HSD_JObj *joint;
} WeaponSlot;
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
typedef struct ZoidState {
    char name[0x20];
    s32 type;
    s32 person;
    void *field_0028;
    void *camera;
    u8 pad_0030[0x40 - 0x30];
    HSD_JObj *joint;
    u8 pad_0044[0x12A4 - 0x44];
    s32 mount_has_weapon[20];
    u8 pad_12F4[0x12FC - 0x12F4];
    s32 weapon_mount;
    u8 pad_1300[0x130C - 0x1300];
    WeaponMount weapon_mounts[20];
    u8 pad_176C[0x185C - 0x176C];
    WeaponSlot weapon_slots[30];
    s32 camera_joint;
    u8 pad_19C8[0x1B68 - 0x19C8];
    f32 position[3];
    u8 pad_1B74[0x1C2C - 0x1B74];
    u8 us_pad_1C2C[0xC];
    u8 pad_1B74_us[0x1D46 - 0x1C2C];
    u16 enemy_smoke_timer;
    u8 pad_1D48[0x1D64 - 0x1D48];
    s32 death_started;
    u8 pad_1D68[0x1D7C - 0x1D68];
    u8 index;
    u8 slot;
    u8 target_pending;
    u8 target_slot;
    u8 pad_1D80[0x1DAC - 0x1D80];
    s32 stel_state;
    u8 pad_1DB0[0x1DE0 - 0x1DB0];
    f32 distances[16];
    u8 pad_1E20[0x1F70 - 0x1E20];
    s32 bullet_power_bonus;
    u8 pad_1F74[0x1F8C - 0x1F74];
} ZoidState;
extern ZoidState Zoid[];

typedef struct WeaponInverse3 {
    HSD_JObj *joint;
    f32 inverse[3][4];
    u8 pad_34[4];
} WeaponInverse3;
typedef struct WeaponMarker3 {
    f32 position[3];
    s32 locked;
    s32 field_10;
    ZoidState *target;
    void *source;
    HSD_JObj *joint;
    s32 object_index;
} WeaponMarker3;
typedef struct WeaponState3 {
    s32 active;
    u32 flags;
    u32 state_flags;
    s32 type;
    ModelArchive *archive;
    s32 model;
    void *pose;
    HSD_JObj *mount_joint;
    HSD_JObj *link_joint;
    HSD_JObj *link_target;
    HSD_JObj *pre_link_joint;
    HSD_JObj *pre_link_target;
    HSD_JObj *target_joint;
    HSD_JObj *scope_joint;
    ZoidState *owner;
    u8 pad_3C[8];
    HSD_JObj *barrels[12];
    HSD_JObj *trace_joints[13];
    f32 trace_positions[13][3];
    WeaponInverse3 inverses[12];
    s32 animation;
    s32 ammo;
    f32 range;
    f32 rate;
    s32 color;
    s32 bullet_kind;
    s32 marker_count;
    s32 marker_cursor;
    s16 drain;
    u8 pad_406[2];
    WeaponMarker3 markers[20];
} WeaponState3;
typedef struct PartData3 {
    u8 pad_00[0x48];
    char model_name[8];
    u8 pad_50[0x5C - 0x50];
    s32 bullets[2];
    u32 flags;
    u8 pad_68[4];
    s16 power;
    s16 range;
    s16 ammo;
    u8 pad_72[2];
    s16 marker_count;
    s16 rate;
    u8 pad_78[0xEE - 0x78];
    s16 cam_a[3];
    s16 cam_b[3];
    u8 pad_FA[0x124 - 0xFA];
} PartData3;
typedef struct BulletData3 {
    u8 pad_00[0xC];
    s16 speed;
    u8 pad_0E[4];
    s16 trace;
    u8 pad_14[0x1E - 0x14];
    s16 fire_effect;
    u8 pad_20[0x4C - 0x20];
} BulletData3;
typedef struct ImageDesc3 {
    void *image;
    u16 width;
    u16 height;
    s32 format;
} ImageDesc3;
typedef struct TObj3 {
    u8 pad_00[0x58];
    ImageDesc3 *imagedesc;
} TObj3;

typedef struct Lsr2 {
    HSD_JObj *source;
    f32 start[3][4];
    f32 end[3][4];
    void *archive;
} Lsr2;
typedef struct Lsr1_3 {
    f32 start[3][4];
    f32 end[3][4];
    HSD_JObj *joint;
    HSD_JObj *start_joint;
    HSD_JObj *end_joint;
} Lsr1_3;

typedef struct Missile2 {
    u8 pad_00[0x1C];
    HSD_JObj *joint;
    u8 pad_20[0xDC - 0x20];
    s32 purge_timer;
    s32 hit;
    u8 pad_E4[0x18C - 0xE4];
} Missile2;

extern WeaponState3 sWeapon[100];
extern Missile2 missileStart[200];
extern s32 vsyncN;
extern Lsr2 sLsr[11];
extern Lsr1_3 sLsr1[400];
extern PartData3 Tea_partsdat[];
extern BulletData3 Tea_bulletdat[];
extern void __assert(const char *file, s32 line, const char *expr);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *joint);
extern void sGlobal2Local(HSD_JObj *joint);
extern void sMakeMatrix(HSD_JObj *joint);
extern void PSMTXInverse(f32 src[3][4], f32 inv[3][4]);
extern void PSMTXCopy(f32 src[3][4], f32 dst[3][4]);
extern s32 sprintf(char *buffer, const char *format, ...);
extern void OSReport(const char *format, ...);
extern HSD_JObj *searchArcJoint2(ModelArchive *archive, s32 model, char *name);
extern HSD_JObj *makeJobj(void);
extern void stCopyObj(HSD_JObj *source, void *pose);
extern void stCopyMatMobj(HSD_JObj *source, void *pose);
extern void HSD_JObjAnimAll(HSD_JObj *joint);
extern s32 getEndF(HSD_JObj *joint);
extern void sLsr1Set(Lsr2 *lsr);
extern void sLsr1Purge(Lsr1_3 *beam);
extern void sEffectSet(s32 type, s32 mode, f32 *position, HSD_JObj *source, ZoidState *zoid);
extern ModelArchive *LoadArc2USB(char *name);
typedef struct GroundMask {
    u8 bits[4];
} GroundMask;
extern f32 sColRealPt(Vec3 *a, Vec3 *b, GroundMask *mask);
extern f32 sLiner(f32 target, f32 current, f32 fraction);
extern f32 sLenPoint3dR(f32 *point);
extern f32 sLenJobj(HSD_JObj *a, HSD_JObj *b);
extern void *getAnimAobj(HSD_JObj *joint);
extern u32 HSD_AObjGetFlags(void *aobj);
extern void free(void *p);
extern void sLinier3d(f32 *current, f32 *target, f32 fraction);
extern void *camTmp;
extern struct HSD_Fog *fogG;
extern void HSD_FogSet(struct HSD_Fog *fog);
extern void HSD_JObjDispAll(HSD_JObj *joint, void *matrix, u32 passes, u32 flags);
extern void stSetFlag(HSD_JObj *joint, void *pose, f32 opacity);
void sWeaponDisp(WeaponState3 *weapon);
void sWeaponSetPer00(s32 index, f32 opacity);
extern void camSeTmJobj(void *camera, s32 x, void *joint, s32 y);
extern void setAnimRate(HSD_JObj *joint, f32 rate);
extern s32 jobjAnim(HSD_JObj *joint);
extern void stCopyMatReset(HSD_JObj *joint, void *pose);
extern void stStJobjUnref(void *pose);
extern void sWeaponPurgeD(ModelArchive *archive);
extern void PurgeArc2(ModelArchive *archive);
extern void PurgeArc(ModelArchive *archive);
extern void ZeroMemory(void *memory, s32 size);
extern void *sReadFileU(char *name, s32 mode);
extern void putErr(const char *msg);
extern TObj3 *searchTObj(void *jobj);
extern s32 calc_doltexture_size(s32 format, s32 w, s32 h);
extern s32 SetTex2(void *data, void *dest, s32 size);
extern s32 grdObjAliveN(s32 index);
extern s32 grdObjGetMax(void);
extern s32 grdObjGetGrp(s32 index);
extern HSD_JObj *grdObjGetJObj(s32 index);
extern s32 sCameraPt2d(void *camera, Vec3 *position, s32 *screen_point);
extern s32 sCameraJobjPt2d(void *camera, HSD_JObj *joint, s32 *screen_point);
extern void sMissileFire(ZoidState *owner, ZoidState *target_zoid, void *target_missile, s32 lock, f32 matrix[3][4], s32 bullet, s32 extra_damage);

extern f64 __frsqrte(f64 value);

namespace { extern const f32 lit_753[1]; extern const f64 lit_754[1]; extern const f64 lit_755[1]; extern const f64 lit_756[1]; }
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
#pragma dont_inline on

void sWeaponPurge(s32 weapon);
s32 sWeaponGetBullet(s32 weapon);
s32 sWeaponSetN(ModelArchive *archive, s32 type, s32 model, ZoidState *zoid, s32 slot, s32 ammo);
void sWeaponTarget(WeaponState3 *weapon, HSD_JObj *target);
void sWeaponTargetColli(WeaponState3 *weapon);
void sWeaponSight(WeaponState3 *weapon);
void sWeaponTrace(WeaponState3 *weapon);
void sWeaponFire(WeaponState3 *weapon);
void sWeaponSetAnim(s32 weapon, s32 animation);
s32 sWpSearchSight1z(WeaponState3 *weapon, ZoidState *zoid, ZoidState *target);
s32 sWpSearchSight1g(WeaponState3 *weapon, ZoidState *zoid, s32 index);
#pragma force_active on
WeaponState3 sWeapon[100];
static Lsr2 sLsr[11];
static Lsr1_3 sLsr1[400];
#pragma force_active off
void sWeaponPrologue(void)
{
    ZeroMemory(sWeapon, 0x2AC60);
}
void sWeaponPurgeD(ModelArchive *archive)
{
    WeaponState3 *weapon;
    s32 index;

    for (index = 0; index < 100; index++) {
        weapon = &sWeapon[index];
        if (weapon->active != 0 && weapon->model != 0 && weapon->archive == archive) {
            sWeaponPurge(index);
        }
    }
}
void sWeaponPurge(s32 index)
{
    WeaponState3 *weapon;
    ZoidState *zoid;
    WeaponMount *mount;
    s32 selected;
    s32 component;

    if (index >= 0 && index < 100) {
        weapon = &sWeapon[index];
        if (weapon->active == 0) {
            return;
        }
        stCopyMatReset(weapon->archive->models[weapon->model], weapon->pose);
        stStJobjUnref(weapon->pose);
        weapon->pose = 0;
        if (weapon->model == 0) {
            sWeaponPurgeD(weapon->archive);
            PurgeArc2(weapon->archive);
        }
        weapon->archive = 0;
        zoid = weapon->owner;
        if (zoid != 0) {
            s32 slot;

            for (slot = 0; slot < 30; slot++) {
                if (zoid->weapon_slots[slot].weapon == index) {
                    zoid->weapon_slots[slot].weapon = -1;
                    for (selected = 0; selected < 20; selected++) {
                        mount = &zoid->weapon_mounts[selected];
                        if (mount->active != 0) {
                            for (component = 0; component < 4; component++) {
                                WeaponMountEntry *entry = &mount->entries[component];

                                if (entry->active != 0 && entry->slot == slot) {
                                    entry->slot = -1;
                                    entry->active = 0;
                                }
                            }
                        }
                    }
                }
            }
            weapon->owner = 0;
        }
        weapon->active = 0;
        ZeroMemory(weapon, sizeof(*weapon));
    }
}
s32 sWeaponSetN(ModelArchive *archive, s32 type, s32 model, ZoidState *zoid, s32 slot, s32 ammo)
{
    char name[256];
    WeaponState3 *weapon;
    s32 bullet;
    HSD_JObj *joint;
    HSD_JObj *link;
    HSD_JObj *target;
    s32 i;
    s32 j;
    s32 index;

    if (zoid == (void *)0)
        return -1;
    if (archive == 0)
        return -1;
    if (slot >= 30)
        return -1;
    if (slot < 0)
        return -1;
    joint = zoid->weapon_slots[slot].joint;
    if (joint == (void *)0)
        return -1;
    if (zoid->weapon_slots[slot].weapon != -1)
        return -2;
    for (i = 0; i < 100; i++) {
        weapon = &sWeapon[i];
        if (weapon->active == 0)
            break;
    }
    if (i == 100)
        return -1;
    index = i;
    if (weapon->active != 0)
        return -1;
    weapon->archive = archive;
    weapon->model = model;
    weapon->owner = zoid;
    weapon->type = type;
    weapon->mount_joint = joint;
    weapon->target_joint = searchArcJoint2(weapon->archive, model, "target_joint");
    if (weapon->target_joint == (void *)0)
        weapon->target_joint = searchArcJoint2(weapon->archive, model, "target1_joint");
    weapon->scope_joint = searchArcJoint2(weapon->archive, model, "scope_joint");
    for (i = 0; i < 12; i++) {
        sprintf(name, "pa_%02d_joint", i);
        weapon->barrels[i] = searchArcJoint2(weapon->archive, model, name);
    }
    for (i = 0; i < 13; i++) {
        sprintf(name, "te_%02d_joint", i);
        weapon->trace_joints[i] = searchArcJoint2(weapon->archive, model, name);
        for (j = 0; j < 3; j++)
            weapon->trace_positions[i][j] = zoid->position[j];
    }
    link = searchArcJoint2(weapon->archive, model, "link_joint");
    if (link != 0 && slot < 29) {
        target = zoid->weapon_slots[slot + 1].joint;
        if (target != 0) {
            weapon->link_joint = link;
            weapon->link_target = target;
        }
    }
    link = searchArcJoint2(weapon->archive, model, "pre_link_joint");
    if (link != 0 && slot > 0) {
        target = zoid->weapon_slots[slot - 1].joint;
        if (target != 0) {
            weapon->pre_link_joint = link;
            weapon->pre_link_target = target;
        }
    }
    for (i = 0; i < 12; i++) {
        sprintf(name, "a_%x_joint", i);
        weapon->inverses[i].joint = searchArcJoint2(weapon->archive, model, name);
    }
    weapon->pose = makeJobj();
    stCopyObj(archive->models[model], weapon->pose);
    stCopyMatMobj(archive->models[model], weapon->pose);
    weapon->active = 1;
    weapon->ammo = ammo;
    weapon->animation = 0;
    weapon->marker_count = Tea_partsdat[type].marker_count;
    if (weapon->marker_count > 20)
        weapon->marker_count = 20;
    weapon->bullet_kind = 0;
    bullet = Tea_partsdat[type].bullets[0];
    if (bullet > 0)
        weapon->bullet_kind = Tea_bulletdat[bullet].trace;
    weapon->range = Tea_partsdat[type].range;
    weapon->rate = 0.01 * Tea_partsdat[type].rate;
    weapon->drain = Tea_partsdat[type].flags & 0x37;
    zoid->weapon_slots[slot].weapon = index;
    sWeaponSetAnim(index, 4);
    return index;
}
#undef lit_486
#define lit_486 "target_joint"
#undef lit_487
#define lit_487 "target1_joint"
#undef lit_488
#define lit_488 "scope_joint"
#undef lit_489
#define lit_489 "pa_%02d_joint"
#undef lit_490
#define lit_490 "te_%02d_joint"
#undef lit_491
#define lit_491 "link_joint"
#undef lit_492
#define lit_492 "pre_link_joint"
#undef lit_493
#define lit_493 "a_%x_joint"
s32 sWeaponSetU(s32 part, ZoidState *zoid, s32 slot, WeaponMount *mount, s32 usb)
{
    char name[100];
    ModelArchive *archive;
    s32 model;
    s32 i;
    s32 current;
    s32 ammo;
    s32 entry;
    s32 result;

    if (zoid == (void *)0)
        return -1;
    if (zoid->joint == (void *)0)
        return -1;
    if (slot < 0)
        return -1;
    if (slot > 27)
        return -1;
    if (zoid->weapon_slots[slot].joint == (void *)0)
        return -1;
    if (zoid->weapon_slots[slot].weapon != -1)
        return -1;
    if (part <= 0)
        return -1;
    if (part >= 450)
        return -1;
    sprintf(name, "weapon/%s.dat", Tea_partsdat[part].model_name);
    if (usb != 0)
        archive = LoadArc2USB(name);
    else
        archive = LoadArc2(name);
    if (archive == (void *)0)
        return -1;
    if (archive->model_data == (void *)0)
        return -1;
    ammo = Tea_partsdat[part].ammo;
    current = slot;
    result = 0;
    entry = 0;
    for (model = 0; model < archive->model_count; model++, current++) {
        for (; entry < 4; entry++) {
            if (mount->entries[entry].active == 0)
                break;
        }
        if (entry == 4)
            break;
        if (current >= 27)
            break;
        result = sWeaponSetN(archive, part, model, zoid, current, ammo);
        if (result == -1)
            break;
        if (result != -2) {
            mount->active = 1;
            mount->entries[entry].active = 1;
            mount->entries[entry].slot = current;
            mount->entries[entry].field_08 = 0;
        }
    }
    if (result == -1) {
        for (i = 0; i < model; i++)
            sWeaponPurge(zoid->weapon_slots[slot + i].weapon);
        return -1;
    }
    return archive->model_count;
}
#undef lit_549
#define lit_549 "weapon/%s.dat"
void sWeaponMountU(s32 weapon, ZoidState *zoid, s32 slot, s32 mount, s32 flag)
{
    WeaponMount *entry;
    s32 result;

    OSReport("mount wp    zoid=%d mt=%d act=%d wp=%d\n", zoid != 0 ? zoid->type : -1, slot, mount, weapon);
    if (zoid != (void *)0 && zoid->joint != (void *)0 && weapon > 0 && mount >= 0) {
        entry = &zoid->weapon_mounts[mount];
        result = sWeaponSetU(weapon, zoid, slot, entry, flag);
        if (result != -1) {
            zoid->mount_has_weapon[mount] = 1;
        }
    }
}
#undef lit_562
#define lit_562 "mount wp    zoid=%d mt=%d act=%d wp=%d\n"
#pragma force_active on
static const char lit_1491[12] = "start_joint";
static const char lit_1492[10] = "end_joint";
#pragma force_active off
void sWeaponUnMount(ZoidState *zoid, s32 slot)
{
    s32 weapon;

    if (zoid != (void *)0 && zoid->joint != (void *)0 && slot >= 0 && slot < 30) {
        weapon = zoid->weapon_slots[slot].weapon;
        sWeaponPurge(weapon);
    }
}
s32 sWeaponGetBullet(s32 index)
{
    WeaponState3 *weapon;

    if (index < 0) {
        return 0;
    }
    if (index >= 100) {
        return 0;
    }
    weapon = &sWeapon[index];
    if (weapon->active == 0) {
        return 0;
    }
    return weapon->ammo;
}
s32 sWeaponGetMountBullet(ZoidState *zoid, s32 index)
{
    WeaponMount *mount;
    s32 total;
    s32 i;
    s32 found;
    s32 slot;
    s32 weapon;

    if (zoid == (void *)0) {
        return 0;
    }
    if (zoid->joint == (void *)0) {
        return 0;
    }
    if (index == -1) {
        return 0;
    }
    mount = &zoid->weapon_mounts[index];
    if (mount->active == 0) {
        return 0;
    }
    total = 0;
    found = 0;
    for (i = 0; i < 4; i++) {
        if (mount->entries[i].active != 0) {
            slot = mount->entries[i].slot;
            if (slot == -1) {
                mount->entries[i].active = 0;
            } else {
                weapon = zoid->weapon_slots[slot].weapon;
                if (weapon == -1) {
                    mount->entries[i].active = 0;
                    mount->entries[i].slot = -1;
                } else {
                    found++;
                    total += sWeaponGetBullet(weapon);
                }
            }
        }
    }
    if (found == 0) {
        mount->active = 0;
    }
    return total;
}
s32 sWeaponGetNumber(ZoidState *zoid, s32 selected)
{
    WeaponMount *mount;
    s32 index;
    s32 slot;
    s32 weapon;

    if (zoid == (void *)0) {
        return -1;
    }
    if (zoid->joint == (void *)0) {
        return -1;
    }
    if (selected == -1) {
        return -1;
    }
    mount = &zoid->weapon_mounts[selected];
    if (mount->active == 0) {
        return -1;
    }
    {
        s32 found = 0;

        for (index = 0; index < 4; index++) {
            if (mount->entries[index].active != 0) {
                slot = mount->entries[index].slot;
                if (slot != -1) {
                    weapon = zoid->weapon_slots[slot].weapon;
                    if (weapon != -1) {
                        return sWeapon[weapon].type;
                    }
                }
            }
        }
    }
    return -1;
}
void sWeaponSetAnimWp(WeaponState3 *weapon, s32 animation)
{
    s32 index;
    HSD_JObj *joint;
    s32 model;
    PartData3 *part;
    s32 type;
    s16 cam_a;

    if (weapon == 0) {
        return;
    }
    if (weapon->active == 0) {
        return;
    }
    if (weapon->archive->models[weapon->model] == (void *)0) {
        return;
    }
    model = weapon->model;
    SetAnimArc2(weapon->archive, model, animation);
    joint = weapon->archive->models[model];
    if (joint != 0) {
        setAnimRate(joint, 1.0f);
        jobjAnim(joint);
        sMakeMatrix(joint);
    }
    weapon->animation = animation;
    weapon->state_flags &= ~0x10;
    type = weapon->type;
    part = &Tea_partsdat[type];
    index = -1;
    if (animation == 0) {
        index = 0;
    }
    if (animation == 2) {
        index = 1;
    }
    if (animation == 3) {
        index = 2;
    }
    if (index == -1) {
        return;
    }
    cam_a = part->cam_a[index];
    camSeTmJobj(camTmp, cam_a, weapon->mount_joint, part->cam_b[index]);
}
void sWeaponSetAnim(s32 index, s32 animation)
{
    WeaponState3 *weapon;

    if (index < 0) {
        return;
    }
    if (index >= 100) {
        return;
    }
    weapon = &sWeapon[index];
    sWeaponSetAnimWp(weapon, animation);
}
#pragma dont_inline off
void sWeaponTarget(WeaponState3 *weapon, HSD_JObj *target)
{
    HSD_JObj *joint;
    HSD_JObj *parent;
    s32 i;
    s32 j;

    if (weapon == (void *)0) {
        return;
    }
    if (target == (void *)0) {
        return;
    }
    if (weapon->animation >= 4) {
        return;
    }
    joint = weapon->target_joint;
    if (joint == (void *)0) {
        return;
    }
    parent = joint->parent;
    if (parent != 0) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                joint->matrix[i][j] = parent->matrix[i][j];
            }
        }
    }
    for (i = 0; i < 3; i++) {
        joint->matrix[i][3] = target->matrix[i][3];
    }
    sGlobal2Local(joint);
    if (joint != 0 && !HSD_JObjMtxIsDirty(joint)) {
        HSD_JObjSetMtxDirtySub(joint);
    }
}
#pragma dont_inline on
#undef lit_689
#define lit_689 "jobj.h"
#undef lit_690
#define lit_690 "jobj"
#pragma force_active on
namespace { __declspec(section ".sdata2") extern const f32 lit_753[1] = { 0.0f }; }
namespace { __declspec(section ".sdata2") extern const f64 lit_754[1] = { 0.2 }; }
namespace { __declspec(section ".sdata2") extern const f64 lit_755[1] = { 0.5 }; }
namespace { __declspec(section ".sdata2") extern const f64 lit_756[1] = { 3.0 }; }
static const f64 lit_757[1] = { 0.1 };
static const f64 lit_758[1] = { 0.025 };
#pragma force_active off
#pragma dont_inline off
void sWeaponTargetColli(WeaponState3 *weapon)
{
    HSD_JObj *child;
    s32 i;
    f32 a[3];
    f32 b[3];
    GroundMask mask = {{0x17, 0, 0, 0}};
    GroundMask mask_copy;
    f32 hit;

    if (weapon == (void *)0)
        return;
    if (weapon->animation >= 4)
        return;
    {
        HSD_JObj *joint = weapon->target_joint;
        if (joint == 0)
            return;
        child = joint->parent;
        if (child == 0)
            return;
        for (i = 0; i < 3; i++) {
            a[i] = child->matrix[i][3];
            b[i] = a[i] + 300.0f * child->matrix[i][0];
        }
        mask_copy = mask;
        hit = sColRealPt((Vec3 *)a, (Vec3 *)b, &mask_copy);
        for (i = 0; i < 3; i++) {
            joint->matrix[i][3] = b[i];
            if (hit != 10000.0f)
                joint->matrix[i][3] = a[i] + 300.0f * child->matrix[i][0] * hit;
        }
        joint ? (void)0 : __assert(lit_689, 833, lit_690);
        joint->scale.z = 1.0f;
        if (!(joint->flags & 0x02000000))
            HSD_JObjSetMtxDirty(joint);
        sGlobal2Local(joint);
        HSD_JObjSetMtxDirty(joint);
    }
}
#pragma dont_inline on
#undef lit_794
#define lit_794 "F\x1c@"
void sWeaponTrace(WeaponState3 *weapon)
{
    s32 i;
    s32 index;

    if (weapon->trace_joints[0] != (void *)0) {
        for (i = 0; i < 3; i++) {
            weapon->trace_positions[0][i] = weapon->trace_joints[0]->matrix[i][3];
        }
        for (index = 12; index > 0; index--) {
            HSD_JObj *previous_joint = weapon->trace_joints[index - 1];
            HSD_JObj *joint = weapon->trace_joints[index];

            if (previous_joint != (void *)0 && joint != (void *)0) {
                f32 *previous = weapon->trace_positions[index - 1];
                f32 *point = weapon->trace_positions[index];

                sLinier3d(point, previous, 0.8f);
                for (i = 0; i < 3; i++) {
                    joint->matrix[i][3] = point[i];
                }
                sGlobal2Local(joint);
            }
        }
    }
}
#pragma dont_inline off
static inline f32 HSD_JObjGetScaleX(HSD_JObj *jobj)
{
    f32 scale;

    jobj ? (void)0 : __assert(lit_689, 899, lit_690);
    scale = jobj->scale.x;
    return scale;
}

void sWeaponFire(WeaponState3 *weapon)
{
    s32 i;
    s32 k;
    HSD_JObj *joint;
    s32 bullet;
    s32 extra_damage;
    s32 effect;
    WeaponMarker3 *marker;
    s32 model;
    s32 type;
    s32 bullets[2];

    if (weapon == (void *)0)
        return;
    if (weapon->active == 0)
        return;
    type = weapon->type;
    model = weapon->model;
    if (type <= 0)
        return;
    bullets[0] = Tea_partsdat[type].bullets[0];
    bullets[1] = Tea_partsdat[type].bullets[1];
    if (bullets[0] <= 0)
        return;
    if (bullets[1] <= 0)
        bullets[1] = bullets[0];
    if (weapon->archive->models[model] == (void *)0)
        return;
    if (weapon->flags & 4)
        return;
    if (weapon->state_flags & 0x10)
        return;
    if (weapon->owner->death_started != 0) {
        weapon->flags |= 8;
        return;
    }
    extra_damage = Tea_partsdat[type].power;
    if (weapon->owner != 0)
        extra_damage += weapon->owner->bullet_power_bonus;
    for (i = 0; i < 12; i++) {
        joint = weapon->barrels[i];
        if (joint == (void *)0)
            continue;
        if (HSD_JObjGetScaleX(joint) < 1.0)
            continue;
        if (weapon->ammo == 0)
            continue;
        weapon->ammo--;
        bullet = bullets[i & 1];
        effect = Tea_bulletdat[bullet].fire_effect;
        if (effect > 0)
            sEffectSet(effect, 1, 0, joint, 0);
        if (weapon->bullet_kind != 1) {
            sMissileFire(weapon->owner, weapon->owner->target_pending != 0 ? &Zoid[weapon->owner->target_slot] : 0, 0, -1, joint->matrix, bullet, extra_damage);
        } else {
            for (k = 0; k < 20; k++) {
                weapon->marker_cursor++;
                if (weapon->marker_cursor >= weapon->marker_count)
                    weapon->marker_cursor = 0;
                marker = &weapon->markers[weapon->marker_cursor];
                if (marker->joint != (void *)0)
                    break;
            }
            if (k != 20)
                sMissileFire(weapon->owner, marker->target, marker->source, marker->object_index, joint->matrix, bullet, extra_damage);
            else
                sMissileFire(weapon->owner, 0, 0, -1, joint->matrix, bullet, extra_damage);
        }
    }
}
#pragma dont_inline on
#pragma dont_inline off
static inline f32 sqrtf(f32 x, volatile f32 *y)
{
    if (x > lit_753[0]) {
        f64 guess = __frsqrte(x);
        guess = lit_755[0] * guess * (lit_756[0] - guess * guess * x);
        guess = lit_755[0] * guess * (lit_756[0] - guess * guess * x);
        guess = lit_755[0] * guess * (lit_756[0] - guess * guess * x);
        *y = x * guess;
        return *y;
    }
    return x;
}

s32 sWeaponGetBarrel(WeaponState3 *weapon, Vec3 *out, f32 distance)
{
    s32 result;
    s32 j;
    s32 i;
    s32 model;
    s32 type;
    HSD_JObj *joint;
    f32 length;
    s32 bullets[2];
    volatile f32 root;

    if (weapon == (void *)0) {
        result = 0;
        return result;
    }
    if (weapon->active == 0) {
        result = 0;
        return result;
    }
    model = weapon->model;
    type = weapon->type;
    bullets[0] = Tea_partsdat[type].bullets[0];
    bullets[1] = Tea_partsdat[type].bullets[1];
    if (lit_753[0] == distance)
        distance = (f32)Tea_partsdat[type].range;
    if (bullets[0] <= 0) {
        result = 0;
        return result;
    }
    if (bullets[1] <= 0)
        bullets[1] = bullets[0];
    if (weapon->archive->models[model] == (void *)0) {
        result = 0;
        return result;
    }
    for (i = 0; i < 12; i++) {
        joint = weapon->barrels[i];
        if (joint == (void *)0)
            continue;
        joint ? (void)0 : __assert(lit_689, 899, lit_690);
        length = joint->scale.x;
        if (length < lit_754[0])
            continue;
        length = sqrtf(((HSD_JObj *)(s32)(s64)(s32)joint)->matrix[1][0] * ((HSD_JObj *)(s32)(s64)(s32)joint)->matrix[1][0] +
                       ((HSD_JObj *)(s32)(s64)(s32)joint)->matrix[0][0] * ((HSD_JObj *)(s32)(s64)(s32)joint)->matrix[0][0] +
                       joint->matrix[2][0] * joint->matrix[2][0], &root);
        for (j = 0; j < 3; j++) {
            (&out->x)[j] = distance * (joint->matrix[j][0] / length) + joint->matrix[j][3];
        }
        break;
    }
    if (i == 12) {
        result = 0;
        return result;
    }
    result = 1;
    return result;
}
#pragma dont_inline on
s32 sWeaponGetBarrelParabola(WeaponState3 *weapon, Vec3 *out, f32 distance)
{
    s32 j;
    s32 i;
    s32 bullet;
    s32 model;
    s32 type;
    HSD_JObj *joint;
    f32 horizontal_root;
    f32 direction_root;
    f32 drop;
    f32 horizontal_length;
    f32 length;
    f32 horizontal_squared;
    f32 speed;
    f32 t;
    s32 bullets[2];
    Vec3 dir;
    volatile f32 rounded_horizontal;
    volatile f32 rounded_direction;

    if (weapon == (void *)0)
        return 0;
    if (weapon->active == 0)
        return 0;
    model = weapon->model;
    type = weapon->type;
    bullets[0] = Tea_partsdat[type].bullets[0];
    bullets[1] = Tea_partsdat[type].bullets[1];
    if (lit_753[0] == distance)
        distance = distance;
    if (bullets[0] <= 0)
        return 0;
    if (bullets[1] <= 0)
        bullets[1] = bullets[0];
    if (weapon->archive->models[model] == (void *)0)
        return 0;
    for (i = 0; i < 12; i++) {
        joint = weapon->barrels[i];
        if (joint == (void *)0)
            continue;
        joint ? (void)0 : __assert(lit_689, 899, lit_690);
        length = joint->scale.x;
        if (length < lit_754[0])
            continue;
        {
            f32 x = joint->matrix[1][0] * joint->matrix[1][0] +
                    joint->matrix[0][0] * joint->matrix[0][0] +
                    joint->matrix[2][0] * joint->matrix[2][0];
            if (x > lit_753[0]) {
                f64 guess = __frsqrte(x);
                guess = lit_755[0] * guess * (lit_756[0] - guess * guess * x);
                guess = lit_755[0] * guess * (lit_756[0] - guess * guess * x);
                guess = lit_755[0] * guess * (lit_756[0] - guess * guess * x);
                rounded_direction = x * guess;
                direction_root = rounded_direction;
            } else {
                direction_root = x;
            }
        }
        length = direction_root;
        bullet = bullets[i & 1];
        if (bullet < 0)
            continue;
        speed = lit_757[0] * Tea_bulletdat[bullet].speed;
        for (j = 0; j < 3; j++) {
            (&dir.x)[j] = joint->matrix[j][0] * speed / length;
        }
        {
            horizontal_squared = (&dir.x)[0] * (&dir.x)[0] + (&dir.x)[2] * (&dir.x)[2];
            if (horizontal_squared > lit_753[0]) {
                f64 guess = __frsqrte((f64)horizontal_squared);
                guess = lit_755[0] * guess * (lit_756[0] - guess * guess * horizontal_squared);
                guess = lit_755[0] * guess * (lit_756[0] - guess * guess * horizontal_squared);
                guess = lit_755[0] * guess * (lit_756[0] - guess * guess * horizontal_squared);
                guess = lit_755[0] * guess * (lit_756[0] - guess * guess * horizontal_squared);
                rounded_horizontal = (f32)(horizontal_squared * guess);
                horizontal_root = rounded_horizontal;
            } else {
                horizontal_root = horizontal_squared;
            }
        }
        horizontal_length = horizontal_root;
        if (lit_753[0] == horizontal_length)
            return 0;
        t = distance / horizontal_length;
        drop = lit_758[0] * (t * (1.0f + t) / 2.0f);
        for (j = 0; j < 3; j++) {
            (&out->x)[j] = t * (&dir.x)[j] + joint->matrix[j][3];
        }
        out->y -= drop;
        break;
    }
    if (i == 12)
        return 0;
    return 1;
}
void sWeaponSight(WeaponState3 *weapon)
{
    WeaponMarker3 *marker;
    s32 i;
    s32 k;
    Missile2 *source;
    ZoidState *target;
    f32 delta[3];
    f32 goal;
    f32 length;

    if (weapon == (void *)0)
        return;
    if (weapon->active == 0)
        return;
    if (weapon->bullet_kind != 1)
        return;
    for (i = 0; i < 20; i++) {
        marker = &weapon->markers[i];
        if (marker->joint == (void *)0)
            continue;
        source = (Missile2 *)marker->source;
        if (source != 0 && (source->joint == 0 || source->purge_timer != 0 || source->hit != 0)) {
            marker->joint = 0;
            marker->source = 0;
            marker->target = 0;
            marker->object_index = -1;
            continue;
        }
        target = marker->target;
        if (target != 0 && (target->death_started != 0 || target->stel_state == 2)) {
            marker->joint = 0;
            marker->target = 0;
            marker->source = 0;
            marker->object_index = -1;
            continue;
        }
        for (k = 0; k < 3; k++) {
            goal = marker->joint->matrix[k][3];
            (&marker->position[0])[k] = sLiner(goal, (&marker->position[0])[k], weapon->rate);
            delta[k] = goal - (&marker->position[0])[k];
        }
        length = sLenPoint3dR(delta);
        if (length < lit_755[0])
            marker->locked = 1;
        if (marker->locked != 0) {
            for (k = 0; k < 3; k++) {
                (&marker->position[0])[k] = marker->joint->matrix[k][3];
            }

            length = sLenJobj(marker->joint, weapon->owner->joint);
            if (length > weapon->range)
                marker->joint = 0;
        }
    }
}
#pragma dont_inline off
void sWeaponAnim(WeaponState3 *weapon)
{
    HSD_JObj *mount;
    HSD_JObj *joint;
    HSD_JObj *target_joint;
    void *aobj;
    ZoidState *owner;
    ZoidState *target;
    s32 model;
    s32 type;
    u32 part_flags;

    if (weapon == 0)
        return;
    mount = weapon->mount_joint;
    model = weapon->model;
    if (mount == (void *)0)
        return;
    joint = weapon->archive->models[model];
    if (joint == (void *)0)
        return;
    if (weapon->flags & 4)
        return;
    type = weapon->type;
    if (type <= 0)
        return;
    part_flags = Tea_partsdat[type].flags;
    owner = weapon->owner;
    target = 0;
    target_joint = 0;
    if (owner != 0) {
        if (owner->target_pending != 0) {
            target = &Zoid[owner->target_slot];
            if (target->joint == (void *)0)
                target = 0;
        }
        if (owner->death_started != 0)
            target = 0;
    }
    if (target != 0 && target->joint == (void *)0)
        target = 0;
    if (target != 0) {
        target_joint = target->joint;
        if ((HSD_JObj *)target->camera_joint != 0)
            target_joint = (HSD_JObj *)target->camera_joint;
    }
    aobj = getAnimAobj(joint);
    if (!(weapon->state_flags & 0x10)) {
        HSD_JObjAnimAll(joint);
        weapon->state_flags |= 8;
        if (HSD_AObjGetFlags(aobj) & 0x04000000) {
            weapon->state_flags &= ~8;
            if (weapon->flags & 8)
                weapon->state_flags |= 0x10;
        }
        sMakeMatrix(joint);
    }
    if (part_flags & 0x100)
        sWeaponTargetColli(weapon);
    else
        sWeaponTarget(weapon, target_joint);
    HSD_JObjSetMtxDirty(joint);
    sMakeMatrix(joint);
}
#pragma dont_inline on
#pragma dont_inline off
void sWeaponMove(WeaponState3 *weapon)
{
    s32 type;
    HSD_JObj *target_joint;
    HSD_JObj *joint;
    HSD_JObj *mount;
    s32 model;
    ZoidState *target;
    ZoidState *owner;
    s32 j;
    HSD_JObj *a;
    s32 i;
    HSD_JObj *c;
    u32 part_flags;

    if (weapon == 0)
        return;
    mount = weapon->mount_joint;
    model = weapon->model;
    if (mount == (void *)0)
        return;
    joint = weapon->archive->models[model];
    if (joint == (void *)0)
        return;
    if (weapon->flags & 4)
        return;
    type = weapon->type;
    if (type <= 0)
        return;
    owner = weapon->owner;
    part_flags = Tea_partsdat[type].flags;
    target = 0;
    target_joint = 0;
    if (owner != 0) {
        if (owner->target_pending != 0) {
            target = &Zoid[owner->target_slot];
            if (target->joint == (void *)0)
                target = 0;
        }
        if (owner->death_started != 0)
            target = 0;
    }
    if (target != 0 && target->joint == (void *)0)
        target = 0;
    if (target != 0) {
        target_joint = target->joint;
        if ((HSD_JObj *)target->camera_joint != 0)
            target_joint = (HSD_JObj *)target->camera_joint;
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            joint->matrix[i][j] = mount->matrix[i][j];
        }
    }
    sGlobal2Local(joint);
    if (weapon->link_joint != 0 && weapon->link_target != 0) {
        a = weapon->link_joint;
        a ? (void)0 : __assert(lit_689, 952, lit_690);
        a->translation.x = 1.0f;
        if (!(a->flags & 0x02000000))
            HSD_JObjSetMtxDirty(a);
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 4; j++) {
                weapon->link_joint->matrix[i][j] = weapon->link_target->matrix[i][j];
            }
        }
        sGlobal2Local(weapon->link_joint);
    }
    if (weapon->pre_link_joint != 0 && weapon->pre_link_target != 0) {
        c = weapon->pre_link_joint;
        c ? (void)0 : __assert(lit_689, 952, lit_690);
        c->translation.x = 1.0f;
        if (!(c->flags & 0x02000000))
            HSD_JObjSetMtxDirty(c);
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 4; j++) {
                weapon->pre_link_joint->matrix[i][j] = weapon->pre_link_target->matrix[i][j];
            }
        }
        sGlobal2Local(weapon->pre_link_joint);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            joint->matrix[i][j] = mount->matrix[i][j];
        }
    }
    if (part_flags & 0x100)
        sWeaponTargetColli(weapon);
    else
        sWeaponTarget(weapon, target_joint);
    sWeaponSight(weapon);
    sWeaponTrace(weapon);
    HSD_JObjSetMtxDirty(joint);
    sMakeMatrix(joint);
    sWeaponFire(weapon);
    for (i = 0; i < 12; i++) {
        if (weapon->inverses[i].joint != 0)
            PSMTXInverse(weapon->inverses[i].joint->matrix, weapon->inverses[i].inverse);
    }
}
#pragma dont_inline on
void sWeaponAnimAll(void)
{
    WeaponState3 *weapon;
    s32 index;

    for (index = 0; index < 100; index++) {
        weapon = &sWeapon[index];
        if (weapon->active != 0) {
            sWeaponAnim(weapon);
        }
    }
}
void sWeaponMoveAll(void)
{
    WeaponState3 *weapon;
    s32 index;

    for (index = 0; index < 100; index++) {
        weapon = &sWeapon[index];
        if (weapon->active != 0) {
            sWeaponMove(weapon);
        }
    }
}
void sWeaponLoopOFF(s32 index)
{
    WeaponState3 *weapon;

    if (index >= 0 && index < 100) {
        weapon = &sWeapon[index];
        if (weapon->active != 0) {
            weapon->flags |= 8;
        }
    }
}
void sWeaponLoopON(s32 index)
{
    WeaponState3 *weapon;

    if (index >= 0 && index < 100) {
        weapon = &sWeapon[index];
        if (weapon->active != 0) {
            weapon->flags &= ~8;
        }
    }
}
void sWeaponStopOFF(s32 index)
{
    WeaponState3 *weapon;

    if (index >= 0 && index < 100) {
        weapon = &sWeapon[index];
        if (weapon->active != 0) {
            weapon->state_flags &= ~0x10;
        }
    }
}
s32 sWeaponStopF(s32 index)
{
    WeaponState3 *weapon;

    if (index < 0) {
        return 1;
    }
    if (index >= 100) {
        return 1;
    }
    weapon = &sWeapon[index];
    if (weapon->active == 0) {
        return 1;
    }
    return weapon->state_flags & 0x10;
}
void sWeaponDisp(WeaponState3 *weapon)
{
    HSD_JObj *joint;
    s32 model;

    if (weapon != (void *)0 && (weapon->flags & 1) == 0) {
        model = weapon->model;
        joint = weapon->archive->models[model];
        if (joint != (void *)0) {
            HSD_JObjDispAll(joint, 0, 7, 0);
        }
    }
}
void sWeaponDispAll(void)
{
    register s32 use_fog = 1;
    WeaponState3 *weapon;
    s32 index;
    s32 flag = 1;

    for (index = 0; index < 100; index++) {
        weapon = &sWeapon[index];
        if (weapon->active != 0) {
            s32 type = weapon->type;

            if ((Tea_partsdat[type].flags & 0x80) != 0) {
                use_fog = 0;
            } else {
                use_fog = 1;
            }
            if (fogG != 0) {
                HSD_FogSet(use_fog != 0 ? fogG : 0);
            }
            sWeaponDisp(weapon);
            flag = 0;
        }
    }
    if (fogG != 0) {
        HSD_FogSet(fogG);
    }
}
s32 sWeaponGetAnim(s32 index)
{
    WeaponState3 *weapon;

    if (index < 0) {
        return -1;
    }
    if (index >= 100) {
        return -1;
    }
    weapon = &sWeapon[index];
    if (weapon->active == 0) {
        return -1;
    }
    return weapon->animation;
}
s32 sWeaponEndF(s32 index)
{
    WeaponState3 *weapon;

    if (index < 0) {
        return 0;
    }
    if (index >= 100) {
        return 0;
    }
    weapon = &sWeapon[index];
    if (weapon->active == 0) {
        return 0;
    }
    if ((weapon->state_flags & 0x10) != 0) {
        return 1;
    }
    return getEndF(weapon->archive->models[weapon->model]);
}
void sWeaponSetPer00(s32 index, f32 opacity)
{
    WeaponState3 *weapon;

    if (index >= 0 && index < 100) {
        weapon = &sWeapon[index];
        if (weapon->active != 0) {
            stSetFlag(weapon->archive->models[weapon->model], weapon->pose, opacity);
        }
    }
}
void sWeaponSetPer(ZoidState *zoid, f32 opacity)
{
    s32 slot;
    s32 index;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        for (slot = 0; slot < 30; slot++) {
            index = zoid->weapon_slots[slot].weapon;
            if (index != -1) {
                sWeaponSetPer00(index, opacity);
            }
        }
    }
}
s32 sWpSearchSight1z(WeaponState3 *weapon, ZoidState *zoid, ZoidState *target)
{
    s32 i;
    s32 count;
    WeaponMarker3 *marker;
    s32 pt[3];
    f32 range;

    if (target == (void *)0)
        return 0;
    if (target->joint == (void *)0)
        return 0;
    count = weapon->marker_count;
    if (count < 1)
        return 1;
    for (i = 0; i < count; i++) {
        marker = &weapon->markers[i];
        if (marker->joint != (void *)0 && marker->target == target)
            return 0;
    }
    for (i = 0; i < count; i++) {
        marker = &weapon->markers[i];
        if (marker->joint == (void *)0)
            break;
    }
    if (i == count)
        return 1;
    range = weapon->range;
    if (range < zoid->distances[target->index])
        return 0;
    if (zoid != 0 && zoid->camera != 0 && sCameraPt2d(zoid->camera, (Vec3 *)target->position, pt) == 0)
        return 0;
    marker->target = target;
    marker->locked = 0;
    marker->joint = (HSD_JObj *)target->camera_joint;
    marker->source = 0;
    marker->object_index = -1;
    for (i = 0; i < 3; i++) {
        if (zoid != 0)
            marker->position[i] = ((HSD_JObj *)zoid->camera_joint)->matrix[i][3];
        else
            marker->position[i] = ((HSD_JObj *)target->camera_joint)->matrix[i][3];
    }

    marker->position[1] -= 100.0f;
    return 0;
}
s32 sWpSearchSight1g(WeaponState3 *weapon, ZoidState *zoid, s32 index)
{
    s32 i;
    s32 count;
    WeaponMarker3 *marker;
    HSD_JObj *joint;
    s32 pt[3];
    f32 range;
    f32 distance;

    if (index < 0)
        return 0;
    if (grdObjAliveN(index) == 0)
        return 0;
    if (zoid->slot == grdObjGetGrp(index))
        return 0;
    count = weapon->marker_count;
    if (count < 1)
        return 1;
    for (i = 0; i < count; i++) {
        marker = &weapon->markers[i];
        if (marker->joint != (void *)0 && marker->object_index == index)
            return 0;
    }
    for (i = 0; i < count; i++) {
        marker = &weapon->markers[i];
        if (marker->joint == (void *)0)
            break;
    }
    if (i == count)
        return 1;
    range = weapon->range;
    joint = grdObjGetJObj(index);
    if (joint == 0)
        return 0;
    distance = sLenJobj(zoid->joint, joint);
    if (range < distance)
        return 0;
    if (zoid->camera != 0 && sCameraJobjPt2d(zoid->camera, joint, pt) == 0)
        return 0;
    marker->target = 0;
    marker->locked = 0;
    marker->joint = joint;
    marker->source = 0;
    marker->object_index = index;
    for (i = 0; i < 3; i++) {
        marker->position[i] = joint->matrix[i][3];
    }

    marker->position[1] -= 50.0f;
    return 0;
}
void sWeaponSearchSightClear(WeaponState3 *weapon)
{
    s32 i;

    for (i = 0; i < 20; i++) {
        weapon->markers[i].joint = 0;
    }
}
void sWeaponSearchSightCam(WeaponState3 *weapon, ZoidState *zoid)
{
    s32 index;
    s32 i;
    s32 slot;
    s32 max;
    s32 pt[2];
    WeaponMarker3 *marker;
    ZoidState *target;
    Missile2 *missile;
    HSD_JObj *joint;

    for (i = 0; i < 20; i++) {
        marker = &weapon->markers[i];
        if (marker->joint == 0)
            continue;
        target = marker->target;
        missile = (Missile2 *)marker->source;
        if (marker->locked != 0) {
            if (target != 0 && target->death_started == 0) {
                if (((s32)(s64)vsyncN & 0x1E0) != 0)
                    continue;
                if (zoid->camera != 0 &&
                    sCameraPt2d(zoid->camera, (Vec3 *)target->position, pt) != 0)
                    continue;
                marker->joint = 0;
                continue;
            }
            if (marker->object_index != 0 && grdObjAliveN(marker->object_index) != 0) {
                if (((s32)(s64)vsyncN & 0x1E0) != 0)
                    continue;
                if (zoid->camera != 0) {
                    joint = grdObjGetJObj(marker->object_index);
                    if (sCameraJobjPt2d(zoid->camera, joint, pt) != 0)
                        continue;
                }
                marker->joint = 0;
                continue;
            }
            if (missile != 0 && missile->purge_timer == 0)
                continue;
            marker->joint = 0;
        } else {
            if (((s32)(s64)vsyncN & 0x3E0) != 0)
                continue;
            marker->joint = 0;
        }
    }
    slot = zoid->slot;
    index = 0;
    if (zoid->target_pending != 0)
        index = zoid->target_slot;
    for (i = 0; i < 16; i++, index++) {
        if (index < 0)
            index = 0;
        if (index >= 16)
            index = 0;
        target = &Zoid[index];
        if (target != zoid && target->joint != (void *)0 &&
            target->death_started == 0 && slot != target->slot) {
            if (sWpSearchSight1z(weapon, zoid, target) != 0)
                return;
        }
    }
    max = grdObjGetMax();
    for (i = 0; i < max; i++) {
        if (sWpSearchSight1g(weapon, zoid, i) != 0)
            return;
    }
    missile = missileStart;
    for (i = 0; i < 200; i++, missile++) {
        if (missile == (void *)0)
            break;
        if (missile->purge_timer == 0) {
            if (missile->joint != (void *)0) {
                (void)0;
            }
        }
    }
}
void sWeaponSearchSight(WeaponState3 *weapon)
{
    ZoidState *zoid = weapon->owner;

    if (zoid != (void *)0 && zoid->joint != (void *)0) {
        if (((u32)vsyncN & 31) == zoid->index && zoid->enemy_smoke_timer == 0) {
            sWeaponSearchSightCam(weapon, zoid);
        }
    }
}
void sLsrEpilogue(void)
{
    s32 i;
    ModelArchive *archive;

    for (i = 0; i < 11; i++) {
        archive = (ModelArchive *)sLsr[i].archive;
        if (archive != (void *)0) {
            PurgeArc(archive);
            sLsr[i].archive = 0;
            sLsr[i].source = 0;
        }
    }
}
void sLsrSet(HSD_JObj *source, ModelArchive *archive)
{
    s32 i;
    s32 r;
    s32 c;
    Lsr2 *lsr;

    if (source == (void *)0) {
        return;
    }
    if (archive == (void *)0) {
        return;
    }
    for (i = 0; i < 11; i++) {
        lsr = &sLsr[i];
        if (lsr->archive == 0) {
            lsr->source = source;
            for (r = 0; r < 3; r++) {
                for (c = 0; c < 4; c++) {
                    lsr->start[r][c] = source->matrix[r][c];
                    lsr->end[r][c] = source->matrix[r][c];
                }
            }
            lsr->archive = archive;
            return;
        }
    }
}
#pragma dont_inline off
static inline void HSD_JObjCopyMtx(HSD_JObj *jobj, f32 mtx[3][4])
{
    jobj ? (void)0 : __assert(lit_689, 1173, lit_690);
    mtx ? (void)0 : __assert(lit_689, 1174, "mtx");
    PSMTXCopy(mtx, jobj->matrix);
    jobj->flags |= 0x03800000;
    HSD_JObjSetMtxDirty(jobj);
}

void sLsrMove(void)
{
    s32 i;
    s32 r;
    s32 c;
    Lsr2 *lsr;
    Lsr1_3 *beam;

    for (i = 0; i < 11; i++) {
        lsr = &sLsr[i];
        if (lsr->archive != (void *)0) {
            for (r = 0; r < 3; r++) {
                for (c = 0; c < 4; c++) {
                    lsr->end[r][c] = lsr->start[r][c];
                    lsr->start[r][c] = lsr->source->matrix[r][c];
                }
            }
            sLsr1Set(lsr);
        }
    }
    for (i = 0; i < 400; i++) {
        beam = &sLsr1[i];
        if (beam->joint != (void *)0) {
            HSD_JObjAnimAll(beam->joint);
            if (getEndF(beam->joint) != 0) {
                sLsr1Purge(beam);
            } else {
                HSD_JObjCopyMtx(beam->joint, beam->start);
                HSD_JObjCopyMtx(beam->start_joint, beam->start);
                HSD_JObjCopyMtx(beam->end_joint, beam->end);
            }
        }
    }
}
#pragma dont_inline on
#undef lit_1574
#define lit_1574 "mtx"
s32 WeaponColor(WeaponState3 *weapon, s32 color, s32 usb)
{
    register ImageDesc3 *desc;
    TObj3 *tobj;
    void *data;
    void *image;
    s32 size;
    HSD_JObj *model;
    char name[256];

    if (color < 0)
        return 0;
    if (weapon == (void *)0)
        return 0;
    if (weapon->archive == (void *)0)
        return 0;
    if (weapon->color == color)
        return 1;
    model = weapon->archive->models[weapon->model];
    if (model == (void *)0) {
        putErr("wpColor : Weapon.jobj not found.");
        return 0;
    }
    tobj = searchTObj(model);
    if (tobj == (void *)0) {
        putErr("zoidColor : tobj not found.");
        return 0;
    }
    desc = tobj->imagedesc;
    if (desc == (void *)0) {
        putErr("wpColor : imagedesc(tobj) not found.");
        return 0;
    }
    size = calc_doltexture_size(desc->format, desc->width, desc->height);
    image = desc->image;
    sprintf(name, "weapon/all_tex/%s_%c.tpl", weapon->archive, color + 0x41);
    data = sReadFileU(name, usb);
    if (data != 0 && SetTex2(data, image, size) != 0) {
        weapon->color = color;
        free(data);
        OSReport("tex2\n");
        return 1;
    }
    if (data == (void *)0) {
        putErr("wpColor : file not found.");
        return 0;
    }
    OSReport("tex set. error   wp-tex (%d,%d,[%d])\n", desc->width, desc->height, desc->format);
    if (desc->format != 14) {
        putErr("wpColor : set Tex  error    wp-tex not CMPR");
        OSReport("wp -tex   not CMPR\n");
    }
    free(data);
    return 0;
}
#undef lit_1606
#define lit_1606 "wpColor : Weapon.jobj not found."
#undef lit_1607
#define lit_1607 "zoidColor : tobj not found."
#undef lit_1608
#define lit_1608 "wpColor : imagedesc(tobj) not found."
#undef lit_1609
#define lit_1609 "weapon/all_tex/%s_%c.tpl"
#undef lit_1610
#define lit_1610 "tex2\n"
#undef lit_1611
#define lit_1611 "wpColor : file not found."
#undef lit_1612
#define lit_1612 "tex set. error   wp-tex (%d,%d,[%d])\n"
#undef lit_1613
#define lit_1613 "wpColor : set Tex  error    wp-tex not CMPR"
#undef lit_1614
#define lit_1614 "wp -tex   not CMPR\n"
s32 sWeaponColor(s32 index, s32 a, s32 b)
{
    WeaponState3 *weapon;

    if (index < 0) {
        return 0;
    }
    if (index >= 100) {
        return 0;
    }
    weapon = &sWeapon[index];
    return WeaponColor(weapon, a, b);
}
}
