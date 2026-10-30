void OSFillFPUContext(void);

extern char debug_context[];

asm void HSD_SaveContext(void)
{
    nofralloc
    mtspr 272, r3
    lis r3, debug_context@ha
    addi r3, r3, debug_context@l
    stmw r0, 0(r3)
    mfspr r4, 272
    stw r4, 0xc(r3)
    mfspr r4, 912
    stw r4, 0x1a4(r3)
    mfspr r4, 913
    stw r4, 0x1a8(r3)
    mfspr r4, 914
    stw r4, 0x1ac(r3)
    mfspr r4, 915
    stw r4, 0x1b0(r3)
    mfspr r4, 916
    stw r4, 0x1b4(r3)
    mfspr r4, 917
    stw r4, 0x1b8(r3)
    mfspr r4, 918
    stw r4, 0x1bc(r3)
    mfspr r4, 919
    stw r4, 0x1c0(r3)
    mfcr r4
    stw r4, 0x80(r3)
    mfspr r4, 8
    stw r4, 0x84(r3)
    mfspr r4, 9
    stw r4, 0x88(r3)
    mfspr r4, 1
    stw r4, 0x8c(r3)
    mfspr r4, 26
    stw r4, 0x198(r3)
    mfspr r4, 27
    stw r4, 0x19c(r3)
    lhz r4, 0x1a2(r3)
    ori r4, r4, 1
    sth r4, 0x1a2(r3)
    b OSFillFPUContext
}
