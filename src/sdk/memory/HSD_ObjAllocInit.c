/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/objalloc.c. */
#include "objalloc.h"

static inline void remove_allocator(HSD_ObjAllocData *data)
{
    HSD_ObjAllocData **current = &alloc_datas;
    while (*current != 0) {
        if (*current == data) {
            *current = (*current)->next;
        } else {
            current = &(*current)->next;
        }
    }
}

void HSD_ObjAllocInit(HSD_ObjAllocData *data, u32 size, u32 alignment)
{
    if (data == 0) {
        __assert("objalloc.c", 0x185, "data");
    }
    if (data != 0) {
        remove_allocator(data);
    } else {
        alloc_datas = 0;
    }
    memset(data, 0, sizeof(HSD_ObjAllocData));
    data->number_limit = -1;
    data->heap_limit_size = 0;
    data->heap_limit_count = -1;
    data->alignment_mask = alignment - 1;
    data->size = (size + data->alignment_mask) & ~data->alignment_mask;
    data->next = alloc_datas;
    alloc_datas = data;
}
