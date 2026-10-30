#ifndef ZOIDS_OBJALLOC_H
#define ZOIDS_OBJALLOC_H

#include "types.h"

/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/objalloc.h. */
typedef struct ObjectHeap {
    u32 top;
    u32 current;
    u32 size;
    u32 remaining;
} ObjectHeap;

typedef struct HSD_ObjAllocLink {
    struct HSD_ObjAllocLink *next;
} HSD_ObjAllocLink;

typedef struct HSD_ObjAllocData {
    u32 number_limit_enabled : 1;
    u32 heap_limit_enabled : 1;
    HSD_ObjAllocLink *free_head;
    u32 used;
    u32 free;
    u32 peak;
    u32 number_limit;
    u32 heap_limit_size;
    u32 heap_limit_count;
    u32 size;
    u32 alignment_mask;
    struct HSD_ObjAllocData *next;
} HSD_ObjAllocData;

extern ObjectHeap obj_heap;
extern HSD_ObjAllocData *alloc_datas;
extern s32 HSD_ObjAllocAddFree(HSD_ObjAllocData *data, u32 count);
extern void *HSD_Alloc(u32 size);
extern s32 HSD_GetHeap(void);
extern long OSCheckHeap(s32 heap);
extern void *memset(void *memory, s32 value, u32 size);
extern void __assert(const char *file, u32 line, const char *condition);

#endif
