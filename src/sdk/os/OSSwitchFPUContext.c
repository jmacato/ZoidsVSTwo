void __OSLoadFPUContext(void);
void __OSSaveFPUContext(void);

asm void OSSwitchFPUContext(void)
{
    nofralloc
    mfmsr r5
    ori r5, r5, 0x2000
    mtmsr r5
    isync
    lwz r5, 0x19c(r4)
    ori r5, r5, 0x2000
    mtspr 27, r5
    lis r3, -0x8000
    lwz r5, 0xd8(r3)
    stw r4, 0xd8(r3)
    cmpw r5, r4
    beq lab_40
    cmpwi r5, 0
    beq lab_3c
    bl __OSSaveFPUContext
lab_3c:
    bl __OSLoadFPUContext
lab_40:
    lwz r3, 0x80(r4)
    mtcrf 0xff, r3
    lwz r3, 0x84(r4)
    mtspr 8, r3
    lwz r3, 0x198(r4)
    mtspr 26, r3
    lwz r3, 0x88(r4)
    mtspr 9, r3
    lwz r3, 0x8c(r4)
    mtspr 1, r3
    lhz r3, 0x1a2(r4)
    rlwinm r3, r3, 0, 0x1f, 0x1d
    sth r3, 0x1a2(r4)
    lwz r5, 0x14(r4)
    lwz r3, 0xc(r4)
    lwz r4, 0x10(r4)
    rfi
}
