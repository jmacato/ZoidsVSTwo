#ifndef ZOIDS_CLASS_MEMORY_H
#define ZOIDS_CLASS_MEMORY_H

#include "class.h"

typedef struct HSD_FreeList {
    struct HSD_FreeList *next;
} HSD_FreeList;

typedef struct HSD_MemoryEntry {
    u32 size;
    u32 allocated;
    u32 freed;
    HSD_FreeList *free_list;
    struct HSD_MemoryEntry *next;
} HSD_MemoryEntry;

extern HSD_MemoryEntry **memory_list;
extern s32 nb_memory_list;
extern struct HSD_Hash *info_hash;
extern HSD_ClassInfo hsdClass;
extern HSD_MemoryEntry *GetMemoryEntry(s32 index);
extern void *hsdAllocMemPiece(s32 size);
extern void hsdFreeMemPiece(void *memory, s32 size);
extern void ClassInfoInit(HSD_ClassInfo *info);
extern void *HSD_Alloc(u32 size);
extern void *memset(void *memory, s32 value, u32 size);
extern void *memcpy(void *destination, const void *source, u32 size);
extern s32 strcmp(const char *first, const char *second);
extern void *HSD_HashSearch(struct HSD_Hash *hash, void *key, s32 *success);
extern void __assert(const char *file, u32 line, const char *condition);

#endif
