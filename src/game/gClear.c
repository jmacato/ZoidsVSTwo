#include "types.h"

extern void HSD_VISetBlack(int black);
extern void HSD_StartRender(int pass);
extern void HSD_EndRender(void);
extern void HSD_VICopyXFB(int pass);
extern void HSD_VISetXFBDrawDone(void);
extern void waitV(s32 n);

void gClear(void)
{
    HSD_VISetBlack(1);
    HSD_StartRender(0);
    HSD_EndRender();
    HSD_VICopyXFB(0);
    HSD_VISetXFBDrawDone();
    waitV(3);
    HSD_VISetBlack(0);
}
