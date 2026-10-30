#include "gx_geometry.h"
#include "gx_state.h"

void GXGetCullMode(GXCullMode *mode)
{
    GXCullMode hwMode;

    hwMode = GX_GET_FIELD(gx->genMode, 2, 14);
    switch (hwMode) {
    case GX_CULL_FRONT:
        *mode = GX_CULL_BACK;
        break;
    case GX_CULL_BACK:
        *mode = GX_CULL_FRONT;
        break;
    default:
        *mode = hwMode;
        break;
    }
}
