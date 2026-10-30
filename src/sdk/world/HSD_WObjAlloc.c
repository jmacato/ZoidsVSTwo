/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/wobj.c. */
#include "wobj.h"

extern void __assert(const char *file, u32 line, const char *condition);
extern HSD_ClassInfo *default_class;
extern HSD_WObjInfo hsdWObj;

HSD_WObj *HSD_WObjAlloc(void)
{
    HSD_WObj *world_object = hsdNew(default_class ? default_class : &hsdWObj.parent);
    if (world_object == 0) {
        __assert("wobj.c", 599, "wobj");
    }
    return world_object;
}
