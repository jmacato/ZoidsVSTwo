#include "fobj.h"

extern struct HSD_ObjAllocData alloc_data;
extern void HSD_ObjFree(struct HSD_ObjAllocData *data, void *object);

void HSD_FObjFree(HSD_FObj *function)
{
    HSD_ObjFree(&alloc_data, function);
}
