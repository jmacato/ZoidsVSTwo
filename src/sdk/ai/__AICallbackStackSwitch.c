extern char __OldStack[];
extern char __CallbackStack[];

asm void __AICallbackStackSwitch(void)
{
    nofralloc
    mfspr r0, 8
    stw r0, 4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr r31, r3
    lis r5, __OldStack@ha
    addi r5, r5, __OldStack@l
    stw r1, 0(r5)
    lis r5, __CallbackStack@ha
    addi r5, r5, __CallbackStack@l
    lwz r1, 0(r5)
    addi r1, r1, -8
    mtspr 8, r31
    blrl
    lis r5, __OldStack@ha
    addi r5, r5, __OldStack@l
    lwz r1, 0(r5)
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtspr 8, r0
    blr
}
