#include "shadow.h"
#include "state.h"
#include "perf.h"
extern void *memset(void *, s32, u32);
static HSD_ObjAllocData shadow_alloc_data;

__attribute__((weak)) inline HSD_ObjAllocData *HSD_ShadowGetAllocData(void)
{
    return &shadow_alloc_data;
}

static inline HSD_TObj *makeShadowTObj(void)
{
    HSD_TObj *shadowTObj;
    shadowTObj = HSD_TObjAlloc();
    shadowTObj->src = 0;
    shadowTObj->wrap_s = 0;
    shadowTObj->wrap_t = 0;
    shadowTObj->flags = TEX_COORD_SHADOW | TEX_LIGHTMAP_SHADOW | TEX_COLORMAP_MODULATE |
                        TEX_ALPHAMAP_PASS;
    shadowTObj->imagedesc = HSD_ImageDescAlloc();
    return shadowTObj;
}

HSD_Shadow *HSD_ShadowAlloc(void)
{
    HSD_Shadow *shadow;

    shadow = HSD_ObjAlloc(HSD_ShadowGetAllocData());
    memset(shadow, 0, sizeof(HSD_Shadow));
    shadow->camera = HSD_CObjAlloc();
    shadow->texture = makeShadowTObj();

    shadow->scaleS = 0.5F;
    shadow->scaleT = -0.5F;
    shadow->transS = 0.5F;
    shadow->transT = 0.5F;
    shadow->intensity = 0;

    shadow->texture->imagedesc->format = GX_TF_I4;
    shadow->texture->imagedesc->width = 256;
    shadow->texture->imagedesc->height = 256;

    HSD_CObjSetViewportfx4(shadow->camera, 0.0f, 256.f, 0.0f, 256.f);
    HSD_CObjSetScissorx4(shadow->camera, 0, 256, 0, 256);

    return shadow;
}
