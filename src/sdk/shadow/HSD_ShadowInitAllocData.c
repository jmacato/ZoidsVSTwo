#include "shadow.h"

static HSD_ObjAllocData shadow_alloc_data;

static inline HSD_ObjAllocData *HSD_ShadowGetAllocData(void)
{
    return &shadow_alloc_data;
}

void HSD_ShadowInitAllocData(void)
{
    HSD_ObjAllocInit(HSD_ShadowGetAllocData(), sizeof(HSD_Shadow), 4);
}
