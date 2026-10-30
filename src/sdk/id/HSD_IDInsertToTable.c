/* Based on https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/id.c.
 */
#include "id.h"

static inline u32 hash(u32 key)
{
    return key % HSD_ID_BUCKET_COUNT;
}

extern void __assert(const char *file, u32 line, const char *condition);

static inline IDEntry *allocate_entry(void)
{
    IDEntry *entry = HSD_ObjAlloc(&alloc_data);
    if (entry == 0) {
        __assert("id.c", 67, "entry");
    }
    memset(entry, 0, sizeof(*entry));
    return entry;
}

void HSD_IDInsertToTable(HSD_IDTable *table, u32 key, void *data)
{
    IDEntry *entry;

    if (table == 0) {
        table = &default_table;
    }

    entry = table->buckets[hash(key)];
    while (entry != 0) {
        if (entry->key == key) {
            break;
        }
        entry = entry->next;
    }

    if (entry != 0) {
        entry->key = key;
        entry->data = data;
    } else {
        entry = allocate_entry();
        entry->key = key;
        entry->data = data;
        entry->next = table->buckets[hash(key)];
        table->buckets[hash(key)] = entry;
    }
}
