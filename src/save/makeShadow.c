#pragma readonly_strings on
#pragma auto_inline off
#include "types.h"
#include "shadow.h"
#include "lobj.h"

extern void OSReport(const char *format, ...);
extern void __assert(const char *file, u32 line, const char *condition);

HSD_Shadow *makeShadow(HSD_LObj *light)
{
    HSD_Shadow *shadow = HSD_ShadowAlloc();

    if (shadow == (void *)0)
        OSReport("shadow alloc error\n");
    shadow ? (void)0 : __assert("shadow.h", 117, "shadow");
    shadow->user_data = light;
    switch (light->flags & LOBJ_TYPE_MASK) {
    case LOBJ_INFINITE:
        HSD_CObjSetProjectionType(shadow->camera, PROJ_ORTHO);
        break;
    case LOBJ_POINT:
    case LOBJ_SPOT:
        HSD_CObjSetProjectionType(shadow->camera, PROJ_FRUSTUM);
        break;
    default:
        __assert("Sub.c", 143, "0");
        break;
    }
    HSD_ShadowSetSize(shadow, 512, 512);
    HSD_CObjSetNear(shadow->camera, 0.1f);
    HSD_CObjSetFar(shadow->camera, 1000.0f);
    return shadow;
}
