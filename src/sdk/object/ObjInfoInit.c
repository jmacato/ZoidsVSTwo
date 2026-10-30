/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/object.c. */
#include "object.h"

extern HSD_ClassInfo hsdClass;

void ObjInfoInit(void)
{
    hsdInitClassInfo(&hsdObj, &hsdClass, "sysdolphin_base_library", "hsd_obj",
                     sizeof(HSD_ClassInfo), sizeof(HSD_Obj));
}
