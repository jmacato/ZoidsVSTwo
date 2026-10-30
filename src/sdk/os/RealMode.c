asm void RealMode(void)
{
    nofralloc
    clrlwi r3, r3, 2
    mtspr 26, r3
    mfmsr r3
    rlwinm r3, r3, 0, 0x1c, 0x19
    mtspr 27, r3
    rfi
}
