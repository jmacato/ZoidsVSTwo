#ifndef ZOIDS_GX_PIXEL_H
#define ZOIDS_GX_PIXEL_H
#include "gx_tev.h"
typedef enum _GXCompare {
    GX_NEVER,
    GX_LESS,
    GX_EQUAL,
    GX_LEQUAL,
    GX_GREATER,
    GX_NEQUAL,
    GX_GEQUAL,
    GX_ALWAYS,
} GXCompare;
typedef enum _GXZFmt16 {
    GX_ZC_LINEAR,
    GX_ZC_NEAR,
    GX_ZC_MID,
    GX_ZC_FAR,
} GXZFmt16;
typedef enum _GXPixelFmt {
    GX_PF_RGB8_Z24,
    GX_PF_RGBA6_Z24,
    GX_PF_RGB565_Z16,
    GX_PF_Z24,
    GX_PF_Y8,
    GX_PF_U8,
    GX_PF_V8,
    GX_PF_YUV420,
} GXPixelFmt;
typedef enum _GXBlendMode {
    GX_BM_NONE,
    GX_BM_BLEND,
    GX_BM_LOGIC,
    GX_BM_SUBTRACT,
    GX_MAX_BLENDMODE,
} GXBlendMode;
typedef enum _GXBlendFactor {
    GX_BL_ZERO,
    GX_BL_ONE,
    GX_BL_SRCCLR,
    GX_BL_INVSRCCLR,
    GX_BL_SRCALPHA,
    GX_BL_INVSRCALPHA,
    GX_BL_DSTALPHA,
    GX_BL_INVDSTALPHA,
    GX_BL_DSTCLR = GX_BL_SRCCLR,
    GX_BL_INVDSTCLR = GX_BL_INVSRCCLR,
} GXBlendFactor;
typedef enum _GXLogicOp {
    GX_LO_CLEAR,
    GX_LO_AND,
    GX_LO_REVAND,
    GX_LO_COPY,
    GX_LO_INVAND,
    GX_LO_NOOP,
    GX_LO_XOR,
    GX_LO_OR,
    GX_LO_NOR,
    GX_LO_EQUIV,
    GX_LO_INV,
    GX_LO_REVOR,
    GX_LO_INVCOPY,
    GX_LO_INVOR,
    GX_LO_NAND,
    GX_LO_SET,
} GXLogicOp;
typedef enum _GXFogType {
    GX_FOG_NONE = 0,
    GX_FOG_LIN = 2,
    GX_FOG_EXP = 4,
    GX_FOG_EXP2 = 5,
    GX_FOG_REVEXP = 6,
    GX_FOG_REVEXP2 = 7
} GXFogType;
typedef struct GXFogAdjTable {
    u16 r[10];
} GXFogAdjTable;
extern void GXSetFog(GXFogType type, f32 start, f32 end, f32 near, f32 far,
                     GXColor color);
extern void GXInitFogAdjTable(GXFogAdjTable *table, u16 width, f32 projection[4][4]);
extern void GXSetFieldMode(u8 field, u8 half_aspect);
extern void GXSetFogRangeAdj(u8 enable, u16 center, GXFogAdjTable *table);
extern void GXSetBlendMode(GXBlendMode mode, GXBlendFactor source,
                           GXBlendFactor destination, GXLogicOp operation);
extern void GXSetColorUpdate(u8 enable);
extern void GXSetAlphaUpdate(u8 enable);
extern void GXSetZMode(u8 compare, GXCompare function, u8 update);
extern void GXSetZCompLoc(u8 before_texture);
extern void GXSetPixelFmt(GXPixelFmt pixel_format, GXZFmt16 depth_format);
extern void GXSetDither(u8 dither);
extern void GXSetDstAlpha(u8 enable, u8 alpha);
extern void GXSetFieldMask(u8 odd, u8 even);
extern void __GXSetRange(void);
#endif
