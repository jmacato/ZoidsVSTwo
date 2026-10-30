#include "aobj.h"

extern struct HSD_ObjAllocData alloc_data;
extern void *HSD_ObjAlloc(struct HSD_ObjAllocData *data);
extern void __assert(const char *file, u32 line, const char *condition);
extern void *memset(void *memory, s32 value, u32 size);

HSD_AObj *HSD_AObjAlloc(void)
{
    HSD_AObj *animation = HSD_ObjAlloc(&alloc_data);

    if (animation == 0) {
        __assert("aobj.c", 489, "new");
    }
    memset(animation, 0, sizeof(*animation));
    animation->flags = AOBJ_NO_ANIM;
    animation->rate = 1.0f;
    return animation;
}
