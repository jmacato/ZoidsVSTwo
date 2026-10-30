void TRKPostInterruptEvent(void);

extern char gTRKState[];

asm void TRKInterruptHandlerEnableInterrupts(void)
{
    nofralloc
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    lwz r0, 0x8c(r2)
    sync
    mtmsr r0
    sync
    lwz r0, 0x80(r2)
    mtspr 8, r0
    lwz r0, 0x84(r2)
    mtspr 9, r0
    lwz r0, 0x88(r2)
    mtspr 1, r0
    lwz r0, 0x94(r2)
    mtspr 18, r0
    lwz r0, 0x90(r2)
    mtspr 19, r0
    lmw r3, 0xc(r2)
    lwz r0, 0(r2)
    lwz r1, 4(r2)
    lwz r2, 8(r2)
    b TRKPostInterruptEvent
}
