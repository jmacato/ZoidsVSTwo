/* Based on https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/id.c.
 */
#include "id.h"

void HSD_IDInitAllocData(void)
{
    HSD_ObjAllocInit(&alloc_data, sizeof(IDEntry), 4);
}
