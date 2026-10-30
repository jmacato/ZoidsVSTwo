#include "model_archive.h"
#include "cobj.h"
#include "lobj.h"
#include "video.h"

extern ModelArchive *ending;
extern ModelArchive *textPos;
extern ModelArchive *dspFilter;
extern s32 cOn, fOn;
extern void HSD_SetZSortMode(s32 a, s32 b);
extern void HSD_StartRender(int pass);
extern void HSD_EndRender(void);
extern void HSD_VISetXFBDrawDone(void);
extern void VIWaitForRetrace(void);
extern void jobjDispAll(HSD_JObj *joint);
extern void closeDisp(void);
extern void closeDispC(void);
extern void dispAscii(void);

void closeDispAll(void)
{
    HSD_StartRender(HSD_RP_SCREEN);
    if (HSD_CObjSetCurrent(ending->camera) != 0) {
        HSD_LObjSetCurrentAll(ending->lights);
        HSD_LObjSetupInit(ending->camera);
        if (cOn == 0)
            closeDisp();
        HSD_CObjEndCurrent();
    }
    if (HSD_CObjSetCurrent(textPos->camera) != 0) {
        dispAscii();
        if (cOn != 0)
            closeDispC();
        if (fOn != 0)
            jobjDispAll(dspFilter->models[0]);
        HSD_CObjEndCurrent();
    }
    HSD_EndRender();
    HSD_VICopyXFB(HSD_RP_SCREEN);
    HSD_VISetXFBDrawDone();
    VIWaitForRetrace();
}
