#include "os_context.h"
#include "asm_ops.h"

void OSSetCurrentContext(OSContext *context)
{
    ASM_INSN(lis r4, -0x8000);
    ASM_INSN(stw r3, 0xd4(r4));
    ASM_INSN(clrlwi r5, r3, 2);
    ASM_INSN(stw r5, 0xc0(r4));
    ASM_INSN(lwz r5, 0xd8(r4));
    ASM_INSN(cmpw r5, r3);
    ASM_INSN(bne lab_38);
    ASM_INSN(lwz r6, 0x19c(r3));
    ASM_INSN(ori r6, r6, 0x2000);
    ASM_INSN(stw r6, 0x19c(r3));
    ASM_INSN(mfmsr r6);
    ASM_INSN(ori r6, r6, 2);
    ASM_INSN(mtmsr r6);
    ASM_INSN(blr);
    ASM_INSN(lab_38: lwz r6, 0x19c(r3));
    ASM_INSN(rlwinm r6, r6, 0, 0x13, 0x11);
    ASM_INSN(stw r6, 0x19c(r3));
    ASM_INSN(mfmsr r6);
    ASM_INSN(rlwinm r6, r6, 0, 0x13, 0x11);
    ASM_INSN(ori r6, r6, 2);
    ASM_INSN(mtmsr r6);
    ASM_INSN(isync);
}
