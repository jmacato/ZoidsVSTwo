#ifndef ZOIDS_OBJECT_H
#define ZOIDS_OBJECT_H

#include "class.h"

typedef struct HSD_Obj {
    HSD_Class parent;
    u16 reference_count;
    u16 individual_reference_count;
} HSD_Obj;

extern HSD_ClassInfo hsdObj;

#endif
