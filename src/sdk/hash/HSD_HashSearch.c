#include "hash.h"
extern void __assert(const char *file, u32 line, const char *condition);
void *HSD_HashSearch(HSD_Hash *hash, void *key, s32 *success)
{
    HSD_HashEntry *entry;
    s32 index = hash->class_info->bucket_index(hash);
    if ((u32)index >= hash->bucket_count)
        __assert("hash.c", 113, "idx < hash->table_size");
    entry = HashSearchEntry(hash, index, key, 0);
    if (success != 0)
        *success = entry != 0;
    return entry ? entry->value : 0;
}
