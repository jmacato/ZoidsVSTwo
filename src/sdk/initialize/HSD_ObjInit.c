#include "initialize.h"

extern void HSD_ListInitAllocData(void);
extern void HSD_AObjInitAllocData(void);
extern void HSD_FObjInitAllocData(void);
extern void HSD_IDInitAllocData(void);
extern void HSD_VecInitAllocData(void);
extern void HSD_MtxInitAllocData(void);
extern void HSD_RObjInitAllocData(void);
extern void HSD_RenderInitAllocData(void);
extern void HSD_ShadowInitAllocData(void);
extern void HSD_ZListInitAllocData(void);

void HSD_ObjInit(void)
{
    HSD_ListInitAllocData();
    HSD_AObjInitAllocData();
    HSD_FObjInitAllocData();
    HSD_IDInitAllocData();
    HSD_VecInitAllocData();
    HSD_MtxInitAllocData();
    HSD_RObjInitAllocData();
    HSD_RenderInitAllocData();
    HSD_ShadowInitAllocData();
    HSD_ZListInitAllocData();
}
