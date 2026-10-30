#ifndef ZOIDS_GX_TRANSFORM_H
#define ZOIDS_GX_TRANSFORM_H
#include "types.h"
#include "gx_vertex_types.h"
typedef s32 GXProjectionType;
#define GX_PERSPECTIVE 0
#define GX_ORTHOGRAPHIC 1
typedef enum _GXTexMtxType {
    GX_MTX3x4,
    GX_MTX2x4,
} GXTexMtxType;
typedef enum _GXClipMode {
    GX_CLIP_ENABLE = 0,
    GX_CLIP_DISABLE = 1,
} GXClipMode;
#define GX_PNMTX0 0
#define GX_TEXMTX0 30
#define GX_PTTEXMTX0 64
#define GX_PTIDENTITY 125
void GXProject(f32 x, f32 y, f32 z, f32 mtx[3][4], f32 *pm, f32 *vp, f32 *sx, f32 *sy,
               f32 *sz);
void GXSetProjection(f32 mtx[4][4], GXProjectionType type);
void GXGetProjectionv(f32 *ptr);
void GXLoadPosMtxImm(f32 mtx[3][4], u32 id);
void GXLoadNrmMtxImm(f32 mtx[3][4], u32 id);
void GXSetCurrentMtx(u32 id);
void GXLoadTexMtxImm(f32 mtx[][4], u32 id, GXTexMtxType type);
void GXSetViewportJitter(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz,
                         u32 field);
void GXSetViewport(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz);
void GXGetViewportv(f32 *vp);
void GXSetScissor(u32 left, u32 top, u32 wd, u32 ht);
void GXSetScissorBoxOffset(s32 x_off, s32 y_off);
void GXSetClipMode(GXClipMode mode);
void __GXSetMatrixIndex(s32 matIdxAttr);
#endif
