#ifndef ZOIDS_ID_H
#define ZOIDS_ID_H

#include "types.h"

/* Based on https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/id.h.
 */
#define HSD_ID_BUCKET_COUNT 101

typedef struct IDEntry {
    struct IDEntry *next;
    u32 key;
    void *data;
} IDEntry;

typedef struct HSD_IDTable {
    IDEntry *buckets[HSD_ID_BUCKET_COUNT];
} HSD_IDTable;

extern struct HSD_ObjAllocData alloc_data;
extern HSD_IDTable default_table;
extern void HSD_ObjAllocInit(struct HSD_ObjAllocData *data, u32 size, u32 alignment);
extern void *HSD_ObjAlloc(struct HSD_ObjAllocData *data);
extern void HSD_ObjFree(struct HSD_ObjAllocData *data, void *object);
extern void *memset(void *memory, s32 value, u32 size);
extern void *HSD_IDGetDataFromTable(HSD_IDTable *table, u32 key, s32 *success);

#endif
