#include "shadow.h"
#include "state.h"
#include "perf.h"
#define NULL 0
extern void HSD_Free(void *);
static void makeMatrix(HSD_Shadow *shadow);
static HSD_ObjAllocData shadow_alloc_data;

static inline HSD_ObjAllocData *HSD_ShadowGetAllocData(void)
{
    return &shadow_alloc_data;
}

extern void HSD_ShadowDeleteObject(HSD_Shadow *shadow, HSD_JObj *jobj);
static inline s32 ref_DEC(void *o)
{
    s32 result;
    if ((result = (((HSD_Obj *)o)->reference_count == (u16)-1)))
        return result;
    return ((HSD_Obj *)o)->reference_count-- == 0;
}
static inline void hsdDelete(void *o)
{
    if (o) {
        ((HSD_Class *)o)->class_info->release(o);
        ((HSD_Class *)o)->class_info->destroy(o);
    }
}

void HSD_ShadowRemove(HSD_Shadow *shadow)
{
    HSD_CObj *cobj;
    HSD_TObj *tobj;

    if (shadow == NULL) {
        return;
    }
    if ((cobj = shadow->camera) != NULL) {
        if (ref_DEC(cobj)) {
            hsdDelete(cobj);
        }
    }
    HSD_ShadowDeleteObject(shadow, 0);
    if (shadow->active) {
        HSD_MObjDeleteShadowTexture(shadow->texture);
    }
    if (shadow->texture->imagedesc->image_ptr != NULL) {
        HSD_Free(shadow->texture->imagedesc->image_ptr);
    }
    tobj = shadow->texture;
    HSD_ImageDescFree(tobj->imagedesc);
    HSD_TObjFree(tobj);
    HSD_ObjFree(HSD_ShadowGetAllocData(), shadow);
}
