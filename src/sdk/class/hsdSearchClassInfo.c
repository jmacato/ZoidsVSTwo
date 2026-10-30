#include "class_memory.h"

HSD_ClassInfo *hsdSearchClassInfo(const char *class_name)
{
    if (info_hash != 0) {
        return HSD_HashSearch(info_hash, (void *)class_name, 0);
    }
    return 0;
}
