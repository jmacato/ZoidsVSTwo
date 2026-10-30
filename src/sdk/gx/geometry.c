/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/GXGeometry.c. */
#include "gx_geometry.h"
#include "gx_state.h"
void __GXSetDirtyState(void)
{
    if (gx->dirtyState & 1) {
        __GXSetSUTexRegs();
    }
    if (gx->dirtyState & 2) {
        __GXUpdateBPMask();
    }
    if (gx->dirtyState & 4) {
        __GXSetGenMode();
    }
    if (gx->dirtyState & 8) {
        __GXSetVCD();
    }
    if (gx->dirtyState & 0x10) {
        __GXSetVAT();
    }
    if (gx->dirtyState & 0x18)
        __GXCalculateVLim();
    gx->dirtyState = 0;
}

void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts)
{
    if (gx->dirtyState != 0) {
        __GXSetDirtyState();
    }

    /* The word test includes vcdClear and bpSent. */
    if (*(u32 *)&gx->vcdClear == 0) {
        __GXSendFlushPrim();
    }
    GX_WRITE_U8(vtxfmt | type);
    GX_WRITE_U16(nverts);
}

void __GXSendFlushPrim(void)
{
    u32 i;
    u32 numD = gx->vNum * gx->vLim;

    GX_WRITE_U8(0x98);
    GX_WRITE_U16(gx->vNum);
    for (i = 0; i < numD; i += 4) {
        GX_WRITE_U32(0);
    }
    gx->bpSent = 1;
}

void GXSetLineWidth(u8 width, GXTexOffset texOffsets)
{
    GX_SET_FIELD(gx->lpSize, 8, 0, width);
    GX_SET_FIELD(gx->lpSize, 3, 16, texOffsets);
    GX_WRITE_RAS_REG(gx->lpSize);
    gx->bpSent = 0;
}

void GXSetPointSize(u8 pointSize, GXTexOffset texOffsets)
{
    GX_SET_FIELD(gx->lpSize, 8, 8, pointSize);
    GX_SET_FIELD(gx->lpSize, 3, 19, texOffsets);
    GX_WRITE_RAS_REG(gx->lpSize);
    gx->bpSent = 0;
}

void GXEnableTexOffsets(GXTexCoordID coord, u8 line_enable, u8 point_enable)
{
    GX_SET_FIELD(gx->suTs0[coord], 1, 18, line_enable);
    GX_SET_FIELD(gx->suTs0[coord], 1, 19, point_enable);
    GX_WRITE_RAS_REG(gx->suTs0[coord]);
    gx->bpSent = 0;
}

void GXSetCullMode(GXCullMode mode)
{
    GXCullMode hwMode;

    switch (mode) {
    case GX_CULL_FRONT:
        hwMode = GX_CULL_BACK;
        break;
    case GX_CULL_BACK:
        hwMode = GX_CULL_FRONT;
        break;
    default:
        hwMode = mode;
        break;
    }
    GX_SET_FIELD(gx->genMode, 2, 14, hwMode);
    gx->dirtyState |= 4;
}

void GXSetCoPlanar(u8 enable)
{
    u32 reg;

    GX_SET_FIELD(gx->genMode, 1, 19, enable);
    reg = 0xFE080000;
    GX_WRITE_RAS_REG(reg);
    GX_WRITE_RAS_REG(gx->genMode);
}

void __GXSetGenMode(void)
{
    GX_WRITE_RAS_REG(gx->genMode);
    gx->bpSent = 0;
}
