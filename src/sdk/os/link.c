/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/os/OSLink.c. */
#include "types.h"

typedef struct OSModuleInfo OSModuleInfo;

typedef struct OSModuleQueue {
    OSModuleInfo *head;
    OSModuleInfo *tail;
} OSModuleQueue;

extern OSModuleQueue __OSModuleInfoList;
__asm__(".set __OSModuleInfoList, 0x800030C8");
extern void *__OSStringTable;
__asm__(".set __OSStringTable, 0x800030D0");

void __OSModuleInit(void)
{
    __OSModuleInfoList.head = __OSModuleInfoList.tail = 0;
    __OSStringTable = 0;
}
