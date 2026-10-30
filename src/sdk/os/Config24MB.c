asm void Config24MB(void)
{
    nofralloc
    li r7, 0
    lis r4, 0
    addi r4, r4, 2
    lis r3, -0x8000
    addi r3, r3, 0x1ff
    lis r6, 0x100
    addi r6, r6, 2
    lis r5, -0x7f00
    addi r5, r5, 0xff
    isync
    mtspr 536, r7
    mtspr 537, r4
    mtspr 536, r3
    isync
    mtspr 528, r7
    mtspr 529, r4
    mtspr 528, r3
    isync
    mtspr 540, r7
    mtspr 541, r6
    mtspr 540, r5
    isync
    mtspr 532, r7
    mtspr 533, r6
    mtspr 532, r5
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtspr 27, r3
    mfspr r3, 8
    mtspr 26, r3
    rfi
}
