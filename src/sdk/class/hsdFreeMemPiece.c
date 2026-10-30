#include "class_memory.h"

void hsdFreeMemPiece(void *memory, s32 size)
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
