#ifndef ZOIDS_COLLISION_H
#define ZOIDS_COLLISION_H

#include "joint.h"

typedef struct CoObj {
    HSD_JObj *joint, *base_joint;
    f32 minimum_scale, maximum_scale;
    s32 object_contact, ground_contact;
    u8 reserved_18[4];
    u16 ground_polygon_index;
    u8 reserved_1E[0xE];
    f32 inverse[3][4];
    u32 field_5C;
    s32 health, maximum_health;
    f32 position[3];
    f32 base_position[3];
    f32 movement[3];
    f32 ground_y;
} CoObj;
typedef char CoObjSizeCheck[sizeof(CoObj) == 0x90 ? 1 : -1];

typedef struct CoDamage {
    u32 flags;
    s32 health, maximum_health;
} CoDamage;

typedef char CoDamageSizeCheck[sizeof(CoDamage) == 12 ? 1 : -1];

#endif
