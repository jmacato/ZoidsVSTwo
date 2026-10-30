#ifndef ZOIDS_LIST_H
#define ZOIDS_LIST_H

#include "objalloc.h"

typedef struct HSD_SList {
    struct HSD_SList *next;
    void *data;
} HSD_SList;

typedef struct HSD_DList {
    struct HSD_DList *next;
    struct HSD_DList *previous;
    void *data;
} HSD_DList;

extern void HSD_ListInitAllocData(void);
extern HSD_ObjAllocData *HSD_SListGetAllocData(void);
extern HSD_ObjAllocData *HSD_DListGetAllocData(void);
extern HSD_SList *HSD_SListAlloc(void);
extern HSD_SList *HSD_SListAppend(HSD_SList *next, void *data);
extern HSD_SList *HSD_SListPrepend(HSD_SList *next, void *data);
extern HSD_SList *HSD_SListAppendList(HSD_SList *list, HSD_SList *next);
extern HSD_SList *HSD_SListPrependList(HSD_SList *list, HSD_SList *previous);
extern HSD_SList *HSD_SListRemove(HSD_SList *list);

#endif
