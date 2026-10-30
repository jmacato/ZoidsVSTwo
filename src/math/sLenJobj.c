#include "joint.h"

extern f32 sLenMtx(f32 first[3][4], f32 second[3][4]);

f32 sLenJobj(HSD_JObj *first, HSD_JObj *second)
{
    if (first == (void *)0) {
        return 0.0f;
    }
    if (second == (void *)0) {
        return 0.0f;
    }
    return sLenMtx(first->matrix, second->matrix);
}
