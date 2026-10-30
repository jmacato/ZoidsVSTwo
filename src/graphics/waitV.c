#include "types.h"

extern void HSD_VIWaitXFBFlush(void);
extern void VIWaitForRetrace(void);

void waitV(s32 count)
{
    while (count != 0) {
        HSD_VIWaitXFBFlush();
        VIWaitForRetrace();
        count--;
    }
}
