void TRKPostInterruptEvent(void);
void TRKSaveExtended1Block(void);
void TRKUARTInterruptHandler(void);

extern char gTRKState[];
extern char TRK_saved_exceptionID[];
extern char gTRKCPUState[];
extern char gTRKExceptionStatus[];
extern char gTRKSaveState[];

asm void TRKInterruptHandler(void)
{
    nofralloc
    mtspr 26, r2
    mtspr 27, r4
    mfspr r4, 275
    mfcr r2
    mtspr 275, r2
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    lwz r2, 0x8c(r2)
    ori r2, r2, 0x8002
    xori r2, r2, 0x8002
    sync
    mtmsr r2
    sync
    lis r2, TRK_saved_exceptionID@h
    ori r2, r2, TRK_saved_exceptionID@l
    sth r3, 0(r2)
    cmpwi r3, 0x500
    bne lab_c8
    lis r2, gTRKCPUState@h
    ori r2, r2, gTRKCPUState@l
    mfspr r3, 8
    stw r3, 0x42c(r2)
    bl TRKUARTInterruptHandler
    lis r2, gTRKCPUState@h
    ori r2, r2, gTRKCPUState@l
    lwz r3, 0x42c(r2)
    mtspr 8, r3
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    lwz r2, 0xa0(r2)
    lbz r2, 0(r2)
    cmpwi r2, 0
    beq lab_ac
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    lbz r2, 0xc(r2)
    cmpwi r2, 1
    beq lab_ac
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    li r3, 1
    stb r3, 0x9c(r2)
    b lab_c8
lab_ac:
    lis r2, gTRKSaveState@h
    ori r2, r2, gTRKSaveState@l
    lwz r3, 0x88(r2)
    mtcrf 0xff, r3
    lwz r3, 0xc(r2)
    lwz r2, 8(r2)
    rfi
lab_c8:
    lis r2, TRK_saved_exceptionID@h
    ori r2, r2, TRK_saved_exceptionID@l
    lhz r3, 0(r2)
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    lbz r2, 0xc(r2)
    cmpwi r2, 0
    bne *+0xb0
    lis r2, gTRKCPUState@h
    ori r2, r2, gTRKCPUState@l
    stw r0, 0(r2)
    stw r1, 4(r2)
    mfspr r0, 273
    stw r0, 8(r2)
    sth r3, 0x2f8(r2)
    sth r3, 0x2fa(r2)
    mfspr r0, 274
    stw r0, 0xc(r2)
    stmw r4, 0x10(r2)
    mfspr r27, 26
    mfspr r28, 8
    mfspr r29, 275
    mfspr r30, 9
    mfspr r31, 1
    stmw r27, 0x80(r2)
    bl TRKSaveExtended1Block
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    li r3, 1
    stb r3, 0xc(r2)
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
