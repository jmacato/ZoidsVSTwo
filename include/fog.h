#ifndef ZOIDS_FOG_H
#define ZOIDS_FOG_H
#include "object.h"
#include "aobj.h"
#include "gx_tev.h"
typedef f32 Mtx44[4][4];
/* HSD_FogAdj.flags */
#define HSD_FOGADJ_USE_CENTER 1
#define HSD_FOGADJ_USE_WIDTH 2
#define HSD_FOGADJ_USE_MTX 4

typedef struct HSD_FogAdj {
    HSD_Obj parent;
    u32 flags;
    s16 center;
    u16 width;
    Mtx44 matrix;
    HSD_AObj *animation;
} HSD_FogAdj;
typedef struct HSD_Fog {
    HSD_Obj parent;
    u32 type;
    HSD_FogAdj *adjustment;
    f32 start;
    f32 end;
    GXColor color;
    HSD_AObj *animation;
} HSD_Fog;
typedef struct HSD_FogAdjDesc {
    u32 flags;
    s16 center;
    u16 width;
    Mtx44 matrix;
} HSD_FogAdjDesc;
typedef struct HSD_FogDesc {
    u32 type;
    HSD_FogAdjDesc *adjustment;
    f32 start;
    f32 end;
    GXColor color;
} HSD_FogDesc;
typedef struct HSD_FogInfo {
    HSD_ClassInfo parent;
    AnimationUpdate update;
} HSD_FogInfo;
extern HSD_FogInfo hsdFog;
extern HSD_Fog *HSD_FogLoadDesc(HSD_FogDesc *description);
extern HSD_ClassInfo hsdFogAdj;
extern void HSD_FogInit(HSD_Fog *fog, HSD_FogDesc *description);
extern void HSD_FogAdjInit(HSD_FogAdj *adjustment, HSD_FogAdjDesc *description);
extern HSD_FogAdj *HSD_FogAdjLoadDesc(HSD_FogAdjDesc *description);
extern u32 HSD_FogAdjGetFlags(HSD_FogAdj *adjustment);
#endif
