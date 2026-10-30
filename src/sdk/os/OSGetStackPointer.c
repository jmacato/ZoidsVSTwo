#include "os_context.h"
#include "asm_ops.h"

u32 OSGetStackPointer(void)
{
    ASM_INSN(mr r3, r1);
}
