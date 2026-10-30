/* Source: https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/mtx/mtx.c */
#include "types.h"
#include "asm_ops.h"

typedef f32 Mtx[3][4];
typedef struct {
    f32 x, y, z, w;
} Quaternion;
typedef Vec3 Vec;

extern f32 sinf(f32 value);
extern f32 cosf(f32 value);
extern void PSVECNormalize(Vec *source, Vec *destination);
void PSMTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA);

static f32 Unit01[2] = {0.0f, 1.0f};

void PSMTXIdentity(register Mtx m)
{
    register f32 c_zero = 0.0f;
    register f32 c_one  = 1.0f;
    register f32 c_01;
    register f32 c_10;

    ASM_INSN(psq_st c_zero, 8(m), 0, 0);
    ASM_INSN(ps_merge01 c_01, c_zero, c_one);
    ASM_INSN(psq_st c_zero, 24(m), 0, 0);
    ASM_INSN(ps_merge10 c_10, c_one, c_zero);
    ASM_INSN(psq_st c_zero, 32(m), 0, 0);
    ASM_INSN(psq_st c_01, 16(m), 0, 0);
    ASM_INSN(psq_st c_10, 0(m), 0, 0);
    ASM_INSN(psq_st c_10, 40(m), 0, 0);
}

void PSMTXCopy(Mtx src, Mtx dst)
{
    ASM_INSN(psq_l f0, 0(r3), 0, 0);
    ASM_INSN(psq_st f0, 0(r4), 0, 0);
    ASM_INSN(psq_l f1, 8(r3), 0, 0);
    ASM_INSN(psq_st f1, 8(r4), 0, 0);
    ASM_INSN(psq_l f2, 16(r3), 0, 0);
    ASM_INSN(psq_st f2, 16(r4), 0, 0);
    ASM_INSN(psq_l f3, 24(r3), 0, 0);
    ASM_INSN(psq_st f3, 24(r4), 0, 0);
    ASM_INSN(psq_l f4, 32(r3), 0, 0);
    ASM_INSN(psq_st f4, 32(r4), 0, 0);
    ASM_INSN(psq_l f5, 40(r3), 0, 0);
    ASM_INSN(psq_st f5, 40(r4), 0, 0);
}

asm void PSMTXConcat(Mtx a, Mtx b, Mtx ab)
{
    nofralloc
    stwu r1, -0x40(r1)
    psq_l f0, 0(r3), 0, 0
    stfd f14, 8(r1)
    psq_l f6, 0(r4), 0, 0
    lis r6, Unit01@ha
    psq_l f7, 8(r4), 0, 0
    stfd f15, 0x10(r1)
    addi r6, r6, Unit01@l
    stfd f31, 0x28(r1)
    psq_l f8, 0x10(r4), 0, 0
    ps_muls0 f12, f6, f0
    psq_l f2, 0x10(r3), 0, 0
    ps_muls0 f13, f7, f0
    psq_l f31, 0(r6), 0, 0
    ps_muls0 f14, f6, f2
    psq_l f9, 0x18(r4), 0, 0
    ps_muls0 f15, f7, f2
    psq_l f1, 8(r3), 0, 0
    ps_madds1 f12, f8, f0, f12
    psq_l f3, 0x18(r3), 0, 0
    ps_madds1 f14, f8, f2, f14
    psq_l f10, 0x20(r4), 0, 0
    ps_madds1 f13, f9, f0, f13
    psq_l f11, 0x28(r4), 0, 0
    ps_madds1 f15, f9, f2, f15
    psq_l f4, 0x20(r3), 0, 0
    psq_l f5, 0x28(r3), 0, 0
    ps_madds0 f12, f10, f1, f12
    ps_madds0 f13, f11, f1, f13
    ps_madds0 f14, f10, f3, f14
    ps_madds0 f15, f11, f3, f15
    psq_st f12, 0(r5), 0, 0
    ps_muls0 f2, f6, f4
    ps_madds1 f13, f31, f1, f13
    ps_muls0 f0, f7, f4
    psq_st f14, 0x10(r5), 0, 0
    ps_madds1 f15, f31, f3, f15
    psq_st f13, 8(r5), 0, 0
    ps_madds1 f2, f8, f4, f2
    ps_madds1 f0, f9, f4, f0
    ps_madds0 f2, f10, f5, f2
    lfd f14, 8(r1)
    psq_st f15, 0x18(r5), 0, 0
    ps_madds0 f0, f11, f5, f0
    psq_st f2, 0x20(r5), 0, 0
    ps_madds1 f0, f31, f5, f0
    lfd f15, 0x10(r1)
    psq_st f0, 0x28(r5), 0, 0
    lfd f31, 0x28(r1)
    addi r1, r1, 0x40
    blr
}

void PSMTXTranspose(register Mtx src, register Mtx xPose)
{
    register f32 c_zero = 0;
    register f32 row0a;
    register f32 row1a;
    register f32 row0b;
    register f32 row1b;
    register f32 trns0;
    register f32 trns1;
    register f32 trns2;

    ASM_INSN(psq_l row0a, 0(src), 0, 0);
    xPose[2][3] = c_zero;
    ASM_INSN(psq_l row1a, 16(src), 0, 0);
    ASM_INSN(ps_merge00 trns0, row0a, row1a);
    ASM_INSN(psq_l row0b, 8(src), 1, 0);
    ASM_INSN(ps_merge11 trns1, row0a, row1a);
    ASM_INSN(psq_l row1b, 24(src), 1, 0);
    ASM_INSN(psq_st trns0, 0(xPose), 0, 0);
    ASM_INSN(psq_l row0a, 32(src), 0, 0);
    ASM_INSN(ps_merge00 trns2, row0b, row1b);
    ASM_INSN(psq_st trns1, 16(xPose), 0, 0);
    ASM_INSN(ps_merge00 trns0, row0a, c_zero);
    ASM_INSN(psq_st trns2, 32(xPose), 0, 0);
    ASM_INSN(ps_merge10 trns1, row0a, c_zero);
    ASM_INSN(psq_st trns0, 8(xPose), 0, 0);
    row0b = src[2][2];
    ASM_INSN(psq_st trns1, 24(xPose), 0, 0);
    xPose[2][2] = row0b;
}

u32 PSMTXInverse(Mtx src, Mtx inv)
{
    ASM_INSN(psq_l f0, 0(r3), 1, 0);
    ASM_INSN(psq_l f1, 4(r3), 0, 0);
    ASM_INSN(psq_l f2, 16(r3), 1, 0);
    ASM_INSN(ps_merge10 f6, f1, f0);
    ASM_INSN(psq_l f3, 20(r3), 0, 0);
    ASM_INSN(psq_l f4, 32(r3), 1, 0);
    ASM_INSN(ps_merge10 f7, f3, f2);
    ASM_INSN(psq_l f5, 36(r3), 0, 0);
    ASM_INSN(ps_mul f11, f3, f6);
    ASM_INSN(ps_mul f13, f5, f7);
    ASM_INSN(ps_merge10 f8, f5, f4);
    ASM_INSN(ps_msub f11, f1, f7, f11);
    ASM_INSN(ps_mul f12, f1, f8);
    ASM_INSN(ps_msub f13, f3, f8, f13);
    ASM_INSN(ps_mul f10, f3, f4);
    ASM_INSN(ps_msub f12, f5, f6, f12);
    ASM_INSN(ps_mul f9, f0, f5);
    ASM_INSN(ps_mul f8, f1, f2);
    ASM_INSN(ps_sub f6, f6, f6);
    ASM_INSN(ps_msub f10, f2, f5, f10);
    ASM_INSN(ps_mul f7, f0, f13);
    ASM_INSN(ps_msub f9, f1, f4, f9);
    ASM_INSN(ps_madd f7, f2, f12, f7);
    ASM_INSN(ps_msub f8, f0, f3, f8);
    ASM_INSN(ps_madd f7, f4, f11, f7);
    ASM_INSN(ps_cmpo0 cr0, f7, f6);
    ASM_INSN(bne skip_return);
    ASM_INSN(li r3, 0);
    ASM_INSN(blr);
    ASM_INSN(skip_return: fres f0, f7);
    ASM_INSN(ps_add f6, f0, f0);
    ASM_INSN(ps_mul f5, f0, f0);
    ASM_INSN(ps_nmsub f0, f7, f5, f6);
    ASM_INSN(lfs f1, 12(r3));
    ASM_INSN(ps_muls0 f13, f13, f0);
    ASM_INSN(lfs f2, 28(r3));
    ASM_INSN(ps_muls0 f12, f12, f0);
    ASM_INSN(lfs f3, 44(r3));
    ASM_INSN(ps_muls0 f11, f11, f0);
    ASM_INSN(ps_merge00 f5, f13, f12);
    ASM_INSN(ps_muls0 f10, f10, f0);
    ASM_INSN(ps_merge11 f4, f13, f12);
    ASM_INSN(ps_muls0 f9, f9, f0);
    ASM_INSN(psq_st f5, 0(r4), 0, 0);
    ASM_INSN(ps_mul f6, f13, f1);
    ASM_INSN(psq_st f4, 16(r4), 0, 0);
    ASM_INSN(ps_muls0 f8, f8, f0);
    ASM_INSN(ps_madd f6, f12, f2, f6);
    ASM_INSN(psq_st f10, 32(r4), 1, 0);
    ASM_INSN(ps_nmadd f6, f11, f3, f6);
    ASM_INSN(psq_st f9, 36(r4), 1, 0);
    ASM_INSN(ps_mul f7, f10, f1);
    ASM_INSN(ps_merge00 f5, f11, f6);
    ASM_INSN(psq_st f8, 40(r4), 1, 0);
    ASM_INSN(ps_merge11 f4, f11, f6);
    ASM_INSN(psq_st f5, 8(r4), 0, 0);
    ASM_INSN(ps_madd f7, f9, f2, f7);
    ASM_INSN(psq_st f4, 24(r4), 0, 0);
    ASM_INSN(ps_nmadd f7, f8, f3, f7);
    ASM_INSN(li r3, 1);
    ASM_INSN(psq_st f7, 44(r4), 1, 0);
}

u32 PSMTXInvXpose(Mtx src, Mtx invX)
{
    ASM_INSN(psq_l f0, 0(r3), 1, 0);
    ASM_INSN(psq_l f1, 4(r3), 0, 0);
    ASM_INSN(psq_l f2, 16(r3), 1, 0);
    ASM_INSN(ps_merge10 f6, f1, f0);
    ASM_INSN(psq_l f3, 20(r3), 0, 0);
    ASM_INSN(psq_l f4, 32(r3), 1, 0);
    ASM_INSN(ps_merge10 f7, f3, f2);
    ASM_INSN(psq_l f5, 36(r3), 0, 0);
    ASM_INSN(ps_mul f11, f3, f6);
    ASM_INSN(ps_merge10 f8, f5, f4);
    ASM_INSN(ps_mul f13, f5, f7);
    ASM_INSN(ps_msub f11, f1, f7, f11);
    ASM_INSN(ps_mul f12, f1, f8);
    ASM_INSN(ps_msub f13, f3, f8, f13);
    ASM_INSN(ps_msub f12, f5, f6, f12);
    ASM_INSN(ps_mul f10, f3, f4);
    ASM_INSN(ps_mul f9, f0, f5);
    ASM_INSN(ps_mul f8, f1, f2);
    ASM_INSN(ps_msub f10, f2, f5, f10);
    ASM_INSN(ps_msub f9, f1, f4, f9);
    ASM_INSN(ps_msub f8, f0, f3, f8);
    ASM_INSN(ps_mul f7, f0, f13);
    ASM_INSN(ps_sub f1, f1, f1);
    ASM_INSN(ps_madd f7, f2, f12, f7);
    ASM_INSN(ps_madd f7, f4, f11, f7);
    ASM_INSN(ps_cmpo0 cr0, f7, f1);
    ASM_INSN(bne skip_return);
    ASM_INSN(li r3, 0);
    ASM_INSN(blr);
    ASM_INSN(skip_return: fres f0, f7);
    ASM_INSN(psq_st f1, 12(r4), 1, 0);
    ASM_INSN(ps_add f6, f0, f0);
    ASM_INSN(ps_mul f5, f0, f0);
    ASM_INSN(psq_st f1, 28(r4), 1, 0);
    ASM_INSN(ps_nmsub f0, f7, f5, f6);
    ASM_INSN(psq_st f1, 44(r4), 1, 0);
    ASM_INSN(ps_muls0 f13, f13, f0);
    ASM_INSN(ps_muls0 f12, f12, f0);
    ASM_INSN(ps_muls0 f11, f11, f0);
    ASM_INSN(psq_st f13, 0(r4), 0, 0);
    ASM_INSN(psq_st f12, 16(r4), 0, 0);
    ASM_INSN(ps_muls0 f10, f10, f0);
    ASM_INSN(ps_muls0 f9, f9, f0);
    ASM_INSN(psq_st f11, 32(r4), 0, 0);
    ASM_INSN(psq_st f10, 8(r4), 1, 0);
    ASM_INSN(ps_muls0 f8, f8, f0);
    ASM_INSN(li r3, 1);
    ASM_INSN(psq_st f9, 24(r4), 1, 0);
    ASM_INSN(psq_st f8, 40(r4), 1, 0);
}

#pragma peephole off
void PSMTXRotRad(Mtx m, char axis, f32 rad)
{
    f32 s = sinf(rad);
    f32 c = cosf(rad);

    PSMTXRotTrig(m, axis, s, c);
}

void PSMTXRotTrig(register Mtx m, register char axis, register f32 sinA, register f32 cosA)
{
    register f32 fc0;
    register f32 fc1;
    register f32 nsinA;
    register f32 fw0, fw1, fw2, fw3;

    fc0 = 0.0f;
    fc1 = 1.0f;

    ASM_INSN(ori axis, axis, 0x20);
    ASM_INSN(ps_neg nsinA, sinA);
    ASM_INSN(cmplwi axis, 'x');
    ASM_INSN(beq case_x);
    ASM_INSN(cmplwi axis, 'y');
    ASM_INSN(beq case_y);
    ASM_INSN(cmplwi axis, 'z');
    ASM_INSN(beq case_z);
    ASM_INSN(b end);
    ASM_INSN(case_x: psq_st fc1, 0(m), 1, 0);
    ASM_INSN(psq_st fc0, 4(m), 0, 0);
    ASM_INSN(ps_merge00 fw0, sinA, cosA);
    ASM_INSN(psq_st fc0, 12(m), 0, 0);
    ASM_INSN(ps_merge00 fw1, cosA, nsinA);
    ASM_INSN(psq_st fc0, 28(m), 0, 0);
    ASM_INSN(psq_st fc0, 44(m), 1, 0);
    ASM_INSN(psq_st fw0, 36(m), 0, 0);
    ASM_INSN(psq_st fw1, 20(m), 0, 0);
    ASM_INSN(b end);
    ASM_INSN(case_y: ps_merge00 fw0, cosA, fc0);
    ASM_INSN(ps_merge00 fw1, fc0, fc1);
    ASM_INSN(psq_st fc0, 24(m), 0, 0);
    ASM_INSN(psq_st fw0, 0(m), 0, 0);
    ASM_INSN(ps_merge00 fw2, nsinA, fc0);
    ASM_INSN(ps_merge00 fw3, sinA, fc0);
    ASM_INSN(psq_st fw0, 40(m), 0, 0);
    ASM_INSN(psq_st fw1, 16(m), 0, 0);
    ASM_INSN(psq_st fw3, 8(m), 0, 0);
    ASM_INSN(psq_st fw2, 32(m), 0, 0);
    ASM_INSN(b end);
    ASM_INSN(case_z: psq_st fc0, 8(m), 0, 0);
    ASM_INSN(ps_merge00 fw0, sinA, cosA);
    ASM_INSN(ps_merge00 fw2, cosA, nsinA);
    ASM_INSN(psq_st fc0, 24(m), 0, 0);
    ASM_INSN(psq_st fc0, 32(m), 0, 0);
    ASM_INSN(ps_merge00 fw1, fc1, fc0);
    ASM_INSN(psq_st fw0, 16(m), 0, 0);
    ASM_INSN(psq_st fw2, 0(m), 0, 0);
    ASM_INSN(psq_st fw1, 40(m), 0, 0);
end:;
}

void PSMTXRotAxisRad(register Mtx m, Vec* axis, register f32 rad)
{
    register f32 tmp0, tmp1, tmp2, tmp3, tmp4;
    register f32 tmp5, tmp6, tmp7, tmp8, tmp9;

    register f32 sT;
    register f32 cT;
    register f32 oneMinusCosT;
    register f32 zero;
    Vec axisNormalized;
    register Vec* axisNormalizedPtr;

    zero              = 0.0f;
    axisNormalizedPtr = &axisNormalized;
    sT                = sinf(rad);
    cT                = cosf(rad);
    oneMinusCosT      = 1.0f - cT;

    PSVECNormalize(axis, axisNormalizedPtr);

    ASM_INSN(psq_l rad, 0x0(axisNormalizedPtr), 0, 0);
    ASM_INSN(lfs tmp1, 0x8(axisNormalizedPtr));
    ASM_INSN(ps_merge00 tmp0, cT, cT);
    ASM_INSN(ps_muls0 tmp4, rad, oneMinusCosT);
    ASM_INSN(ps_muls0 tmp5, tmp1, oneMinusCosT);
    ASM_INSN(ps_muls1 tmp3, tmp4, rad);
    ASM_INSN(ps_muls0 tmp2, tmp4, rad);
    ASM_INSN(ps_muls0 rad, rad, sT);
    ASM_INSN(ps_muls0 tmp4, tmp4, tmp1);
    ASM_INSN(fnmsubs tmp6, tmp1, sT, tmp3);
    ASM_INSN(fmadds tmp7, tmp1, sT, tmp3);
    ASM_INSN(ps_neg tmp9, rad);
    ASM_INSN(ps_sum0 tmp8, tmp4, zero, rad);
    ASM_INSN(ps_sum0 tmp2, tmp2, tmp6, tmp0);
    ASM_INSN(ps_sum1 tmp3, tmp0, tmp7, tmp3);
    ASM_INSN(ps_sum0 tmp6, tmp9, zero, tmp4);
    ASM_INSN(ps_sum0 tmp9, tmp4, tmp4, tmp9);
    ASM_INSN(psq_st tmp8, 0x8(m), 0, 0);
    ASM_INSN(ps_muls0 tmp5, tmp5, tmp1);
    ASM_INSN(psq_st tmp2, 0x0(m), 0, 0);
    ASM_INSN(ps_sum1 tmp4, rad, tmp9, tmp4);
    ASM_INSN(psq_st tmp3, 0x10(m), 0, 0);
    ASM_INSN(ps_sum0 tmp5, tmp5, zero, tmp0);
    ASM_INSN(psq_st tmp6, 0x18(m), 0, 0);
    ASM_INSN(psq_st tmp4, 0x20(m), 0, 0);
    ASM_INSN(psq_st tmp5, 0x28(m), 0, 0);
}

void PSMTXTrans(register Mtx m, register f32 xT, register f32 yT, register f32 zT)
{
    register f32 c0 = 0.0F;
    register f32 c1 = 1.0F;
    ASM_INSN(stfs xT, 12(m));
    ASM_INSN(stfs yT, 28(m));
    ASM_INSN(psq_st c0, 4(m), 0, 0);
    ASM_INSN(psq_st c0, 32(m), 0, 0);
    ASM_INSN(stfs c0, 16(m));
    ASM_INSN(stfs c1, 20(m));
    ASM_INSN(stfs c0, 24(m));
    ASM_INSN(stfs c1, 40(m));
    ASM_INSN(stfs zT, 44(m));
    ASM_INSN(stfs c1, 0(m));
}

void PSMTXScale(register Mtx m, register f32 xS, register f32 yS, register f32 zS)
{
    register f32 c0 = 0.0F;
    ASM_INSN(stfs xS, 0(m));
    ASM_INSN(psq_st c0, 4(m), 0, 0);
    ASM_INSN(psq_st c0, 12(m), 0, 0);
    ASM_INSN(stfs yS, 20(m));
    ASM_INSN(psq_st c0, 24(m), 0, 0);
    ASM_INSN(psq_st c0, 32(m), 0, 0);
    ASM_INSN(stfs zS, 40(m));
    ASM_INSN(stfs c0, 44(m));
}

void PSMTXQuat(register Mtx m, register Quaternion* q)
{
    register f32 c_zero, c_one, c_two, scale;
    register f32 tmp0, tmp1, tmp2, tmp3, tmp4;
    register f32 tmp5, tmp6, tmp7, tmp8, tmp9;

    c_one = 1.0F;
    ASM_INSN(psq_l tmp0, 0(q), 0, 0);
    ASM_INSN(psq_l tmp1, 8(q), 0, 0);
    ASM_INSN(fsubs c_zero, c_one, c_one);
    ASM_INSN(fadds c_two, c_one, c_one);
    ASM_INSN(ps_mul tmp2, tmp0, tmp0);
    ASM_INSN(ps_merge10 tmp5, tmp0, tmp0);
    ASM_INSN(ps_madd tmp4, tmp1, tmp1, tmp2);
    ASM_INSN(ps_mul tmp3, tmp1, tmp1);
    ASM_INSN(ps_sum0 scale, tmp4, tmp4, tmp4);
    ASM_INSN(ps_muls1 tmp7, tmp5, tmp1);
    ASM_INSN(fres tmp9, scale);
    ASM_INSN(ps_sum1 tmp4, tmp3, tmp4, tmp2);
    ASM_INSN(ps_nmsub scale, scale, tmp9, c_two);
    ASM_INSN(ps_muls1 tmp6, tmp1, tmp1);
    ASM_INSN(ps_mul scale, tmp9, scale);
    ASM_INSN(ps_sum0 tmp2, tmp2, tmp2, tmp2);
    ASM_INSN(fmuls scale, scale, c_two);
    ASM_INSN(ps_madd tmp8, tmp0, tmp5, tmp6);
    ASM_INSN(ps_msub tmp6, tmp0, tmp5, tmp6);
    ASM_INSN(psq_st c_zero, 12(m), 1, 0);
    ASM_INSN(ps_nmsub tmp2, tmp2, scale, c_one);
    ASM_INSN(ps_nmsub tmp4, tmp4, scale, c_one);
    ASM_INSN(psq_st c_zero, 44(m), 1, 0);
    ASM_INSN(ps_mul tmp8, tmp8, scale);
    ASM_INSN(ps_mul tmp6, tmp6, scale);
    ASM_INSN(psq_st tmp2, 40(m), 1, 0);
    ASM_INSN(ps_madds0 tmp5, tmp0, tmp1, tmp7);
    ASM_INSN(ps_merge00 tmp1, tmp8, tmp4);
    ASM_INSN(ps_nmsub tmp7, tmp7, c_two, tmp5);
    ASM_INSN(ps_merge10 tmp0, tmp4, tmp6);
    ASM_INSN(psq_st tmp1, 16(m), 0, 0);
    ASM_INSN(ps_mul tmp5, tmp5, scale);
    ASM_INSN(ps_mul tmp7, tmp7, scale);
    ASM_INSN(psq_st tmp0, 0(m), 0, 0);
    ASM_INSN(psq_st tmp5, 8(m), 1, 0);
    ASM_INSN(ps_merge10 tmp3, tmp7, c_zero);
    ASM_INSN(ps_merge01 tmp9, tmp7, tmp5);
    ASM_INSN(psq_st tmp3, 24(m), 0, 0);
    ASM_INSN(psq_st tmp9, 32(m), 0, 0);
}

