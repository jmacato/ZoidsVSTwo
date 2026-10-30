#include "asm_ops.h"

void __OSDBIntegrator(void)
{
    ASM_INSN(li r5, 0x40);
    ASM_INSN(mfspr r3, 8);
    ASM_INSN(stw r3, 0xc(r5));
    ASM_INSN(lwz r3, 8(r5));
    ASM_INSN(oris r3, r3, 0x8000);
    ASM_INSN(mtspr 8, r3);
    ASM_INSN(li r3, 0x30);
    ASM_INSN(mtmsr r3);
}
