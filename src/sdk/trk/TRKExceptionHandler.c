extern char gTRKExceptionStatus[];

asm void TRKExceptionHandler(void)
{
    nofralloc
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    sth r3, 8(r2)
    mfspr r3, 26
    stw r3, 0(r2)
    lhz r3, 8(r2)
    cmpwi r3, 0x200
    beq lab_6c
    cmpwi r3, 0x300
    beq lab_6c
    cmpwi r3, 0x400
    beq lab_6c
    cmpwi r3, 0x600
    beq lab_6c
    cmpwi r3, 0x700
    beq lab_6c
    cmpwi r3, 0x800
    beq lab_6c
    cmpwi r3, 0x1000
    beq lab_6c
    cmpwi r3, 0x1100
    beq lab_6c
    cmpwi r3, 0x1200
    beq lab_6c
    cmpwi r3, 0x1300
    beq lab_6c
    b lab_78
lab_6c:
    mfspr r3, 26
    addi r3, r3, 4
    mtspr 26, r3
lab_78:
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    li r3, 1
    stb r3, 0xd(r2)
    mfspr r3, 275
    mtcrf 0xff, r3
    mfspr r2, 273
    mfspr r3, 274
    rfi
}
