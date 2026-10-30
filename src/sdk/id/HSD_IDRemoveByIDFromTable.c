/* Based on https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/id.c.
 */
#include "id.h"

static inline u32 hash(u32 key)
{
    return key % HSD_ID_BUCKET_COUNT;
}

static inline void free_entry(IDEntry *entry)
{
    HSD_ObjFree(&alloc_data, entry);
}

void HSD_IDRemoveByIDFromTable(HSD_IDTable *table, u32 key)
{
    IDEntry *entry;
    IDEntry *previous;

    if (table == 0) {
        table = &default_table;
    }

    previous = 0;
    for (entry = table->buckets[hash(key)]; entry != 0; entry = entry->next) {
        if (entry->key == key) {
            if (previous != 0) {
                previous->next = entry->next;
            } else {
                table->buckets[hash(key)] = entry->next;
            }
            free_entry(entry);
            return;
        }
        previous = entry;
    }
}
