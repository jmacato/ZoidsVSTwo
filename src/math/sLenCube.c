#pragma readonly_strings on
#include "joint.h"
#include "math_support.h"

extern void __assert(const char *file, u32 line, const char *condition);

static inline f32 getsx(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 899, "jobj");
    return jobj->scale.x;
}
static inline f32 getsy(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 911, "jobj");
    return jobj->scale.y;
}
static inline f32 getsz(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 923, "jobj");
    return jobj->scale.z;
}

f32 sLenCube(HSD_JObj *jobj)
{
    f32 x;
    f32 y;
    f32 z;

    if (jobj == 0) {
        return 0.0f;
    }
    x = getsx(jobj) / 2.0f;
    y = getsy(jobj) / 2.0f;
    z = getsz(jobj) / 2.0f;
    return square_root(x * x + y * y + z * z);
}
