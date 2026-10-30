#include "cobj.h"

float HSD_CObjGetFov(HSD_CObj *cobj)
{
    if (cobj == 0 || cobj->projection_type != PROJ_PERSPECTIVE) {
        return 0.0F;
    }
    return cobj->projection_param.perspective.fov;
}