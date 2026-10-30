/* References:
 * https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/GXFrameBuf.c
 * https://github.com/doldecomp/dolsdk2004/blob/main/src/gx/GXFrameBuf.c.
 */
#include "gx_framebuffer.h"
#include "gx_state.h"
extern void __GetImageTileCount(s32 format, u16 width, u16 height, u32 *rows,
                                u32 *columns, u32 *components);
void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht)
{

    gx->cpDispSrc = 0;
    GX_SET_FIELD(gx->cpDispSrc, 10, 0, left);
    GX_SET_FIELD(gx->cpDispSrc, 10, 10, top);
    GX_SET_FIELD(gx->cpDispSrc, 8, 24, 0x49);

    gx->cpDispSize = 0;
    GX_SET_FIELD(gx->cpDispSize, 10, 0, wd - 1);
    GX_SET_FIELD(gx->cpDispSize, 10, 10, ht - 1);
    GX_SET_FIELD(gx->cpDispSize, 8, 24, 0x4A);
}

void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht)
{

    gx->cpTexSrc = 0;
    GX_SET_FIELD(gx->cpTexSrc, 10, 0, left);
    GX_SET_FIELD(gx->cpTexSrc, 10, 10, top);
    GX_SET_FIELD(gx->cpTexSrc, 8, 24, 0x49);

    gx->cpTexSize = 0;
    GX_SET_FIELD(gx->cpTexSize, 10, 0, wd - 1);
    GX_SET_FIELD(gx->cpTexSize, 10, 10, ht - 1);
    GX_SET_FIELD(gx->cpTexSize, 8, 24, 0x4A);
}

void GXSetDispCopyDst(u16 wd, u16 ht)
{
    u16 stride;

    stride = (int)wd * 2;
    gx->cpDispStride = 0;
    GX_SET_FIELD(gx->cpDispStride, 10, 0, (stride >> 5));
    GX_SET_FIELD(gx->cpDispStride, 8, 24, 0x4D);
}

void GXSetTexCopyDst(u16 wd, u16 ht, s32 fmt, u8 mipmap)
{
    u32 rowTiles;
    u32 colTiles;
    u32 cmpTiles;
    u32 peTexFmt;
    u32 peTexFmtH;

    gx->cpTexZ = 0;
    peTexFmt = fmt & 0xF;

    if (fmt == GX_TF_Z16) {
        peTexFmt = 0xB;
    }
    switch (fmt) {
    case GX_TF_I4:
    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_IA8:
    case GX_CTF_YUVA8:
        GX_SET_FIELD(gx->cpTex, 2, 15, 3);
        break;
    default:
        GX_SET_FIELD(gx->cpTex, 2, 15, 2);
        break;
    }

    gx->cpTexZ = (fmt & _GX_TF_ZTF) == _GX_TF_ZTF;
    peTexFmtH = (peTexFmt >> 3) & 1;
    /* This SDK expression preserves MWCC register allocation. */
    !peTexFmt;
    GX_SET_FIELD(gx->cpTex, 1, 3, peTexFmtH);
    peTexFmt = peTexFmt & 7;
    __GetImageTileCount(fmt, wd, ht, &rowTiles, &colTiles, &cmpTiles);

    gx->cpTexStride = 0;
    GX_SET_FIELD(gx->cpTexStride, 10, 0, rowTiles * cmpTiles);
    GX_SET_FIELD(gx->cpTexStride, 8, 24, 0x4D);
    GX_SET_FIELD(gx->cpTex, 1, 9, mipmap);
    GX_SET_FIELD(gx->cpTex, 3, 4, peTexFmt);
}

void GXSetDispCopyFrame2Field(GXCopyMode mode)
{

    GX_SET_FIELD(gx->cpDisp, 2, 12, mode);
    GX_SET_FIELD(gx->cpTex, 2, 12, 0);
}

void GXSetCopyClamp(GXFBClamp clamp)
{
    u8 clmpB;
    u8 clmpT;

    clmpT = (clamp & GX_CLAMP_TOP) == GX_CLAMP_TOP;
    clmpB = (clamp & GX_CLAMP_BOTTOM) == GX_CLAMP_BOTTOM;

    GX_SET_FIELD(gx->cpDisp, 1, 0, clmpT);
    GX_SET_FIELD(gx->cpDisp, 1, 1, clmpB);

    GX_SET_FIELD(gx->cpTex, 1, 0, clmpT);
    GX_SET_FIELD(gx->cpTex, 1, 1, clmpB);
}

static inline u32 __GXGetNumXfbLines(u32 efbHt, u32 iScale)
{
    u32 count;
    u32 realHt;
    u32 iScaleD;

    count = (efbHt - 1) * 0x100;
    realHt = (count / iScale) + 1;

    iScaleD = iScale;

    if (iScaleD > 0x80 && iScaleD < 0x100) {
        while (iScaleD % 2 == 0) {
            iScaleD /= 2;
        }

        if (efbHt % iScaleD == 0) {
            realHt++;
        }
    }

    if (realHt > 0x400) {
        realHt = 0x400;
    }

    return realHt;
}

u32 GXSetDispCopyYScale(f32 vscale)
{
    u8 enable;
    u32 iScale;
    u32 ht;
    u32 reg;

    iScale = (u32)(256.0f / vscale) & 0x1FF;
    enable = (iScale != 256);

    reg = 0;
    GX_SET_FIELD(reg, 9, 0, iScale);
    GX_SET_FIELD(reg, 8, 24, 0x4E);
    GX_WRITE_RAS_REG(reg);
    gx->bpSent = 0;
    GX_SET_FIELD(gx->cpDisp, 1, 10, enable);
    ht = (u32)GX_GET_FIELD(gx->cpDispSize, 10, 10) + 1;
    return __GXGetNumXfbLines(ht, iScale);
}
void GXSetCopyClear(GXColor clear_clr, u32 clear_z)
{
    u32 reg;

    reg = 0;
    GX_SET_FIELD(reg, 8, 0, clear_clr.r);
    GX_SET_FIELD(reg, 8, 8, clear_clr.a);
    GX_SET_FIELD(reg, 8, 24, 0x4F);
    GX_WRITE_RAS_REG(reg);

    reg = 0;
    GX_SET_FIELD(reg, 8, 0, clear_clr.b);
    GX_SET_FIELD(reg, 8, 8, clear_clr.g);
    GX_SET_FIELD(reg, 8, 24, 0x50);
    GX_WRITE_RAS_REG(reg);

    reg = 0;
    GX_SET_FIELD(reg, 24, 0, clear_z);
    GX_SET_FIELD(reg, 8, 24, 0x51);
    GX_WRITE_RAS_REG(reg);
    gx->bpSent = 0;
}

void GXSetCopyFilter(u8 aa, const u8 sample_pattern[12][2], u8 vf, const u8 vfilter[7])
{
    u32 msLoc[4];
    u32 coeff0;
    u32 coeff1;

    if (aa != 0) {
        msLoc[0] = 0;
        GX_SET_FIELD(msLoc[0], 4, 0, sample_pattern[0][0]);
        GX_SET_FIELD(msLoc[0], 4, 4, sample_pattern[0][1]);
        GX_SET_FIELD(msLoc[0], 4, 8, sample_pattern[1][0]);
        GX_SET_FIELD(msLoc[0], 4, 12, sample_pattern[1][1]);
        GX_SET_FIELD(msLoc[0], 4, 16, sample_pattern[2][0]);
        GX_SET_FIELD(msLoc[0], 4, 20, sample_pattern[2][1]);
        GX_SET_FIELD(msLoc[0], 8, 24, 1);

        msLoc[1] = 0;
        GX_SET_FIELD(msLoc[1], 4, 0, sample_pattern[3][0]);
        GX_SET_FIELD(msLoc[1], 4, 4, sample_pattern[3][1]);
        GX_SET_FIELD(msLoc[1], 4, 8, sample_pattern[4][0]);
        GX_SET_FIELD(msLoc[1], 4, 12, sample_pattern[4][1]);
        GX_SET_FIELD(msLoc[1], 4, 16, sample_pattern[5][0]);
        GX_SET_FIELD(msLoc[1], 4, 20, sample_pattern[5][1]);
        GX_SET_FIELD(msLoc[1], 8, 24, 2);

        msLoc[2] = 0;
        GX_SET_FIELD(msLoc[2], 4, 0, sample_pattern[6][0]);
        GX_SET_FIELD(msLoc[2], 4, 4, sample_pattern[6][1]);
        GX_SET_FIELD(msLoc[2], 4, 8, sample_pattern[7][0]);
        GX_SET_FIELD(msLoc[2], 4, 12, sample_pattern[7][1]);
        GX_SET_FIELD(msLoc[2], 4, 16, sample_pattern[8][0]);
        GX_SET_FIELD(msLoc[2], 4, 20, sample_pattern[8][1]);
        GX_SET_FIELD(msLoc[2], 8, 24, 3);

        msLoc[3] = 0;
        GX_SET_FIELD(msLoc[3], 4, 0, sample_pattern[9][0]);
        GX_SET_FIELD(msLoc[3], 4, 4, sample_pattern[9][1]);
        GX_SET_FIELD(msLoc[3], 4, 8, sample_pattern[10][0]);
        GX_SET_FIELD(msLoc[3], 4, 12, sample_pattern[10][1]);
        GX_SET_FIELD(msLoc[3], 4, 16, sample_pattern[11][0]);
        GX_SET_FIELD(msLoc[3], 4, 20, sample_pattern[11][1]);
        GX_SET_FIELD(msLoc[3], 8, 24, 4);
    } else {
        msLoc[0] = 0x01666666;
        msLoc[1] = 0x02666666;
        msLoc[2] = 0x03666666;
        msLoc[3] = 0x04666666;
    }
    GX_WRITE_RAS_REG(msLoc[0]);
    GX_WRITE_RAS_REG(msLoc[1]);
    GX_WRITE_RAS_REG(msLoc[2]);
    GX_WRITE_RAS_REG(msLoc[3]);

    coeff0 = 0;
    GX_SET_FIELD(coeff0, 8, 24, 0x53);
    coeff1 = 0;
    GX_SET_FIELD(coeff1, 8, 24, 0x54);
    if (vf != 0) {
        GX_SET_FIELD(coeff0, 6, 0, vfilter[0]);
        GX_SET_FIELD(coeff0, 6, 6, vfilter[1]);
        GX_SET_FIELD(coeff0, 6, 12, vfilter[2]);
        GX_SET_FIELD(coeff0, 6, 18, vfilter[3]);
        GX_SET_FIELD(coeff1, 6, 0, vfilter[4]);
        GX_SET_FIELD(coeff1, 6, 6, vfilter[5]);
        GX_SET_FIELD(coeff1, 6, 12, vfilter[6]);
    } else {
        GX_SET_FIELD(coeff0, 6, 0, 0);
        GX_SET_FIELD(coeff0, 6, 6, 0);
        GX_SET_FIELD(coeff0, 6, 12, 21);
        GX_SET_FIELD(coeff0, 6, 18, 22);
        GX_SET_FIELD(coeff1, 6, 0, 21);
        GX_SET_FIELD(coeff1, 6, 6, 0);
        GX_SET_FIELD(coeff1, 6, 12, 0);
    }
    GX_WRITE_RAS_REG(coeff0);
    GX_WRITE_RAS_REG(coeff1);
    gx->bpSent = 0;
}

void GXSetDispCopyGamma(GXGamma gamma)
{

    GX_SET_FIELD(gx->cpDisp, 2, 7, gamma);
}

void GXCopyDisp(void *dest, u8 clear)
{
    u32 reg;
    u32 tempPeCtrl;
    u32 phyAddr;
    u8 changePeCtrl;

    if (clear) {
        reg = gx->zmode;
        GX_SET_FIELD(reg, 1, 0, 1);
        GX_SET_FIELD(reg, 3, 1, 7);
        GX_WRITE_RAS_REG(reg);

        reg = gx->cmode0;
        GX_SET_FIELD(reg, 1, 0, 0);
        GX_SET_FIELD(reg, 1, 1, 0);
        GX_WRITE_RAS_REG(reg);
    }
    changePeCtrl = 0;
    if ((clear || (u32)GX_GET_FIELD(gx->peCtrl, 3, 0) == 3) &&
        (u32)GX_GET_FIELD(gx->peCtrl, 1, 6) == 1) {
        changePeCtrl = 1;
        tempPeCtrl = gx->peCtrl;
        GX_SET_FIELD(tempPeCtrl, 1, 6, 0);
        GX_WRITE_RAS_REG(tempPeCtrl);
    }
    GX_WRITE_RAS_REG(gx->cpDispSrc);
    GX_WRITE_RAS_REG(gx->cpDispSize);
    GX_WRITE_RAS_REG(gx->cpDispStride);

    phyAddr = (u32)dest & 0x3FFFFFFF;
    reg = 0;
    GX_SET_FIELD(reg, 21, 0, phyAddr >> 5);
    GX_SET_FIELD(reg, 8, 24, 0x4B);
    GX_WRITE_RAS_REG(reg);

    GX_SET_FIELD(gx->cpDisp, 1, 11, clear);
    GX_SET_FIELD(gx->cpDisp, 1, 14, 1);
    GX_SET_FIELD(gx->cpDisp, 8, 24, 0x52);
    GX_WRITE_RAS_REG(gx->cpDisp);

    if (clear) {
        GX_WRITE_RAS_REG(gx->zmode);
        GX_WRITE_RAS_REG(gx->cmode0);
    }
    if (changePeCtrl) {
        GX_WRITE_RAS_REG(gx->peCtrl);
    }
    gx->bpSent = 0;
}

void GXCopyTex(void *dest, u8 clear)
{
    u32 reg;
    u32 tempPeCtrl;
    u32 phyAddr;
    u8 changePeCtrl;

    if (clear) {
        reg = gx->zmode;
        GX_SET_FIELD(reg, 1, 0, 1);
        GX_SET_FIELD(reg, 3, 1, 7);
        GX_WRITE_RAS_REG(reg);

        reg = gx->cmode0;
        GX_SET_FIELD(reg, 1, 0, 0);
        GX_SET_FIELD(reg, 1, 1, 0);
        GX_WRITE_RAS_REG(reg);
    }
    changePeCtrl = 0;
    tempPeCtrl = gx->peCtrl;
    if (((u8)gx->cpTexZ != 0) && ((u32)(tempPeCtrl & 7) != 3)) {
        changePeCtrl = 1;
        tempPeCtrl = (tempPeCtrl & 0xFFFFFFF8) | 3;
    }
    if (((clear != 0) || ((u32)(tempPeCtrl & 7) == 3)) &&
        ((u32)((tempPeCtrl >> 6U) & 1) == 1)) {
        changePeCtrl = 1;
        tempPeCtrl &= 0xFFFFFFBF;
    }
    if (changePeCtrl) {
        GX_WRITE_RAS_REG(tempPeCtrl);
    }
    GX_WRITE_RAS_REG(gx->cpTexSrc);
    GX_WRITE_RAS_REG(gx->cpTexSize);
    GX_WRITE_RAS_REG(gx->cpTexStride);

    phyAddr = (u32)dest & 0x3FFFFFFF;
    reg = 0;
    GX_SET_FIELD(reg, 21, 0, phyAddr >> 5);
    GX_SET_FIELD(reg, 8, 24, 0x4B);
    GX_WRITE_RAS_REG(reg);

    GX_SET_FIELD(gx->cpTex, 1, 11, clear);
    GX_SET_FIELD(gx->cpTex, 1, 14, 0);
    GX_SET_FIELD(gx->cpTex, 8, 24, 0x52);
    GX_WRITE_RAS_REG(gx->cpTex);

    if (clear != 0) {
        GX_WRITE_RAS_REG(gx->zmode);
        GX_WRITE_RAS_REG(gx->cmode0);
    }
    if (changePeCtrl) {
        GX_WRITE_RAS_REG(gx->peCtrl);
    }
    gx->bpSent = 0;
}

void GXClearBoundingBox(void)
{
    u32 reg;

    reg = 0x550003FF;
    GX_WRITE_RAS_REG(reg);
    reg = 0x560003FF;
    GX_WRITE_RAS_REG(reg);
    gx->bpSent = 0;
}
