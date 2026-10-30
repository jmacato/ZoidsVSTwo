#include "initialize.h"

extern int need_invalidate;

void _HSD_NeedCacheInvalidate(int flags)
{
    need_invalidate |= flags;
}
