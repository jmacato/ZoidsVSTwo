#ifndef ZOIDS_LOBJ_H
#define ZOIDS_LOBJ_H
/* Layout follows
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/lobj.h. */
#include "object.h"
#include "wobj.h"
#include "gx_light.h"
#include "list.h"
#include "matrix.h"
typedef struct HSD_LightPoint {
    f32 cutoff;
    u32 point_func;
    f32 ref_br, ref_dist;
    u32 dist_func;
} HSD_LightPoint;
typedef struct HSD_LightSpot {
    f32 cutoff;
    u32 spot_func;
    f32 ref_br, ref_dist;
    u32 dist_func;
} HSD_LightSpot;
typedef struct HSD_LightPointDesc {
    f32 ref_br, ref_dist;
    u32 dist_func;
} HSD_LightPointDesc;
typedef HSD_LightSpot HSD_LightSpotDesc;
typedef struct HSD_LightAttn {
    f32 a0, a1, a2, k0, k1, k2;
} HSD_LightAttn;
typedef struct HSD_LObj HSD_LObj;
typedef struct HSD_LightDesc HSD_LightDesc;
typedef struct HSD_LObjInfo {
    HSD_ClassInfo parent;
    s32 (*load)(HSD_LObj *, HSD_LightDesc *);
    AnimationUpdate update;
} HSD_LObjInfo;
struct HSD_LObj {
    HSD_Obj parent;
    u16 flags, priority;
    HSD_LObj *next;
    GXColor color, hw_color;
    HSD_WObj *position, *interest;
    union {
        HSD_LightPoint point;
        HSD_LightSpot spot;
        HSD_LightAttn attn;
    } u;
    f32 shininess;
    Vec3 lvec;
    HSD_AObj *aobj;
    GXLightID id;
    GXLightObj lightobj;
    GXLightID spec_id;
    GXLightObj spec_lightobj;
};
struct HSD_LightDesc {
    char *class_name;
    HSD_LightDesc *next;
    u16 flags, attnflags;
    GXColor color;
    HSD_WObjDesc *position, *interest;
    union {
        void *p;
        f32 *shininess;
        HSD_LightPointDesc *point;
        HSD_LightSpotDesc *spot;
        HSD_LightAttn *attn;
    } u;
};
typedef struct HSD_LightAnim {
    struct HSD_LightAnim *next;
    HSD_AObjDesc *aobjdesc;
    HSD_WObjAnim *position_anim, *interest_anim;
} HSD_LightAnim;
struct HSD_CObj;
extern HSD_LObjInfo hsdLObj;
#define HSD_A_L_LITC_R 9
#define HSD_A_L_LITC_G 10
#define HSD_A_L_LITC_B 11
#define HSD_A_L_VIS 12
#define HSD_A_L_A0 13
#define HSD_A_L_A1 14
#define HSD_A_L_A2 15
#define HSD_A_L_K0 16
#define HSD_A_L_K1 17
#define HSD_A_L_K2 18
#define HSD_A_L_CUTOFF 19
#define HSD_A_L_REFDIST 20
#define HSD_A_L_REFBRIGHT 21
#define HSD_A_L_LITC_A 22
#define LOBJ_AMBIENT (0 << 0)
#define LOBJ_INFINITE (1 << 0)
#define LOBJ_POINT (2 << 0)
#define LOBJ_SPOT (3 << 0)
#define LOBJ_FLAGS_B1 (1 << 1)
#define LOBJ_DIFFUSE (1 << 2)
#define LOBJ_SPECULAR (1 << 3)
#define LOBJ_ALPHA (1 << 4)
#define LOBJ_HIDDEN (1 << 5)
#define LOBJ_RAW_PARAM (1 << 6)
#define LOBJ_DIFF_DIRTY (1 << 7)
#define LOBJ_SPEC_DIRTY (1 << 8)
#define LOBJ_TYPE_MASK (LOBJ_INFINITE | LOBJ_FLAGS_B1)
#define LOBJ_LIGHT_ATTN_NONE 0
#define LOBJ_LIGHT_ATTN 1
#define MAX_GXLIGHT 9
extern void __assert(const char *, u32, const char *);
extern void PSVECSubtract(Vec3 *, Vec3 *, Vec3 *);
extern void PSVECAdd(Vec3 *, Vec3 *, Vec3 *);
extern void PSMTXMultVec(Mtx, Vec3 *, Vec3 *);
extern void PSMTXMultVecSR(Mtx, Vec3 *, Vec3 *);
extern void HSD_WObjGetPosition(HSD_WObj *, Vec3 *);
extern void HSD_WObjInterpretAnim(HSD_WObj *);
extern void HSD_WObjReqAnim(HSD_WObj *, f32);
extern void HSD_WObjAddAnim(HSD_WObj *, HSD_WObjAnim *);
extern HSD_WObj *HSD_WObjLoadDesc(HSD_WObjDesc *);
extern void OSReport(const char *, ...);
extern void HSD_Panic(const char *, u32, const char *);
extern s32 hsdIsDescendantOf(HSD_ClassInfo *, HSD_ClassInfo *);
extern GXLightID HSD_LObjGetLightMaskDiffuse(void);
extern s32 HSD_LObjGetLightMaskAttnFunc(void);
extern s32 HSD_LObjGetLightMaskAlpha(void);
extern s32 HSD_LObjGetLightMaskSpecular(void);
extern s32 HSD_LObjGetNbActive(void);
extern HSD_LObj *HSD_LObjGetActiveByID(GXLightID id);
extern HSD_LObj *HSD_LObjGetActiveByIndex(s32 idx);
extern void HSD_LObjAnimAll(HSD_LObj *lobj);
extern void HSD_LObjReqAnimAll(HSD_LObj *lobj, f32 startframe);
extern void HSD_LObjGetLightVector(HSD_LObj *lobj, Vec3 *dir);
extern void HSD_LObjSetup(HSD_LObj *lobj, GXColor color, f32 shininess);
extern void HSD_LObjSetupSpecularInit(Mtx pmtx);
extern void HSD_LObjSetupInit(struct HSD_CObj *cobj);
extern void HSD_LObjAddCurrent(HSD_LObj *lobj);
extern void HSD_LObjDeleteCurrent(HSD_LObj *lobj);
extern void HSD_LObjDeleteCurrentAll(HSD_LObj *lobj);
extern void HSD_LObjSetCurrentAll(HSD_LObj *lobj);
extern HSD_LObj *HSD_LObjGetCurrentByType(u16 flags);
extern u32 HSD_LightID2Index(GXLightID id);
extern s32 HSD_Index2LightID(u32 index);
extern void HSD_LObjRemoveAll(HSD_LObj *lobj);
extern void HSD_LObjSetPosition(HSD_LObj *lobj, Vec3 *position);
extern s32 HSD_LObjGetPosition(HSD_LObj *lobj, Vec3 *position);
extern s32 HSD_LObjGetInterest(HSD_LObj *lobj, Vec3 *interest);
extern HSD_WObj *HSD_LObjGetPositionWObj(HSD_LObj *lobj);
extern HSD_WObj *HSD_LObjGetInterestWObj(HSD_LObj *lobj);
extern HSD_LObj *HSD_LObjLoadDesc(HSD_LightDesc *ldesc);
extern void HSD_LObjAddAnim(HSD_LObj *lobj, HSD_LightAnim *lanim);

#endif
