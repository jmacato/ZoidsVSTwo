#ifndef ZOIDS_GX_ATTR_H
#define ZOIDS_GX_ATTR_H
#include "gx_tev.h"
#include "gx_vertex_types.h"
enum {
    GX_TG_MTX3x4,
    GX_TG_MTX2x4,
    GX_TG_BUMP0,
    GX_TG_BUMP1,
    GX_TG_BUMP2,
    GX_TG_BUMP3,
    GX_TG_BUMP4,
    GX_TG_BUMP5,
    GX_TG_BUMP6,
    GX_TG_BUMP7,
    GX_TG_SRTG,
};
enum {
    GX_TG_POS,
    GX_TG_NRM,
    GX_TG_BINRM,
    GX_TG_TANGENT,
    GX_TG_TEX0,
    GX_TG_TEX1,
    GX_TG_TEX2,
    GX_TG_TEX3,
    GX_TG_TEX4,
    GX_TG_TEX5,
    GX_TG_TEX6,
    GX_TG_TEX7,
    GX_TG_TEXCOORD0,
    GX_TG_TEXCOORD1,
    GX_TG_TEXCOORD2,
    GX_TG_TEXCOORD3,
    GX_TG_TEXCOORD4,
    GX_TG_TEXCOORD5,
    GX_TG_TEXCOORD6,
    GX_TG_COLOR0,
    GX_TG_COLOR1,
};
extern void GXSetVtxDesc(GXAttr attr, GXAttrType type);
extern void GXClearVtxDesc(void);
extern void GXSetVtxAttrFmt(u32 format, GXAttr attr, GXCompCnt count, GXCompType type,
                            u8 fraction);
extern void GXSetArray(GXAttr attr, const void *base, u8 stride);
extern void GXInvalidateVtxCache(void);
extern void GXSetTexCoordGen2(GXTexCoordID coord, s32 function, s32 source, u32 matrix,
                              u8 normalize, u32 post_matrix);
extern void GXSetNumTexGens(u8 count);
#endif
