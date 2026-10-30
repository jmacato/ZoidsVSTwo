/* Types follow
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/os/OSThread.h. */
#ifndef ZOIDS_OS_THREAD_H
#define ZOIDS_OS_THREAD_H
#include "os_context.h"
#define OS_THREAD_STATE_READY 1
#define OS_THREAD_STATE_RUNNING 2
#define OS_THREAD_STATE_WAITING 4
#define OS_THREAD_STATE_MORIBUND 8
#define OS_PRIORITY_MIN 0
#define OS_PRIORITY_MAX 31
#define OS_THREAD_ATTR_DETACH 0x0001u
#define OS_THREAD_STACK_MAGIC 0xDEADBABE
typedef struct OSMutex OSMutex;
typedef struct OSThreadLink {
    OSThread *next;
    OSThread *prev;
} OSThreadLink;
typedef struct OSMutexQueue {
    OSMutex *head;
    OSMutex *tail;
} OSMutexQueue;
struct OSThread {
    OSContext context;
    u16 state;
    u16 attr;
    s32 suspend;
    s32 priority;
    s32 base;
    void *val;
    OSThreadQueue *queue;
    OSThreadLink link;
    OSThreadQueue queueJoin;
    OSMutex *mutex;
    OSMutexQueue queueMutex;
    OSThreadLink linkActive;
    u8 *stackBase;
    u32 *stackEnd;
};
extern void OSCancelThread(OSThread *thread);
extern void __OSThreadInit(void);
extern void OSInitThreadQueue(OSThreadQueue *queue);
extern OSThread *OSGetCurrentThread(void);
extern s32 OSDisableScheduler(void);
extern s32 OSEnableScheduler(void);
extern s32 __OSGetEffectivePriority(OSThread *thread);
extern void __OSReschedule(void);
extern int OSCreateThread(OSThread *thread, void *(*function)(void *), void *argument,
                          void *stack, unsigned long stack_size, long priority,
                          unsigned short attributes);
extern void OSExitThread(void *value);
extern long OSResumeThread(OSThread *thread);
extern long OSSuspendThread(OSThread *thread);
extern void OSSleepThread(OSThreadQueue *queue);
extern void OSWakeupThread(OSThreadQueue *queue);
#endif
