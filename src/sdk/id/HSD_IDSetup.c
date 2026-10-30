/* Based on https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/id.c.
 */
#include "id.h"

void HSD_IDSetup(void)
{
    memset(&default_table, 0, sizeof(HSD_IDTable));
}
