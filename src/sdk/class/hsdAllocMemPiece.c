#include "class_memory.h"

void *hsdAllocMemPiece(s32 size)
{
    HSD_FreeList *piece;
    HSD_FreeList *remainder;
    HSD_FreeList *remainder_piece;
    HSD_FreeList *split_piece;
    HSD_MemoryEntry *entry;
    HSD_MemoryEntry *remainder_entry;
    HSD_MemoryEntry *larger_entry;
    HSD_MemoryEntry *new_remainder_entry;
    s32 index;
    void *memory;

    index = (size + 0x1F) / 32 - 1;
    entry = GetMemoryEntry((size + 0x1F) / 32 - 1);
    if (entry == 0) {
        return 0;
    }
    if ((piece = entry->free_list)) {
        entry->free_list = piece->next;
        entry->freed -= 1;
        return piece;
    }
    larger_entry = entry->next;
    while (larger_entry != 0) {
        if (larger_entry->free_list != 0) {
            remainder_entry =
                GetMemoryEntry((s32)(larger_entry->size - entry->size + 0x1F) / 32 - 1);
            if (remainder_entry == 0) {
                return 0;
            }
            split_piece = larger_entry->free_list;
            larger_entry->free_list = larger_entry->free_list->next;
            larger_entry->freed -= 1;
            larger_entry->allocated -= 1;
            remainder_piece = (void *)((char *)split_piece + entry->size);
            remainder_piece->next = remainder_entry->free_list;
            remainder_entry->free_list = remainder_piece;
            remainder_entry->allocated += 1;
            remainder_entry->freed += 1;
            entry->allocated += 1;
            return split_piece;
        }
        larger_entry = larger_entry->next;
    }
    index = index = (nb_memory_list - index) - 2;
    if (index >= 0) {
        new_remainder_entry = GetMemoryEntry(index);
        if (new_remainder_entry == 0) {
            return 0;
        }
    }
    memory = HSD_Alloc(nb_memory_list * 32);
    if (memory == 0) {
        return 0;
    }
    if (index >= 0) {
        remainder = (void *)((char *)memory + entry->size);
        remainder->next = new_remainder_entry->free_list;
        new_remainder_entry->free_list = remainder;
        new_remainder_entry->allocated += 1;
        new_remainder_entry->freed += 1;
    }
    entry->allocated += 1;
    return memory;
}
