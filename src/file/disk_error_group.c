#pragma readonly_strings on
#include "model_archive.h"
#include "video.h"
#include "gx_geometry.h"

extern ModelArchive *dspLoad;
extern s32 dspLoadF;
extern void OSReport(const char *format, ...);
extern void HSD_StateInvalidate(s32 state);
extern void HSD_SetZSortMode(s32 a, s32 b);
extern void HSD_FogSet(void *fog);
extern void VIWaitForRetrace(void);
extern s32 HSD_CObjSetCurrent(struct HSD_CObj *cobj);
extern void HSD_CObjEndCurrent(void);
extern void HSD_LObjSetCurrentAll(struct HSD_LObj *lobj);
extern void HSD_LObjSetupInit(struct HSD_CObj *cobj);
extern void jobjDispAll(HSD_JObj *joint);
extern void sPadAll(void);

#include "dvd_file.h"
#include "disk_error.h"

void dspLoadNow(void)
{
    GXColor clear;

    OSReport("LOADing\n");
    if (dspLoad == 0)
        return;
    if (dspLoadF == 0)
        return;
    if (dspLoadF > 1)
        return;
    HSD_StateInvalidate(-1);
    HSD_SetZSortMode(0, 0);
    GXSetCullMode(GX_CULL_NONE);
    clear = (GXColor){ 0, 0, 0, 0 };
    HSD_VIData.current.vi.clear_clr = clear;
    HSD_FogSet(0);
    VIWaitForRetrace();
    HSD_StartRender(HSD_RP_SCREEN);
    if (HSD_CObjSetCurrent(dspLoad->camera) != 0) {
        HSD_LObjSetCurrentAll(dspLoad->lights);
        HSD_LObjSetupInit(dspLoad->camera);
        jobjDispAll(dspLoad->models[0]);
        HSD_CObjEndCurrent();
    }
    HSD_EndRender();
    HSD_VICopyXFB(HSD_RP_SCREEN);
    HSD_VISetXFBDrawDone();
    VIWaitForRetrace();
    sPadAll();
    dspLoadF = 2;
}


extern s32 DVDCheckDisk(void);
extern void sbk(s32 result, DVDFileInfo *file);

static const f32 disk_pool_0 = 0.0F;
static const f32 disk_pool_1 = -300.0F;
static const f32 disk_pool_2 = 0.6F;
static const f32 disk_pool_3 = 0.7F;




extern char *diskErrMsg[];
extern s32 sPutFontCamPrologue(void);
extern void sPutStringXYC2(char *text, GXColor color, GXColor shadow, s32 alpha, s32 flags, s32 align, f32 x,
                           f32 y, f32 z, f32 scale_x, f32 scale_y);
extern s32 DVDGetDriveStatus(void);


