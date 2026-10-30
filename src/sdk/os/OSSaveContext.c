#include "os_context.h"
#include "asm_ops.h"

u32 OSSaveContext(void)
{
    ASM_INSN(stmw r13, 0x34(r3));
    ASM_INSN(mfspr r0, 913);
    ASM_INSN(stw r0, 0x1a8(r3));
    ASM_INSN(mfspr r0, 914);
    ASM_INSN(stw r0, 0x1ac(r3));
    ASM_INSN(mfspr r0, 915);
    ASM_INSN(stw r0, 0x1b0(r3));
    ASM_INSN(mfspr r0, 916);
    ASM_INSN(stw r0, 0x1b4(r3));
    ASM_INSN(mfspr r0, 917);
    ASM_INSN(stw r0, 0x1b8(r3));
    ASM_INSN(mfspr r0, 918);
    ASM_INSN(stw r0, 0x1bc(r3));
    ASM_INSN(mfspr r0, 919);
    ASM_INSN(stw r0, 0x1c0(r3));
    ASM_INSN(mfcr r0);
    ASM_INSN(stw r0, 0x80(r3));
    ASM_INSN(mfspr r0, 8);
    ASM_INSN(stw r0, 0x84(r3));
    ASM_INSN(stw r0, 0x198(r3));
    ASM_INSN(mfmsr r0);
    ASM_INSN(stw r0, 0x19c(r3));
    ASM_INSN(mfspr r0, 9);
    ASM_INSN(stw r0, 0x88(r3));
    ASM_INSN(mfspr r0, 1);
    ASM_INSN(stw r0, 0x8c(r3));
    ASM_INSN(stw r1, 4(r3));
    ASM_INSN(stw r2, 8(r3));
    ASM_INSN(li r0, 1);
    ASM_INSN(stw r0, 0xc(r3));
    ASM_INSN(li r3, 0);
}
