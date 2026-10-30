#include "types.h"
#include "metrotrk.h"

/* Preserve the original TRK branch structure. */
#pragma peephole off

extern u8 gTRKInterruptVectorTable[];

#define DB_EXCEPTION_MASK 0x44
#define EXCEPTION_SIZE 0x100
#define NUM_EXCEPTIONS 15

static u32 TRK_ISR_OFFSETS[NUM_EXCEPTIONS] = {
    PPC_SYSTEMRESET, PPC_MACHINECHECK, PPC_DATAACCESSERROR, PPC_INSTACCESSERROR,
    PPC_EXTERNALINTERRUPT, PPC_ALIGNMENTERROR, PPC_PROGRAMERROR, PPC_FPUNAVAILABLE,
    PPC_DECREMENTERINTERRUPT, PPC_SYSTEMCALL, PPC_TRACE, PPC_PERFORMANCE_MONITOR,
    PPC7xx_603E_INSTR_ADDR_BREAK, PPC7xx_603E_SYS_MANAGE, PPC_THERMAL_MANAGE
};

void TRK_copy_vector(u32 offset)
{
    u32 destination = TRKTargetTranslate(offset);

    TRK_memcpy((void *)destination, gTRKInterruptVectorTable + offset, EXCEPTION_SIZE);
    TRK_flush_cache(destination, EXCEPTION_SIZE);
}

void __TRK_copy_vectors(void)
{
    s32 i;
    u32 mask = *(u32 *)TRKTargetTranslate(DB_EXCEPTION_MASK);

    for (i = 0; i <= NUM_EXCEPTIONS - 1; i++) {
        if (mask & (1U << i))
            TRK_copy_vector(TRK_ISR_OFFSETS[i]);
    }
}
