/* Reference:
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/os/OSAlloc.h. */
#ifndef ZOIDS_OS_ALLOC_H
#define ZOIDS_OS_ALLOC_H
#include "types.h"
extern volatile int __OSCurrHeap;
extern void *OSAllocFromHeap(int heap, unsigned long size);
extern void OSFreeToHeap(int heap, void *address);
extern int OSSetCurrentHeap(int heap);
extern void *OSInitAlloc(void *start, void *end, int heaps);
extern int OSCreateHeap(void *start, void *end);
extern void OSDestroyHeap(int heap);
extern long OSCheckHeap(int heap);
#endif
