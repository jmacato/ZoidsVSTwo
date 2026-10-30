/* Types follow
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/os/OSMutex.h. */
#ifndef ZOIDS_OS_MUTEX_H
#define ZOIDS_OS_MUTEX_H
#include "os_thread.h"
typedef struct OSMutexLink {
    OSMutex *next;
    OSMutex *prev;
} OSMutexLink;
struct OSMutex {
    OSThreadQueue queue;
    OSThread *thread;
    s32 count;
    OSMutexLink link;
};
extern void __OSUnlockAllMutex(OSThread *thread);
#endif
