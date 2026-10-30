#include "class_memory.h"

#define HSD_CLASS_INFO_INITIALIZED 1

void *hsdNew(HSD_ClassInfo *index)
{
    HSD_ClassInfo *info = index;
    HSD_ClassInfo *allocation_info = info;
    HSD_Class *object;
    if (!(allocation_info->head.flags & HSD_CLASS_INFO_INITIALIZED)) {
        allocation_info->head.initialize_info();
    }
    object = info->allocate(allocation_info);
    if (object == 0) {
        return 0;
    }
    ClassInfoInit(info);
    memset(object, 0, info->head.object_size);
    object->class_info = info;
    if (info->initialize(object) < 0) {
        info->destroy(object);
        return 0;
    }
    return object;
}
