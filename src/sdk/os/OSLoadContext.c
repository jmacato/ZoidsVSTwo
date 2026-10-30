void __RAS_OSDisableInterrupts_begin(void);
void __RAS_OSDisableInterrupts_end(void);

asm void OSLoadContext(void)
{
    nofralloc
    lis r4, __RAS_OSDisableInterrupts_begin@ha
    lwz r6, 0x198(r3)
    addi r5, r4, __RAS_OSDisableInterrupts_begin@l
    cmplw r6, r5
    blt lab_28
    lis r4, __RAS_OSDisableInterrupts_end@ha
    addi r0, r4, __RAS_OSDisableInterrupts_end@l
    cmplw r6, r0
    bgt lab_28
    stw r5, 0x198(r3)
lab_28:
    lwz r0, 0(r3)
    lwz r1, 4(r3)
    lwz r2, 8(r3)
    lhz r4, 0x1a2(r3)
    rlwinm. r5, r4, 0, 0x1e, 0x1e
    beq lab_50
    rlwinm r4, r4, 0, 0x1f, 0x1d
    sth r4, 0x1a2(r3)
    lmw r5, 0x14(r3)
    b lab_54
lab_50:
    lmw r13, 0x34(r3)
lab_54:
    lwz r4, 0x1a8(r3)
    mtspr 913, r4
    lwz r4, 0x1ac(r3)
    mtspr 914, r4
    lwz r4, 0x1b0(r3)
    mtspr 915, r4
    lwz r4, 0x1b4(r3)
    mtspr 916, r4
    lwz r4, 0x1b8(r3)
    mtspr 917, r4
    lwz r4, 0x1bc(r3)
    mtspr 918, r4
    lwz r4, 0x1c0(r3)
    mtspr 919, r4
    lwz r4, 0x80(r3)
    mtcrf 0xff, r4
    lwz r4, 0x84(r3)
    mtspr 8, r4
    lwz r4, 0x88(r3)
    mtspr 9, r4
    lwz r4, 0x8c(r3)
    mtspr 1, r4
    mfmsr r4
    rlwinm r4, r4, 0, 0x11, 0xf
    rlwinm r4, r4, 0, 0x1f, 0x1d
    mtmsr r4
    lwz r4, 0x198(r3)
    mtspr 26, r4
    lwz r4, 0x19c(r3)
    mtspr 27, r4
    lwz r4, 0x10(r3)
    lwz r3, 0xc(r3)
    rfi
}
