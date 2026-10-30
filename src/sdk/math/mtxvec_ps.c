/* Source: https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/mtx/mtxvec.c */
#include "types.h"
#include "asm_ops.h"

typedef f32 Mtx[3][4];

void PSMTXMultVec(Mtx m, Vec3 * src, Vec3 * dst)
{
    ASM_INSN(psq_l f0, 0(r4), 0, 0);
    ASM_INSN(psq_l f2, 0(r3), 0, 0);
    ASM_INSN(psq_l f1, 8(r4), 1, 0);
    ASM_INSN(ps_mul f4, f2, f0);
    ASM_INSN(psq_l f3, 8(r3), 0, 0);
    ASM_INSN(ps_madd f5, f3, f1, f4);
    ASM_INSN(psq_l f8, 16(r3), 0, 0);
    ASM_INSN(ps_sum0 f6, f5, f6, f5);
    ASM_INSN(psq_l f9, 24(r3), 0, 0);
    ASM_INSN(ps_mul f10, f8, f0);
    ASM_INSN(psq_st f6, 0(r5), 1, 0);
    ASM_INSN(ps_madd f11, f9, f1, f10);
    ASM_INSN(psq_l f2, 32(r3), 0, 0);
    ASM_INSN(ps_sum0 f12, f11, f12, f11);
    ASM_INSN(psq_l f3, 40(r3), 0, 0);
    ASM_INSN(ps_mul f4, f2, f0);
    ASM_INSN(psq_st f12, 4(r5), 1, 0);
    ASM_INSN(ps_madd f5, f3, f1, f4);
    ASM_INSN(ps_sum0 f6, f5, f6, f5);
    ASM_INSN(psq_st f6, 8(r5), 1, 0);
}

void PSMTXMultVecArray(Mtx m, Vec3 * srcBase, Vec3 * dstBase, u32 count)
{
    ASM_INSN(psq_l f13, 0x0(r3), 0, 0);
    ASM_INSN(psq_l f12, 0x10(r3), 0, 0);
    ASM_INSN(subi r6, r6, 0x1);
    ASM_INSN(psq_l f11, 0x8(r3), 0, 0);
    ASM_INSN(ps_merge00 f0, f13, f12);
    ASM_INSN(subi r5, r5, 0x4);
    ASM_INSN(psq_l f10, 0x18(r3), 0, 0);
    ASM_INSN(ps_merge11 f1, f13, f12);
    ASM_INSN(mtctr r6);
    ASM_INSN(psq_l f4, 0x20(r3), 0, 0);
    ASM_INSN(ps_merge00 f2, f11, f10);
    ASM_INSN(psq_l f5, 0x28(r3), 0, 0);
    ASM_INSN(ps_merge11 f3, f11, f10);
    ASM_INSN(psq_l f6, 0(r4), 0, 0);
    ASM_INSN(psq_lu f7, 8(r4), 1, 0);
    ASM_INSN(ps_madds0 f8, f0, f6, f3);
    ASM_INSN(ps_mul f9, f4, f6);
    ASM_INSN(ps_madds1 f8, f1, f6, f8);
    ASM_INSN(ps_madd f10, f5, f7, f9);
    ASM_INSN(loop: psq_lu f6, 4(r4), 0, 0);
    ASM_INSN(ps_madds0 f12, f2, f7, f8);
    ASM_INSN(psq_lu f7, 8(r4), 1, 0);
    ASM_INSN(ps_sum0 f13, f10, f9, f10);
    ASM_INSN(ps_madds0 f8, f0, f6, f3);
    ASM_INSN(ps_mul f9, f4, f6);
    ASM_INSN(psq_stu f12, 0x4(r5), 0, 0);
    ASM_INSN(ps_madds1 f8, f1, f6, f8);
    ASM_INSN(psq_stu f13, 0x8(r5), 1, 0);
    ASM_INSN(ps_madd f10, f5, f7, f9);
    ASM_INSN(bdnz loop);
    ASM_INSN(ps_madds0 f12, f2, f7, f8);
    ASM_INSN(ps_sum0 f13, f10, f9, f10);
    ASM_INSN(psq_stu f12, 0x4(r5), 0, 0);
    ASM_INSN(psq_stu f13, 0x8(r5), 1, 0);
}

void PSMTXMultVecSR(Mtx m, Vec3 * src, Vec3 * dst)
{
    ASM_INSN(psq_l f0, 0x0(r3), 0, 0);
    ASM_INSN(psq_l f6, 0(r4), 0, 0);
    ASM_INSN(psq_l f2, 0x10(r3), 0, 0);
    ASM_INSN(ps_mul f8, f0, f6);
    ASM_INSN(psq_l f4, 0x20(r3), 0, 0);
    ASM_INSN(ps_mul f10, f2, f6);
    ASM_INSN(psq_l f7, 8(r4), 1, 0);
    ASM_INSN(ps_mul f12, f4, f6);
    ASM_INSN(psq_l f3, 0x18(r3), 0, 0);
    ASM_INSN(ps_sum0 f8, f8, f8, f8);
    ASM_INSN(psq_l f5, 0x28(r3), 0, 0);
    ASM_INSN(ps_sum0 f10, f10, f10, f10);
    ASM_INSN(psq_l f1, 0x8(r3), 0, 0);
    ASM_INSN(ps_sum0 f12, f12, f12, f12);
    ASM_INSN(ps_madd f9, f1, f7, f8);
    ASM_INSN(psq_st f9, 0(r5), 1, 0);
    ASM_INSN(ps_madd f11, f3, f7, f10);
    ASM_INSN(psq_st f11, 4(r5), 1, 0);
    ASM_INSN(ps_madd f13, f5, f7, f12);
    ASM_INSN(psq_st f13, 8(r5), 1, 0);
}

