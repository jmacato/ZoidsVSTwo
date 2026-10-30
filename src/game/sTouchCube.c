#include "types.h"
#include "jobj.h"

extern f32 sLenCube(HSD_JObj *jobj);
extern f32 sLenJobj(HSD_JObj *first, HSD_JObj *second);

s32 sTouchCube(HSD_JObj *a, HSD_JObj *b)
{
    f32 a_len;
    f32 b_len;
    f32 dist;

    if (a == 0) {
        return 0;
    }
    if (b == 0) {
        return 0;
    }
    a_len = sLenCube(a);
    b_len = sLenCube(b);
    dist = sLenJobj(a, b);
    return dist <= a_len + b_len;
}
