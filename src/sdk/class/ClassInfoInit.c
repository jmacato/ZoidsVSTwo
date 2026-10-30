#include "class_memory.h"

#define HSD_CLASS_INFO_INITIALIZED 1

#pragma push
#pragma dont_inline on
void ClassInfoInit(HSD_ClassInfo *info)
{
    if ((info->head.flags & HSD_CLASS_INFO_INITIALIZED) == 0) {
        (*info->head.initialize_info)();
    }
}

#pragma pop
