#include "class_memory.h"

void _hsdClassAmnesia(HSD_ClassInfo *info)
{
    info->head.live_count = 0;
    info->head.peak_count = 0;
    if (info == &hsdClass) {
        nb_memory_list = 0;
        memory_list = 0;
        info_hash = 0;
    }
}
