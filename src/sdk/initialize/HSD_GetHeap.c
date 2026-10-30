#include "initialize.h"

extern volatile s32 hsd_heap;

s32 HSD_GetHeap(void)
{
    return hsd_heap;
}
