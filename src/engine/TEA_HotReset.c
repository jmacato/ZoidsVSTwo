#include "os_reset.h"
void TEA_HotReset(u32 code)
{
    OSResetSystem(OS_RESET_HOTRESET, code, 0);
}
