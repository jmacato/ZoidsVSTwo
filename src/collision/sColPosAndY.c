#include "ground_collision.h"

extern s32 offShadow;

static inline f32 absf(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

f32 sColPosAndY(Vec3 *point, u16 *selector, GroundMask *mask)
{
    u16 index;
    u16 *table;
    GroundMask *material;
    u16 best_index;
    u16 material_index;
    f32 best_distance;
    f32 best_y;
    s32 normal_index;
    f32 y;
    f32 absf_value;
    f32 diff;

    if (selector != (void *)0)
        *selector = GROUND_POLYGON_NONE;
    if (grdcCol == 0)
        return 0.0f;
    table = sColPosTBL(point);
    if (table == (void *)0)
        return point->y;
    best_y = point->y;
    best_index = GROUND_POLYGON_NONE;
    while (1) {
        index = *table;
        if (index == GROUND_POLYGON_NONE)
            break;
        normal_index = grdcPolygon[index].normal_index;
        if (grdcNormal[normal_index].y < 0.0f)
            goto next;
        material_index = grdcPolygon[index].material;
        material = &grdcMat[material_index];
        if (!(material->bits[0] & mask->bits[0]) && !(material->bits[1] & mask->bits[1]) &&
            !(material->bits[2] & mask->bits[2]))
            goto next;
        if (sColPosPolygonON(point, index) == 0)
            goto next;
        y = sColGetPolyY(index, point);
        diff = y - point->y;
        absf_value = absf(diff);
        if (best_distance > absf_value || best_index == GROUND_POLYGON_NONE) {
            best_y = y;
            best_index = index;
            best_distance = absf_value;
        }
    next:
        table++;
    }
    if (selector != (void *)0)
        *selector = best_index;
    return best_y;
}
