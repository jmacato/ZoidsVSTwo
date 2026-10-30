void ICFlashInvalidate(void);
void OSDisableInterrupts(void);

asm void Run(void)
{
    nofralloc
    mfspr r0, 8
    stw r0, 4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr r31, r3
    bl OSDisableInterrupts
    bl ICFlashInvalidate
    sync
    isync
    mtspr 8, r31
    blr
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtspr 8, r0
    blr
}
