void InitMetroTRKCommTable(void);
void TRKSaveExtended1Block(void);
void TRK_main(void);

extern char gTRKCPUState[];
extern char _db_stack_addr[];

asm void InitMetroTRK(void)
{
    nofralloc
    addi r1, r1, -4
    stw r3, 0(r1)
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    stmw r0, 0(r3)
    lwz r4, 0(r1)
    addi r1, r1, 4
    stw r1, 4(r3)
    stw r4, 0xc(r3)
    mfspr r4, 8
    stw r4, 0x84(r3)
    stw r4, 0x80(r3)
    mfcr r4
    stw r4, 0x88(r3)
    mfmsr r4
    ori r3, r4, 0x8000
    xori r3, r3, 0x8000
    mtmsr r3
    mtspr 27, r4
    bl TRKSaveExtended1Block
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    lmw r0, 0(r3)
    li r0, 0
    mtspr 1010, r0
    mtspr 1013, r0
    lis r1, _db_stack_addr@h
    ori r1, r1, _db_stack_addr@l
    mr r3, r5
    bl InitMetroTRKCommTable
    cmpwi r3, 1
    bne lab_90
    lwz r4, 0x84(r3)
    mtspr 8, r4
    lmw r0, 0(r3)
    blr
lab_90:
    b TRK_main
}
