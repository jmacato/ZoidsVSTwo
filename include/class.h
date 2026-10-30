#ifndef ZOIDS_CLASS_H
#define ZOIDS_CLASS_H

#include "types.h"

/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/class.h. */
typedef struct HSD_Class {
    struct HSD_ClassInfo *class_info;
} HSD_Class;

typedef struct HSD_ClassInfoHead {
    void (*initialize_info)(void);
    u32 flags;
    const char *library_name;
    const char *class_name;
    s16 object_size;
    s16 info_size;
    struct HSD_ClassInfo *parent;
    struct HSD_ClassInfo *next;
    struct HSD_ClassInfo *child;
    u32 live_count;
    u32 peak_count;
} HSD_ClassInfoHead;

typedef struct HSD_ClassInfo {
    HSD_ClassInfoHead head;
    HSD_Class *(*allocate)(struct HSD_ClassInfo *info);
    s32 (*initialize)(HSD_Class *object);
    void (*release)(HSD_Class *object);
    void (*destroy)(HSD_Class *object);
    void (*forget)(struct HSD_ClassInfo *info);
} HSD_ClassInfo;

extern void *hsdNew(HSD_ClassInfo *info);
extern HSD_ClassInfo *hsdSearchClassInfo(const char *name);
extern void hsdInitClassInfo(HSD_ClassInfo *info, HSD_ClassInfo *parent,
                              char *library, char *name,
                              s32 info_size, s32 object_size);

#endif
