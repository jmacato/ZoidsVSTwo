void TRKInterruptHandlerEnableInterrupts(void);
void TRKRestoreExtended1Block(void);

extern char gTRKState[];
extern char gTRKExceptionStatus[];
extern char gTRKCPUState[];

asm void TRKSwapAndGo(void)
{
    nofralloc
    lis r3, gTRKState@h
    ori r3, r3, gTRKState@l
    stmw r0, 0(r3)
    mfmsr r0
    stw r0, 0x8c(r3)
    mfspr r0, 8
    stw r0, 0x80(r3)
    mfspr r0, 9
    stw r0, 0x84(r3)
    mfspr r0, 1
    stw r0, 0x88(r3)
    mfspr r0, 18
    stw r0, 0x94(r3)
    mfspr r0, 19
    stw r0, 0x90(r3)
    li r1, -0x7ffe
    nor r1, r1, r1
    mfmsr r3
    and r3, r3, r1
    mtmsr r3
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    lwz r2, 0xa0(r2)
    lbz r2, 0(r2)
    cmpwi r2, 0
    beq lab_7c
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    li r3, 1
    stb r3, 0x9c(r2)
    b TRKInterruptHandlerEnableInterrupts
lab_7c:
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    li r3, 0
    stb r3, 0xc(r2)
    bl TRKRestoreExtended1Block
    lis r2, gTRKCPUState@h
    ori r2, r2, gTRKCPUState@l
    lmw r27, 0x80(r2)
    mtspr 26, r27
    mtspr 8, r28
    mtcrf 0xff, r29
    mtspr 9, r30
    mtspr 1, r31
    lmw r3, 0xc(r2)
    lwz r0, 0(r2)
    lwz r1, 4(r2)
    lwz r2, 8(r2)
    rfi
}
