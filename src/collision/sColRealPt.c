#include "ground_collision.h"

extern f32 sLiner(f32 a, f32 b, f32 t);
extern f32 sColRealPt01(Vec3 *start, Vec3 *end, u16 *indices, GroundMask *mask);

f32 sColRealPt(Vec3 *start, Vec3 *end, GroundMask *mask)
{
    f32 sample[3];
    u16 *tables[24];
    GroundMask mask_copy;
    u16 *table;
    f32 result;
    f32 time;
    s32 t;
    s32 i;
    s32 j;

    for (t = 0; t < 22; t++) {
        tables[t] = (void *)0;
    }
    for (t = 0; t < 20; t++) {
        time = t / 19.0f;
        for (i = 0; i < 3; i++) {
            sample[i] = sLiner(((f32 *)end)[i], ((f32 *)start)[i], time);
        }
        table = sColPosTBL((Vec3 *)sample);
        if (table != (void *)0) {
            for (j = 0; j < 20; j++) {
                if (table == tables[j] || tables[j] == (void *)0)
                    break;
            }
            if (j == 20)
                break;
            if (tables[j] != table)
                tables[j] = table;
        }
    }
    for (j = 0; j < 20; j++) {
        table = tables[j];
        if (table == (void *)0)
            break;
        mask_copy = *mask;
        result = sColRealPt01(start, end, table, &mask_copy);
        if (result != GROUND_NO_HIT)
            return result;
    }
    return GROUND_NO_HIT;
}
