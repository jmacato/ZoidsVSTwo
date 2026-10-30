#ifndef ZOIDS_GX_GEOMETRY_H
#define ZOIDS_GX_GEOMETRY_H
#include "gx_tev.h"
typedef enum _GXPrimitive {
    GX_QUADS = 0x80,
    GX_TRIANGLES = 0x90,
    GX_TRIANGLESTRIP = 0x98,
    GX_TRIANGLEFAN = 0xA0,
    GX_LINES = 0xA8,
    GX_LINESTRIP = 0xB0,
    GX_POINTS = 0xB8,
} GXPrimitive;
typedef enum _GXVtxFmt {
    GX_VTXFMT0,
    GX_VTXFMT1,
    GX_VTXFMT2,
    GX_VTXFMT3,
    GX_VTXFMT4,
    GX_VTXFMT5,
    GX_VTXFMT6,
    GX_VTXFMT7,
    GX_MAX_VTXFMT,
} GXVtxFmt;
typedef enum _GXTexOffset {
    GX_TO_ZERO,
    GX_TO_SIXTEENTH,
    GX_TO_EIGHTH,
    GX_TO_FOURTH,
    GX_TO_HALF,
    GX_TO_ONE,
    GX_MAX_TEXOFFSET,
} GXTexOffset;
typedef enum _GXCullMode {
    GX_CULL_NONE,
    GX_CULL_FRONT,
    GX_CULL_BACK,
    GX_CULL_ALL

} GXCullMode;
extern void GXBegin(GXPrimitive primitive, GXVtxFmt format, u16 count);
extern void GXSetLineWidth(u8 width, GXTexOffset offset);
extern void GXSetPointSize(u8 size, GXTexOffset offset);
extern void GXEnableTexOffsets(GXTexCoordID coord, u8 line, u8 point);
extern void GXSetCullMode(GXCullMode mode);
extern void GXSetCoPlanar(u8 enable);
#endif
