void OSDefaultExceptionHandler(void);

asm void OSExceptionVector(void)
{
    nofralloc
    mtspr 272, r4
    lwz r4, 0xc0(r0)
    stw r3, 0xc(r4)
    mfspr r3, 272
    stw r3, 0x10(r4)
    stw r5, 0x14(r4)
    lhz r3, 0x1a2(r4)
    ori r3, r3, 2
    sth r3, 0x1a2(r4)
    mfcr r3
    stw r3, 0x80(r4)
    mfspr r3, 8
    stw r3, 0x84(r4)
    mfspr r3, 9
    stw r3, 0x88(r4)
    mfspr r3, 1
    stw r3, 0x8c(r4)
    mfspr r3, 26
    stw r3, 0x198(r4)
    mfspr r3, 27
    stw r3, 0x19c(r4)
    mr r5, r3
    nop
    mfmsr r3
    ori r3, r3, 0x30
    mtspr 27, r3
    li r3, 0
    lwz r4, 0xd4(r0)
    rlwinm. r5, r5, 0, 0x1e, 0x1e
    bne lab_88
    lis r5, OSDefaultExceptionHandler@ha
    addi r5, r5, OSDefaultExceptionHandler@l
    mtspr 26, r5
    rfi
lab_88:
    rlwinm r5, r3, 2, 0x16, 0x1d
    lwz r5, 0x3000(r5)
    mtspr 26, r5
    rfi
    nop
}
