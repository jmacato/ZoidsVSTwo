#include "types.h"
#include "asm_ops.h"

void TRK_ppc_memcpy(void *destination, const void *source, u32 length, u32 destinationMSR, u32 sourceMSR)
{
    ASM_INSN(mfmsr r8);
    ASM_INSN(li r10, 0);
    ASM_INSN(lccf8: cmpw r10, r5);
    ASM_INSN(beq lcd20);
    ASM_INSN(mtmsr r7);
    ASM_INSN(sync);
    ASM_INSN(lbzx r9, r10, r4);
    ASM_INSN(mtmsr r6);
    ASM_INSN(sync);
    ASM_INSN(stbx r9, r10, r3);
    ASM_INSN(addi r10, r10, 1);
    ASM_INSN(b lccf8);
    ASM_INSN(lcd20: mtmsr r8);
    ASM_INSN(sync);
}
