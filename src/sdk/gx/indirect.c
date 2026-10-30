/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/GXBump.c. */
#include "gx_indirect.h"
#include "gx_state.h"
void GXSetTevIndirect(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
                      GXIndTexFormat format, GXIndTexBiasSel bias_sel,
                      GXIndTexMtxID matrix_sel, GXIndTexWrap wrap_s,
                      GXIndTexWrap wrap_t, u8 add_prev, u8 utc_lod,
                      GXIndTexAlphaSel alpha_sel)
{
    u32 reg;

    reg = 0;
    GX_SET_FIELD(reg, 2, 0, ind_stage);
    GX_SET_FIELD(reg, 2, 2, format);
    GX_SET_FIELD(reg, 3, 4, bias_sel);
    GX_SET_FIELD(reg, 2, 7, alpha_sel);
    GX_SET_FIELD(reg, 4, 9, matrix_sel);
    GX_SET_FIELD(reg, 3, 13, wrap_s);
    GX_SET_FIELD(reg, 3, 16, wrap_t);
    GX_SET_FIELD(reg, 1, 19, utc_lod);
    GX_SET_FIELD(reg, 1, 20, add_prev);
    GX_SET_FIELD(reg, 8, 24, tev_stage + 16);
    GX_WRITE_RAS_REG(reg);
    gx->bpSent = 0;
}

void GXSetIndTexCoordScale(GXIndTexStageID ind_state, GXIndTexScale scale_s,
                           GXIndTexScale scale_t)
{

    switch (ind_state) {
    case GX_INDTEXSTAGE0:
        GX_SET_FIELD(gx->IndTexScale0, 4, 0, scale_s);
        GX_SET_FIELD(gx->IndTexScale0, 4, 4, scale_t);
        GX_SET_FIELD(gx->IndTexScale0, 8, 24, 0x25);
        GX_WRITE_RAS_REG(gx->IndTexScale0);
        break;
    case GX_INDTEXSTAGE1:
        GX_SET_FIELD(gx->IndTexScale0, 4, 8, scale_s);
        GX_SET_FIELD(gx->IndTexScale0, 4, 12, scale_t);
        GX_SET_FIELD(gx->IndTexScale0, 8, 24, 0x25);
        GX_WRITE_RAS_REG(gx->IndTexScale0);
        break;
    case GX_INDTEXSTAGE2:
        GX_SET_FIELD(gx->IndTexScale1, 4, 0, scale_s);
        GX_SET_FIELD(gx->IndTexScale1, 4, 4, scale_t);
        GX_SET_FIELD(gx->IndTexScale1, 8, 24, 0x26);
        GX_WRITE_RAS_REG(gx->IndTexScale1);
        break;
    case GX_INDTEXSTAGE3:
        GX_SET_FIELD(gx->IndTexScale1, 4, 8, scale_s);
        GX_SET_FIELD(gx->IndTexScale1, 4, 12, scale_t);
        GX_SET_FIELD(gx->IndTexScale1, 8, 24, 0x26);
        GX_WRITE_RAS_REG(gx->IndTexScale1);
        break;
    default:

        break;
    }
    gx->bpSent = 0;
}

void GXSetNumIndStages(u8 nIndStages)
{

    GX_SET_FIELD(gx->genMode, 3, 16, nIndStages);
    gx->dirtyState |= 6;
}

#pragma dont_inline on
void GXSetTevDirect(GXTevStageID tev_stage)
{

    GXSetTevIndirect(tev_stage, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF,
                     GX_ITW_OFF, GX_ITW_OFF, 0U, 0, GX_ITBA_OFF);
}

#pragma dont_inline off
void __GXUpdateBPMask(void)
{
    u32 nIndStages;
    u32 i;
    u32 tmap;
    u32 new_imask;

    new_imask = 0;
    nIndStages = GX_GET_FIELD(gx->genMode, 3, 16);
    for (i = 0; i < nIndStages; i++) {
        switch (i) {
        case 0:
            tmap = GX_GET_FIELD(gx->iref, 3, 0);
            break;
        case 1:
            tmap = GX_GET_FIELD(gx->iref, 3, 6);
            break;
        case 2:
            tmap = GX_GET_FIELD(gx->iref, 3, 12);
            break;
        case 3:
            tmap = GX_GET_FIELD(gx->iref, 3, 18);
            break;
        }
        new_imask |= 1 << tmap;
    }

    if ((u8)gx->bpMask != new_imask) {
        GX_SET_FIELD(gx->bpMask, 8, 0, new_imask);
        GX_WRITE_RAS_REG(gx->bpMask);
        gx->bpSent = 0;
    }
}

void __GXFlushTextureState(void)
{
    GX_WRITE_RAS_REG(gx->bpMask);
    gx->bpSent = 0;
}
