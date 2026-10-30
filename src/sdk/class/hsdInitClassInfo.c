#include "class_memory.h"

#define HSD_CLASS_INFO_INITIALIZED 1

void hsdInitClassInfo(HSD_ClassInfo *class_info, HSD_ClassInfo *parent_info,
                      char *base_class_library, char *type, s32 info_size,
                      s32 class_size)
{
    class_info->head.flags = HSD_CLASS_INFO_INITIALIZED;
    class_info->head.library_name = base_class_library;
    class_info->head.class_name = type;
    class_info->head.object_size = (s16)class_size;
    class_info->head.info_size = (s16)info_size;
    class_info->head.parent = parent_info;
    class_info->head.child = 0;
    class_info->head.next = 0;
    class_info->head.live_count = 0;
    class_info->head.peak_count = 0;

    if (parent_info != 0) {
        if ((parent_info->head.flags & HSD_CLASS_INFO_INITIALIZED) == 0) {
            (*parent_info->head.initialize_info)();
        }
        if (class_info->head.object_size < parent_info->head.object_size) {
            __assert("class.c", 95,
                     "class_info->head.obj_size >= parent_info->head.obj_size");
        }
        if (class_info->head.info_size < parent_info->head.info_size) {
            __assert("class.c", 96,
                     "class_info->head.info_size >= parent_info->head.info_size");
        }
        memcpy(&class_info->allocate, &parent_info->allocate,
               parent_info->head.info_size - sizeof(HSD_ClassInfoHead));
        class_info->head.next = parent_info->head.child;
        parent_info->head.child = class_info;
    }
}
