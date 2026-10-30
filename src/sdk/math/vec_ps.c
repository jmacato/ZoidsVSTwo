/* Source: https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/mtx/vec.c */
#include "types.h"
#include "asm_ops.h"

void PSVECAdd(Vec3 * a, Vec3 * b, Vec3 * c)
{
    ASM_INSN(psq_l f2, 0(r3), 0, 0);
    ASM_INSN(psq_l f4, 0(r4), 0, 0);
    ASM_INSN(ps_add f6, f2, f4);
    ASM_INSN(psq_st f6, 0(r5), 0, 0);
    ASM_INSN(psq_l f3, 8(r3), 1, 0);
    ASM_INSN(psq_l f5, 8(r4), 1, 0);
    ASM_INSN(ps_add f7, f3, f5);
    ASM_INSN(psq_st f7, 8(r5), 1, 0);
}

void PSVECSubtract(Vec3 * a, Vec3 * b, Vec3 * c)
{
    ASM_INSN(psq_l f2, 0(r3), 0, 0);
    ASM_INSN(psq_l f4, 0(r4), 0, 0);
    ASM_INSN(ps_sub f6, f2, f4);
    ASM_INSN(psq_st f6, 0(r5), 0, 0);
    ASM_INSN(psq_l f3, 8(r3), 1, 0);
    ASM_INSN(psq_l f5, 8(r4), 1, 0);
    ASM_INSN(ps_sub f7, f3, f5);
    ASM_INSN(psq_st f7, 8(r5), 1, 0);
}

void PSVECScale(Vec3 * src, Vec3 * dst, f32 mult)
{
    ASM_INSN(psq_l f0, 0(r3), 0, 0);
    ASM_INSN(psq_l f2, 8(r3), 1, 0);
    ASM_INSN(ps_muls0 f0, f0, f1);
    ASM_INSN(psq_st f0, 0(r4), 0, 0);
    ASM_INSN(ps_muls0 f0, f2, f1);
    ASM_INSN(psq_st f0, 8(r4), 1, 0);
}

void PSVECNormalize(register Vec3 * vec1, register Vec3 * dst)
{
    register f32 c_half  = 0.5f;
    register f32 c_three = 3.0f;
    register f32 v1_xy;
    register f32 v1_z;
    register f32 xx_zz;
    register f32 xx_yy;
    register f32 sqsum;
    register f32 rsqrt;
    register f32 nwork0;
    register f32 nwork1;

    ASM_INSN(psq_l v1_xy, 0(vec1), 0, 0);
    ASM_INSN(ps_mul xx_yy, v1_xy, v1_xy);
    ASM_INSN(psq_l v1_z, 8(vec1), 1, 0);
    ASM_INSN(ps_madd xx_zz, v1_z, v1_z, xx_yy);
    ASM_INSN(ps_sum0 sqsum, xx_zz, v1_z, xx_yy);
    ASM_INSN(frsqrte rsqrt, sqsum);
    ASM_INSN(fmuls nwork0, rsqrt, rsqrt);
    ASM_INSN(fmuls nwork1, rsqrt, c_half);
    ASM_INSN(fnmsubs nwork0, nwork0, sqsum, c_three);
    ASM_INSN(fmuls rsqrt, nwork0, nwork1);
    ASM_INSN(ps_muls0 v1_xy, v1_xy, rsqrt);
    ASM_INSN(psq_st v1_xy, 0(dst), 0, 0);
    ASM_INSN(ps_muls0 v1_z, v1_z, rsqrt);
    ASM_INSN(psq_st v1_z, 8(dst), 1, 0);
}

f32 PSVECMag(Vec3 *v)
{
    register f32 c_three;
    register f32 c_half;

    ASM_INSN(psq_l f0, 0(r3), 0, 0);
    ASM_INSN(ps_mul f0, f0, f0);
    ASM_INSN(lfs f1, 8(r3));
    ASM_INSN(ps_madd f1, f1, f1, f0);
    c_half = 0.5f;
    ASM_INSN(ps_sum0 f1, f1, f0, f0);
    ASM_INSN(frsqrte f0, f1);
    c_three = 3.0f;
    ASM_INSN(fmuls f2, f0, f0);
    ASM_INSN(fmuls f0, f0, c_half);
    ASM_INSN(fnmsubs f2, f2, f1, c_three);
    ASM_INSN(fmuls f0, f2, f0);
    ASM_INSN(fsel f0, f0, f0, f1);
    ASM_INSN(fmuls f1, f1, f0);
}

f32 PSVECDotProduct(Vec3 * vec1, Vec3 * vec2)
{
    ASM_INSN(psq_l f2, 4(r3), 0, 0);
    ASM_INSN(psq_l f3, 4(r4), 0, 0);
    ASM_INSN(ps_mul f2, f2, f3);
    ASM_INSN(psq_l f5, 0(r3), 0, 0);
    ASM_INSN(psq_l f4, 0(r4), 0, 0);
    ASM_INSN(ps_madd f3, f5, f4, f2);
    ASM_INSN(ps_sum0 f1, f3, f2, f2);
}

void PSVECCrossProduct(Vec3 * vec1, Vec3 * vec2, Vec3 * dst)
{
    ASM_INSN(psq_l f1, 0(r4), 0, 0);
    ASM_INSN(lfs f2, 8(r3));
    ASM_INSN(psq_l f0, 0(r3), 0, 0);
    ASM_INSN(ps_merge10 f6, f1, f1);
    ASM_INSN(lfs f3, 8(r4));
    ASM_INSN(ps_mul f4, f1, f2);
    ASM_INSN(ps_muls0 f7, f1, f0);
    ASM_INSN(ps_msub f5, f0, f3, f4);
    ASM_INSN(ps_msub f8, f0, f6, f7);
    ASM_INSN(ps_merge11 f9, f5, f5);
    ASM_INSN(ps_merge01 f10, f5, f8);
    ASM_INSN(psq_st f9, 0(r5), 1, 0);
    ASM_INSN(ps_neg f10, f10);
    ASM_INSN(psq_st f10, 4(r5), 0, 0);
}

