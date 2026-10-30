/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/GXAttr.c. */
#include "gx_attr.h"
#include "gx_state.h"
static void __GXXfVtxSpecs(void)
{
    u32 nCols = 0;
    u32 nNrm;
    u32 nTex;
    u32 reg;

    nCols = GX_GET_FIELD(gx->vcdLo, 2, 13) ? 1 : 0;
    nCols += GX_GET_FIELD(gx->vcdLo, 2, 15) ? 1 : 0;
    nNrm = gx->hasBiNrms ? 2 : gx->hasNrms ? 1 : 0;
    nTex = 0;
    nTex += GX_GET_FIELD(gx->vcdHi, 2, 0) ? 1 : 0;
    nTex += GX_GET_FIELD(gx->vcdHi, 2, 2) ? 1 : 0;
    nTex += GX_GET_FIELD(gx->vcdHi, 2, 4) ? 1 : 0;
    nTex += GX_GET_FIELD(gx->vcdHi, 2, 6) ? 1 : 0;
    nTex += GX_GET_FIELD(gx->vcdHi, 2, 8) ? 1 : 0;
    nTex += GX_GET_FIELD(gx->vcdHi, 2, 10) ? 1 : 0;
    nTex += GX_GET_FIELD(gx->vcdHi, 2, 12) ? 1 : 0;
    nTex += GX_GET_FIELD(gx->vcdHi, 2, 14) ? 1 : 0;
    reg = (nCols) | (nNrm << 2) | (nTex << 4);
    GX_WRITE_XF_REG(8, reg);
    gx->bpSent = 1;
}

static inline void SETVCDATTR(GXAttr Attr, GXAttrType Type)
{
    switch (Attr) {
    case GX_VA_PNMTXIDX:
        GX_SET_FIELD(gx->vcdLo, 1, 0, Type);
        break;
    case GX_VA_TEX0MTXIDX:
        GX_SET_FIELD(gx->vcdLo, 1, 1, Type);
        break;
    case GX_VA_TEX1MTXIDX:
        GX_SET_FIELD(gx->vcdLo, 1, 2, Type);
        break;
    case GX_VA_TEX2MTXIDX:
        GX_SET_FIELD(gx->vcdLo, 1, 3, Type);
        break;
    case GX_VA_TEX3MTXIDX:
        GX_SET_FIELD(gx->vcdLo, 1, 4, Type);
        break;
    case GX_VA_TEX4MTXIDX:
        GX_SET_FIELD(gx->vcdLo, 1, 5, Type);
        break;
    case GX_VA_TEX5MTXIDX:
        GX_SET_FIELD(gx->vcdLo, 1, 6, Type);
        break;
    case GX_VA_TEX6MTXIDX:
        GX_SET_FIELD(gx->vcdLo, 1, 7, Type);
        break;
    case GX_VA_TEX7MTXIDX:
        GX_SET_FIELD(gx->vcdLo, 1, 8, Type);
        break;
    case GX_VA_POS:
        GX_SET_FIELD(gx->vcdLo, 2, 9, Type);
        break;
    case GX_VA_NRM:
        if (Type != GX_NONE) {
            gx->hasNrms = 1;
            gx->hasBiNrms = 0;
            gx->nrmType = Type;
        } else
            gx->hasNrms = 0;
        break;
    case GX_VA_NBT:
        if (Type != GX_NONE) {
            gx->hasBiNrms = 1;
            gx->hasNrms = 0;
            gx->nrmType = Type;
        } else
            gx->hasBiNrms = 0;
        break;
    case GX_VA_CLR0:
        GX_SET_FIELD(gx->vcdLo, 2, 13, Type);
        break;
    case GX_VA_CLR1:
        GX_SET_FIELD(gx->vcdLo, 2, 15, Type);
        break;
    case GX_VA_TEX0:
        GX_SET_FIELD(gx->vcdHi, 2, 0, Type);
        break;
    case GX_VA_TEX1:
        GX_SET_FIELD(gx->vcdHi, 2, 2, Type);
        break;
    case GX_VA_TEX2:
        GX_SET_FIELD(gx->vcdHi, 2, 4, Type);
        break;
    case GX_VA_TEX3:
        GX_SET_FIELD(gx->vcdHi, 2, 6, Type);
        break;
    case GX_VA_TEX4:
        GX_SET_FIELD(gx->vcdHi, 2, 8, Type);
        break;
    case GX_VA_TEX5:
        GX_SET_FIELD(gx->vcdHi, 2, 10, Type);
        break;
    case GX_VA_TEX6:
        GX_SET_FIELD(gx->vcdHi, 2, 12, Type);
        break;
    case GX_VA_TEX7:
        GX_SET_FIELD(gx->vcdHi, 2, 14, Type);
        break;
    }
}

void GXSetVtxDesc(GXAttr attr, GXAttrType type)
{
    SETVCDATTR(attr, type);
    if (gx->hasNrms || gx->hasBiNrms) {
        GX_SET_FIELD(gx->vcdLo, 2, 11, gx->nrmType);
    } else {
        GX_SET_FIELD(gx->vcdLo, 2, 11, 0);
    }
    gx->dirtyState |= 8;
}

void __GXSetVCD(void)
{
    GX_WRITE_CP_REG(0x50, gx->vcdLo);
    GX_WRITE_CP_REG(0x60, gx->vcdHi);
    __GXXfVtxSpecs();
}
void GXClearVtxDesc(void)
{
    gx->vcdLo = 0;
    GX_SET_FIELD(gx->vcdLo, 2, 9, 1);
    gx->vcdHi = 0;
    gx->hasNrms = 0;
    gx->hasBiNrms = 0;
    gx->dirtyState |= 8;
}

static inline void SETVAT(unsigned long *va, unsigned long *vb, u32 *vc, GXAttr attr,
                          GXCompCnt cnt, GXCompType type, u8 shft)
{
    switch (attr) {
    case GX_VA_POS:
        GX_SET_FIELD(*va, 1, 0, cnt);
        GX_SET_FIELD(*va, 3, 1, type);
        GX_SET_FIELD(*va, 5, 4, shft);
        break;
    case GX_VA_NRM:
    case GX_VA_NBT:
        GX_SET_FIELD(*va, 3, 10, type);
        if (cnt == GX_NRM_NBT3) {
            GX_SET_FIELD(*va, 1, 9, 1);
            GX_SET_FIELD(*va, 1, 31, 1);
        } else {
            GX_SET_FIELD(*va, 1, 9, cnt);
            GX_SET_FIELD(*va, 1, 31, 0);
        }
        break;
    case GX_VA_CLR0:
        GX_SET_FIELD(*va, 1, 13, cnt);
        GX_SET_FIELD(*va, 3, 14, type);
        break;
    case GX_VA_CLR1:
        GX_SET_FIELD(*va, 1, 0x11, cnt);
        GX_SET_FIELD(*va, 3, 18, type);
        break;
    case GX_VA_TEX0:
        GX_SET_FIELD(*va, 1, 0x15, cnt);
        GX_SET_FIELD(*va, 3, 0x16, type);
        GX_SET_FIELD(*va, 5, 0x19, shft);
        break;
    case GX_VA_TEX1:
        GX_SET_FIELD(*vb, 1, 0, cnt);
        GX_SET_FIELD(*vb, 3, 1, type);
        GX_SET_FIELD(*vb, 5, 4, shft);
        break;
    case GX_VA_TEX2:
        GX_SET_FIELD(*vb, 1, 9, cnt);
        GX_SET_FIELD(*vb, 3, 10, type);
        GX_SET_FIELD(*vb, 5, 13, shft);
        break;
    case GX_VA_TEX3:
        GX_SET_FIELD(*vb, 1, 18, cnt);
        GX_SET_FIELD(*vb, 3, 19, type);
        GX_SET_FIELD(*vb, 5, 22, shft);
        break;
    case GX_VA_TEX4:
        GX_SET_FIELD(*vb, 1, 27, cnt);
        GX_SET_FIELD(*vb, 3, 28, type);
        GX_SET_FIELD(*vc, 5, 0, shft);
        break;
    case GX_VA_TEX5:
        GX_SET_FIELD(*vc, 1, 5, cnt);
        GX_SET_FIELD(*vc, 3, 6, type);
        GX_SET_FIELD(*vc, 5, 9, shft);
        break;
    case GX_VA_TEX6:
        GX_SET_FIELD(*vc, 1, 14, cnt);
        GX_SET_FIELD(*vc, 3, 15, type);
        GX_SET_FIELD(*vc, 5, 18, shft);
        break;
    case GX_VA_TEX7:
        GX_SET_FIELD(*vc, 1, 23, cnt);
        GX_SET_FIELD(*vc, 3, 24, type);
        GX_SET_FIELD(*vc, 5, 27, shft);
        break;
    }
}

void GXSetVtxAttrFmt(u32 vtxfmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac)
{
    unsigned long *va;
    unsigned long *vb;
    u32 *vc;

    va = &gx->vatA[vtxfmt];
    vb = &gx->vatB[vtxfmt];
    vc = &gx->vatC[vtxfmt];
    SETVAT(va, vb, vc, attr, cnt, type, frac);
    gx->dirtyState |= 0x10;
    gx->dirtyVAT |= (u8)(1 << (u8)vtxfmt);
}

void __GXSetVAT(void)
{
    u8 i;

    for (i = 0; i < 8; i++) {
        if (gx->dirtyVAT & (1 << (u8)i)) {
            GX_WRITE_CP_REG(i | 0x70, gx->vatA[i]);
            GX_WRITE_CP_REG(i | 0x80, gx->vatB[i]);
            GX_WRITE_CP_REG(i | 0x90, gx->vatC[i]);
        }
    }
    gx->dirtyVAT = 0;
}

void GXSetArray(GXAttr attr, const void *base_ptr, u8 stride)
{
    GXAttr cpAttr;
    unsigned long phyAddr;

    if (attr == GX_VA_NBT) {
        attr = GX_VA_NRM;
    }

    cpAttr = attr - GX_VA_POS;
    phyAddr = (u32)base_ptr & 0x3FFFFFFF;
    GX_WRITE_ARRAY_BASE(cpAttr | 0xA0, phyAddr, cpAttr - 12);
    GX_WRITE_ARRAY_STRIDE(cpAttr | 0xB0, stride, cpAttr - 12);
}

void GXInvalidateVtxCache(void)
{
    GX_WRITE_U8(0x48);
}

void GXSetTexCoordGen2(GXTexCoordID dst_coord, s32 func, s32 src_param, u32 mtx,
                       u8 normalize, u32 pt_texmtx)
{
    u32 reg = 0;
    u32 row;
    u32 form;
    GXAttr mtxIdAttr;

    form = 0;
    row = 5;
    switch (src_param) {
    case GX_TG_POS:
        row = 0;
        form = 1;
        break;
    case GX_TG_NRM:
        row = 1;
        form = 1;
        break;
    case GX_TG_BINRM:
        row = 3;
        form = 1;
        break;
    case GX_TG_TANGENT:
        row = 4;
        form = 1;
        break;
    case GX_TG_COLOR0:
        row = 2;
        break;
    case GX_TG_COLOR1:
        row = 2;
        break;
    case GX_TG_TEX0:
        row = 5;
        break;
    case GX_TG_TEX1:
        row = 6;
        break;
    case GX_TG_TEX2:
        row = 7;
        break;
    case GX_TG_TEX3:
        row = 8;
        break;
    case GX_TG_TEX4:
        row = 9;
        break;
    case GX_TG_TEX5:
        row = 10;
        break;
    case GX_TG_TEX6:
        row = 11;
        break;
    case GX_TG_TEX7:
        row = 12;
        break;
    case GX_TG_TEXCOORD0:
        break;
    case GX_TG_TEXCOORD1:
        break;
    case GX_TG_TEXCOORD2:
        break;
    case GX_TG_TEXCOORD3:
        break;
    case GX_TG_TEXCOORD4:
        break;
    case GX_TG_TEXCOORD5:
        break;
    case GX_TG_TEXCOORD6:
        break;
    default:

        break;
    }
    switch (func) {
    case GX_TG_MTX2x4:
        GX_SET_FIELD(reg, 1, 1, 0);
        GX_SET_FIELD(reg, 1, 2, form);
        GX_SET_FIELD(reg, 3, 4, 0);
        GX_SET_FIELD(reg, 5, 7, row);
        break;
    case GX_TG_MTX3x4:
        GX_SET_FIELD(reg, 1, 1, 1);
        GX_SET_FIELD(reg, 1, 2, form);
        GX_SET_FIELD(reg, 3, 4, 0);
        GX_SET_FIELD(reg, 5, 7, row);
        break;
    case GX_TG_BUMP0:
    case GX_TG_BUMP1:
    case GX_TG_BUMP2:
    case GX_TG_BUMP3:
    case GX_TG_BUMP4:
    case GX_TG_BUMP5:
    case GX_TG_BUMP6:
    case GX_TG_BUMP7:

        GX_SET_FIELD(reg, 1, 1, 0);
        GX_SET_FIELD(reg, 1, 2, form);
        GX_SET_FIELD(reg, 3, 4, 1);
        GX_SET_FIELD(reg, 5, 7, row);
        GX_SET_FIELD(reg, 3, 12, src_param - GX_TG_TEXCOORD0);
        GX_SET_FIELD(reg, 3, 15, func - GX_TG_BUMP0);
        break;
    case GX_TG_SRTG:
        GX_SET_FIELD(reg, 1, 1, 0);
        GX_SET_FIELD(reg, 1, 2, form);
        if (src_param == GX_TG_COLOR0) {
            GX_SET_FIELD(reg, 3, 4, 2);
        } else {
            GX_SET_FIELD(reg, 3, 4, 3);
        }
        GX_SET_FIELD(reg, 5, 7, 2);
        break;
    default:

        break;
    }
    GX_WRITE_XF_REG(dst_coord + 0x40, reg);
    reg = 0;
    GX_SET_FIELD(reg, 6, 0, pt_texmtx - 64);
    GX_SET_FIELD(reg, 1, 8, normalize);
    GX_WRITE_XF_REG(dst_coord + 0x50, reg);
    switch (dst_coord) {
    case GX_TEXCOORD0:
        GX_SET_FIELD(gx->matIdxA, 6, 6, mtx);
        break;
    case GX_TEXCOORD1:
        GX_SET_FIELD(gx->matIdxA, 6, 12, mtx);
        break;
    case GX_TEXCOORD2:
        GX_SET_FIELD(gx->matIdxA, 6, 18, mtx);
        break;
    case GX_TEXCOORD3:
        GX_SET_FIELD(gx->matIdxA, 6, 24, mtx);
        break;
    case GX_TEXCOORD4:
        GX_SET_FIELD(gx->matIdxB, 6, 0, mtx);
        break;
    case GX_TEXCOORD5:
        GX_SET_FIELD(gx->matIdxB, 6, 6, mtx);
        break;
    case GX_TEXCOORD6:
        GX_SET_FIELD(gx->matIdxB, 6, 12, mtx);
        break;
    default:
        GX_SET_FIELD(gx->matIdxB, 6, 18, mtx);
        break;
    }
    mtxIdAttr = dst_coord + 1;
    __GXSetMatrixIndex(mtxIdAttr);
}

void GXSetNumTexGens(u8 nTexGens)
{
    GX_SET_FIELD(gx->genMode, 4, 0, nTexGens);
    GX_WRITE_XF_REG(0x3F, nTexGens);
    gx->dirtyState |= 4;
}
