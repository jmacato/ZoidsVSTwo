#include "class_memory.h"

#define OSRoundDown32B(size) ((u32)(size) & ~31u)
#define OSRoundUp32B(size) (((u32)(size) + 31u) & ~31u)

HSD_MemoryEntry *GetMemoryEntry(s32 index)
{

    if (index < 0) {
        __assert("class.c", 174, "idx >= 0");
    }

    if (index >= nb_memory_list) {
        if (nb_memory_list == 0) {
            s32 new_count;

            for (new_count = 32; index >= new_count; new_count *= 2) {
            }
            memory_list = HSD_Alloc(new_count * sizeof(*memory_list));
            if (memory_list == 0) {
                return 0;
            }
            memset(memory_list, 0, new_count * sizeof(*memory_list));
            nb_memory_list = new_count;
        } else {
            HSD_MemoryEntry **old_entries;
            HSD_MemoryEntry **new_entries;
            s32 old_size, new_count;

            new_count = nb_memory_list * 2;
            while (index >= new_count) {
                new_count *= 2;
            }

            new_entries = HSD_Alloc(sizeof(*memory_list) * new_count);
            if (new_entries == 0) {
                return 0;
            }

            memcpy(new_entries, memory_list, sizeof(*memory_list) * nb_memory_list);
            memset(&new_entries[nb_memory_list], 0, 4 * (new_count - nb_memory_list));

            old_size = OSRoundDown32B(nb_memory_list * sizeof(*memory_list));
            old_entries = memory_list;
            nb_memory_list = new_count;
            memory_list = new_entries;

            hsdFreeMemPiece(old_entries, old_size);
            memory_list[OSRoundUp32B(old_size) / 32 - 1]->allocated += 1;
        }
    }

    {
        s32 neighbor;
        s32 found;
        HSD_MemoryEntry *entry;
        if (memory_list[index] == 0) {
            entry = HSD_Alloc(sizeof(HSD_MemoryEntry));
            if (entry == 0) {
                return 0;
            }
            memset(entry, 0, sizeof(HSD_MemoryEntry));
            entry->size = (index + 1) * 32;
            memory_list[index] = entry;

            found = 0;
            for (neighbor = index - 1; neighbor >= 0; --neighbor) {
                if (memory_list[neighbor] != 0) {
                    found = 1;
                    entry->next = memory_list[neighbor]->next;
                    memory_list[neighbor]->next = entry;
                    break;
                }
            }
            if (found == 0) {
                for (neighbor = index + 1; neighbor < nb_memory_list; neighbor++) {
                    if (memory_list[neighbor] != 0) {
                        entry->next = memory_list[neighbor];
                        break;
                    }
                }
            }
        }
        return memory_list[index];
    }
}
