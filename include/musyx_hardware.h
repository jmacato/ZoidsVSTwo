#ifndef ZOIDS_MUSYX_HARDWARE_H
#define ZOIDS_MUSYX_HARDWARE_H

#include "types.h"

extern void hwEnableIrq(void);
extern void hwDisableIrq(void);
extern u32 hwIsActive(u32 v);
extern void hwSetPriority(u32 v, u32 prio);
extern void hwBreak(s32 v);
extern void *salMalloc(u32 length);
extern void salFree(void *address);

#endif
