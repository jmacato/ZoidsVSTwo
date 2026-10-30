#ifndef ZOIDS_HASH_H
#define ZOIDS_HASH_H

#include "class.h"

/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/hash.h. */
struct HSD_Hash;

typedef struct HSD_HashEntry {
    struct HSD_HashEntry *next;
    void *key;
    void *value;
} HSD_HashEntry;

typedef struct HSD_HashClassInfo {
    HSD_ClassInfo parent;
    s32 (*bucket_index)(struct HSD_Hash *hash);
    s32 (*compare_keys)(struct HSD_Hash *hash, void *stored_key, void *key);
} HSD_HashClassInfo;

typedef struct HSD_Hash {
    HSD_HashClassInfo *class_info;
    HSD_HashEntry **buckets;
    u32 bucket_count;
} HSD_Hash;

extern HSD_HashEntry *HashSearchEntry(HSD_Hash *hash, s32 index, void *key,
                                      HSD_HashEntry ***link);
extern void *HSD_HashSearch(HSD_Hash *hash, void *key, s32 *success);

#endif
