#include "class_memory.h"

HSD_Class *_hsdClassAlloc(HSD_ClassInfo *info)
{
    HSD_Class *memory = hsdAllocMemPiece(info->head.object_size);
    if (memory != 0) {
        info->head.live_count += 1;
        if (info->head.live_count > info->head.peak_count) {
            info->head.peak_count = info->head.live_count;
        }
    }
    return memory;
}
