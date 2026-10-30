#include "gx_tev_api.h"

extern s32 sPutFont(char *string, s32 x, s32 y, s32 z);

s32 sPutFont1(char *string, s32 x, s32 y, s32 z, GXColor *color, GXColor *shadow, s32 alpha, u32 flags)
{
    if (flags & 1)
        GXSetZMode(1, GX_LESS, 1);
    else
        GXSetZMode(0, GX_ALWAYS, 0);
    if (flags & 2) {
        if (flags & 4)
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_COPY);
        else
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    } else {
        if (flags & 4)
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);
        else
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    }
    GXSetAlphaCompare(GX_GEQUAL, alpha, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, (flags & 0x10) ? GX_CA_ZERO : GX_CA_A1, (flags & 8) ? GX_CA_ZERO : GX_CA_A0,
                    GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, *color);
    GXSetTevColor(GX_TEVREG1, *shadow);
    return sPutFont(string, x, y, z);
}
