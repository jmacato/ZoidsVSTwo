/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/class.c. */
#include "class.h"

extern HSD_Class *_hsdClassAlloc(HSD_ClassInfo *info);
extern s32 _hsdClassInit(HSD_Class *object);
extern void _hsdClassRelease(HSD_Class *object);
extern void _hsdClassDestroy(HSD_Class *object);
extern void _hsdClassAmnesia(HSD_ClassInfo *info);

void _hsdClassInfoInit(void);
HSD_ClassInfo hsdClass = {{_hsdClassInfoInit}};

/* Keep the original class diagnostics before the shared class-name strings. */
#pragma push
#pragma force_active on
static char object_size_check[] = "class_info->head.obj_size >= parent_info->head.obj_size";
static char info_size_check[] = "class_info->head.info_size >= parent_info->head.info_size";
static char entry_missing[] = "entry %d <null>\n";
static char entry_range_missing[] = "entry %d - %d <null>\n";
static char entry_header[] = "entry %d(%d)";
static char entry_counts[] = "  nb_alloc %d nb_free %d\n";
static char index_check[] = "idx >= 0";
#pragma pop

void _hsdClassInfoInit(void)
{
    hsdInitClassInfo(&hsdClass, 0, "sysdolphin_base_library", "hsd_class",
                    sizeof(HSD_ClassInfo), sizeof(HSD_Class));
    hsdClass.allocate = _hsdClassAlloc;
    hsdClass.initialize = _hsdClassInit;
    hsdClass.release = _hsdClassRelease;
    hsdClass.destroy = _hsdClassDestroy;
    hsdClass.forget = _hsdClassAmnesia;
}
