#include "asm_ops.h"

char *GetR2()
{
    ASM_INSN(mr r3, r2);
}
