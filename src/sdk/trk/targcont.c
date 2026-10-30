#include "types.h"
#include "metrotrk.h"

extern void TRKSwapAndGo(void);

s32 TRKTargetContinue(void)
{
    TRKTargetSetStopped(0);
    UnreserveEXI2Port();
    TRKSwapAndGo();
    ReserveEXI2Port();
    return 0;
}
