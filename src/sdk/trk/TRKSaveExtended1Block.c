extern char gTRKCPUState[];

asm void TRKSaveExtended1Block(void)
{
    nofralloc
    lis r2, gTRKCPUState@h
    ori r2, r2, gTRKCPUState@l
    mfsr r16, 0
    mfsr r17, 1
    mfsr r18, 2
    mfsr r19, 3
    mfsr r20, 4
    mfsr r21, 5
    mfsr r22, 6
    mfsr r23, 7
    mfsr r24, 8
    mfsr r25, 9
    mfsr r26, 0xa
    mfsr r27, 0xb
    mfsr r28, 0xc
    mfsr r29, 0xd
    mfsr r30, 0xe
    mfsr r31, 0xf
    stmw r16, 0x1a8(r2)
    mftb r10, 0x10c
    mftbu r11
    mfspr r12, 1008
    mfspr r13, 1009
    mfspr r14, 27
    mfspr r15, 287
    mfspr r16, 528
    mfspr r17, 529
    mfspr r18, 530
    mfspr r19, 531
    mfspr r20, 532
    mfspr r21, 533
    mfspr r22, 534
    mfspr r23, 535
    mfspr r24, 536
    mfspr r25, 537
    mfspr r26, 538
    mfspr r27, 539
    mfspr r28, 540
    mfspr r29, 541
    mfspr r30, 542
    mfspr r31, 543
    stmw r10, 0x1e8(r2)
    mfspr r22, 25
    mfspr r23, 19
    mfspr r24, 18
    mfspr r25, 272
    mfspr r26, 273
    mfspr r27, 274
    mfspr r28, 275
    li r29, 0
    mfspr r30, 1010
    mfspr r31, 282
    stmw r22, 0x25c(r2)
    mfspr r20, 912
    mfspr r21, 913
    mfspr r22, 914
    mfspr r23, 915
    mfspr r24, 916
    mfspr r25, 917
    mfspr r26, 918
    mfspr r27, 919
    mfspr r28, 920
    mfspr r29, 921
    mfspr r30, 922
    mfspr r31, 923
    stmw r20, 0x2fc(r2)
    b lab_150
    mfspr r16, 928
    mfspr r17, 935
    mfspr r18, 936
    mfspr r19, 937
    mfspr r20, 938
    mfspr r21, 939
    mfspr r22, 940
    mfspr r23, 941
    mfspr r24, 942
    mfspr r25, 943
    mfspr r26, 944
    mfspr r27, 951
    mfspr r28, 959
    mfspr r29, 1014
    mfspr r30, 1015
    mfspr r31, 1023
    stmw r16, 0x2b8(r2)
lab_150:
    mfspr r19, 1013
    mfspr r20, 953
    mfspr r21, 954
    mfspr r22, 957
    mfspr r23, 958
    mfspr r24, 955
    mfspr r25, 952
    mfspr r26, 956
    mfspr r27, 1020
    mfspr r28, 1021
    mfspr r29, 1022
    mfspr r30, 1019
    mfspr r31, 1017
    stmw r19, 0x284(r2)
    b lab_1b4
    mfspr r25, 976
    mfspr r26, 977
    mfspr r27, 978
    mfspr r28, 979
    mfspr r29, 980
    mfspr r30, 981
    mfspr r31, 982
    stmw r25, 0x240(r2)
    mfspr r31, 22
    stw r31, 0x278(r2)
lab_1b4:
    blr
}
