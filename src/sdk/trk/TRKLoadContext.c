void TRKInterruptHandler(void);

asm void TRKLoadContext(void)
{
    nofralloc
    lwz r0, 0(r3)
    lwz r1, 4(r3)
    lwz r2, 8(r3)
    lhz r5, 0x1a2(r3)
    rlwinm. r6, r5, 0, 0x1e, 0x1e
    beq lab_28
    rlwinm r5, r5, 0, 0x1f, 0x1d
    sth r5, 0x1a2(r3)
    lmw r5, 0x14(r3)
    b lab_2c
lab_28:
    lmw r13, 0x34(r3)
lab_2c:
    mr r31, r3
    mr r3, r4
    lwz r4, 0x80(r31)
    mtcrf 0xff, r4
    lwz r4, 0x84(r31)
    mtspr 8, r4
    lwz r4, 0x88(r31)
    mtspr 9, r4
    lwz r4, 0x8c(r31)
    mtspr 1, r4
    mfmsr r4
    rlwinm r4, r4, 0, 0x11, 0xf
    rlwinm r4, r4, 0, 0x1f, 0x1d
    mtmsr r4
    mtspr 273, r2
    lwz r4, 0xc(r31)
    mtspr 274, r4
    lwz r4, 0x10(r31)
    mtspr 275, r4
    lwz r2, 0x198(r31)
    lwz r4, 0x19c(r31)
    lwz r31, 0x7c(r31)
    b TRKInterruptHandler
}
