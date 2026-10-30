#include "types.h"

static char bc_s368[] = "bytecode.c";
static char bc_s369[] = "operand < nb_args";
static char bc_s370[] = "specified stack doesn't exist (%d).\n";
static char bc_s371[] = "";
static char bc_s372[] = "stack";
static char bc_s373[] = "not yet implemented.\n";
static char bc_s374[] = "unexpected byte code.\n";
static const f32 bc_zero = 0.0f;
static const f64 bc_d2r = 0.017453292519943295;
static const f64 bc_r2d = 57.29577951308232;
static const f64 bc_half = 0.5;
static const f64 bc_three = 3.0;
static char bc_s379[] = "stack->next";
static const f32 bc_pi2 = 1.5707963705062866f;
static const f32 bc_mpi2 = -1.5707963705062866f;
static char bc_s382[] = "unexpected opcode 0x%x.\n";
static const f64 bc_magic = 4503601774854144.0;


void HSD_Panic(void);
void HSD_Randf(void);
void HSD_Randi(void);
void HSD_SListPrepend(void);
void HSD_SListRemove(void);
void OSReport(void);
void __assert(void);
void __cvt_dbl_usll(void);
void __cvt_sll_flt(void);
void acosf(void);
void asinf(void);
void atan2f(void);
void atanf(void);
void cosf(void);
void expf(void);
void logf(void);
void powf(void);
void sinf(void);
void tanf(void);

asm f32 HSD_ByteCodeEval(u8 *bytecode, u32 *args, u32 nb_args)
{
    nofralloc
    mfspr r0, 8
    stw r0, 4(r1)
    stwu r1, -0xa8(r1)
    stfd f31, 0xa0(r1)
    stfd f30, 0x98(r1)
    stfd f29, 0x90(r1)
    stfd f28, 0x88(r1)
    stfd f27, 0x80(r1)
    stfd f26, 0x78(r1)
    stfd f25, 0x70(r1)
    stfd f24, 0x68(r1)
    stmw r21, 0x3c(r1)
    or. r29, r3, r3
    lis r3, bc_s368@ha
    addi r31, r3, bc_s368@l
    addi r25, r4, 0
    addi r24, r5, 0
    li r23, 0
    li r27, 0
    bne lab_58
    lfs f1, bc_zero(r2)
    b lab_15bc
lab_58:
    lfs f31, bc_d2r-0x8(r2)
    lis r26, 0x4330
    lfd f28, bc_r2d+0x8(r2)
    lfd f27, bc_half+0x8(r2)
    lfd f29, bc_three-0x18(r2)
    lfd f26, bc_pi2+0x8(r2)
    lfd f30, bc_mpi2-0x1c(r2)
lab_74:
    cmpwi r27, 0
    ble lab_20c
    lbz r0, 0(r29)
    addic. r27, r27, -1
    slwi r3, r30, 8
    or r30, r3, r0
    addi r29, r29, 1
    bne lab_74
    cmpwi r28, 6
    beq lab_1d4
    bge lab_c4
    cmpwi r28, 3
    beq lab_194
    bge lab_b8
    cmpwi r28, 2
    bge lab_dc
    b lab_1f8
lab_b8:
    cmpwi r28, 5
    bge lab_10c
    b lab_1cc
lab_c4:
    cmpwi r28, 0xff
    beq lab_1e8
    bge lab_1f8
    cmpwi r28, 0x3c
    beq lab_130
    b lab_1f8
lab_dc:
    cmplw r30, r24
    blt lab_f4
    addi r3, r31, 0
    addi r5, r31, 0xc
    li r4, 0x119
    bl __assert
lab_f4:
    slwi r0, r30, 2
    lwzx r4, r25, r0
    mr r3, r23
    bl HSD_SListPrepend
    mr r23, r3
    b lab_74
lab_10c:
    li r21, 0
    b lab_124
lab_114:
    mr r3, r23
    bl HSD_SListRemove
    addi r23, r3, 0
    addi r21, r21, 1
lab_124:
    cmplw r21, r30
    blt lab_114
    b lab_74
lab_130:
    addi r4, r23, 0
    li r3, 0
    b lab_144
lab_13c:
    lwz r4, 0(r4)
    addi r3, r3, 1
lab_144:
    cmplwi r4, 0
    beq lab_154
    cmplw r3, r30
    blt lab_13c
lab_154:
    cmplwi r4, 0
    bne lab_180
    addi r4, r30, 0
    crxor 6, 6, 6
    addi r3, r31, 0x20
    bl OSReport
    addi r3, r31, 0
    li r4, 0x12b
    addi r5, r13, bc_s371
    bl HSD_Panic
    b lab_74
lab_180:
    mr r3, r23
    lwz r4, 4(r4)
    bl HSD_SListPrepend
    mr r23, r3
    b lab_74
lab_194:
    cmplwi r23, 0
    bne lab_1ac
    addi r3, r31, 0
    li r4, 0x133
    addi r5, r13, bc_s372
    bl __assert
lab_1ac:
    lwz r0, 4(r23)
    cmpwi r0, 0
    beq lab_1bc
    add r29, r29, r30
lab_1bc:
    mr r3, r23
    bl HSD_SListRemove
    mr r23, r3
    b lab_74
lab_1cc:
    add r29, r29, r30
    b lab_74
lab_1d4:
    addi r3, r23, 0
    addi r4, r30, 0
    bl HSD_SListPrepend
    mr r23, r3
    b lab_74
lab_1e8:
    addi r3, r31, 0
    addi r5, r31, 0x48
    li r4, 0x143
    bl HSD_Panic
lab_1f8:
    addi r3, r31, 0
    addi r5, r31, 0x60
    li r4, 0x146
    bl HSD_Panic
    b lab_74
lab_20c:
    lbz r28, 0(r29)
    addi r29, r29, 1
    addi r4, r28, 0
    cmpwi r4, 0x20
    beq lab_b6c
    bge lab_2fc
    cmpwi r4, 0x10
    beq lab_5f4
    bge lab_298
    cmpwi r4, 8
    beq lab_460
    bge lab_26c
    cmpwi r4, 5
    beq lab_40c
    bge lab_260
    cmpwi r4, 1
    beq lab_3cc
    bge lab_418
    cmpwi r4, 0
    bge lab_74
    b lab_159c
lab_260:
    cmpwi r4, 7
    bge lab_430
    b lab_424
lab_26c:
    cmpwi r4, 0xc
    beq lab_520
    bge lab_288
    cmpwi r4, 0xa
    beq lab_4d0
    bge lab_4f8
    b lab_4a0
lab_288:
    cmpwi r4, 0xe
    beq lab_584
    bge lab_5bc
    b lab_54c
lab_298:
    cmpwi r4, 0x18
    beq lab_86c
    bge lab_2d0
    cmpwi r4, 0x14
    beq lab_6cc
    bge lab_2c0
    cmpwi r4, 0x12
    beq lab_664
    bge lab_69c
    b lab_62c
lab_2c0:
    cmpwi r4, 0x16
    beq lab_764
    bge lab_808
    b lab_6fc
lab_2d0:
    cmpwi r4, 0x1c
    beq lab_a1c
    bge lab_2ec
    cmpwi r4, 0x1a
    beq lab_934
    bge lab_998
    b lab_8d0
lab_2ec:
    cmpwi r4, 0x1e
    beq lab_ac4
    bge lab_b18
    b lab_a70
lab_2fc:
    cmpwi r4, 0x30
    beq lab_1340
    bge lab_36c
    cmpwi r4, 0x28
    beq lab_734
    bge lab_340
    cmpwi r4, 0x24
    beq lab_cf4
    bge lab_330
    cmpwi r4, 0x22
    beq lab_c2c
    bge lab_c90
    b lab_bc8
lab_330:
    cmpwi r4, 0x26
    beq lab_da4
    bge lab_153c
    b lab_d4c
lab_340:
    cmpwi r4, 0x2c
    beq lab_11c0
    bge lab_35c
    cmpwi r4, 0x2a
    beq lab_10fc
    bge lab_1160
    b lab_1098
lab_35c:
    cmpwi r4, 0x2e
    beq lab_127c
    bge lab_12d8
    b lab_1220
lab_36c:
    cmpwi r4, 0x38
    beq lab_1030
    bge lab_3a4
    cmpwi r4, 0x34
    beq lab_e98
    bge lab_394
    cmpwi r4, 0x32
    beq lab_13a8
    bge lab_e34
    b lab_7dc
lab_394:
    cmpwi r4, 0x36
    beq lab_f64
    bge lab_fcc
    b lab_efc
lab_3a4:
    cmpwi r4, 0x3c
    beq lab_40c
    bge lab_3c0
    cmpwi r4, 0x3a
    beq lab_1494
    bge lab_14e8
    b lab_1440
lab_3c0:
    cmpwi r4, 0xff
    beq lab_40c
    b lab_159c
lab_3cc:
    cmplwi r23, 0
    bne lab_3e4
    addi r3, r31, 0
    li r4, 0x153
    addi r5, r13, bc_s372
    bl __assert
lab_3e4:
    lfs f0, 4(r23)
    stfs f0, 0x24(r1)
    b lab_3fc
lab_3f0:
    mr r3, r23
    bl HSD_SListRemove
    mr r23, r3
lab_3fc:
    cmplwi r23, 0
    bne lab_3f0
    lfs f1, 0x24(r1)
    b lab_15bc
lab_40c:
    li r27, 1
    li r30, 0
    b lab_74
lab_418:
    li r27, 2
    li r30, 0
    b lab_74
lab_424:
    li r27, 4
    li r30, 0
    b lab_74
lab_430:
    cmplwi r23, 0
    bne lab_448
    addi r3, r31, 0
    li r4, 0x178
    addi r5, r13, bc_s372
    bl __assert
lab_448:
    lfs f0, 4(r23)
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    stw r0, 4(r23)
    b lab_74
lab_460:
    cmplwi r23, 0
    bne lab_478
    addi r3, r31, 0
    li r4, 0x17d
    addi r5, r13, bc_s372
    bl __assert
lab_478:
    lwz r0, 4(r23)
    xoris r0, r0, 0x8000
    stw r0, 0x34(r1)
    stw r26, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f26
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_4a0:
    cmplwi r23, 0
    bne lab_4b8
    addi r3, r31, 0
    li r4, 0x183
    addi r5, r13, bc_s372
    bl __assert
lab_4b8:
    lfs f0, 4(r23)
    fneg f0, f0
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_4d0:
    cmplwi r23, 0
    bne lab_4e8
    addi r3, r31, 0
    li r4, 0x189
    addi r5, r13, bc_s372
    bl __assert
lab_4e8:
    lwz r0, 4(r23)
    neg r0, r0
    stw r0, 4(r23)
    b lab_74
lab_4f8:
    cmplwi r23, 0
    bne lab_510
    addi r3, r31, 0
    li r4, 0x18f
    addi r5, r13, bc_s372
    bl __assert
lab_510:
    li r3, 2
    bl HSD_Randi
    stw r3, 4(r23)
    b lab_74
lab_520:
    cmplwi r23, 0
    bne lab_538
    addi r3, r31, 0
    li r4, 0x195
    addi r5, r13, bc_s372
    bl __assert
lab_538:
    bl HSD_Randf
    stfs f1, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_54c:
    cmplwi r23, 0
    bne lab_564
    addi r3, r31, 0
    li r4, 0x19b
    addi r5, r13, bc_s372
    bl __assert
lab_564:
    lfs f0, 4(r23)
    fmul f1, f29, f0
    frsp f1, f1
    bl sinf
    stfs f1, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_584:
    cmplwi r23, 0
    bne lab_59c
    addi r3, r31, 0
    li r4, 0x1a1
    addi r5, r13, bc_s372
    bl __assert
lab_59c:
    lfs f0, 4(r23)
    fmul f1, f29, f0
    frsp f1, f1
    bl cosf
    stfs f1, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_5bc:
    cmplwi r23, 0
    bne lab_5d4
    addi r3, r31, 0
    li r4, 0x1a7
    addi r5, r13, bc_s372
    bl __assert
lab_5d4:
    lfs f0, 4(r23)
    fmul f1, f29, f0
    frsp f1, f1
    bl tanf
    stfs f1, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_5f4:
    cmplwi r23, 0
    bne lab_60c
    addi r3, r31, 0
    li r4, 0x1ad
    addi r5, r13, bc_s372
    bl __assert
lab_60c:
    lfs f1, 4(r23)
    bl asinf
    fmul f0, f30, f1
    frsp f0, f0
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_62c:
    cmplwi r23, 0
    bne lab_644
    addi r3, r31, 0
    li r4, 0x1b3
    addi r5, r13, bc_s372
    bl __assert
lab_644:
    lfs f1, 4(r23)
    bl acosf
    fmul f0, f30, f1
    frsp f0, f0
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_664:
    cmplwi r23, 0
    bne lab_67c
    addi r3, r31, 0
    li r4, 0x1b9
    addi r5, r13, bc_s372
    bl __assert
lab_67c:
    lfs f1, 4(r23)
    bl atanf
    fmul f0, f30, f1
    frsp f0, f0
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_69c:
    cmplwi r23, 0
    bne lab_6b4
    addi r3, r31, 0
    li r4, 0x1bf
    addi r5, r13, bc_s372
    bl __assert
lab_6b4:
    lfs f1, 4(r23)
    bl logf
    stfs f1, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_6cc:
    cmplwi r23, 0
    bne lab_6e4
    addi r3, r31, 0
    li r4, 0x1c5
    addi r5, r13, bc_s372
    bl __assert
lab_6e4:
    lfs f1, 4(r23)
    bl expf
    stfs f1, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_6fc:
    cmplwi r23, 0
    bne lab_714
    addi r3, r31, 0
    li r4, 0x1cb
    addi r5, r13, bc_s372
    bl __assert
lab_714:
    lfs f0, 4(r23)
    fcmpo cr0, f0, f31
    bge lab_74
    fneg f0, f0
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_734:
    cmplwi r23, 0
    bne lab_74c
    addi r3, r31, 0
    li r4, 0x1d3
    addi r5, r13, bc_s372
    bl __assert
lab_74c:
    lwz r0, 4(r23)
    cmpwi r0, 0
    bge lab_74
    neg r0, r0
    stw r0, 4(r23)
    b lab_74
lab_764:
    cmplwi r23, 0
    bne lab_77c
    addi r3, r31, 0
    li r4, 0x1da
    addi r5, r13, bc_s372
    bl __assert
lab_77c:
    lfs f2, 4(r23)
    fcmpo cr0, f2, f31
    ble lab_7cc
    frsqrte f1, f2
    fmul f0, f1, f1
    fmul f1, f28, f1
    fnmsub f0, f2, f0, f27
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f28, f1
    fnmsub f0, f2, f0, f27
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f28, f1
    fnmsub f0, f2, f0, f27
    fmul f0, f1, f0
    fmul f0, f2, f0
    frsp f0, f0
    stfs f0, 0x20(r1)
    lfs f2, 0x20(r1)
lab_7cc:
    stfs f2, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_7dc:
    cmplwi r23, 0
    bne lab_7f4
    addi r3, r31, 0
    li r4, 0x1e0
    addi r5, r13, bc_s372
    bl __assert
lab_7f4:
    lwz r0, 4(r23)
    cntlzw r0, r0
    srwi r0, r0, 5
    stw r0, 4(r23)
    b lab_74
lab_808:
    cmplwi r23, 0
    bne lab_820
    addi r3, r31, 0
    li r4, 0x1f5
    addi r5, r13, bc_s372
    bl __assert
lab_820:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_83c
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x1f5
    bl __assert
lab_83c:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fadds f0, f1, f0
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r3)
    b lab_74
lab_86c:
    cmplwi r23, 0
    bne lab_884
    addi r3, r31, 0
    li r4, 0x1fb
    addi r5, r13, bc_s372
    bl __assert
lab_884:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_8a0
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x1fb
    bl __assert
lab_8a0:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fsubs f0, f1, f0
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r3)
    b lab_74
lab_8d0:
    cmplwi r23, 0
    bne lab_8e8
    addi r3, r31, 0
    li r4, 0x201
    addi r5, r13, bc_s372
    bl __assert
lab_8e8:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_904
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x201
    bl __assert
lab_904:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fmuls f0, f1, f0
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r3)
    b lab_74
lab_934:
    cmplwi r23, 0
    bne lab_94c
    addi r3, r31, 0
    li r4, 0x207
    addi r5, r13, bc_s372
    bl __assert
lab_94c:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_968
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x207
    bl __assert
lab_968:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fdivs f0, f1, f0
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r3)
    b lab_74
lab_998:
    cmplwi r23, 0
    bne lab_9b0
    addi r3, r31, 0
    li r4, 0x20d
    addi r5, r13, bc_s372
    bl __assert
lab_9b0:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_9cc
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x20d
    bl __assert
lab_9cc:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f25, 0x24(r1)
    mr r23, r3
    lfs f24, 4(r3)
    fabs f1, f25
    fabs f0, f24
    fcmpo cr0, f1, f0
    ble lab_9fc
    b lab_a0c
lab_9fc:
    fdivs f1, f24, f25
    bl __cvt_dbl_usll
    bl __cvt_sll_flt
    fnmsubs f24, f25, f1, f24
lab_a0c:
    stfs f24, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_a1c:
    cmplwi r23, 0
    bne lab_a34
    addi r3, r31, 0
    li r4, 0x213
    addi r5, r13, bc_s372
    bl __assert
lab_a34:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_a50
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x213
    bl __assert
lab_a50:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    add r0, r0, r21
    stw r0, 4(r3)
    b lab_74
lab_a70:
    cmplwi r23, 0
    bne lab_a88
    addi r3, r31, 0
    li r4, 0x218
    addi r5, r13, bc_s372
    bl __assert
lab_a88:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_aa4
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x218
    bl __assert
lab_aa4:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    subf r0, r21, r0
    stw r0, 4(r3)
    b lab_74
lab_ac4:
    cmplwi r23, 0
    bne lab_adc
    addi r3, r31, 0
    li r4, 0x21d
    addi r5, r13, bc_s372
    bl __assert
lab_adc:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_af8
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x21d
    bl __assert
lab_af8:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    mullw r0, r0, r21
    stw r0, 4(r3)
    b lab_74
lab_b18:
    cmplwi r23, 0
    bne lab_b30
    addi r3, r31, 0
    li r4, 0x222
    addi r5, r13, bc_s372
    bl __assert
lab_b30:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_b4c
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x222
    bl __assert
lab_b4c:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    divw r0, r0, r21
    stw r0, 4(r3)
    b lab_74
lab_b6c:
    cmplwi r23, 0
    bne lab_b84
    addi r3, r31, 0
    li r4, 0x227
    addi r5, r13, bc_s372
    bl __assert
lab_b84:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_ba0
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x227
    bl __assert
lab_ba0:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    mr r23, r3
    lwz r3, 4(r3)
    divw r0, r3, r21
    mullw r0, r0, r21
    subf r0, r0, r3
    stw r0, 4(r23)
    b lab_74
lab_bc8:
    cmplwi r23, 0
    bne lab_be0
    addi r3, r31, 0
    li r4, 0x22c
    addi r5, r13, bc_s372
    bl __assert
lab_be0:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_bfc
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x22c
    bl __assert
lab_bfc:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f2, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    bl powf
    stfs f1, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_c2c:
    cmplwi r23, 0
    bne lab_c44
    addi r3, r31, 0
    li r4, 0x232
    addi r5, r13, bc_s372
    bl __assert
lab_c44:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_c60
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x232
    bl __assert
lab_c60:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fcmpo cr0, f1, f0
    ble lab_74
    lwz r0, 0x24(r1)
    stw r0, 4(r23)
    b lab_74
lab_c90:
    cmplwi r23, 0
    bne lab_ca8
    addi r3, r31, 0
    li r4, 0x239
    addi r5, r13, bc_s372
    bl __assert
lab_ca8:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_cc4
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x239
    bl __assert
lab_cc4:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fcmpo cr0, f1, f0
    bge lab_74
    lwz r0, 0x24(r1)
    stw r0, 4(r23)
    b lab_74
lab_cf4:
    cmplwi r23, 0
    bne lab_d0c
    addi r3, r31, 0
    li r4, 0x240
    addi r5, r13, bc_s372
    bl __assert
lab_d0c:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_d28
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x240
    bl __assert
lab_d28:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    cmpw r0, r21
    ble lab_74
    stw r21, 4(r23)
    b lab_74
lab_d4c:
    cmplwi r23, 0
    bne lab_d64
    addi r3, r31, 0
    li r4, 0x247
    addi r5, r13, bc_s372
    bl __assert
lab_d64:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_d80
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x247
    bl __assert
lab_d80:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    cmpw r0, r21
    bge lab_74
    stw r21, 4(r23)
    b lab_74
lab_da4:
    cmplwi r23, 0
    bne lab_dbc
    addi r3, r31, 0
    li r4, 0x24e
    addi r5, r13, bc_s372
    bl __assert
lab_dbc:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_dd8
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x24e
    bl __assert
lab_dd8:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f2, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fcmpu cr0, f31, f2
    bne lab_e18
    fcmpo cr0, f1, f31
    cror 2, 1, 2
    bne lab_e10
    lfs f1, bc_magic-0x8(r2)
    b lab_e1c
lab_e10:
    lfs f1, bc_mpi2(r2)
    b lab_e1c
lab_e18:
    bl atan2f
lab_e1c:
    fmul f0, f30, f1
    frsp f0, f0
    stfs f0, 0x28(r1)
    lwz r0, 0x28(r1)
    stw r0, 4(r23)
    b lab_74
lab_e34:
    cmplwi r23, 0
    bne lab_e4c
    addi r3, r31, 0
    li r4, 0x254
    addi r5, r13, bc_s372
    bl __assert
lab_e4c:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_e68
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x254
    bl __assert
lab_e68:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fcmpo cr0, f1, f0
    mfcr r0
    srwi r0, r0, 0x1f
    stw r0, 4(r3)
    b lab_74
lab_e98:
    cmplwi r23, 0
    bne lab_eb0
    addi r3, r31, 0
    li r4, 0x259
    addi r5, r13, bc_s372
    bl __assert
lab_eb0:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_ecc
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x259
    bl __assert
lab_ecc:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fcmpo cr0, f1, f0
    mfcr r0
    rlwinm r0, r0, 2, 0x1f, 0x1f
    stw r0, 4(r3)
    b lab_74
lab_efc:
    cmplwi r23, 0
    bne lab_f14
    addi r3, r31, 0
    li r4, 0x25e
    addi r5, r13, bc_s372
    bl __assert
lab_f14:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_f30
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x25e
    bl __assert
lab_f30:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fcmpo cr0, f1, f0
    cror 2, 0, 2
    mfcr r0
    rlwinm r0, r0, 3, 0x1f, 0x1f
    stw r0, 4(r3)
    b lab_74
lab_f64:
    cmplwi r23, 0
    bne lab_f7c
    addi r3, r31, 0
    li r4, 0x263
    addi r5, r13, bc_s372
    bl __assert
lab_f7c:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_f98
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x263
    bl __assert
lab_f98:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fcmpo cr0, f1, f0
    cror 2, 1, 2
    mfcr r0
    rlwinm r0, r0, 3, 0x1f, 0x1f
    stw r0, 4(r3)
    b lab_74
lab_fcc:
    cmplwi r23, 0
    bne lab_fe4
    addi r3, r31, 0
    li r4, 0x268
    addi r5, r13, bc_s372
    bl __assert
lab_fe4:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_1000
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x268
    bl __assert
lab_1000:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fcmpu cr0, f1, f0
    mfcr r0
    rlwinm r0, r0, 3, 0x1f, 0x1f
    stw r0, 4(r3)
    b lab_74
lab_1030:
    cmplwi r23, 0
    bne lab_1048
    addi r3, r31, 0
    li r4, 0x26d
    addi r5, r13, bc_s372
    bl __assert
lab_1048:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_1064
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x26d
    bl __assert
lab_1064:
    lfs f0, 4(r23)
    mr r3, r23
    stfs f0, 0x24(r1)
    bl HSD_SListRemove
    lfs f0, 0x24(r1)
    mr r23, r3
    lfs f1, 4(r3)
    fcmpu cr0, f1, f0
    mfcr r0
    rlwinm r0, r0, 3, 0x1f, 0x1f
    xori r0, r0, 1
    stw r0, 4(r3)
    b lab_74
lab_1098:
    cmplwi r23, 0
    bne lab_10b0
    addi r3, r31, 0
    li r4, 0x272
    addi r5, r13, bc_s372
    bl __assert
lab_10b0:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_10cc
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x272
    bl __assert
lab_10cc:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    mr r23, r3
    lwz r3, 4(r3)
    eqv r0, r21, r3
    subfc r3, r21, r3
    srwi r0, r0, 0x1f
    addze r0, r0
    clrlwi r0, r0, 0x1f
    stw r0, 4(r23)
    b lab_74
lab_10fc:
    cmplwi r23, 0
    bne lab_1114
    addi r3, r31, 0
    li r4, 0x277
    addi r5, r13, bc_s372
    bl __assert
lab_1114:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_1130
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x277
    bl __assert
lab_1130:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    mr r23, r3
    lwz r3, 4(r3)
    eqv r0, r3, r21
    subfc r3, r3, r21
    srwi r0, r0, 0x1f
    addze r0, r0
    clrlwi r0, r0, 0x1f
    stw r0, 4(r23)
    b lab_74
lab_1160:
    cmplwi r23, 0
    bne lab_1178
    addi r3, r31, 0
    li r4, 0x27c
    addi r5, r13, bc_s372
    bl __assert
lab_1178:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_1194
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x27c
    bl __assert
lab_1194:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    srawi r3, r21, 0x1f
    srwi r4, r0, 0x1f
    subfc r0, r0, r21
    adde r0, r3, r4
    stw r0, 4(r23)
    b lab_74
lab_11c0:
    cmplwi r23, 0
    bne lab_11d8
    addi r3, r31, 0
    li r4, 0x281
    addi r5, r13, bc_s372
    bl __assert
lab_11d8:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_11f4
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x281
    bl __assert
lab_11f4:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    srwi r4, r21, 0x1f
    srawi r3, r0, 0x1f
    subfc r0, r21, r0
    adde r0, r3, r4
    stw r0, 4(r23)
    b lab_74
lab_1220:
    cmplwi r23, 0
    bne lab_1238
    addi r3, r31, 0
    li r4, 0x286
    addi r5, r13, bc_s372
    bl __assert
lab_1238:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_1254
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x286
    bl __assert
lab_1254:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    subf r0, r0, r21
    cntlzw r0, r0
    srwi r0, r0, 5
    stw r0, 4(r3)
    b lab_74
lab_127c:
    cmplwi r23, 0
    bne lab_1294
    addi r3, r31, 0
    li r4, 0x28b
    addi r5, r13, bc_s372
    bl __assert
lab_1294:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_12b0
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x28b
    bl __assert
lab_12b0:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    subf r3, r0, r21
    addic r0, r3, -1
    subfe r0, r0, r3
    stw r0, 4(r23)
    b lab_74
lab_12d8:
    cmplwi r23, 0
    bne lab_12f0
    addi r3, r31, 0
    li r4, 0x290
    addi r5, r13, bc_s372
    bl __assert
lab_12f0:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_130c
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x290
    bl __assert
lab_130c:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    li r3, 0
    cmpwi r0, 0
    beq lab_1338
    cmpwi r21, 0
    beq lab_1338
    li r3, 1
lab_1338:
    stw r3, 4(r23)
    b lab_74
lab_1340:
    cmplwi r23, 0
    bne lab_1358
    addi r3, r31, 0
    li r4, 0x295
    addi r5, r13, bc_s372
    bl __assert
lab_1358:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_1374
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x295
    bl __assert
lab_1374:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    li r3, 1
    cmpwi r0, 0
    bne lab_13a0
    cmpwi r21, 0
    bne lab_13a0
    li r3, 0
lab_13a0:
    stw r3, 4(r23)
    b lab_74
lab_13a8:
    cmplwi r23, 0
    bne lab_13c0
    addi r3, r31, 0
    li r4, 0x29a
    addi r5, r13, bc_s372
    bl __assert
lab_13c0:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_13dc
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x29a
    bl __assert
lab_13dc:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r4, 4(r3)
    mr r23, r3
    li r0, 1
    cmpwi r4, 0
    li r3, 0
    bne lab_140c
    cmpwi r21, 0
    beq lab_140c
    mr r3, r0
lab_140c:
    cmpwi r3, 0
    bne lab_1438
    cmpwi r4, 0
    li r3, 0
    beq lab_142c
    cmpwi r21, 0
    bne lab_142c
    li r3, 1
lab_142c:
    cmpwi r3, 0
    bne lab_1438
    li r0, 0
lab_1438:
    stw r0, 4(r23)
    b lab_74
lab_1440:
    cmplwi r23, 0
    bne lab_1458
    addi r3, r31, 0
    li r4, 0x29f
    addi r5, r13, bc_s372
    bl __assert
lab_1458:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_1474
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x29f
    bl __assert
lab_1474:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    and r0, r0, r21
    stw r0, 4(r3)
    b lab_74
lab_1494:
    cmplwi r23, 0
    bne lab_14ac
    addi r3, r31, 0
    li r4, 0x2a4
    addi r5, r13, bc_s372
    bl __assert
lab_14ac:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_14c8
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x2a4
    bl __assert
lab_14c8:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    or r0, r0, r21
    stw r0, 4(r3)
    b lab_74
lab_14e8:
    cmplwi r23, 0
    bne lab_1500
    addi r3, r31, 0
    li r4, 0x2a9
    addi r5, r13, bc_s372
    bl __assert
lab_1500:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_151c
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x2a9
    bl __assert
lab_151c:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r0, 4(r3)
    mr r23, r3
    xor r0, r0, r21
    stw r0, 4(r3)
    b lab_74
lab_153c:
    cmplwi r23, 0
    bne lab_1554
    addi r3, r31, 0
    li r4, 0x2af
    addi r5, r13, bc_s372
    bl __assert
lab_1554:
    lwz r0, 0(r23)
    cmplwi r0, 0
    bne lab_1570
    addi r3, r31, 0
    addi r5, r31, 0x78
    li r4, 0x2af
    bl __assert
lab_1570:
    lwz r21, 4(r23)
    mr r3, r23
    bl HSD_SListRemove
    lwz r22, 4(r3)
    mr r23, r3
    subf r3, r22, r21
    addi r3, r3, 1
    bl HSD_Randi
    add r0, r22, r3
    stw r0, 4(r23)
    b lab_74
lab_159c:
    crxor 6, 6, 6
    addi r3, r31, 0x84
    bl OSReport
    addi r3, r31, 0
    li r4, 0x2b5
    addi r5, r13, bc_s371
    bl HSD_Panic
    b lab_74
lab_15bc:
    lmw r21, 0x3c(r1)
    lwz r0, 0xac(r1)
    lfd f31, 0xa0(r1)
    lfd f30, 0x98(r1)
    mtspr 8, r0
    lfd f29, 0x90(r1)
    lfd f28, 0x88(r1)
    lfd f27, 0x80(r1)
    lfd f26, 0x78(r1)
    lfd f25, 0x70(r1)
    lfd f24, 0x68(r1)
    addi r1, r1, 0xa8
    blr
}
