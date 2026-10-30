#ifndef ZOIDS_OS_CACHE_H
#define ZOIDS_OS_CACHE_H
#include "types.h"
extern void LCEnable(void);
extern u32 LCStoreData(void *destination, void *source, u32 length);
extern void L2GlobalInvalidate(void);
extern void __OSCacheInit(void);
#endif
