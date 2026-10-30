#ifndef ZOIDS_GX_LIGHT_H
#define ZOIDS_GX_LIGHT_H
#include "gx_tev.h"
typedef s32 GXLightID;
typedef struct GXLightObj {
    u32 storage[16];
} GXLightObj;
#define GX_LIGHT_NULL 0
#define GX_MAX_LIGHT 256
#define GX_LIGHT0 1
#define GX_LIGHT1 2
#define GX_LIGHT2 4
#define GX_LIGHT3 8
#define GX_LIGHT4 16
#define GX_LIGHT5 32
#define GX_LIGHT6 64
#define GX_LIGHT7 128
enum {
    GX_DF_NONE,
    GX_DF_SIGN,
    GX_DF_CLAMP,
};
enum {
    GX_SRC_REG,
    GX_SRC_VTX,
};
enum {
    GX_AF_SPEC,
    GX_AF_SPOT,
    GX_AF_NONE,
};
enum {
    GX_SP_OFF,
    GX_SP_FLAT,
    GX_SP_COS,
    GX_SP_COS2,
    GX_SP_SHARP,
    GX_SP_RING1,
    GX_SP_RING2,
};
enum {
    GX_DA_OFF,
    GX_DA_GENTLE,
    GX_DA_MEDIUM,
    GX_DA_STEEP,
};
extern void GXInitLightAttn(GXLightObj *light, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1,
                            f32 k2);
extern void GXInitLightSpot(GXLightObj *light, f32 cutoff, s32 function);
extern void GXInitLightDistAttn(GXLightObj *light, f32 distance, f32 brightness,
                                s32 function);
extern void GXInitLightPos(GXLightObj *light, f32 x, f32 y, f32 z);
extern void GXInitLightDir(GXLightObj *light, f32 x, f32 y, f32 z);
extern void GXInitLightColor(GXLightObj *light, GXColor color);
extern void GXLoadLightObjImm(GXLightObj *light, GXLightID id);
extern void GXSetChanAmbColor(GXChannelID channel, GXColor color);
extern void GXSetChanMatColor(GXChannelID channel, GXColor color);
extern void GXSetNumChans(u8 count);
extern void GXSetChanCtrl(GXChannelID channel, u8 enable, s32 ambient_source,
                          s32 material_source, u32 light_mask, s32 diffuse_function,
                          s32 attenuation_function);
#endif
