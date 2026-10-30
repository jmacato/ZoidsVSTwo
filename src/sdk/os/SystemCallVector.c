asm void SystemCallVector(void)
{
    nofralloc
    mfspr r9, 1008
    ori r10, r9, 8
    mtspr 1008, r10
    isync
    sync
    mtspr 1008, r9
    rfi
    nop
}
