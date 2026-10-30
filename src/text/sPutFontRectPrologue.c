#include "gx_attr.h"
#include "gx_light.h"
#include "gx_tev_api.h"
#include "gx_geometry.h"
#include "cobj.h"

extern u32 PSMTXInverse(Mtx src, Mtx inv);
extern void PSMTXTranspose(Mtx src, Mtx xpose);

void sPutFontRectPrologue(GXColor *color, GXColor *shadow, u32 alpha, u32 flags)
{
    Mtx mtx;
    HSD_CObj *camera;

    GXClearVtxDesc();
    GXSetNumChans(2);
    GXSetNumTexGens(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, 0, GX_PTIDENTITY);
    PSMTXTrans(mtx, 0.0f, 0.0f, 1000.0f);
    camera = HSD_CObjGetCurrent();
    HSD_CObjGetViewingMtx(camera, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    PSMTXInverse(mtx, mtx);
    PSMTXTranspose(mtx, mtx);
    GXLoadNrmMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_BLEND);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    if (flags & 1)
        GXSetZMode(1, GX_LESS, 1);
    else
        GXSetZMode(0, GX_ALWAYS, 0);
    if (flags & 2) {
        if (flags & 4)
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_COPY);
        else
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    } else {
        if (flags & 4)
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);
        else
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    }
    GXSetAlphaCompare(GX_GEQUAL, alpha, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, (flags & 0x10) ? GX_CA_ZERO : GX_CA_A1, (flags & 8) ? GX_CA_ZERO : GX_CA_A0,
                    GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, *color);
    GXSetTevColor(GX_TEVREG1, *shadow);
    GXSetCullMode(GX_CULL_NONE);
}
