/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/GXTransform.c. */
/* These options preserve instruction order and separate multiply/add rounding. */
#pragma peephole off
#include "gx_transform.h"
#include "gx_state.h"
#include "asm_ops.h"
static void WriteMTXPS4x3(f32 mtx[3][4], volatile f32 *destination);
static void WriteMTXPS3x3from3x4(f32 mtx[3][4], volatile f32 *destination);
static void WriteMTXPS4x2(f32 mtx[2][4], volatile f32 *destination);

extern void __GXSetRange(f32 nearz, f32 side);
#pragma fp_contract off
void GXProject(f32 x, f32 y, f32 z, f32 mtx[3][4], f32 *pm, f32 *vp, f32 *sx, f32 *sy,
               f32 *sz)
{
    Vec3 peye;
    f32 xc;
    f32 yc;
    f32 zc;
    f32 wc;

    peye.x = mtx[0][3] + ((mtx[0][2] * z) + ((mtx[0][0] * x) + (mtx[0][1] * y)));
    peye.y = mtx[1][3] + ((mtx[1][2] * z) + ((mtx[1][0] * x) + (mtx[1][1] * y)));
    peye.z = mtx[2][3] + ((mtx[2][2] * z) + ((mtx[2][0] * x) + (mtx[2][1] * y)));
    if (pm[0] == 0.0f) {
        xc = (peye.x * pm[1]) + (peye.z * pm[2]);
        yc = (peye.y * pm[3]) + (peye.z * pm[4]);
        zc = pm[6] + (peye.z * pm[5]);
        wc = 1.0f / -peye.z;
    } else {
        xc = pm[2] + (peye.x * pm[1]);
        yc = pm[4] + (peye.y * pm[3]);
        zc = pm[6] + (peye.z * pm[5]);
        wc = 1.0f;
    }
    *sx = (vp[2] / 2.0f) + (vp[0] + (wc * (xc * vp[2] / 2.0f)));
    *sy = (vp[3] / 2.0f) + (vp[1] + (wc * (-yc * vp[3] / 2.0f)));
    *sz = vp[5] + (wc * (zc * (vp[5] - vp[4])));
}

void GXSetProjection(f32 mtx[4][4], GXProjectionType type)
{
    u32 reg;

    gx->projType = type;
    gx->projMtx[0] = mtx[0][0];
    gx->projMtx[2] = mtx[1][1];
    gx->projMtx[4] = mtx[2][2];
    gx->projMtx[5] = mtx[2][3];
    if (type == GX_ORTHOGRAPHIC) {
        gx->projMtx[1] = mtx[0][3];
        gx->projMtx[3] = mtx[1][3];
    } else {
        gx->projMtx[1] = mtx[0][2];
        gx->projMtx[3] = mtx[1][2];
    }

    reg = 0x00061020;
    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
    GX_WRITE_F32(gx->projMtx[0]);
    GX_WRITE_F32(gx->projMtx[1]);
    GX_WRITE_F32(gx->projMtx[2]);
    GX_WRITE_F32(gx->projMtx[3]);
    GX_WRITE_F32(gx->projMtx[4]);
    GX_WRITE_F32(gx->projMtx[5]);
    GX_WRITE_U32(gx->projType);
    gx->bpSent = 1;
}

#pragma peephole on
void GXGetProjectionv(f32 *ptr)
{

    ptr[0] = gx->projType;
    ptr[1] = gx->projMtx[0];
    ptr[2] = gx->projMtx[1];
    ptr[3] = gx->projMtx[2];
    ptr[4] = gx->projMtx[3];
    ptr[5] = gx->projMtx[4];
    ptr[6] = gx->projMtx[5];
}

#pragma peephole off
void GXLoadPosMtxImm(f32 mtx[3][4], u32 id)
{
    u32 reg;
    u32 addr;

    addr = id * 4;
    reg = addr | 0xB0000;

    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
    WriteMTXPS4x3(mtx, &GXWGFifo.f32);
}

void GXLoadNrmMtxImm(f32 mtx[3][4], u32 id)
{
    u32 reg;
    u32 addr;

    addr = id * 3 + 0x400;
    reg = addr | 0x80000;

    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
    WriteMTXPS3x3from3x4(mtx, &GXWGFifo.f32);
}

void GXSetCurrentMtx(u32 id)
{

    GX_SET_FIELD(gx->matIdxA, 6, 0, id);
    __GXSetMatrixIndex(GX_VA_PNMTXIDX);
}

void GXLoadTexMtxImm(f32 mtx[][4], u32 id, GXTexMtxType type)
{
    u32 reg;
    u32 addr;
    u32 count;

    if (id >= GX_PTTEXMTX0) {
        addr = (id - GX_PTTEXMTX0) * 4 + 0x500;

    } else {
        addr = id * 4;
    }
    count = (type == GX_MTX2x4) ? 8 : 12;
    reg = addr | ((count - 1) << 16);

    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
    if (type == GX_MTX3x4) {
        WriteMTXPS4x3(mtx, &GXWGFifo.f32);
    } else {
        WriteMTXPS4x2(mtx, &GXWGFifo.f32);
    }
}

void GXSetViewportJitter(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz,
                         u32 field)
{
    f32 sx;
    f32 sy;
    f32 sz;
    f32 ox;
    f32 oy;
    f32 oz;
    f32 zmin;
    f32 zmax;
    u32 reg;

    if (field == 0) {
        top -= 0.5f;
    }
    sx = wd / 2.0f;
    sy = -ht / 2.0f;
    ox = 342.0f + (left + (wd / 2.0f));
    oy = 342.0f + (top + (ht / 2.0f));
    zmin = 1.6777215e7f * nearz;
    zmax = 1.6777215e7f * farz;
    sz = zmax - zmin;
    oz = zmax;
    gx->vpLeft = left;
    gx->vpTop = top;
    gx->vpWd = wd;
    gx->vpHt = ht;
    gx->vpNearz = nearz;
    gx->vpFarz = farz;

    if (gx->fgRange != 0) {
        __GXSetRange(nearz, gx->fgSideX);
    }
    reg = 0x5101A;
    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
    GX_WRITE_F32(sx);
    GX_WRITE_F32(sy);
    GX_WRITE_F32(sz);
    GX_WRITE_F32(ox);
    GX_WRITE_F32(oy);
    GX_WRITE_F32(oz);
    gx->bpSent = 1;
}

#pragma dont_inline on
void GXSetViewport(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz)
{
    GXSetViewportJitter(left, top, wd, ht, nearz, farz, 1U);
}

#pragma dont_inline off
void GXGetViewportv(f32 *vp)
{

    vp[0] = gx->vpLeft;
    vp[1] = gx->vpTop;
    vp[2] = gx->vpWd;
    vp[3] = gx->vpHt;
    vp[4] = gx->vpNearz;
    vp[5] = gx->vpFarz;
}

void GXSetScissor(u32 left, u32 top, u32 wd, u32 ht)
{
    u32 tp;
    u32 lf;
    u32 bm;
    u32 rt;

    tp = top + 342;
    lf = left + 342;
    bm = tp + ht - 1;
    rt = lf + wd - 1;

    GX_SET_FIELD(gx->suScis0, 11, 0, tp);
    GX_SET_FIELD(gx->suScis0, 11, 12, lf);
    GX_SET_FIELD(gx->suScis1, 11, 0, bm);
    GX_SET_FIELD(gx->suScis1, 11, 12, rt);

    GX_WRITE_RAS_REG(gx->suScis0);
    GX_WRITE_RAS_REG(gx->suScis1);
    gx->bpSent = 0;
}

void GXSetScissorBoxOffset(s32 x_off, s32 y_off)
{
    u32 reg = 0;
    u32 hx;
    u32 hy;

    hx = (u32)(x_off + 342) >> 1;
    hy = (u32)(y_off + 342) >> 1;

    GX_SET_FIELD(reg, 10, 0, hx);
    GX_SET_FIELD(reg, 10, 10, hy);
    GX_SET_FIELD(reg, 8, 24, 0x59);
    GX_WRITE_RAS_REG(reg);
    gx->bpSent = 0;
}

void GXSetClipMode(GXClipMode mode)
{

    GX_WRITE_XF_REG(5, mode);
    gx->bpSent = 1;
}

void __GXSetMatrixIndex(s32 matIdxAttr)
{
    if (matIdxAttr < GX_VA_TEX4MTXIDX) {
        GX_WRITE_CP_REG(0x30, gx->matIdxA);
        GX_WRITE_XF_REG(24, gx->matIdxA);
    } else {
        GX_WRITE_CP_REG(0x40, gx->matIdxB);
        GX_WRITE_XF_REG(25, gx->matIdxB);
    }
    gx->bpSent = 1;
}

static void WriteMTXPS4x3(f32 mtx[3][4], volatile f32 *destination)
{
    ASM_INSN(psq_l f0, 0(r3), 0, 0);
    ASM_INSN(psq_l f1, 8(r3), 0, 0);
    ASM_INSN(psq_l f2, 16(r3), 0, 0);
    ASM_INSN(psq_l f3, 24(r3), 0, 0);
    ASM_INSN(psq_l f4, 32(r3), 0, 0);
    ASM_INSN(psq_l f5, 40(r3), 0, 0);
    ASM_INSN(psq_st f0, 0(r4), 0, 0);
    ASM_INSN(psq_st f1, 0(r4), 0, 0);
    ASM_INSN(psq_st f2, 0(r4), 0, 0);
    ASM_INSN(psq_st f3, 0(r4), 0, 0);
    ASM_INSN(psq_st f4, 0(r4), 0, 0);
    ASM_INSN(psq_st f5, 0(r4), 0, 0);
}

static void WriteMTXPS3x3from3x4(f32 mtx[3][4], volatile f32 *destination)
{
    ASM_INSN(psq_l f0, 0(r3), 0, 0);
    ASM_INSN(lfs f1, 8(r3));
    ASM_INSN(psq_l f2, 16(r3), 0, 0);
    ASM_INSN(lfs f3, 24(r3));
    ASM_INSN(psq_l f4, 32(r3), 0, 0);
    ASM_INSN(lfs f5, 40(r3));
    ASM_INSN(psq_st f0, 0(r4), 0, 0);
    ASM_INSN(stfs f1, 0(r4));
    ASM_INSN(psq_st f2, 0(r4), 0, 0);
    ASM_INSN(stfs f3, 0(r4));
    ASM_INSN(psq_st f4, 0(r4), 0, 0);
    ASM_INSN(stfs f5, 0(r4));
}

static void WriteMTXPS4x2(f32 mtx[2][4], volatile f32 *destination)
{
    ASM_INSN(psq_l f0, 0(r3), 0, 0);
    ASM_INSN(psq_l f1, 8(r3), 0, 0);
    ASM_INSN(psq_l f2, 16(r3), 0, 0);
    ASM_INSN(psq_l f3, 24(r3), 0, 0);
    ASM_INSN(psq_st f0, 0(r4), 0, 0);
    ASM_INSN(psq_st f1, 0(r4), 0, 0);
    ASM_INSN(psq_st f2, 0(r4), 0, 0);
    ASM_INSN(psq_st f3, 0(r4), 0, 0);
}
