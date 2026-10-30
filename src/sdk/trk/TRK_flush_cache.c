#include "types.h"
#include "asm_ops.h"

void TRK_flush_cache(u32 address, u32 length)
{
    ASM_INSN(lis r5, -1);
    ASM_INSN(ori r5, r5, 65521);
    ASM_INSN(and r5, r5, r3);
    ASM_INSN(sub r3, r3, r5);
    ASM_INSN(add r4, r4, r3);
    ASM_INSN(lcb84: dcbst r0, r5);
    ASM_INSN(dcbf r0, r5);
    ASM_INSN(sync);
    ASM_INSN(icbi r0, r5);
    ASM_INSN(addic r5, r5, 8);
    ASM_INSN(addic. r4, r4, -8);
    ASM_INSN(bge lcb84);
    ASM_INSN(isync);
}
