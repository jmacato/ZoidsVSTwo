/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/objalloc.c. */
#include "objalloc.h"

void HSD_ObjSetHeap(u32 size, void *memory)
{
    obj_heap.current = (u32)memory;
    obj_heap.top = (u32)memory;
    obj_heap.remaining = size;
    obj_heap.size = size;
}
