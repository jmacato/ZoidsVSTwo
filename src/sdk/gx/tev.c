/* References:
 * https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/GXTev.c
 * https://github.com/doldecomp/dolsdk2004/blob/main/src/gx/GXTev.c.
 */
#define GX_TRUE 1
#include "gx_tev_api.h"
#include "gx_state.h"
void GXSetTevOp(GXTevStageID id, GXTevMode mode)
{
    GXTevColorArg carg = GX_CC_RASC;
    GXTevAlphaArg aarg = GX_CA_RASA;

    if (id != GX_TEVSTAGE0) {
        carg = GX_CC_CPREV;
        aarg = GX_CA_APREV;
    }

    switch (mode) {
    case GX_MODULATE:
        GXSetTevColorIn(id, GX_CC_ZERO, GX_CC_TEXC, carg, GX_CC_ZERO);
        GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_TEXA, aarg, GX_CA_ZERO);
        break;
    case GX_DECAL:
        GXSetTevColorIn(id, carg, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, aarg);
        break;
    case GX_BLEND:
        GXSetTevColorIn(id, carg, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_TEXA, aarg, GX_CA_ZERO);
        break;
    case GX_REPLACE:
        GXSetTevColorIn(id, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        break;
    case GX_PASSCLR:
        GXSetTevColorIn(id, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, carg);
        GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, aarg);
        break;
    default:
        break;
    }
    GXSetTevColorOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

void GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                     GXTevColorArg c, GXTevColorArg d)
{
    u32 *pTevReg;

    pTevReg = &gx->tevc[stage];
    GX_SET_FIELD(*pTevReg, 4, 12, a);
    GX_SET_FIELD(*pTevReg, 4, 8, b);
    GX_SET_FIELD(*pTevReg, 4, 4, c);
    GX_SET_FIELD(*pTevReg, 4, 0, d);

    GX_WRITE_RAS_REG(*pTevReg);
    gx->bpSent = 0;
}

void GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b,
                     GXTevAlphaArg c, GXTevAlphaArg d)
{
    u32 *pTevReg;

    pTevReg = &gx->teva[stage];
    GX_SET_FIELD(*pTevReg, 3, 13, a);
    GX_SET_FIELD(*pTevReg, 3, 10, b);
    GX_SET_FIELD(*pTevReg, 3, 7, c);
    GX_SET_FIELD(*pTevReg, 3, 4, d);

    GX_WRITE_RAS_REG(*pTevReg);
    gx->bpSent = 0;
}

void GXSetTevColorOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale,
                     u8 clamp, GXTevRegID out_reg)
{
    u32 *pTevReg;

    pTevReg = &gx->tevc[stage];
    GX_SET_FIELD(*pTevReg, 1, 18, op & 1);
    if (op <= 1) {
        GX_SET_FIELD(*pTevReg, 2, 20, scale);
        GX_SET_FIELD(*pTevReg, 2, 16, bias);
    } else {
        GX_SET_FIELD(*pTevReg, 2, 20, (op >> 1) & 3);
        GX_SET_FIELD(*pTevReg, 2, 16, 3);
    }
    GX_SET_FIELD(*pTevReg, 1, 19, clamp & 0xFF);
    GX_SET_FIELD(*pTevReg, 2, 22, out_reg);

    GX_WRITE_RAS_REG(*pTevReg);
    gx->bpSent = 0;
}

void GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale,
                     u8 clamp, GXTevRegID out_reg)
{
    u32 *pTevReg;

    pTevReg = &gx->teva[stage];
    GX_SET_FIELD(*pTevReg, 1, 18, op & 1);
    if (op <= 1) {
        GX_SET_FIELD(*pTevReg, 2, 20, scale);
        GX_SET_FIELD(*pTevReg, 2, 16, bias);
    } else {
        GX_SET_FIELD(*pTevReg, 2, 20, (op >> 1) & 3);
        GX_SET_FIELD(*pTevReg, 2, 16, 3);
    }
    GX_SET_FIELD(*pTevReg, 1, 19, clamp & 0xFF);
    GX_SET_FIELD(*pTevReg, 2, 22, out_reg);

    GX_WRITE_RAS_REG(*pTevReg);
    gx->bpSent = 0;
}

void GXSetTevColor(GXTevRegID id, GXColor color)
{
    u32 regRA;
    u32 regBG;

    regRA = 0;
    GX_SET_FIELD(regRA, 11, 0, color.r);
    GX_SET_FIELD(regRA, 11, 12, color.a);
    GX_SET_FIELD(regRA, 8, 24, 224 + id * 2);

    regBG = 0;
    GX_SET_FIELD(regBG, 11, 0, color.b);
    GX_SET_FIELD(regBG, 11, 12, color.g);
    GX_SET_FIELD(regBG, 8, 24, 225 + id * 2);

    GX_WRITE_RAS_REG(regRA);
    GX_WRITE_RAS_REG(regBG);
    GX_WRITE_RAS_REG(regBG);
    GX_WRITE_RAS_REG(regBG);
    gx->bpSent = 0;
}

void GXSetTevColorS10(GXTevRegID id, GXColorS10 color)
{
    u32 regRA;
    u32 regBG;

    regRA = 0;
    GX_SET_FIELD(regRA, 11, 0, color.r & 0x7FF);
    GX_SET_FIELD(regRA, 11, 12, color.a & 0x7FF);
    GX_SET_FIELD(regRA, 8, 24, 224 + id * 2);

    regBG = 0;
    GX_SET_FIELD(regBG, 11, 0, color.b & 0x7FF);
    GX_SET_FIELD(regBG, 11, 12, color.g & 0x7FF);
    GX_SET_FIELD(regBG, 8, 24, 225 + id * 2);

    GX_WRITE_RAS_REG(regRA);
    GX_WRITE_RAS_REG(regBG);
    GX_WRITE_RAS_REG(regBG);
    GX_WRITE_RAS_REG(regBG);
    gx->bpSent = 0;
}

void GXSetTevKColor(GXTevKColorID id, GXColor color)
{
    u32 regRA;
    u32 regBG;

    regRA = 0;
    GX_SET_FIELD(regRA, 8, 0, color.r);
    GX_SET_FIELD(regRA, 8, 12, color.a);
    GX_SET_FIELD(regRA, 4, 20, 8);
    GX_SET_FIELD(regRA, 8, 24, 224 + id * 2);

    regBG = 0;
    GX_SET_FIELD(regBG, 8, 0, color.b);
    GX_SET_FIELD(regBG, 8, 12, color.g);
    GX_SET_FIELD(regBG, 4, 20, 8);
    GX_SET_FIELD(regBG, 8, 24, 225 + id * 2);

    GX_WRITE_RAS_REG(regRA);
    GX_WRITE_RAS_REG(regBG);
    gx->bpSent = 0;
}

void GXSetTevKColorSel(GXTevStageID stage, GXTevKColorSel sel)
{
    u32 *Kreg;

    Kreg = &gx->tevKsel[stage >> 1];
    if (stage & 1) {
        GX_SET_FIELD(*Kreg, 5, 14, sel);
    } else {
        GX_SET_FIELD(*Kreg, 5, 4, sel);
    }

    GX_WRITE_RAS_REG(*Kreg);
    gx->bpSent = 0;
}

void GXSetTevKAlphaSel(GXTevStageID stage, GXTevKAlphaSel sel)
{
    u32 *Kreg;

    Kreg = &gx->tevKsel[stage >> 1];
    if (stage & 1) {
        GX_SET_FIELD(*Kreg, 5, 19, sel);
    } else {
        GX_SET_FIELD(*Kreg, 5, 9, sel);
    }

    GX_WRITE_RAS_REG(*Kreg);
    gx->bpSent = 0;
}

void GXSetTevSwapMode(GXTevStageID stage, GXTevSwapSel ras_sel, GXTevSwapSel tex_sel)
{
    u32 *pTevReg;

    pTevReg = &gx->teva[stage];
    GX_SET_FIELD(*pTevReg, 2, 0, ras_sel);
    GX_SET_FIELD(*pTevReg, 2, 2, tex_sel);

    GX_WRITE_RAS_REG(*pTevReg);
    gx->bpSent = 0;
}

void GXSetTevSwapModeTable(GXTevSwapSel table, GXTevColorChan red, GXTevColorChan green,
                           GXTevColorChan blue, GXTevColorChan alpha)
{
    u32 *Kreg;
    int index = table * 2;

    Kreg = &gx->tevKsel[index];

    GX_SET_FIELD(*Kreg, 2, 0, red);
    GX_SET_FIELD(*Kreg, 2, 2, green);

    GX_WRITE_RAS_REG(*Kreg);

    Kreg = &gx->tevKsel[table * 2 + 1];
    GX_SET_FIELD(*Kreg, 2, 0, blue);
    GX_SET_FIELD(*Kreg, 2, 2, alpha);

    GX_WRITE_RAS_REG(*Kreg);
    gx->bpSent = 0;
}

void GXSetTevClampMode(void)
{
}

void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1, u8 ref1)
{
    u32 reg = 0;

    GX_SET_FIELD(reg, 8, 0, ref0);
    GX_SET_FIELD(reg, 8, 8, ref1);
    GX_SET_FIELD(reg, 3, 16, comp0);
    GX_SET_FIELD(reg, 3, 19, comp1);
    GX_SET_FIELD(reg, 2, 22, op);
    GX_SET_FIELD(reg, 8, 24, 0xF3);

    GX_WRITE_RAS_REG(reg);
    gx->bpSent = 0;
}

void GXSetZTexture(GXZTexOp op, s32 fmt, u32 bias)
{
    u32 zenv0;
    u32 zenv1;
    u32 type;

    zenv0 = 0;
    GX_SET_FIELD(zenv0, 24, 0, bias);
    GX_SET_FIELD(zenv0, 8, 24, 0xF4);

    zenv1 = 0;
    switch (fmt) {
    case GX_TF_Z8:
        type = 0;
        break;
    case GX_TF_Z16:
        type = 1;
        break;
    case GX_TF_Z24X8:
        type = 2;
        break;
    default:

        type = 2;
        break;
    }
    GX_SET_FIELD(zenv1, 2, 0, type);
    GX_SET_FIELD(zenv1, 2, 2, op);
    GX_SET_FIELD(zenv1, 8, 24, 0xF5);

    GX_WRITE_RAS_REG(zenv0);
    GX_WRITE_RAS_REG(zenv1);
    gx->bpSent = 0;
}

void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map,
                   GXChannelID color)
{
    u32 *ptref;
    u32 tmap;
    u32 tcoord;
    static int c2r[] = {0, 1, 0, 1, 0, 1, 7, 5, 6};

    ptref = &gx->tref[stage / 2];
    gx->texmapId[stage] = map;

    tmap = map & ~GX_TEX_DISABLE;
    tmap = (tmap >= GX_MAX_TEXMAP) ? GX_TEXMAP0 : tmap;
    if (coord >= GX_MAX_TEXCOORD) {
        tcoord = GX_TEXCOORD0;
        gx->tevTcEnab = gx->tevTcEnab & ~(1 << stage);
    } else {
        tcoord = coord;
        gx->tevTcEnab = gx->tevTcEnab | (1 << stage);
    }

    if (stage & 1) {
        GX_SET_FIELD(*ptref, 3, 12, tmap);
        GX_SET_FIELD(*ptref, 3, 15, tcoord);
        GX_SET_FIELD(*ptref, 3, 19, (color == GX_COLOR_NULL) ? 7 : c2r[color]);
        GX_SET_FIELD(*ptref, 1, 18, (map != GX_TEXMAP_NULL && !(map & GX_TEX_DISABLE)));
    } else {
        GX_SET_FIELD(*ptref, 3, 0, tmap);
        GX_SET_FIELD(*ptref, 3, 3, tcoord);
        GX_SET_FIELD(*ptref, 3, 7, (color == GX_COLOR_NULL) ? 7 : c2r[color]);
        GX_SET_FIELD(*ptref, 1, 6, (map != GX_TEXMAP_NULL && !(map & GX_TEX_DISABLE)));
    }

    GX_WRITE_RAS_REG(*ptref);
    gx->bpSent = 0;
    gx->dirtyState |= 1;
}

void GXSetNumTevStages(u8 nStages)
{

    GX_SET_FIELD(gx->genMode, 4, 10, nStages - 1);
    gx->dirtyState |= 4;
}
