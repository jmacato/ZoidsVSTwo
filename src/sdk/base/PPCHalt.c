asm void PPCHalt(void)
{
    nofralloc
    sync
lab_4:
    nop
    li r3, 0
    nop
    b lab_4
}
