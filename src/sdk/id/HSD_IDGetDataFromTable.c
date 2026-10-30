/* Based on https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/id.c.
 */
#include "id.h"

static inline u32 hash(u32 key)
{
    return key % HSD_ID_BUCKET_COUNT;
}

void *HSD_IDGetDataFromTable(HSD_IDTable *table, u32 key, s32 *success)
{
    IDEntry *entry;

    if (table == 0) {
        table = &default_table;
    }

    entry = table->buckets[hash(key)];
    while (entry != 0) {
        if (entry->key == key) {
            if (success != 0) {
                *success = 1;
            }
            return entry->data;
        }
        entry = entry->next;
    }

    if (success != 0) {
        *success = 0;
    }
    return 0;
}
