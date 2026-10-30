extern char gTRKCPUState[];
extern char gTRKRestoreFlags[];

asm void TRKRestoreExtended1Block(void)
{
    nofralloc
    lis r2, gTRKCPUState@h
    ori r2, r2, gTRKCPUState@l
    lis r5, gTRKRestoreFlags@h
    ori r5, r5, gTRKRestoreFlags@l
    lbz r3, 0(r5)
    lbz r6, 1(r5)
    li r0, 0
    stb r0, 0(r5)
    stb r0, 1(r5)
    cmpwi r3, 0
    beq lab_3c
    lwz r24, 0x1e8(r2)
    lwz r25, 0x1ec(r2)
    mtspr 284, r24
    mtspr 285, r25
lab_3c:
    lmw r20, 0x2fc(r2)
    mtspr 912, r20
    mtspr 913, r21
    mtspr 914, r22
    mtspr 915, r23
    mtspr 916, r24
    mtspr 917, r25
    mtspr 918, r26
    mtspr 919, r27
    mtspr 920, r28
    mtspr 922, r30
    mtspr 923, r31
    b lab_88
    lmw r26, 0x2e0(r2)
    mtspr 944, r26
    mtspr 951, r27
    mtspr 1014, r29
    mtspr 1015, r30
    mtspr 1023, r31
lab_88:
    lmw r19, 0x284(r2)
    mtspr 1013, r19
    mtspr 953, r20
    mtspr 954, r21
    mtspr 957, r22
    mtspr 958, r23
    mtspr 955, r24
    mtspr 952, r25
    mtspr 956, r26
    mtspr 1020, r27
    mtspr 1021, r28
    mtspr 1022, r29
    mtspr 1019, r30
    mtspr 1017, r31
    b lab_f4
    cmpwi r6, 0
    beq lab_d4
    lwz r26, 0x278(r2)
    mtspr 22, r26
lab_d4:
    lmw r25, 0x240(r2)
    mtspr 976, r25
    mtspr 977, r26
    mtspr 978, r27
    mtspr 979, r28
    mtspr 980, r29
    mtspr 981, r30
    mtspr 982, r31
lab_f4:
    lmw r16, 0x1a8(r2)
    mtsr 0, r16
    mtsr 1, r17
    mtsr 2, r18
    mtsr 3, r19
    mtsr 4, r20
    mtsr 5, r21
    mtsr 6, r22
    mtsr 7, r23
    mtsr 8, r24
    mtsr 9, r25
    mtsr 0xa, r26
    mtsr 0xb, r27
    mtsr 0xc, r28
    mtsr 0xd, r29
    mtsr 0xe, r30
    mtsr 0xf, r31
    lmw r12, 0x1f0(r2)
    mtspr 1008, r12
    mtspr 1009, r13
    mtspr 27, r14
    mtspr 287, r15
    mtspr 528, r16
    mtspr 529, r17
    mtspr 530, r18
    mtspr 531, r19
    mtspr 532, r20
    mtspr 533, r21
    mtspr 534, r22
    mtspr 535, r23
    mtspr 536, r24
    mtspr 537, r25
    mtspr 538, r26
    mtspr 539, r27
    mtspr 540, r28
    mtspr 541, r29
    mtspr 542, r30
    mtspr 543, r31
    lmw r22, 0x25c(r2)
    mtspr 25, r22
    mtspr 19, r23
    mtspr 18, r24
    mtspr 272, r25
    mtspr 273, r26
    mtspr 274, r27
    mtspr 275, r28
    mtspr 1010, r30
    mtspr 282, r31
    blr
}
