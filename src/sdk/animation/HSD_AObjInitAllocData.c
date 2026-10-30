#include "aobj.h"

extern struct HSD_ObjAllocData alloc_data;
extern void HSD_ObjAllocInit(struct HSD_ObjAllocData *data, u32 size, u32 alignment);

void HSD_AObjInitAllocData(void)
{
    HSD_ObjAllocInit(&alloc_data, sizeof(HSD_AObj), 4);
}
