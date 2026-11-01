extern "C" {
#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"

typedef struct JObjT {
    u8 pad0[0x8];
    struct JObjT *next;
    u8 pad0c[0x4];
    struct JObjT *child;
    u32 flags;
    u8 pad1[4];
    f32 rot_x;
    f32 rot_y;
    f32 rot_z;
    u8 pad1b[4];
    f32 scale_x;
    f32 scale_y;
    f32 scale_z;
    u8 pad2[0xc];
    f32 mtx[3][4];
} JObjT;
typedef struct InfoT {
    u16 dims[3];
    u16 h6;
    u16 bullet;
    s8 valid;
    s8 b11;
    s8 fire;
    s8 rotate;
    s8 b13;
    u8 pad3;
    u16 h30;
    u16 h32;
    u16 range;
    u16 h36;
    u8 pad5[8];
} InfoT;
typedef char InfoTSizeCheck[sizeof(InfoT) == 0x20 ? 1 : -1];
typedef struct GObj3 {
    JObjT *source;
    f32 pos[3];
    f32 mtx[3][4];
    f32 inv[3][4];
    s32 slot;
    JObjT *joint;
    s32 anim;
    s32 w7c;
    s32 w80;
    s32 w84;
    s32 w88;
    s32 w8c;
    f32 rotY;
    f32 rotErr;
    s32 wa0;
    InfoT *info;
} GObj3;
typedef char GObj3SizeCheck[sizeof(GObj3) == 0xA0 ? 1 : -1];
typedef struct GZoid {
    u8 pad_0000[0x40];
    JObjT *joint;
    u8 pad_0044[0x1B68 - 0x44];
    f32 position[3];
    u8 pad_1B74[0x1C2C - 0x1B74];
    u8 us_pad_1C2C[0xC];
    u8 pad_1B74_us[0x1D64 - 0x1C2C];
    s32 death_started;
    u8 pad_1D68[0x1D7D - 0x1D68];
    u8 slot;
    u8 pad_1D7E[0x1F34 - 0x1D7E];
    s32 ai_active;
    u8 pad_1F38[0x1F8C - 0x1F38];
} GZoid;
typedef char GZoidSizeCheck[sizeof(GZoid) == 0x1F98 ? 1 : -1];
extern GZoid Zoid[];
extern s32 vsyncN;
extern f32 sLenPoint3d(f32 *first, f32 *second);
extern f32 calcAng(f32 *from, f32 *to);
extern void angLimit(f32 *angle);
typedef struct GVec3 {
    f32 x;
    f32 y;
    f32 z;
} GVec3;
typedef struct GMissile {
    u8 pad_00[0x1C];
    JObjT *joint;
    GZoid *owner;
    u8 pad_24[0x40 - 0x24];
    f32 matrix[3][4];
    f32 previous[3][4];
    u8 pad_A0[0xD8 - 0xA0];
    s32 damage;
    u8 pad_DC[0x18C - 0xDC];
} GMissile;
extern GMissile missileStart[200];
typedef struct GCam {
    u8 pad_000[0x214];
    f32 relative_origin[3];
    u8 pad_220[0x254 - 0x220];
    f32 planar[2];
    u8 pad_25C[0x2FA - 0x25C];
    s8 clip_disabled;
} GCam;
extern GCam *camTmp;
extern s32 ctrlZoid;
extern f32 sPassage(f32 *origin, f32 *target, GVec3 *point);
extern s32 sDispClipOnOff;
extern void camSe(void *cam, s16 se, f32 *pos);
extern s32 sColCube4Point(GVec3 *point);
extern void PSMTXMultVecArray(f32 m[3][4], GVec3 *src, GVec3 *dst, u32 count);
extern s32 gameStartF;
extern JObjT *GetChild(JObjT *joint, s32 a, s32 b, s32 c, s32 d, s32 e);
extern void sMissileSet(void *owner, GZoid *target_zoid, void *target_missile, s32 lock, f32 mtx[3][4], s32 bullet, s32 extra_damage);
typedef struct FogT {
    u8 pad[0x18];
    s32 vi;
} FogT;
typedef struct ArcT {
    u8 pad[0x34];
    FogT *fog;
    u8 pad38[4];
    JObjT *jobj;
} ArcT;
typedef struct VIDataT {
    u8 pad[0x48];
    s32 v;
} VIDataT;
extern VIDataT HSD_VIData;
extern FogT *fogG;
extern void LoadArcU(void *arc, char *name, s32 usb);
extern void setModelShadow2(JObjT *j);
extern void getSkyAmbient(void);
extern void GroundObjLoad(s32 stage, s32 usb);
extern InfoT *sReadFileLenU(char *name, s32 usb, u32 *len);
extern void GroundObjLoadSub(JObjT *joint, s32 stage, s32 usb);
extern void GroundObjMapInfo(s32 usb);
extern s32 clipDat[241];
extern s32 nowStage;
extern void PurgeStageData(void);
extern void sColPrologue(char *name, s32 mode);
extern void LoadStageDataUSB2(s32 stage);
extern void LoadStageData2(s32 stage);
extern void jobjUnrefAll(JObjT *joint);
extern void ZeroMemory(void *dest, u32 size);
extern void PurgeArc2(ArcT *arc);
extern void free(void *ptr);
extern s32 sprintf(char *buffer, const char *format, ...);
extern void OSReport(const char *format, ...);
extern s32 usbForce;
extern ArcT *LoadArc2(char *name);
extern ArcT *LoadArc2USB(char *name);
extern void jobjAmbient(JObjT *jobj, s32 *amb);
extern void grdObjSetAnim(GObj3 *o, s32 anim);
extern void HSD_JObjSetMtxDirtySub(JObjT *j);
extern void __assert(const char *file, s32 line, const char *expr);
extern void sMakeMatrix(JObjT *j);
extern void PSMTXInverse(f32 *a, f32 *b);
extern s32 fileLen(char *name);
extern JObjT *CopyJoint2(ArcT *arc, s32 model);
extern void sGlobal2Local(JObjT *j);
extern void setModelShadow(JObjT *j);
extern f32 sColPosY(f32 *pos, s32 flag);
extern ArcT *GroundObj[20];
extern void grdDispSubXLU(JObjT *joint);
extern void grdDispSub(JObjT *joint);
extern GObj3 obj[50];
extern InfoT *MapObjDef;
extern s32 MapObjDefN;
extern s32 objNow;
extern s32 objSize;
extern s32 SkyAmbient;
extern ArcT *Sky;
extern s32 grdCamChkObjR(JObjT *joint);
extern void HSD_JObjDispAll(JObjT *joint, void *cobj, u32 flags, u32 rendermode);
extern s32 jobjAnim(JObjT *joint);
#pragma dont_inline off
#pragma auto_inline off
static inline s32 HSD_JObjMtxIsDirty(JObjT *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 605, "jobj");
    return !(jobj->flags & 0x00800000) && (jobj->flags & 0x40);
}
static inline void HSD_JObjSetRotY(JObjT *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 696, "jobj");
    !(jobj->flags & 0x20000) ? (void)0 : __assert("jobj.h", 697, "!(jobj->flags & JOBJ_USE_QUATERNION)");
    jobj->rot_y = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj))
            HSD_JObjSetMtxDirtySub(jobj);
    }
}
static inline void HSD_JObjSetScaleX(JObjT *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 805, "jobj");
    jobj->scale_x = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj))
            HSD_JObjSetMtxDirtySub(jobj);
    }
}
static inline void HSD_JObjSetScaleZ(JObjT *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 833, "jobj");
    jobj->scale_z = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj))
            HSD_JObjSetMtxDirtySub(jobj);
    }
}
static inline void HSD_JObjSetScaleY(JObjT *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 819, "jobj");
    jobj->scale_y = value;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj))
            HSD_JObjSetMtxDirtySub(jobj);
    }
}
typedef struct MapObjRec3 {
    u8 pad_00[0x20];
    s32 field_20;
    u8 pad_24[0xB2 - 0x24];
    s16 field_B2;
    u8 pad_B4[0xB6 - 0xB4];
    s16 field_B6;
    u8 pad_B8[0xBA - 0xB8];
    s16 field_BA;
    u8 pad_BC[0xBD - 0xBC];
    s8 field_BD;
    u8 pad_BE[0x24C - 0xBE];
} MapObjRec3;
extern u8 *mgP;
static inline f32 absf(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}
#pragma force_active on
ArcT *GroundObj[20];
GObj3 obj[50];
u64 Ground[8];
u64 sPad[153];
s32 clipDat[241] = {
    0, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    -1, 
};
#pragma force_active off
s32 grdObjAlive(s32 type)
{
    s32 count;
    s32 i;
    GObj3 *object;

    i = 0;
    count = 0;
    for (; i < 50; i++) {
        object = &obj[i];
        if (object->joint != (void *)0 && object->slot == type && object->anim != 2)
            count++;
    }
    return count;
}
s32 grdObjAliveN(s32 index)
{
    GObj3 *object;

    if (index < 0)
        return 0;
    if (index >= 50)
        return 0;
    object = &obj[index];
    if (object->joint == (void *)0)
        return 0;
    if (object->anim == 2)
        return 0;
    return 1;
}
JObjT *grdObjGetJObj(s32 index)
{
    register JObjT *joint;
    GObj3 *object;

    if (index < 0)
        return 0;
    if (index >= 50)
        return 0;
    object = &obj[index];
    joint = object->joint;
    return GetChild(joint, 0, 2, -1, -1, -1);
}
s32 grdObjGetGrp(s32 index)
{
    GObj3 *object;

    if (index < 0)
        return -1;
    if (index >= 50)
        return -1;
    object = &obj[index];
    if (object->joint == (void *)0)
        return -1;
    return object->w8c;
}
void GroundObjLoadSub2(JObjT *joint, s32 stage, s32 usb)
{
    char name[256];
    s32 color;
    s32 j;
    s32 i;
    s32 type;
    JObjT *copy;
    InfoT *map;
    f32 sy;

    if (usbForce != 0)
        usb = 1;
    if (joint == (void *)0)
        return;
    if (objNow >= 50)
        return;
    if (stage < 0)
        return;
    joint ? (void)0 : __assert("jobj.h", 911, "jobj");
    sy = joint->scale_y;
    type = (s32)sy - 1;
    if (type == 0)
        return;
    if (type >= 20)
        return;
    if (GroundObj[type] == (void *)0) {
        sprintf(name, "stage/map_obj/map_obj%02d.dat", type);
        if (usb != 0)
            GroundObj[type] = LoadArc2USB(name);
        else
            GroundObj[type] = LoadArc2(name);
        if (GroundObj[type] != 0) {
            jobjAmbient(GroundObj[type]->jobj, (color = SkyAmbient, &color));
            objSize += fileLen(name);
        }
    }
    if (GroundObj[type] == (void *)0)
        return;
    obj[objNow].source = joint;
    sMakeMatrix(joint);
    for (i = 0; i < 3; i++)
        obj[objNow].pos[i] = joint->mtx[i][3];
    obj[objNow].slot = type;
    copy = CopyJoint2(GroundObj[type], 0);
    obj[objNow].joint = copy;
    obj[objNow].anim = 0;
    obj[objNow].w7c = 4000;
    obj[objNow].w80 = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++)
            copy->mtx[i][j] = joint->mtx[i][j];
        obj[objNow].pos[i] = joint->mtx[i][j];
    }
    sGlobal2Local(copy);
    HSD_JObjSetScaleX(copy, 1.0f);
    HSD_JObjSetScaleY(copy, 1.0f);
    HSD_JObjSetScaleZ(copy, 1.0f);
    sMakeMatrix(copy);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++)
            obj[objNow].mtx[i][j] = copy->mtx[i][j];
    }
    PSMTXInverse(&obj[objNow].mtx[0][0], &obj[objNow].inv[0][0]);
    grdObjSetAnim(&obj[objNow], 0);
    obj[objNow].wa0 = -1;
    obj[objNow].w84 = 0;
    obj[objNow].info = 0;
    if (MapObjDef != 0 && type < MapObjDefN) {
        map = &MapObjDef[type];
        obj[objNow].info = map;
        obj[objNow].w7c = map->h6;
        obj[objNow].w80 = map->h36;
        if (map->b11 != 0)
            setModelShadow(GroundObj[type]->jobj);
        obj[objNow].w84 = map->h32;
        obj[objNow].w8c = map->b13;
        obj[objNow].w88 = map->h30;
    }
    objNow++;
}
#undef lit_481
#define lit_481 "jobj.h"
#undef lit_482
#define lit_482 "jobj"
#undef lit_483
#define lit_483 "stage/map_obj/map_obj%02d.dat"
void GroundObjLoadMapInfo(s32 type, s32 x, s32 z, s32 rot, s32 usb)
{
    InfoT *map;
    JObjT *copy;
    char name[256];
    s32 color;

    if (usbForce != 0)
        usb = 1;
    if (objNow >= 50)
        return;
    if (type < 0)
        return;
    if (type >= 20)
        return;
    if (GroundObj[type] == (void *)0) {
        sprintf(name, "stage/map_obj/map_obj%02d.dat", type);
        if (usb != 0)
            GroundObj[type] = LoadArc2USB(name);
        else
            GroundObj[type] = LoadArc2(name);
        if (GroundObj[type] != 0) {
            jobjAmbient(GroundObj[type]->jobj, (color = SkyAmbient, &color));
            objSize += fileLen(name);
        }
    }
    if (GroundObj[type] == (void *)0)
        return;
    obj[objNow].source = 0;
    obj[objNow].pos[0] = (f32)x;
    obj[objNow].pos[1] = 0.0f;
    obj[objNow].pos[2] = (f32)z;
    obj[objNow].pos[1] = sColPosY(&obj[objNow].pos[0], 0);
    obj[objNow].slot = type;
    copy = CopyJoint2(GroundObj[type], 0);
    obj[objNow].joint = copy;
    obj[objNow].anim = 0;
    obj[objNow].w7c = 4000;
    obj[objNow].w80 = 0;
    obj[objNow].rotY = 2.0 * (3.1415926 * (f32)rot) / 360.0;
    HSD_JObjSetRotY(copy, (f32)rot);
    HSD_JObjSetScaleX(copy, 1.0f);
    HSD_JObjSetScaleY(copy, 1.0f);
    HSD_JObjSetScaleZ(copy, 1.0f);
    sMakeMatrix(copy);
    for (x = 0; x < 3; x++) {
        for (z = 0; z < 4; z++)
            obj[objNow].mtx[x][z] = copy->mtx[x][z];
    }
    PSMTXInverse(&obj[objNow].mtx[0][0], &obj[objNow].inv[0][0]);
    grdObjSetAnim(&obj[objNow], 0);
    obj[objNow].wa0 = -1;
    obj[objNow].w84 = 0;
    obj[objNow].info = 0;
    if (MapObjDef != 0 && type < MapObjDefN) {
        map = &MapObjDef[type];
        obj[objNow].info = map;
        obj[objNow].w7c = map->h6;
        obj[objNow].w80 = map->h36;
        if (map->b11 != 0)
            setModelShadow(GroundObj[type]->jobj);
        obj[objNow].w84 = map->h32;
        obj[objNow].w88 = map->h30;
    }
    OSReport("obj %d from mapInfo\n", type);
    copy = copy;
    objNow++;
}
#undef lit_545
#define lit_545 "!(jobj->flags & JOBJ_USE_QUATERNION)"
#undef lit_546
#define lit_546 "obj %d from mapInfo\n"

void GroundObjMapInfo(s32 usb)
{
    MapObjRec3 *rec;
    s32 i;

    if (mgP != 0) {
        for (i = 0; i < 0x11; i++) {
            rec = (MapObjRec3 *)((u8 *)mgP + (i * 0x24C) + 0x12C);
            if (rec->field_20 == 2 && rec->field_BD != 0)
                GroundObjLoadMapInfo(rec->field_BD, *(s32 *)((u8 *)rec + 0xB0), *(s32 *)((u8 *)rec + 0xB8), *(s32 *)((u8 *)rec + 0xB4), usb);
        }
    }
}
void GroundObjLoad(s32 stage, s32 usb)
{
    JObjT *joint;
    u32 len;

    if (usbForce != 0)
        usb = 1;
    MapObjDef = sReadFileLenU("stage/map_obj/def.bin", usb, &len);
    if (MapObjDef == (void *)0)
        MapObjDefN = 0;
    else
        MapObjDefN = len >> 5;
    joint = *(JObjT **)((u8 *)Ground + 0x3C);
    if (joint != (void *)0) {
        sMakeMatrix(joint);
        joint = joint != 0 ? joint->child : 0;
        objSize = 0;
        for (; joint != 0; joint = joint != 0 ? joint->next : 0)
            GroundObjLoadSub(joint, stage, usb);
        sMakeMatrix(*(JObjT **)((u8 *)Ground + 0x3C));
        GroundObjMapInfo(usb);
        OSReport("ground stage=%d , obj=%d   objSize=%d  \n", stage, objNow, objSize);
    }
}
#undef lit_583
#define lit_583 "stage/map_obj/def.bin"
#undef lit_584
#define lit_584 "ground stage=%d , obj=%d   objSize=%d  \n"
void GroundObjPurge(void)
{
    s32 i;
    GObj3 *o;
    for (i = 0; i < 50; i++) {
        o = &obj[i];
        if (o->joint != 0)
            jobjUnrefAll(o->joint);
        o->joint = 0;
        ZeroMemory(o, sizeof(GObj3));
    }
    for (i = 0; i < 20; i++) {
        if (GroundObj[i] != 0) {
            PurgeArc2(GroundObj[i]);
            GroundObj[i] = 0;
        }
    }
    objNow = 0;
    if (MapObjDef != 0)
        free(MapObjDef);
    MapObjDef = 0;
    MapObjDefN = 0;
}
s32 LoadStageData2__(s32 stage, s32 usb)
{
    char sky[40];
    char name[256];
    s32 tmp[1];
    s32 amb;

    if (usbForce != 0)
        usb = 1;
    sprintf(name, "stage/st%02d.dat", stage);
    LoadArcU(Ground, name, usb);
    setModelShadow2(*(JObjT **)((u8 *)Ground + 0x3C));
    sprintf(sky, "stage/skymap%02d.dat", stage);
    if (usb != 0)
        Sky = LoadArc2USB(sky);
    else
        Sky = LoadArc2(sky);
    fogG = 0;
    if (Sky != 0)
        fogG = Sky->fog;
    if (fogG != 0) {
        tmp[0] = fogG->vi;
        HSD_VIData.v = tmp[0];
    }
    getSkyAmbient();
    jobjAmbient(*(JObjT **)((u8 *)Ground + 0x3C), (amb = SkyAmbient, &amb));
    GroundObjLoad(stage, usb);
    return 1;
}
#undef lit_646
#define lit_646 "stage/st%02d.dat"
#undef lit_647
#define lit_647 "stage/skymap%02d.dat"
s32 grdCamChkObjR(JObjT *joint)
{
    f32 dx;
    f32 dz;
    f32 side;
    f32 forward;

    if (joint == 0)
        return 0;
    dx = joint->mtx[0][3] - camTmp->relative_origin[0];
    dz = joint->mtx[2][3] - camTmp->relative_origin[2];
    if (camTmp->clip_disabled != 0)
        return 1;
    if (sDispClipOnOff == 2) {
        if (absf(dx) > 800.0f)
            return 0;
        if (absf(dz) > 800.0f)
            return 0;
    }
    if (dx * dx + dz * dz > 640000.0f)
        return 0;
    side = dx * camTmp->planar[0] - dz * camTmp->planar[1];
    forward = dx * camTmp->planar[1] + dz * camTmp->planar[0];
    if (forward < -800.0f)
        return 0;
    if (forward > 1100.0f)
        return 0;
    if (side > 1000.0f)
        return 0;
    if (side < -1000.0f)
        return 0;
    if (side - forward > 800.0f)
        return 0;
    if (side + forward < -800.0f)
        return 0;
    return 1;
}
#undef lit_710
#define lit_710 "I\x1c@"
#undef lit_712
#define lit_712 "D\x89\x80"
f32 grdCamChkS(JObjT *joint)
{
    GVec3 point;
    point.x = joint->mtx[0][3];
    point.y = joint->mtx[1][3];
    point.z = joint->mtx[2][3];
    return sPassage(camTmp->relative_origin, Zoid[ctrlZoid].position, &point);
}
void grdObjDispOpa(void)
{
    GObj3 *object;
    s32 i;

    for (i = 0; i < objNow; i++) {
        object = &obj[i];
        if (object->joint != (void *)0 && grdCamChkObjR(object->joint) != 0) {
            HSD_JObjSetRotY(object->joint, object->rotY);
            HSD_JObjDispAll(object->joint, 0, 1, 0);
        }
    }
}
void grdObjDispXLU(void)
{
    GObj3 *object;
    JObjT *joint;
    s32 i;

    for (i = 0; i < objNow; i++) {
        object = &obj[i];
        if (object->joint != (void *)0 && grdCamChkObjR(object->joint) != 0) {
            joint = object->joint;
            HSD_JObjSetRotY(joint, object->rotY);
            HSD_JObjDispAll(object->joint, 0, 2, 0);
            HSD_JObjDispAll(object->joint, 0, 4, 0);
        }
    }
}
void grdDisp(void)
{
    JObjT *joint = *(JObjT **)((u8 *)Ground + 0x3C);

    if (obj != 0) {
        if (sDispClipOnOff == 0) {
            HSD_JObjDispAll(*(JObjT **)((u8 *)Ground + 0x3C), 0, 1, 0);
        } else {
            grdDispSub((*(JObjT **)((u8 *)Ground + 0x3C))->child);
        }
        grdObjDispOpa();
    }
}
void grdDispXLU(void)
{
    if (sDispClipOnOff == 0) {
        HSD_JObjDispAll(*(JObjT **)((u8 *)Ground + 0x3C), 0, 2, 0);
        HSD_JObjDispAll(*(JObjT **)((u8 *)Ground + 0x3C), 0, 4, 0);
    } else {
        grdDispSubXLU((*(JObjT **)((u8 *)Ground + 0x3C))->child);
    }
    grdObjDispXLU();
}
void grdObjAnim(void)
{
    GObj3 *p;
    s32 j;
    s32 i;

    for (i = 0; i < 50; i++) {
        p = &obj[i];
        if (p->joint != (void *)0) {
            for (j = 0; j < 3; j++)
                p->pos[j] = p->joint->mtx[j][3];
            if (p->anim == 0) {
                jobjAnim(p->joint);
            } else if (p->anim != 2) {
                if (jobjAnim(p->joint) != 0 && p->anim == 1) {
                    grdObjSetAnim(p, 2);
                    jobjAnim(p->joint);
                }
                HSD_JObjSetScaleY(p->joint, 1.0f);
            }
        }
    }
    jobjAnim(*(JObjT **)((u8 *)Ground + 0x3C));
    sMakeMatrix(*(JObjT **)((u8 *)Ground + 0x3C));
    jobjAnim(Sky->jobj);
}
void ssLoadStageNow(s32 stage, s32 mode)
{
    char dat_name[40];
    char xb_name[40];
    s32 clip;

    if (stage >= 0) {
        clip = clipDat[stage];
        if (clip == 9)
            stage = 3;
        OSReport("load stage %d    clip=%d\n", stage, clip);
        clip = clipDat[stage];
        sDispClipOnOff = clip;
        (void)(clip == nowStage);
        if (nowStage != -1)
            PurgeStageData();
        sprintf(dat_name, "st%02d.dat", stage);
        sprintf(xb_name, "st%02d.xb", stage);
        sColPrologue(xb_name, mode);
        if (mode != 0)
            LoadStageDataUSB2(stage);
        else
            LoadStageData2(stage);
        nowStage = stage;
    }
}
#undef lit_867
#define lit_867 "load stage %d    clip=%d\n"
#undef lit_868
#define lit_868 "st%02d.dat"
#undef lit_869
#define lit_869 "st%02d.xb"
#pragma force_active on
static const f32 lit_738[1] = { 250.0f };
static const f32 lit_739[1] = { 1210000.0f };
static const f32 lit_740[1] = { -390.0f };
static const f32 lit_741[1] = { 600.0f };
static const f32 lit_742[1] = { -600.0f };
#pragma force_active off
s32 grdObjIn(GObj3 *o, GMissile *missile)
{
    InfoT *info;
    GVec3 local[2];
    GVec3 points[2];
    s32 slot;
    s32 i;
    s32 k;

    if (o->anim != 0)
        return 1;
    info = o->info;
    slot = o->slot;
    if (info == 0) {
        if ((vsyncN & 0x10) == 0)
            OSReport("MapObjDef\202\252\221\253\202\350\202\334\202\271\202\361\201A\222\350\213`\202\265\202\304\202\255\202\276\202\263\202\242\201B\n");
        return 0;
    }
    if (info->valid == 0)
        return 0;
    for (i = 0; i < 3; i++) {
        if (info->dims[i] == 0)
            return 0;
    }
    for (i = 0; i < 3; i++) {
        (&points[0].x)[i] = missile->matrix[i][3];
        (&points[1].x)[i] = missile->previous[i][3];
    }
    PSMTXMultVecArray(o->inv, points, local, 2);
    for (i = 0; i < 2; i++) {
        for (k = 0; k < 3; k++)
            (&local[i].x)[k] = (&local[i].x)[k] / (f32)info->dims[k];
    }
    if (sColCube4Point(&local[0]) == 0 && sColCube4Point(&local[1]) == 0)
        return 0;
    o->w7c -= missile->damage;
    if (o->w7c < 0)
        o->w7c = 0;
    if (o->w7c > 0)
        return 1;
    grdObjSetAnim(o, 1);
    if (o->w80 != 0)
        camSe(camTmp, o->w80, o->pos);
    return 1;
}
#undef lit_925
#define lit_925 "MapObjDef\x82\xaa\x91\xab\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81" "A\x92\xe8\x8b`\x82\xb5\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2\x81" "B\n"
void grdObjFire(GObj3 *o)
{
    f32 matrix[3][4];
    InfoT *info;
    GZoid *zoid;
    JObjT *child;
    s32 index;
    s32 i;
    s32 j;

    info = o->info;
    child = GetChild(o->joint, 0, 0, -1, -1, -1);
    if (child == 0)
        OSReport("fire ========NULL\n");
    if (child == 0)
        return;
    if (info == 0)
        return;
    if (gameStartF != 0)
        return;
    if (o->anim != 0)
        return;
    if (o->w84 != 0)
        o->w84--;
    if (o->w84 != 0)
        return;
    if (o->w88 == 0)
        return;
    index = o->wa0;
    if (index == -1)
        return;
    zoid = &Zoid[index];
    if (zoid->joint == 0)
        return;
    if (zoid->death_started != 0)
        return;
    if (absf(o->rotErr) > 0.08726646111111111)
        return;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++)
            matrix[i][j] = child->mtx[i][j];
    }
    sMissileSet(0, zoid, 0, -1, matrix, info->bullet, 0);
    o->w88--;
    o->w84 = info->h32;
}
#undef lit_964
#define lit_964 "fire ========NULL\n"
void grdObjRot(GObj3 *o)
{
    s32 i;
    GZoid *zoid;
    InfoT *info;
    s32 best;
    f32 distance;
    f32 best_distance;
    f32 angle;
    f32 value;
    f32 value2;

    info = o->info;
    if (info == 0)
        return;
    if (o->anim != 0)
        return;
    best = -1;
    if ((vsyncN & 0x3F) == 0) {
        o->wa0 = -1;
        for (i = 0; i < 16; i++) {
            zoid = &Zoid[i];
            if (zoid->joint == 0)
                continue;
            if (zoid->death_started != 0)
                continue;
            if (info->b13 == zoid->slot)
                continue;
            distance = sLenPoint3d(o->pos, zoid->position);
            if (distance > info->range)
                continue;
            if (best != -1 && !(distance < best_distance))
                continue;
            best = i;
            best_distance = distance;
        }
        o->wa0 = best;
    }
    if (o->wa0 == -1)
        return;
    zoid = &Zoid[o->wa0];
    if (zoid->joint == 0)
        return;
    if (zoid->death_started != 0)
        return;
    angle = calcAng(zoid->position, o->pos);
    if (-100.0f == angle)
        return;
    value = o->rotY;
    if (angle > value) {
        value += 0.005235987874726123;
        if (angle < value)
            value = angle;
    }
    if (angle < value) {
        value -= 0.005235987874726123;
        if (angle > value)
            value = angle;
    }
    angLimit(&value);
    o->rotY = value;
    HSD_JObjSetRotY(o->joint, value);
    value2 = angle - value;
    angLimit(&value2);
    o->rotErr = value2;
}
void grdObjTest(void)
{
    GObj3 *object;
    InfoT *info;
    GMissile *missile;
    s32 i;
    s32 j;

    for (i = 0; i < objNow; i++) {
        object = &obj[i];
        info = object->info;
        if (object->joint != (void *)0 && info != 0) {
            if (info->valid != 0) {
                for (j = 0, missile = missileStart; j < 200; j++, missile++) {
                    if (missile->joint != (void *)0 && missile->owner != (void *)0 && missile->owner->ai_active == 0)
                        grdObjIn(object, missile);
                }
            }
            if (info->rotate != 0)
                grdObjRot(object);
            if (info->fire != 0)
                grdObjFire(object);
        }
    }
}
#pragma force_active on
ArcT *Sky;
InfoT *MapObjDef;
s32 MapObjDefN;
s32 objNow;
s32 objSize;
u32 nowShadow;
s32 SkyAmbient;
FogT *fogG;
s32 nowStage = -1;
s32 sDispClipOnOff = 1;
u32 sPad1 = 0x80151A08;
u32 sPad2 = 0x80151A90;
u32 sPad3 = 0x80151B18;
u32 sPad4 = 0x80151BA0;
u32 padBy = 0x000000CA;
#pragma force_active off
}
