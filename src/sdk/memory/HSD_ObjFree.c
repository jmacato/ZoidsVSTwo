/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/objalloc.c. */
#include "objalloc.h"

void HSD_ObjFree(HSD_ObjAllocData *data, void *object)
{
    HSD_ObjAllocLink *link = object;
    link->next = data->free_head;
    data->free_head = link;
    data->free += 1;
    data->used -= 1;
}
