/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/objalloc.c. */
#include "objalloc.h"

ObjectHeap obj_heap = {0, 0, -1, -1};

s32 HSD_ObjAllocAddFree(HSD_ObjAllocData *data, u32 count)
{
    u32 pool_end;
    u32 pool_size;
    u32 pool_start;

    if (data == 0) {
        __assert("objalloc.c", 0xEE, "data");
    }
    pool_size = data->size * count;
    if (obj_heap.top != 0) {
        u32 alignment = data->alignment_mask;
        pool_end = obj_heap.top + obj_heap.size;
        pool_start = (obj_heap.current + alignment) & ~alignment;
        if (pool_start > pool_end) {
            return 0;
        }
        if (pool_end - pool_start < pool_size) {
            pool_size = pool_end - pool_start - (pool_end - pool_start) % data->size;
        }
        count = pool_size / data->size;
        if (count == 0) {
            return 0;
        }
        obj_heap.current = pool_start + pool_size;
        obj_heap.remaining = pool_end - obj_heap.current;
    } else {
        pool_start = (u32)HSD_Alloc(pool_size);
        if (pool_start == 0) {
            return 0;
        }
        obj_heap.remaining -= pool_size;
    }

    {
        int index;
        HSD_ObjAllocLink *link;
        for (index = 0; (unsigned)index < count - 1; index++) {
            link = (HSD_ObjAllocLink *)(pool_start + data->size * index);
            link->next = (HSD_ObjAllocLink *)(pool_start + data->size * (index + 1));
        }
        link = (HSD_ObjAllocLink *)(pool_start + data->size * index);
        link->next = data->free_head;
    }

    data->free_head = (HSD_ObjAllocLink *)pool_start;
    data->free += count;
    return count;
}
