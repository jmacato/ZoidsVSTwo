#include "gx_pixel.h"
#include "gx_geometry.h"

/* the SDK defines GXSetTevClampMode(void) (gx_tev_api.h); the original call passes 2 arguments */
extern void GXSetTevClampMode(s32 stage, s32 mode);
extern void GXSetAlphaCompare(s32 comp0, u8 ref0, s32 op, s32 comp1, u8 ref1);

void sPutFontAllEpilogue(void)
{
    GXSetTevClampMode(GX_TEVSTAGE0, GX_TC_LINEAR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    GXSetCullMode(GX_CULL_BACK);
    GXSetAlphaCompare(GX_ALWAYS, 0, 0, GX_ALWAYS, 0);
}
