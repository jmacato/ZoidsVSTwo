/* Reference: https://github.com/doldecomp/sms/blob/main/libs/THPPlayer/src/THPDraw.c.
 */
#include "thp_player.h"
#include "initialize.h"
#include "gx_state.h"
#include "gx_pixel.h"
#include "gx_tev_api.h"
#include "gx_attr.h"
#include "gx_transform.h"
#include "gx_texture.h"
#include "gx_framebuffer.h"
#include "gx_geometry.h"
#include "matrix.h"
typedef f32 Mtx44[4][4];
extern void C_MTXOrtho(Mtx44, f32, f32, f32, f32, f32, f32);
extern void GXSetNumChans(u8);
#define GX_TRUE 1
#define GX_FALSE 0
#define GX_DISABLE 0
#define GX_ENABLE 1
#define GX_IDENTITY 60
static inline void GXSetTexCoordGen(GXTexCoordID coord, s32 type, s32 source,
                                    u32 matrix)
{
    GXSetTexCoordGen2(coord, type, source, matrix, GX_FALSE, GX_PTIDENTITY);
}
static inline void GXPosition3s16(s16 x, s16 y, s16 z)
{
    GXWGFifo.s16 = x;
    GXWGFifo.s16 = y;
    GXWGFifo.s16 = z;
}
static inline void GXTexCoord2u16(u16 s, u16 t)
{
    GXWGFifo.u16 = s;
    GXWGFifo.u16 = t;
}

void THPGXRestore(void)
{
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE,
                          GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN,
                          GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE,
                          GX_CH_ALPHA);
}

void THPGXYuv2RgbSetup(GXRenderModeObj *rmode)
{
    s32 w, h;
    Mtx44 m;
    Mtx e_m;

    w = rmode->fbWidth;
    h = rmode->efbHeight;

    GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);

    C_MTXOrtho(m, 0.0f, h, 0.0f, w, 0.0f, -1.0f);
    GXSetProjection(m, GX_ORTHOGRAPHIC);
    GXSetViewport(0.0f, 0.0f, w, h, 0.0f, 1.0f);
    GXSetScissor(0, 0, w, h);

    PSMTXIdentity(e_m);
    GXLoadPosMtxImm(e_m, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetColorUpdate(TRUE);
    GXSetAlphaUpdate(FALSE);
    GXSetDispCopyGamma(GX_GM_1_0);
    GXSetNumChans(0);

    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXInvalidateTexAll();

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0);

    GXSetNumTevStages(4);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE,
                    GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE,
                    GX_TEVPREV);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_DISABLE,
                    GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE,
                    GX_TEVPREV);
    GXSetTevKColorSel(GX_TEVSTAGE1, GX_TEV_KCSEL_K1);
    GXSetTevKAlphaSel(GX_TEVSTAGE1, GX_TEV_KASEL_K1_A);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_ONE, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE,
                    GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE,
                    GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_APREV, GX_CC_CPREV, GX_CC_KONST, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE,
                    GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE,
                    GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE3, GX_TEV_KCSEL_K2);

    GXSetTevColorS10(GX_TEVREG0, (GXColorS10){-90, 0, -114, 135});
    GXSetTevKColor(GX_KCOLOR0, (GXColor){0, 0, 226, 88});
    GXSetTevKColor(GX_KCOLOR1, (GXColor){179, 0, 0, 182});
    GXSetTevKColor(GX_KCOLOR2, (GXColor){255, 0, 255, 128});
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE,
                          GX_CH_ALPHA);
}

void THPGXYuv2RgbDraw(u8 *y_data, u8 *u_data, u8 *v_data, s16 x, s16 y,
                      s16 textureWidth, s16 textureHeight, s16 polygonWidth,
                      s16 polygonHeight)
{
    GXTexObj tobj0, tobj1, tobj2;

    GXInitTexObj(&tobj0, y_data, textureWidth, textureHeight, GX_TF_I8, GX_CLAMP,
                 GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tobj0, GX_NEAR, GX_NEAR, 0, 0, 0, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tobj0, GX_TEXMAP0);
    GXInitTexObj(&tobj1, u_data, (textureWidth >> 1), (textureHeight >> 1), GX_TF_I8,
                 GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tobj1, GX_NEAR, GX_NEAR, 0, 0, 0, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tobj1, GX_TEXMAP1);
    GXInitTexObj(&tobj2, v_data, (textureWidth >> 1), (textureHeight >> 1), GX_TF_I8,
                 GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tobj2, GX_NEAR, GX_NEAR, 0, 0, 0, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tobj2, GX_TEXMAP2);

    GXBegin(GX_QUADS, GX_VTXFMT7, 4);
    GXPosition3s16(x, y, 0);
    GXTexCoord2u16(0, 0);
    GXPosition3s16((x + polygonWidth), y, 0);
    GXTexCoord2u16(1, 0);
    GXPosition3s16((x + polygonWidth), (y + polygonHeight), 0);
    GXTexCoord2u16(1, 1);
    GXPosition3s16(x, (y + polygonHeight), 0);
    GXTexCoord2u16(0, 1);
}
