#include "weapon.h"

extern void sWeaponPurge(s32 index);

void sWeaponEpilogue(void)
{
    s32 index;

    for (index = 0; index < 100; index++) {
        sWeaponPurge(index);
    }
}
