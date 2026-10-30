/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/hash.c. */
#include "hash.h"

HSD_HashEntry *HashSearchEntry(HSD_Hash *hash, s32 index, void *key,
                               HSD_HashEntry ***link)
{
    if (hash->buckets[index] == 0) {
        return 0;
    }
    if (link != 0) {
        HSD_HashEntry **entry;
        for (entry = &hash->buckets[index]; *entry != 0; entry = &(*entry)->next) {
            if (hash->class_info->compare_keys(hash, (*entry)->key, key) == 0) {
                *link = entry;
                return *entry;
            }
        }
    } else {
        HSD_HashEntry *entry;
        for (entry = hash->buckets[index]; entry != 0; entry = entry->next) {
            if (hash->class_info->compare_keys(hash, entry->key, key) == 0) {
                return entry;
            }
        }
    }
    return 0;
}
