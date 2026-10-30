void __OSUnhandledException(void);

asm void OSDefaultExceptionHandler(void)
{
    nofralloc
    stw r0, 0(r4)
    stw r1, 4(r4)
    stw r2, 8(r4)
    stmw r6, 0x18(r4)
    mfspr r0, 913
    stw r0, 0x1a8(r4)
    mfspr r0, 914
    stw r0, 0x1ac(r4)
    mfspr r0, 915
    stw r0, 0x1b0(r4)
    mfspr r0, 916
    stw r0, 0x1b4(r4)
    mfspr r0, 917
    stw r0, 0x1b8(r4)
    mfspr r0, 918
    stw r0, 0x1bc(r4)
    mfspr r0, 919
    stw r0, 0x1c0(r4)
    mfspr r5, 18
    mfspr r6, 19
    stwu r1, -8(r1)
    b __OSUnhandledException
}
