#include "ground_collision.h"
extern u32 *grdcD2TBL;
u16 *sColPosTBL(Vec3 *point)
{
    s32 index;

    index = sColPos(point);
    if (index >= grdcDivX * grdcDivZ) {
        return 0;
    }
    return (u16 *)grdcD2TBL[index];
}
