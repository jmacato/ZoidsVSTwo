/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/objalloc.c. */
#include "objalloc.h"

void *HSD_ObjAlloc(HSD_ObjAllocData *data)
{
    ObjectHeap *heap = &obj_heap;
    HSD_ObjAllocLink *current;
    u32 size;

    if (data->number_limit_enabled && data->used >= data->number_limit) {
        return 0;
    }
    if (data->heap_limit_enabled) {
        if (data->heap_limit_count == (unsigned)-1) {
            if (heap->top != 0) {
                size = heap->remaining;
            } else {
                size = OSCheckHeap(HSD_GetHeap());
            }
            if (size <= data->heap_limit_size) {
                data->heap_limit_count = data->used + data->free;
            }
        } else {
            if (heap->top != 0) {
                size = heap->remaining;
            } else {
                size = OSCheckHeap(HSD_GetHeap());
            }
            if (size > data->heap_limit_size) {
                data->heap_limit_count = -1;
            }
        }
        if (data->used >= data->heap_limit_count) {
            return 0;
        }
    }
    if (data->free == 0) {
        HSD_ObjAllocAddFree(data, 1);
        if (data->free == 0) {
            return 0;
        }
    }
    current = data->free_head;
    data->free_head = current->next;
    data->used += 1;
    data->free -= 1;
    if (data->used > data->peak) {
        data->peak = data->used;
    }
    return current;
}
