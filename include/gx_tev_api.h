#ifndef ZOIDS_GX_TEV_API_H
#define ZOIDS_GX_TEV_API_H
#include "gx_tev.h"
#include "gx_pixel.h"
typedef enum _GXTevMode {
    GX_MODULATE,
    GX_DECAL,
    GX_BLEND,
    GX_REPLACE,
    GX_PASSCLR,
} GXTevMode;
#define GX_TEV_ADD 0
#define GX_TEV_SUB 1
#define GX_TEV_COMP_R8_GT 8
#define GX_TEV_COMP_R8_EQ 9
#define GX_TEV_COMP_GR16_GT 10
#define GX_TEV_COMP_GR16_EQ 11
#define GX_TEV_COMP_BGR24_GT 12
#define GX_TEV_COMP_BGR24_EQ 13
#define GX_TEV_COMP_RGB8_GT 14
#define GX_TEV_COMP_RGB8_EQ 15
#define GX_TEV_COMP_A8_GT 14
#define GX_TEV_COMP_A8_EQ 15

#define GX_TB_ZERO 0
#define GX_TB_ADDHALF 1
#define GX_TB_SUBHALF 2
#define GX_MAX_TEVBIAS 3

#define GX_CS_SCALE_1 0
#define GX_CS_SCALE_2 1
#define GX_CS_SCALE_4 2
#define GX_CS_DIVIDE_2 3
#define GX_MAX_TEVSCALE 4

typedef enum _GXTevColorChan {
    GX_CH_RED = 0,
    GX_CH_GREEN,
    GX_CH_BLUE,
    GX_CH_ALPHA
} GXTevColorChan;
typedef enum _GXAlphaOp {
    GX_AOP_AND,
    GX_AOP_OR,
    GX_AOP_XOR,
    GX_AOP_XNOR,
    GX_MAX_ALPHAOP,
} GXAlphaOp;
typedef enum _GXZTexOp {
    GX_ZT_DISABLE,
    GX_ZT_ADD,
    GX_ZT_REPLACE,
    GX_MAX_ZTEXOP,
} GXZTexOp;
void GXSetTevOp(GXTevStageID id, GXTevMode mode);
void GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                     GXTevColorArg c, GXTevColorArg d);
void GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b,
                     GXTevAlphaArg c, GXTevAlphaArg d);
void GXSetTevColorOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale,
                     u8 clamp, GXTevRegID out_reg);
void GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale,
                     u8 clamp, GXTevRegID out_reg);
void GXSetTevColor(GXTevRegID id, GXColor color);
void GXSetTevColorS10(GXTevRegID id, GXColorS10 color);
void GXSetTevKColor(GXTevKColorID id, GXColor color);
void GXSetTevKColorSel(GXTevStageID stage, GXTevKColorSel sel);
void GXSetTevKAlphaSel(GXTevStageID stage, GXTevKAlphaSel sel);
void GXSetTevSwapMode(GXTevStageID stage, GXTevSwapSel ras_sel, GXTevSwapSel tex_sel);
void GXSetTevSwapModeTable(GXTevSwapSel table, GXTevColorChan red, GXTevColorChan green,
                           GXTevColorChan blue, GXTevColorChan alpha);
void GXSetTevClampMode(void);
void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1,
                       u8 ref1);
void GXSetZTexture(GXZTexOp op, s32 fmt, u32 bias);
void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map,
                   GXChannelID color);
void GXSetNumTevStages(u8 nStages);
#endif
