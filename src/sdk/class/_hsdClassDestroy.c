#include "class_memory.h"

static inline void free_mem_piece(void *memory, s32 size)
{
    HSD_MemoryEntry *entry;
    HSD_FreeList *piece = (HSD_FreeList *)memory;

    if (memory != 0) {
        entry = GetMemoryEntry((size + 31) / 32 - 1);
        piece->next = entry->free_list;
        entry->free_list = piece;
        entry->freed += 1;
    }
}

void _hsdClassDestroy(HSD_Class *object)
{
    HSD_ClassInfo *info = object->class_info;
    info->head.live_count -= 1;
    free_mem_piece(object, info->head.object_size);
}
