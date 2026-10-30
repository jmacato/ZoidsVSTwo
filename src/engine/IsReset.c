#include "os_reset.h"
int IsReset(void)
{
    return OSGetResetSwitchState();
}
