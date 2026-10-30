#include "fobj.h"

void FObjLaunchKeyData(HSD_FObj *function)
{
    if ((function->flags & 0x40) != 0) {
        function->interpolation_operation = function->operation;
        function->flags &= ~0x40;
        function->flags |= 0x80;
        function->value_0 = function->value_1;
    }
}
