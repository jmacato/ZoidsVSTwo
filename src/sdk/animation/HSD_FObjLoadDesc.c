#include "fobj.h"

extern HSD_FObj *HSD_FObjAlloc(void);

HSD_FObj *HSD_FObjLoadDesc(HSD_FObjDesc *description)
{
    HSD_FObj *function;

    if (description != 0) {
        function = HSD_FObjAlloc();
        function->next = HSD_FObjLoadDesc(description->next);
        function->start_frame = description->start_frame;
        function->object_type = description->object_type;
        function->value_format = description->value_format;
        function->slope_format = description->slope_format;
        function->data_head = description->data;
        function->length = description->length;
        function->flags = 0;
        return function;
    }
    return 0;
}
