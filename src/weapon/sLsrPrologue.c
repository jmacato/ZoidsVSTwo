#include "weapon.h"

extern void ZeroMemory(void *address, u32 size);

void sLsrPrologue(void)
{
    ZeroMemory(sLsr, sizeof(sLsr));
    ZeroMemory(sLsr1, sizeof(sLsr1));
}
