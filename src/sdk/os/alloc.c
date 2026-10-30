/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/os/OSAlloc.c. */
#include "os_alloc.h"
#define NULL 0
#define OFFSET(value, align) ((u32)(value) & ((align) - 1))
extern void OSReport(const char *, ...);
#define ALIGNMENT 32

#define InRange(cell, arenaStart, arenaEnd)                                            \
    ((u32)arenaStart <= (u32)cell) && ((u32)cell < (u32)arenaEnd)

#define HEADERSIZE 32u
#define MINOBJSIZE 64u

struct Cell {
    struct Cell *prev;
    struct Cell *next;
    long size;
};

struct HeapDesc {
    long size;
    struct Cell *free;
    struct Cell *allocated;
};

volatile int __OSCurrHeap = -1;

static struct HeapDesc *HeapArray = 0;
static int NumHeaps = 0;
static void *ArenaStart = 0;
static void *ArenaEnd = 0;

static struct Cell *DLInsert(struct Cell *, struct Cell *);
static inline struct Cell *DLAddFront(struct Cell *list, struct Cell *cell)
{
    cell->next = list;
    cell->prev = 0;
    if (list) {
        list->prev = cell;
    }
    return cell;
}
static inline struct Cell *DLExtract(struct Cell *list, struct Cell *cell)
{
    if (cell->next) {
        cell->next->prev = cell->prev;
    }
    if (cell->prev == NULL) {
        return cell->next;
    }
    cell->prev->next = cell->next;
    return list;
}
#define ASSERTREPORT(line, cond)                                                       \
    if (!(cond)) {                                                                     \
        OSReport("OSCheckHeap: Failed " #cond " in %d", line);                         \
        return -1;                                                                     \
    }

static struct Cell *DLInsert(struct Cell *list, struct Cell *cell)
{
    struct Cell *prev;
    struct Cell *next;

    for (next = list, prev = NULL; next != 0; prev = next, next = next->next) {
        if (cell <= next) {
            break;
        }
    }

    cell->next = next;
    cell->prev = prev;
    if (next) {
        next->prev = cell;
        if ((u8 *)cell + cell->size == (u8 *)next) {
            cell->size += next->size;
            next = next->next;
            cell->next = next;
            if (next) {
                next->prev = cell;
            }
        }
    }
    if (prev) {
        prev->next = cell;
        if ((u8 *)prev + prev->size == (u8 *)cell) {
            prev->size += cell->size;
            prev->next = next;
            if (next) {
                next->prev = prev;
            }
        }
        return list;
    }
    return cell;
}
void *OSAllocFromHeap(int heap, unsigned long size)
{
    struct HeapDesc *hd;
    struct Cell *cell;
    struct Cell *newCell;
    long leftoverSize;
    long requested;

    requested = size;

    hd = &HeapArray[heap];
    size += HEADERSIZE;
    size = (size + 0x1F) & 0xFFFFFFE0;

    for (cell = hd->free; cell != NULL; cell = cell->next) {
        if ((signed)size <= (signed)cell->size) {
            break;
        }
    }

    if (cell == NULL) {
        return NULL;
    }

    leftoverSize = cell->size - size;
    if (leftoverSize < MINOBJSIZE) {
        hd->free = DLExtract(hd->free, cell);
    } else {
        cell->size = size;
        newCell = (void *)((u8 *)cell + size);
        newCell->size = leftoverSize;
        newCell->prev = cell->prev;
        newCell->next = cell->next;
        if (newCell->next != NULL) {
            newCell->next->prev = newCell;
        }
        if (newCell->prev != NULL) {
            newCell->prev->next = newCell;
        } else {
            hd->free = newCell;
        }
    }

    hd->allocated = DLAddFront(hd->allocated, cell);
    return (u8 *)cell + HEADERSIZE;
}
void OSFreeToHeap(int heap, void *ptr)
{
    struct HeapDesc *hd;
    struct Cell *cell;

    cell = (void *)((u32)ptr - HEADERSIZE);
    hd = &HeapArray[heap];
    hd->allocated = DLExtract(hd->allocated, cell);
    hd->free = DLInsert(hd->free, cell);
}
int OSSetCurrentHeap(int heap)
{
    int prev;

    prev = __OSCurrHeap;
    __OSCurrHeap = heap;
    return prev;
}
void *OSInitAlloc(void *arenaStart, void *arenaEnd, int maxHeaps)
{
    unsigned long arraySize;
    int i;
    struct HeapDesc *hd;

    arraySize = maxHeaps * sizeof(struct HeapDesc);
    HeapArray = arenaStart;
    NumHeaps = maxHeaps;

    for (i = 0; i < NumHeaps; i++) {
        hd = &HeapArray[i];
        hd->size = -1;
        hd->free = hd->allocated = 0;
    }
    __OSCurrHeap = -1;
    arenaStart = (void *)((u32)((char *)HeapArray + arraySize));
    arenaStart = (void *)(((u32)arenaStart + 0x1F) & 0xFFFFFFE0);
    ArenaStart = arenaStart;
    ArenaEnd = (void *)((u32)arenaEnd & 0xFFFFFFE0);
    return arenaStart;
}
int OSCreateHeap(void *start, void *end)
{
    int heap;
    struct HeapDesc *hd;
    struct Cell *cell;

    start = (void *)(((u32)start + 0x1FU) & ~((ALIGNMENT) - 1));
    end = (void *)(((u32)end) & ~((ALIGNMENT) - 1));

    for (heap = 0; heap < NumHeaps; heap++) {
        hd = &HeapArray[heap];
        if (hd->size < 0) {
            hd->size = (u32)end - (u32)start;
            cell = start;
            cell->prev = 0;
            cell->next = 0;
            cell->size = hd->size;
            hd->free = cell;
            hd->allocated = 0;
            return heap;
        }
    }
    return -1;
}

/* Stringified checks preserve the original diagnostic text. */
// clang-format off
long OSCheckHeap(int heap) {
    struct HeapDesc * hd;
    struct Cell * cell;
    long total = 0;
    long free = 0;

    ASSERTREPORT(0x37D, HeapArray);
    ASSERTREPORT(0x37E, 0 <= heap && heap < NumHeaps);
    hd = &HeapArray[heap];
    ASSERTREPORT(0x381, 0 <= hd->size);
    
    ASSERTREPORT(0x383, hd->allocated == NULL || hd->allocated->prev == NULL);

    for(cell = hd->allocated; cell; cell = cell->next) {
        ASSERTREPORT(0x386, InRange(cell, ArenaStart, ArenaEnd));
        ASSERTREPORT(0x387, OFFSET(cell, ALIGNMENT) == 0);
        ASSERTREPORT(0x388, cell->next == NULL || cell->next->prev == cell);
        ASSERTREPORT(0x389, MINOBJSIZE <= cell->size);
        ASSERTREPORT(0x38A, OFFSET(cell->size, ALIGNMENT) == 0);
        total += cell->size;
        ASSERTREPORT(0x38D, 0 < total && total <= hd->size);
    }

    
    ASSERTREPORT(0x395, hd->free == NULL || hd->free->prev == NULL); 
    
    for(cell = hd->free; cell; cell = cell->next) {
        ASSERTREPORT(0x398, InRange(cell, ArenaStart, ArenaEnd));
        ASSERTREPORT(0x399, OFFSET(cell, ALIGNMENT) == 0);
        ASSERTREPORT(0x39A, cell->next == NULL || cell->next->prev == cell);
        ASSERTREPORT(0x39B, MINOBJSIZE <= cell->size);
        ASSERTREPORT(0x39C, OFFSET(cell->size, ALIGNMENT) == 0);
        ASSERTREPORT(0x39D, cell->next == NULL || (char*) cell + cell->size < (char*) cell->next);
        total += cell->size;
        free = (cell->size + free);
        free -= HEADERSIZE;
        ASSERTREPORT(0x3A1, 0 < total && total <= hd->size);
    }
    ASSERTREPORT(0x3A8, total == hd->size);
    return free;
}

// clang-format on
