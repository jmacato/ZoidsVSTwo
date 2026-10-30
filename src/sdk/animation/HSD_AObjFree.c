#include "aobj.h"

extern struct HSD_ObjAllocData alloc_data;
extern void HSD_ObjFree(struct HSD_ObjAllocData *data, void *object);

void HSD_AObjFree(HSD_AObj *animation)
{
    if (animation != 0) {
        HSD_ObjFree(&alloc_data, animation);
    }
}
