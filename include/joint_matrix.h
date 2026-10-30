#ifndef ZOIDS_JOINT_MATRIX_H
#define ZOIDS_JOINT_MATRIX_H

#include "joint.h"

/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/jobj.h. */
extern void HSD_JObjSetupMatrixSub(HSD_JObj *joint);
extern void PSMTXMultVec(f32 matrix[3][4], Vec3 *source, Vec3 *destination);
extern void __assert(const char *file, u32 line, const char *condition);

static inline s32 matrix_is_dirty(HSD_JObj *joint)
{
    s32 result;
    if (joint == 0) {
        __assert("jobj.h", 605, "jobj");
    }
    result = 0;
    if (!(joint->flags & 0x00800000) && (joint->flags & 0x40)) {
        result = 1;
    }
    return result;
}

static inline void setup_matrix(HSD_JObj *joint)
{
    if (joint == 0 || !matrix_is_dirty(joint)) {
        return;
    }
    HSD_JObjSetupMatrixSub(joint);
}

#endif
