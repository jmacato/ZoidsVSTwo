#include "initialize.h"
#include "gx_pixel.h"

struct HSD_VIDataState;
extern struct HSD_VIDataState HSD_VIData;
extern int current_render_pass;
extern int current_pix_fmt;
extern int current_z_fmt;
extern int need_invalidate;

extern void GXInvalidateVtxCache(void);
extern void GXInvalidateTexAll(void);
extern void GXSetFieldMode(u8 field, u8 half_aspect);

void HSD_StartRender(int pass)
{
    GXRenderModeObj *mode = (GXRenderModeObj *)&HSD_VIData;

    current_render_pass = pass;
    if (mode->aa)
        GXSetPixelFmt(GX_PF_RGB565_Z16, current_z_fmt);
    else
        GXSetPixelFmt(current_pix_fmt, GX_ZC_LINEAR);
    GXSetFieldMode(mode->field_rendering, mode->xfbHeight < mode->viHeight);
    if (need_invalidate) {
        if (need_invalidate & 1)
            GXInvalidateVtxCache();
        if (need_invalidate & 2)
            GXInvalidateTexAll();
        need_invalidate = 0;
    }
}
