/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/list.c. */
#include "list.h"

extern HSD_ObjAllocData s_alloc_data;
extern HSD_ObjAllocData d_alloc_data;
extern void HSD_ObjAllocInit(HSD_ObjAllocData *data, u32 size, u32 alignment);
extern void *HSD_ObjAlloc(HSD_ObjAllocData *data);
extern void HSD_ObjFree(HSD_ObjAllocData *data, void *object);
void HSD_ListInitAllocData(void)
{
    HSD_ObjAllocInit(&s_alloc_data, sizeof(HSD_SList), 4);
    HSD_ObjAllocInit(&d_alloc_data, sizeof(HSD_DList), 4);
}


HSD_SList *HSD_SListAlloc(void)
{
    HSD_SList *list = HSD_ObjAlloc(&s_alloc_data);
    if (list == 0)
        __assert("list.c", 76, "list");
    memset(list, 0, sizeof(*list));
    return list;
}
HSD_SList *HSD_SListAppend(HSD_SList *next, void *data)
{
    HSD_SList *list = HSD_SListAlloc();
    list->data = data;
    return HSD_SListAppendList(next, list);
}
HSD_SList *HSD_SListPrepend(HSD_SList *next, void *data)
{
    HSD_SList *list = HSD_SListAlloc();
    list->data = data;
    return HSD_SListPrependList(next, list);
}
HSD_SList *HSD_SListAppendList(HSD_SList *list, HSD_SList *next)
{
    if (next == 0)
        __assert("list.c", 179, "next");
    if (list != 0) {
        next->next = list->next;
        list->next = next;
        return list;
    } else {
        next->next = 0;
        return next;
    }
}
HSD_SList *HSD_SListPrependList(HSD_SList *list, HSD_SList *previous)
{
    if (previous == 0)
        __assert("list.c", 202, "prev");
    previous->next = list;
    return previous;
}
HSD_SList *HSD_SListRemove(HSD_SList *list)
{
    HSD_SList *next;
    if (list != 0) {
        next = list->next;
        HSD_ObjFree(&s_alloc_data, list);
        return next;
    }
    return 0;
}
