#ifndef ZOIDS_GROUND_OBJECT_H
#define ZOIDS_GROUND_OBJECT_H

#include "joint.h"

/* values of GroundObject.animation, also the animation index in stage/map_obj/map_objNN.dat */
enum GroundObjectAnim {
    GROUND_OBJECT_ANIM_INTACT = 0,
    GROUND_OBJECT_ANIM_BREAKING = 1,
    GROUND_OBJECT_ANIM_BROKEN = 2
};

/* placed copy of a stage/map_obj/map_objNN.dat model, created by GroundObjLoadSub2 */
typedef struct GroundObject {
    HSD_JObj *source;
    f32 position[3];
    f32 matrix[3][4];
    f32 inverse[3][4];
    s32 archive_slot;
    HSD_JObj *joint;
    s32 animation;
    s32 health;
} GroundObject;

typedef char GroundObjectSizeCheck[sizeof(GroundObject) == 0x80 ? 1 : -1];

extern void grdObjSetAnim(GroundObject *object, s32 animation);

#endif
